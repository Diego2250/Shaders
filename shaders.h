#pragma once
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "uniform.h"
#include "fragment.h"
#include "noise.h"
#include "print.h"

Vertex vertexShader(const Vertex& vertex, const Uniform& uniforms) {
    // Apply transformations to the input vertex using the matrices from the uniforms
    glm::vec4 clipSpaceVertex = uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex.position, 1.0f);

    // Perspective divide
    glm::vec3 ndcVertex = glm::vec3(clipSpaceVertex) / clipSpaceVertex.w;

    // Apply the viewport transform
    glm::vec4 screenVertex = uniforms.viewport * glm::vec4(ndcVertex, 1.0f);

    // Transform the normal
    glm::vec3 transformedNormal = glm::mat3(uniforms.model) * vertex.normal;
    transformedNormal = glm::normalize(transformedNormal);

    glm::vec3 transformedWorldPosition = glm::vec3(uniforms.model * glm::vec4(vertex.position, 1.0f));

    // Return the transformed vertex as a vec3
    return Vertex{
            glm::vec3(screenVertex),
            transformedNormal,
            vertex.tex,
            transformedWorldPosition,
            vertex.position
    };
}

// Shader para el planeta rocoso
Fragment planetaRocoso(Fragment& fragment) {
    Color color;

    // Definir los colores del planeta
    glm::vec3 rockColor = glm::vec3(0.7f, 0.7f, 0.7f);
    glm::vec3 surfaceColor = glm::vec3(0.4f, 0.4f, 0.4f);

    // Obtener las coordenadas UV
    glm::vec2 uv = glm::vec2(fragment.originalPos.x, fragment.originalPos.y);

    // Generar ruido para simular la textura del planeta
    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    float offsetX = 5000.0f;
    float offsetY = 8000.0f;
    float scale = 500.0f;

    float noiseValue = noiseGenerator.GetNoise((uv.x + offsetX) * scale, (uv.y + offsetY) * scale);
    noiseValue = (noiseValue + 1.0f) * 0.5f; // Mapear [-1, 1] a [0, 1]

    // Combinar los colores del planeta con el ruido
    glm::vec3 finalColor = glm::mix(rockColor, surfaceColor, noiseValue);

    color = Color(finalColor.x, finalColor.y, finalColor.z);

    fragment.color = color * fragment.intensity;

    return fragment;
}


// Shader para el planeta gaseoso
Fragment giganteGaseoso(Fragment& fragment) {
    Color color;

    // Definir los colores del gigante gaseoso
    glm::vec3 mainColor = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 cloudColor = glm::vec3(0.8f, 0.8f, 0.8f);

    // Obtener las coordenadas UV
    glm::vec2 uv = glm::vec2(fragment.originalPos.x * 2.0 - 1.0, fragment.originalPos.y * 2.0 - 1.0);

    // Frecuencia y amplitud de las ondas en el gigante gaseoso
    float frequency = 10.0; // Ajustar la frecuencia de las líneas
    float amplitude = 0.2;  // Ajustar la amplitud de las líneas

    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    float offsetX = 10000.0f;
    float offsetY = 10000.0f;
    float scale = 900.0f;

    // Generar el valor de ruido
    float noiseValue = noiseGenerator.GetNoise((uv.x + offsetX) * scale, (uv.y + offsetY) * scale);
    noiseValue = (noiseValue + 1.0f) * 0.5f; // Mapear [-1, 1] a [0, 1]

    // Calcular el valor sinusoide para crear líneas
    float sinValue = glm::sin(uv.y * frequency) * amplitude;

    // Combinar el color base con las líneas sinusoide
    cloudColor = mainColor + glm::vec3(sinValue);

    color = Color(cloudColor.x, cloudColor.y, cloudColor.z);

    fragment.color = color * fragment.intensity;

    return fragment;
}

// Shader para la estrella
Fragment estrellaCreativa(Fragment& fragment) {
    Color color;

    // Genera colores aleatorios para la estrella
    float r = static_cast<float>(rand()) / RAND_MAX;
    float g = static_cast<float>(rand()) / RAND_MAX;
    float b = static_cast<float>(rand()) / RAND_MAX;

    // Añade un efecto de destello aleatorio
    float intensity = 1.0 + (rand() % 5) / 10.0;

    // Simula cambios de color sutiles con el tiempo
    float time = static_cast<float>(rand()) / RAND_MAX;
    r += sin(time);
    g += cos(time);
    b += sin(time * 0.5);

    // Añade un efecto de parpadeo aleatorio
    intensity *= 0.8 + (rand() % 4) / 10.0;

    // Añade un efecto de brillo aleatorio
    intensity *= 1.0 + (rand() % 2) / 10.0;

    // Limita los valores de color y intensidad
    r = std::min(1.0f, r);
    g = std::min(1.0f, g);
    b = std::min(1.0f, b);
    intensity = std::max(1.0f, intensity);

    // Asigna el color y la intensidad a la fragment
    color = Color(r, g, b);

    // Aumenta la intensidad para simular el brillo de la estrella
    fragment.color = color * intensity;

    return fragment;
}





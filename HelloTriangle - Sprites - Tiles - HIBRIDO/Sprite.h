#pragma once

//GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Sprite
{
    public:
    Sprite() {} //Método construtor padrão
    ~Sprite(); //Método destrutor

    void inicializar(GLuint texID, int nAnimations = 1, int nFrames = 1, glm::vec3 pos=glm::vec3(0), glm::vec3 escala = glm::vec3(1), float angulo = 0.0, glm::vec3 cor = glm::vec3(1.0, 0.0, 1.0));
    void atualizar();
    void desenhar();
    void finalizar();

    void moveLeft();
    void moveRight();

    void moveItem();

    inline void setShader(Shader *shader) { this->shader = shader; }
    inline void setShaderDebug(Shader *shader) { this->shaderDebug = shader; }
    inline void setAngulo(float angulo) { this->angulo = angulo; }
    void setPos(glm::vec2 pos) { this->pos.x = pos.x; this->pos.y = pos.y; }
    
    inline glm::vec3 getPMin() { return this->pmin;}
    inline glm::vec3 getPMax() { return this->pmax;}
    
    void setColisao() { colidiu = true; }

    void getAABB();
   
    protected:

    GLuint VAO; //Identificador do buffer de geometria VAO
    GLuint texID; //Identificador da textura

    //Transformações na geometria
    glm::vec3 pos, escala;
    float angulo;
    float vel;

    //Animação por sprite
    int nAnimations, nFrames, iAnimation, iFrame;
    glm::vec2 offsetTex; //ds e dt 

    float lastTime; //pra fazer a sincronização do FPS -- TODO
    Shader *shader;
    Shader *shaderDebug;

    glm::vec3 pmin, pmax;
    bool colidiu;

};
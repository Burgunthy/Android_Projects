//
// Created by thrha on 2021-09-10.
//

#ifndef SHADER_TEXTURE_BASE_H
#define SHADER_TEXTURE_BASE_H

#include <jni.h>
#include <android/log.h>

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

#include <opencv2/opencv.hpp>

// open GL libs
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/closest_point.hpp>

#define  LOG_TAG    "TAG_libgl2jni"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

using namespace cv;

int w, h;

std::vector<Mat> img;
std::vector<Mat> depth;

glm::mat4 mPMatrix = glm::mat4(1.0f);
glm::mat4 mMMatrix = glm::mat4(1.0f);

// GL
GLuint mProgram;
GLuint maPositionHandle, maTextureHandle, muPMatrixHandle, muMMatrixHandle;

GLuint mProgram2;
GLuint maPositionHandle2, maTextureHandle2, muPMatrixHandle2, muMMatrixHandle2;

GLuint colorTexture = 0;
GLuint depthTexture = 0;

GLuint loadShader(GLenum shaderType, const char* pSource) {

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &pSource, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        LOGD("ERROR::SHADER::COMPILATION_FAILED %s",infoLog);
    }

    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {

    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);

    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    int success;
    char infoLog[512];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        LOGD("ERROR::SHADER::PROGRAM::LINKING_FAILED %s",infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

const char* gVertexShader = "uniform mat4 uPMatrix;\n"
                            "uniform mat4 uMMatrix;\n"
                            "attribute vec4 aPosition;\n"
                            "attribute vec2 aTextureCoord;\n"
                            "varying vec2 vTextureCoord;\n"
                            "\n"
                            "void main () \n"
                            "{\n"
                            "    gl_Position = uPMatrix * ( uMMatrix * aPosition );\n"
                            "    vTextureCoord = aTextureCoord;\n"
                            "}\n";

const char* gFragmentShader = "precision mediump float;\n"
                              "varying vec2 vTextureCoord;\n"
                              "uniform sampler2D sTexture;\n"
                              "\n"
                              "void main()\n"
                              "{\n"
                              "    gl_FragColor = texture2D(sTexture, vTextureCoord); // Fragment Shader?????? ??????????????? ?????????  ?????? ????????????\n"
                              "}\n";

const char* gVertexShader2 = "uniform mat4 uPMatrix;\n"
                            "uniform mat4 uMMatrix;\n"
                            "attribute vec4 aPosition;\n"
                            "\n"
                            "void main () \n"
                            "{\n"
                            "    gl_Position = uPMatrix * ( uMMatrix * aPosition );\n"
                            "}\n";

const char* gFragmentShader2 = "precision mediump float;\n"
                              "\n"
                              "void main()\n"
                              "{\n"
                              "    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Fragment Shader?????? ??????????????? ?????????  ?????? ????????????\n"
                              "}\n";

float vertices_plane[] = {
        0.0f,		0.0f,		0.0f,
        640.0f,		0.0f,		0.0f,
        640.0f,		480.0f,		0.0f,
        0.0f,		480.0f,		0.0f,
};
short index_plane[] = {
        0, 1, 2,
        0, 2, 3
};
float texture_plane[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
};
float vertices[] = {
        // ??????
        -1.0f, -1.0f, 1.0f, // ?????? ?????? ??????
        1.0f, -1.0f, 1.0f,  // ????????? ??????
        -1.0f, 1.0f, 1.0f,  // ?????? ???
        1.0f, 1.0f, 1.0f,   // ????????? ???

        // ????????? ???
        1.0f, -1.0f, 1.0f,  // ?????? ??????
        1.0f, -1.0f, -1.0f, // ????????? ??????
        1.0f, 1.0f, 1.0f,   // ?????? ???
        1.0f, 1.0f, -1.0f,  // ????????? ???

        // ??????
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        // ?????????
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,

        // ????????? ???
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,

        // ?????????
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
};

short indexes[] = {
        0,1,3, 0,3,2,           //????????? ???????????? 2?????? 3??????
        4,5,7, 4,7,6,           //????????????
        8,9,11, 8,11,10,        //...
        12,13,15, 12,15,14,
        16,17,19, 16,19,18,
        20,21,23, 20,23,22,
};

float texture[] = {
        //6?????? ?????? ????????? ????????? ?????? 24??????  ????????????
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
};

#endif //SHADER_TEXTURE_BASE_H

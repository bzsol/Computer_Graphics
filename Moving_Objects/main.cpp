
#include "gl_utils.h"   // utility functions discussed in earlier tutorials
#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define GL_LOG_FILE "gl.log"

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 640;
int g_gl_height = 640;
GLFWwindow* g_window = NULL;

int main() {
    restart_gl_log();
    // all the GLFW and GLEW start-up code is moved to here in gl_utils.cpp
    start_gl();
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS);    // depth-testing interprets a smaller value as "closer"

    /* OTHER STUFF GOES HERE NEXT */
    GLfloat points[] = { 0.0f, 0.25f, 0.0f,
                         0.25f, -0.25f, 0.0f,
                        -0.25f, -0.25f, 0.0f };

    GLfloat points2[] = { 0.75f, 0.25f, 0.0f,
                          0.99f, -0.25f, 0.0f,
                          0.5f, -0.25f, 0.0f };

    GLfloat colours[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat colours2[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };

    GLuint VBO[4];
    GLuint VAO[2];

    glGenBuffers(4, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points2), points2, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colours2), colours2, GL_STATIC_DRAW);

    glGenVertexArrays(2, VAO);
 
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    char vertex_shader[512 * 256];
    char fragment_shader[512 * 256];

    parse_file_into_str("test_vs.glsl", vertex_shader, 512 * 256);
    parse_file_into_str("test_fs.glsl", fragment_shader, 512 * 256);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* p = (const GLchar*)vertex_shader;
    glShaderSource(vs, 1, &p, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    p = (const GLchar*)fragment_shader;
    glShaderSource(fs, 1, &p, NULL);
    glCompileShader(fs);

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    char vertex_shader2[512 * 256];
    char fragment_shader2[512 * 256];

    parse_file_into_str("test_vs2.glsl", vertex_shader2, 512 * 256);
    parse_file_into_str("test_fs2.glsl", fragment_shader2, 512 * 256);

    GLuint vs2 = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* p2 = (const GLchar*)vertex_shader2;
    glShaderSource(vs2, 1, &p2, NULL);
    glCompileShader(vs2);

    GLuint fs2 = glCreateShader(GL_FRAGMENT_SHADER);
    p2 = (const GLchar*)fragment_shader2;
    glShaderSource(fs2, 1, &p2, NULL);
    glCompileShader(fs2);

    GLuint shader_programme2 = glCreateProgram();
    glAttachShader(shader_programme2, fs2);
    glAttachShader(shader_programme2, vs2);
    glLinkProgram(shader_programme2);

    GLfloat matrix[] = {
      1.0f, 0.0f, 0.0f, 0.0f, // first column
      0.0f, 1.0f, 0.0f, 0.0f, // second column
      0.0f, 0.0f, 1.0f, 0.0f, // third column
      0.0f, 0.0f, 0.0f, 1.0f  // fourth column
    };

    GLfloat matrix2[] = {
     1.0f, 0.0f, 0.0f, 0.0f, // first column
     0.0f, 1.0f, 0.0f, 0.0f, // second column
     0.0f, 0.0f, 1.0f, 0.0f, // third column
     0.0f, 0.0f, 0.0f, 1.0f  // fourth column
    };

    int matrix_location = glGetUniformLocation(shader_programme, "matrix");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);

    int matrix_location2 = glGetUniformLocation(shader_programme2, "matrix2");
    glUseProgram(shader_programme2);
    glUniformMatrix4fv(matrix_location2, 1, GL_FALSE, matrix2);

    glEnable(GL_CULL_FACE); // cull face
    glCullFace(GL_BACK);    // cull back face
    glFrontFace(GL_CW);     // GL_CCW for counter clock-wise

    float speed = 1.0f; // move at 1 unit per second
    float last_position = 0.0f;

    float speed2 = 1.0f; // move at 1 unit per second
    float last_position2 = 0.0f;

    while (!glfwWindowShouldClose(g_window)) {
        // add a timer for doing animation
        static double previous_seconds = glfwGetTime();
        double current_seconds = glfwGetTime();
        double elapsed_seconds = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        _update_fps_counter(g_window);
        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, g_gl_width, g_gl_height);

        //
        // Note: this call is not necessary, but I like to do it anyway before any
        // time that I call glDrawArrays() so I never use the wrong shader programme
        glUseProgram(shader_programme);
        glBindVertexArray(VAO[0]);


        // update the matrix
        // - you could simplify this by just using sin(current_seconds)
        matrix[13] = elapsed_seconds * speed + last_position;
        last_position = matrix[13];
        if (fabs(last_position) > 0.75f) { speed = -speed; }
        //
        // Note: this call is related to the most recently 'used' shader programme
        glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);

        //
        // Note: this call is not necessary, but I like to do it anyway before any
        // time that I call glDrawArrays() so I never use the wrong vertex data

        // draw points 0-3 from the currently bound VAO with current in-use shader
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glUseProgram(shader_programme2);

        matrix2[12] = elapsed_seconds * speed2 + last_position2;
        last_position2 = matrix2[12];
        if (last_position2 > 0.0f || last_position2 < -1.5f) { speed2 = -speed2; }
        //
        // Note: this call is related to the most recently 'used' shader programme
        glUniformMatrix4fv(matrix_location2, 1, GL_FALSE, matrix2);

        //
        // Note: this call is not necessary, but I like to do it anyway before any
        // time that I call glDrawArrays() so I never use the wrong vertex data
        glBindVertexArray(VAO[1]);
        // draw points 0-3 from the currently bound VAO with current in-use shader
        glDrawArrays(GL_TRIANGLES, 0, 3);


        // update other events like input handling
        glfwPollEvents();
        if (GLFW_PRESS == glfwGetKey(g_window, GLFW_KEY_ESCAPE)) { glfwSetWindowShouldClose(g_window, 1); }
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(g_window);
    }

    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}

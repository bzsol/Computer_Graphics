/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Dr Anton Gerdelan, Trinity College Dublin, Ireland.                          |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| Matrices and Vectors                                                         |
| Note: code discussed in previous tutorials is moved into gl_utils file       |
| On Apple don't forget to uncomment the version number hint in start_gl()     |
\******************************************************************************/
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
#define	 numVBOs			4
#define	 numVAOs			2

#define PI 3.14159265359

// keep track of window size for things like the viewport and the mouse cursor
int g_gl_width = 640;
int g_gl_height = 480;
GLFWwindow* g_window = NULL;



int main() {

    GLfloat x0 = 0.0f;
    GLfloat y0 = 0.0f;
    GLfloat r = 0.2f;

    GLfloat circle[315]; // 303 / 101 -> 3 pont -> x,y,z
    GLfloat circle_color[315];

    GLfloat circle2[315]; // 303 / 101 -> 3 pont -> x,y,z
    GLfloat circle_color2[315];

    GLfloat t;
    GLint i;

    GLfloat step = (2 * PI) / 100;

    restart_gl_log();
    // all the GLFW and GLEW start-up code is moved to here in gl_utils.cpp
    start_gl();
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS);    // depth-testing interprets a smaller value as "closer"

    /* OTHER STUFF GOES HERE NEXT */
    GLfloat points[] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f };
    GLfloat points2[] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f };

    GLfloat colours[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat colours2[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };

    GLuint VAO[numVAOs];
    GLuint VBO[numVBOs];

    glGenBuffers(numVBOs, VBO);
    glGenVertexArrays(numVAOs, VAO);

    ////// GENERATING CIRCLE POINTS ///////
    circle[0] = 0.0f;
    circle[1] = 0.0f;
    circle[2] = 0.0f;
    circle_color[0] = 0.0f;
    circle_color[1] = 0.0f;
    circle_color[2] = 1.0f;
    for (i = 1; i <= 104; i++) {
        t = i * step;
        circle[i * 3] = x0 + r * cos(t);
        circle[i * 3 + 1] = y0 + r * sin(t);
        circle[i * 3 + 2] = 0.0f;
        circle_color[i * 3] = 0.0f;
        circle_color[i * 3 + 1] = 0.0f;
        circle_color[i * 3 + 2] = 1.0f;
    }

    ////// GENERATING CIRCLE POINTS ///////
    circle2[0] = 0.0f;
    circle2[1] = 0.0f;
    circle2[2] = 0.0f;
    circle_color2[0] = 1.0f;
    circle_color2[1] = 0.0f;
    circle_color2[2] = 0.0f;
    for (i = 1; i <= 104; i++) {
        t = i * step;
        circle2[i * 3] = x0 + r * cos(t);
        circle2[i * 3 + 1] = y0 + r * sin(t);
        circle2[i * 3 + 2] = 0.0f;
        circle_color2[i * 3] = 1.0f;
        circle_color2[i * 3 + 1] = 0.0f;
        circle_color2[i * 3 + 2] = 0.0f;
    }

    // Coordinates
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glBindVertexArray(VAO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);


    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle_color), circle_color, GL_STATIC_DRAW);
    glBindVertexArray(VAO[0]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    // Coordinates
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle2), circle2, GL_STATIC_DRAW);
    glBindVertexArray(VAO[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);


    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle_color2), circle_color2, GL_STATIC_DRAW);
    glBindVertexArray(VAO[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    char vertex_shader[512 * 256];
    char fragment_shader[512 * 256];
    char vertex_shader2[512 * 256];
    char fragment_shader2[512 * 256];
    parse_file_into_str("test_vs.glsl", vertex_shader, 512 * 256);
    parse_file_into_str("test_fs.glsl", fragment_shader, 512 * 256);
    parse_file_into_str("test_vs2.glsl", vertex_shader2, 512 * 256);
    parse_file_into_str("test_fs2.glsl", fragment_shader2, 512 * 256);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* p = (const GLchar*)vertex_shader;
    glShaderSource(vs, 1, &p, NULL);
    glCompileShader(vs);

    // check for compile errors
    int params = -1;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params) {
        fprintf(stderr, "ERROR: GL shader index %i did not compile\n", vs);
        print_shader_info_log(vs);
        return 1; // or exit or something
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    p = (const GLchar*)fragment_shader;
    glShaderSource(fs, 1, &p, NULL);
    glCompileShader(fs);

    // check for compile errors
    glGetShaderiv(fs, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params) {
        fprintf(stderr, "ERROR: GL shader index %i did not compile\n", fs);
        print_shader_info_log(fs);
        return 1; // or exit or something
    }
    ////////////////////////////////////////////////////////////////////////////

    GLuint vs2 = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* p2 = (const GLchar*)vertex_shader2;
    glShaderSource(vs2, 1, &p2, NULL);
    glCompileShader(vs2);

    // check for compile errors
    int params2 = -1;
    glGetShaderiv(vs2, GL_COMPILE_STATUS, &params2);
    if (GL_TRUE != params2) {
        fprintf(stderr, "ERROR: GL shader index %i did not compile\n", vs2);
        print_shader_info_log(vs2);
        return 1; // or exit or something
    }

    GLuint fs2 = glCreateShader(GL_FRAGMENT_SHADER);
    p = (const GLchar*)fragment_shader2;
    glShaderSource(fs2, 1, &p, NULL);
    glCompileShader(fs2);

    // check for compile errors
    glGetShaderiv(fs2, GL_COMPILE_STATUS, &params2);
    if (GL_TRUE != params2) {
        fprintf(stderr, "ERROR: GL shader index %i did not compile\n", fs2);
        print_shader_info_log(fs2);
        return 1; // or exit or something
    }

   ////////////////////////////////////////////////////////////////////////////

    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    GLuint shader_programme2 = glCreateProgram();
    glAttachShader(shader_programme2, fs2);
    glAttachShader(shader_programme2, vs2);
    glLinkProgram(shader_programme2);

    glGetProgramiv(shader_programme, GL_LINK_STATUS, &params);
    if (GL_TRUE != params) {
        fprintf(stderr, "ERROR: could not link shader programme GL index %i\n", shader_programme);
        print_programme_info_log(shader_programme);
        return false;
    }

    glGetProgramiv(shader_programme2, GL_LINK_STATUS, &params2);
    if (GL_TRUE != params2) {
        fprintf(stderr, "ERROR: could not link shader programme GL index %i\n", shader_programme2);
        print_programme_info_log(shader_programme2);
        return false;
    }

    GLfloat matrix[] = {
      1.0f, 0.0f, 0.0f, 0.0f, // first column
      0.0f, 1.0f, 0.0f, 0.0f, // second column
      0.0f, 0.0f, 1.0f, 0.0f, // third column
      0.0f, 0.0f, 0.0f, 1.0f  // fourth column x y z
    };

    GLfloat matrix2[] = {
    1.0f, 0.0f, 0.0f, 0.0f, // first column
    0.0f, 1.0f, 0.0f, 0.0f, // second column
    0.0f, 0.0f, 1.0f, 0.0f, // third column
    0.0f, 0.0f, 0.0f, 1.0f  // fourth column x y z
    };

    int matrix_location = glGetUniformLocation(shader_programme, "matrix");
    glUseProgram(shader_programme);
    glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);

    int matrix_location2 = glGetUniformLocation(shader_programme2, "matrix2");
    glUseProgram(shader_programme2);
    glUniformMatrix4fv(matrix_location2, 1, GL_FALSE, matrix2);

    //glEnable(GL_CULL_FACE); // cull face
    //glCullFace(GL_BACK);    // cull back face
    //glFrontFace(GL_CW);     // GL_CCW for counter clock-wise

    float speed = 1.0f; // move at 1 unit per second
    float speed2 = 1.0f; // move at 1 unit per second
    float last_position = 0.0f;
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

        // update the matrix
        // - you could simplify this by just using sin(current_seconds)
        matrix[12] = elapsed_seconds * speed + last_position;
        last_position = matrix[12];
        if (fabs(last_position) > 0.8f) { speed = -speed; }
        //
        // Note: this call is related to the most recently 'used' shader programme
        glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);

        //
        // Note: this call is not necessary, but I like to do it anyway before any
        // time that I call glDrawArrays() so I never use the wrong vertex data
        glBindVertexArray(VAO[0]);
        // draw points 0-3 from the currently bound VAO with current in-use shader
        glDrawArrays(GL_TRIANGLE_FAN, 0, 104);
        glBindVertexArray(0);
        glUseProgram(0);
        glUseProgram(shader_programme2);
        matrix2[13] = elapsed_seconds * speed2 + last_position2;
        last_position2 = matrix2[13];
        if (fabs(last_position2) > 0.8) { speed2 = -speed2; }
        //
        // Note: this call is related to the most recently 'used' shader programme
        glUniformMatrix4fv(matrix_location2, 1, GL_FALSE, matrix2);
        glBindVertexArray(VAO[1]);
        // draw points 0-3 from the currently bound VAO with current in-use shader
        glDrawArrays(GL_TRIANGLE_FAN, 0, 104);
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
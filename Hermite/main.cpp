#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>

#define WIDTH 1280
#define HEIGHT 720

#define PI 3.14159265359


GLuint vao[1];
GLuint vbo[2];


GLint dragged = -1;

int pointsNum = 24; // alapból ekkora a mérete a points tömbnek

GLfloat points[42] = {
    -0.75f,0.75f,0.0f, // P1
    -0.3f,0.4f,0.0f,  // P2                                                     
    -0.1f,0.3f,0.0f,  // P3
    0.2f,-0.6f,0.0f,  // P4
    0.5f,0.2f,0.0f,    // P5
    0.4f,0.6f,0.0f,    // P6
    0.2f,-0.3f,0.0f,     // P7
    -0.4f,-0.4f,0.0f     // P8
    //0.7f, -0.3f, 0.0f,
    //0.8f, -0.4f, 0.0f,
    //0.2f, 0.3f, 0.0f,
    //0.5f, -0.4f, 0.0f
};


GLfloat HermiteCurve[6045]; // 948
GLfloat colors[6045];  // 948 volt alapbol // 3324

// hermite

GLfloat t;
GLint i;

GLfloat step = 1.0 / 99.0; // t \in [0,1]

// circle

GLfloat tCircle;
GLint iCircle;
GLfloat r = 0.02f;  // 0.05

GLfloat stepCircle = (2 * PI) / 100;

GLfloat dist2_2d(GLfloat P1x, GLfloat P1y, GLfloat P2x, GLfloat P2y) {

    GLfloat dx = P1x - P2x;
    GLfloat dy = P1y - P2y;
    return dx * dx + dy * dy;
}

GLint getActivePoint(GLfloat* p, GLfloat sensitivity, GLfloat x, GLfloat y) {

    GLfloat		s = sensitivity * sensitivity;
    GLfloat		xNorm = -1 + x / (WIDTH / 2);
    GLfloat		yNorm = -1 + (HEIGHT - y) / (HEIGHT / 2);

    for (GLint i = 0; i < pointsNum / 3; i++)
        if (dist2_2d(p[i * 3], p[i * 3 + 1], xNorm, yNorm) < s)
            return i;

    return -1;
}

void cursorPosCallback(GLFWwindow* window, double x, double y) {

    if (dragged >= 0) {

        GLfloat		xNorm = -1 + x / (WIDTH / 2);
        GLfloat		yNorm = -1 + (HEIGHT - y) / (HEIGHT / 2);
        
        GLfloat regix = points[3 * dragged];
        GLfloat regiy = points[3 * dragged + 1];

        points[3 * dragged] = xNorm;  // x coord
        points[3 * dragged + 1] = yNorm;  // y coord

        if (dragged % 2 == 0) {
            points[3 * dragged + 3] = points[3 * dragged + 3] + (points[3 * dragged] - regix);  // x coord
            points[3 * dragged + 1 + 3] = points[3 * dragged + 1 + 3] + (points[3 * dragged + 1] - regiy);;  // y coord
        }

        for (int j = 0; j < pointsNum - 6; j += 6)
        {
            HermiteCurve[(j * 300 / 6)] = points[j];
            HermiteCurve[(j * 300 / 6) + 1] = points[j + 1];
            HermiteCurve[(j * 300 / 6) + 2] = points[j + 2];

            colors[(j * 300 / 6)] = 1.0f;
            colors[(j * 300 / 6) + 1] = 0.0f;
            colors[(j * 300 / 6) + 2] = 0.0f;

            //std::cout << j/6 << ". pont: "<< points[j] << " " << points[j+1] << " " << points[j+2] << "\n";

            for (i = 1; i < 99; i++) {
                t = 0 + i * step;
                HermiteCurve[(j * 300 / 6) + i * 3] = points[j] * (2 * (t * t * t) - 3 * (t * t) + 1) + points[j + 6] * (-2 * (t * t * t) + 3 * (t * t)) + (points[j + 3] - points[j]) * ((t * t * t) - 2 * (t * t) + t) + (points[j + 9] - points[j + 6]) * ((t * t * t) - (t * t));
                HermiteCurve[(j * 300 / 6) + i * 3 + 1] = points[j + 1] * (2 * (t * t * t) - 3 * (t * t) + 1) + points[j + 7] * (-2 * (t * t * t) + 3 * (t * t)) + (points[j + 4] - points[j + 1]) * ((t * t * t) - 2 * (t * t) + t) + (points[j + 10] - points[j + 7]) * ((t * t * t) - (t * t));
                HermiteCurve[(j * 300 / 6) + i * 3 + 2] = 0.0f;

                colors[(j * 300 / 6) + i * 3] = 1.0f;
                colors[(j * 300 / 6) + i * 3 + 1] = 0.0f;
                colors[(j * 300 / 6) + i * 3 + 2] = 0.0f;
            }

            HermiteCurve[(j * 300 / 6) + 297] = points[j + 6];
            HermiteCurve[(j * 300 / 6) + 298] = points[j + 7];
            HermiteCurve[(j * 300 / 6) + 299] = 0.0f;

            colors[(j * 300 / 6) + 297] = 1.0f;
            colors[(j * 300 / 6) + 297 + 1] = 0.0f;
            colors[(j * 300 / 6) + 297 + 2] = 0.0f;

        }

        // érintõvektorok
        for (int i = 0; i < pointsNum; i += 3)
        {
            HermiteCurve[(pointsNum / 3 - 2) * 300 / 2 + i] = points[i];
            HermiteCurve[(pointsNum / 3 - 2) * 300 / 2 + i + 1] = points[i + 1];
            HermiteCurve[(pointsNum / 3 - 2) * 300 / 2 + i + 2] = points[i + 2];

            colors[(pointsNum / 3 - 2) * 300 / 2 + i] = 1.0f;  // P1x
            colors[(pointsNum / 3 - 2) * 300 / 2 + i + 1] = 1.0f;
            colors[(pointsNum / 3 - 2) * 300 / 2 + i + 2] = 0.0f;
        }

        ////// GENERATING CIRCLE POINTS ///////
        for (int i = 0; i < pointsNum; i += 3)
        {
            for (iCircle = 0; iCircle < 101; iCircle++) {
                tCircle = iCircle * stepCircle;

                HermiteCurve[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + i * 100] = points[i] + r * cos(tCircle);
                HermiteCurve[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + 1 + i * 100] = points[i + 1] + r * sin(tCircle);
                HermiteCurve[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + 2 + i * 100] = 0.0f;

                colors[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + i * 100] = 1.0f;
                colors[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + 1 + i * 100] = 0.0f;
                colors[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + 2 + i * 100] = 1.0f;
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(HermiteCurve), HermiteCurve, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double	x, y;

        glfwGetCursorPos(window, &x, &y);
        dragged = getActivePoint(points, 0.1f, x, y);
        if (dragged == -1) {
            GLfloat		xNorm = -1 + x / (WIDTH / 2);
            GLfloat		yNorm = -1 + (HEIGHT - y) / (HEIGHT / 2);

            if (pointsNum <= 36) {
                points[pointsNum] = xNorm;  // x coord
                points[pointsNum + 1] = yNorm;  // y coord
                points[pointsNum + 2] = 0.0f;

                points[pointsNum + 3] = xNorm + 0.2f;
                points[pointsNum + 4] = yNorm + 0.2f;
                points[pointsNum + 5] = 0.0f;

                pointsNum += 6;

                for (int j = 0; j < pointsNum - 6; j += 6)
                {
                    HermiteCurve[(j * 300 / 6)] = points[j];
                    HermiteCurve[(j * 300 / 6) + 1] = points[j + 1];
                    HermiteCurve[(j * 300 / 6) + 2] = points[j + 2];

                    colors[(j * 300 / 6)] = 1.0f;
                    colors[(j * 300 / 6) + 1] = 0.0f;
                    colors[(j * 300 / 6) + 2] = 0.0f;

                    //std::cout << j/6 << ". pont: "<< points[j] << " " << points[j+1] << " " << points[j+2] << "\n";

                    for (i = 1; i < 99; i++) {
                        t = 0 + i * step;
                        HermiteCurve[(j * 300 / 6) + i * 3] = points[j] * (2 * (t * t * t) - 3 * (t * t) + 1) + points[j + 6] * (-2 * (t * t * t) + 3 * (t * t)) + (points[j + 3] - points[j]) * ((t * t * t) - 2 * (t * t) + t) + (points[j + 9] - points[j + 6]) * ((t * t * t) - (t * t));
                        HermiteCurve[(j * 300 / 6) + i * 3 + 1] = points[j + 1] * (2 * (t * t * t) - 3 * (t * t) + 1) + points[j + 7] * (-2 * (t * t * t) + 3 * (t * t)) + (points[j + 4] - points[j + 1]) * ((t * t * t) - 2 * (t * t) + t) + (points[j + 10] - points[j + 7]) * ((t * t * t) - (t * t));
                        HermiteCurve[(j * 300 / 6) + i * 3 + 2] = 0.0f;

                        colors[(j * 300 / 6) + i * 3] = 1.0f;
                        colors[(j * 300 / 6) + i * 3 + 1] = 0.0f;
                        colors[(j * 300 / 6) + i * 3 + 2] = 0.0f;
                    }

                    HermiteCurve[(j * 300 / 6) + 297] = points[j + 6];
                    HermiteCurve[(j * 300 / 6) + 298] = points[j + 7];
                    HermiteCurve[(j * 300 / 6) + 299] = 0.0f;

                    colors[(j * 300 / 6) + 297] = 1.0f;
                    colors[(j * 300 / 6) + 297 + 1] = 0.0f;
                    colors[(j * 300 / 6) + 297 + 2] = 0.0f;

                }

                // érintõvektorok
                for (int i = 0; i < pointsNum; i += 3)
                {
                    HermiteCurve[(pointsNum / 3 - 2) * 300 / 2 + i] = points[i];
                    HermiteCurve[(pointsNum / 3 - 2) * 300 / 2 + i + 1] = points[i + 1];
                    HermiteCurve[(pointsNum / 3 - 2) * 300 / 2 + i + 2] = points[i + 2];

                    colors[(pointsNum / 3 - 2) * 300 / 2 + i] = 1.0f;  // P1x
                    colors[(pointsNum / 3 - 2) * 300 / 2 + i + 1] = 1.0f;
                    colors[(pointsNum / 3 - 2) * 300 / 2 + i + 2] = 0.0f;
                }

                ////// GENERATING CIRCLE POINTS ///////
                for (int i = 0; i < pointsNum; i += 3)
                {
                    for (iCircle = 0; iCircle < 101; iCircle++) {
                        tCircle = iCircle * stepCircle;

                        HermiteCurve[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + i * 100] = points[i] + r * cos(tCircle);
                        HermiteCurve[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + 1 + i * 100] = points[i + 1] + r * sin(tCircle);
                        HermiteCurve[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + 2 + i * 100] = 0.0f;

                        colors[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + i * 100] = 1.0f;
                        colors[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + 1 + i * 100] = 0.0f;
                        colors[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + 2 + i * 100] = 1.0f;

                        //std::cout << "index: " << iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + 2 + i * 100 << "\n";
                    }
                }

                glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(HermiteCurve), HermiteCurve, GL_STATIC_DRAW);
                glBindBuffer(GL_ARRAY_BUFFER, 0);

                glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
                glBindBuffer(GL_ARRAY_BUFFER, 0);


                /*for (int i = 0; i < pointsNum; i++)
                {
                    std::cout << i << ". elem: " << points[i] << "\n";
                }*/
            }
        }
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        dragged = -1;
}

int main() {
    GLFWwindow* window = NULL;
    const GLubyte* renderer;
    const GLubyte* version;

    const char* vertex_shader =
        "#version 410\n"
        "layout (location = 0) in vec3 aPoints;"
        "layout (location = 1) in vec3 aColor;"
        "out vec3 myColor;"
        "void main () {"
        "gl_Position = vec4(aPoints, 1.0);"
        "myColor = aColor;"
        "}";

    const char* fragment_shader =
        "#version 410\n"
        "in vec3 myColor;"
        "out vec4 color;"
        "void main () {"
        "color = vec4(myColor,1.0);"
        "}";

    GLuint vert_shader, frag_shader;
    GLuint shader_programme;

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Drag&Drop", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    /*for (int i = 0; i < pointsNum; i++)
    {
        std::cout << i << ": " << points[i] << "\n";
    }
    std::cout << "\n";*/

    for (int j = 0; j < pointsNum - 6; j += 6)
    {
        HermiteCurve[(j * 300 / 6)] = points[j];
        HermiteCurve[(j * 300 / 6) + 1] = points[j + 1];
        HermiteCurve[(j * 300 / 6) + 2] = points[j + 2];

        colors[(j * 300 / 6)] = 1.0f;
        colors[(j * 300 / 6) + 1] = 0.0f;
        colors[(j * 300 / 6) + 2] = 0.0f;

        //std::cout << j/6 << ". pont: "<< points[j] << " " << points[j+1] << " " << points[j+2] << "\n";

        for (i = 1; i < 99; i++) {
            t = 0 + i * step;
            HermiteCurve[(j * 300 / 6) + i * 3] = points[j] * (2 * (t * t * t) - 3 * (t * t) + 1) + points[j + 6] * (-2 * (t * t * t) + 3 * (t * t)) + (points[j + 3] - points[j]) * ((t * t * t) - 2 * (t * t) + t) + (points[j + 9] - points[j + 6]) * ((t * t * t) - (t * t));
            HermiteCurve[(j * 300 / 6) + i * 3 + 1] = points[j + 1] * (2 * (t * t * t) - 3 * (t * t) + 1) + points[j + 7] * (-2 * (t * t * t) + 3 * (t * t)) + (points[j + 4] - points[j + 1]) * ((t * t * t) - 2 * (t * t) + t) + (points[j + 10] - points[j + 7]) * ((t * t * t) - (t * t));
            HermiteCurve[(j * 300 / 6) + i * 3 + 2] = 0.0f;

            colors[(j * 300 / 6) + i * 3] = 1.0f;
            colors[(j * 300 / 6) + i * 3 + 1] = 0.0f;
            colors[(j * 300 / 6) + i * 3 + 2] = 0.0f;
        }

        HermiteCurve[(j * 300 / 6) + 297] = points[j + 6];
        HermiteCurve[(j * 300 / 6) + 298] = points[j + 7];
        HermiteCurve[(j * 300 / 6) + 299] = 0.0f;

        colors[(j * 300 / 6) + 297] = 1.0f;
        colors[(j * 300 / 6) + 297 + 1] = 0.0f;
        colors[(j * 300 / 6) + 297 + 2] = 0.0f;

    }
    /*for (int i = 0; i < (pointsNum/3-2)*300/2; i++)
    {
        std::cout << i << ": " << HermiteCurve[i] << "\n";
    }
    std::cout << "\n";*/



    // érintõvektorok
    for (int i = 0; i < pointsNum; i += 3)
    {
        HermiteCurve[(pointsNum / 3 - 2) * 300 / 2 + i] = points[i];
        HermiteCurve[(pointsNum / 3 - 2) * 300 / 2 + i + 1] = points[i + 1];
        HermiteCurve[(pointsNum / 3 - 2) * 300 / 2 + i + 2] = points[i + 2];

        colors[(pointsNum / 3 - 2) * 300 / 2 + i] = 1.0f;  // P1x
        colors[(pointsNum / 3 - 2) * 300 / 2 + i + 1] = 1.0f;
        colors[(pointsNum / 3 - 2) * 300 / 2 + i + 2] = 0.0f;
    }
    /*
    for (int i = (pointsNum / 3 - 2) * 300 / 2; i < (pointsNum / 3 - 2) * 300 / 2 + pointsNum; i++)
    {
        std::cout << i << ": " << HermiteCurve[i] << "\n";
    }
    std::cout << "\n";

    std::cout << (pointsNum / 3 - 2) * 300 / 2 << "\n";*/

    ////// GENERATING CIRCLE POINTS ///////
    for (int i = 0; i < pointsNum; i += 3)
    {
        for (iCircle = 0; iCircle < 101; iCircle++) {
            tCircle = iCircle * stepCircle;

            HermiteCurve[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + i * 100] = points[i] + r * cos(tCircle);
            HermiteCurve[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + 1 + i * 100] = points[i + 1] + r * sin(tCircle);
            HermiteCurve[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + 2 + i * 100] = 0.0f;

            colors[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + i * 100] = 1.0f;
            colors[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + 1 + i * 100] = 0.0f;
            colors[iCircle * 3 + ((pointsNum / 3 - 2) * 300 / 2 + pointsNum) + 2 + i * 100] = 1.0f;
        }
    }

    glGenBuffers(2, vbo);
    glGenVertexArrays(1, vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(HermiteCurve), HermiteCurve, GL_STATIC_DRAW);
    glBindVertexArray(vao[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);



    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glBindVertexArray(vao[0]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);


    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vertex_shader, NULL);
    glCompileShader(vert_shader);

    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &fragment_shader, NULL);
    glCompileShader(frag_shader);

    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, frag_shader);
    glAttachShader(shader_programme, vert_shader);
    glLinkProgram(shader_programme);

    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    glPointSize(15.0f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_programme);
        glBindVertexArray(vao[0]);


        //std::cout << "asd " << (pointsNum / 3 * 100 + 100)/3 + pointsNum / 3 << "\n";


        // a köröket rajzoltatom ki elõször h majd azok legyenek felül
        for (int i = 0; i < pointsNum; i += 3)
        {
            glDrawArrays(GL_TRIANGLE_FAN, (pointsNum / 3 - 2) * 300 / 2 / 3 + pointsNum / 3 + i / 3 * 100, 100);
        }

        // hermite ív
        glDrawArrays(GL_LINE_STRIP, 0, (pointsNum / 3 - 2) * 300 / 2 / 3);

        // érintõvektorok
        glDrawArrays(GL_LINES, (pointsNum / 3 - 2) * 300 / 2 / 3, pointsNum / 3);

        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
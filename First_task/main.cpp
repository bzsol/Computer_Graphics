#define _USE_MATH_DEFINES
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>

#define WIDTH 800
#define HEIGHT 800

#define VAOSIZE 3
#define VBOSIZE 6

GLuint VAO[VAOSIZE];
GLuint VBO[VBOSIZE];


GLint dragged = -1;

std::vector<GLfloat> control_points = {
    -0.8f,-0.25f,0.0f, // P1
    -0.9f,-0.4f,0.0f,  // P2                                                     
    -0.5f,0.10f,0.0f,  // P3
    -0.2f,-0.6f,0.0f,  // P4
    0.4f,0.3f,0.0f,    // P5
    0.3f,0.5f,0.0f,    // P6
    0.8f,0.5f,0.0f,     // P7
    0.9f,-0.8f,0.0f     // P8
};

std::vector<GLfloat> HermiteCurve;
std::vector<GLfloat> Hermite_color;
std::vector<GLfloat> control_points_colors;
std::vector<GLfloat> control_points_colors2;
std::vector<GLfloat> circle_points;


GLfloat t;
GLint i;

GLfloat step = 1.0 / 99.0; // t \in [0,1]


GLfloat P1x = control_points[0];
GLfloat P1y = control_points[1];

GLfloat P2x = control_points[3];
GLfloat P2y = control_points[4];

GLfloat P3x = control_points[6];
GLfloat P3y = control_points[7];

GLfloat P4x = control_points[9];
GLfloat P4y = control_points[10];

GLfloat P5x = control_points[12];
GLfloat P5y = control_points[13];

GLfloat P6x = control_points[15];
GLfloat P6y = control_points[16];

GLfloat P7x = control_points[18];
GLfloat P7y = control_points[19];

GLfloat P8x = control_points[21];
GLfloat P8y = control_points[22];

GLfloat R1x = P2x - P1x;
GLfloat R1y = P2y - P1y;

GLfloat R2x = P4x - P3x;
GLfloat R2y = P4y - P3y;

GLfloat R3x = P6x - P5x;
GLfloat R3y = P6y - P5y;

GLfloat R4x = P8x - P7x;
GLfloat R4y = P8y - P7y;




GLfloat dist2_2d(GLfloat P1x, GLfloat P1y, GLfloat P2x, GLfloat P2y) {

    GLfloat dx = P1x - P2x;
    GLfloat dy = P1y - P2y;
    return dx * dx + dy * dy;
}

GLint getActivePoint(GLfloat* p, GLfloat sensitivity, GLfloat x, GLfloat y) {

    GLfloat		s = sensitivity * sensitivity;
    GLfloat		xNorm = -1 + x / (WIDTH / 2);
    GLfloat		yNorm = -1 + (HEIGHT - y) / (HEIGHT / 2);

    for (GLint i = 0; i < control_points.size() - 1; i++)
        if (dist2_2d(p[i * 3], p[i * 3 + 1], xNorm, yNorm) < s)
            return i;

    return -1;
}

void HermiteCalc(GLfloat P1x, GLfloat P1y, GLfloat P2x, GLfloat P2y, GLfloat R1x, GLfloat R1y, GLfloat R2x, GLfloat R2y, std::vector<GLfloat> &vector) {
    vector.push_back(P1x);
    vector.push_back(P1y);
    vector.push_back(0.0f);
    for (i = 1; i <= 99; i++) {
        t = 0 + i * step;
        vector.push_back(P1x * (2 * (t * t * t) - 3 * (t * t) + 1) + P2x * (-2 * (t * t * t) + 3 * (t * t)) + R1x * ((t * t * t) - 2 * (t * t) + t) + R2x * ((t * t * t) - (t * t)));
        vector.push_back(P1y * (2 * (t * t * t) - 3 * (t * t) + 1) + P2y * (-2 * (t * t * t) + 3 * (t * t)) + R1y * ((t * t * t) - 2 * (t * t) + t) + R2y * ((t * t * t) - (t * t)));
        vector.push_back(0.0f);
    }
    vector.push_back(P2x);
    vector.push_back(P2y);
    vector.push_back(0.0f);

}


std::vector<GLfloat> GenerateColorPoints(GLfloat R, GLfloat G, GLfloat B, int size) {
    std::vector<GLfloat> color_vector;
    for (i = 0; i < size / 3; i++) {
        color_vector.push_back(R);
        color_vector.push_back(G);
        color_vector.push_back(B);
    }
    return color_vector;
}

std::vector<GLfloat> GenerateCirclePoints(std::vector<GLfloat> control) {
    std::vector<GLfloat> circle;
    GLfloat t;
    GLfloat r = 0.0145f;

    GLfloat stepCircle = (2 * M_PI) / 100;

    for (int i = 0; i < control.size(); i += 3)
    {
        for (int j = 0; j <= 100; j++) {
            t = j * stepCircle;

            circle.push_back(control.at(i) + r * cos(t));
            circle.push_back(control.at(i+1) + r * sin(t));
            circle.push_back(0.0f);
        }
    }

    return circle;
}



void cursorPosCallback(GLFWwindow* window, double x, double y) {

    if (dragged >= 0) {

        GLfloat		xNorm = -1 + x / (WIDTH / 2);
        GLfloat		yNorm = -1 + (HEIGHT - y) / (HEIGHT / 2);

        control_points[3 * dragged] = xNorm;  // x coord
        control_points[3 * dragged + 1] = yNorm;  // y coord


        HermiteCurve.clear();

        for (int i = 0; i < control_points.size()-9; i += 6) {
            GLfloat P1x = control_points.at(i);
            GLfloat P1y = control_points.at(i+1);

            GLfloat P3x = control_points.at(i + 6);
            GLfloat P3y = control_points.at(i + 7);

            GLfloat P2x = control_points.at(i+3);
            GLfloat P2y = control_points.at(i+4);

            GLfloat P4x = control_points.at(i+9);
            GLfloat P4y = control_points.at(i+10);

            GLfloat R1x = P2x - P1x;
            GLfloat R1y = P2y - P1y;

            GLfloat R2x = P4x - P3x;
            GLfloat R2y = P4y - P3y;

            HermiteCalc(P1x, P1y, P3x, P3y, R1x, R1y, R2x, R2y, HermiteCurve);
        }
        Hermite_color = GenerateColorPoints(0.0f, 1.0f, 0.0f, HermiteCurve.size());

        circle_points = GenerateCirclePoints(control_points);
        control_points_colors = GenerateColorPoints(1.0f, 0.0f, 0.0f, circle_points.size());
        control_points_colors2 = GenerateColorPoints(0.0f, 0.0f, 1.0f, control_points.size());

        glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
        glBufferData(GL_ARRAY_BUFFER, circle_points.size() * sizeof(GLfloat), circle_points.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);


        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, control_points_colors.size() * sizeof(GLfloat), control_points_colors.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO[0]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, control_points.size() * sizeof(GLfloat), control_points.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO[1]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
        glBufferData(GL_ARRAY_BUFFER, control_points_colors2.size() * sizeof(GLfloat), control_points_colors2.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO[1]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
        glBufferData(GL_ARRAY_BUFFER, HermiteCurve.size() * sizeof(GLfloat), HermiteCurve.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO[2]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
        glBufferData(GL_ARRAY_BUFFER, Hermite_color.size() * sizeof(GLfloat), Hermite_color.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO[2]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);


    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double	x, y;

        glfwGetCursorPos(window, &x, &y);
        dragged = getActivePoint(control_points.data(), 0.1f, x, y);
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) { dragged = -1; }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        double	x, y;
        glfwGetCursorPos(window, &x, &y);
        GLfloat		xNorm = -1 + x / (WIDTH / 2);
        GLfloat		yNorm = -1 + (HEIGHT - y) / (HEIGHT / 2);
        control_points.push_back(xNorm);
        control_points.push_back(yNorm);
        control_points.push_back(0.0f);

        HermiteCurve.clear();

        for (int i = 0; i < control_points.size() - 9; i += 6) {
            GLfloat P1x = control_points.at(i);
            GLfloat P1y = control_points.at(i + 1);

            GLfloat P3x = control_points.at(i + 6);
            GLfloat P3y = control_points.at(i + 7);

            GLfloat P2x = control_points.at(i + 3);
            GLfloat P2y = control_points.at(i + 4);

            GLfloat P4x = control_points.at(i + 9);
            GLfloat P4y = control_points.at(i + 10);

            GLfloat R1x = P2x - P1x;
            GLfloat R1y = P2y - P1y;

            GLfloat R2x = P4x - P3x;
            GLfloat R2y = P4y - P3y;

            HermiteCalc(P1x, P1y, P3x, P3y, R1x, R1y, R2x, R2y, HermiteCurve);
        }
        Hermite_color = GenerateColorPoints(0.0f, 1.0f, 0.0f, HermiteCurve.size());

        circle_points = GenerateCirclePoints(control_points);
        control_points_colors = GenerateColorPoints(1.0f, 0.0f, 0.0f, circle_points.size());
        control_points_colors2 = GenerateColorPoints(0.0f, 0.0f, 1.0f, control_points.size());

        glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
        glBufferData(GL_ARRAY_BUFFER, circle_points.size() * sizeof(GLfloat), circle_points.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);


        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, control_points_colors.size() * sizeof(GLfloat), control_points_colors.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO[0]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, control_points.size() * sizeof(GLfloat), control_points.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO[1]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
        glBufferData(GL_ARRAY_BUFFER, control_points_colors2.size() * sizeof(GLfloat), control_points_colors2.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO[1]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
        glBufferData(GL_ARRAY_BUFFER, HermiteCurve.size() * sizeof(GLfloat), HermiteCurve.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO[2]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
        glBufferData(GL_ARRAY_BUFFER, Hermite_color.size() * sizeof(GLfloat), Hermite_color.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO[2]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);
    }
}

int main() {
    GLFWwindow* window = NULL;
    const GLubyte* renderer;
    const GLubyte* version;

    const char* vertex_shader =
        "#version 330\n"
        "layout (location = 0) in vec3 aPoints;"
        "layout (location = 1) in vec3 aColor;"
        "out vec3 myColor;"
        "void main () {"
        "gl_Position = vec4(aPoints, 1.0);"
        "myColor = aColor;"
        "}";

    const char* fragment_shader =
        "#version 330\n"
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

    circle_points = GenerateCirclePoints(control_points);
    control_points_colors = GenerateColorPoints(1.0f, 0.0f, 0.0f, circle_points.size());
    control_points_colors2 = GenerateColorPoints(0.0f, 0.0f, 1.0f, control_points.size());

    HermiteCalc(P1x, P1y, P3x, P3y, R1x, R1y, R2x, R2y, HermiteCurve);
    HermiteCalc(P3x, P3y, P5x, P5y, R2x, R2y, R3x, R3y, HermiteCurve);
    HermiteCalc(P5x, P5y, P7x, P7y, R3x, R3y, R4x, R4y, HermiteCurve);
    for (int i = 0; i < HermiteCurve.size(); i++) {
        std::cout << i << "index   " << HermiteCurve[i] << std::endl;
    }

    Hermite_color = GenerateColorPoints(0.0f, 1.0f, 0.0f, HermiteCurve.size());


    glGenBuffers(VBOSIZE, VBO);
    glGenVertexArrays(VAOSIZE, VAO);

    // Vektor pontok
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, circle_points.size() * sizeof(GLfloat), circle_points.data(), GL_STATIC_DRAW);
    glBindVertexArray(VAO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);




    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, control_points_colors.size() * sizeof(GLfloat), control_points_colors.data(), GL_STATIC_DRAW);
    glBindVertexArray(VAO[0]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);


    // Vektor vonalak
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, control_points.size() * sizeof(GLfloat), control_points.data(), GL_STATIC_DRAW);
    glBindVertexArray(VAO[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, control_points_colors2.size() * sizeof(GLfloat), control_points_colors2.data(), GL_STATIC_DRAW);
    glBindVertexArray(VAO[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    // Hermite

    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, HermiteCurve.size() * sizeof(GLfloat), HermiteCurve.data(), GL_STATIC_DRAW);
    glBindVertexArray(VAO[2]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, Hermite_color.size() * sizeof(GLfloat), Hermite_color.data(), GL_STATIC_DRAW);
    glBindVertexArray(VAO[2]);
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


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_programme);

        glBindVertexArray(VAO[0]);
        for (int i = 0; i < circle_points.size() / 3; i++) {
            glDrawArrays(GL_TRIANGLE_FAN, i * 100 + i, 100);
        }
        glBindVertexArray(0);
        glBindVertexArray(VAO[2]);
        glDrawArrays(GL_LINE_STRIP, 0, (HermiteCurve.size() / 3));
        glBindVertexArray(0);
        glBindVertexArray(VAO[1]);
        glLineWidth(3.5);
        glDrawArrays(GL_LINES, 0, (control_points.size() / 3));
        glBindVertexArray(0);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
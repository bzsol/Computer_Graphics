#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>

#define WIDTH 600
#define HEIGHT 600

GLuint VAO[3];
GLuint VBO[6];


GLint dragged = -1;

GLfloat points[24] = {
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
std::vector<GLfloat> control_points;
std::vector<GLfloat> control_points_colors;
std::vector<GLfloat> control_points_colors2;


GLfloat t;
GLint i;

GLfloat step = 1.0 / 99.0; // t \in [0,1]


GLfloat P1x = points[0];
GLfloat P1y = points[1];

GLfloat P2x = points[3];
GLfloat P2y = points[4];

GLfloat P3x = points[6];
GLfloat P3y = points[7];

GLfloat P4x = points[9];
GLfloat P4y = points[10];

GLfloat P5x = points[12];
GLfloat P5y = points[13];

GLfloat P6x = points[15];
GLfloat P6y = points[16];

GLfloat P7x = points[18];
GLfloat P7y = points[19];

GLfloat P8x = points[21];
GLfloat P8y = points[22];

GLfloat R1x = fabs(P2x - P1x);
GLfloat R1y = fabs(P2y - P1y);

GLfloat R2x = fabs(P4x - P3x);
GLfloat R2y = fabs(P4y - P3y);

GLfloat R3x = fabs(P6x - P5x);
GLfloat R3y = fabs(P6y - P5y);

GLfloat R4x = fabs(P8x - P7x);
GLfloat R4y = fabs(P8y - P7y);




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

void  HermiteCalc(GLfloat P1x, GLfloat P1y, GLfloat P2x, GLfloat P2y, GLfloat R1x, GLfloat R1y, GLfloat R2x, GLfloat R2y, std::vector<GLfloat>& vector) {
    vector.push_back(P1x);
    vector.push_back(P1y);
    vector.push_back(0.0f);
    for (i = 1; i < 99; i++) {
        t = 0 + i * step;
        vector.push_back(P1x * (2 * (t * t * t) - 3 * (t * t) + 1) + P2x * (-2 * (t * t * t) + 3 * (t * t)) + R1x * ((t * t * t) - 2 * (t * t) + t) + R2x * ((t * t * t) - (t * t)));
        vector.push_back(P1y * (2 * (t * t * t) - 3 * (t * t) + 1) + P2y * (-2 * (t * t * t) + 3 * (t * t)) + R1y * ((t * t * t) - 2 * (t * t) + t) + R2y * ((t * t * t) - (t * t)));
        vector.push_back(0.0f);
    }
    vector.push_back(P2x);
    vector.push_back(P2y);
    vector.push_back(0.0f);

}



std::vector<GLfloat> Generate_Color_Points(GLfloat R, GLfloat G, GLfloat B, int size) {
    std::vector<GLfloat> color_vector;
    for (i = 0; i < size / 3; i++) {
        color_vector.push_back(R);
        color_vector.push_back(G);
        color_vector.push_back(B);
    }
    return color_vector;
}


void Generate_Control_Points() {

    control_points.push_back(P1x);
    control_points.push_back(P1y);
    control_points.push_back(0.0f);

    control_points.push_back(P2x);
    control_points.push_back(P2y);
    control_points.push_back(0.0f);

    control_points.push_back(P3x);
    control_points.push_back(P3y);
    control_points.push_back(0.0f);

    control_points.push_back(P4x);
    control_points.push_back(P4y);
    control_points.push_back(0.0f);

    control_points.push_back(P5x);
    control_points.push_back(P5y);
    control_points.push_back(0.0f);

    control_points.push_back(P6x);
    control_points.push_back(P6y);
    control_points.push_back(0.0f);

    control_points.push_back(P7x);
    control_points.push_back(P7y);
    control_points.push_back(0.0f);

    control_points.push_back(P8x);
    control_points.push_back(P8y);
    control_points.push_back(0.0f);

}


void cursorPosCallback(GLFWwindow* window, double x, double y) {

    if (dragged >= 0) {

        GLfloat		xNorm = -1 + x / (WIDTH / 2);
        GLfloat		yNorm = -1 + (HEIGHT - y) / (HEIGHT / 2);

        points[3 * dragged] = xNorm;  // x coord
        points[3 * dragged + 1] = yNorm;  // y coord

        GLfloat P1x = points[0];
        GLfloat P1y = points[1];

        GLfloat P2x = points[3];
        GLfloat P2y = points[4];

        GLfloat P3x = points[6];
        GLfloat P3y = points[7];

        GLfloat P4x = points[9];
        GLfloat P4y = points[10];

        GLfloat P5x = points[12];
        GLfloat P5y = points[13];

        GLfloat P6x = points[15];
        GLfloat P6y = points[16];

        GLfloat P7x = points[18];
        GLfloat P7y = points[19];

        GLfloat P8x = points[21];
        GLfloat P8y = points[22];

        GLfloat R1x = fabs(P2x - P1x);
        GLfloat R1y = fabs(P2y - P1y);

        GLfloat R2x = fabs(P4x - P3x);
        GLfloat R2y = fabs(P4y - P3y);

        GLfloat R3x = fabs(P6x - P5x);
        GLfloat R3y = fabs(P6y - P5y);

        GLfloat R4x = fabs(P8x - P7x);
        GLfloat R4y = fabs(P8y - P7y);





        HermiteCurve.clear();
        control_points.clear();


        HermiteCalc(P1x, P1y, P3x, P3y, R1x, R1y, R2x, R2y, HermiteCurve);
        HermiteCalc(P3x, P3y, P5x, P5y, R2x, R2y, R3x, R3y, HermiteCurve);
        HermiteCalc(P5x, P5y, P7x, P7y, R3x, R3y, R4x, R4y, HermiteCurve);


        control_points.push_back(P1x);
        control_points.push_back(P1y);
        control_points.push_back(0.0f);

        control_points.push_back(P2x);
        control_points.push_back(P2y);
        control_points.push_back(0.0f);

        control_points.push_back(P3x);
        control_points.push_back(P3y);
        control_points.push_back(0.0f);

        control_points.push_back(P4x);
        control_points.push_back(P4y);
        control_points.push_back(0.0f);

        control_points.push_back(P5x);
        control_points.push_back(P5y);
        control_points.push_back(0.0f);

        control_points.push_back(P6x);
        control_points.push_back(P6y);
        control_points.push_back(0.0f);

        control_points.push_back(P7x);
        control_points.push_back(P7y);
        control_points.push_back(0.0f);

        control_points.push_back(P8x);
        control_points.push_back(P8y);
        control_points.push_back(0.0f);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, control_points.size() * sizeof(GLfloat), control_points.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
        glBufferData(GL_ARRAY_BUFFER, HermiteCurve.size() * sizeof(GLfloat), HermiteCurve.data(), GL_STATIC_DRAW);
        glBindVertexArray(VAO[2]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);


    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double	x, y;

        glfwGetCursorPos(window, &x, &y);
        dragged = getActivePoint(control_points.data(), 0.1f, x, y);
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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


    Generate_Control_Points();
    control_points_colors = Generate_Color_Points(1.0f, 0.0f, 0.0f, control_points.size());
    control_points_colors2 = Generate_Color_Points(0.0f, 1.0f, 0.0f, control_points.size());

    HermiteCalc(P1x, P1y, P3x, P3y, R1x, R1y, R2x, R2y, HermiteCurve);
    HermiteCalc(P3x, P3y, P5x, P5y, R2x, R2y, R3x, R3y, HermiteCurve);
    HermiteCalc(P5x, P5y, P7x, P7y, R3x, R3y, R4x, R4y, HermiteCurve);

    Hermite_color = Generate_Color_Points(1.0f, 0.5f, 0.3f, HermiteCurve.size());


    glGenBuffers(6, VBO);
    glGenVertexArrays(3, VAO);

    // Vektor pontok
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, control_points.size() * sizeof(GLfloat), control_points.data(), GL_STATIC_DRAW);
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

    glPointSize(15.0f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_programme);

        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_POINTS, 0, control_points.size() / 3);
        glBindVertexArray(0);
        glBindVertexArray(VAO[1]);
        glLineWidth(3.5);
        glDrawArrays(GL_LINES, 0, control_points.size() / 3);
        glBindVertexArray(0);
        glBindVertexArray(VAO[2]);
        glDrawArrays(GL_LINE_STRIP, 0, HermiteCurve.size() / 3);
        glBindVertexArray(0);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#define	 numVBOs			2
#define	 numVAOs			1

#define PI 3.14159265359

int main() {
    GLFWwindow* window = NULL;
    const GLubyte* renderer;
    const GLubyte* version;


    GLfloat x0 = 0.0f;
    GLfloat y0 = 0.0f;
    GLfloat r = 0.6f;

    GLuint VAO[numVAOs];
    GLuint VBO[numVBOs];
    GLfloat circle[315]; // 303 / 101 -> 3 pont -> x,y,z
    GLfloat circle_color[315];

    GLfloat t;
    GLint i;

    GLfloat step = (2 * PI) / 100;

    const char* vertex_shader =
        "#version 410\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        "out vec3 ourColor;"
        "void main () {"
        "gl_Position = vec4(aPos, 1.0);"
        "ourColor = aColor;"
        "}";

    const char* fragment_shader =
        "#version 410\n"
        "out vec4 frag_colour;"
        "in vec3 ourColor;"
        "void main () {"
        "  frag_colour = vec4(ourColor,1.0);"
        "}";

    GLuint vert_shader, frag_shader;
    GLuint shader_programme;

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(640, 640, "Hello Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    renderer = glGetString(GL_RENDERER);
    version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);

    ////// GENERATING CIRCLE POINTS ///////
    circle[0] = 0.0f;
    circle[1] = 0.0f;
    circle[2] = 0.0f;
    circle_color[0] = 1.0f;
    circle_color[1] = 0.0f;
    circle_color[2] = 0.0f;
    for (i = 1; i <= 104; i++) {
        t = i * step;
        circle[i * 3] = x0 + r * cos(t);
        circle[i * 3 + 1] = y0 + r * sin(t);
        circle[i * 3 + 2] = 0.0f;
        circle_color[i * 3] = 0.0f;
        circle_color[i * 3 + 1] = 0.0f;
        circle_color[i * 3 + 2] = 1.0f;
    }


    glGenBuffers(numVBOs, VBO);
    glGenVertexArrays(numVAOs, VAO);

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

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_programme);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 104);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
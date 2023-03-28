

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>
#define		numVBOs			2
#define		numVAOs			1

std::vector<GLfloat> generateCirclePoints(GLfloat x_start, GLfloat y_start, GLfloat z_start, GLfloat r, GLint num_segments) {
    std::vector<GLfloat> circle_points;
    GLfloat x, y;
    GLfloat full_circle = 2.0f * 3.14159265358979323846;
    GLfloat alpha = full_circle / (GLfloat)num_segments;
    circle_points.push_back(x_start);
    circle_points.push_back(y_start);
    circle_points.push_back(z_start);
    for (int i = 0; i <= num_segments; i++) {
        float angle = i * 2.0f * 3.14159265358979323846 / 360;
        float x = r * cosf(angle);
        float y = r * sinf(angle);
        circle_points.push_back(x);
        circle_points.push_back(y);
        circle_points.push_back(0.0f);
    }
    return circle_points;
}

int main() {
    srand((unsigned)time(NULL));
    GLFWwindow* window = NULL;
    const GLubyte* renderer;
    const GLubyte* version;

    GLuint		VBO[numVBOs];
    GLuint		VAO[numVAOs];

   /* GLfloat points[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
         -0.5f,  0.5f, 0.0f,  // top left

         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    }; */

    std::vector<GLfloat> points = generateCirclePoints(0.0,0.0,0.0,0.5,360);
    std::vector<GLfloat> colors;
    colors.push_back(1);
    colors.push_back(0);
    colors.push_back(0);
    for (int i = points.size()-1; i >= 0; i--) {
        colors.push_back(0);
        colors.push_back(1);
        colors.push_back(0);
    }

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

    /* start GL context and O/S window using the GLFW helper library */
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    /* Version 4.1 Core is a good default that should run on just about everything. Adjust later to suit project requirements. */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(600, 600, "Colored Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    /* start GLEW extension handler */
    glewExperimental = GL_TRUE;
    glewInit();

    /* get version info */
    renderer = glGetString(GL_RENDERER); /* get renderer string */
    version = glGetString(GL_VERSION);  /* version as a string */
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    /* tell GL to only draw onto a pixel if the shape is closer to the viewer
    than anything already drawn at that pixel */
    glEnable(GL_DEPTH_TEST); /* enable depth-testing */
    /* with LESS depth-testing interprets a smaller depth value as meaning "closer" */
    glDepthFunc(GL_LESS);

    glGenBuffers(numVBOs, VBO);
    glGenVertexArrays(numVAOs, VAO);


    // Coordinates
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(GLfloat), points.data(), GL_STATIC_DRAW);
    glBindVertexArray(VAO[0]);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);


    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
    glBindVertexArray(VAO[0]);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);



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




    /* this loop clears the drawing surface, then draws the geometry described
        by the VAO onto the drawing surface. we 'poll events' to see if the window
    was closed, etc. finally, we 'swap the buffers' which displays our drawing
        surface onto the view area. we use a double-buffering system which means
        that we have a 'currently displayed' surface, and 'currently being drawn'
        surface. hence the 'swap' idea. in a single-buffering system we would see
        stuff being drawn one-after-the-other */
    while (!glfwWindowShouldClose(window)) {
        /* wipe the drawing surface clear */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_programme);
        glBindVertexArray(VAO[0]);
        /* draw points 0-3 from the currently bound VAO with current in-use shader */
        glDrawArrays(GL_TRIANGLE_FAN, 0, colors.size() * 3);
        /* update other events like input handling */
        glfwPollEvents();
        /* put the stuff we've been drawing onto the display */
        glfwSwapBuffers(window);
    }

    /* close GL context and any other GLFW resources */
    glfwTerminate();
    return 0;
}
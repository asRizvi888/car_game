#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

// global variables
float initial = 0;
float camera = 15.0f;
float carSpeed = 0.25f;
// Car position
float carX = 0.0f;
float carY = 0.0f;
// state variables
bool paused = false;
int GAME_OVER = 0;

// reshape function
void reshape(GLFWwindow *window, int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float h2wRatio = (float)h / (float)w;
    float w2hRatio = (float)w / (float)h;

    if (w <= h)
    {
        glOrtho(0.0, 60.0, 0.0, 80.0 * h2wRatio, -1.0, 1.0);
    }
    else
    {
        glOrtho(0.0, 60.0 * w2hRatio, 0.0, 80.0, -1.0, 1.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// print "Game Over" ascii art
void printASCII(string filename)
{
    string line = "";
    ifstream file;
    file.open(filename);

    if (file.is_open())
    {
        while (getline(file, line))
        {
            cout << line << endl;
        }
    }
    else
    {
        cout << "GAME OVER" << endl;
    }
}

// update myCar position based on animated path
void updateCarPosition()
{
    camera -= carSpeed;
    if (camera < -200) // here 200 is max thershold
    {
        camera = 5;
    }
}

// keyboard callback function
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_RELEASE)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            exit(0);
            break;
        case GLFW_KEY_F:
            // glfwMaximizeWindow(window);
            glfwSetWindowSize(window, 600, 800);
            break;
        case GLFW_KEY_M:
            glfwSetWindowSize(window, 300, 400);
            break;
        case GLFW_KEY_A:
            if (carX > -13 && !paused)
            {
                carX -= (carSpeed / 0.25);
            }
            break;
        case GLFW_KEY_S:
            if (carY > 0 && !paused)
            {
                carY -= (carSpeed / 0.25);
            }
            break;
        case GLFW_KEY_W:
            if (carY < 60 && !paused)
            {
                carY += (carSpeed / 0.25);
            }
            break;
        case GLFW_KEY_D:
            if (carX < 13 && !paused)
            {
                carX += (carSpeed / 0.25);
            }
            break;
        case GLFW_KEY_LEFT_SHIFT:
            if (carSpeed <= 0.75 && !paused)
            {
                carSpeed += 0.25;
            }
            break;
        case GLFW_KEY_TAB:
            if (carSpeed > 0.25 && !paused)
            {
                carSpeed -= 0.25;
            }
            break;
        case GLFW_KEY_SPACE:
            paused = !paused;
            break;
        default:
            break;
        }
    }
}

// displays game background
void draw()
{
    // background
    glColor3ub(48, 48, 48);
    glRectf(0, 0, 60, 80);

    // sidegreen
    glColor3ub(23, 113, 20);
    glRectf(0, 0, 7, 80);
    glRectf(60, 0, 53, 80);

    // pavement
    glColor3ub(90, 90, 90);
    glRectf(7, 0, 12, 80);
    glRectf(53, 0, 48, 80);

    // yellowline
    glColor3ub(255, 255, 0);
    glRectf(12, 0, 12.5, 80);
    glRectf(48, 0, 48.5, 80);
}

// draw opponent car at varriable positions with colors
void opponentCar(float x = 0, float y = 0, string color = "white")
{
    // car-body

    // dynamic color switching
    if (color == "black")
        glColor3ub(0, 0, 0);
    else if (color == "red")
        glColor3ub(255, 0, 0);
    else if (color == "green")
        glColor3ub(0, 255, 0);
    else if (color == "blue")
        glColor3ub(0, 0, 255);
    else
        glColor3ub(255, 255, 255);

    float diffY = carY - camera + 15; // vertical clamp

    // condiotion for game over
    if (((x == -12 && carX < -6 && y <= diffY) ||
         (x == 0 && x == carX && y == diffY) ||
         (x == 12 && carX > 6 && y <= diffY)))
    {
        paused = true;
        // change state
        if (GAME_OVER == 0)
        {
            GAME_OVER = 1;
        }
    }

    if (GAME_OVER == 1)
    {
        printASCII("ascii_art.txt");
        GAME_OVER = 2; // deadlock
    }

    // car body
    glBegin(GL_POLYGON);
    glVertex2d(26.75 + x, 17 + y);
    glVertex2d(26.5 + x, 11 + y);
    glVertex2d(26.5 + x, 9 + y);
    glVertex2d(27 + x, 2 + y);

    glVertex2d(32 + x, 2 + y);
    glVertex2d(32.5 + x, 9 + y);
    glVertex2d(32.5 + x, 11 + y);
    glVertex2d(32.25 + x, 17 + y);
    glEnd();

    // windshield
    glColor3ub(185, 233, 252);

    glBegin(GL_POLYGON);
    glVertex2d(27 + x, 6 + y);
    glVertex2d(27.5 + x, 10 + y);
    glVertex2d(31.5 + x, 10 + y);
    glVertex2d(32 + x, 6 + y);
    glEnd();

    // light
    glColor3ub(240, 240, 20);

    glBegin(GL_POLYGON);
    glVertex2d(26.75 + x, 2 + y);
    glVertex2d(28.5 + x, 2 + y);
    glVertex2d(28.5 + x, 3 + y);
    glVertex2d(26.65 + x, 3 + y);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(32.25 + x, 2 + y);
    glVertex2d(30.35 + x, 2 + y);
    glVertex2d(30.35 + x, 3 + y);
    glVertex2d(32.35 + x, 3 + y);
    glEnd();

    // roof
    glColor3ub(0, 0, 0);

    glBegin(GL_POLYGON);
    glVertex2d(27.5 + x, 10.5 + y);
    glVertex2d(27 + x, 15.5 + y);
    glVertex2d(32 + x, 15.5 + y);
    glVertex2d(31.5 + x, 10.5 + y);
    glEnd();
}

// white-line(s)
void drawMotionPath(int n)
{
    glTranslatef(0.0, camera, 0.0f);

    glColor3ub(255, 255, 255);

    for (int i = 15; i < n * 20; i += 20)
    {
        glRectf(23, i, 24, i - 10);
        glRectf(35, i, 36, i - 10);
    }

    // position opponent cars in different location
    opponentCar(0, 140, "black");
    opponentCar(-12, 100, "blue");
    // opponentCar(12, 150, "green");
    opponentCar(12, 180, "green");
}

// driver car
void myCar()
{
    glTranslatef(carX, carY - camera, 0.0f);

    // car-body
    glColor3ub(255, 48, 28);

    glBegin(GL_POLYGON);
    glVertex2d(26.75, 17);
    glVertex2d(26.5, 11);
    glVertex2d(26.5, 9);
    glVertex2d(27, 2);

    glVertex2d(32, 2);
    glVertex2d(32.5, 9);
    glVertex2d(32.5, 11);
    glVertex2d(32.25, 17);
    glEnd();

    // windshield
    glColor3ub(185, 233, 252);

    glBegin(GL_POLYGON);
    glVertex2d(27, 12);
    glVertex2d(27.5, 9);
    glVertex2d(31.5, 9);
    glVertex2d(32, 12);
    glEnd();

    // light
    glColor3ub(240, 240, 20);

    glBegin(GL_POLYGON);
    glVertex2d(26.75, 17);
    glVertex2d(28.5, 17);
    glVertex2d(28.5, 16);
    glVertex2d(26.65, 16);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2d(32.25, 17);
    glVertex2d(30.35, 17);
    glVertex2d(30.35, 16);
    glVertex2d(32.35, 16);
    glEnd();

    // roof
    glColor3ub(0, 0, 0);

    glBegin(GL_POLYGON);
    glVertex2d(27.5, 8.75);
    glVertex2d(27, 3.5);
    glVertex2d(32, 3.5);
    glVertex2d(31.5, 8.75);
    glEnd();
}

int main(int argc, char **argv)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // turns off resizeable window

    window = glfwCreateWindow(600, 800, "Car Game", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // glfwSetFramebufferSizeCallback(window, reshape);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 1.0);

        /* Set the modelview matrix */
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        /* Set the projection matrix */
        glOrtho(0.0, 60.0, 0.0, 80.0, -1.0, 1.0);

        /* this function deals ith drawing */
        draw();
        drawMotionPath(14);

        if (!paused)
        {
            updateCarPosition();
        }
        myCar();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
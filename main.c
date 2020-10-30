#define GLFW_NO_INCLUDE
#include <GLFW/glfw3.h>
#include <vinox/vinox.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BUNNIES 80000

typedef struct Bunny {
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Vector4 color;
} Bunny;

int generateRandoms(int lower, int upper) {
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    GLFWwindow *window;
    glfwInit();
    window = glfwCreateWindow(screenWidth, screenHeight, "Vinox - bunnymark", NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    vinoxInit(screenWidth, screenHeight);

    // Load bunny texture
    Texture texBunny;
    vinoxLoadTexture("wabbit_alpha.png", &texBunny, GL_LINEAR);
    
    Bunny bunnies[MAX_BUNNIES];

    int bunniesCount = 0;

    double previousTime = glfwGetTime();
    int frameCount = 0;
    
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
            
            double currentTime = glfwGetTime();
            frameCount++;

            if (currentTime - previousTime >= 1.0) {
                printf("Frames FPS: %i\n", frameCount);
                printf("Mili second time: %f\n", 1000.0/(double)(frameCount));

                frameCount = 0;
                previousTime = currentTime;
            }
            
            srand(time(0));

            for (int i = 0; i < MAX_BUNNIES; i++)
            {
                if (bunniesCount < MAX_BUNNIES)
                {
                    bunnies[bunniesCount].position = (Vector2) { 0.0f, 50.0f };
                    bunnies[bunniesCount].speed.x = generateRandoms(-250,250)/60.0f;
                    bunnies[bunniesCount].speed.y = generateRandoms(-250,250)/60.0f;
                    if(generateRandoms(0, 1) == 0) {
                    bunnies[bunniesCount].color = (Vector4) { 0.0f, 1.0f, (float)generateRandoms(5, 10)/10, 1.0f };
                    } else {
                    bunnies[bunniesCount].color = (Vector4) { 0.8f, 0.0f, (float)generateRandoms(6, 10)/10, 1.0f };
                    }
                    bunniesCount++;
                }
            }

        // Update bunnies
        for (int i = 0; i < bunniesCount; i++)
        {
            bunnies[i].position.x += bunnies[i].speed.x;
            bunnies[i].position.y += bunnies[i].speed.y;

            if (((bunnies[i].position.x + texBunny.width/2) > width) ||
                ((bunnies[i].position.x + texBunny.width/2) < 0)) bunnies[i].speed.x *= -1;
            if (((bunnies[i].position.y + texBunny.height/2) > height) || 
                    ((bunnies[i].position.y - texBunny.height/2) < 0)) bunnies[i].speed.y *= -1;
        }
        
        vinoxBeginDrawing(width, height);

            vinoxClear(WHITE);

            for (int i = 0; i < bunniesCount; i++)
            {
                vinoxCreateQuad((Quad) { bunnies[i].position, (Vector2) {texBunny.width, texBunny.height} }, EMPTYQUAD, texBunny.id, (Vector2) { texBunny.width, texBunny.height }, bunnies[i].color, 0.0f);
            }


        vinoxEndDrawing();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

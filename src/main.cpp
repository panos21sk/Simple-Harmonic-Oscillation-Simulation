#include "raylib.h"
#include "raymath.h"
#include "stdlib.h"
#include "stdio.h"
#include "util.h"
#include <vector>
#include "math.h"

#define SCRWIDTH 1200
#define SCRHEIGHT 800
#define FPS 60

//extra structs
typedef struct{
    float xEP, xNL, u, a, TF, KE, UE;
} physicsVals;
typedef struct{
    float A, K, m, t;
    bool paused;
    char* surfaceType;
}state;
//extra functions
void DrawDottedLines(int xi, int yi, int xf, int yf, Color color);
physicsVals OscilatorSimulation(float A, float K, float m, char *surfaceType);


state simState = (state){A: 300, K: 10, m: 1, t: 0, paused: false, surfaceType: "V"};


void draw(){
    //draw
    BeginDrawing();
        //statics
        DrawRing((Vector2){SCRWIDTH/2, SCRHEIGHT/2}, SCRWIDTH/4, SCRWIDTH/4 + 5, 0, 360, 1, RED);
        DrawDottedLines(SCRWIDTH/2 - SCRWIDTH/4, SCRHEIGHT/2, SCRWIDTH/2 + SCRWIDTH/4, SCRHEIGHT/2, RED);
        DrawDottedLines(SCRWIDTH/2, SCRHEIGHT/2 + SCRWIDTH/4, SCRWIDTH/2, SCRHEIGHT/2 - SCRWIDTH/4, BLUE);
        DrawText("Equilibrium position", SCRWIDTH/2 + SCRWIDTH/4 + 10, SCRHEIGHT/2, 18, RED);
        ClearBackground(RAYWHITE);

        //dynamics
        DrawFPS(0, 0);
        physicsVals osc = OscilatorSimulation(simState.A, simState.K, simState.m, "V");
        DrawCircle(SCRWIDTH/2, osc.xEP, 10, BLACK); //oscillator
        DrawLineEx((Vector2){SCRWIDTH/2, SCRHEIGHT/2}, (Vector2){SCRWIDTH/2 + simState.A * cos(sqrt(simState.K / simState.m) * simState.t), osc.xEP}, 5.0, BLACK); 


    EndDrawing();
}

physicsVals OscilatorSimulation(float A, float K, float m, char *surfaceType){
    float omega = sqrt(K/m);
    if (surfaceType == "V"){
        float d = m*10/K;
    }
    float xEP = SCRHEIGHT/2 - (A * sin(omega * simState.t));
    return (physicsVals){xEP, xEP, xEP, xEP, xEP, xEP, xEP};
}

int main(void)
{   // initialization
    InitWindow(SCRWIDTH, SCRHEIGHT, "Simple harmonic oscillator simulation by Panagiotis Skoulis");
    SetTargetFPS(FPS); //sets framerate
    bool paused = false;

    //init

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        //updating variables
        if(!paused){
            simState.t += GetFrameTime();
            draw();    
        }    
    }

    CloseWindow();        // Close window and OpenGL context
    return 0;
}




//Extra Functions Defined
void DrawDottedLines(int xi, int yi, int xf, int yf, Color color){
    if ((xf - xi) != 0){
        float L = (yf - yi) / (xf - xi);
        float currentX = xi;
        float currentY = yi;
        float dx = 10; float dy = dx*L;
        while (currentX + dx < xf && currentY + dy <= yf){
            DrawLineEx((Vector2){currentX, currentY}, (Vector2){currentX + dx, currentY + dy}, 5.0, color);
            currentX += dx * 2;
            currentY += dy * 2;
        }
    }
    else {
        float currentY = yi;
        float dy = 10;
        while (currentY > yf){
            DrawLineEx((Vector2){(float)xi, currentY}, (Vector2){(float)xi, currentY - dy}, 5.0, color);
            currentY -= dy * 2;
        }
    }
}
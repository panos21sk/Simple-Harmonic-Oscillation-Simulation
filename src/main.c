#include "raylib.h"
#include "raymath.h"
#include "stdlib.h"
#include "stdio.h"
#include "util.h"
#include "math.h"

#define SCRWIDTH 1200
#define SCRHEIGHT 800
#define FPS 60

//extra structs
typedef struct{
    float xEP, xNL, u, a, TF, KE, UE, TE;
} physicsVals;
typedef struct{
    float A, K, m, t;
    bool paused;
    char* surfaceType;
}state;
//extra functions
void DrawDottedLines(int xi, int yi, int xf, int yf, Color color);
physicsVals OscilatorSimulation(float A, float K, float m, char *surfaceType);
void draw();


state simState = (state){A: 300, K: 10, m: 1, t: 0, paused: false, surfaceType: "V"};

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

void draw(){
    //draw
    BeginDrawing();
        //statics
        DrawRing((Vector2){SCRWIDTH/2, SCRHEIGHT/2}, simState.A, simState.A + 5, 0, 360, 1, RED);
        DrawDottedLines(SCRWIDTH/2 - simState.A, SCRHEIGHT/2, SCRWIDTH/2 + simState.A, SCRHEIGHT/2, RED);
        DrawDottedLines(SCRWIDTH/2, SCRHEIGHT/2 + simState.A, SCRWIDTH/2, SCRHEIGHT/2 - simState.A, BLUE);
        DrawText("Equilibrium position", SCRWIDTH/2 + simState.A + 10, SCRHEIGHT/2, 18, RED);
        ClearBackground(RAYWHITE);

        //dynamics
        DrawFPS(0, 0);
        physicsVals osc = OscilatorSimulation(simState.A, simState.K, simState.m, "V");
        DrawCircle(SCRWIDTH/2, SCRHEIGHT/2 - osc.xEP, 10, BLACK); //oscillator
        DrawLineEx((Vector2){SCRWIDTH/2, SCRHEIGHT/2}, (Vector2){SCRWIDTH/2 + simState.A * cos(sqrt(simState.K / simState.m) * simState.t), SCRHEIGHT/2 - osc.xEP}, 5.0, BLACK); //trig circle
        DrawDottedLines(SCRWIDTH/2, SCRHEIGHT/2 - osc.xEP, (SCRWIDTH/2 + simState.A * cos(sqrt(simState.K / simState.m) * simState.t)), SCRHEIGHT/2 - osc.xEP, BLACK);

        char* values;
        asprintf(&values, "xEP: %f m \nxNL: %f m\nu: %f m/s \na: %f m/s^2 \nTF: %f N\nKE: %f J\nUE: %f J \nTE: %fJ ", osc.xEP, osc.xNL, osc.u, osc.a, osc.TF, osc.KE, osc.UE, osc.TE);
        DrawText(values, 5, SCRHEIGHT - 18 * 8 * 3/2, 18, BLACK);
        free(values);


    EndDrawing();
}

physicsVals OscilatorSimulation(float A, float K, float m, char *surfaceType){
    float omega = sqrt(K/m);
    float d;
    if (surfaceType == "V"){
        d = m*10/K;
    }
    float xEP = (A * sin(omega * simState.t));
    float xNL = (d + xEP);
    float u = omega * A * cos(omega * simState.t);
    float a = -omega * omega * A * sin(omega * simState.t);
    float TF = -K * xEP;
    float KE = 1/2 * m * u*u;
    float UE = 1/2 * K * xEP*xEP;
    float TE = 1/2 * K * A*A;


    // float xEP, xNL, u, a, TF, KE, UE;
    return (physicsVals){xEP: xEP,xNL: xNL,u: u,a: a,TF: TF,KE: KE,UE: UE,TE: TE};
}



//Extra Functions Defined
void DrawDottedLines(int xi, int yi, int xf, int yf, Color color){
    if ((xf - xi) != 0){
        float L = (yf - yi) / (xf - xi);
        float currentX = xi;
        float currentY = yi;
        float dx = 10; float dy = dx*L;
        while (currentX + dx < abs(xf) && currentY + dy <= yf){
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
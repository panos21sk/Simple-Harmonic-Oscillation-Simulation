#include "raylib.h"
#include "raymath.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

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

state simStateInit = (state){A:300, K: 10, m: 3,t: 0, paused: false, surfaceType: "V"}; //A edw = A(m) * 100, gia na fenetai
state* simState = &simStateInit;

//extra functions
void DrawDottedLines(int xi, int yi, int xf, int yf, Color color);
physicsVals OscilatorSimulation(float A, float K, float m, char *surfaceType);
void draw();

int main(void)
{   // initialization
    InitWindow(SCRWIDTH, SCRHEIGHT, "Simple harmonic oscillator simulation by Panagiotis Skoulis");
    SetTargetFPS(FPS); //sets framerate

    //init

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        //updating variables
        draw();  

    }

    CloseWindow();        // Close window and OpenGL context
    return 0;
}

void draw(){
    //draw
    BeginDrawing();

        if(!simState->paused){
            simState->t += GetFrameTime();
        }  
        //statics
        DrawRing((Vector2){SCRWIDTH/2, SCRHEIGHT/2}, simState->A, simState->A + 5, 0, 360, 1, RED);
        DrawDottedLines(SCRWIDTH/2 - simState->A, SCRHEIGHT/2, SCRWIDTH/2 + simState->A, SCRHEIGHT/2, RED);
        DrawDottedLines(SCRWIDTH/2, SCRHEIGHT/2 + simState->A, SCRWIDTH/2, SCRHEIGHT/2 - simState->A, BLUE);
        DrawText("Equilibrium position", SCRWIDTH/2 + simState->A + 10, SCRHEIGHT/2 - 9, 18, RED);
        DrawDottedLines(SCRWIDTH/2 - simState->A, SCRHEIGHT/2 - (simState->m * 10 / simState->K) * 100, SCRWIDTH/2 + simState->A, SCRHEIGHT/2 - (simState->m * 10 / simState->K) * 100, GREEN); //natural length pos
        DrawText("Natural Length position", SCRWIDTH/2 + simState->A + 10, SCRHEIGHT/2 - (simState->m * 10 / simState->K) * 100 - 9, 18, GREEN);
        ClearBackground(RAYWHITE);

        char staticVariables[200];
        snprintf(staticVariables, sizeof(staticVariables),"A: %f m \nK: %f N/m \nm: %f kg \nt: %f s \nPaused: %i", simState->A/100, simState->K, simState->m, simState->t, simState->paused);
        DrawText(staticVariables, 5, 24, 18, BLACK);
        //free(staticVariables);

        //dynamics
        DrawFPS(5, 0);
        physicsVals osc = OscilatorSimulation(simState->A, simState->K, simState->m, "V");
        DrawCircle(SCRWIDTH/2, SCRHEIGHT/2 - osc.xEP, 10, BLACK); //oscillator
        DrawLineEx((Vector2){SCRWIDTH/2, SCRHEIGHT/2}, (Vector2){SCRWIDTH/2 + simState->A * cos(sqrt(simState->K / simState->m) * simState->t), SCRHEIGHT/2 - osc.xEP}, 5.0, BLACK); //trig circle
        if(cos(sqrt(simState->K / simState->m) * simState->t)>0){
            DrawDottedLines(SCRWIDTH/2, SCRHEIGHT/2 - osc.xEP, (SCRWIDTH/2 + simState->A * cos(sqrt(simState->K / simState->m) * simState->t)), SCRHEIGHT/2 - osc.xEP, BLACK);
        }
        else if(cos(sqrt(simState->K / simState->m) * simState->t)<0){
            DrawDottedLines(SCRWIDTH/2 + simState->A * cos(sqrt(simState->K / simState->m) * simState->t), SCRHEIGHT/2 - osc.xEP, SCRWIDTH/2, SCRHEIGHT/2 - osc.xEP, BLACK);
        }
        

        char variableValues[200];
        snprintf(variableValues, sizeof(variableValues), "xEP: %f m \nxNL: %f m\nu: %f m/s \na: %f m/s^2 \nTF: %f N\nKE: %f J\nUE: %f J \nTE: %f J", osc.xEP, osc.xNL, osc.u, osc.a, osc.TF, osc.KE, osc.UE, osc.TE);
        DrawText(variableValues, 5, SCRHEIGHT - 18 * 8 * 3/2, 18, BLACK);

        //GUI
        GuiSliderBar((Rectangle){SCRWIDTH - 210, 10, 200, 40}, "A:", NULL, &simState->A, 0, 400);
        GuiSliderBar((Rectangle){SCRWIDTH - 210, 60, 200, 40}, "K:", NULL, &simState->K, 0, 100);
        GuiSliderBar((Rectangle){SCRWIDTH - 210, 110, 200, 40}, "m:", NULL, &simState->m, 0, 100);
        GuiCheckBox((Rectangle){SCRWIDTH - 210, 160, 40, 40}, "Paused:", &simState->paused);

    EndDrawing();
}

physicsVals OscilatorSimulation(float A, float K, float m, char *surfaceType){
    float omega = sqrt(K/m);
    float d;
    if (surfaceType == "V"){
        d = m*10/K;
    }
    float xEP = (A * sin(omega * simState->t));
    float xNL = (d + xEP);
    float u = omega * A * cos(omega * simState->t);
    float a = -omega * omega * A * sin(omega * simState->t);
    float TF = -K * xEP;
    float KE = (m * u*u)/2;
    float UE = (K * xEP*xEP)/2;
    float TE = (K * A*A/100)/2;

    return (physicsVals){xEP: xEP,xNL: xNL,u: u,a: a,TF: TF,KE: KE,UE: UE,TE: TE};
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
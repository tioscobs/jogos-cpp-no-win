#include <iostream>
#include <windows.h>
#include <conio.h>
#include <conmanip.h>
#include <cmath>

#define MOVE_UP 119 // W
#define MOVE_DOWN 115 // S
#define MOVE_UP2 117 // U
#define MOVE_DOWN2 106 // J

using namespace std;
using namespace conmanip;

// DEF VARIABLES
const int SCREEN_WIDTH = 100; // in chars
const int SCREEN_HEIGHT = 20; // in chars
const int RENDER_FRAMERATE = 20; // in milisseconds

// GAMES

class Pong{
    private:
        console_out console;
        int playerX = 6, playerY = 0;
        int player2X = SCREEN_WIDTH - 6, player2Y = 0;
        int playersW = 1, playersH = 5;

    public:
        int ballx = 0, bally = 0;
        int points1 = 0, points2 = 0;
        int ballx_velocity = 1, bally_velocity = 1;

        Pong(int sX, int sY, auto console2){
            ballx = sX, bally = sY;
            playerY = player2Y =  sY - (playersH/2);
            console = console2;
        }

        void move_player(){
            char pressed_key = getch();
            if(pressed_key == MOVE_UP){
                if(playerY - 1 > 0){
                    playerY--;
                }
            }else if(pressed_key == MOVE_DOWN){
                if(playerY + playersH < SCREEN_HEIGHT){
                    playerY++;
                }
            }else if(pressed_key == MOVE_UP2){
                if(player2Y - 1 > 0){
                    player2Y--;
                }
            }else if(pressed_key == MOVE_DOWN2){
                if(player2Y + playersH < SCREEN_HEIGHT){
                    player2Y++;
                }
            }
        }

        void reset_ball(int last_point){
            ballx = SCREEN_WIDTH/2, bally = SCREEN_HEIGHT/2;
            bally_velocity = 1;
            if(last_point == 1){
                ballx_velocity = 1;
            }else if(last_point == 2){
                ballx_velocity = -1;
            }
        }

        void ball_collision(){
            if(ballx - 1 == playerX){
                if(bally >= playerY && bally <= playerY + playersH){
                    ballx_velocity = 1;
                }
            }else if(ballx + 1 == player2X){
                if(bally >= player2Y && bally <= player2Y + playersH){
                    ballx_velocity = -1;
                }
            }
        }

        void render_players(){
            for(int i = 0; i < playersH; i++){
                console.setpos(playerX, playerY + i);
                cout << "$";
                console.setpos(player2X, player2Y + i);
                cout << "$";
            }
        }

        void render_ball(){
            console.setpos(ballx, bally);
            cout << "O";
        }
};

// UTILITY FUNCTIONS

/*
void isColliding(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2){
    if(x1 <= x2 + w2 || x1 + w1 >= x2){
        if(y1 <= y2 + h2 || y1 + h1 >=  y2){
            //Useless for now
        }
    }
}*/

// DO NOT COPY THIS BECAUSE IT IS STILL IN TESTING xD
void drawline(auto point1, auto point2, auto console){
   float distance = sqrt(pow(point1[0] - point2[0], 2) + pow(point1[1] - point2[1], 2));
   float co = abs(point1[0] - point2[0]), ca = abs(point1[1] - point2[1]);
   float tang = co/ca;
   int X, Y, fY;
   for(int i = 0; i < round(distance); i++){
      X = point1[0], Y = point1[1];

      if(point2[0] != point1[0] && point2[1] != point1[1]){
         X = ((point1[1] - point2[1]) < 0) ? X + i : X - i;

         if((point2[0] < point1[0] && point2[1] < point1[1])){ // X2 < 0 && Y2 < 0
            fY = point1[1] - ca;
            Y = fY + ((X - point2[0])/tang);
         }else if((point2[0] > point1[0] && point2[1] > point1[1])){ // X2 > 0 && Y2 > 0
            fY = point1[1] + ca;
            Y = fY + ((X - point2[0])/tang);
         }else{ // Using to test quadrants (X2 > 0 && Y2 < 0) and (X2 < 0 && Y2 > 0)
            Y = fY - ((X - point2[0])/tang);
         }
      }else if(point2[0] == point1[0]){ // X1 == X2
         Y = (point1[1] < point2[1]) ? Y += i : Y -= i;

      }else if(point2[1] == point1[1]){ // Y1 == Y2
         X = (point1[0] < point2[0]) ? X + i : X - i;
      }
      

      console.setpos(X, Y);
      cout << "#";
      //cout << X << " " << Y << endl << endl;
   }
}

int main (){
    console_out_context context_out;
    console_out console(context_out);

    int screen[4][2] = {
        {0, 0},
        {SCREEN_WIDTH, 0},
        {SCREEN_WIDTH, SCREEN_HEIGHT},
        {0, SCREEN_HEIGHT}
    };

    Pong PongGame(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, console);

    // RENDER
    while(true){
        system("cls");
        // Draw screen rectangle
        for(int i = 0; i < 4; i++){
            drawline(screen[i], screen[(i+1) % 4], console);
        }
    
        PongGame.ballx += PongGame.ballx_velocity;
        PongGame.bally += PongGame.bally_velocity;
        
        if(PongGame.ballx + 1 == SCREEN_WIDTH){
            PongGame.reset_ball(1);
            PongGame.points1++;
        }else if(PongGame.ballx - 1 == 0){
            PongGame.reset_ball(2);
            PongGame.points2++;
        }

        if(PongGame.bally + 1 == SCREEN_HEIGHT){
            PongGame.bally_velocity = -1;
        }else if(PongGame.bally - 1 == 0){
            PongGame.bally_velocity = 1;
        }

        if(kbhit()){
            PongGame.move_player();
        }

        PongGame.ball_collision();
        PongGame.render_players();
        PongGame.render_ball();

        console.setpos(0, SCREEN_HEIGHT + 2);
        cout << "PLAYER 1: " << PongGame.points1 << " points" << endl << "PLAYER 2: " << PongGame.points2 << " points";

        Sleep(RENDER_FRAMERATE);
    }
    return 0;
}
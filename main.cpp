#include <iostream>
#include <raylib.h>
#include <raymath.h>


float timecalculator(float time, float timetracker);

/*TODO: Effects on the button
 *      Render the music files
 *      ProgressBar
 *      ScreenWidth customizable
 *
 *
 */

struct Beats {
    Rectangle rectangle;
    Vector2 position;
    Vector2 velocity = {0,1};
    Color color;
};

struct Section {
    Rectangle rectangle; //this one is the big section boxes
    char Button;
    Color color;
    Color background = BLACK;
    Rectangle smallerRecs; //this one is the where it goes to the gray boxes
    Color lettercolor;
    std::string letter;

};

struct ProgressionBar {
    Rectangle rectangle;
    Color color;
};


int main() {
    //Windows
    float screen_width = 900;
    float screen_height = 600;
    float window_width = 800;
    float window_height = 600;
    InitWindow(screen_width, screen_height, "Muzik");
    SetTargetFPS(60);

    //Audio
    InitAudioDevice();
    Music music = LoadMusicStream("demo.mp3");

    // Creating sections
    char num_sections = 7;
    Font font = GetFontDefault();
    Section section[num_sections];

    section[0].Button = KEY_A;
    section[1].Button = KEY_J;
    section[2].Button = KEY_S;
    section[3].Button = KEY_K;
    section[4].Button = KEY_D;
    section[5].Button = KEY_L;
    section[6].Button = KEY_F;

    section[0].letter = 'A';
    section[1].letter = 'J';
    section[2].letter = 'S';
    section[3].letter = 'K';
    section[4].letter = 'D';
    section[5].letter = 'L';
    section[6].letter = 'F';

    section[0].color = GREEN;
    section[1].color = ORANGE;
    section[2].color = RED;
    section[3].color = LIME;
    section[4].color = YELLOW;
    section[5].color = BLUE;
    section[6].color = BEIGE;

    float width_sections = window_width / num_sections;

    float x = 50, y = 0;

    for(int i = 0; i < num_sections; i++) {
        section[i].rectangle = Rectangle{x, y, width_sections, window_height};
        if (i % 2 == 1) {
            section[i].smallerRecs = Rectangle{x,window_height - 75,width_sections,50};
            section[i].lettercolor = RED;
        }
        else {
            section[i].smallerRecs = Rectangle{x,window_height - 50,width_sections,50};
            section[i].lettercolor = BLACK;
        }
        x += window_width/num_sections;
    }

    Beats beat[100];
    float bpm = 70.0f;
    double beat_per_second = bpm/60;
    double second_per_beat = 1/beat_per_second;

    float time_tracker = 0;
    int num_beats = 0;
    float beat_size = 10;

    int streak_num = 0;
    PlayMusicStream(music);


    while(!WindowShouldClose()) {
        //Music Stream
        UpdateMusicStream(music);


        //Spawning the beats at random intervals in different blocks
        char box_selector = GetRandomValue(0,6);
        float time_elapsed = GetFrameTime();

        time_tracker = timecalculator(time_elapsed, time_tracker);

        //time should elapse and be set to 0 each time it spawns
        if (second_per_beat < time_tracker) {
            time_tracker = 0;
            Beats beats = {};
            beats.position = {section[box_selector].rectangle.x,0};
            //color selection
            switch(box_selector) {
                case 0:
                    beats.color = GREEN;
                    break;
                case 1:
                    beats.color = ORANGE;
                    break;
                case 2:
                    beats.color = RED;
                    break;
                case 3:
                    beats.color = LIME;
                    break;
                case 4:
                    beats.color = YELLOW;
                    break;
                case 5:
                    beats.color = BLUE;
                    break;
                case 6:
                    beats.color = BEIGE;
                    break;
                case 7:
                    beats.color = PURPLE;
                    break;
            }
            beat[num_beats++] = beats;
        }

        //music block beats to go downwards
        for (int i = 0; i < num_beats; i++) {
            beat[i].position.y += beat[i].velocity.y;
            //beats at the end bottom they disappear
            if (beat[i].position.y > window_height) {
                Beats temp = beat[i];
                beat[i] = beat[num_beats -1];
                beat[num_beats -1] = temp;
                num_beats--;
                streak_num = 0;
            }
        }

        //Key pressed Action
        for (int i = 0; i < num_sections; i++) {
            if (IsKeyDown(section[i].Button)) {
                //change the color
                section[i].background = section[i].color;
                //check if the smaller boxes hit the beats
                for(int j = 0; j < num_beats; j++) {
                    Rectangle beatchecker = {beat[j].position.x,beat[j].position.y,width_sections,beat_size};
                    if(CheckCollisionRecs(beatchecker, section[i].smallerRecs)) {
                        Beats temp = beat[j];
                        beat[j] = beat[num_beats -1];
                        beat[num_beats -1] = temp;
                        num_beats--;
                        streak_num++;
                    }
                }
            }
            else {
                section[i].background = BLACK;
            }
        }

        //Drawing
        BeginDrawing();
        ClearBackground(GRAY);
        for(int i = 0; i < num_sections; i++) {
            DrawRectanglePro(section[i].rectangle,{0,0},0,section[i].background);
            DrawRectanglePro(section[i].smallerRecs,{0,0},0,LIGHTGRAY);
            DrawText(section[i].letter.c_str(), section[i].smallerRecs.x + 50,section[i].smallerRecs.y + 20,20,section[i].lettercolor);
            DrawRectangleLinesEx(section[i].rectangle, 3,GRAY);
        }

        //Streak Records
        std::string streak = "Streak: " + std::to_string(streak_num);
        DrawText(streak.c_str(), 10, 35, 20, RED);

        //Drawing beats
        for (int i = 0; i < num_beats; i++) {
            DrawRectangle(beat[i].position.x,beat[i].position.y,width_sections,beat_size,beat[i].color);
        }
        EndDrawing();
    }
    UnloadMusicStream(music);
    CloseAudioDevice();
    return 0;
}

float timecalculator(float time,float timetracker) {
    timetracker += time;
    return timetracker;
}

#include <SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "Zoo";
int xxx = 0;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

const SDL_Color GREEN_COLOR = { 0, 255, 0, 255 };
const SDL_Color YELLOW_COLOR = { 255, 255, 0, 255 };
const SDL_Color GRAY_COLOR = { 169, 169, 169, 255 };
const SDL_Color BLACK_COLOR = { 0, 0, 0, 255 };
const SDL_Color ELEPHANT_COLOR = { 160, 160, 160, 255 };

const int WALK_ZONE_WIDTH = SCREEN_WIDTH / 2;
const int WALK_ZONE_HEIGHT = SCREEN_HEIGHT;

const int FEED_ZONE_WIDTH = SCREEN_WIDTH / 4;
const int FEED_ZONE_HEIGHT = SCREEN_HEIGHT / 4;

const int BUTTON_WIDTH = 30;
const int BUTTON_HEIGHT = 30;

const float MOVEMENT_SPEED = 1.9f;
const int TIME_TO_MOVE = 10000;

const int MAX_SQUARES = 3;
int createdSquares = 0;
enum class SquareState
{
    Walking,
    MovingToFeed,
    MovingToWater,
    MovingToWalk,
};


struct Square
{
    SDL_Rect rect;
    SDL_Color color;
    bool isInWalkZone;
    bool isInFeedZone;
    bool isInWaterZone;
    Uint32 startTime;
    SquareState state;
    int targetX;
    int targetY;
};

void updateSquareState(Square& square, Uint32 currentTime)
{
    if (square.state == SquareState::Walking)
    {
        if (currentTime - square.startTime >= TIME_TO_MOVE)
        {
            square.state = SquareState::MovingToFeed;
            square.targetX = SCREEN_WIDTH - FEED_ZONE_WIDTH + rand() % (FEED_ZONE_WIDTH - square.rect.w -2);
            square.targetY = SCREEN_HEIGHT - FEED_ZONE_HEIGHT + rand() % (FEED_ZONE_HEIGHT - square.rect.h - 60);
            square.startTime = currentTime;
        }
    }
    else if (square.state == SquareState::MovingToFeed)
    {
        if (square.rect.x == square.targetX && square.rect.y == square.targetY)
        {
            if (currentTime - square.startTime >= 3000) // 3 секунды
            {
                square.state = SquareState::MovingToWater;
                square.targetX = SCREEN_WIDTH - FEED_ZONE_WIDTH + rand() % (FEED_ZONE_WIDTH - square.rect.w - 2);
                square.targetY = 0 + rand() % (FEED_ZONE_HEIGHT - square.rect.h-1);
                square.startTime = currentTime;
            }
        }
    }
    else if (square.state == SquareState::MovingToWater)
    {
        if (square.rect.x == square.targetX && square.rect.y == square.targetY)
        {
            if (currentTime - square.startTime >= 3000) // 3 секунды
            {
                square.state = SquareState::MovingToWalk;
                square.targetX = rand() % (WALK_ZONE_WIDTH - square.rect.w) -2;
                square.targetY = rand() % (WALK_ZONE_HEIGHT - square.rect.h) -60;
                square.startTime = currentTime;
            }
        }
    }
    else if (square.state == SquareState::MovingToWalk)
    {
        if (square.rect.x == square.targetX && square.rect.y == square.targetY)
        {
            square.state = SquareState::Walking;
            square.startTime = currentTime;
        }
    }
}
void moveSquare(Square& square, Uint32 deltaTime)
{
    if (square.state == SquareState::Walking)
    {
        if (xxx % 4 == 0) {
            square.rect.x += 1;
        }
        else if (xxx % 4 == 1) {
            square.rect.y -= 1;
        }
        else if (xxx % 4 == 2) {
            square.rect.x -= 1;
        }
        else if (xxx % 4 == 3) {
            square.rect.y += 1;
        }
        xxx++;
        if (square.rect.x > WALK_ZONE_WIDTH - square.rect.w)
        {
            square.rect.x = 0;
        }
    }
    else
    {
        int dx = square.targetX - square.rect.x;
        int dy = square.targetY - square.rect.y;
        int distance = std::sqrt(dx * dx + dy * dy);

        if (distance > 0)
        {
            float speed = MOVEMENT_SPEED;
            float deltaX = (dx * speed * deltaTime) / distance;
            float deltaY = (dy * speed * deltaTime) / distance;

            std::cout << "dx: " << dx << ", dy: " << dy << ", distance: " << distance << ", speed: " << speed << ", deltaX: " << deltaX << ", deltaY: " << deltaY << std::endl;

            square.rect.x += static_cast<int>(deltaX);
            square.rect.y += static_cast<int>(deltaY);
        }
    }
}

bool initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void closeSDL()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawBackground()
{
    SDL_SetRenderDrawColor(renderer, GREEN_COLOR.r, GREEN_COLOR.g, GREEN_COLOR.b, GREEN_COLOR.a);
    SDL_RenderClear(renderer);
}

void drawZones()
{
    SDL_Rect walkZone = { 0, 0, WALK_ZONE_WIDTH, WALK_ZONE_HEIGHT };
    SDL_SetRenderDrawColor(renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, BLACK_COLOR.a);
    SDL_RenderDrawRect(renderer, &walkZone);

    SDL_Rect feedZone = { SCREEN_WIDTH - FEED_ZONE_WIDTH, SCREEN_HEIGHT - FEED_ZONE_HEIGHT, FEED_ZONE_WIDTH, FEED_ZONE_HEIGHT };
    SDL_SetRenderDrawColor(renderer, ELEPHANT_COLOR.r, ELEPHANT_COLOR.g, ELEPHANT_COLOR.b, ELEPHANT_COLOR.a);
    SDL_RenderDrawRect(renderer, &feedZone);

    SDL_Rect waterZone = { SCREEN_WIDTH - FEED_ZONE_WIDTH, 0, FEED_ZONE_WIDTH, FEED_ZONE_HEIGHT };
    SDL_SetRenderDrawColor(renderer, GRAY_COLOR.r, GRAY_COLOR.g, GRAY_COLOR.b, GRAY_COLOR.a);
    SDL_RenderDrawRect(renderer, &waterZone);
}

void drawButtons()
{
    SDL_Rect yellowButton = { 10, SCREEN_HEIGHT - BUTTON_HEIGHT - 10, BUTTON_WIDTH, BUTTON_HEIGHT };
    SDL_SetRenderDrawColor(renderer, YELLOW_COLOR.r, YELLOW_COLOR.g, YELLOW_COLOR.b, YELLOW_COLOR.a);
    SDL_RenderFillRect(renderer, &yellowButton);

    SDL_Rect elephantButton = { 50, SCREEN_HEIGHT - BUTTON_HEIGHT - 10, BUTTON_WIDTH, BUTTON_HEIGHT };
    SDL_SetRenderDrawColor(renderer, ELEPHANT_COLOR.r, ELEPHANT_COLOR.g, ELEPHANT_COLOR.b, ELEPHANT_COLOR.a);
    SDL_RenderFillRect(renderer, &elephantButton);

    SDL_Rect blackButton = { 90, SCREEN_HEIGHT - BUTTON_HEIGHT - 10, BUTTON_WIDTH, BUTTON_HEIGHT };
    SDL_SetRenderDrawColor(renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, BLACK_COLOR.a);
    SDL_RenderFillRect(renderer, &blackButton);

    SDL_Rect yellowButtonMinus = { SCREEN_WIDTH - 40, SCREEN_HEIGHT - BUTTON_HEIGHT - 10, BUTTON_WIDTH, BUTTON_HEIGHT };
    SDL_SetRenderDrawColor(renderer, YELLOW_COLOR.r, YELLOW_COLOR.g, YELLOW_COLOR.b, YELLOW_COLOR.a);
    SDL_RenderFillRect(renderer, &yellowButtonMinus);

    SDL_Rect elephantButtonMinus = { SCREEN_WIDTH - 80, SCREEN_HEIGHT - BUTTON_HEIGHT - 10, BUTTON_WIDTH, BUTTON_HEIGHT };
    SDL_SetRenderDrawColor(renderer, ELEPHANT_COLOR.r, ELEPHANT_COLOR.g, ELEPHANT_COLOR.b, ELEPHANT_COLOR.a);
    SDL_RenderFillRect(renderer, &elephantButtonMinus);

    SDL_Rect blackButtonMinus = { SCREEN_WIDTH - 120, SCREEN_HEIGHT - BUTTON_HEIGHT - 10, BUTTON_WIDTH, BUTTON_HEIGHT };
    SDL_SetRenderDrawColor(renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, BLACK_COLOR.a);
    SDL_RenderFillRect(renderer, &blackButtonMinus);
}

void drawSquare(const Square& square)
{
    SDL_SetRenderDrawColor(renderer, square.color.r, square.color.g, square.color.b, square.color.a);
    SDL_RenderFillRect(renderer, &square.rect);
}

bool isInsideZone(const SDL_Rect& rect, const SDL_Rect& zone)
{
    return rect.x >= zone.x && rect.x + rect.w <= zone.x + zone.w &&
        rect.y >= zone.y && rect.y + rect.h <= zone.y + zone.h;
}

bool areColorsEqual(const SDL_Color& color1, const SDL_Color& color2)
{
    return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b && color1.a == color2.a;
}

bool initSquares(std::vector<Square>& squares)
{
    for (int i = 0; i < 5; ++i)
    {
        int squareX = rand() % (WALK_ZONE_WIDTH - 20);
        int squareY = rand() % (WALK_ZONE_HEIGHT - 20);
        Square newSquare = { SDL_Rect{squareX, squareY, 20, 20}, YELLOW_COLOR, true, false, false, 0, SquareState::Walking, 0, 0 };
        squares.push_back(newSquare);
    }

    return true;
}

int main(int argc, char* args[])
{
    if (!initSDL())
    {
        return -1;
    }

    srand(static_cast<unsigned>(time(nullptr)));

    std::vector<Square> squares;

    Uint32 lastMoveTime = SDL_GetTicks();

    bool quit = false;
    SDL_Event e;
    int created_black = 0;
    int created_yeallow = 0;
    int created_elefhant = 0;
    while (!quit)
    {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 deltaTime = currentTime - lastMoveTime;

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                if (mouseX >= 10 && mouseX <= 10 + BUTTON_WIDTH && mouseY >= SCREEN_HEIGHT - BUTTON_HEIGHT - 10 && mouseY <= SCREEN_HEIGHT - 10)
                {
                    if (createdSquares < MAX_SQUARES)
                    {
                        Square newSquare = { SDL_Rect{rand() % (WALK_ZONE_WIDTH - 20), rand() % (WALK_ZONE_HEIGHT - 20), 20, 20}, YELLOW_COLOR, true, false, false, 0, SquareState::Walking, 0, 0 };
                        squares.push_back(newSquare);
                        createdSquares++;
                        created_yeallow++;
                    }
                }
                else if (mouseX >= 50 && mouseX <= 50 + BUTTON_WIDTH && mouseY >= SCREEN_HEIGHT - BUTTON_HEIGHT - 10 && mouseY <= SCREEN_HEIGHT - 10)
                {
                    if (createdSquares < MAX_SQUARES)
                    {
                        Square newSquare = { SDL_Rect{rand() % (WALK_ZONE_WIDTH - 20), rand() % (WALK_ZONE_HEIGHT - 20), 20, 20}, ELEPHANT_COLOR, true, false, false, 0, SquareState::Walking, 0, 0 };
                        squares.push_back(newSquare);
                        createdSquares++;
                        created_elefhant++;
                    }
                }
                else if (mouseX >= 90 && mouseX <= 90 + BUTTON_WIDTH && mouseY >= SCREEN_HEIGHT - BUTTON_HEIGHT - 10 && mouseY <= SCREEN_HEIGHT - 10)
                {
                    if (createdSquares < MAX_SQUARES)
                    {
                        Square newSquare = { SDL_Rect{rand() % (WALK_ZONE_WIDTH - 20), rand() % (WALK_ZONE_HEIGHT - 20), 20, 20}, BLACK_COLOR, true, false, false, 0, SquareState::Walking, 0, 0 };
                        squares.push_back(newSquare);
                        createdSquares++;
                        created_black++;
                    }
                
                }
                else if (mouseX >= SCREEN_WIDTH - 40 && mouseX <= SCREEN_WIDTH - 40 + BUTTON_WIDTH && mouseY >= SCREEN_HEIGHT - BUTTON_HEIGHT - 10 && mouseY <= SCREEN_HEIGHT - 10)
                {
                    squares.erase(std::remove_if(squares.begin(), squares.end(), [&](const Square& square) { return areColorsEqual(square.color, YELLOW_COLOR); }), squares.end());
                    createdSquares = createdSquares - created_yeallow;
                    created_yeallow = 0;
                }
                else if (mouseX >= SCREEN_WIDTH - 80 && mouseX <= SCREEN_WIDTH - 80 + BUTTON_WIDTH && mouseY >= SCREEN_HEIGHT - BUTTON_HEIGHT - 10 && mouseY <= SCREEN_HEIGHT - 10)
                {
                    squares.erase(std::remove_if(squares.begin(), squares.end(), [&](const Square& square) { return areColorsEqual(square.color, ELEPHANT_COLOR); }), squares.end());
                    createdSquares = createdSquares - created_elefhant;
                    created_elefhant = 0;
                }
                else if (mouseX >= SCREEN_WIDTH - 120 && mouseX <= SCREEN_WIDTH - 120 + BUTTON_WIDTH && mouseY >= SCREEN_HEIGHT - BUTTON_HEIGHT - 10 && mouseY <= SCREEN_HEIGHT - 10)
                {
                    squares.erase(std::remove_if(squares.begin(), squares.end(), [&](const Square& square) { return areColorsEqual(square.color, BLACK_COLOR); }), squares.end());
                    createdSquares = createdSquares - created_black;
                    created_black = 0;
                }
            }
        }

        for (auto& square : squares)
        {
            updateSquareState(square, currentTime);
            moveSquare(square, deltaTime);
        }

        drawBackground();
        drawZones();
        drawButtons();

        for (const auto& square : squares)
        {
            drawSquare(square);
        }

        SDL_RenderPresent(renderer);

        lastMoveTime = currentTime;
    }

    closeSDL();

    return 0;
}

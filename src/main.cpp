#include "./game/game.h"

int main(int argc, char *argv[]) {
    Game game;

    game.Init();
    game.Run();
    game.Destory();

    return 0;
}

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include "GuiGame.h"
#include "GameView.h"

int main(int argc, char** argv) {
    const int cellPx = 32;
    const int winW = static_cast<int>(GuiGame::COLS * cellPx);
    const int winH = static_cast<int>(GuiGame::ROWS * cellPx + 60);

    Fl_Double_Window win(winW, winH, "Castle Defender - FLTK");

    Fl_Box status(10, 10, winW - 20, 20, "");
    status.labelfont(FL_HELVETICA);
    status.labelsize(12);
    status.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    status.box(FL_NO_BOX);

    GuiGame engine;
    GameView view(0, 40, winW, winH - 40, engine.grid(), cellPx);
    engine.attachView(&view);
    engine.attachStatus(&status);

    Fl_Button btnNextWave(10, 10 + 24, 100, 22, "Start Wave");
    Fl_Button btnNext(120, 10 + 24, 90, 22, "Next Turn");
    Fl_Button btnStart(215, 10 + 24, 90, 22, "Start Auto");
    Fl_Button btnStop(310, 10 + 24, 90, 22, "Stop Auto");
    Fl_Button btnUDmg(405, 10 + 24, 90, 22, "Upgrade Dmg");
    Fl_Button btnURng(500, 10 + 24, 90, 22, "Upgrade Rng");

    btnNextWave.callback([](Fl_Widget*, void* ud){
        auto* e = static_cast<GuiGame*>(ud);
        e->startNextWave();
    }, &engine);

    btnNext.callback([](Fl_Widget*, void* ud){
        auto* e = static_cast<GuiGame*>(ud);
        e->nextButtonAction();
    }, &engine);

    btnStart.callback([](Fl_Widget*, void* ud){
        auto* e = static_cast<GuiGame*>(ud);
        e->startAuto(1.0);
    }, &engine);

    btnStop.callback([](Fl_Widget*, void* ud){
        auto* e = static_cast<GuiGame*>(ud);
        e->stopAuto();
    }, &engine);

    btnUDmg.callback([](Fl_Widget*, void* ud){
        auto* e = static_cast<GuiGame*>(ud);
        e->stopAuto();
        e->setUpgradeTypePublic(false); // damage
    }, &engine);

    btnURng.callback([](Fl_Widget*, void* ud){
        auto* e = static_cast<GuiGame*>(ud);
        e->stopAuto();
        e->setUpgradeTypePublic(true); // range
    }, &engine);

    view.setOnClick([&](std::size_t r, std::size_t c){
        return engine.onCellClick(r, c);
    });

    win.end();
    win.show(argc, argv);
    return Fl::run();
}

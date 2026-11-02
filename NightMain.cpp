#include "plugin.h"
#include "common.h"
#include "CWeather.h"
#include "CClock.h"
#include <windows.h>

using namespace plugin;

static bool nightMode = false;
static bool keyWasDown = false;
static int savedWeather = 1;

static void EndNight() {
    nightMode = false;
    CWeather::ForcedWeatherType = -1;
    CWeather::NewWeatherType = savedWeather;
    CWeather::OldWeatherType = savedWeather;
}

static void NightDraw() {
    bool down = (GetAsyncKeyState(VK_F7) & 0x8000) != 0;
    if (down && !keyWasDown) {
        if (nightMode) {
            EndNight();
        } else if (CWeather::ForcedWeatherType == -1) {
            int w = CWeather::NewWeatherType;
            savedWeather = (w >= 0 && w < 21) ? w : 1;
            nightMode = true;
            CClock::ms_nGameClockHours = 19;
            CClock::ms_nGameClockMinutes = 0;
            CClock::ms_nGameClockSeconds = 0;
        }
    }
    keyWasDown = down;

    if (!nightMode) return;

    if (CClock::ms_nGameClockHours < 19) {
        EndNight();
        return;
    }

    CWeather::ForcedWeatherType = 22;
    CWeather::NewWeatherType = 22;
    CWeather::OldWeatherType = 22;
}

class NightPlugin {
public:
    NightPlugin() {
        Events::drawingEvent += NightDraw;
    }
} nightPlugin;

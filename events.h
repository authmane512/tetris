#ifndef EVENTS_H
#define EVENTS_H

#ifdef __ANDROID__
#include "platforms/android/events.h"
#else
#include "platforms/desktop/events.h"
#endif

#endif
// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.
#pragma once
#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <tchar.h>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <ctime>
#include <cstdlib>            // For atoi()
#include <thread>
#include <chrono>

#include "json.hpp"

#include "framework.h"
#include "common.h"
#include "serialization.h"
#include "network.h"
#include "getters.h"
#include "spawners.h"
#include "movers.h"
#include "test.h"


#endif //PCH_H

/*
 * Copyright (C) 2022 Javier Marrero.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */

/* 
 * File:   Runtime.cpp
 * Author: Javier Marrero
 * 
 * Created on December 9, 2022, 3:16 PM
 */

#include <Axf/Core/Runtime.h>
#include <Axf/Core/NullPointerException.h>
#include <Axf/API/Platform.h>

#ifdef ARTEMIS_PLATFORM_W32
#    include <windows.h>
#endif

// C++
#include <exception>

// C
#include <cstdlib>
#include <cstdio>
#include <csignal>

//
// DO NOT MESS WITH THIS
//

typedef struct eh_frame eh_frame_t;

extern const axf::core::Exception* peekLastException();
extern const axf::core::Exception* getException(const eh_frame_t* current);
extern const eh_frame_t* currentExceptionFrame();
extern const eh_frame_t* unwindExceptionFrames(const eh_frame_t* current);

static void terminate()
{
    const axf::core::Exception* uncaught = peekLastException();
    if (uncaught != NULL)
    {
        char formattedExceptionMessage[0x1000] = {0};
        std::sprintf(formattedExceptionMessage,
                     " The runtime system has detected an uncaught exception and\n"
                     "as a result, the program has been forcefully terminated.\n"
                     "\n"
                     "%s: %s.",
                     uncaught->getClassName(), uncaught->getMessage());

#ifdef ARTEMIS_PLATFORM_W32
        MessageBoxA(NULL, formattedExceptionMessage, uncaught->getClass().getClassName(), MB_ICONERROR);
#endif

        std::fprintf(stderr, "\nUncaught exception!:\n\n\t%s: %s\n\n", uncaught->getClassName(), uncaught->getMessage());
        std::fprintf(stderr, "The system has detected an uncaught exception.\n"
                     "System exception handling frame:\n\n"
                     "%s: %s\n", uncaught->getClassName(), uncaught->getMessage());
        {
            const eh_frame_t* frame = unwindExceptionFrames(currentExceptionFrame());
            while (frame != NULL)
            {
                const axf::core::Exception* e = getException(frame);
                std::fprintf(stderr, "\t%s: %s\n", e->getClassName(), e->getMessage());

                // unwind
                frame = unwindExceptionFrames(frame);
            }
            std::fprintf(stderr, "\t-- no more exceptions --\n");
        }
    }
    else
    {
        // The exception is of another type
#ifdef ARTEMIS_PLATFORM_W32
        MessageBoxW(NULL, L"The system has detected an uncaught exception. Program execution must be aborted",
                    L"Uncaught exception!", MB_ICONERROR);
#endif

        std::fputs("the system has detected an uncaught exception and now must abort program execution.", stderr);
    }
    // Exit with error code 1
    std::exit(1);
}

//
// DO NOT MESS WITH WHAT IS ABOVE
//

static void sigsegv_handler(int signum)
{
    throw axf::core::NullPointerException("the program has incurred into a segment violation.");
}

using namespace axf;
using namespace axf::core;

Runtime::StaticRuntimeInitializer::StaticRuntimeInitializer()
{
    std::set_terminate(terminate);
    std::signal(SIGSEGV, sigsegv_handler);
}

Runtime::StaticRuntimeInitializer::~StaticRuntimeInitializer()
{
}

Runtime::StaticRuntimeInitializer Runtime::_srinit;

const Runtime::SystemType Runtime::getCurrentPlatform()
{
#ifdef ARTEMIS_PLATFORM_W32
    return MS_WINDOWS;
#else
    return UNKNOWN_OS;
#endif
}



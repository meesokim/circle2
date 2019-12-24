//
// main.c
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2014  R. Stange <rsta2@o2online.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include "kernel.h"
#include "common.h"
#include <circle/startup.h>
#include <circle/bcm2835.h>

CKernel m_Kernel;
unsigned GetTicks()
{
	return m_Kernel.GetTicks();
}
int main (void)
{
	// cannot return here because some destructors used in CKernel are not implemented

	if (!m_Kernel.Initialize ())
	{
		halt ();
		return EXIT_HALT;
	}
	
	TShutdownMode ShutdownMode = m_Kernel.Run (); 

	switch (ShutdownMode)
	{
	case ShutdownReboot:
		reboot ();
		return EXIT_REBOOT;

	case ShutdownHalt:
	default:
		halt ();
		return EXIT_HALT;
	}
}

long unsigned int circle_get_time()
{
    if (CKernel::s_pThis)
		return CKernel::s_pThis->GetTicks();
	return 0;
}
void circle_initialize()
{
    m_Kernel.Initialize();
}
void circle_run()
{
    m_Kernel.Run();
}
 void circle_start_timer()
{
}

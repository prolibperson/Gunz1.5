#ifndef _MAIN_H
#define _MAIN_H

//#pragma once

#pragma warning(disable:4099)

#ifdef _DEBUG
#pragma comment(lib,"cmld.lib")
#pragma comment(lib,"mint2d.lib")
#pragma comment(lib,"RealSpace2d.lib")
#pragma comment(lib,"CSCommonDE.lib")
#pragma comment(lib,"SafeUDPd.lib")
#pragma comment(lib, "dxerr8.lib")

#else

#ifdef _WIN64
#pragma comment(lib,"cml64.lib")
#pragma comment(lib,"mint264.lib")
#pragma comment(lib,"RealSpace264.lib")
#pragma comment(lib,"CSCommonE64.lib")
#pragma comment(lib,"SafeUDP64.lib")
#pragma comment(lib, "dxerr.lib")
#else
#pragma comment(lib,"cml.lib")
#pragma comment(lib,"mint2.lib")
#pragma comment(lib,"RealSpace2.lib")
#pragma comment(lib,"CSCommonE.lib")
#pragma comment(lib,"SafeUDP.lib")
#pragma comment(lib, "dxerr8.lib")
#endif
#endif


#pragma comment(lib,"winmm.lib")

#endif
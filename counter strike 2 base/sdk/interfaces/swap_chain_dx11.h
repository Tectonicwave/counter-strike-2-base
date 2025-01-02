#pragma once
#include "../../definitions.h"

// forward declarations
struct IDXGISwapChain;

namespace sdk
{
	class ISwapChainDx11
	{
		Memory_pad(0x170);
		IDXGISwapChain* pDXGISwapChain;
	};
}
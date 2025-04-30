#include <cstdint>
#include <string>
#include <cstring>
#include <emmintrin.h>
#include <intrin.h>
#include <thread>
#include <mutex>
#include <windows.h>
#include <DoubleEndedStackAllocator.h>
#include <DoubleBufferedAllocator.h>
#include <StackAllocator.h>
#include <PoolAllocator.h>
#include <FileHandler.h>
#include <GameLoopManager.h>
#include <CPUSpecs.h>
#include <Test.h>

int main() {
	GameLoopManager::init();

	while (GameLoopManager::getRunning()) {
		for (const auto& listener : GameLoopManager::getFrameListnerList()) {
			listener->frameStarted();

		}
		
		GameLoopManager::renderMain();

		for (const auto& listener : GameLoopManager::getFrameListnerList()) {
			listener->frameEnded();

		}
	}

	GameLoopManager::destroy();
	return 0;
}
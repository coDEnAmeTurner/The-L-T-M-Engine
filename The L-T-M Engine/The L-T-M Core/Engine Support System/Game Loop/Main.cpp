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
#include <JobSystem.h>
#include <CPUSpecs.h>
#include <Test.h>
#include <GameLoop Common.h>

SpinLockLTM g_spin;

int main() {
	set_thread_affinity(GetCurrentThread(), 0);
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

	CPUSpecs::init();
	GameLoopManager::init();
	JobSystem::init();

	bool firstTime = true;

	while (GameLoopManager::getRunning()) {
		GameLoopManager::getDoubleBufferMain()->swapBuffers();
		GameLoopManager::getStackMain()->clear();
		GameLoopManager::getDoubleBufferMain()->clearCurrentBuffer();
		GameLoopManager::getDoubleEndedMain()->clear();

		if (!GameLoopManager::getFrameListnerList().empty())
			for (const auto& listener : GameLoopManager::getFrameListnerList()) {
				listener->frameStarted();

			}
		if (firstTime)
		{
			firstTime = false;
			testJobSystem();
		}

		std::cout << "Dummy text" << std::endl; // Should print 42

		GameLoopManager::renderMain();

		if (!GameLoopManager::getFrameListnerList().empty())
			for (const auto& listener : GameLoopManager::getFrameListnerList()) {
				listener->frameEnded();

			}
		std::this_thread::yield();

	}

	JobSystem::destroy();
	GameLoopManager::destroy();
	CPUSpecs::destroy();
	return 0;
}
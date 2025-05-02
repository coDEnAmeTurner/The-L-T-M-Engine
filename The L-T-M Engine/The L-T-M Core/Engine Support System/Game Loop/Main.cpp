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

int main() {
	set_thread_affinity(GetCurrentThread(), 0);
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

	CPUSpecs::init();
	GameLoopManager::init();
	JobSystem::init();

	bool firsttime = true;
	while (GameLoopManager::getRunning()) {
		if (!GameLoopManager::getFrameListnerList().empty())
			for (const auto& listener : GameLoopManager::getFrameListnerList()) {
				listener->frameStarted();

			}

		GameLoopManager::renderMain();

		if (firsttime)
		{
			testJobSystem();
			firsttime = false;
		}

		std::cout << "Dummy is " << std::endl; // Should print 42

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
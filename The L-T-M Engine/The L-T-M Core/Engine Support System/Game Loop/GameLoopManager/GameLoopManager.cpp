#include "GameLoopManager.h"

std::unique_ptr<GameLoopManager> GameLoopManager::s_instance = nullptr;

GameLoopManager::GameLoopManager() {
	m_stackMain = std::shared_ptr<StackAllocator>(new StackAllocator(WORD_SIZE * pow_of_2(20), MINIMUM_ALIGNMENT)); //8MB
	m_doubleEndedMain = std::shared_ptr<DoubleEndedStackAllocator>(new DoubleEndedStackAllocator(WORD_SIZE * pow_of_2(21), MINIMUM_ALIGNMENT)); //16MB
	m_doubleBufferMain = std::shared_ptr<DoubledBufferedAllocator>(new DoubledBufferedAllocator(WORD_SIZE * pow_of_2(20), MINIMUM_ALIGNMENT)); //8MB
}

void GameLoopManager::init() {
	if (s_instance == nullptr) {
		s_instance = std::unique_ptr<GameLoopManager>(new GameLoopManager());
	}
}
void GameLoopManager::destroy() {
	s_instance = nullptr;
}
void GameLoopManager::pushFrameLis(std::shared_ptr<FrameListener> framelis) {
	assert(framelis != nullptr || "Frame listner can not be null");

	s_instance->m_frameLisList.push_back(framelis);
}
void GameLoopManager::eraseFrameLis(std::shared_ptr<FrameListener> framelis) {
	assert(framelis != nullptr || "Frame listner can not be null");

	auto it = std::find(
		s_instance->m_frameLisList.begin(),
		s_instance->m_frameLisList.end(),
		framelis
	);
	assert(it != s_instance->m_frameLisList.end() || "Can not find frame listener");

	s_instance->m_frameLisList.erase(it);
}
std::vector<std::shared_ptr<FrameListener>>& GameLoopManager::getFrameListnerList()
{
	return s_instance->m_frameLisList;
}
void GameLoopManager::renderMain() {
	//placeholder
}

std::unique_ptr<GameLoopManager>& GameLoopManager::getInstance() {
	return s_instance;
}
bool GameLoopManager::getRunning() {
	//s_instance->m_lockRunning.Acquire();
	bool running = s_instance->m_running.load(std::memory_order_relaxed);
	//s_instance->m_lockRunning.Release();

	return running;
}

void GameLoopManager::setRunning(bool run)
{
	//ScopedLock<SpinLockLTM> lock(&s_instance->m_lockRunning);
	s_instance->m_running.store(run, std::memory_order_relaxed);
}

MutexLTM& GameLoopManager::getMutexConsole()
{
	return s_instance->m_mutexConsole;
}

std::shared_ptr<StackAllocator> GameLoopManager::getStackMain()
{
	return s_instance->m_stackMain;
}

std::shared_ptr<DoubledBufferedAllocator> GameLoopManager::getDoubleBufferMain()
{
	return s_instance->m_doubleBufferMain;
}

std::shared_ptr<DoubleEndedStackAllocator> GameLoopManager::getDoubleEndedMain()
{
	return s_instance->m_doubleEndedMain;
}

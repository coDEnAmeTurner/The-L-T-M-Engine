#include "GameLoopManager.h"

std::unique_ptr<GameLoopManager> GameLoopManager::s_instance = nullptr;

GameLoopManager::GameLoopManager() {
	if (s_instance == nullptr) {
		s_instance = std::make_unique<GameLoopManager>();
	}
}

GameLoopManager::~GameLoopManager() {
	s_instance = nullptr;
}

void GameLoopManager::init() {
	GameLoopManager();
}
void GameLoopManager::destroy() {
	
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
	return s_instance->m_running;
}
#pragma once
#include<memory>
#include<vector>
#include<cassert>
#include<algorithm>
#include<SpinLockLTM.h>
#include<ScopedLock.h>
#include<FrameListener.h>
#include<atomic>

class GameLoopManager {
public:
	static void init();
	static void destroy();
	static void pushFrameLis(std::shared_ptr<FrameListener> framelis);
	static void eraseFrameLis(std::shared_ptr<FrameListener> framelis);
	static std::unique_ptr<GameLoopManager>& getInstance();
	static std::vector<std::shared_ptr<FrameListener>>& getFrameListnerList();
	static void renderMain();
	static bool getRunning();
	static void setRunning(bool run);
private:
	static std::unique_ptr<GameLoopManager> s_instance;

	SpinLockLTM m_lockRunning;
	std::vector<std::shared_ptr<FrameListener>> m_frameLisList;
	std::atomic<bool> m_running = true;

	GameLoopManager();
};
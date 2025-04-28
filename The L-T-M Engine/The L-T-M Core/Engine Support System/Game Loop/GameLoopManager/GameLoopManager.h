#pragma once
#include<memory>
#include<vector>
#include<cassert>
#include<algorithm>
#include<FrameListener.h>

class GameLoopManager {
public:
	static void init();
	static void destroy();
	static void pushFrameLis(std::shared_ptr<FrameListener> framelis);
	static void eraseFrameLis(std::shared_ptr<FrameListener> framelis);
	static std::unique_ptr<GameLoopManager>& getInstance();
	static std::vector<std::shared_ptr<FrameListener>>& getFrameListnerList();
	static void renderMaster();
	static bool getRunning();
private:
	static std::unique_ptr<GameLoopManager> s_instance;
	std::vector<std::shared_ptr<FrameListener>> m_frameLisList;
	bool m_running = true;

	GameLoopManager();
	~GameLoopManager();
};
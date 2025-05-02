#pragma once
#include<memory>
#include<vector>
#include<cassert>
#include<algorithm>
#include<SpinLockLTM.h>
#include<ScopedLock.h>
#include<FrameListener.h>
#include<StackAllocator.h>
#include<DoubleEndedStackAllocator.h>
#include<DoubleBufferedAllocator.h>
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

	static std::shared_ptr<StackAllocator> getStackMain();
	static std::shared_ptr<DoubledBufferedAllocator> getDoubleBufferMain();
	static std::shared_ptr<DoubleEndedStackAllocator> getDoubleEndedMain();



private:
	static std::unique_ptr<GameLoopManager> s_instance;

	std::shared_ptr<StackAllocator> m_stackMain;
	std::shared_ptr<DoubledBufferedAllocator> m_doubleBufferMain;
	std::shared_ptr<DoubleEndedStackAllocator> m_doubleEndedMain;

	SpinLockLTM m_lockRunning;
	std::vector<std::shared_ptr<FrameListener>> m_frameLisList;
	std::atomic<bool> m_running = true;

	GameLoopManager();
};
#pragma once
class FrameListener {
public:
	virtual void frameStarted() = 0;
	virtual void frameEnded() = 0;
};
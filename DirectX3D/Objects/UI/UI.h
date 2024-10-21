#pragma once

class UI : public Quad
{
public:
	UI(wstring file);
	virtual ~UI() = 0;

	virtual void Render() = 0;

public:
	bool isVisible = false;
};


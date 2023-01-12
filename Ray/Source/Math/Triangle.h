#pragma once

class Rect
{
public:
	Rect() = default;

	Rect(int width, int height)
		: Width(width), Height(height)
	{
	
	}
	Rect(int left, int top, int width, int height)
		: Left(left), Top(top), Width(width), Height(height)
	{

	}
	Rect(const Rect& r)
		: Left(r.Left), Top(r.Top), Width(r.Width), Height(r.Height)
	{

	}

	// getters
public:
	INT		width() { return Width; }
	INT		height() { return Height; }
	INT		left() { return Left; }
	INT		top() { return Top; }

	//setters
public:
	VOID	width(int w) { Width = w; }
	VOID	height(int h) { Height = h; }
	VOID	left(int l) { Left = l; }
	VOID	top(int t) { Top = t; }

private:
	INT Width = 0, Height = 0, Left = 0, Top = 0;
};
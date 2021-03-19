#pragma once

enum class Keyboard : int {
  kArrow = 224,
  kArrowUp = 72,
  kArrowLeft = 75,
  kArrowRight = 77,
  kArrowDown = 80,
  kSpace = 32,
  kEscape = 27
};

enum class Tetromino : int {
	kUnknown = -1,
	I,
	O,
	T,
	J,
	L,
	S,
	Z
};

struct Vec2 {
  int x;
  int y;
};
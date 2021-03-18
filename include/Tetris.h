#pragma once

#include <Windows.h>
#include <array>
#include <cmath>
#include <map>
#include <random>
#include <utility>
#include <memory>

#include "Auxiliary.h"
#include "BaseApp.h"

// using Vec2 = std::pair<int, int>;
using Mat2 = std::array<Vec2, 2>;
using Mat4x2 = std::array<Vec2, 4>;

class Tetris : public BaseApp {
 public:
  Tetris();

  void KeyPressed(int btnCode) override;
  void UpdateF(float deltaTime) override;

  int GetNextTetromino();
  Mat4x2 CalculateCoordinatesTetromino();
  Mat4x2 Rotate(Mat4x2&);
  bool CheckNewPosition(Mat4x2& objectj, Vec2&& vectorMove);

 private:
  float mDelay = 0.5f;
  float mTime = 0.0f;

  Vec2 mGameFieldLeftUp;
  Vec2 mGameFieldRightDown;

  int mWindowWidth, mWindowHeight;
  bool mKeyboradArrowFlag = false;

  Vec2 mObjectOld;
  Vec2 mObject;

  std::array<std::array<int, 4>, 7> mTetrominoFigures = {{
      {1, 5, 9, 13},  // I
      {1, 2, 5, 6},   // O
      {1, 4, 5, 6},   // T
      {1, 5, 9, 8},   // J
      {1, 5, 9, 10},  // L
      {4, 5, 1, 2},   // S
      {0, 1, 5, 6}    // Z
  }};

  // true = vertical
  bool mStateTetromino = true;

  Mat4x2 mTetromino;
  Mat4x2 mTetrominoOld;
  Mat4x2 mTetrominoNext;

  int mTetrominoNum;
  bool mTetrominoNextFlag = false;
};
#pragma once

#include <Windows.h>
#include <array>
#include <cmath>
#include <memory>
#include <random>
#include <utility>

#include "Auxiliary.h"
#include "BaseApp.h"

using Mat2 = std::array<Vec2, 2>;
using Mat4x2 = std::array<Vec2, 4>;

class Tetris : public BaseApp {
 public:
  Tetris();

  void KeyPressed(int btnCode) override;
  void UpdateF(float deltaTime) override;

  Tetromino GetNextTetromino();
  Mat4x2 CalculateCoordinatesTetromino(Tetromino tetrominoNum);
  Mat4x2 Rotate(Mat4x2&);
  bool CheckNewPosition(Mat4x2& objectj, Vec2&& vectorMove);
  void LineFill—heck();

 private:
  float mDelay = 0.5f;
  float mTime = 0.0f;

  Vec2 mGameFieldLeftUp;
  Vec2 mGameFieldRightDown;

  int mWindowWidth, mWindowHeight;
  bool mKeyboradArrowFlag = false;

  Vec2 mSpawnPosition;
  Vec2 mSpawnPositionPreview;

  std::array<std::array<int, 4>, 7> mTetrominoFigures = {{
      {1, 5, 9, 13},  // I
      {1, 2, 5, 6},   // O
      {1, 4, 5, 6},   // T
      {1, 5, 9, 8},   // J
      {1, 5, 9, 10},  // L
      {4, 5, 1, 2},   // S
      {0, 1, 5, 6}    // Z
  }};

  Mat4x2 mTetromino;
  Mat4x2 mTetrominoOld;
  Mat4x2 mTetrominoNext;

  Tetromino mTetrominoNum = Tetromino::kUnknown;
  Tetromino mTetrominoNextNum = Tetromino::kUnknown;
  
  // true = vertical
  bool mStateTetromino = true;
};
#pragma once

#include <Windows.h>

#include <array>
#include <cmath>
#include <memory>
#include <random>
#include <string>
#include <utility>

#include "Auxiliary.h"
#include "BaseApp.h"

using Mat2 = std::array<Vec2, 2>;
using Mat4x2 = std::array<Vec2, 4>;

/**
 * @brief Class that implements Tetris logic.
 *
 * Inherits a render class that calls overridden program logic functions.
 */
class Tetris : public BaseApp {
 public:
  Tetris() = delete;

  /**
   * @brief Construct a new Tetris object.
   *
   * The constructor sets the initial coordinates for drawing the game
   * interface.
   */
  Tetris(int&& windowWidth, int&& windowHeight);

  /**
   * @brief Event handling method.
   *
   * Left / Right Arrows to move the Tetrimino left / right.
   * The down arrow accelerates the movement of the figure until the next
   * tetrimino appears.
   * The space bar wraps around the tetrimino.
   *
   * @param btnCode Accepts the pressed button.
   */
  void KeyPressed(int btnCode) override;

  /**
   * @brief Determining the coordinates of the tetrimino and sending them for
   * drawing.
   *
   * @param deltaTime Time difference between the previous iteration and this
   * one, in seconds.
   */
  void UpdateF(float deltaTime) override;

  /**
   * @brief Get the Next random Tetromino number
   *
   * @return Tetromino number next tetromino
   */
  Tetromino GetNextTetromino();

  /**
   * @brief Determination of local coordinates of tetromino
   *
   * Determines the coordinates of a tetromino from a matrix in the local
   * coordinate system.
   *
   * @param tetrominoNum A tetromino number that identifies a tetromino from an
   * array of figures.
   * @return Mat4x2 Local coordinates of tetromino.
   */
  Mat4x2 CalculateCoordinatesTetromino(Tetromino tetrominoNum);

  /**
   * @brief Rotate tetromino/
   *
   * @param object Curent tetromino.
   * @return Mat4x2 New tetromino.
   */
  Mat4x2 Rotate(Mat4x2& object);

  /**
   * @brief Checking a new position.
   *
   * Checking if the new position is free and if we have gone out of the play
   * area.
   *
   * @param object Curent tetromino.
   * @param vectorMove The vector of the direction of movement of the object.
   * @return true New position fits.
   * @return false New position does not fit.
   */
  bool CheckNewPosition(Mat4x2& object, Vec2&& vectorMove);

  /**
   * @brief Line fill check.
   *
   * Checking the filling of the line, clear if the line is full and changing
   * the score readings.
   */
  void LineFillCheck();

 private:
  /**
   * @brief
   *
   * Tetriminos are defined in a 4 by 4 matrix. The matrix is numbered for each
   * cell starting at 0. By drawing a figure in this matrix, we get the numbers
   * that correspond to this figure.
   */
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

  long mScore = 0;

  Vec2 mGameFieldLeftUp;
  Vec2 mGameFieldRightDown;

  Vec2 mSpawnPosition;
  Vec2 mSpawnPositionPreview;
  Vec2 mSpawnPositionScore;

  /**
   * @brief Time delay through which the tetromino moves downward.
   */
  float mDelay = 0.5f;

  float mTime = 0.0f;

  Tetromino mTetrominoNum = Tetromino::kUnknown;
  Tetromino mTetrominoNextNum = Tetromino::kUnknown;

  int mWindowWidth, mWindowHeight;

  /**
   * @brief Determines the horizontal/vertical position of the tetromino.
   *
   * false The horizontal position of the tetromino.
   * true The vertical position of the tetromino.
   */
  bool mStateTetromino = true;

  bool mKeyboradArrowFlag = false;
};
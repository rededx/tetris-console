#include "Tetris.h"

#include <thread>

Tetris::Tetris(int &&windowWidth, int &&windowHeight)
    : BaseApp(windowWidth, windowHeight) {
  mWindowWidth = windowWidth;
  mWindowHeight = windowHeight;

  // Set game field
  mGameFieldLeftUp = {1, 1};
  mGameFieldRightDown = {15 + mGameFieldLeftUp.x, 20 + mGameFieldLeftUp.y};

  // Set spawn position
  mSpawnPosition = {(mGameFieldRightDown.x - mGameFieldLeftUp.x) / 2,
                    mGameFieldLeftUp.y};

  // Set spawn position preview
  mSpawnPositionPreview = {
      (mWindowWidth - (mGameFieldRightDown.x - mGameFieldLeftUp.x)) / 2 +
          (mGameFieldRightDown.x - mGameFieldLeftUp.x),
      mGameFieldLeftUp.y + 2};

  // Set spawn position score
  mSpawnPositionScore = {
      2 + mGameFieldLeftUp.x,
      (mWindowHeight - (mGameFieldRightDown.y + mGameFieldLeftUp.y)) / 2 +
          (mGameFieldRightDown.y + mGameFieldLeftUp.y),
  };

  // Create border and game field
  for (int i = 0; i < mWindowWidth; i++)
    for (int j = 0; j < mWindowHeight; j++) {
      if (i == 0 ||                  // outer left border
          i == mWindowWidth - 1 ||   // outer right border
          j == mWindowHeight - 1 ||  // outer bottom border
          j == 0 ||                  // outer upper border
          (i == mGameFieldRightDown.x + 1 &&
           j < mGameFieldRightDown.y + 1) ||  // inner right border
          j == mGameFieldRightDown.y + 1      // inner bottom border
      )
        SetChar(i, j, L'#');

      if (i >= mGameFieldLeftUp.x && i <= mGameFieldRightDown.x &&
          j >= mGameFieldLeftUp.y && j <= mGameFieldRightDown.y)
        SetChar(i, j, L'.');
    }
}

void Tetris::KeyPressed(int btnCode) {
  if (mKeyboradArrowFlag) {
    mKeyboradArrowFlag = false;
    std::shared_ptr<int> dx = nullptr;

    switch (btnCode) {
      case (int)Keyboard::kArrowUp:
        break;

      case (int)Keyboard::kArrowLeft:
        dx = std::make_shared<int>(-1);
        break;

      case (int)Keyboard::kArrowRight:
        dx = std::make_shared<int>(1);
        break;

      case (int)Keyboard::kArrowDown:
        mDelay = 0.05f;
        break;

      default:
        break;
    }

    if (dx != nullptr)
      if (CheckNewPosition(mTetromino, {*dx, 0}))
        for (auto &&[x, y] : mTetromino) x += *dx;

  } else {
    switch (btnCode) {
      case (int)Keyboard::kSpace:
        auto tempTetromino = Rotate(mTetromino);
        for (auto &&[x, y] : mTetromino) SetChar(x, y, L'.');

        if (CheckNewPosition(tempTetromino, {0, 0}))
          mTetromino = tempTetromino;
        else
          mStateTetromino = !mStateTetromino;

        break;

      default:
        break;
    }
  }

  if (btnCode == (int)Keyboard::kArrow) mKeyboradArrowFlag = true;
}

void Tetris::UpdateF(float deltaTime) {
  // First draw
  if (mTetrominoNum == Tetromino::kUnknown) {
    // Building the first tetromino
    mTetrominoNum = GetNextTetromino();
    mTetromino = CalculateCoordinatesTetromino(mTetrominoNum);

    for (auto &&[x, y] : mTetromino) {
      x += mSpawnPosition.x;
      y += mSpawnPosition.y;
    }

    mTetrominoOld = mTetromino;

    // Create next tetromino
    mTetrominoNextNum = GetNextTetromino();
    mTetrominoNext = CalculateCoordinatesTetromino(mTetrominoNextNum);

    // Draw the first preview
    for (auto &&[x, y] : mTetrominoNext) {
      SetChar(x + mSpawnPositionPreview.x, y + mSpawnPositionPreview.y, L'O');
    }

    // Draw the first score
    auto scoreStr = std::wstring(L"> Score: " + std::to_wstring(mScore));
    for (auto i = 0; i < scoreStr.size(); ++i) {
      SetChar(mSpawnPositionScore.x + i, mSpawnPositionScore.y, scoreStr.at(i));
    }
  }

  // Clear old tetromino
  for (auto &&[x, y] : mTetrominoOld) {
    SetChar(x, y, L'.');
  }

  // Draw new tetromino
  for (auto &&[x, y] : mTetromino) {
    SetChar(x, y, L'O');
  }

  mTetrominoOld = mTetromino;

  // Updating еetrщmino after a specified time
  if (mTime += deltaTime; mTime > mDelay) {
    mTime -= mDelay;

    if (CheckNewPosition(mTetromino, {0, 1}))
      // Move tetromino down
      for (auto &&[x, y] : mTetromino) ++y;

    else {
      LineFillCheck();

      // Update score
      auto scoreStr = std::wstring(L"> Score: " + std::to_wstring(mScore));
      for (auto i = 0; i < scoreStr.size(); ++i) {
        SetChar(mSpawnPositionScore.x + i, mSpawnPositionScore.y,
                scoreStr.at(i));
      }

      // Clear old preview
      for (auto &&[x, y] : mTetrominoNext) {
        SetChar(x + mSpawnPositionPreview.x, y + mSpawnPositionPreview.y, L' ');
      }

      // Update curent tetromino
      for (auto &&[x, y] : mTetrominoNext) {
        x += mSpawnPosition.x;
        y += mSpawnPosition.y;
      }
      mTetrominoOld = mTetromino = mTetrominoNext;
      mTetrominoNum = mTetrominoNextNum;

      // Create new next tetromino
      mTetrominoNextNum = GetNextTetromino();
      mTetrominoNext = CalculateCoordinatesTetromino(mTetrominoNextNum);

      // Create new preview
      for (auto &&[x, y] : mTetrominoNext) {
        SetChar(x + mSpawnPositionPreview.x, y + mSpawnPositionPreview.y, L'O');
      }

      // Set default value
      mDelay = 0.5f;
      mStateTetromino = true;

      // Check end game and close window
      if (!CheckNewPosition(mTetromino, {0, 0})) {
        HWND myConsole = GetConsoleWindow();
        PostMessage(myConsole, WM_CLOSE, 0, 0);
      }
    }
  }
}

Tetromino Tetris::GetNextTetromino() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(0, mTetrominoFigures.size() - 1);

  return (Tetromino)dist(mt);
}

Mat4x2 Tetris::CalculateCoordinatesTetromino(Tetromino tetrominoNum) {
  Mat4x2 coordinates;

  // Determination of coordinates according to position in the tetromino matrix
  for (int i = 0; i < coordinates.size(); i++) {
    coordinates[i] = {mTetrominoFigures[(int)tetrominoNum][i] % 4,
                      mTetrominoFigures[(int)tetrominoNum][i] / 4};
  }

  return std::move(coordinates);
}

Mat4x2 Tetris::Rotate(Mat4x2 &object) {
  auto rotatePoint = [](auto &&vec2, auto &&center) -> Vec2 {
    double radianAngle = 90 * std::_Pi / 180;
    Vec2 newVec2 = {center->x - (vec2.y - center->y),
                    center->y + (vec2.x - center->x)};
    return std::move(newVec2);
  };

  auto rotatePointBack = [](auto &&vec2, auto &&center) -> Vec2 {
    double radianAngle = 90 * std::_Pi / 180;
    Vec2 newVec2 = {center->x + (vec2.y - center->y),
                    center->y - (vec2.x - center->x)};
    return std::move(newVec2);
  };

  auto rotateObject = [](auto &&object, auto &&center,
                         auto &&rotateFunction) -> Mat4x2 {
    Mat4x2 newObject;
    auto iterNewObj = newObject.begin();

    for (auto &&vec2 : object) {
      *iterNewObj = rotateFunction(vec2, center);
      iterNewObj++;
    }

    return std::move(newObject);
  };

  Mat4x2 newObject;

  switch (auto center = mTetromino.begin(); mTetrominoNum) {
    case Tetromino::I:
      center = mTetromino.begin() + 1;
      if (mStateTetromino) {
        newObject = rotateObject(object, center, rotatePoint);
        mStateTetromino = !mStateTetromino;
      } else {
        newObject = rotateObject(object, center, rotatePointBack);
        mStateTetromino = !mStateTetromino;
      }
      break;

    case Tetromino::O:
      return std::move(mTetromino);
      break;

    case Tetromino::T:
      center = mTetromino.begin() + 2;
      newObject = rotateObject(object, center, rotatePoint);
      break;

    case Tetromino::J:
      center = mTetromino.begin() + 1;
      newObject = rotateObject(object, center, rotatePoint);
      break;

    case Tetromino::L:
      center = mTetromino.begin() + 1;
      newObject = rotateObject(object, center, rotatePoint);
      break;

    case Tetromino::S:
      center = mTetromino.begin() + 1;
      if (mStateTetromino)
        newObject = rotateObject(object, center, rotatePoint);
      else
        newObject = rotateObject(object, center, rotatePointBack);
      mStateTetromino = !mStateTetromino;
      break;

    case Tetromino::Z:
      center = mTetromino.begin() + 2;
      if (mStateTetromino)
        newObject = rotateObject(object, center, rotatePoint);
      else
        newObject = rotateObject(object, center, rotatePointBack);
      mStateTetromino = !mStateTetromino;
      break;

    default:
      break;
  }

  return std::move(newObject);
}

bool Tetris::CheckNewPosition(Mat4x2 &object, Vec2 &&vectorMove) {
  auto edgeRightOrLeft = [&]() -> decltype(auto) {
    std::unordered_map<decltype(object.begin()->y), decltype(object.begin()->x)>
        edgeElement;

    for (auto &&[x, y] : object) {
      auto element = edgeElement.find(y);

      if (element != edgeElement.end()) {
        if (vectorMove.x > 0)
          if (edgeElement.at(y) < x) edgeElement[y] = x;
        if (vectorMove.x < 0)
          if (edgeElement.at(y) > x) edgeElement[y] = x;
      } else
        edgeElement[y] = x;
    }
    return edgeElement;
  };

  auto edgeDown = [&]() -> decltype(auto) {
    std::unordered_map<decltype(object.begin()->y), decltype(object.begin()->x)>
        edgeElement;

    for (auto &&[x, y] : object) {
      auto element = edgeElement.find(x);

      if (element != edgeElement.end()) {
        if (edgeElement.at(x) < y) edgeElement[x] = y;
      } else
        edgeElement[x] = y;
    }
    return edgeElement;
  };

  if (vectorMove.x == 0 && vectorMove.y == 0) {
    for (auto &&[x, y] : object) {
      if (GetChar(x, y) == L'O') return false;
      if (x < mGameFieldLeftUp.x || x > mGameFieldRightDown.x ||
          y > mGameFieldRightDown.y)
        return false;
    }
  } else if (vectorMove.x != 0)
    for (auto &&[y, x] : edgeRightOrLeft()) {
      if (GetChar(x + vectorMove.x, y + vectorMove.y) == L'O') return false;

      if (x + vectorMove.x < mGameFieldLeftUp.x ||
          x + vectorMove.x > mGameFieldRightDown.x)
        return false;
    }
  else if (vectorMove.y != 0)
    for (auto &&[x, y] : edgeDown()) {
      if (y + vectorMove.y > mGameFieldRightDown.y) return false;
      if (GetChar(x + vectorMove.x, y + vectorMove.y) == L'O') return false;
    }

  return true;
}

void Tetris::LineFillCheck() {
  for (auto y = mGameFieldLeftUp.y; y <= mGameFieldRightDown.y; ++y) {
    auto count = 0;

    for (auto x = mGameFieldLeftUp.x; x < mGameFieldRightDown.x; ++x) {
      if (GetChar(x, y) == L'O') ++count;
    }

    if (count == (mGameFieldRightDown.x - mGameFieldLeftUp.x)) {
      for (auto j = y; j > mGameFieldLeftUp.y; --j)
        for (auto i = mGameFieldLeftUp.x; i < mGameFieldRightDown.x; ++i) {
          SetChar(i, j, GetChar(i, j - 1));
        }

      mScore += 100l;
    }
  }
}

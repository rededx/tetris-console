#include "Tetris.h"

#include <thread>

Tetris::Tetris() : BaseApp(25, 26) {
  mWindowWidth = 25;
  mWindowHeight = 26;

  // set game field
  mGameFieldLeftUp = {1, 1};
  mGameFieldRightDown = {15 + mGameFieldLeftUp.x, 20 + mGameFieldLeftUp.y};

  // create first tetromino
  mTetrominoNum = GetNextTetromino();
  mTetromino = CalculateCoordinatesTetromino();
  mTetrominoNextFlag = true;

  // TODO: calculate Y
  mObjectOld = mObject = {(mGameFieldRightDown.x - mGameFieldLeftUp.y) / 2,
                          mGameFieldLeftUp.y};

  for (auto&& [x, y] : mTetromino) {
    x += mObject.x;
    y += mObject.y;
  }
  mTetrominoOld = mTetromino;

  // Create border
  for (int i = 0; i < mWindowWidth; i++) {
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
    }
  }
}

void Tetris::KeyPressed(int btnCode) {
  std::shared_ptr<int> dx = nullptr;

  if (mKeyboradArrowFlag) {
    mKeyboradArrowFlag = false;

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
        for (auto&& [x, y] : mTetromino) x += *dx;

  } else {
    Mat4x2 tempTetromino;

    switch (btnCode) {
      case (int)Keyboard::kSpace:
        // TODO: crear temp
        tempTetromino = Rotate(mTetromino);
        for (auto&& [x, y] : mTetromino) SetChar(x, y, L'-');
        if (CheckNewPosition(tempTetromino, {0, 0})) mTetromino = tempTetromino;
        break;

      default:
        break;
    }
  }

  if (btnCode == (int)Keyboard::kArrow) mKeyboradArrowFlag = true;
}

void Tetris::UpdateF(float deltaTime) {
  // action tetromino
  for (auto&& [x, y] : mTetrominoOld) {
    SetChar(x, y, L'-');
  }
  for (auto&& [x, y] : mTetromino) {
    SetChar(x, y, L'O');
  }
  mTetrominoOld = mTetromino;

  mTime += deltaTime;
  if (mTime > mDelay) {
    mTime -= mDelay;
    // move tetromino
    if (CheckNewPosition(mTetromino, {0, 1}))
      for (auto&& [x, y] : mTetromino) ++y;
    else {
      // check line
      LineFill—heck();

      // check and create new tetromino
      mTetrominoNum = GetNextTetromino();
      mTetromino = CalculateCoordinatesTetromino();

      for (auto&& [x, y] : mTetromino) {
        x += mObject.x;
        y += mObject.y;
      }

      if (CheckNewPosition(mTetromino, {0, 0})) {
        mTetrominoOld = mTetromino;

        mDelay = 0.3f;
      } else
        exit(0);
    }
  }
}

int Tetris::GetNextTetromino() {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> dist(0, mTetrominoFigures.size() - 1);

  return dist(mt);
}

Mat4x2 Tetris::CalculateCoordinatesTetromino() {
  Mat4x2 coordinates;

  for (int i = 0; i < coordinates.size(); i++) {
    coordinates[i] = {mTetrominoFigures[mTetrominoNum][i] % 4,
                      mTetrominoFigures[mTetrominoNum][i] / 4};
  }

  return std::move(coordinates);
}

Mat4x2 Tetris::Rotate(Mat4x2& object) {
  // labdas block
  auto rotatePoint = [](auto&& vec2, auto&& center) -> Vec2 {
    double radianAngle = 90 * std::_Pi / 180;
    Vec2 newVec2 = {center->x - (vec2.y - center->y),
                    center->y + (vec2.x - center->x)};
    return std::move(newVec2);
  };

  auto rotatePointBack = [](auto&& vec2, auto&& center) -> Vec2 {
    double radianAngle = 90 * std::_Pi / 180;
    Vec2 newVec2 = {center->x + (vec2.y - center->y),
                    center->y - (vec2.x - center->x)};
    return std::move(newVec2);
  };

  auto rotateObject = [](auto&& object, auto&& center,
                         auto&& rotateFunction) -> Mat4x2 {
    Mat4x2 newObject;
    auto iterNewObj = newObject.begin();

    for (auto&& vec2 : object) {
      *iterNewObj = rotateFunction(vec2, center);
      iterNewObj++;
    }

    return std::move(newObject);
  };

  Mat4x2 newObject;
  auto center = mTetromino.begin();

  switch (mTetrominoNum) {
    case (int)Tetromino::I:
      center = mTetromino.begin() + 1;
      if (mStateTetromino) {
        newObject = rotateObject(object, center, rotatePoint);
        mStateTetromino = !mStateTetromino;
      } else {
        newObject = rotateObject(object, center, rotatePointBack);
        mStateTetromino = !mStateTetromino;
      }
      break;

    case (int)Tetromino::O:
      return std::move(mTetromino);
      break;

    case (int)Tetromino::T:
      center = mTetromino.begin() + 2;
      newObject = rotateObject(object, center, rotatePoint);
      break;

    case (int)Tetromino::J:
      center = mTetromino.begin() + 1;
      newObject = rotateObject(object, center, rotatePoint);
      break;

    case (int)Tetromino::L:
      center = mTetromino.begin() + 1;
      newObject = rotateObject(object, center, rotatePoint);
      break;

    case (int)Tetromino::S:
      center = mTetromino.begin() + 1;
      if (mStateTetromino)
        newObject = rotateObject(object, center, rotatePoint);
      else
        newObject = rotateObject(object, center, rotatePointBack);
      mStateTetromino = !mStateTetromino;
      break;

    case (int)Tetromino::Z:
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

bool Tetris::CheckNewPosition(Mat4x2& object, Vec2&& vectorMove) {
  auto edgeRightOrLeft = [&]() -> decltype(auto) {
    std::unordered_map<decltype(object.begin()->y), decltype(object.begin()->x)>
        edgeElement;

    for (auto&& [x, y] : object) {
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

    for (auto&& [x, y] : object) {
      auto element = edgeElement.find(x);

      if (element != edgeElement.end()) {
        if (edgeElement.at(x) < y) edgeElement[x] = y;
      } else
        edgeElement[x] = y;
    }
    return edgeElement;
  };

  if (vectorMove.x == 0 && vectorMove.y == 0) {
    for (auto&& [x, y] : object) {
      if (GetChar(x, y) == L'O') return false;
      if (x < mGameFieldLeftUp.x || x > mGameFieldRightDown.x ||
          y > mGameFieldRightDown.y)
        return false;
    }
  } else if (vectorMove.x != 0)
    for (auto&& [y, x] : edgeRightOrLeft()) {
      if (GetChar(x + vectorMove.x, y + vectorMove.y) == L'O') return false;

      if (x + vectorMove.x < mGameFieldLeftUp.x ||
          x + vectorMove.x > mGameFieldRightDown.x)
        return false;
    }
  else if (vectorMove.y != 0)
    for (auto&& [x, y] : edgeDown()) {
      if (y + vectorMove.y > mGameFieldRightDown.y) return false;
      if (GetChar(x + vectorMove.x, y + vectorMove.y) == L'O') return false;
    }

  return true;
}

void Tetris::LineFill—heck() {
  for (auto y = mGameFieldLeftUp.y; y <= mGameFieldRightDown.y; ++y) {
    auto count = 0;

    for (auto x = mGameFieldLeftUp.x; x < mGameFieldRightDown.x; ++x) {
      if (GetChar(x, y) == L'O') ++count;
    }

    if (count == (mGameFieldRightDown.x - mGameFieldLeftUp.x))
      for (auto j = y; j > mGameFieldLeftUp.y; --j)
        for (auto i = mGameFieldLeftUp.x; i < mGameFieldRightDown.x; ++i)
          SetChar(i, j, GetChar(i, j - 1));
  }
}

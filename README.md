# Simple Tetris implementation in cpp
2020-06-07

Implement a simple Tetirs Game using cpp and Window API

## OUTLINES
- User inputs
- Console display & control
- Play background sound (BG)

## NOTE 
해당 프로젝트는 콘솔 입력창의 종비를 설정해논 상태이기 때문에 실행 전 콘솔의 속성 창에서 디스플레이 설정을 맞춰줘야 함. 

- 현재 콘솔 횡비는 W:80, H:30
- `nScreenWidth` 와 `nScreenHeight` 에 설정된 값으로 콘솔 디스플레이를 설정해 줄 것.

```cpp
static constexpr int nScreenWidth = 80; // Console Screen Size X (columns)
static constexpr int nScreenHeight = 30; // Console Screen Size Y (rows)
...
```

## Result Preview

[![Tetris in cpp](https://i.ytimg.com/vi/ZhRBXwRGf7U/hqdefault.jpg?sqp=-oaymwEZCNACELwBSFXyq4qpAwsIARUAAIhCGAFwAQ==&rs=AOn4CLDalQbW8dUJMi2-Gc4Jp71AdCbSBQ)](https://www.youtube.com/embed/ZhRBXwRGf7U)

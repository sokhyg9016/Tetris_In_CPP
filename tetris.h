#pragma once

/*
Simple Tetris implementation in cpp
2020. 06. 07

- using WINAPI
- using cpp and c

[OUTLINES]
- user inputs
- console print & console control
- play background sound (BG)

[NOTE]: 
�ش� ������Ʈ�� �ܼ� �Է�â�� ���� �����س� �����̱� ������ ���� �� �ܼ��� �Ӽ� â���� ���÷��� ������ ������� ��. 
	-> ���� �ܼ� Ⱦ��� 80:30 ��
	-> nScreenWidth �� nScreenHeight�� ������ ������ �ܼ� ���÷��̸� ������ �� ��.
*/

#include <iostream>
#include <thread>
#include <vector>

using namespace std;

#include <stdio.h>
#include <Windows.h>

static wstring tetromino[7];

static constexpr int nFieldWidth = 12; // Play Field Size X (columns)
static constexpr int nFieldHeight = 18; // Play Field Size Y (rows)
static constexpr int nScreenWidth = 80; // Console Screen Size X (columns)
static constexpr int nScreenHeight = 30; // Console Screen Size Y (rows)

static unsigned char* pField = nullptr;

// Rotate shape of tetris per degree
int Rotate(int px, int py, int r);

// Check if a piece of tetromino is fit with play field.
bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY);

// Play or Stop a Background Music
void PlayMusic();
void StopMusic();

// Test the game
void StartGame();

// Game Logic Stuff
static bool bGameOver = false;

static int nCurrentPiece = 1;
static int nCurrentRotation = 0;
static int nCurrentX = nFieldWidth / 2; //someone in the middle or top..
static int nCurrentY = 0;

static bool bKey[4];
static bool bRotateHold = false;

static int nSpeed = 20;
static int nSpeedCounter = 0;
static bool bForceDown = false;
static int nPieceCount = 0;
static int nScore = 0;

static vector<int> vLines;
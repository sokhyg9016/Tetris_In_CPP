#include "tetris.h"


// Rotate shape of tetris per degree
int Rotate(int px, int py, int r)
{
	switch (r % 4) {
	case 0:
		return py * 4 + px;
	case 1:
		return 12 + py - (px * 4);
	case 2:
		return 15 - (py * 4) - px;
	case 3:
		return 3 - py + (px * 4);
	default:
		fprintf(stderr, "Please Check that you entered right parameters..");
		break;
	}

	return 0;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
	for (int px = 0; px < 4; px++) {
		for (int py = 0; py < 4; py++) {

			// Get index into piece
			int pi = Rotate(px, py, nRotation);

			// Get index into field
			int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

			// Checks array's range
			if (nPosX + px >= 0 && nPosX + px < nFieldWidth) {
				if (nPosY + py >= 0 && nPosY + py < nFieldHeight) {

					// if meet the field boundary or other pieces.. 
					if (tetromino[nTetromino][pi] == L'X' && pField[fi] != 0) {
						return false; // fail on first hit
					}
				}
			}
		}
	}

	return true;
}

void PlayMusic() 
{
	PlaySound(TEXT("tetris.wav"), NULL, SND_LOOP | SND_ASYNC);
	cin.get();
}

void StopMusic() 
{
	PlaySound(NULL, 0, 0);
}

void StartGame() 
{
	srand((unsigned int)time(0));
	thread bgMusic(PlayMusic);

	// Create assets
	tetromino[0].append(L"..X...X...X...X.");
	tetromino[1].append(L"..X..XX..X......");
	tetromino[2].append(L".X...XX...X.....");
	tetromino[3].append(L".....XX..XX.....");
	tetromino[4].append(L"..X..XX...X.....");
	tetromino[5].append(L".....XX...X...X.");
	tetromino[6].append(L".....XX..X...X..");

	// Create play field buffer
	pField = new unsigned char[nFieldWidth * nFieldHeight];

	// Board Boundary
	/* '9' to represent the border, otherwise set '0' to represent empty space. */
	for (int x = 0; x < nFieldWidth; x++) // Board Boundary
		for (int y = 0; y < nFieldHeight; y++)
			pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;


	// Create  Screen Buffer
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];

	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) {
		screen[i] = L' ';
	}

	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);

	DWORD dwBytesWritten = 0;

	while (!bGameOver) {

		// Game Timing ================================
		this_thread::sleep_for(50ms); //Game Tick
		nSpeedCounter++;
		bForceDown = (nSpeedCounter == nSpeed); // If the speed is max state, it should be stop..


		// INPUT ======================================
		for (int k = 0; k < 4; k++) {
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0; // R L D Z
		}

		// Game Logic =================================
		nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0; // R
		nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0; // L
		nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0; // D

		if (bKey[3]) {
			nCurrentRotation += (!bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) ? 1 : 0; // Z
			bRotateHold = true;
		}
		else {
			bRotateHold = false;
		}

		if (bForceDown) {

			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) {
				nCurrentY++; // If can, so do it.
			}
			else {

				// It can't! Lock the piece in place
				for (int px = 0; px < 4; px++)
					for (int py = 0; py < 4; py++)
						if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] != L'.')
							pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

				nPieceCount++;
				if (nPieceCount % 10 == 0) {
					if (nSpeed >= 10) {
						nSpeed--;
					}
				}


				// Check have we got any lines
				// Don't need to check whole playing field, only need to check where the last teromino piece existed
				for (int py = 0; py < 4; py++) {

					if (nCurrentY + py < nFieldHeight - 1) {

						// Assume that there is a line..
						bool bLine = true;

						// If any empty spaces in the line this gets set to false so it can't be a line.
						for (int px = 1; px < nFieldWidth - 1; px++) {
							bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;
						}

						if (bLine) {
							for (int px = 1; px < nFieldWidth - 1; px++) {
								pField[(nCurrentY + py) * nFieldWidth + px] = 8;
							}

							PlaySound(TEXT("recycle.wav"), NULL, SND_FILENAME | SND_NOSTOP);
							vLines.push_back(nCurrentY + py);
						}

					}
				}

				nScore += 25;
				if (!vLines.empty()) {
					nScore += (1 << vLines.size()) * 100;
				}

				// Choose next piece (randomly generated and it shows at middle of top.) 
				nCurrentX = nFieldWidth / 2;
				nCurrentY = 0;
				nCurrentRotation = 0;
				nCurrentPiece = rand() % 7;

				// If piece does not fit
				bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
			}

			nSpeedCounter = 0;
		}

		// Draw Field (our playing filed is offset by '2' so just add '2' about each x and y.)
		for (int x = 0; x < nFieldWidth; x++)
			for (int y = 0; y < nFieldHeight; y++)
				screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];


		// Draw Current Piece
		for (int px = 0; px < 4; px++) {
			for (int py = 0; py < 4; py++) {
				if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X') {
					screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 'A';
				}
			}
		}

		// Draw Score
		swprintf_s(&screen[2 * nScreenWidth + nScreenHeight + 6], 16, L"SCORE: %8d", nScore);

		if (!vLines.empty()) {
			// Display Frame (cheekily to draw lines)
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
			this_thread::sleep_for(400ms); // Deplay a bit

			// If a line exists we've got a number for the line which coms back that gives us the the row and 
			// than we want to iterate a column by column across the row .. simply moving the peces down.
			for (auto& v : vLines) {
				for (int px = 1; px < nFieldWidth - 1; px++) {
					for (int py = v; py > 0; py--) {
						pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
					}
					pField[px] = 0;
				}
			}
			vLines.clear();
		}

		// Display Frame
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	// Display Score if game over
	bgMusic.detach();

	CloseHandle(hConsole);
	cout << "Game Over! Your Score is " << nScore << '\n';
	system("pause");
}

#pragma once

#include "Graphics.h"
#include "Sound.h"


class MineField
{
public:
	enum class State
	{
		Fucked,
		Winrar,
		Mineming,
	};

private:
	class Tile
	{
	public:

		enum class StateTile
		{
			Hidden,
			Flagged,
			Revealed

		};
		void SpawnBomb();
		bool HasBomb() const;
		void Draw(const Vei2 screenPos, MineField::State state, Graphics& gfx) const;
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
		void SetNeighborBombCount(int bombCount);

	private:
		StateTile stateTile = StateTile::Hidden;
		bool hasBomb = false;		
		int nNeighborBombs = -1;
	};

public:
	MineField(const Vei2 center, int nMines);
	void Draw(Graphics& gfx) const;
	RectI GetRect() const;
	void OnRevealClick(const Vei2 screenPos);
	void OnFlagClick(const Vei2 screenPos);
	State GetState() const;

private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
	Vei2 ScreenToGrid(const Vei2& screenPos);
	int CountNeighborBombs(const Vei2& gridPos);
	bool GameIsWon() const;
	

private:

	static constexpr int width = 6;
	static constexpr int height = 5;
	static constexpr int borderThickness = 10;
	static constexpr Color borderColor = Colors::Blue;
	Sound sndLose = Sound(L"spayed.wav");

	Vei2 topLeft;

	State state = State::Mineming;
	

	// Tiles in the field
	Tile field[width * height];
};

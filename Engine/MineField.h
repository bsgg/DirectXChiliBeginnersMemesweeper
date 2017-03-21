#pragma once

#include "Graphics.h"


class MineField
{
private:
	class Tile
	{
	public:

		enum class State
		{
			Hidden,
			Flagged,
			Revealed

		};
		void SpawnBomb();
		bool HasBomb() const;
		void Draw(const Vei2 screenPos, bool fucked, Graphics& gfx) const;
		void Reveal();
		bool IsRevealed() const;
		void ToggleFlag();
		bool IsFlagged() const;
		void SetNeighborBombCount(int bombCount);

	private:
		State state = State::Hidden;
		bool hasBomb = false;		
		int nNeighborBombs = -1;
	};
public:
	MineField(const Vei2 center, int nMines);
	void Draw(Graphics& gfx) const;
	RectI GetRect() const;
	void OnRevealClick(const Vei2 screenPos);
	void OnFlagClick(const Vei2 screenPos);
	

private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
	Vei2 ScreenToGrid(const Vei2& screenPos);
	int CountNeighborBombs(const Vei2& gridPos);

private:

	static constexpr int width = 20;
	static constexpr int height = 16;
	static constexpr int borderThickness = 10;
	static constexpr Color borderColor = Colors::Blue;

	Vei2 topLeft;

	bool isFucked = false;

	// Tiles in the field
	Tile field[width * height];
};

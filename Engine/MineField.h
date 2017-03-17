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
		void SpawnMine();
		bool HasMine() const;
		void Draw(const Vei2 screenPos, Graphics& gfx) const;

	private:
		State state = State::Hidden;
		bool hasBomb = false;

	};
public:
	MineField(int nMines);
	void Draw(Graphics& gfx) const;
	RectI GetRect() const;
private:
	Tile& TileAt(const Vei2& gridPos);
	const Tile& TileAt(const Vei2& gridPos) const;
private:

	static constexpr int width = 20;
	static constexpr int height = 16;
	// Tiles in the field
	Tile field[width * height];
};

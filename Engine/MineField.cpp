#include "MineField.h"
#include "SpriteCodex.h"
#include "Vei2.h"
#include <assert.h>
#include <random>
#include <algorithm>

void MineField::Tile::SpawnMine()
{
	// Only spawn a mine if there is no mine
	assert(!hasBomb);

	hasBomb = true;
}

bool MineField::Tile::HasMine() const
{
	return hasBomb;
}

void MineField::Tile::Draw(const Vei2 screenPos, Graphics & gfx) const
{
	switch (state)
	{
	case MineField::Tile::State::Hidden:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case MineField::Tile::State::Flagged:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	case MineField::Tile::State::Revealed:
		if (!hasBomb)
		{
			SpriteCodex::DrawTile0(screenPos, gfx);
		}
		else
		{
			SpriteCodex::DrawTileBomb(screenPos, gfx);
		}

		break;
	default:
		break;
	}
}

void MineField::Tile::Reveal()
{
	// Only call this function if state is hidden
	assert(state == State::Hidden);

	state = State::Revealed;
}

bool MineField::Tile::IsRevealed() const
{
	return (state == State::Revealed);
}

void MineField::Tile::ToggleFlag()
{
	// Only call this function if state is hidden
	assert(!IsRevealed());

	if (state == State::Hidden)
	{
		state = State::Flagged;
	}else
	{
		state = State::Hidden;
	}
}
bool MineField::Tile::IsFlagged() const
{
	return (state == State::Flagged);
}

void MineField::Tile::SetNeighborBombCount(int bombCount)
{
	// Only if this tile is initialized
	assert(nNeighborBombs == -1);

	nNeighborBombs = bombCount;
}



MineField::MineField(int nMines)
{
	// nMines only can be more than 0 and less than the mine field size
	assert(nMines > 0 && (nMines < width * height));

	std::random_device rd;
	std::mt19937 rng(rd());
	// Random position for the mine
	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, height - 1);


	// Create mines until the nMines are filled
	for (int nSpawned = 0; nSpawned < nMines; nSpawned++)
	{
		// Spawn positions until the field doesn't have any mines
		Vei2 spawnPos;
		do
		{
			spawnPos = { xDist(rng), yDist(rng) };

		} while (TileAt(spawnPos).HasMine());
		TileAt(spawnPos).SpawnMine();
	}

	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			TileAt(gridPos).SetNeighborBombCount(CountNeighborBombs(gridPos));
		}
	}
}

void MineField::Draw(Graphics& gfx) const
{
	gfx.DrawRect(GetRect(), SpriteCodex::baseColor);

	for (Vei2 gridPos = { 0,0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			TileAt(gridPos).Draw(gridPos * SpriteCodex::tileSize, gfx);
		}
	}
}

RectI MineField::GetRect() const
{
	return RectI(0,width * SpriteCodex::tileSize,0,height * SpriteCodex::tileSize);
}

void MineField::OnRevealClick(const Vei2 screenPos)
{
	const Vei2 gridPos = ScreenToGrid(screenPos);
	assert(gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < height);

	Tile& tile = TileAt(gridPos);

	if (!tile.IsRevealed() && !tile.IsFlagged())
	{
		TileAt(gridPos).Reveal();
	}
}

void MineField::OnFlagClick(const Vei2 screenPos)
{
	const Vei2 gridPos = ScreenToGrid(screenPos);
	assert(gridPos.x >= 0 && gridPos.x < width && gridPos.y >= 0 && gridPos.y < height);

	Tile& tile = TileAt(gridPos);
	if (!tile.IsRevealed())
	{
		TileAt(gridPos).ToggleFlag();
	}
}

MineField::Tile & MineField::TileAt(const Vei2 & gridPos)
{
	return field[gridPos.y * width + gridPos.x ];
}

const MineField::Tile & MineField::TileAt(const Vei2 & gridPos) const
{
	return field[gridPos.y * width + gridPos.x];
}

Vei2 MineField::ScreenToGrid(const Vei2 & screenPos)
{
	// Convert screen position (pixels) into grid position (tiles)
	return (screenPos/SpriteCodex::tileSize);
}

int MineField::CountNeighborBombs(const Vei2& gridPos)
{
	// Set the boundaries for a gridPos. Maximun 9 tiles covering that gridPos
	// Taking into account the boundaries of the grid
	const int xStart = std::max(0, gridPos.x - 1);
	const int yStart = std::max(0, gridPos.y - 1);
	const int xEnd = std::min(width - 1, gridPos.x + 1);
	const int yEnd = std::min(height - 1, gridPos.y + 1);

	int count = 0;
	for (Vei2 gridPos = { xStart,yStart }; gridPos.y <= yEnd; gridPos.y++)
	{
		for (gridPos.x = xStart; gridPos.x <= xEnd; gridPos.x++)
		{
			if (TileAt(gridPos).HasMine())
			{
				count++;
			}
		}
	}

	return count;
}

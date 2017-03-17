#include "MineField.h"
#include "SpriteCodex.h"
#include "Vei2.h"
#include <assert.h>
#include <random>

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

MineField::Tile & MineField::TileAt(const Vei2 & gridPos)
{
	return field[gridPos.y * width + gridPos.x ];
}

const MineField::Tile & MineField::TileAt(const Vei2 & gridPos) const
{
	return field[gridPos.y * width + gridPos.x];
}

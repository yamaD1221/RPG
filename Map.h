#pragma once

constexpr int MAP_ROWS = 10;
constexpr int MAP_COLS = 20;

enum class MapData
{
	Tile = 0,
	NotTile = 1
};

// 横向き（10行 x 20列）アスキーアート
constexpr char ascii_map[MAP_ROWS][MAP_COLS + 1] =
{
	"+------------------+",
	"|..................|",
	"|.o..ooo...oo..o...|",
	"|.o.oo.oo..o..oo...|",
	"|.o...o....o...o...|",
	"|.ooooo....oooo....|",
	"|.o..o..o..o..o....|",
	"|.o.oo.oo.oo.oo.oo.|",
	"|..................|",
	"+------------------+"
};

class Map
{
private:
	static constexpr bool IsFrameChar(char c) noexcept
	{
		return (c == '|' || c == '-' || c == '+');
	}

	static constexpr MapData CharToMapData(char c) noexcept
	{
		// '.' は通れるタイル、スペースも通れる。それ以外は通れないとみなす
		if (c == '.' || c == ' ') return MapData::Tile;
		return MapData::NotTile;
	}

public:
	static bool IsBlocked(int row, int col) noexcept
	{
		if (row < 0 || row >= MAP_ROWS || col < 0 || col >= MAP_COLS)
			return true;
		return CharToMapData(ascii_map[row][col]) == MapData::NotTile;
	}

	static MapData GetTile(int row, int col) noexcept
	{
		if (row < 0 || row >= MAP_ROWS || col < 0 || col >= MAP_COLS)
			return MapData::NotTile;
		return CharToMapData(ascii_map[row][col]);
	}

	// 追加：マップの元文字を返す（枠や記号をそのまま表示したいときに使う）
	static char GetChar(int row, int col) noexcept
	{
		if (row < 0 || row >= MAP_ROWS || col < 0 || col >= MAP_COLS)
			return '#';
		return ascii_map[row][col];
	}
};
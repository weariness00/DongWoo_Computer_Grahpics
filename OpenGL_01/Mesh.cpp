#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::InitVertax(Vector2 dot[2])
{
	xDot = dot[1].x - dot[0].x + 1;
	yDot = dot[1].y - dot[0].y + 1;

	meshVertax = new Vector2 *[yDot];
	Real_MeshVertax = new Vector2 * [yDot];
	isAcitve = new bool*[yDot];
	for (int i = 0; i < yDot; i++)
	{
		meshVertax[i] = new Vector2[xDot];
		Real_MeshVertax[i] = new Vector2[xDot];
		isAcitve[i] = new bool[xDot];
	}

	for (int y = 0; y < yDot; y++)
	{
		for (int x = 0; x < xDot; x++)
		{
			meshVertax[y][x] = { dot[0].x + x, dot[0].y + y };
			isAcitve[y][x] = true;
		}
	}
}

void Mesh::SetAllAcetive(bool value)
{
	for (int y = 0; y < yDot; y++)
	{
		for (int x = 0; x < xDot; x++)
		{
			isAcitve[y][x] = value;
		}
	}
}
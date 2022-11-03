#include "Cube.h"

ObjectBlock* Cube::cube_Block = nullptr;

Cube::Cube()
{
	if (cube_Block == nullptr)
	{
		cube_Block = new ObjectBlock;
		ReadObj((char*)"Cube.obj", *cube_Block);
	}

	block.vertIndex = cube_Block->vertIndex;
	block.faceIndex = cube_Block->faceIndex;
	block.vertex = cube_Block->vertex;
	block.face = cube_Block->face;

	isActive = true;
}

Cube::~Cube()
{
}


void Cube::Init()
{
	Object::Init();
}

void Cube::Draw()
{
	if (!isActive)
		return;

	MoveMent();

	Object::SetMatrix();

	//glPointSize(5.0f);
	//glDrawArrays(GL_POINTS, 0, block.vertIndex);
	glDrawElements(GL_TRIANGLES, block.faceIndex * 3, GL_UNSIGNED_SHORT, 0);
}

void Cube::MoveMent()
{
	transform.worldPosition += worldSpeed;
	transform.worldRotation += worldRotateSpeed;
}


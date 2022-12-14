#include "Afx.h"

GLuint s_program;
GLuint vertexShader; //--- 버텍스 세이더 객체
GLuint fragmentShader; //--- 프래그먼트 세이더 객체

char default_Cube[] = "Cube.obj";

int windowSize_W = 1080;
int windowSize_H = 860;

bool isFullScreen = false;
bool isOnTimer = false;
bool isErase = false;

bool left_button = false;
bool right_button = false;

int ObjectSpawnCount = 4;

time_t Start_Time;
int Time_Duration;

float PI = 3.14159265;

Vector2 RealPosition(Vector2 pos)
{
	Vector2 real_pos;

	real_pos.x = pos.x / windowSize_W * 2;
	real_pos.y = pos.y / windowSize_H * 2;

	return real_pos;
}

Vector2 Coordinate(Vector2 pos)
{
	Vector2 result;
	result.x = pos.x - windowSize_W / 2;
	result.y = pos.y - windowSize_H / 2;

	return result;
}

Vector2 operator+ (const Vector2 my, const Vector2 other)
{
	Vector2 result;
	result.x = my.x + other.x;
	result.y = my.y + other.y;

	return result;
}

Vector2 operator+ (const Vector2 my, const int other)
{
	Vector2 result;
	result.x = my.x + other;
	result.y = my.y + other;

	return result;
}

Vector2 operator- (const Vector2 my, const Vector2 other)
{
	Vector2 result;
	result.x = my.x - other.x;
	result.y = my.y - other.y;

	return result;
}

Vector2 operator- (const Vector2 my, const int other)
{
	Vector2 result;
	result.x = my.x - other;
	result.y = my.y - other;

	return result;
}

Vector2 operator* (const Vector2 my, const Vector2 other)
{
	Vector2 result;
	result.x = my.x * other.x;
	result.y = my.y * other.y;

	return result;
}

Vector2 operator* (const Vector2 my, const int other)
{
	Vector2 result;
	result.x = my.x * other;
	result.y = my.y * other;

	return result;
}

Vector2 operator/ (const Vector2 my, const Vector2 other)
{
	Vector2 result;
	result.x = my.x / other.x;
	result.y = my.y / other.y;

	return result;
}

Vector2 operator/ (const Vector2 my, const int other)
{
	Vector2 result;
	result.x = my.x / other;
	result.y = my.y / other;

	return result;
}

ostream& operator<< (ostream& outputStream, const Vector2& my)
{
	cout << my.x << ", " << my.y;
	return outputStream;
}

Position2 operator+(const Position2 my, const Position2 other)
{
	Position2 result;
	result.x = my.x + other.x;
	result.y = my.y + other.y;

	return result;
}

ostream& operator<< (ostream& outputStream, const vec3& my)
{
	cout << my.x << ", " << my.y << ", " << my.z;
	return outputStream;
}

ostream& operator<< (ostream& outputStream, const Face& my)
{
	cout << my.a << ", " << my.b << ", " << my.c;
	return outputStream;
}

vec3 operator* (const vec3 my, const float other)
{
	vec3 result = my;
	result.x = my.x * other;
	result.y = my.y * other;
	result.z = my.z * other;

	return result;
}

vec3 operator/ (const vec3 my, const float other)
{
	vec3 result = my;
	result.x = my.x / other;
	result.y = my.y / other;
	result.z = my.z / other;

	return result;
}


bool operator== (const vec3 my, const float other)
{
	if (my.x != other) return false;
	if (my.y != other)return false;
	if (my.z != other)return false;

	return true;
}

Vector2 window_RealPos = RealPosition({ 1, 1 });
Vector2 StartMouse = { 0,0 };

float DistanceVec3(const vec3 my, const vec3 other)
{
	return sqrt(abs(pow(my.x - other.x, 2)) +
		abs(pow(my.y - other.y, 2)) +
		abs(pow(my.z - other.z, 2))
	);
}


#pragma region Defualt_Funtion

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}

void make_vertexShaders()
{
	GLchar* vertexSource;
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	//--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
	vertexSource = filetobuf("Vertex.glsl");

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}

void make_fragmentShaders()
{
	GLchar* fragmentSource;
	//--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
	fragmentSource = filetobuf("Fragment.glsl"); // 프래그세이더 읽어오기

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);
	glCompileShader(fragmentShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}

void InitShader()
{
	GLint result;
	GLchar errorLog[512];

	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexShader);
	glAttachShader(s_program, fragmentShader);
	glLinkProgram(s_program);
	glGetProgramiv(s_program, GL_LINK_STATUS, &result); // ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(s_program, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결 실패\n" << errorLog << endl;
		return;
	}
	//--- 세이더 삭제하기
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//--- Shader Program 사용하기
	glUseProgram(s_program);
}

void FrameTimer(int value)
{
	glutPostRedisplay();
}


void ReadObj(char* fileName, ObjectBlock& block)
{
	FILE* obj;
	obj = fopen(fileName, "r");

	//--- 1. 전체 버텍스 개수 및 삼각형 개수 세기
	char count[100];
	int vertexNum = 0;
	int faceNum = 0;
	while (!feof(obj)) {
		fscanf(obj, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			faceNum += 1;
		memset(count, '\0', sizeof(count));
	}

	//--- 2. 메모리 할당
	block.vertex = new vec3[vertexNum];
	block.face = new Face[faceNum];
	unsigned short faces[3] = {};

	//--- 3. 할당된 메모리에 각 버텍스, 페이스 정보 입력
	obj = fopen(fileName, "r");
	while (!feof(obj)) {
		fscanf(obj, "%s", count);
		if (count[0] == 'v' && count[1] == '\0') {
			fscanf(obj, "%f %f %f",
				&block.vertex[block.vertIndex].x,
				&block.vertex[block.vertIndex].y,
				&block.vertex[block.vertIndex].z);
			block.vertIndex++;
		}
		else if (count[0] == 'f' && count[1] == '\0') {

			unsigned short vt;
			unsigned short vn;
			for (unsigned short& i : faces)
			{
				fscanf(obj, "%hu", &i);
				fgetc(obj);
				fscanf(obj, "%hu", &vt);
				fgetc(obj);
				fscanf(obj, "%hu", &vn);

				--i;
			}
			block.face[block.faceIndex] = { faces[0], faces[1], faces[2] };
			block.faceIndex++;	
			//fscanf(obj, "%f %f %f",
			//	&block.face[block.faceIndex].x,
			//	&block.face[block.faceIndex].y,
			//	&block.face[block.faceIndex].z);
			//cout << block.face[block.faceIndex] << endl;
			//block.faceIndex++;
		}
	}
}

#pragma endregion

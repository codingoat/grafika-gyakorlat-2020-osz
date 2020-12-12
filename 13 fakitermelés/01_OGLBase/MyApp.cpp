﻿#include "MyApp.h"

#include <math.h>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <sstream>     // ui strings

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
#include "includes/GLUtils.hpp"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_mesh = nullptr;
}

CMyApp::~CMyApp(void)
{
	std::cout << "dtor!\n";
}

void CMyApp::InitCube()
{
	//struct Vertex{ glm::vec3 position; glm::vec3 normals; glm::vec2 texture; };
	std::vector<Vertex>vertices;
	
	//front									 
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(0, 0, 1), glm::vec2(1, 1) });
	//back
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(0, 0, -1), glm::vec2(1, 1) });
	//right									 
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(1, 0, 0), glm::vec2(1, 1) });
	//left									 
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(-1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(-1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(-1, 0, 0), glm::vec2(1, 1) });
	//top									 
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5), glm::vec3(0, 1, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5), glm::vec3(0, 1, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5), glm::vec3(0, 1, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5), glm::vec3(0, 1, 0), glm::vec2(1, 1) });
	//bottom								 
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0, -1, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5), glm::vec3(0, -1, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5), glm::vec3(0, -1, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5), glm::vec3(0, -1, 0), glm::vec2(1, 1) });

	std::vector<int> indices(36);
	int index = 0;
	for (int i = 0; i < 6 * 4; i += 4)
	{
		indices[index + 0] = i + 0;
		indices[index + 1] = i + 1;
		indices[index + 2] = i + 2;
		indices[index + 3] = i + 1;
		indices[index + 4] = i + 3;
		indices[index + 5] = i + 2;
		index += 6;
	}

	//
	// geometria definiálása (std::vector<...>) és GPU pufferekbe való feltöltése BufferData-val
	//

	// vertexek pozíciói:
	/*
	Az m_CubeVertexBuffer konstruktora már létrehozott egy GPU puffer azonosítót és a most következõ BufferData hívás ezt
	1. bind-olni fogja GL_ARRAY_BUFFER target-re (hiszen m_CubeVertexBuffer típusa ArrayBuffer) és
	2. glBufferData segítségével áttölti a GPU-ra az argumentumban adott tároló értékeit

	*/

	m_CubeVertexBuffer.BufferData(vertices);

	// és a primitíveket alkotó csúcspontok indexei (az elõzõ tömbökbõl) - triangle list-el való kirajzolásra felkészülve
	m_CubeIndices.BufferData(indices);

	// geometria VAO-ban való regisztrálása
	m_CubeVao.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_CubeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attribútum: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a következõ csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_CubeVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_CubeVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_CubeVertexBuffer },
		},
		m_CubeIndices
	);
}

glm::vec3 CMyApp::GetPos(float u, float v)
{
	return glm::vec3(u, sin(u * 10), -v);
}

glm::vec3 CMyApp::GetNorm(float u, float v)
{
	glm::vec3 du = GetPos(u + 0.01, v) - GetPos(u - 0.01, v);
	glm::vec3 dv = GetPos(u, v + 0.01) - GetPos(u, v - 0.01);

	return glm::normalize(glm::cross(du, dv));
}


void CMyApp::InitPlane()
{
	std::vector<Vertex> vert((N + 1) * (M + 1));
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			vert[i + j * (N + 1)].t = glm::vec2(u, v);
			vert[i + j * (N + 1)].p = GetPos(u, v);
			vert[i + j * (N + 1)].n = GetNorm(u, v);
		}

	std::vector<int> indices(3 * 2 * (N) * (M));
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
		{
			indices[6 * i + j * 3 * 2 * (N)+0] = (i)+(j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+1] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+2] = (i)+(j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+4] = (i + 1) + (j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+5] = (i)+(j + 1) * (N + 1);
		}

	m_PlaneVbo.BufferData(vert);
	m_PlaneIndices.BufferData(indices);
	m_PlaneVao.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_CubeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attribútum: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a következõ csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_PlaneVbo },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_PlaneVbo },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_PlaneVbo },
		},
		m_PlaneIndices
	);
}


glm::vec3 CMyApp::GetSpherePos(float u, float v)
{
	u *= float(2 * M_PI);
	v *= float(M_PI);
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	float r = 1;

	return glm::vec3(r * cu * sv, r * cv, r * su * sv);
}

glm::vec3 CMyApp::GetSphereNorm(float u, float v)
{
	u *= float(2 * M_PI);
	v *= float(M_PI);
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);

	return glm::vec3(cu * sv, cv, su * sv);
}



void CMyApp::InitSphere()
{
	std::vector<Vertex> vert((N + 1) * (M + 1));
	for (int i = 0; i <= N; ++i)
		for (int j = 0; j <= M; ++j)
		{
			float u = i / (float)N;
			float v = j / (float)M;

			vert[i + j * (N + 1)].t = glm::vec2(u, v);
			vert[i + j * (N + 1)].p = GetSpherePos(u, v);
			vert[i + j * (N + 1)].n = GetSphereNorm(u, v);
		}

	std::vector<int> indices(3 * 2 * (N) * (M));
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < M; ++j)
		{
			indices[6 * i + j * 3 * 2 * (N)+0] = (i)+(j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+1] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+2] = (i)+(j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+3] = (i + 1) + (j) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+4] = (i + 1) + (j + 1) * (N + 1);
			indices[6 * i + j * 3 * 2 * (N)+5] = (i)+(j + 1) * (N + 1);
		}

	m_SphereVbo.BufferData(vert);
	m_SphereIndices.BufferData(indices);
	m_SphereVao.Init(
		{
			// 0-ás attribútum "lényegében" glm::vec3-ak sorozata és az adatok az m_CubeVertexBuffer GPU pufferben vannak
			{ CreateAttribute<		0,						// attribútum: 0
									glm::vec3,				// CPU oldali adattípus amit a 0-ás attribútum meghatározására használtunk <- az eljárás a glm::vec3-ból kikövetkezteti, hogy 3 darab float-ból áll a 0-ás attribútum
									0,						// offset: az attribútum tároló elejétõl vett offset-je, byte-ban
									sizeof(Vertex)			// stride: a következõ csúcspont ezen attribútuma hány byte-ra van az aktuálistól
								>, m_SphereVbo },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_SphereVbo },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_SphereVbo },
		},
		m_SphereIndices
	);
}


void CMyApp::InitSkyBox()
{
	m_SkyboxPos.BufferData(
		std::vector<glm::vec3>{
		// hátsó lap
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, -1),
		// elülsõ lap
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, 1),
	}
	);

	// és a primitíveket alkotó csúcspontok indexei (az elõzõ tömbökbõl) - triangle list-el való kirajzolásra felkészülve
	m_SkyboxIndices.BufferData(
		std::vector<int>{
			// hátsó lap
			0, 1, 2,
			2, 3, 0,
			// elülsõ lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// alsó
			1, 0, 4,
			1, 4, 5,
			// felsõ
			3, 2, 6,
			3, 6, 7,
	}
	);

	// geometria VAO-ban való regisztrálása
	m_SkyboxVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_SkyboxPos },
		}, m_SkyboxIndices
	);

	// skybox texture
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glGenTextures(1, &m_skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	TextureFromFileAttach("assets/xpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	TextureFromFileAttach("assets/xneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	TextureFromFileAttach("assets/ypos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	TextureFromFileAttach("assets/yneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	TextureFromFileAttach("assets/zpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	TextureFromFileAttach("assets/zneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

void CMyApp::InitBackground()
{
	m_BackgroundVbo.BufferData(
		std::vector<Vertex>{
			// hátsó lap
			{ glm::vec3(-1, -1, -1), glm::vec3(0, 0, 1), glm::vec2(0, 0) }, // 0
			{ glm::vec3(1, -1, -1),  glm::vec3(0, 0, 1), glm::vec2(1, 0) },
			{ glm::vec3(1, 1, -1),   glm::vec3(0, 0, 1), glm::vec2(1, 1) },
			{ glm::vec3(-1, 1, -1),  glm::vec3(0, 0, 1), glm::vec2(0, 1) }, // 3
			// jobb oldali lap
			{ glm::vec3(1, -1, -1),  glm::vec3(-1, 0, 0), glm::vec2(0, 0) }, // 4
			{ glm::vec3(1, -1, 1),   glm::vec3(-1, 0, 0), glm::vec2(1, 0) },
			{ glm::vec3(1, 1, 1),    glm::vec3(-1, 0, 0), glm::vec2(1, 1) },
			{ glm::vec3(1, 1, -1),   glm::vec3(-1, 0, 0), glm::vec2(0, 1) }, // 7
			// elülsõ lap		 	 
			{ glm::vec3(1, -1, 1),   glm::vec3(0, 0, -1), glm::vec2(0, 0) }, // 8
			{ glm::vec3(-1, -1, 1),  glm::vec3(0, 0, -1), glm::vec2(1, 0) },
			{ glm::vec3(-1, 1, 1),   glm::vec3(0, 0, -1), glm::vec2(1, 1) },
			{ glm::vec3(1, 1, 1),    glm::vec3(0, 0, -1), glm::vec2(0, 1) }, // 11
			// bal oldali lap
			{ glm::vec3(-1, -1, 1),  glm::vec3(0, 0, -1), glm::vec2(0, 0) }, // 12
			{ glm::vec3(-1, -1, -1), glm::vec3(0, 0, -1), glm::vec2(1, 0) },
			{ glm::vec3(-1, 1, -1),  glm::vec3(0, 0, -1), glm::vec2(1, 1) },
			{ glm::vec3(-1, 1, 1),   glm::vec3(0, 0, -1), glm::vec2(0, 1) } // 15
	});

	m_BackgroundIndices.BufferData(
		std::vector<int>{
			// hátsó lap
			0, 1, 2,
			2, 3, 0,
			// jobb
			4, 5, 6,
			6, 7, 4,
			// elülső
			8, 9, 10,
			10, 11, 8,
			// jobb
			12, 13, 14,
			14, 15, 12
	});

	m_BackgroundVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(Vertex)>, m_BackgroundVbo },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_BackgroundVbo },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_BackgroundVbo },
		},
		m_BackgroundIndices
	);
}

void CMyApp::InitShaders()
{
	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
	});

	// attributomok osszerendelese a VAO es shader kozt
	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
		{ 2, "vs_in_tex" },				// VAO 2-es csatorna menjen a vs_in_tex-be
	});

	m_program.LinkProgram();

// shader program rövid létrehozása, egyetlen függvényhívással a fenti három:
m_programSkybox.Init(
	{
		{ GL_VERTEX_SHADER, "skybox.vert" },
		{ GL_FRAGMENT_SHADER, "skybox.frag" }
	},
		{
			{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		}
		);

m_programAxes.Init(
	{
		{ GL_VERTEX_SHADER, "axes.vert"},
		{ GL_FRAGMENT_SHADER, "axes.frag"}
	},
	{}
);
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	InitShaders();
	InitCube();
	InitSkyBox();
	InitPlane();
	InitSphere();
	InitBackground();

	// egyéb textúrák betöltése
	m_woodTexture.FromFile("assets/wood.jpg");
	m_suzanneTexture.FromFile("assets/marron.jpg");
	m_grassTexture.FromFile("assets/grass.jpg");
	m_leavesTexture.FromFile("assets/leaves.jpg");
	m_barkTexture.FromFile("assets/bark.jpg");
	m_forestTexture.FromFile("assets/forest.jpg");

	// mesh betoltese
	m_mesh = ObjParser::parse("assets/Suzanne.obj");
	m_mesh->initBuffers();

	m_cylinderMesh = ObjParser::parse("assets/henger.obj");
	m_cylinderMesh->initBuffers();

	// kamera
	m_camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);

	//random
	srand(time(NULL));

	ResetForest();

	return true;
}

void CMyApp::ResetForest()
{
	for (int i = 0; i < 20; i++)
	{
		treeLines[i] = false;
		for (int j = 0; j < 20; j++)
		{
			trees[i][j] = 0;
		}
	}
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_skyboxTexture);

	delete m_mesh;
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	treeDeltaTime = delta_time / treeGrowthLength;
	yearTime += delta_time / yearLength * 4;
	yearTime = fmod(yearTime, 4); // yearTime maradeka 4-el osztva: yearTime [0, 4)
	newTreeTime += delta_time;

	bool plantTrees = false;
	if (newTreeTime > 2)
	{
		newTreeTime -= 2;
		plantTrees = true;
	}

	// tree growth and planting
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (trees[i][j] >= maxTreeSize)
				trees[i][j] = 0;
			else if (trees[i][j] > 0)
				trees[i][j] += treeDeltaTime;
			else if (plantTrees && (rand() % 20) == 0 && !treeLines[i])
				trees[i][j] = 0.05f;
		}
	}

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}

void CMyApp::DrawTree(glm::mat4& treeWorld)
{
	m_program.Use();
	glm::mat4 viewProj = m_camera.GetViewProj();


	m_SphereVao.Bind();
	glm::mat4 sphereWorld = treeWorld * glm::translate(glm::vec3(0, 4, 0));
	m_program.SetUniform("MVP", viewProj * sphereWorld);
	m_program.SetUniform("world", sphereWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(sphereWorld)));
	m_program.SetTexture("texImage", 0, m_leavesTexture);


	glDrawElements(GL_TRIANGLES, 3 * 2 * (N) * (M), GL_UNSIGNED_INT, 0);


	glm::mat4 cylinderWorld = treeWorld * glm::translate(glm::vec3(0, 2, 0));
	m_program.SetUniform("MVP", viewProj * cylinderWorld);
	m_program.SetUniform("world", cylinderWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(cylinderWorld)));
	m_program.SetTexture("texImage", 0, m_barkTexture);


	m_cylinderMesh->draw();

	m_program.Unuse();

}


// 0 <= param <= 4
glm::vec3 CMyApp::GetLightColor(float param)
{
	if (param < 0) return lightColors[0];
	if (param >= 4) return lightColors[4];

	int interval = (int)param;
	float localT = param - interval;

	return lightColors[interval] * (1 - localT) + lightColors[interval + 1] * (localT);
}

void CMyApp::Render()
{
	glm::vec3 currentLightColor = GetLightColor(yearTime);

	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewProj = m_camera.GetViewProj();

	//Suzanne
	/*glm::mat4 suzanneWorld = glm::mat4(1.0f);
	m_program.Use();
	m_program.SetTexture("texImage", 0, m_suzanneTexture);
	m_program.SetUniform("MVP", viewProj * suzanneWorld);
	m_program.SetUniform("world", suzanneWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(suzanneWorld)));

	m_program.SetUniform("Ld", currentLightColor);
	m_program.SetUniform("Ls", 1.5f * currentLightColor);
	m_program.SetUniform("cam_pos", m_camera.GetEye());

	m_mesh->draw();*/


	m_program.Use();
	m_program.SetUniform("Ld", currentLightColor);
	m_program.SetUniform("Ls", 1.5f * currentLightColor);
	m_program.SetUniform("cam_pos", m_camera.GetEye());

	//plane

	m_PlaneVao.Bind();
	glm::mat4 planeWorld = glm::translate(glm::vec3(-10, 0, 10)) * glm::scale(glm::vec3(20, 1, 20));
	m_program.SetUniform("MVP", viewProj * planeWorld);
	m_program.SetUniform("world", planeWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(planeWorld)));
	m_program.SetTexture("texImage", 0, m_grassTexture);

	glDrawElements(GL_TRIANGLES, 3 * 2 * (N) * (M), GL_UNSIGNED_INT, 0);



	// background
	m_BackgroundVao.Bind();
	glm::mat4 backgroundWorld = glm::translate(glm::vec3(0, 1.5, 0)) * glm::scale(glm::vec3(10, 3, 10));
	m_program.SetUniform("MVP", viewProj * backgroundWorld);
	m_program.SetUniform("world", backgroundWorld);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(backgroundWorld)));
	m_program.SetTexture("texImage", 0, m_forestTexture);

	glDrawElements(GL_TRIANGLES, 3 * 2 * 4, GL_UNSIGNED_INT, 0);


	// fak

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (trees[i][j] > 0)
			{
				float u = i / (float)20;
				float v = j / (float)20;

				glm::vec3 treePos = glm::translate(glm::vec3(-10, 0, 10)) * glm::scale(glm::vec3(20, 1, 20)) * glm::vec4(GetPos(u, v), 1);

				DrawTree(glm::translate(treePos) * glm::scale(glm::vec3(trees[i][j], trees[i][j], trees[i][j])));
			}
		}
	}


	/*m_programAxes.Use();
	m_programAxes.SetUniform("MVP", viewProj * glm::translate(glm::vec3(1, 0, 0)));
	glDrawArrays(GL_LINES, 0, 6);*/

	// skybox
	// mentsük el az elõzõ Z-test eredményt, azaz azt a relációt, ami alapján update-eljük a pixelt.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// most kisebb-egyenlõt használjunk, mert mindent kitolunk a távoli vágósíkokra
	glDepthFunc(GL_LEQUAL);

	m_SkyboxVao.Bind();
	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", viewProj * glm::translate(m_camera.GetEye()));

	// cube map textúra beállítása 0-ás mintavételezõre és annak a shaderre beállítása
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(m_programSkybox.GetLocation("skyboxTexture"), 0);
	// az elõzõ három sor <=> m_programSkybox.SetCubeTexture("skyboxTexture", 0, m_skyboxTexture);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	m_programSkybox.Unuse();

	// végül állítsuk vissza
	glDepthFunc(prevDepthFnc);


	// 1. feladat: készíts egy vertex shader-fragment shader párt, ami tárolt geometria _nélkül_ kirajzol egy tetszõleges pozícióba egy XYZ tengely-hármast,
	//			   ahol az X piros, az Y zöld a Z pedig kék!

	//ImGui Testwindow
	ImGui::ShowTestWindow();

	if (ImGui::Begin("fakitermeles"))
	{
		ImGui::InputFloat("ev hossza", &yearLength, 1, 0.5);
		yearLength = std::max(yearLength, 1.0f);
		ImGui::InputFloat("fa novesi ideje", &treeGrowthLength, 1, 0.5);
		treeGrowthLength = std::max(treeGrowthLength, 1.0f);
		ImGui::SliderFloat("kivagando fak merete", &maxTreeSize, 0.5, 1.2);

		if (ImGui::Button("erdoirtas")) ResetForest();

		ImGui::Text("Fasorok letiltasa");

		for (int i = 0; i < 20; i++)
		{
 			std::stringstream title; // #include<sstream>
			title << (i + 1) << ". fasor"; 

			ImGui::Selectable(title.str().c_str(), &treeLines[i]);
		}

		ImGui::ColorEdit3("nyar", (float*)&lightColors[0]);
		lightColors[4] = lightColors[0];
		ImGui::ColorEdit3("tel", (float*)&lightColors[2]);
	
	}
	ImGui::End();
}



void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}

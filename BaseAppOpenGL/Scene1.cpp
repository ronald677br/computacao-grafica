#include "Scene1.h"


CScene1::CScene1()
{
	pCamera = NULL;
	pTexto = NULL;
	pTextures = NULL;

	fLowLight[0] = 0.5;
	fLowLight[1] = 0.5;
	fLowLight[2] = 0.5;
	fLowLight[3] = 1.0;
	
	bIsWireframe = false;
	bIsCameraFPS = true;

	iFPS = 0;
	iFrames = 0;
	ulLastFPS = 0;
	szTitle[256] = 0;

	// Cria gerenciador de impressão de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(0.0f, 1.0f, 20.0f, 0.5f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;

	glPushAttrib(GL_TEXTURE_BIT);
	// Carrega todas as texturas
	pTextures = new CTexture();	
	pTextures->CreateTextureMipMap(0, "../Scene1/back.png");
	pTextures->CreateTextureMipMap(1, "../Scene1/front.png");
	pTextures->CreateTextureMipMap(2, "../Scene1/bottom.png");
	pTextures->CreateTextureMipMap(3, "../Scene1/top.png");
	pTextures->CreateTextureMipMap(4, "../Scene1/left.png");
	pTextures->CreateTextureMipMap(5, "../Scene1/right.png");
	pTextures->CreateTextureMipMap(6, "../Scene1/parede.png");
	pTextures->CreateTextureMipMap(7, "../Scene1/BEZAO.png");
	pTextures->CreateTextureTGA(8, "../Scene1/palmtree.tga");
	pTextures->CreateTextureTGA(9, "../Scene1/desertgrass.tga");
	pTextures->CreateTextureTGA(10, "../Scene1/Fundo1.jpg");
	pTextures->CreateTextureTGA(11, "../Scene1/Fundo2.jpg");
	pTextures->CreateTextureTGA(12, "../Scene1/Fundo3.jpg");
	pTextures->CreateTextureTGA(13, "../Scene1/chao.png");

	glPopAttrib();
	
	disp = 0.0f;
	fPosX = 0.0f;
	fPosY = 0.0f;
	fPosZ = 0.0f;
	fMovementFactor = 0.01f;	

	fAngle = 0.0f;		
	

	for (auto& a : modelsAssimp) a = NULL;

	//modelsAssimp[0] = new CModel_ASSIMP(CModel_ASSIMP::IMMEDIATE, "../Scene1/Castle/Castle.obj");
	modelsAssimp[1] = new CModel_ASSIMP(CModel_ASSIMP::IMMEDIATE, "../Scene1/escuro.obj");
	modelsAssimp[2] = new CModel_ASSIMP(CModel_ASSIMP::IMMEDIATE, "../Scene1/poste.obj");
	modelsAssimp[3] = new CModel_ASSIMP(CModel_ASSIMP::IMMEDIATE, "../Scene1/entire_map.obj");
	modelsAssimp[4] = new CModel_ASSIMP(CModel_ASSIMP::IMMEDIATE, "../Scene1/parede_esquerda_baixo.obj");
	modelsAssimp[5] = new CModel_ASSIMP(CModel_ASSIMP::IMMEDIATE, "../Scene1/escuro.obj");
	modelsAssimp[6] = new CModel_ASSIMP(CModel_ASSIMP::IMMEDIATE, "../Scene1/porta-meio.obj");
	modelsAssimp[7] = new CModel_ASSIMP(CModel_ASSIMP::IMMEDIATE, "../Scene1/barris.obj");
	modelsAssimp[8] = new CModel_ASSIMP(CModel_ASSIMP::IMMEDIATE, "../Scene1/box.obj");
	modelsAssimp[9] = new CModel_ASSIMP(CModel_ASSIMP::IMMEDIATE, "../Scene1/chao_escuro.obj");


	pModel3DS = NULL;
	//pModel3DS = new CModel_3DS();
	//pModel3DS->Load("../Scene1/Casa.3DS");
	
	pTerreno = NULL;
	pTerreno = new CTerreno();
	pTerreno->LoadRawFile("../Scene1/terreno.raw");

	pTerreno3DS = NULL;
	//pTerreno3DS = new CModel_3DS();
	//pTerreno3DS->Load("../Scene1/terreno.3ds");
}


CScene1::~CScene1(void)
{
	if (pTexto)
	{
		delete pTexto;
		pTexto = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}

	if (modelsAssimp)
	{
		delete modelsAssimp;
		for (int i = 0; i < 100; i++) {
			modelsAssimp[i] = NULL;
		}
	}
		
	if (pModel3DS)
	{
		delete pModel3DS;
		pModel3DS = NULL;
	}

	if (pTerreno)
	{
		delete pTerreno;
		pTerreno = NULL;
	}

	if (pTerreno3DS)
	{
		delete pTerreno3DS;
		pTerreno3DS = NULL;
	}
	
}




int CScene1::DrawGLScene(void)	// Função que desenha a cena
{
	// Get FPS
	if (GetTickCount() - ulLastFPS >= 1000)	// When A Second Has Passed...
	{
		ulLastFPS = GetTickCount();				// Update Our Time Variable
		iFPS = iFrames;							// Save The FPS
		iFrames = 0;							// Reset The FPS Counter
	}
	iFrames++;									// FPS counter

	pTimer->Update();							// Atualiza o timer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual


	// Seta as posições da câmera
	pCamera->setView();

	// Desenha grid 
	//Draw3DSGrid(20.0f, 20.0f);

	// Desenha os eixos do plano cartesiano
	//DrawAxis();

	// Modo FILL ou WIREFRAME (pressione barra de espaço)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Primeiro definir a cor da atual para BRANCO 100%
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);

	// Habilita Blending
	glEnable(GL_BLEND);
	// Configura função de Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Seta cor do quadrado azul
	glColor4f(1.0f, 1.0f, 1.0f, 0.6f);

	CreateSkyBox(0.0f, 100.0f, 0.0f, 1500.0f, 1500.0f, 1500.0f, pTextures);
	
	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR, fLowLight);
	glFogf(GL_FOG_START, 0.0f);
	glFogf(GL_FOG_END, 250.0f);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	
	for (int i = 0; i < 100; i++) {
		if (modelsAssimp[i] != NULL) {
			glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.0f);
				modelsAssimp[i]->Draw();
			glPopMatrix();
		}
	}


	// Palmeira 1
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.2);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	pTextures->ApplyTexture(8);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 10.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(60.0f, 30.0f, -60.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(140.0f, 30.0f, -60.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(140.0f, 150.0f, -60.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(60.0f, 150.0f, -60.0f);

	glTexCoord2d(0.0f, 0.0f); glVertex3f(100.0f, 30.0f, -100.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(100.0f, 30.0f, -20.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(100.0f, 150.0f, -20.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(100.0f, 150.0f, -100.0f);
	glEnd();
	glPopMatrix();

	glDisable(GL_ALPHA_TEST);
	// Fim Palmeira 1

	// Palmeira 2
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.2);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	pTextures->ApplyTexture(8);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 10.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-100.0f, 30.0f, -50.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(0.0f, 30.0f, -50.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(0.0f, 130.0f, -50.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-100.0f, 130.0f, -50.0f);

	glTexCoord2d(0.0f, 0.0f); glVertex3f(-50.0f, 30.0f, -100.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-50.0f, 30.0f, 0.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-50.0f, 130.0f, 0.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-50.0f, 130.0f, -100.0f);
	glEnd();
	glPopMatrix();

	glDisable(GL_ALPHA_TEST);
	//Fim Palmeira 2

	//Palmeira 3
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.2);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	pTextures->ApplyTexture(8);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 10.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(60.0f, 30.0f, 50.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(160.0f, 30.0f, 50.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(160.0f, 120.0f, 50.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(60.0f, 120.0f, 50.0f);

	glTexCoord2d(0.0f, 0.0f); glVertex3f(110.0f, 30.0f, 0.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(110.0f, 30.0f, 100.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(110.0f, 120.0f, 100.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(110.0f, 120.0f, 0.0f);
	glEnd();
	glPopMatrix();

	glDisable(GL_ALPHA_TEST);
	//Fim Palmeira 3

	//Grama Seca 1
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.2);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	pTextures->ApplyTexture(9);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 10.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(95.0f, 32.0f, 55.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(115.0f, 32.0f, 55.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(115.0f, 54.0f, 55.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(95.0f, 49.0f, 55.0f);

	glTexCoord2d(0.0f, 0.0f); glVertex3f(105.0f, 32.0f, 65.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(105.0f, 32.0f, 45.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(105.0f, 49.0f, 45.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(105.0f, 54.0f, 65.0f);
	glEnd();
	glPopMatrix();

	glDisable(GL_ALPHA_TEST);
	//Fim Grama Seca 1
	
	//Grama Seca 2
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.2);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	pTextures->ApplyTexture(9);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 10.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(100.0f, 32.0f, -57.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(120.0f, 32.0f, -57.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(120.0f, 49.0f, -57.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(100.0f, 44.0f, -57.0f);

	glTexCoord2d(0.0f, 0.0f); glVertex3f(110.0f, 32.0f, -47.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(110.0f, 32.0f, -67.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(110.0f, 44.0f, -67.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(110.0f, 49.0f, -47.0f);
	glEnd();
	glPopMatrix();

	glDisable(GL_ALPHA_TEST);
	//Fim Grama Seca 2

	//Grama Seca 3
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.2);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	pTextures->ApplyTexture(9);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 10.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(90.0f, 32.0f, -70.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(110.0f, 32.0f, -70.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(110.0f, 49.0f, -70.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(90.0f, 44.0f, -70.0f);

	glTexCoord2d(0.0f, 0.0f); glVertex3f(100.0f, 32.0f, -60.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(100.0f, 32.0f, -80.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(100.0f, 44.0f, -80.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(100.0f, 49.0f, -60.0f);
	glEnd();
	glPopMatrix();

	glDisable(GL_ALPHA_TEST);
	//Fim Grama Seca 3

	//Grama Seca 4
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.2);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	pTextures->ApplyTexture(9);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 10.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-53.0f, 32.0f, -57.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-73.0f, 32.0f, -57.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-73.0f, 53.0f, -57.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-53.0f, 57.0f, -57.0f);

	glTexCoord2d(0.0f, 0.0f); glVertex3f(-63.0f, 32.0f, -47.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-63.0f, 32.0f, -67.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-63.0f, 57.0f, -67.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-63.0f, 53.0f, -47.0f);
	glEnd();
	glPopMatrix();

	glDisable(GL_ALPHA_TEST);
	//Fim Grama Seca 4

	//Grama Seca 5
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.2);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	pTextures->ApplyTexture(9);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 10.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-23.0f, -6.0f, 50.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-2.0f, -6.0f, 50.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-3.0f, 20.0f, 50.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-24.0f, 20.0f, 50.0f);

	glTexCoord2d(0.0f, 0.0f); glVertex3f(-15.0f, -6.0f, 62.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-15.0f, -6.0f, 43.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-15.0f, 20.0f, 38.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-15.0f, 20.0f, 58.0f);
	glEnd();
	glPopMatrix();

	glDisable(GL_ALPHA_TEST);
	//Fim Grama Seca 5

	// Desabilita Blending
	glDisable(GL_BLEND);

	//ParedePrincipal
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	pTextures->ApplyTexture(6);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(117.0f, 75.5f, -80.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(117.0f, 75.5f, 79.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(117.0f, 0.5f, 79.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(117.0f, 0.5f, -80.5f);
	glEnd();
	glPopMatrix();
	//FIM ParedePrincipal

	//Fundo 1
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	pTextures->ApplyTexture(10);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-25.0f, 0.5f, 185.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-68.0f, 0.5f, 185.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-68.0f, 50.0f, 185.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-25.0f, 50.0f, 185.0f);
	glEnd();
	glPopMatrix();
	//FIM Fundo 1

	//Fundo 2
	//Areia
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	pTextures->ApplyTexture(13);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-72.0f, 0.0f, -13.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-72.0f, 0.0f, 42.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-84.0f, 0.0f, 42.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-84.0f, 0.0f, -13.0f);
	glEnd();
	glPopMatrix();
	//Fim Areia

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	pTextures->ApplyTexture(11);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-83.0f, 0.0f, -13.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-83.0f, 0.0f, 42.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-83.0f, 59.0f, 42.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-83.0f, 59.0f, -13.0f);
	glEnd();
	glPopMatrix();
	//FIM Fundo 2

	//Fundo 3
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	pTextures->ApplyTexture(12);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-73.0f, 30.0f, -119.0f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-22.0f, 30.0f, -119.0f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-22.0f, 75.0f, -119.0f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-73.0f, 75.0f, -119.0f);
	glEnd();
	glPopMatrix();
	//FIM Fundo 3

	glDisable(GL_TEXTURE_2D);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glPushAttrib(GL_TEXTURE_BIT); // Salva o estado atual de texturização

	// Configurar a unidade de textura 0
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(6);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);

	// Configurar a unidade de textura 1
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	pTextures->ApplyTexture(7);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);


	// Desenha o objeto com multi-textura
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);

	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 0.0f);
	glVertex3f(-20.0f, 50.0f, 69.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 0.2f, 0.0f);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 0.0f);
	glVertex3f(0.0f, 50.0f, 69.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 0.2f, 0.2f);
	glMultiTexCoord2f(GL_TEXTURE1, 1.0f, 1.0f);
	glVertex3f(0.0f, 70.0f, 69.5f);

	glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.2f);
	glMultiTexCoord2f(GL_TEXTURE1, 0.0f, 1.0f);
	glVertex3f(-20.0f, 70.0f, 69.5f);
	glEnd();
	glPopMatrix();

	// Desabilita multi-textura
	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_TEXTURE_2D);

	glDisable(GL_FOG);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 0.01f;

	// Impressão de texto na tela...
	// Muda para modo de projeção ortogonal 2D
	// OBS: Desabilite Texturas e Iluminação antes de entrar nesse modo de projeção
	OrthoMode(0, 0, WIDTH, HEIGHT);


	glPushMatrix();
	glTranslatef(0.0f, HEIGHT - 200, 0.0f);	// Move 1 unidade para dentro da tela (eixo Z)

	// Cor da fonte
	glColor3f(1.0f, 1.0f, 0.0f);


	glRasterPos2f(10.0f, 0.0f);	// Posicionando o texto na tela
	if (!bIsWireframe) {
		pTexto->glPrint("[TAB]  Modo LINE"); // Imprime texto na tela
	}
	else {
		pTexto->glPrint("[TAB]  Modo FILL");
	}


	//// Camera LookAt
	glRasterPos2f(10.0f, 40.0f);
	pTexto->glPrint("Player LookAt  : %f, %f, %f", pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);

	//// Posição do Player
	glRasterPos2f(10.0f, 60.0f);
	pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	//// Imprime o FPS da aplicação e o Timer
	glRasterPos2f(10.0f, 80.0f);
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime()/1000));

	
	glPopMatrix();

	// Muda para modo de projeção perspectiva 3D
	PerspectiveMode();

	return true;
}




void CScene1::MouseMove(void) // Tratamento de movimento do mouse
{
	// Realiza os cálculos de rotação da visão do Player (através das coordenadas
	// X do mouse.
	POINT mousePos;
	int middleX = WIDTH >> 1;
	int middleY = HEIGHT >> 1;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	fDeltaX = (float)((middleX - mousePos.x)) / 10;
	fDeltaY = (float)((middleY - mousePos.y)) / 10;

	// Rotaciona apenas a câmera
	pCamera->rotateGlob(-fDeltaX, 0.0f, 1.0f, 0.0f);
	pCamera->rotateLoc(-fDeltaY, 1.0f, 0.0f, 0.0f);
}

void CScene1::KeyPressed(void) // Tratamento de teclas pressionadas
{

	// Verifica se a tecla 'W' foi pressionada e move o Player para frente
	if (GetKeyState('W') & 0x80)
	{
		pCamera->moveGlob(pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);
	}
	// Verifica se a tecla 'S' foi pressionada e move o Player para tras
	else if (GetKeyState('S') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Forward[0], -pCamera->Forward[1], -pCamera->Forward[2]);
	}
	// Verifica se a tecla 'A' foi pressionada e move o Player para esquerda
	else if (GetKeyState('A') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Right[0], -pCamera->Right[1], -pCamera->Right[2]);
	}
	// Verifica se a tecla 'D' foi pressionada e move o Player para direira
	else if (GetKeyState('D') & 0x80)
	{
		pCamera->moveGlob(pCamera->Right[0], pCamera->Right[1], pCamera->Right[2]);
	}
	// Senão, interrompe movimento do Player
	else
	{
	}


	if (GetKeyState(VK_UP) & 0x80)
	{
		fPosZ -= fMovementFactor;
	}
	if (GetKeyState(VK_DOWN) & 0x80)
	{
		fPosZ += fMovementFactor;
	}
	if (GetKeyState(VK_LEFT) & 0x80)
	{
		fPosX -= fMovementFactor;
	}
	if (GetKeyState(VK_RIGHT) & 0x80)
	{
		fPosX += fMovementFactor;
	}
	if (GetKeyState(VK_PRIOR) & 0x80)
	{
		fPosY += fMovementFactor;
	}
	if (GetKeyState(VK_NEXT) & 0x80)
	{
		fPosY -= fMovementFactor;
	}

	
	
}

void CScene1::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
{
	switch (wParam)
	{
	case VK_TAB:
		bIsWireframe = !bIsWireframe;
		break;

	case VK_SPACE:
	{
		pTimer->Init();
	}
		break;

	case VK_RETURN:

		break;


	}

}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene1::Draw3DSGrid(float width, float length)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.3f, 0.0f);
	glPushMatrix();
	for (float i = -width; i <= length; i += 1)
	{
		for (float j = -width; j <= length; j += 1)
		{
			// inicia o desenho das linhas
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i, 0.0f, j + 1);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 1, 0.0f, j + 1);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 1, 0.0f, j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i, 0.0f, j);
			glEnd();
		}
	}
	glPopMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void CScene1::DrawCube(float pX, float pY, float pZ,
	float rX, float rY, float rZ, float angle,
	float sX, float sY, float sZ,
	int texID)
{

	// Seta a textura ativa
	if (texID >= 0)
		pTextures->ApplyTexture(texID);

	glPushMatrix();
	glTranslatef(pX, pY, pZ);
	glRotatef(angle, rX, rY, rZ);
	glScalef(sX, sY, sZ);

	glBegin(GL_QUADS);
	// face frente
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f, 0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, 0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, 0.5f);

	// face trás
	glTexCoord2d(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, -0.5f);
	
	// face direita
	glTexCoord2d(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);

	// face esquerda
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, 0.5f, -0.5f);

	// face baixo
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f( 0.5f, -0.5f,  0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);

	// face cima
	glTexCoord2d(0.0f, 0.0f); glVertex3f(-0.5f,  0.5f,  0.5f);
	glTexCoord2d(0.0f, 1.0f); glVertex3f( 0.5f,  0.5f,  0.5f);
	glTexCoord2d(1.0f, 1.0f); glVertex3f( 0.5f,  0.5f, -0.5f);
	glTexCoord2d(1.0f, 0.0f); glVertex3f(-0.5f,  0.5f,  -0.5f);

	glEnd();

	glPopMatrix();
}


void CScene1::DrawAxis()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	// Eixo X
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);

	// Eixo Y
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);

	// Eixo Z
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glEnd();
	glPopMatrix();
}


void CScene1::CreateSkyBox(float x, float y, float z,
	float width, float height, float length,
	CTexture* pTextures)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPushMatrix();

	// Centraliza o Skybox em torno da posição especificada(x, y, z)
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;


	// Aplica a textura que representa a parte da frente do skybox (BACK map)
	pTextures->ApplyTexture(0);

	// Desenha face BACK do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (FRONT map)
	pTextures->ApplyTexture(1);

	// Desenha face FRONT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (DOWN map)
	pTextures->ApplyTexture(2);

	// Desenha face BOTTOM do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (UP map)
	pTextures->ApplyTexture(3);

	// Desenha face TOP do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (LEFT map)
	pTextures->ApplyTexture(4);

	// Desenha face LEFT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (RIGHT map)
	pTextures->ApplyTexture(5);

	// Desenha face RIGHT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	glPopMatrix();
}



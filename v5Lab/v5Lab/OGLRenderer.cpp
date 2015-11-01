#include "OGLRenderer.h"
#include "LogoActor.h"
#include <glm/gtx/constants.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <typeinfo>

COGLRenderer::COGLRenderer()
{
	mCamera.mName = L"Main camera";
}

COGLRenderer::~COGLRenderer(void)
{
	KillFont();
}

/*
* Resize GL scene
* Resize and initialize the GL Window and calculate the view frustrum
*/
void COGLRenderer::Resize(const int width, int height)		
{
	mHeight = height == 0 ? 1 : height;
	mWidth = width;
	glViewport(0, 0, mWidth, mHeight);
	double aspectRatio = (double)mWidth / (double)mHeight;
	mfH = tan(mCamera.GetFOV() / 360.0 * glm::pi<double>()) * NEARPLANE;
    mfW = mfH * aspectRatio;
}

/*
* Init GL scene
* All Setup For OpenGL Goes Here
*/
bool COGLRenderer::Init(const HDC hdc)
{
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.01f, 0.02f, 0.01f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	// Light0 setup
	const static GLfloat LightAmbient[]= { 0.0f, 0.0f, 0.0f, 1.0f};
	const static GLfloat LightDiffuse[]= { 0.5f, 0.5f, 0.5f, 1.0f}; 
	const static GLfloat LightPosition[] = {-10.f, 10.f, 10.f, 1.f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);   
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
	// Fonts
	BuildFont(hdc);
	return true;
}

// DrawGrid
void COGLRenderer::DrawGrid(const GLfloat fGridsize, const GLfloat fStep)
{
	const GLfloat gridsize = fGridsize * fStep ;
	glBegin(GL_LINES);
		for(GLfloat i = -gridsize ; i <= gridsize ; i += fStep )
		{
			glVertex3f( -gridsize, -0.01f, i );	
			glVertex3f( gridsize, -0.01f, i );
			glVertex3f( i, -0.01f, -gridsize );	
			glVertex3f( i, -0.01f, gridsize );
		}
	glEnd() ;
}

/*
* 
*/
bool COGLRenderer::Render(vector<unique_ptr<CEntity>>& entities)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glFrustum(-mfW, mfW, -mfH, mfH, NEARPLANE, FARPLANE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(mCamera.mRotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(mCamera.mRotation.y, 0.0f, 1.0f, 0.0f);
	// Invert world transform to emulate camera transform
	glTranslatef(-mCamera.mPosition.x, -mCamera.mPosition.y, -mCamera.mPosition.z);
	/* Render grid */
    glDisable(GL_LIGHTING);
	glColor3ub(0, 20, 0);
	glLineWidth(1.f) ;
	DrawGrid(100.f, 1.f);
	/* Render entities */
	for(const auto& p : entities)
	{
		glPushMatrix();
		glTranslatef(p->mPosition.x, p->mPosition.y, p->mPosition.z);
		glMultMatrixf(glm::value_ptr(glm::mat4_cast(p->mQuat)));
		if (typeid(*p) == typeid(CLogoActor))
		{
//		    glDisable(GL_LIGHTING);
			glColor3ub(200, 0, 0);
			glBegin(GL_TRIANGLES);
				glVertex3f(-0.5f, 0.01f, 0);
				glVertex3f(0, 0.01f, 1);
				glVertex3f(0.5f, 0.01f, 0);
			glEnd();
		}
		else
		{
/*
			glEnable(GL_LIGHTING);
			GLfloat MaterialAmbient[] = {0.0f, 0.0f, 0.0f};
			GLfloat MaterialDiffuse[] = {0.0f, 0.0f, 1.0f};
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
			glutSolidCube(1.0);
*/
		}
		glPopMatrix();
	}
	/* Render trace */
    glDisable(GL_LIGHTING);
	glColor3ub(200, 200, 200);
	for(const auto& p : entities)
	{
		if (typeid(*p) == typeid(CLogoActor))
		{
			CLogoActor* pActor = static_cast<CLogoActor*>(p.get());
			for (const auto& q : pActor->mTrace)
			{
				CLine* pLine = static_cast<CLine*>(q.get());
				glBegin(GL_TRIANGLES);
					glVertex3f(pLine->mP0.x, pLine->mP0.y, pLine->mP0.z);
					glVertex3f(pLine->mP1.x, pLine->mP1.y, pLine->mP1.z);
					glVertex3f(pLine->mP3.x, pLine->mP3.y, pLine->mP3.z);
					glVertex3f(pLine->mP1.x, pLine->mP1.y, pLine->mP1.z);
					glVertex3f(pLine->mP2.x, pLine->mP2.y, pLine->mP2.z);
					glVertex3f(pLine->mP3.x, pLine->mP3.y, pLine->mP3.z);
				glEnd();
			}
		}
	}
	return true;
}


void COGLRenderer::RenderText(vector<wstring>& text)
{
	const wstring cursor(L"? ");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, mWidth, 0, mHeight, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();									// Reset The Current Modelview Matrix
	glDisable(GL_LIGHTING);
	glColor3f(0.9f, 0.0f, 0.0f);
	glRasterPos2i(8, 8);
	glPrint(mCommandStatus.c_str());
	glColor3f(0.f, 0.9f, 0.0f);
	int y = 32;
	int x = 32;
	auto i = text.rbegin();
	glRasterPos2i(x, y);
	wstring first = (*i).c_str();
	first.insert(0, cursor);
	glPrint(first.c_str());
	y += 24;
	for (i = text.rbegin() + 1; i != text.rend(); ++i ) 
	{
		glRasterPos2i(x, y);
		glPrint((*i).c_str());
		y += 24;
	}
}

void COGLRenderer::BuildFont(const HDC hdc)
{
	HFONT font = CreateFont(-24,						// Height Of Font
						0,								// Width Of Font
						0,								// Angle Of Escapement
						0,								// Orientation Angle
						FW_BOLD,						// Font Weight
						FALSE,							// Italic
						FALSE,							// Underline
						FALSE,							// Strikeout
						ANSI_CHARSET,					// Character Set Identifier
						OUT_TT_PRECIS,					// Output Precision
						CLIP_DEFAULT_PRECIS,			// Clipping Precision
						ANTIALIASED_QUALITY,			// Output Quality
						FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
						L"Courier New");					// Font Name

	const HFONT oldfont = (HFONT)SelectObject(hdc, font);
	mBase = glGenLists(96);
	wglUseFontBitmaps(hdc, 32, 96, mBase);
	SelectObject(hdc, oldfont);
	DeleteObject(font);
}

void COGLRenderer::KillFont() const								
{
	glDeleteLists(mBase, 96);
}

/* TODO: glPrint(const wchar_t* fmt, ...) */
void COGLRenderer::glPrint(const wstring wstr)
{
	string str(wstr.begin(), wstr.end());
	glPrint(str.c_str());
}

void COGLRenderer::glPrint(const char* fmt, ...)
{
	char text[256];
	va_list ap;
	va_start(ap, fmt);
	vsprintf_s(text, fmt, ap);
	va_end(ap);
	glPushAttrib(GL_LIST_BIT);
	glListBase(mBase - 32);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}
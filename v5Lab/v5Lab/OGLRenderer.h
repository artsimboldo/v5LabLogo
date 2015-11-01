#pragma once
#include <windows.h>
#include "Camera.h"
#include <vector>
#include <memory>
#pragma comment(lib, "opengl32.lib")  
#include <gl\gl.h>			

class COGLRenderer
{
public:
	COGLRenderer();
	~COGLRenderer(void);
	bool Init(const HDC);
	void Resize(const int width, int height);
	bool Render(vector<unique_ptr<CEntity>>&);
	void RenderText(vector<wstring>&);
	CCamera& GetCamera() { return mCamera; };
	void SetCommandStatus(wstring text) { mCommandStatus = text; }

private:
	static void DrawGrid(const GLfloat, const GLfloat);
	void BuildFont(const HDC);
	void KillFont() const;
	void inline glPrint(const wstring);
	void inline glPrint(const char* fmt, ...);
	void inline Quad(const glm::vec3 v1, const glm::vec3 v2, const float w1, const float w2) const
	{
		glVertex3f(v1.x - w1, v1.y, v1.z);
		glVertex3f(v1.x + w1, v1.y, v1.z);
		glVertex3f(v2.x - w2, v2.y, v2.z);
		glVertex3f(v2.x - w2, v2.y, v2.z);
		glVertex3f(v2.x + w2, v2.y, v2.z);
		glVertex3f(v1.x + w1, v1.y, v1.z);	
	}

protected:
	CCamera	mCamera;
	int		mWidth, mHeight;
	double	mfH, mfW;

private:
	int		mBase;
	wstring mCommandStatus;
};


// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "PgmImage2.h"

#include "DialogImage.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_COMMAND(ID_IMAGE_OUVRIR_AFFICHER, OnImageOuvrirAfficher)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_IMAGE_OUVRIRAUTOMATIQUEMENT, &CMainFrame::OnImageOuvrirautomatiquement)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here

}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnImageOuvrirAfficher()
{
	CString pPathName;
	CString strFileExtension;
	ImageOuverte = false;
	static char BASED_CODE szFilter[] = "Fichiers .pgm |*.pgm||";
	CFileDialog dlg(TRUE, "pgm", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (dlg.DoModal() == IDOK) {
		strFileExtension = dlg.GetFileExt();
		strFileExtension.MakeUpper();
		if (strFileExtension == "PGM") {
			pPathName = dlg.GetPathName();

			if (Image1.lecture(pPathName))
			{
				ImageOuverte = true;
				CDialogImage DialogImage1(&Image1);
				DialogImage1.DoModal();
			}
		}
		else AfxMessageBox("Le fichier n'est pas de type PGM: l'image n'a pas été ouverte");
	}
	else AfxMessageBox("aucune image n'a été ouverte");

}


void CMainFrame::OnImageOuvrirautomatiquement()
{
	// TODO: Add your command handler code here
	CString pPathName;

	pPathName = "D:\\Dropbox\\UQTR\\PRO1027\\Ftp\\Exemples\\prof.pgm";

	if (Image1.lecture(pPathName))
	{
		ImageOuverte = true;
		CDialogImage DialogImage1(&Image1);
		DialogImage1.DoModal();
	}
}
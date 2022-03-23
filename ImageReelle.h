#if !defined(AFX_IMAGEREELLE_H__0DFAC956_0738_11D5_AA78_0050FC20AEF7__INCLUDED_)
#define AFX_IMAGEREELLE_H__0DFAC956_0738_11D5_AA78_0050FC20AEF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageReelle.h : header file
//

#include "ImageReelle.h"


// definition de la classe image reelle
class CImageReelle{
 
public:
    CImageReelle(int width, int length, double maxGris, double minGris);   // constructeur avec initialisation
	CImageReelle();   // constructeur simple
    ~CImageReelle();  //destructeur
	void initialiser(int width, int length); 
	bool lecture(CString pNomFcihier);
	void ecriture();

public :
	FILE *fichier;  //fichier lu
	int NbLignes,NbColonnes;
	double MaxNivGris;
	double **matrice; 
};



/******************************************/
//class CImageReelle : public CDialog
//{
// Construction
//public:
//	CImageReelle(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImageReelle)
//	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageReelle)
//	protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
//protected:

	// Generated message map functions
	//{{AFX_MSG(CImageReelle)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	//DECLARE_MESSAGE_MAP()
//};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEREELLE_H__0DFAC956_0738_11D5_AA78_0050FC20AEF7__INCLUDED_)

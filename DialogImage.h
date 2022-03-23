#if !defined(AFX_DIALOGIMAGE_H__7D8FC2BD_E66A_4306_A18B_924A83B05B12__INCLUDED_)
#define AFX_DIALOGIMAGE_H__7D8FC2BD_E66A_4306_A18B_924A83B05B12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogImage.h : header file
//
#include "ImageReelle.h"
#include "Util.h"
#include "math.h"
#include <string>

#define  H_OFFSET_I 20
#define  V_OFFSET_I 20

#define  HIST_X_OFFSET 700
#define HIST_Y_OFFSET 680
#define  HIST_HEIGTH 100
#define HIST_WIDTH 256
#define  HIST_SIZE_RATIO 2

#define _USE_MATH_DEFINES

/////////////////////////////////////////////////////////////////////////////
// CDialogImage dialog

class CDialogImage : public CDialog
{
	// Construction
public:
	CDialogImage(CImageReelle* pImage, CWnd* pParent = NULL);   // standard constructor
	CDialogImage(int, int, CImageReelle* pImage, CWnd* pParent = NULL);   // constructeur avec redimensionnement

	// Dialog Data
	//{{AFX_DATA(CDialogImage)
	enum { IDD = IDD_DIALOG_IMAGE };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogImage)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogImage)
	afx_msg void OnPaint();
	afx_msg void AffichePoint(int x, int y, string etiquette);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CImageReelle* ptImage;
	int width, length;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	int initialX;
	int initialY;
	int finalX;
	int finalY;
	bool valide;

	int filtre;
	int maximum1;
	int maximum2;
	int seuil;
	int threshold;

	float* histogramme;
	float* histogrammeLisse;
	float* derive;
	float* derive2;

	void Derive(int df);
	void AfficheCourbe(float* fonct);
	void ChercherSeuil();
	int Filtre(int filtreTest);
	void MethodeOtsu();
	void Filtrer(int seuil);
	void ClearROI();
	void InitGraphique();
	void VectHistogram();
	void Lissage(int df);

	afx_msg void OnBnClickedButtonCalculSeuil();
	afx_msg void OnBnClickedButtonAppliquerFiltre();
	afx_msg void OnBnClickedButtonDerive();
	afx_msg void OnBnClickedButtonMethodeOtsu();
	afx_msg void OnBnClickedButtonAppliquerOtsu();
	afx_msg void OnBnClickedButtonAuto();
	afx_msg void AfficheHistogramme(float* vect, COLORREF color);
	afx_msg void OnBnClickedHistogramme();
	afx_msg void OnBnClickeButtonReset();
	afx_msg void OnBnClickedButtonLissage();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGIMAGE_H__7D8FC2BD_E66A_4306_A18B_924A83B05B12__INCLUDED_)
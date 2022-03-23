// DialogImage.cpp : implementation file
//

#include "stdafx.h"
#include "PgmImage2.h"
#include "DialogImage.h"
#include <iostream>
#include <string>
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogImage dialog


CDialogImage::CDialogImage(CImageReelle* pImage, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogImage::IDD, pParent)

        , filtre(1)
		, maximum1(0)
		, maximum2(0)
		, seuil(0)
		, threshold(0)
	{
		//{{AFX_DATA_INIT(CDialogImage)
		// NOTE: the ClassWizard will add member initialization here
		//}}AFX_DATA_INIT
		ptImage = pImage;
		width = pImage->NbColonnes;
		length = pImage->NbLignes;

		initialX = H_OFFSET_I;
		initialY = V_OFFSET_I;
		finalX = width + H_OFFSET_I;
		finalY = length + V_OFFSET_I;
		filtre = 3;
		seuil = 0;
		threshold = 0;

		valide = TRUE;
		histogramme = Util::vector(1, 256);
		histogrammeLisse = Util::vector(1, 256);
		derive = Util::vector(1, 256);
		derive2 = Util::vector(1, 256);
	
}
CDialogImage::CDialogImage(int largeur, int longueur, CImageReelle* pImage, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogImage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogImage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
		ptImage = pImage;
		width=largeur;
		length=longueur;
}


void CDialogImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogImage)
	// NOTE: the ClassWizard will add DDX and DDV calls here

	DDX_Text(pDX, IDC_EDIT1, initialX);
	DDX_Text(pDX, IDC_EDIT2, initialY);
	DDX_Text(pDX, IDC_EDIT3, finalX);
	DDX_Text(pDX, IDC_EDIT4, finalY);

	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT5, filtre);
	DDX_Text(pDX, IDC_EDIT7, maximum1);
	DDX_Text(pDX, IDC_EDIT8, maximum2);
	DDX_Text(pDX, IDC_EDIT9, seuil);
	DDX_Text(pDX, IDC_EDIT6, threshold);
}


BEGIN_MESSAGE_MAP(CDialogImage, CDialog)
	//{{AFX_MSG_MAP(CDialogImage)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON2, &CDialogImage::OnBnClickedHistogramme)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialogImage::OnBnClickeButtonReset)
	ON_BN_CLICKED(IDC_BUTTON4, &CDialogImage::OnBnClickedButtonLissage)
	ON_BN_CLICKED(IDCANCEL, &CDialogImage::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDialogImage::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON6, &CDialogImage::OnBnClickedButtonCalculSeuil)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialogImage::OnBnClickedButtonAppliquerFiltre)
	ON_BN_CLICKED(IDC_BUTTON7, &CDialogImage::OnBnClickedButtonDerive)
	ON_BN_CLICKED(IDC_BUTTON5, &CDialogImage::OnBnClickedButtonMethodeOtsu)
	ON_BN_CLICKED(IDC_BUTTON8, &CDialogImage::OnBnClickedButtonAppliquerOtsu)
	ON_BN_CLICKED(IDC_BUTTON9, &CDialogImage::OnBnClickedButtonAuto)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogImage message handlers

void CDialogImage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	int HorizontalShift=0;  //decalages entre les bords de la boite de dialogue et l'image
	int VerticalShift=0;
	int SX=8, SY=6; // facteur de scaling pour reduire la dimension de l'image originale
	int i, j; // compteurs
	float valeur; //valeur du niveau de gris en un point
	CClientDC ClientDC(this);

   
	for (i=0;i<length;i+=SY){
	  for (j = 0; j<width; j+=SX) {
	    if (ptImage->matrice[i][j]>=0)
			valeur = ptImage->matrice[i][j];
		else 
			valeur = 256+ptImage->matrice[i][j];
	    ClientDC.SetPixelV((j+HorizontalShift)/SX, (i+VerticalShift)/SY, RGB(valeur, valeur,valeur));
		  
	  }
    }
	InitGraphique();
}

void CDialogImage::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC ClientDC(this);

	
	CPen pen;
	CPen penLine;
	CBrush brush;

	penLine.CreatePen(PS_SOLID, 2, RGB(108, 175, 48));
	

	pen.CreatePen(PS_SOLID, 2, RGB(255,0,0));
	ClientDC.SelectObject(&pen);

	brush.CreateSolidBrush(RGB(0,255,0));
	ClientDC.SelectObject(&brush);
	if (!(point.x > (width + H_OFFSET_I) || point.y > (length + V_OFFSET_I) ||
		point.x < H_OFFSET_I || point.y < V_OFFSET_I))
	{
		try
		{
			if (initialX == H_OFFSET_I) {

				//1er point ROI
				UpdateData(TRUE);

				ClientDC.Ellipse(point.x - 5, point.y - 5, point.x + 5, point.y + 5);
				initialX = point.x;
				initialY = point.y;

				valide = FALSE;
				UpdateData(FALSE);
			}
			else if (finalX == (width + H_OFFSET_I)) {

				//2e point ROI
				UpdateData(TRUE);

				ClientDC.Ellipse(point.x - 5, point.y - 5, point.x + 5, point.y + 5);
				finalX = point.x;
				finalY = point.y;
				valide = TRUE;


				CPoint t1, t2;
				if (initialX < finalX)
				{
					t1.x = initialX; t2.x = finalX;
				}
				else
				{
					t1.x = finalX; t2.x = initialX;
				}
				if (initialY < finalY)
				{
					t1.y = initialY; t2.y = finalY;
				}
				else
				{
					t1.y = finalY; t2.y = initialY;
				}

				initialX = t1.x; initialY = t1.y;
				finalX = t2.x; finalY = t2.y;

				ClientDC.SelectObject(&penLine);

				ClientDC.MoveTo(initialX - 2, initialY - 2);
				ClientDC.LineTo(finalX + 2, initialY - 2);
				ClientDC.LineTo(finalX + 2, finalY + 2);
				ClientDC.LineTo(initialX - 2, finalY + 2);
				ClientDC.LineTo(initialX - 2, initialY - 2);

				UpdateData(FALSE);
			}
			else {
				throw MesException(" vous ne pouvez que selectionner 2 points.");
			}

		}
		catch (MesException e) {

			MessageBox(TEXT((std::string(e.message)).c_str()), TEXT("Error !"), NULL);
		}
	}

	CDialog::OnLButtonDown(nFlags, point);

}

void CDialogImage::OnBnClickedCancel()
{
	Util::free_vector(histogramme, 1, 256);
	Util::free_vector(histogrammeLisse, 1, 256);
	Util::free_vector(derive, 1, 256);
	Util::free_vector(derive2, 1, 256);
	CDialog::OnCancel();
}

void CDialogImage::OnBnClickedOk()
{
	Util::free_vector(histogramme, 1, 256);
	Util::free_vector(histogrammeLisse, 1, 256);
	Util::free_vector(derive, 1, 256);
	Util::free_vector(derive2, 1, 256);
	CDialog::OnOK();
}

void CDialogImage::OnBnClickeButtonReset()
{
	//ClearROI();
	for (int i = 1; i <= 256; i++)
		histogramme[i] = 0;
	for (int i = 1; i <= 256; i++)
		derive[i] = 0;

	UpdateData(TRUE);
	filtre = 9;
	seuil = 0;
	threshold = 0;
	UpdateData(FALSE);

}

void CDialogImage::OnRButtonDown(UINT nFlags, CPoint point)
{
	OnBnClickeButtonReset();
	CDialog::OnRButtonDown(nFlags, point);
}
void CDialogImage::AffichePoint(int x, int y, string etiquette) {

	CClientDC ClientDC(this);
	CBrush brush;
	brush.CreateSolidBrush(RGB(10, 255, 10));
	ClientDC.SelectObject(&brush);

	ClientDC.Ellipse(x - 5, y - 5, x + 5, y + 5);
	CString cstr(etiquette.c_str());
	ClientDC.TextOutA(x + 5, y, cstr);
}
void CDialogImage::VectHistogram()
{
	int valeurPixel;

	for (int i = 1; i <= 256; i++)
		histogramme[i] = 0;

	// Ajuster selon offset de l'image
	int departX = initialX - H_OFFSET_I + 1;
	int departY = initialY - V_OFFSET_I + 1;
	int finX = finalX - H_OFFSET_I;
	int finY = finalY - V_OFFSET_I;


	for (int j = departX; j < finX; j++)
		for (int i = departY; i < finY; i++) {
			valeurPixel = ptImage->matrice[i][j];  // comptage des pixels de chaque couleur
			histogramme[valeurPixel + 1]++; // 1 à 256
		}
}

void CDialogImage::InitGraphique()
{

	for (int i = 1; i <= 256; i++)
		derive2[i] = 256;

	AfficheHistogramme(derive2, RGB(255, 0, 0));
	AffichePoint(HIST_X_OFFSET + (HIST_WIDTH * HIST_SIZE_RATIO / 2) - 30, (HIST_Y_OFFSET + 20), "HISTOGRAMME");

}

void CDialogImage::AfficheHistogramme(float* fonct, COLORREF color)
{

	float test[257];
	for (int i = 1; i <= 256; i++)
		test[i] = fonct[i];

	CClientDC ClientDC(this);
	CPen penLine;

	//penLine.CreatePen(PS_SOLID, 1, color);
	penLine.CreatePen(PS_SOLID, HIST_SIZE_RATIO, color);
	ClientDC.SelectObject(&penLine);


	float maxVector = 0.1;
	for (int i = 1; i <= 256; i++) {
		if (fonct[i] > maxVector)
			maxVector = fonct[i];
	}

	for (int i = 1; i <= 256; i++) {
		ClientDC.MoveTo(HIST_X_OFFSET - 1 + i * HIST_SIZE_RATIO, HIST_Y_OFFSET);
		ClientDC.LineTo(HIST_X_OFFSET - 1 + i * HIST_SIZE_RATIO, (HIST_Y_OFFSET - ((fonct[i] * (HIST_HEIGTH) / maxVector) * HIST_SIZE_RATIO)));
	}
}

void CDialogImage::AfficheCourbe(float* fonct)
{
	float test2[257];
	for (int i = 1; i <= 256; i++)
		test2[i] = fonct[i];

	CClientDC ClientDC(this);
	CPen penLine;
	CPen penLine2;

	//penLine2.CreatePen(PS_SOLID, 2, RGB(255, 255, 2550));
	penLine2.CreatePen(PS_SOLID, HIST_SIZE_RATIO, RGB(200, 150, 220));
	ClientDC.SelectObject(&penLine2);

	ClientDC.MoveTo(HIST_X_OFFSET, HIST_Y_OFFSET - (HIST_HEIGTH * HIST_SIZE_RATIO / 2));
	ClientDC.LineTo(HIST_X_OFFSET + HIST_WIDTH * HIST_SIZE_RATIO, HIST_Y_OFFSET - (HIST_HEIGTH * HIST_SIZE_RATIO / 2));

	penLine.CreatePen(PS_SOLID, HIST_SIZE_RATIO, RGB(0, 0, 255));
	ClientDC.SelectObject(&penLine);


	float maxVector = 0;
	for (int i = 1; i <= 256; i++) {
		if (abs(fonct[i]) > maxVector)
			maxVector = abs(fonct[i]);
	}
	ClientDC.MoveTo(HIST_X_OFFSET, HIST_Y_OFFSET - (HIST_HEIGTH * HIST_SIZE_RATIO / 2));
	for (int i = 1; i <= 256; i++) {
		ClientDC.LineTo(HIST_X_OFFSET - 1 + i * HIST_SIZE_RATIO, (HIST_Y_OFFSET - (HIST_HEIGTH * HIST_SIZE_RATIO / 2) - (fonct[i] * (((HIST_HEIGTH)*HIST_SIZE_RATIO) / 2) / maxVector)));
	}
}


void CDialogImage::ClearROI()
{
	MessageBox(TEXT((std::string("Appuyer sur ok pour actauliser la page")).c_str()), TEXT("Refresh la page"), NULL);

	OnPaint();

	UpdateData(TRUE);

	initialX = H_OFFSET_I;
	initialY = V_OFFSET_I;
	finalX = width + H_OFFSET_I;
	finalY = length + V_OFFSET_I;

	valide = TRUE;

	UpdateData(FALSE);

}


void CDialogImage::Lissage(int df)
{

	for (int i = 1; i <= 256; i++)
		histogrammeLisse[i] = 0;

	float somme = 0;
	float ratio = 0.5;
	int nbDebordement = 0;

	for (int x = 1; x <= 256; x++) {
		somme = 0; nbDebordement = 0;

		for (int m = ((-df) / 2); m < (df / 2); m++) {
			if (x + m < 1) {
				somme += (histogramme[x + m + 256]) * ratio;
				nbDebordement++;
			}
			else if (x + m > 256) {
				somme += (histogramme[x + m - 256]) * ratio;
				nbDebordement++;
			}
			else
				somme += histogramme[x + m];
		}

		histogrammeLisse[x] = somme / (df - nbDebordement * ratio);
	}


	int test[257];
	for (int i = 1; i <= 256; i++)
		test[i] = derive[i];
}

void CDialogImage::Derive(int df)
{

	for (int i = 1; i <= 256; i++)
		derive[i] = 0;

	float somme = 0;
	float sigma = (df - 1) / 8.0;
	float opGaussien;
	float ratio = 0.5;
	int nbDebordement = 0;


	for (int x = 1; x <= 256; x++) {
		somme = 0; nbDebordement = 0;

		for (int m = ((-df) / 2); m < (df / 2); m++) {
			opGaussien = ((sqrt(2) * m)) / (2 * ((sqrt(M_PI * sigma))) * (sigma * sigma));
			opGaussien *= exp(-(m * m) / (2 * sigma * sigma));
			if (x + m < 1) {
				somme += (histogramme[x + m + 256]) * ratio * opGaussien;
				nbDebordement++;
			}
			else if (x + m > 256) {
				somme += (histogramme[x + m - 256]) * ratio * opGaussien;
				nbDebordement++;
			}
			else
				somme += histogramme[x + m] * opGaussien;
		}

		derive[x] = somme / (df - nbDebordement * ratio);
	}


	int test[257];
	for (int i = 1; i <= 256; i++)
		test[i] = derive[i];
	int stop;
}

void CDialogImage::ChercherSeuil()
{

	UpdateData(TRUE);
	UpdateData(FALSE);
	VectHistogram();
	Derive(filtre);


	int zero1 = 0, zero2 = 0, min = 0, posMin = 0;
	try
	{
		zero1 = Util::rtbis(derive, 1, 128, 1);
		zero2 = Util::rtbis(derive, 129, 256, 1);

		AffichePoint(HIST_X_OFFSET + zero1 * HIST_SIZE_RATIO, HIST_Y_OFFSET - (HIST_HEIGTH * HIST_SIZE_RATIO / 2), "");
		AffichePoint(HIST_X_OFFSET + zero2 * HIST_SIZE_RATIO, HIST_Y_OFFSET - (HIST_HEIGTH * HIST_SIZE_RATIO / 2), "");


		min = histogrammeLisse[zero1];
		for (int i = zero1; i <= zero2; i++) {
			if (histogrammeLisse[i] < min)
				min = histogrammeLisse[i]; posMin = i;
		}

		UpdateData(TRUE);
		maximum1 = zero1;
		maximum2 = zero2;
		seuil = posMin;
		UpdateData(FALSE);

	}
	catch (MesException e)
	{

		int filtreRecommande = Filtre(filtre);
		string message = " Insuffisant. Sélectionnez une nouvelle zone. ";

		if (filtreRecommande == -1)
			MessageBox(TEXT(message.c_str()), TEXT("Error !"), NULL);
		else {
			message = " Filtre est : ";
			message += std::to_string(filtreRecommande);
			MessageBox(TEXT(message.c_str()), TEXT("Error !"), NULL);
		}
	}
}

int CDialogImage::Filtre(int filtreTest)
{
	Derive(filtreTest);


	int zero1 = 0, zero2 = 0, min = 0, posMin = 0;
	try
	{
		zero1 = Util::rtbis(derive, 1, 128, 1);
		zero2 = Util::rtbis(derive, 129, 256, 1);
		string message = " Filtre est : " + std::to_string(filtreTest);

	}
	catch (MesException e)
	{
		if (filtreTest < 99) {

			filtreTest += 2;
			filtreTest = Filtre(filtreTest);
		}
		else {
			filtreTest = -1;
		}
	}
	return filtreTest;
}


void CDialogImage::MethodeOtsu()
{
	int total = (finalX - initialX) * (finalY - initialY);

	float sum = 0;
	for (int t = 1; t <= 256; t++)
		sum += t * histogramme[t];

	float sumB = 0;
	int wb = 0;
	int wf = 0;

	float varMax = 0;
	int threshold = 0;

	for (int t = 1; t <= 256; t++) {
		wb += histogramme[t];
		if (wb == 0) continue;

		wf = total - wb;
		if (wf == 0) break;

		sumB += (float)(t * histogramme[t]);

		float mB = sumB / wb;
		float mF = (sum - sumB) / wf;

		float varBetween = (float)wb * (float)wf * (mB - mF) * (mB - mF);

		if (varBetween > varMax) {
			varMax = varBetween;
			threshold = t;
		}
	}
	UpdateData(TRUE);
	threshold = threshold;
	UpdateData(FALSE);
}


void CDialogImage::Filtrer(int seuil)
{
	CPaintDC dc(this); // device context for painting
	int HorizontalShift = H_OFFSET_I;  //decalages entre les bords de la boite de dialogue et l'image
	int VerticalShift = V_OFFSET_I;
	int i, j; // compteurs
	float valeur; //valeur du niveau de gris en un point
	float valeurFiltre;
	CClientDC ClientDC(this);
	UpdateData(TRUE); UpdateData(FALSE);

	for (j = 0; j < width; j++) {
		for (i = 0; i < length; i++) {
			if (ptImage->matrice[i][j] >= 0)
				valeur = ptImage->matrice[i][j];
			else
				valeur = 256 + ptImage->matrice[i][j];

			if (valeur > seuil)
				valeurFiltre = 235;
			else
				valeurFiltre = 20;
			ClientDC.SetPixelV(j + HorizontalShift, i + VerticalShift, RGB(valeurFiltre, valeurFiltre, valeurFiltre));
		}
	}
}
void CDialogImage::OnBnClickedButtonLissage()
{
	if (valide) {

		InitGraphique();
		VectHistogram();

		UpdateData(TRUE);
		UpdateData(FALSE);
		if (filtre % 2 == 1) {
			Lissage(filtre);
			AfficheHistogramme(histogramme, RGB(0, 0, 0));
			AfficheHistogramme(histogrammeLisse, RGB(0, 255, 0));
		}
		else
			MessageBox(TEXT("La valeur du filtre doit être un nombre impair enre 1 et 21"), TEXT("Entree erronee"), NULL);

	}
	else
		MessageBox(TEXT("Invalidité"), TEXT("Erreur !"), NULL);

}

void CDialogImage::OnBnClickedHistogramme()
{
	if (valide) {

		InitGraphique();
		VectHistogram();
		AfficheHistogramme(histogramme, RGB(0, 0, 0));
	}
	else
		MessageBox(TEXT("Invalidité"), TEXT("Erreur !"), NULL);
}

void CDialogImage::OnBnClickedButtonDerive()
{
	if (valide) {

		InitGraphique();
		VectHistogram();
		AfficheHistogramme(histogramme, RGB(0, 0, 0));

		UpdateData(TRUE);
		UpdateData(FALSE);
		if (filtre % 2 == 1) {
			Derive(filtre);
			AfficheCourbe(derive);
		}
		else
			MessageBox(TEXT("La valeur du filtre doit être un nombre impair"), TEXT("Erreur !"), NULL);

	}
	else
		MessageBox(TEXT("Invalidité"), TEXT("Erreur !"), NULL);

}

void CDialogImage::OnBnClickedButtonCalculSeuil()
{
	if (valide) {
		OnBnClickedButtonDerive();
		ChercherSeuil();
	}
	else
		MessageBox(TEXT("Invalidité"), TEXT("Erreur !"), NULL);
}

void CDialogImage::OnBnClickedButtonAppliquerFiltre()
{
	if (seuil > 0) {
		Filtrer(seuil);
	}
}

void CDialogImage::OnBnClickedButtonMethodeOtsu()
{
	VectHistogram();
	MethodeOtsu();
}

void CDialogImage::OnBnClickedButtonAppliquerOtsu()
{
	if (threshold != 0)
		Filtrer(threshold);
}

void CDialogImage::OnBnClickedButtonAuto()
{

	if (valide) {

		InitGraphique();
		VectHistogram();
		AfficheHistogramme(histogramme, RGB(0, 0, 0));

		if (filtre % 2 == 0) {
			MessageBox(TEXT("La valeur du filtre doit être un nombre impair. Valeur incrementée de 1"), TEXT("Erreur !"), NULL);
			UpdateData(TRUE);
			filtre++;
			UpdateData(FALSE);
		}
		UpdateData(TRUE);
		filtre = Filtre(filtre);
		Lissage(filtre);
		AfficheHistogramme(histogrammeLisse,RGB(51, 76, 255));
		Derive(filtre);
		AfficheCourbe(derive);

		UpdateData(FALSE);
		ChercherSeuil();
		MethodeOtsu();

		Filtrer(seuil);
	}
	else
		MessageBox(TEXT("Invalidité"), TEXT("Erreur !"), NULL);


}


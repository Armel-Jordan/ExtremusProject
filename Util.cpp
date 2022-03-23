#include <stdio.h>
#include "stdafx.h"
#include "Util.h"
#include <malloc.h>
#include<iostream>

#define ISCR 60
#define JSCR 21
#define BLANK '_'
#define ZERO '-'
#define YY '1'
#define XX '-'
#define FF 'x'



/* allocate a float vector with subscript range v[nl..nh] */
int* Util::vectorInt(int nl, int nh) {
	int* v;

	v = (int*)malloc((unsigned)(nh - nl + 1) * sizeof(int));
	if (!v) cout << ("allocation failure in vector()");
	return v - nl;
}

void Util::free_vectorInt(int* v, long nl, long nh) {
	free((char*)(v + nl));
}

/* allocate a float vector with subscript range v[nl..nh] */
float* Util::vector(int nl, int nh) {
	float* v;

	v = (float*)malloc((unsigned)(nh - nl + 1) * sizeof(float));
	if (!v) cout << ("allocation failure in vector()");
	return v - nl;
}

void Util::free_vector(float* v, long nl, long nh) {
	free((char*)(v + nl));
}

/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */
float** Util::matrix(int nrl, int nrh, int ncl, int nch)
{
	int i;
	float** m;

	m = (float**)malloc((unsigned)(nrh - nrl + 1) * sizeof(float*));
	if (!m) cout << ("allocation failure 1 in matrix()");
	m -= nrl;

	for (i = nrl; i <= nrh; i++) {
		m[i] = (float*)malloc((unsigned)(nch - ncl + 1) * sizeof(float));
		if (!m[i]) cout << ("allocation failure 2 in matrix()");
		m[i] -= ncl;
	}
	return m;
}

void Util::free_matrix(float** m, int nrl, int nrh, int ncl, int nch)
{
	int i;

	for (i = nrh; i >= nrl; i--) free((char*)(m[i] + ncl));
	free((char*)(m + nrl));
}

/*Solves A·X = B for a vector X, where A is speciﬁed by the arrays
u[1..m][1..n], w[1..n], v[1..n][1..n] as returned by svdcmp.
m and n are the dimensions of a, and will be equal forsquare matrices.
b[1..m] is the input right-hand side.
x[1..n] is the output solution vector.*/
void Util::svbksb(float** u, float w[], float** v, int m, int n, float b[], float x[])
{

	int jj, j, i;
	float s, * tmp;

	tmp = Util::vector(1, n);
	for (j = 1; j <= n; j++) {
		s = 0.0;
		if (w[j]) {
			for (i = 1; i <= m; i++) s += u[i][j] * b[i];
			s /= w[j];
		}
		tmp[j] = s;
	}
	for (j = 1; j <= n; j++) {
		s = 0.0;
		for (jj = 1; jj <= n; jj++) s += v[j][jj] * tmp[jj];
		x[j] = s;
	}
	Util::free_vector(tmp, 1, n);
}

/*Given a matrix a[1..m][1..n], this routine computes its singular value decomposition, A = U·W ·V^T .
The matrix U replaces a on output.
The diagonal matrix of singular values W is output as a vector w[1..n].
The matrix V (not the transpose V^T ) is output as v[1..n][1..n]*/
void Util::svdcmp(float** a, int m, int n, float* w, float** v)
{
	int flag, i, its, j, jj, k, l, nm;
	float c, f, h, s, x, y, z;
	float anorm = 0.0, g = 0.0, scale = 0.0;
	float* rv1;

	if (m < n) cout << ("SVDCMP: You must augment A with extra zero rows");
	rv1 = Util::vector(1, n);
	for (i = 1; i <= n; i++) {
		l = i + 1;
		rv1[i] = scale * g;
		g = s = scale = 0.0;
		if (i <= m) {
			for (k = i; k <= m; k++) scale += fabs(a[k][i]);
			if (scale) {
				for (k = i; k <= m; k++) {
					a[k][i] /= scale;
					s += a[k][i] * a[k][i];
				}
				f = a[i][i];
				g = -SIGN(sqrt(s), f);
				h = f * g - s;
				a[i][i] = f - g;
				if (i != n) {
					for (j = l; j <= n; j++) {
						for (s = 0.0, k = i; k <= m; k++) s += a[k][i] * a[k][j];
						f = s / h;
						for (k = i; k <= m; k++) a[k][j] += f * a[k][i];
					}
				}
				for (k = i; k <= m; k++) a[k][i] *= scale;
			}
		}
		w[i] = scale * g;
		g = s = scale = 0.0;
		if (i <= m && i != n) {
			for (k = l; k <= n; k++) scale += fabs(a[i][k]);
			if (scale) {
				for (k = l; k <= n; k++) {
					a[i][k] /= scale;
					s += a[i][k] * a[i][k];
				}
				f = a[i][l];
				g = -SIGN(sqrt(s), f);
				h = f * g - s;
				a[i][l] = f - g;
				for (k = l; k <= n; k++) rv1[k] = a[i][k] / h;
				if (i != m) {
					for (j = l; j <= m; j++) {
						for (s = 0.0, k = l; k <= n; k++) s += a[j][k] * a[i][k];
						for (k = l; k <= n; k++) a[j][k] += s * rv1[k];
					}
				}
				for (k = l; k <= n; k++) a[i][k] *= scale;
			}
		}
		anorm = MAX(anorm, (fabs(w[i]) + fabs(rv1[i])));
	}
	for (i = n; i >= 1; i--) {
		if (i < n) {
			if (g) {
				for (j = l; j <= n; j++)
					v[j][i] = (a[i][j] / a[i][l]) / g;
				for (j = l; j <= n; j++) {
					for (s = 0.0, k = l; k <= n; k++) s += a[i][k] * v[k][j];
					for (k = l; k <= n; k++) v[k][j] += s * v[k][i];
				}
			}
			for (j = l; j <= n; j++) v[i][j] = v[j][i] = 0.0;
		}
		v[i][i] = 1.0;
		g = rv1[i];
		l = i;
	}
	for (i = n; i >= 1; i--) {
		l = i + 1;
		g = w[i];
		if (i < n)
			for (j = l; j <= n; j++) a[i][j] = 0.0;
		if (g) {
			g = 1.0 / g;
			if (i != n) {
				for (j = l; j <= n; j++) {
					for (s = 0.0, k = l; k <= m; k++) s += a[k][i] * a[k][j];
					f = (s / a[i][i]) * g;
					for (k = i; k <= m; k++) a[k][j] += f * a[k][i];
				}
			}
			for (j = i; j <= m; j++) a[j][i] *= g;
		}
		else {
			for (j = i; j <= m; j++) a[j][i] = 0.0;
		}
		++a[i][i];
	}
	for (k = n; k >= 1; k--) {
		for (its = 1; its <= 30; its++) {
			flag = 1;
			for (l = k; l >= 1; l--) {
				nm = l - 1;
				if ((float)(fabs(rv1[l]) + anorm) == anorm) {
					flag = 0;
					break;
				}
				if ((float)(fabs(w[nm]) + anorm) == anorm) break;
			}
			if (flag) {
				c = 0.0;
				s = 1.0;
				for (i = l; i <= k; i++) {
					f = s * rv1[i];
					rv1[i] = c * rv1[i];
					if ((float)(fabs(f) + anorm) == anorm) break;
					g = w[i];
					h = PYTHAG(f, g);
					w[i] = h;
					h = 1.0 / h;
					c = g * h;
					s = (-f * h);
					for (j = 1; j <= m; j++) {
						y = a[j][nm];
						z = a[j][i];
						a[j][nm] = y * c + z * s;
						a[j][i] = z * c - y * s;
					}
				}
			}
			z = w[k];
			if (l == k) {
				if (z < 0.0) {
					w[k] = -z;
					for (j = 1; j <= n; j++) v[j][k] = (-v[j][k]);
				}
				break;
			}
			if (its == 30) cout << ("No convergence in 30 SVDCMP iterations");
			x = w[l];
			nm = k - 1;
			y = w[nm];
			g = rv1[nm];
			h = rv1[k];
			f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
			g = PYTHAG(f, 1.0);
			f = ((x - z) * (x + z) + h * ((y / (f + SIGN(g, f))) - h)) / x;
			c = s = 1.0;
			for (j = l; j <= nm; j++) {
				i = j + 1;
				g = rv1[i];
				y = w[i];
				h = s * g;
				g = c * g;
				z = PYTHAG(f, h);
				rv1[j] = z;
				c = f / z;
				s = h / z;
				f = x * c + g * s;
				g = g * c - x * s;
				h = y * s;
				y = y * c;
				for (jj = 1; jj <= n; jj++) {
					x = v[jj][j];
					z = v[jj][i];
					v[jj][j] = x * c + z * s;
					v[jj][i] = z * c - x * s;
				}
				z = PYTHAG(f, h);
				w[j] = z;
				if (z) {
					z = 1.0 / z;
					c = f * z;
					s = h * z;
				}
				f = (c * g) + (s * y);
				x = (c * y) - (s * g);
				for (jj = 1; jj <= m; jj++) {
					y = a[jj][j];
					z = a[jj][i];
					a[jj][j] = y * c + z * s;
					a[jj][i] = z * c - y * s;
				}
			}
			rv1[l] = 0.0;
			rv1[k] = f;
			w[k] = x;
		}
	}
	Util::free_vector(rv1, 1, n);
}

void Util::scrsho(float(*fx)(float))
{
	int jz, j, i;
	float ysml, ybig, x2, x1, x, dyj, dx, y[ISCR + 1];
	char scr[ISCR + 1][JSCR + 1];

	for (;;) {
		printf("\nEnter x1 x2 (x1=x2 to stop):\n");
		scanf("%f %f", &x1, &x2);
		if (x1 == x2) break;
		for (j = 1; j <= JSCR; j++)
			scr[1][j] = scr[ISCR][j] = YY;
		for (i = 2; i <= (ISCR - 1); i++) {
			scr[i][1] = scr[i][JSCR] = XX;
			for (j = 2; j <= (JSCR - 1); j++)
				scr[i][j] = BLANK;
		}
		dx = (x2 - x1) / (ISCR - 1);
		x = x1;
		ysml = ybig = 0.0;
		for (i = 1; i <= ISCR; i++) {
			y[i] = (*fx)(x);
			if (y[i] < ysml) ysml = y[i];
			if (y[i] > ybig) ybig = y[i];
			x += dx;

		}
		if (ybig == ysml) ybig = ysml + 1.0;
		dyj = (JSCR - 1) / (ybig - ysml);
		jz = 1 - (int)(ysml * dyj);
		for (i = 1; i <= ISCR; i++) {
			scr[i][jz] = ZERO;
			j = 1 + (int)((y[i] - ysml) * dyj);
			scr[i][j] = FF;
		}
		printf(" %10.3f ", ybig);
		for (i = 1; i <= ISCR; i++) printf("%c", scr[i][JSCR]);
		printf("\n");
		for (j = (JSCR - 1); j >= 2; j--) {
			printf("%12s", " ");
			for (i = 1; i <= ISCR; i++) printf("%c", scr[i][j]);
			printf("\n");
		}
		printf(" %10.3f ", ysml);

		for (i = 1; i <= ISCR; i++) printf("%c", scr[i][1]);
		printf("\n");
		printf("%8s %10.3f %44s %10.3f\n", " ", x1, " ", x2);

	}

}

int Util::rtbis(float* vect, int x1, int x2, int xacc)
{
	int test[257];
	for (int i = 1; i <= 256; i++)
		test[i] = vect[i];

	int j, dx, rtb, xmid;
	float  f, fmid;

	while (vect[x1] == 0) {
		x1++;
		if (x1 == x2) break;
	}

	f = vect[x1];
	while (vect[x2] == 0) {
		x2--;
		if (x1 == x2) break;
	}
	fmid = vect[x2];
	if (f * fmid >= 0.0) throw MesException("Root must be bracketed for bisection in RTBIS");
	rtb = f < 0.0 ? (dx = x2 - x1, x1) : (dx = x1 - x2, x2);
	for (j = 1; j <= JMAX; j++) {
		fmid = vect[(xmid = rtb + (dx *= 0.5))];
		if (fmid <= 0.0) rtb = xmid;
		if (fabs(dx) < xacc || fmid == 0.0) return rtb;
	}
	//cout << "Too many bisections in RTBIS\n";
	throw MesException("Too many bissection");
}
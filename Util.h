#pragma once
#include <string>

// gestion des mes execptions
struct MesException {
	MesException(const std::string& message) : message(message){}
	std::string message;
};

static double at, bt, ct;
#define PYTHAG(a,b) ((at=fabs(a)) > (bt=fabs(b)) ? \
(ct=bt/at,at*sqrt(1.0+ct*ct)) : (bt ? (ct=at/bt,bt*sqrt(1.0+ct*ct)): 0.0))

static double maxarg1, maxarg2;
#define MAX(a,b) (maxarg1=(a),maxarg2=(b),(maxarg1) > (maxarg2) ?\
(maxarg1) : (maxarg2))
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
#define JMAX 30

using namespace std;

class Util
{
public:

	static float* vector(int nl, int nh);
	static void free_vector(float* v, long nl, long nh);
	static int* vectorInt(int nl, int nh);
	static void free_vectorInt(int* v, long nl, long nh);
	static float** matrix(int nrl, int nrh, int ncl, int nch);
	static void free_matrix(float** m, int nrl, int nrh, int ncl, int nch);
	static void svbksb(float** u, float w[], float** v, int m, int n, float b[], float x[]);
	static void svdcmp(float** a, int m, int n, float* w, float** v);
	static void scrsho(float(*fx)(float));
	static int Util::rtbis(float* vect, int x1, int x2, int xacc);


};

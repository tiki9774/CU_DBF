#ifndef __XMLOUT_H
#define __XMLOUT_H

typedef int     INTEGER;
typedef INTEGER LOGICAL;
typedef float   REAL;
typedef double  DBLPREC;
typedef struct { REAL re; REAL im; } COMPLEX;

// Primary array limits
#define NVMAX 2500
#define NSMAX  200
#define NFMAX   30
#define NLMAX  500
#define NBMAX   20
#define NUMAX    6
#define NDMAX   20
#define NGMAX   20
#define NRMAX   25
#define NTMAX 5000

// Derived array limits
#define ITMAX NVMAX * 2
#define IVMAX NDMAX + 5
#define ICMAX NDMAX + 10
#define IPMAX 30
#define JEMAX 12

// Character variables
struct avl_data_c {
	char fildef[80], frndef[80], fmsdef[80], fprdef[80], fevdef[80];
	char title[80];
	char stitle[NFMAX][40], btitle[NBMAX][40], rtitle[NRMAX][40];
	char dname[NDMAX][16],  gname[NGMAX][16];
	char varnam[IVMAX][12], connam[ICMAX][12];
	char varkey[IVMAX][12];
	char conkey[ICMAX][3];
	char parnam[IPMAX][10];
	char parunch[IPMAX][32];
};

// Integer variables
struct avl_data_i {
	int luinp, lurun, luout, lustd, lusys;
	int iysym, izsym;
	int matsym;
	int nvor, nstrip, nsurf, nlnode, nbody;
	int icon[NRMAX][IVMAX];
	int nvtot, nctot, nptot;
	int ncontrol, ndesign;
	int nitmax;
	int irun, nrun;
	int irune, irunt;
	int itrim[NRMAX];
	int neigen[NRMAX];
	int neigendat[NRMAX];
};

// Logical variables
struct avl_data_l {
	LOGICAL lgeo;
	LOGICAL lenc;
	LOGICAL laic;
	LOGICAL lsrd;
	LOGICAL lvel;
	LOGICAL lsol;
	LOGICAL lsen;
	LOGICAL lvisc;
	LOGICAL lmass;
	LOGICAL lcondef[NDMAX];
	LOGICAL ldesdef[NGMAX];
	LOGICAL lptot;
	LOGICAL lpsurf;
	LOGICAL lpstrp;
	LOGICAL lpele;
	LOGICAL lphinge;
	LOGICAL lpderiv;
	LOGICAL lbforce;
	LOGICAL lnasa_sa;
	LOGICAL lsa_rates;
	LOGICAL lppar[IPMAX];
};

// Real variables
struct avl_data_r {
	float version;
	float dtr, pi;
	float ysym, zsym;
	float alfa, beta;
	float vinf[3];
	float vinf_a[3];
	float vinf_b[3];
	float wrot[3];
	float dtimed;
	float parval[NRMAX][IPMAX];
	float conval[NRMAX][ICMAX];
	float delcon[NDMAX];
	float deldes[NGMAX];
	float sref, cref, bref;
	float xyzref[3];
	float xyzref0[3];
	float mach;
	float mach0;
	float cdref;
	float cdref0;
	float vrcore;
	float srcore;
	float clff, cyff, cdff;
	float clff_u[NUMAX], cyff_u[NUMAX], cdff_u[NUMAX];
	float clff_d[NDMAX], cyff_d[NDMAX], cdff_d[NDMAX];
	float clff_g[NGMAX], cyff_g[NGMAX], cdff_g[NGMAX];
	float spanef;
	float spanef_a;
	float spanef_u[NUMAX];
	float spanef_d[NDMAX];
	float spanef_g[NGMAX];
	float cdtot, cltot;
	float cxtot, cytot, cztot;
	float crtot, cmtot, cntot;
	float cdvtot;
	float cdtot_a, cltot_a;
	float cdtot_u[NUMAX], cltot_u[NUMAX];
	float cxtot_u[NUMAX], cytot_u[NUMAX], cztot_u[NUMAX];
	float crtot_u[NUMAX], cmtot_u[NUMAX], cntot_u[NUMAX];
	float cdtot_d[NDMAX], cltot_d[NDMAX];
	float cxtot_d[NDMAX], cytot_d[NDMAX], cztot_d[NDMAX];
	float crtot_d[NDMAX], cmtot_d[NDMAX], cntot_d[NDMAX];
	float cdtot_g[NGMAX], cltot_g[NGMAX];
	float cxtot_g[NGMAX], cytot_g[NGMAX], cztot_g[NGMAX];
	float crtot_g[NGMAX], cmtot_g[NGMAX], cntot_g[NGMAX];
	float chinge[NDMAX];
	float chinge_u[NUMAX][NDMAX];
	float chinge_d[NDMAX][NDMAX];
	float chinge_g[NGMAX][NDMAX];
	float dcl_a0, dcm_a0;
	float dcl_u0, dcm_u0;
};


struct avl_solv_r {
	float amach;
	float aicn[NVMAX][NVMAX];
	float wc_gam[NVMAX][NVMAX][3];
	float wv_gam[NVMAX][NVMAX][3];
	float wc[NVMAX][3];
	float wc_u[NVMAX][NUMAX][3];
	float wc_d[NVMAX][NDMAX][3];
	float wc_g[NVMAX][NGMAX][3];
	float wv[NVMAX][3];
	float wv_u[NVMAX][NUMAX][3];
	float wv_d[NVMAX][NDMAX][3];
	float wv_g[NVMAX][NGMAX][3];
};

// Common block declarations
struct avl_data_c case_c__;
struct avl_data_i case_i__;
struct avl_data_l case_l__;
struct avl_data_r case_r__;

struct avl_solv_r solv_r__;

// Local copies of common block data
struct avl_data_c cData;
struct avl_data_i iData;
struct avl_data_l lData;
struct avl_data_r rData;

struct avl_solv_r rSolv;

// New functions for xml output
void xmlPrintSymmetry(void);
void xmlPrintAxisOrientation(void);
void xmlPrintForces(void);
void xmlPrintControlVars(void);
void xmlPrintDesignVars(void);
void xmlPrintDerivs(void);
void xmlPrintParam(char*, float);
void xmlPrintComment(char*, int);

const char *fixString(char*);

#endif

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


struct avl_unit_r {
	float unitl, unitm, unitt, unitf, units, unitv, unita, uniti, unitd;
};


struct avl_unit_c {
	char unchl[32], unchm[32], uncht[32], unchf[32], unchv[32], uncha[32], unchi[32], unchd[32];
};


struct avl_unit_i {
	int nul, num, nut, nuf, nus, nuv, nua, nui, nud;
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


struct aeromlData {
	// Control surface position [surface][runcase]
	float  controlPos[NDMAX][NRMAX];

	// Design variable value [variable][runcase]
	float variableVal[NGMAX][NRMAX];

	// Angles alpha and beta [runcase]
	float alpha[NRMAX];
	float  beta[NRMAX];

	// Mach number [runcase]
	float  mach[NRMAX];

// Forces/coefficients [runcase]

	// pb/2V
	float   pb2v[NRMAX];
	// p'b/2V
	float  _pb2v[NRMAX];
	// qc/2V
	float   qc2v[NRMAX];
	// rb/2V
	float   rb2v[NRMAX];
	// r'b/2V
	float  _rb2v[NRMAX];
	// CXtot
	float  cxtot[NRMAX];
	// Cltot
	float  cltot[NRMAX];
	// Cl'tot
	float _cltot[NRMAX];
	// CYtot
	float  cytot[NRMAX];
	// Cmtot
	float  cmtot[NRMAX];
	// CZtot
	float  cztot[NRMAX];
	// Cntot
	float  cntot[NRMAX];
	// Cn'tot
	float _cntot[NRMAX];
	// CLtot
	float cl_tot[NRMAX];
	// CDtot
	float  cdtot[NRMAX];
	// CDvis
	float  cdvis[NRMAX];
	// CDind
	float  cdind[NRMAX];
	// CLff
	float   clff[NRMAX];
	// CDff
	float   cdff[NRMAX];
	// CYff
	float   cyff[NRMAX];
	// e
	float      e[NRMAX];

// Derivatives [runcase]

	float  cla[NRMAX];
	float  cya[NRMAX];
	float _cla[NRMAX];
	float  cma[NRMAX];
	float  cna[NRMAX];
	float  clb[NRMAX];
	float  cyb[NRMAX];
	float _clb[NRMAX];
	float  cmb[NRMAX];
	float  cnb[NRMAX];
	float  clp[NRMAX];
	float  cyp[NRMAX];
	float _clp[NRMAX];
	float  cmp[NRMAX];
	float  cnp[NRMAX];
	float  clq[NRMAX];
	float  cyq[NRMAX];
	float _clq[NRMAX];
	float  cmq[NRMAX];
	float  cnq[NRMAX];
	float  clr[NRMAX];
	float  cyr[NRMAX];
	float _clr[NRMAX];
	float  cmr[NRMAX];
	float  cnr[NRMAX];

// Control surface derivatives [surface][runcase]

	float   cld[NDMAX][NRMAX];
	float   cyd[NDMAX][NRMAX];
	float  _cld[NDMAX][NRMAX];
	float   cmd[NDMAX][NRMAX];
	float   cnd[NDMAX][NRMAX];
	float cdffd[NDMAX][NRMAX];
	float    ed[NDMAX][NRMAX];

// Design variable derivatives [variable][runcase]

	float   clg[NGMAX][NRMAX];
	float   cyg[NGMAX][NRMAX];
	float  _clg[NGMAX][NRMAX];
	float   cmg[NGMAX][NRMAX];
	float   cng[NGMAX][NRMAX];
	float cdffg[NGMAX][NRMAX];
	float    eg[NGMAX][NRMAX];

// Other stuff

	// Neutral point
	float xnp[NRMAX];
	// Clb*Cnr/Clr*Cnb
	float spiralStab[NRMAX];

};

// Common block declarations
struct avl_data_c case_c__;
struct avl_data_i case_i__;
struct avl_data_l case_l__;
struct avl_data_r case_r__;

struct avl_unit_r un_r__;
struct avl_unit_c un_c__;
struct avl_unit_i un_i__;

struct avl_solv_r solv_r__;

// Local copies of common block data
struct avl_data_c cData;
struct avl_data_i iData;
struct avl_data_l lData;
struct avl_data_r rData;

struct avl_unit_r rUnit;
struct avl_unit_c cUnit;
struct avl_unit_i iUnit;

struct avl_solv_r rSolv;

// Container for avl data before it gets dumped into an aeroml file
struct aeromlData xData;

void aeromlread_ (int *);
void aeromlwrite_(const char *);
void aeromlOpenFile(const char *);
void aeromlCloseFile(void);
void aeromlGetUnits(void);
void aeromlPrintHeader(void);
void aeromlPrintMetrics(void);
void aeromlPrintMassBalance(void);
void aeromlPrintSurfacesAndVars(void);
void aeromlPrintForces(void);
void aeromlPrintDerivs(void);
void aeromlPrintParam(char *, float *);
void aeromlPrintComment(char *, int);

int checkArray(float *array);

const char *fixString(char *);

#endif

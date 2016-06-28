#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>

#include "aeroml.h"

#define MAX_STRING_LENGTH		80

// Definition for subroutine GETSA, found in aoutput.f
//   (The 4th parameter is the (char *) string length)
extern void getsa_(int*, char*, float*, int);

// Two external aerodynamics definitions
extern void vinfab_(void);
extern void aero_(void);

// Global variables
float dir;						// -1 or 1, depending on the axis orientation
char satype[9];					// Standard axis orientation string
FILE *handle;					// File handle for the aeroml file to be written to
int totalCases;					// Total number of runcases. Not sure if this is necessary yet.

char lengthUnit[8], massUnit[8], timeUnit[8];

// Store all the calculated data locally
void aeromlread_ (int *runCase)
{
	int i, a = *runCase - 1;
	float wrot_rx[3], wrot_rz[3], wrot_a[3];
	float crsax_u[NUMAX], cmsax_u[NUMAX], cnsax_u[NUMAX],
		  crsax_d[NDMAX], cmsax_d[NDMAX], cnsax_d[NDMAX],
		  crsax_g[NGMAX], cmsax_g[NGMAX], cnsax_g[NGMAX];

	rData = case_r__;
	iData = case_i__;
	lData = case_l__;

	getsa_(&lData.lnasa_sa, satype, &dir, 9);

	// Continue the run case count
	if (a > totalCases)
		totalCases = a;

	float ca = cos(rData.alfa);
	float sa = sin(rData.alfa);

	// Set normalized rates in stability or body axes
	float rx_s = (rData.wrot[0] * ca + rData.wrot[2] * sa) * rData.bref / 2.0;
	float ry_s =  rData.wrot[1]							   * rData.cref / 2.0;
	float rz_s = (rData.wrot[2] * ca + rData.wrot[0] * sa) * rData.bref / 2.0;
	float rx_b =  rData.wrot[0]							   * rData.bref / 2.0;
	float ry_b =  rData.wrot[1]							   * rData.cref / 2.0;
	float rz_b =  rData.wrot[2]							   * rData.bref / 2.0;

	// Set body forces in Geometric axes
	xData.cxtot[a] = rData.cdtot * ca - rData.cltot * sa;
	xData.cztot[a] = rData.cdtot * sa + rData.cltot * ca;

	float crsax = rData.crtot * ca + rData.cntot * sa;
	float cmsax = rData.cmtot;
	float cnsax = rData.cntot * ca - rData.crtot * sa;

	// Now copy all the data from the common block and etc. into their c arrays
/*	xData.sref[a] = rData.sref;
	xData.cref[a] = rData.cref;
	xData.bref[a] = rData.bref;
	xData.xref[a] = rData.xyzref[0];
	xData.yref[a] = rData.xyzref[1];
	xData.zref[a] = rData.xyzref[2];*/
	xData.alpha[a] = rData.alfa / rData.dtr;
	xData.beta[a] = rData.beta / rData.dtr;
	xData.mach[a] = rSolv.amach;
	xData.pb2v[a] = dir * rx_b;
	xData._pb2v[a] = dir * rx_s;
	xData.qc2v[a] = dir * ry_b;
	xData.rb2v[a] = dir * rz_b;
	xData._rb2v[a] = dir * rz_s;

	float cditot = rData.cdtot - rData.cdvtot;

	xData.cxtot[a] = dir * rData.cxtot;
	xData.cltot[a] = dir * rData.crtot;
	xData._cltot[a] = dir * crsax;
	xData.cytot[a] = rData.cytot;
	xData.cmtot[a] = rData.cmtot;
	xData.cztot[a] = dir * rData.cztot;
	xData.cntot[a] = dir * rData.cntot;
	xData._cntot[a] = dir * cnsax;
	xData.cl_tot[a] = rData.cltot;
	xData.cdtot[a] = rData.cdtot;
	xData.cdvis[a] = rData.cdvtot;
	xData.cdind[a] = cditot;
	xData.clff[a] = rData.clff;
	xData.cdff[a] = rData.cdff;
	xData.cyff[a] = rData.cyff;
	xData.e[a] = rData.spanef;

	for (i = 0; i < iData.ncontrol; i++)
		xData.controlPos[i][a] = rData.delcon[i];

	for (i = 0; i < iData.ndesign; i++)
		xData.variableVal[i][a] = rData.deldes[i];

	// set freestream velocity components from alpha, beta
	vinfab_();

	// calculate forces and sensitivities
	aero_();

	// Set stability-axes rates (RX,RY,RZ) in terms of body-axes rates
	float rx = (rData.wrot[0] * ca + rData.wrot[2] * sa) * dir;
	float ry =  rData.wrot[1];
	float rz = (rData.wrot[2] * ca - rData.wrot[0] * sa) * dir;

	// Now vice-versa, and set sensitivities (which is what's really needed)
	wrot_rx[0] =  ca * dir;
	wrot_rx[1] =  0;
	wrot_rx[2] =  sa * dir;

	wrot_rz[0] = -sa * dir;
	wrot_rz[1] =  0;
	wrot_rz[2] =  ca * dir;

	wrot_a[0]  = -rx * sa - rz * ca;
	wrot_a[1]  =  0;
	wrot_a[2]  = -rz * sa + rx * ca;

	float crsax_a = -rData.cntot * sa + rData.cntot * ca;
	float cnsax_a = -rData.cntot * sa - rData.crtot * ca;

	for (i = 0; i <= 5; i++)
	{
		crsax_u[i] = rData.crtot_u[i] * ca + rData.cntot_u[i] * sa;
		cmsax_u[i] = rData.cmtot_u[i];
		cnsax_u[i] = rData.cntot_u[i] * ca - rData.crtot_u[i] * sa;
	}
	for (i = 0; i < iData.ncontrol; i++)
	{
		crsax_d[i] = rData.crtot_d[i] * ca + rData.cntot_d[i] * sa;
		cmsax_d[i] = rData.cmtot_d[i];
		cnsax_d[i] = rData.cntot_d[i] * ca - rData.crtot_d[i] * sa;
	}
	for (i = 0; i < iData.ndesign; i++)
	{
		crsax_g[i] = rData.crtot_g[i] * ca + rData.cntot_g[i] * sa;
		cmsax_g[i] = rData.cmtot_g[i];
		cnsax_g[i] = rData.cntot_g[i] * ca - rData.crtot_g[i] * sa;
	}

	// Set force derivatives in stability axes
	//   CLa
	float cl_al = rData.cltot_u[0] * rData.vinf_a[0]
				+ rData.cltot_u[1] * rData.vinf_a[1]
				+ rData.cltot_u[2] * rData.vinf_a[2]
				+ rData.cltot_u[3] * wrot_a[0]
				+ rData.cltot_u[4] * wrot_a[1]
				+ rData.cltot_u[5] * wrot_a[2]
				+ rData.cltot_a;

	float cl_be = rData.cltot_u[0] * rData.vinf_b[0]
				+ rData.cltot_u[1] * rData.vinf_b[1]
				+ rData.cltot_u[2] * rData.vinf_b[2];

	float cl_rx = rData.cltot_u[3] * wrot_rx[0]
				+ rData.cltot_u[5] * wrot_rx[2];

	float cl_ry = rData.cltot_u[4];

	float cl_rz = rData.cltot_u[5] * wrot_rz[2]
				+ rData.cltot_u[3] * wrot_rz[0];

	float cy_al = rData.cytot_u[0] * rData.vinf_a[0]
				+ rData.cytot_u[1] * rData.vinf_a[1]
				+ rData.cytot_u[2] * rData.vinf_a[2]
				+ rData.cytot_u[3] * wrot_a[0]
				+ rData.cytot_u[4] * wrot_a[1]
				+ rData.cytot_u[5] * wrot_a[2];

	float cy_be = rData.cytot_u[0] * rData.vinf_b[0]
				+ rData.cytot_u[1] * rData.vinf_b[1]
				+ rData.cytot_u[2] * rData.vinf_b[2];

	float cy_rx = rData.cytot_u[3] * wrot_rx[0]
				+ rData.cytot_u[5] * wrot_rx[2];

	float cy_ry = rData.cytot_u[4];

	float cy_rz = rData.cytot_u[5] * wrot_rz[2]
				+ rData.cytot_u[3] * wrot_rz[0];

	float cr_al = crsax_u[0] * rData.vinf_a[0]
				+ crsax_u[1] * rData.vinf_a[1]
				+ crsax_u[2] * rData.vinf_a[2]
				+ crsax_u[3] * wrot_a[0]
				+ crsax_u[4] * wrot_a[1]
				+ crsax_u[5] * wrot_a[2];
				//+ crsax_a;

	float cr_be = crsax_u[0] * rData.vinf_b[0]
				+ crsax_u[1] * rData.vinf_b[1]
				+ crsax_u[2] * rData.vinf_b[2];

	float cr_rx = crsax_u[3] * wrot_rx[0]
				+ crsax_u[5] * wrot_rx[2];

	float cr_ry = crsax_u[4];

	float cr_rz = crsax_u[5] * wrot_rz[2]
				+ crsax_u[3] * wrot_rz[0];

	float cm_al = cmsax_u[0] * rData.vinf_a[0]
				+ cmsax_u[1] * rData.vinf_a[1]
				+ cmsax_u[2] * rData.vinf_a[2]
				+ cmsax_u[3] * wrot_a[0]
				+ cmsax_u[4] * wrot_a[1]
				+ cmsax_u[5] * wrot_a[2];

	float cm_be = cmsax_u[0] * rData.vinf_b[0]
				+ cmsax_u[1] * rData.vinf_b[1]
				+ cmsax_u[2] * rData.vinf_b[2];

	float cm_rx = cmsax_u[3] * wrot_rx[0]
				+ cmsax_u[5] * wrot_rx[2];

	float cm_ry = cmsax_u[4];

	float cm_rz = cmsax_u[5] * wrot_rz[2]
				+ cmsax_u[3] * wrot_rz[0];

	float cn_al = cnsax_u[0] * rData.vinf_a[0]
				+ cnsax_u[1] * rData.vinf_a[1]
				+ cnsax_u[2] * rData.vinf_a[2]
				+ cnsax_u[3] * wrot_a[0]
				+ cnsax_u[4] * wrot_a[1]
				+ cnsax_u[5] * wrot_a[2]
				+ cnsax_a;

	float cn_be = cnsax_u[0] * rData.vinf_b[0]
				+ cnsax_u[1] * rData.vinf_b[1]
				+ cnsax_u[2] * rData.vinf_b[2];

	float cn_rx = cnsax_u[3] * wrot_rx[0]
				+ cnsax_u[5] * wrot_rx[2];

	float cn_ry = cnsax_u[4];

	float cn_rz = cnsax_u[5] * wrot_rz[2]
				+ cnsax_u[3] * wrot_rz[0];

	xData.cla[a]  =       cl_al;
	xData.cya[a]  =       cy_al;
	xData._cla[a] = dir * cr_al;
	xData.cma[a]  =       cm_al;
	xData.cna[a]  = dir * cn_al;

	xData.clb[a]  =       cl_be;
	xData.cyb[a]  =       cy_be;
	xData._clb[a] = dir * cr_be;
	xData.cmb[a]  =       cm_be;
	xData.cnb[a]  = dir * cn_be;

	xData.clp[a]  =       cl_rx * 2.0 / rData.bref;
	xData.cyp[a]  =       cy_rx * 2.0 / rData.bref;
	xData._clp[a] = dir * cr_rx * 2.0 / rData.bref;
	xData.cmp[a]  =       cm_rx * 2.0 / rData.bref;
	xData.cnp[a]  = dir * cn_rx * 2.0 / rData.bref;

	xData.clq[a]  =       cl_ry * 2.0 / rData.cref;
	xData.cyq[a]  =       cy_ry * 2.0 / rData.cref;
	xData._clq[a] = dir * cr_ry * 2.0 / rData.cref;
	xData.cmq[a]  =       cm_ry * 2.0 / rData.cref;
	xData.cnq[a]  = dir * cn_ry * 2.0 / rData.cref;

	xData.clr[a]  =       cl_rz * 2.0 / rData.bref;
	xData.cyr[a]  =       cy_rz * 2.0 / rData.bref;
	xData._clr[a] = dir * cr_rz * 2.0 / rData.bref;
	xData.cmr[a]  =       cm_rz * 2.0 / rData.bref;
	xData.cnr[a]  = dir * cn_rz * 2.0 / rData.bref;

	// Loop through and store the stability derivatives for the control surfaces
	if (iData.ncontrol > 0)
	{
		for (i = 0; i < iData.ncontrol; i++)
		{
			xData.cld[i][a]   =       rData.cltot_d[i];
			xData.cyd[i][a]   =       rData.cytot_d[i];
			xData._cld[i][a]  = dir * rData.crtot_d[i];
			xData.cmd[i][a]   =       rData.cmtot_d[i];
			xData.cnd[i][a]   = dir * rData.cntot_d[i];
			xData.cdffd[i][a] =        rData.cdff_d[i];
			xData.ed[i][a]    =      rData.spanef_d[i];
		}
	}

	// Do the same for all design variables
	if (iData.ndesign > 0)
	{
		for (i = 0; i < iData.ndesign; i++)
		{
			xData.clg[i][a]   =       rData.cltot_g[i];
			xData.cyg[i][a]   =       rData.cytot_g[i];
			xData._clg[i][a]  = dir * rData.crtot_g[i];
			xData.cmg[i][a]   =       rData.cmtot_g[i];
			xData.cng[i][a]   = dir * rData.cntot_g[i];
			xData.cdffg[i][a] =        rData.cdff_g[i];
			xData.eg[i][a]    =      rData.spanef_g[i];
		}
	}

	// Test for division by zero
	if (cl_al != 0.0)
		xData.xnp[a] = rData.xyzref[0] - rData.cref * cm_al / cl_al;
	else
		xData.xnp[a] = 0.0;

	xData.spiralStab[a] = (cr_be * cn_rz) / (cr_rz * cn_be);

}// aeromlread_


// Print out all the pertinent data to the aeroml file pointed to by handle.
void aeromlwrite_(const char *fileName)
{
	// Open the file and do some other important stuff
	aeromlOpenFile(fileName);

	// Make sure the file even exists
	if (!handle)
		return;

	// Not really necessary, but could be handy in the future
//	aeromlGetUnits();

	// Print out header-type information
	aeromlPrintHeader();

	fprintf(handle, " <input>\n\n");

	// Print the wing area/chord/span
	aeromlPrintMetrics();
	// Print the mass balance information (may not be too necessary)
	aeromlPrintMassBalance();

	fprintf(handle, "\n  <runcases> %d </runcases>\n", totalCases+1);
	fprintf(handle, "  <surfaces> %d </surfaces>\n", iData.ncontrol);
	fprintf(handle, "  <vars> %d </vars>\n\n\n", iData.ndesign);

	// Print the control surface and design variable names and values
	aeromlPrintSurfacesAndVars();

	fprintf(handle, " </input>\n\n\n\n <output>\n\n");

	// Print all the angles/forces/whatnot
	aeromlPrintForces();
	// Print all the stability derivatives
	aeromlPrintDerivs();

	fprintf(handle, " </output>\n\n");

	// We're done, close up the file
	aeromlCloseFile();

}// aeromlwrite_


// Opens the aeroml file given by fileName. If this file exists, it will be overwritten,
//   otherwise a new file will be created.
void aeromlOpenFile(const char *fileName)
{
	char buf[80];

	// Read in the common block data
	cData = case_c__;
	iData = case_i__;
	lData = case_l__;

	rUnit = un_r__;
	cUnit = un_c__;
	iUnit = un_i__;

	// Get a local copy of the file name and make it into a c-string
	strncpy(buf, fileName, 80);
	fixString(buf);

	// Open the file
	handle = fopen(buf, "w+");

	// See if this actually worked
	if (handle)
		printf("  Opening %s...\n", buf);
	else {
		printf("  Failed to open %s\n", buf);
	}

}// aeromlOpenFile


// Writes the closing tag to and closes the aeroml file pointed to by handle
void aeromlCloseFile(void)
{
	// Make sure the file exists/is open
	if (!handle)
		return;

	// Output the last closing tag and close the file
	fprintf(handle, "</avldata>\n");
	fclose(handle);

}// aeromlclose_


void aeromlGetUnits(void)
{
	strncpy(lengthUnit, cUnit.unchl, 8);
	strncpy(massUnit, cUnit.unchm, 8);
	strncpy(timeUnit, cUnit.uncht, 8);
}


void aeromlPrintHeader(void)
{
    char dateString[25];
	time_t ttime;
	struct tm *now;

	fprintf(handle, "<?xml version=\"1.0\"?>\n\n");
	fprintf(handle, "<avldata>\n\n\n\n",
			fixString(cData.title));
	fprintf(handle, " <header>\n");

	time(&ttime);
	now = localtime(&ttime);

    static char wday_name[7][3] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };
    static char mon_name[12][3] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    sprintf(dateString, "%.3s %.3s %d, %d %.2d:%.2d:%.2d",
        wday_name[now->tm_wday],
        mon_name[now->tm_mon],
        now->tm_mday,
        1900 + now->tm_year,
		now->tm_hour,
        now->tm_min, now->tm_sec);

	fprintf(handle, "  <date> %s </date>\n", dateString);
	fprintf(handle, "  <description> %s </description>\n", fixString(cData.title));
	fprintf(handle, "  <note> Generated with AVL %0.2f </note>\n", rData.version);
	fprintf(handle, " </header>\n\n\n\n");
}


void aeromlPrintMetrics(void)
{
	fprintf(handle, "  <metrics>\n");
	fprintf(handle, "   <sref> %f </sref>\n", rData.sref);
	fprintf(handle, "   <bref> %f </bref>\n", rData.bref);
	fprintf(handle, "   <cref> %f </cref>\n", rData.cref);
	fprintf(handle, "  </metrics>\n\n");
}


void aeromlPrintMassBalance(void)
{
	fprintf(handle, "  <mass>\n");
	fprintf(handle, "   <xref> %f </xref>\n", rData.parval[0][16]);
	fprintf(handle, "   <yref> %f </yref>\n", rData.parval[0][17]);
	fprintf(handle, "   <zref> %f </zref>\n", rData.parval[0][18]);/*
	fprintf(handle, "   <ixx>%f</ixx>\n", rData.parval[0][20]);
	fprintf(handle, "   <iyy>%f</iyy>\n", rData.parval[0][21]);
	fprintf(handle, "   <izz>%f</izz>\n", rData.parval[0][22]);
	fprintf(handle, "   <ixz>%f</ixz>\n", rData.parval[0][24]);
	fprintf(handle, "   <iyz>%f</iyz>\n", rData.parval[0][25]);
	fprintf(handle, "   <ixy>%f</ixy>\n", rData.parval[0][23]);
*/	fprintf(handle, "   <emptywt> %f </emptywt>\n", rData.parval[0][19]);
	fprintf(handle, "  </mass>\n\n");
}


void aeromlPrintSurfacesAndVars(void)
{

	int i;
	char buf[16], CName[17] = "";

	if (iData.ncontrol > 0)
		for (i = 0; i < iData.ncontrol; i++)
		{
			strncpy(CName, cData.dname[i], 16);
			fprintf(handle, "  <surface_%d> %s </surface_%d>\n\n", i + 1,
					fixString(CName), i + 1);
		}

	for (i = 0; i < iData.ncontrol; i++)
	{
		sprintf(buf, "surface_%d_val", i + 1);
		aeromlPrintParam(buf, xData.controlPos[i]);
	}

	if (iData.ndesign > 0)
		for (i = 0; i < iData.ndesign; i++)
		{
			strncpy(CName, cData.gname[i], 16);
			fprintf(handle, "  <var_%d> %s </var_%d>\n\n", i + 1,
					fixString(CName), i + 1);
		}

	for (i = 0; i < iData.ndesign; i++)
	{
		sprintf(buf, "var_%d_val", i + 1);
		aeromlPrintParam(buf, xData.variableVal[i]);
	}
}


// Calculates and prints out the total forces given the data contained
//   in the current run case.
void aeromlPrintForces(void)
{
	aeromlPrintParam("Alpha", xData.alpha);
	aeromlPrintParam("Beta", xData.beta);
	aeromlPrintParam("Mach", xData.mach);
	aeromlPrintParam("p_b_div_2V", xData.pb2v);
	aeromlPrintParam("p_prime_b_div_2V", xData._pb2v);
	aeromlPrintParam("q_c_div_2V", xData.qc2v);
	aeromlPrintParam("r_b_div_2V", xData.rb2v);
	aeromlPrintParam("r_prime_b_div_2V", xData._rb2v);

	aeromlPrintParam("CXtot", xData.cxtot);
	aeromlPrintParam("Cltot", xData.cltot);
	aeromlPrintParam("Cl_prime_tot", xData._cltot);
	aeromlPrintParam("CYtot", xData.cytot);
	aeromlPrintParam("Cmtot", xData.cmtot);
	aeromlPrintParam("CZtot", xData.cztot);
	aeromlPrintParam("Cntot", xData.cntot);
	aeromlPrintParam("Cn_prime_tot", xData._cntot);
	aeromlPrintParam("CLtot", xData.cl_tot);
	aeromlPrintParam("CDtot", xData.cdtot);
	aeromlPrintParam("CDvis", xData.cdvis);
	aeromlPrintParam("CDind", xData.cdind);
	aeromlPrintParam("CLff", xData.clff);
	aeromlPrintParam("CDff", xData.cdff);
	aeromlPrintParam("CYff", xData.cyff);
	aeromlPrintParam("e", xData.e);

}// aeromlPrintForces


// Calculates and prints out the stability derivatives given the data contained
//   in the current run case.
void aeromlPrintDerivs(void)
{
	int i, p;

	// Print the alpha derivatives
	aeromlPrintParam("CLa",  xData.cla);	// z force
	aeromlPrintParam("CYa",  xData.cya);	// y force
	aeromlPrintParam("Cla", xData._cla);	// x moment
	aeromlPrintParam("Cma",  xData.cma);	// y moment
	aeromlPrintParam("Cna",  xData.cna);	// z moment

	// Print the beta derivatives
	aeromlPrintParam("CLb",  xData.clb);
	aeromlPrintParam("CYb",  xData.cyb);
	aeromlPrintParam("Clb", xData._clb);
	aeromlPrintParam("Cmb",  xData.cmb);
	aeromlPrintParam("Cnb",  xData.cnb);

	// Print the p (roll rate) derivatives
	aeromlPrintParam("CLp",  xData.clp);
	aeromlPrintParam("CYp",  xData.cyp);
	aeromlPrintParam("Clp", xData._clp);
	aeromlPrintParam("Cmp",  xData.cmp);
	aeromlPrintParam("Cnp",  xData.cnp);

	// Print the q (pitch rate) derivatives
	aeromlPrintParam("CLq",  xData.clq);
	aeromlPrintParam("CYq",  xData.cyq);
	aeromlPrintParam("Clq", xData._clq);
	aeromlPrintParam("Cmq",  xData.cmq);
	aeromlPrintParam("Cnq",  xData.cnq);

	// Print the r (yaw rate) derivatives
	aeromlPrintParam("CLr",  xData.clr);
	aeromlPrintParam("CYr",  xData.cyr);
	aeromlPrintParam("Clr", xData._clr);
	aeromlPrintParam("Cmr",  xData.cmr);
	aeromlPrintParam("Cnr",  xData.cnr);

	char buf[6];

	// Loop through and print out the stability derivatives for all control surfaces
	if (iData.ncontrol > 0)
	{
		for (i = 0; i < iData.ncontrol; i++)
		{
			sprintf(buf, "CLd%d", i + 1);
			aeromlPrintParam(buf,       xData.cld[i]);
			strncpy(buf, "CYd", 3);
			aeromlPrintParam(buf,       xData.cyd[i]);
			strncpy(buf, "Cld", 3);
			aeromlPrintParam(buf,      xData._cld[i]);
			strncpy(buf, "Cmd", 3);
			aeromlPrintParam(buf,       xData.cmd[i]);
			strncpy(buf, "Cnd", 3);
			aeromlPrintParam(buf,       xData.cnd[i]);
			sprintf(buf, "CDffd%d", i + 1);
			aeromlPrintParam(buf,     xData.cdffd[i]);
			sprintf(buf, "ed%d", i + 1);
			aeromlPrintParam(buf,        xData.ed[i]);
		}
	}

	// Do the same for all design variables
	if (iData.ndesign > 0)
	{
		for (i = 0; i < iData.ndesign; i++)
		{
			sprintf(buf, "CLg%d", i + 1);
			aeromlPrintParam(buf,       xData.clg[i]);
			strncpy(buf, "CYg", 3);
			aeromlPrintParam(buf,       xData.cyg[i]);
			strncpy(buf, "Clg", 3);
			aeromlPrintParam(buf,      xData._clg[i]);
			strncpy(buf, "Cmg", 3);
			aeromlPrintParam(buf,       xData.cmg[i]);
			strncpy(buf, "Cng", 3);
			aeromlPrintParam(buf,       xData.cng[i]);
			sprintf(buf, "CDffg%d", i + 1);
			aeromlPrintParam(buf,     xData.cdffg[i]);
			sprintf(buf, "eg%d", i + 1);
			aeromlPrintParam(buf,        xData.eg[i]);
		}
	}

	// Print out the neutral point and the spiral stability
	aeromlPrintParam("Xnp", xData.xnp);
	aeromlPrintParam("spiral_stab", xData.spiralStab);

}// aeromlPrintDerivs


// Prints out a parameter in one uniform style, given the parameter's name and
//    value.
void aeromlPrintParam(char *name, float *values)
{
	int i;

	if(checkArray(values))
		fprintf(handle, "  <%s>\n", name);

	for (i = 0; i <= totalCases; i++)
		if (checkArray(values))
			fprintf(handle, "   <run_%d> %0.6f </run_%d>\n", i + 1, values[i], i + 1);
		else
		{
			fprintf(handle, "  <%s> %0.6f </%s>\n\n", name, values[i], name);
			return;
		}

	fprintf(handle, "  </%s>\n\n", name);
/*
	if(OUTPUT_STYLE)
		fprintf(handle, "   <param name=\"%s\" value=\"%0.6f\" />\n", name, value);
	else
	{
		fprintf(handle, "   <parameter>\n");
		fprintf(handle, "    <name>%s</name>\n", name);
		fprintf(handle, "    <value>%0.6f</value>\n", value);
		fprintf(handle, "   </parameter>\n");
	}
*/
}// aeromlPrintParam


// Prints out an aeroml comment tag with lSpaces leading spaces
void aeromlPrintComment(char *comment, int lSpaces)
{
	char format[16];

	sprintf(format, "%%%ds<!-- %%s -->\n", lSpaces);
	fprintf(handle, format, "", comment);

}// aeromlPrintComment


int checkArray(float *array)
{
	int i;
	float last, cur;
	float *lArray = array;

	for (i = 1; i < totalCases; i++)
	{
		last = lArray[i - 1];
		cur = lArray[i];
		// if (last != cur), adjusted for floating-point calculation
		if (fabs(last - cur) >= .000002)
			return 1;
	}

	return 0;
}


// Converts the un-terminated fortran character array contained in string into
//   a nice, null-terminated c character array. fixString returns this array too.
const char *fixString(char *string)
{
	int i, end;

	// Cut the fortran string off at some pre-determined point. So far, there
	//   aren't any common block character arrays with length > 80.
	if (strlen(string) > MAX_STRING_LENGTH)
		end = MAX_STRING_LENGTH;
	else
		end = strlen(string);

	// Loop backwards from the end of the string, filling in all the blanks
	//   with null bytes
	for (i = end - 1; i >= 0; i--)
	if (string[i] == ' ')
		string[i] = '\0';
	else
		break;

	// Return the string too so this function can be nested in other functions.
	const char *temp = string;
	return temp;

}// fixString

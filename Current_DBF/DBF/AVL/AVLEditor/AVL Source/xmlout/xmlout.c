#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>

#include "xmlout.h"

#define HTML					 1

#define OUTPUT_STYLE		  HTML
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
FILE *handle;					// File handle for the xml file to be written to


// Opens the xml file given by fileName. If this file exists, it will be overwritten,
//   otherwise a new file will be created.
void xmlopen_(const char *fileName)
{
	char buf[80];
	time_t ttime;
	struct tm *now;

	// Read in the common block data
	cData = case_c__;
	iData = case_i__;
	lData = case_l__;
	rData = case_r__;

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
		return;
	}

	// Print out a bunch of header-ish data
	fputs("<avldata>\n", handle);

	// Like the date
	time(&ttime);
	now = localtime(&ttime);

    static char wday_name[7][3] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };
    static char mon_name[12][3] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    char dateString[25];
    sprintf(dateString, "%.3s %.3s%3d %d %.2d:%.2d:%.2d",
        wday_name[now->tm_wday],
        mon_name[now->tm_mon],
        now->tm_mday,
        1900 + now->tm_year,
		now->tm_hour,
        now->tm_min, now->tm_sec);

	// And other stuff too
	fprintf(handle, " <title>%s</title>\n", fixString(cData.title));
	fprintf(handle, " <date>%s</date>\n", dateString);
	fprintf(handle, " <surfaces>%d</surfaces>\n", iData.nsurf);
	fprintf(handle, " <strips>%d</strips>\n", iData.nstrip);
	fprintf(handle, " <vortices>%d</vortices>\n", iData.nvor);

	// Output the symmetry and axis-orientation data
	xmlPrintSymmetry();
	xmlPrintAxisOrientation();

}// xmlopen_


// Writes the closing tag to and closes the xml file pointed to by handle
void xmlclose_(void)
{
	// Make sure the file exists/is open
	if (!handle)
		return;

	// Output the last closing tag and close the file
	fputs("</avldata>\n", handle);
	fclose(handle);

}// xmlclose_


// Print out all the pertinent data to the xml file pointed to by handle.
void xmlwrite_(int *runCase)
{
	char buf[100];
	char runCaseName[40];
	int i, runCaseNumber = *runCase;

	// Make sure the file even exists
	if (!handle)
		return;

	// Get the run case description and make it a c-friendly string
	strncpy(runCaseName, cData.rtitle[runCaseNumber - 1], 40);
	fixString(runCaseName);

	// Update the run case data from the CASE_R common block
	rData = case_r__;

	// Let the user know what's going on
	printf("  Writing run case %d (%s)...", runCaseNumber, runCaseName);

	// Begin the output for this particular run case
	if (HTML_STYLE)
		fprintf(handle, " <runcase number=\"%d\" name=\"%s\">\n",
				runCaseNumber, runCaseName);
	else
	{
		fprintf(handle, " <runcase>\n");
		fprintf(handle, "  <number>%d</number>\n", runCaseNumber);
		fprintf(handle, "  <name>%s</name>\n", runCaseName);
	}

	// Print out the control/design variables and their values
	xmlPrintControlVars();
	xmlPrintDesignVars();

	// Print out the total force data
	xmlPrintForces();

	// Print out the stability derivatives
	xmlPrintDerivs();

	// Close this runcase tag
	fprintf(handle, " </runcase>\n");

}// xmlwrite_


// Prints out data for y- and z-symmetry (type and location)
void xmlPrintSymmetry(void)
{
	char buf[12] = "";

	if (iData.iysym > 0)
		strcpy(buf, "wall plane");
	else if (iData.iysym < 0)
		strcpy(buf, "free surface");

	if (buf[0])
	{
		if (HTML_STYLE)
			fprintf(handle, " <y-symmetry type=\"%s\" location=\"%0.6f\" />\n",
					buf, rData.ysym);
		else
		{
			fprintf(handle, " <y_symmetry>\n");
			fprintf(handle, "  <type>%s</type>\n", buf);
			fprintf(handle, "  <location>%0.6f</location>\n", rData.ysym);
			fprintf(handle, " </y_symmetry>\n");
		}
	}

	strcpy(buf, "");

	if (iData.izsym > 0)
		strcpy(buf, "ground plane");
	else if (iData.izsym < 0)
		strcpy(buf, "free surface");

	if (buf[0])
	{
		if (HTML_STYLE)
			fprintf(handle, " <z_symmetry type=\"%s\" location=\"%0.6f\" />\n",
					buf, rData.zsym);
		else
		{
			fprintf(handle, " <z_symmetry>\n");
			fprintf(handle, "  <type>%s</type>\n", buf);
			fprintf(handle, "  <location>%0.6f</location>\n", rData.zsym);
			fprintf(handle, " </z_symmetry>\n");
		}
	}

}// xmlPrintSymmetry


// Prints out the standard axis orientation
void xmlPrintAxisOrientation(void)
{

	// Get the axis orientation (standard or geometric)
	getsa_(&lData.lnasa_sa, satype, &dir, 9);

	// Trim off the trailing space if there is one and make it lowercase
	if (satype[8] == ' ')
		satype[8] = '\0';
	satype[0] += 32;

	// Print out the corresponding xml tags
	if (HTML_STYLE)
		fprintf(handle, " <axis orientation=\"%s\" />\n", satype);
	else
	{
		fprintf(handle, " <axis_orientation>\n");
		fprintf(handle, "  <orientation>%s</orientation>\n", satype);
		fprintf(handle, "  <value>%0.1f</value>\n", dir);
		fprintf(handle, " </axis_orientation>\n");
	}

}// xmlPrintAxisOrientation


// Prints out the name and value of each control surface.
void xmlPrintControlVars(void)
{
	int i;

	// Loop through and print out the xml data for each control surface
	for (i = 0; i < iData.ncontrol; i++) {
		char CName[17];
		strncpy(CName, cData.dname[i], 16);
		if (HTML_STYLE)
			fprintf(handle, "  <control name=\"%s\" number=\"%d\" value=\"%0.6f\" />\n",
					fixString(CName), i + 1, rData.delcon[i]);
		else
		{
			fprintf(handle, "  <control>\n");
			fprintf(handle, "   <name>%s</name>\n", fixString(CName));
			fprintf(handle, "   <number>%d</number>\n", i + 1);
			fprintf(handle, "   <value>%0.6f</value>\n", rData.delcon[i]);
			fprintf(handle, "  </control>\n");
		}
	}

}// xmlPrintControlVars


// Prints out the name and value of each design variable.
void xmlPrintDesignVars(void)
{
	int i;

	// Loop through and print out the xml data for each design variable
	for (i = 0; i < iData.ndesign; i++) {
		char DName[17];
		strncpy(DName, cData.gname[i], 16);
		if (HTML_STYLE)
			fprintf(handle, "  <variable name=\"%s\" number=\"%d\" value=\"%0.6f\" />\n",
					fixString(DName), i + 1, rData.deldes[i]);
		else
		{
			fprintf(handle, "  <variable>\n");
			fprintf(handle, "   <name>%s</name>\n", fixString(DName));
			fprintf(handle, "   <number>%d</number>\n", i + 1);
			fprintf(handle, "   <value>%0.6f</value>\n", rData.deldes[i]);
			fprintf(handle, "  </variable>\n");
		}
	}

}// xmlPrintDesignVars


// Calculates and prints out the total forces given the data contained
//   in the current run case.
void xmlPrintForces(void)
{
	int i;

	// Open the total forces xml tag
	fprintf(handle, "  <forces>\n");

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
	rData.cxtot = rData.cdtot * ca - rData.cltot * sa;
	rData.cztot = rData.cdtot * sa + rData.cltot * ca;


	float crsax = rData.crtot * ca + rData.cntot * sa;
	float cmsax = rData.cmtot;
	float cnsax = rData.cntot * ca - rData.crtot * sa;

	// Now print all of the total forces parameters out to the xml file
	xmlPrintParam("Sref", rData.sref);
	xmlPrintParam("Cref", rData.cref);
	xmlPrintParam("Bref", rData.bref);
	xmlPrintParam("Xref", rData.xyzref[0]);
	xmlPrintParam("Yref", rData.xyzref[1]);
	xmlPrintParam("Zref", rData.xyzref[2]);
	xmlPrintParam("Alpha", rData.alfa / rData.dtr);
	xmlPrintParam("Beta", rData.beta / rData.dtr);
	xmlPrintParam("Mach", rSolv.amach);
	xmlPrintParam("pb/2V", dir * rx_b);
	xmlPrintParam("p'b/2V", dir * rx_s);
	xmlPrintParam("qc/2V", dir * ry_b);
	xmlPrintParam("rb/2V", dir * rz_b);
	xmlPrintParam("r'b/2V", dir * rz_s);

	float cditot = rData.cdtot - rData.cdvtot;

	xmlPrintParam("CXtot", dir * rData.cxtot);
	xmlPrintParam("Cltot", dir * rData.crtot);
	xmlPrintParam("Cl'tot", dir * crsax);
	xmlPrintParam("CYtot", rData.cytot);
	xmlPrintParam("Cmtot", rData.cmtot);
	xmlPrintParam("CZtot", dir * rData.cztot);
	xmlPrintParam("Cntot", dir * rData.cntot);
	xmlPrintParam("Cn'tot", dir * cnsax);
	xmlPrintParam("CLtot", rData.cltot);
	xmlPrintParam("CDtot", rData.cdtot);
	xmlPrintParam("CDvis", rData.cdvtot);
	xmlPrintParam("CDind", cditot);
	xmlPrintParam("CLff", rData.clff);
	xmlPrintParam("CDff", rData.cdff);
	xmlPrintParam("CYff", rData.cyff);
	xmlPrintParam("e", rData.spanef);

	// Close the total forces xml tag
	fprintf(handle, "  </forces>\n");

}// xmlPrintForces


// Calculates and prints out the stability derivatives given the data contained
//   in the current run case.
void xmlPrintDerivs(void)
{
	int i;
	float wrot_rx[3], wrot_rz[3], wrot_a[3];
	float crsax_u[NUMAX], cmsax_u[NUMAX], cnsax_u[NUMAX],
		  crsax_d[NDMAX], cmsax_d[NDMAX], cnsax_d[NDMAX],
		  crsax_g[NGMAX], cmsax_g[NGMAX], cnsax_g[NGMAX];

	// set freestream velocity components from alpha, beta
	vinfab_();

	// calculate forces and sensitivities
	aero_();

	float ca = cos(rData.alfa);
	float sa = sin(rData.alfa);

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

	float crsax   =  rData.crtot * ca + rData.cntot * sa;
	float cmsax   =  rData.cmtot;
	float cnsax   =  rData.cntot * ca - rData.crtot * sa;
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
				+ crsax_u[5] * wrot_a[2]
				+ crsax_a;

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

	// Open the stability derivatives xml tag
	fprintf(handle, "  <derivatives>\n");

	// Print the alpha derivatives
	xmlPrintParam("CLa",       cl_al);		// z force
	xmlPrintParam("CYa",       cy_al);		// y force
	xmlPrintParam("Cla", dir * cr_al);		// x moment
	xmlPrintParam("Cma",       cm_al);		// y moment
	xmlPrintParam("Cna", dir * cn_al);		// z moment

	// Print the beta derivatives
	xmlPrintParam("CLb",       cl_be);
	xmlPrintParam("CYb",       cy_be);
	xmlPrintParam("Clb", dir * cr_be);
	xmlPrintParam("Cmb",       cm_be);
	xmlPrintParam("Cnb", dir * cn_be);

	// Print the p (roll rate) derivatives;
	xmlPrintParam("CLp",       cl_rx * 2.0 / rData.bref);
	xmlPrintParam("CYp",       cy_rx * 2.0 / rData.bref);
	xmlPrintParam("Clp", dir * cr_rx * 2.0 / rData.bref);
	xmlPrintParam("Cmp",       cm_rx * 2.0 / rData.bref);
	xmlPrintParam("Cnp", dir * cn_rx * 2.0 / rData.bref);

	// Print the q (pitch rate) derivatives
	xmlPrintParam("CLq",       cl_ry * 2.0 / rData.cref);
	xmlPrintParam("CYq",       cy_ry * 2.0 / rData.cref);
	xmlPrintParam("Clq", dir * cr_ry * 2.0 / rData.cref);
	xmlPrintParam("Cmq",       cm_ry * 2.0 / rData.cref);
	xmlPrintParam("Cnq", dir * cn_ry * 2.0 / rData.cref);

	// Print the r (yaw rate) derivatives
	xmlPrintParam("CLr",       cl_rz * 2.0 / rData.bref);
	xmlPrintParam("CYr",       cy_rz * 2.0 / rData.bref);
	xmlPrintParam("Clr", dir * cr_rz * 2.0 / rData.bref);
	xmlPrintParam("Cmr",       cm_rz * 2.0 / rData.bref);
	xmlPrintParam("Cnr", dir * cn_rz * 2.0 / rData.bref);

	char buf[6];

	// Loop through and print out the stability derivatives for all control surfaces
	if (iData.ncontrol > 0)
	{
		xmlPrintComment("Begin control surface stability derivatives", 3);
		for (i = 0; i < iData.ncontrol; i++)
		{
			sprintf(buf, "CLd%d", i + 1);
			xmlPrintParam(buf,       rData.cltot_d[i]);
			strncpy(buf, "CYd", 3);
			xmlPrintParam(buf,       rData.cytot_d[i]);
			strncpy(buf, "Cld", 3);
			xmlPrintParam(buf, dir * rData.crtot_d[i]);
			strncpy(buf, "Cmd", 3);
			xmlPrintParam(buf,       rData.cmtot_d[i]);
			strncpy(buf, "Cnd", 3);
			xmlPrintParam(buf, dir * rData.cntot_d[i]);
			sprintf(buf, "CDffd%d", i + 1);
			xmlPrintParam(buf,        rData.cdff_d[i]);
			sprintf(buf, "ed%d", i + 1);
			xmlPrintParam(buf,      rData.spanef_d[i]);
		}
		xmlPrintComment("End control surface stability derivatives", 3);
	}

	// Do the same for all design variables
	if (iData.ndesign > 0)
	{
		xmlPrintComment("Begin design variable stability derivatives", 3);
		for (i = 0; i < iData.ndesign; i++)
		{
			sprintf(buf, "CLg%d", i + 1);
			xmlPrintParam(buf,       rData.cltot_g[i]);
			strncpy(buf, "CYg", 3);
			xmlPrintParam(buf,       rData.cytot_g[i]);
			strncpy(buf, "Clg", 3);
			xmlPrintParam(buf, dir * rData.crtot_g[i]);
			strncpy(buf, "Cmg", 3);
			xmlPrintParam(buf,       rData.cmtot_g[i]);
			strncpy(buf, "Cng", 3);
			xmlPrintParam(buf, dir * rData.cntot_g[i]);
			sprintf(buf, "CDffg%d", i + 1);
			xmlPrintParam(buf,        rData.cdff_g[i]);
			sprintf(buf, "eg%d", i + 1);
			xmlPrintParam(buf,      rData.spanef_g[i]);
		}
		xmlPrintComment("End design variable stability derivatives", 3);
	}

	// Print out the neutral point and that other thing down below it
	if (cl_al != 0.0)
		xmlPrintParam("Xnp", rData.xyzref[0] - rData.cref * cm_al / cl_al);
	if (fabs(cr_rz * cn_be) > 0.0001)
		xmlPrintParam("Clb*Cnr/Clr*Cnb", (cr_be * cn_rz) / (cr_rz * cn_be));

	// Finally, close the stability derivatives xml tag
	fprintf(handle, "  </derivatives>\n");

}// xmlPrintDerivs


// Prints out a parameter in one uniform style, given the parameter's name and
//    value.
void xmlPrintParam(char *name, float value)
{
	if(HTML_STYLE)
		fprintf(handle, "   <param name=\"%s\" value=\"%0.6f\" />\n", name, value);
	else
	{
		fprintf(handle, "   <parameter>\n");
		fprintf(handle, "    <name>%s</name>\n", name);
		fprintf(handle, "    <value>%0.6f</value>\n", value);
		fprintf(handle, "   </parameter>\n");
	}

}// xmlPrintParam


// Prints out an xml comment tag with lSpaces leading spaces
void xmlPrintComment(char *comment, int lSpaces)
{
	char format[16];

	sprintf(format, "%%%ds<!-- %%s -->\n", lSpaces);
	fprintf(handle, format, "", comment);

}// xmlPrintComment


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

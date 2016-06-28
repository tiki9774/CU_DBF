function stabilityroot_small()
clear all
close all;
clc;

%small plane stability
set(0, 'defaulttextfontsize', 30);
set(0, 'defaultaxesfontsize', 25);
set(0, 'defaultlinelinewidth',10);%[207/255, 184/255, 124/255]
set(0,'defaultaxescolororder',[0 0 0 ])
V = 67; %ft/s
rho = .002017;
S = 3.3;
c = 8.588/12;
b = 5;
Ix =115.023;
Iy = 6.824;
Iz = 121.814;
Ixz = -.054;
m = .1469;
q = .5*rho*V^2;
theta_0 = 0;
g = 32.174;
CM_a_dot = 0;
CD = 0.05;
CL = 0.3357;
CD_a = -.5;
CL_a = 4.463773;
CM_a = -.3474;
CL_q = 5.2341;
CM_q = -1.545245;
M_a_dot = (q*S*c/Iy)*(c/2/V)*CM_a_dot;
X_u = -(q*S/m/V)*2*CD;
X_a = (CL-CD_a)*(q*S)/m;
Z_u = 2*CL*(-q*S)/m/V;
Z_a = (CD + CL_a)*(-q*S/m);
Z_q = CL_q*c*-q*S/m/2/V;
M_u = 0;
M_a = CM_a*q*S*c/Iy;
M_q = q*S*c*c*CM_q/Iy/2/V;

CY_b = -.312446;
Cl_b = -.084772;
Cn_b = 0.1266;
CY_p = 0.04;
Cl_p = .000007;
Cn_p = -.022074;
CY_r = 0.045227;
Cl_r = 0.041531;
Cn_r = -.007695;

Y_b = q*S*CY_b/m;
Y_p = CY_p*q*S*b/2/V/m;
Y_r = CY_r*b*q*S/m/2/V;
L_b = Cl_b * q*S*b/Iz;
L_p = Cl_p * b*q*S*b/Ix/2/V;
L_r = q*S*b*b/2/V/Ix*Cl_r;
N_b = Cn_b*q*S*b/Iz;
N_p = b*Cn_p/q/S/b/Iz/2/V;
N_r = Cn_r*b*q*S*b/2/V/Iz;

In = [V 0 0 0; 0 V 0 0; 0 -M_a_dot 1 0; 0 0 0 1];
An = [V*X_u X_a 0 -g*cos(theta_0);V*Z_u Z_a V+Z_q -g*sin(theta_0); V*M_u M_a M_q 0;
    0 0 1 0];
A = inv(In)*An;
P = poly(A);
R =roots(P);
InL = [V 0 0 0; 0 1 0 -Ixz/Ix; 0 0 1 0; 0 -Ixz/Iz 0 1];
AnL = [Y_b Y_p g*cos(theta_0) Y_r-V; L_b L_p 0 L_r; 0 1 0 0; N_b N_p 0 N_r];
AL = inv(InL)*AnL;

PL = poly(AL);
RL = roots(PL);

figure(1);

hold on;
%short period and phugoid modes
plot(real(R(1:2)),imag(R(1:2)),'*');
plot(real(R(3:4)),imag(R(3:4)),'o');
hold off;
R
xlabel('Real Axis');
ylabel('Imaginary Axis');
title('Aircraft Longitudinal Roots: Production Aircraft');
legend('Phugoid--Stable','Short Period--Unstable', 'Location','Best')
grid on;
hold off;

figure(2);
hold on;
RL
plot(real(RL(1)),imag(RL(1)),'<');
plot(real(RL(2:3)),imag(RL(2:3)), 'p');
plot(real(RL(4)),imag(RL(4)),'o');
xlabel('Real Axis');
ylabel('Imaginary Axis');
title('Aircraft Lateral Directional Roots: Production Aircraft')
legend('Roll Mode--Stable', 'Dutch Roll Mode--stable','Spiral Mode--Unstable','Location','Best')

hold off;
grid on

end
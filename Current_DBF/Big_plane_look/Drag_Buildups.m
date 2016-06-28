function Drag_Buildups()
%Purpose:
    %Determine the drag on the 2015-16 DBF planes using the Drag Buildup
    %Method presented in Aircraft design a conceptual approach.
    
%Written By Timothy Kiley
%House Cleaning
close all;
clear all;
clc;

%defining varibles at location: Witchita, KS, USA
%At 1300 ft
rho = 2.2878e-3;% slugs/ft^3
mu = 3.7110e-7 ; % lbf s / ft^2
vel = 65; %ft/s
%percentage laminar
percentage = 10; %percent
%Finding reynolds number

%basic aircraft numbers
AR = 4.55;
C_L = .256;
e = .92;

stuff(1) = wings(percentage,rho,mu,vel)
stuff(2) = fues(percentage,rho,mu,vel)
stuff(3) = tail(percentage,rho,mu,vel)
stuff(4) = vert_tail(percentage,rho,mu,vel)
landinggear_cd0 = other(percentage,rho,mu,vel);

induced_dragcoef = C_L^2 / pi/e/AR;
cd_0 = (sum(stuff) )/3.09+ landinggear_cd0;
C_D = cd_0 + induced_dragcoef
end
function stuff = wings(percentage,rho,mu,vel)
%Geomertry
MAC = 1.25; %feet
Swet = 5.54*2*2; %sqft
max_t = .21;
xonc = .295;
Gamma = 0;
Q = 1.2;
character_l = MAC;
Re = rho*vel*character_l / mu;
M = vel/sqrt(1.4*1716*514.052);
C_f_lam = 1.328 / sqrt(Re);
C_f_turb = 0.455 / ((log10(Re))^2.58) / ((1+.144*M^2)^0.65);
C_f = (C_f_lam*percentage + C_f_turb*(100-percentage)) / 100;
FF = (1+((.6/xonc)*max_t) + 100*max_t^4)*(1.34*M^0.18*(cosd(Gamma))^.28)*1.2;
stuff = C_f * FF * Q*Swet;

end
function stuff = fues(percentage,rho,mu,vel)
%Geomertry
MAC = 1.88; %feet
Swet = 2*.7; %sqft
max_t = .21;
xonc = .45;
Gamma = 0;
Q = 1.0;
character_l = MAC;
Re = rho*vel*character_l / mu;
M = vel/sqrt(1.4*1716*514.052);
C_f_lam = 1.328 / sqrt(Re);
C_f_turb = 0.455 / ((log10(Re))^2.58) / ((1+.144*M^2)^0.65);
C_f = (C_f_lam*percentage + C_f_turb*(100-percentage)) / 100;
FF = (1+((.6/xonc)*max_t) + 100*max_t^4)*(1.34*M^0.18*(cosd(Gamma))^.28)*1.2;
stuff = C_f * FF * Q*Swet;

end
function stuff = tail(percentage,rho,mu,vel)
%Geomertry
MAC = 6/12; %feet
Swet = 2; %sqft
max_t = .12;
xonc = .295;
Gamma = 0;
Q = 1.05;
character_l = MAC;
Re = rho*vel*character_l / mu;
M = vel/sqrt(1.4*1716*514.052);
C_f_lam = 1.328 / sqrt(Re);
C_f_turb = 0.455 / ((log10(Re))^2.58) / (1+.144*M^2)^0.65;
C_f = (C_f_lam*percentage + C_f_turb*(100-percentage)) / 100;
FF = (1+(.6/xonc)*max_t + 100*max_t^4)*(1.34*M^0.18*cosd(Gamma)^.28);
stuff = C_f * FF * Q*Swet;

end
function stuff  = vert_tail(percentage,rho,mu,vel)
%Geomertry
MAC = .32; %feet
Swet = .58*4; %sqft
max_t = .1;
xonc = .3;
Gamma = 34;
Q = 1.05;
character_l = MAC;
Re = rho*vel*character_l / mu;
M = vel/sqrt(1.4*1716*514.052);
C_f_lam = 1.328 / sqrt(Re);
C_f_turb = 0.455 / ((log10(Re))^2.58) / (1+.144*M^2)^0.65;
C_f = (C_f_lam*percentage + C_f_turb*(100-percentage)) / 100;
FF = (1+(.6/xonc)*max_t + 100*max_t^4)*(1.34*M^0.18*cosd(Gamma)^.28);
stuff = C_f * FF * Q*Swet;

end
function stuff = other(percentage,rho,mu,vel)
%TAIL BOOM

MAC = 1.25; %feet
Swet = .19*2; %sqft
max_t = .5;
xonc = .5;
Gamma = 0;
Q = 1.00;
character_l = MAC;
Re = rho*vel*character_l / mu;
M = vel/sqrt(1.4*1716*514.052);
C_f_lam = 1.328 / sqrt(Re);
C_f_turb = 0.455 / ((log10(Re))^2.58) / ((1+.144*M^2)^0.65);
C_f = (C_f_lam*percentage + C_f_turb*(100-percentage)) / 100;
FF = (1+((.6/xonc)*max_t) + 100*max_t^4)*(1.34*M^0.18*(cosd(Gamma))^.28)*1.2;
stuff1 = C_f * FF * Q*Swet;


MAC = 4/12; %feet
Swet = .7; %sqft
max_t = .21;
xonc = .45;
Gamma = 0;
Q = 1.05;
character_l = MAC;
Re = rho*vel*character_l / mu;
M = vel/sqrt(1.4*1716*514.052);
C_f_lam = 1.328 / sqrt(Re);
C_f_turb = 0.455 / ((log10(Re))^2.58) / ((1+.144*M^2)^0.65);
C_f = (C_f_lam*percentage + C_f_turb*(100-percentage)) / 100;
FF = (1+((.6/xonc)*max_t) + 100*max_t^4)*(1.34*M^0.18*(cosd(Gamma))^.28)*1.2;
stuff = C_f * FF * Q*Swet;
%landing gear, also includes siran wrap parts
DQ = 0.25 /2.83 * .007;
spring = 1.4/2.83 * .024;
stuff = (DQ+spring)*2 + stuff+stuff1;
end
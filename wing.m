function [e,Cd,weight] = wing(AR,taper,Sref,tc,phi,w_fuse,N,V,rho,mu,n,We)
% [e,Cd,weight] = wing(AR,taper,Sref,tc,phi,w_fuse,N,V,rho,mu,n,We)
% estimates the Oswald span efficiency factor, weight, and parasite drag of 
% a wing for a small UAV
% Inputs:
% - AR: aspect ratio
% - taper: taper ratio
% - Sref: reference wing area [ft^2]
% - tc: max thickness of the airfoil normalized to the chord
% - phi: quarter-chord sweep angle [degrees]
% - w_fuse: width of the fuselage [ft]
% - N: number of engines mounted on the wings
% - V: mean flight speed [ft/s]
% - rho: air density [slug/ft^3]
% - mu: dynamic viscosity [lb*s/ft^2]
% - n: load factor [g's]
% - We: empty weight of the plane [lb]
% Outputs:
% - e: span efficiency factor
% - Cd: parasite drag coefficient for wing
% - weight: estimated wing weight
% Note: weight estimate requires wing area to be in sq. ft.
% Author: Kevin Bieri
% Last Modified: Oct 19 2015
% Last Modified by: Kevin Bieri

%% calculate root chord and wing span
cr = 2*sqrt(Sref/AR)/(1+taper); % centerline chord
b = sqrt(AR*Sref); % wingspan

%% calculate skin friction drag coefficient

% calculate mean chord
MAC = 2/3*cl*(taper^2+taper+1)/(taper+1)/12; % [ft]

% calculate flight Reynold's number
Re_f = rho*V*MAC/mu;

% cut-off Reynold's number
Re_c = 6*10^5;

% choose minimum Re out of the two
Re = min([Re_f Re_c]);

% calculate skin friction coefficient (assume fully turbulent flow)
cf = 0.455/(log10(Re))^2.58; % Prandtl-Schlichting estimation for friction coefficient

%% Calculate wing parasite drag

% find exposed wing area
c_fuse = cl*(1-taper)*2/-b*w/2 + cl; % chord at fuselage
inside = w_fuse/2*(c_fuse+cr); % total area of refence wing inside fuselage
exposed = Sref - inside; % exposed wing area

% calculate wetted wing area
Swet = exposed*(1.977+0.52*tc);

% calculate parasite drag coefficient
Cd = cf*(1+1.2*tc+100*tc^4)*1.075*Swet/Sref;

%% estimate wing weight
weight = (Sref*AR^.54*n^.84*taper^.14)/tc^.67*We; % [lb]

%% estimate Oswald span efficiency factor
% from Howe Aircraft Conceptual Design Synthesis
% applies to all monoplanes with aspect ratios above 2

M = V/1126; % mach number

e = 1/((1+ 0.12*M^2)*(1+ (0.142 +...
    0.005*(1+1.5*(taper-0.6)^2)*AR*(10*tc)^.33)/(cosd(phi))^2 +...
    0.1*(3*N+1)/(4+AR)^.8)); % Oswald efficiency factor
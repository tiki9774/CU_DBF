function [weight,Cd] = wing(taper,Sref,AR,n,V)
% estimates the weight and parasite drag of the wing for CU DBF 2014-15 plane
% Inputs:
% - taper: taper ratio
% - S: reference wing area [in^2]
% - AR: aspect ratio
% - n: load factor [g's]
% - V: mean flight speed [ft/s]
% Outputs:
% - weight: estimated wing weight [lb]
% - weight is returned as 0 if cr > 12in
% - Cd: parasite drag coefficient for wing
% Author: Kevin Bieri
% Created: Nov. 24 2014
% Last Modified: Nov. 25 2014

%% constants
w = 5.5+sqrt(2)/4; % fuselage width [in]
tc = 0.109; % maximum thickness to chord ratio
tc_mean = 0.0753; % mean thickness to chord ratio
mu = 3.7373*10^-7; % dynamic viscosity [slug/(ft*s)]
Tf = 90; % air temperature [degF]
T = Tf + 459.67; % air temperature [deg Rankine]
Phg = 29.86; % pressure [in Hg]
P = Phg*70.73; % pressure [lb/ft^2]
R = 1716; % gas constant [lb/(slug*degR)]
rho = P/(R*T);  % Air density [slugs/ft^3]

%% calculate centerline chord and wing span
cl = 2*sqrt(Sref/AR)/(1+taper); % centerline chord [in]
b = sqrt(AR*Sref); % wingspan [in]

%% calculate skin friction drag coefficient

% calculate mean chord
MAC = 2/3*cl*(taper^2+taper+1)/(taper+1)/12; % [ft]

% calculate flight Reynold's number
Re_f = rho*V*MAC/mu;

% cut-off Reynold's number
Re_c = 6*10^5;

% choose minimum Re out of the two
Re = min([Re_f Re_c]);

% calculate skin friction coefficient
cf_lam = 1.328/sqrt(Re); % laminar skin friction coefficient
cf_turb = 0.074/Re^0.2; % turbulent skin friction coefficient
cf = mean([cf_lam cf_turb]);

%% calculate wing parasite drag

% find portion of reference wing inside fuselage
cr = cl*(1-taper)*2/-b*w/2 + cl; % root chord chord [in]
inside = w/2*(cl+cr); % total area of refence wing inside fuselage [in^2]

% find area of exposed wing
exposed = Sref-inside; % total exposed wing area [in^2]

% calculate wetted area
Swet = exposed*(1.977+0.52*tc); % wetted wing area [in^2]

% calculate parasite drag coefficient
Cd = cf*(1+1.2*tc+100*tc^4)*1.075*Swet/Sref;

%% calculate LE sweep angle
deltaLE = atan(0.5*(cl-cr)/(b/2)); % leading edge sweep [rad]

%% calculate wing weight
% % ws = 0.0037*n^0.2*AR^0.85*(1+taper)^0.5/tc^0.7/cos(deltaLE); % [lb/ft^2]
% ws = 0.0009; % [lb/in^2]
% weight = ws*Sref; % wing weight [lb]

% spar weight
cf_den = 0.054; % density of carbon fiber [lb/in^3]
wall = 0.03125; % nominal wall thickness [in]
if b <= 40
    diam = 1/4; % spar outer diameter [in]
    wall = 0.03125; % nominal wall thickness [in]
elseif b > 40 && b <= 60
    diam = 3/8;
    wall = 0.03125;
elseif b > 60 && b < 80
    diam = 1/2;
    wall = 0.0625;
else
    diam = 1/2;
    wall = 0.09375;
end
vol_spar = pi*diam*wall*b; % volume of spar [in^3]
spar = vol_spar*cf_den; % weight of spar [lb]

% foam weight
rho_foam = 0.000821; % density of EPS foam [lb/in^3]
vol_wing = cr*(1+taper)/2*tc_mean*exposed; % volume of a wing [in^2]
foam = rho_foam*(vol_wing-pi*(diam/2)^2*b); % weight of foam wing [lb]

weight = spar + foam; % total wing weight [lb]

%% check if root chord exceed 12 inches
if cr > 12
    weight = 0;
end
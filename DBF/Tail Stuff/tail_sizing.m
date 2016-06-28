function [weight,SVT,SHT] = tail_sizing(L,Sref,AR,taper,CVT,CHT)
% calculates the weight optimal tail for the 2014-15 DBF plane
% Inputs:
% - L: length of the tail boom exterior to fuselage [in]
% - Sref: reference area of the wing [in^2]
% - AR: aspect ratio of wing
% - taper: taper ratio
% - CVT: vertical tail volume coefficient
% - CHT: horizontal tail volume coefficient
% Outputs:
% - weight: weight of tail assembly [lb]
% Author: Kevin Bieri
% Created: Dec. 30 2014
% Last Modified: Dec. 30 2014

%% calculate root chord and wing span
cr = 2*sqrt(Sref/AR)/(1+taper); % root chord [in]
b = sqrt(AR*Sref); % wingspan [in]

%% constants
rho_stab = 0.00090; % historical stabilizer weight per area [lb/in^2]
delta_cf = 0.0063; % linear density of cf tube with Do = 0.625in and Di = 0.5in
inside = 4; % length of tail boom inside tail cone [in]

%% calculate tail moment arm
fuse_length = inside+cr*0.75; % length of fuselage from c/4 to tail boom
arm = L + fuse_length; % stabilizer moment arm [in]

%% calculate mean aerodynamic chord
MAC = 2/3*cr*(taper^2+taper+1)/(taper+1); % mean wing aerodynamic chord [in]

%% calculate tail vertical size and weight
SVT = CVT*b*Sref/arm; % area of vertical stabilizer [in^2]
weight_VT = rho_stab*SVT; % weight of vertical stabilizer [lb]

%% calculate tail horizontal size and weight
SHT = CHT*MAC*Sref/arm; % area of horizontal stabilizer [in^2]
weight_HT = rho_stab*SHT; % weight of horizontal stabilizer [lb]

%% calculate boom weight
weight_boom = delta_cf*(L+inside); % weight of tail boom [lb]

%% calculate total tail weight
weight = weight_VT + weight_HT + weight_boom; % [lb]
function [CL,CDi,AR,e,SA] = PLLT(c,alpha0,alpha,a0,b,N)
% [CL,CDi] = PLLT(c,alpha0,alpha,a0)
% Calculates the lift and induced drag for a finite wing using Prandtl
% Lifting Line Theory (PLLT). Assumes that all given values vary linearly
% from the root to the tip.
% Inputs:
% - c: chord vector [c_root c_tip]
% - alpha0: zero lift angle of attack in degrees [alphaL=0_root alphaL=0_tip]
% - alpha: angle of attack in degrees [alpha_root alpha_tip]
% - a0: lift slope [a0_root a0_tip]
% - b: span of wing (use consistent units with chord)
% - N: number of odd terms to be used for PLLT
% Outputs:
% - CL: coefficient of lift for the wing
% - CDi: coefficient of drag for the wing
% - AR: aspect ratio of the wing
% - e: span efficiency factor of the wing
% - SA: wing area
% Author: Kevin Bieri
% Created: November 7 2014
% Last Modified: November 10 2014

%% unpack wing parameter vectors and create functions for their variation

% chord
c_r = c(1); % root chord
c_t = c(2); % tip chord
fc = @(y) (c_t-c_r)/-(b/2)*y + c_r; % function for the variation of the 
% chord for -b/2<=y<=0

% zero-lift angle of attack
alpha0 = alpha0./180*pi; % convert to radians
alpha0_r = alpha0(1); % zero-lift AoA at root
alpha0_t = alpha0(2); % zero-lift AoA at tip
falpha0 = @(y) (alpha0_t-alpha0_r)/-(b/2)*y + alpha0_r; % function for the
% variation of zero-lift AoA for -b/2<=y<=0

% angle of attack (twist)
alpha = alpha./180*pi; % convert to radians
alpha_r = alpha(1); % AoA at root
alpha_t = alpha(2); % AoA at tip
falpha = @(y) (alpha_t-alpha_r)/-(b/2)*y + alpha_r; % function for the
% variation of A0A for -b<=y<=0

% lift-slope
a0_r = a0(1); % lift slope at root
a0_t = a0(2); % lift slope at tip
fa0 = @(y) (a0_t-a0_r)/-(b/2)*y + a0_r; % function for the variation of the
% lift slope for -b/2<=y<=0

%% develop PLLT system of equations

% choose N test points
theta0 = zeros(N,1);
for i = 1:N
    theta0(i) = pi*(1-(i-1)/N)/2;
end

% create unmapped values
y0 = -b/2*cos(theta0);

% initialize coefficient matrix and RHS vector
coeff = zeros(N,N);
RHS = zeros(N,1);

% fill coefficient matrix and RHS vector
for i = 1:N % rows are test points
    RHS(i) = (falpha(y0(i))-falpha0(y0(i)));
    for j = 1:N % columns are coefficients at a given test point
        n = 2*j-1; % only use odd terms
        coeff(i,j) = 4*b/fa0(y0(i))/fc(y0(i))*sin(n*theta0(i))+...
            n*sin(n*theta0(i))/sin(theta0(i));
    end
end

% solve for An values
A = coeff\RHS;
% A(i) = A_(2i-1) where A_n is the nth PLLT coefficient it A(1) = A_1, A(2) =
% A_3, A(3) = A_5, etc.

%% calculate finite wing lift and induced drag

% calculate area of both wings
syms x
SA = 2*double(int((c_t-c_r)/-(b/2)*x + c_r,x,-b/2,0)); % total wing area

% calculate aspect ratio
AR = b^2/SA;

% calculate coefficient of lift CL
CL = A(1)*pi*AR;

% calculate space efficieny factor
delta = 0;
for i = 2:N
    n = 2*i-1; % only odd terms are used in level flight for PLLT
    delta = delta + n*(A(i)/A(1))^2;
end
e = 1/(1+delta);

% calculate induced drag coefficient
CDi = CL^2/(pi*e*AR);
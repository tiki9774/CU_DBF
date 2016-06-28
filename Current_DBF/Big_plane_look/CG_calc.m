function cord = CG_calc()
%metric weights, inches for length units
close all;
clc
%loc = [x,y]
%y out of right wing;
%x out of front
%small plane contribution
m_smallplane = 657;%g
loc_p = [11.7 0];

%fues contribution
fuesx = 14.3;
fuesy = 0;

%battery contribution
m_bat = 350 %g
loc_b = [3 0];

%other contributions
m = [51, 30 35 15 30 15 15 15 15 17 25 10 100 80 30 90 905 81 59];
loc   = [16 0
        6 0
        6.5 0
        6.5 0
        6.5 0
        19 6
        19 -6
        40 0
        40 0
        0 0
        13 0 
        13 0
        3 0
        4 0
        -.5 0
        8 0
        fuesx fuesy
        32.5 0
        43 0];
    

totalmass = sum([m,m_smallplane,m_bat]);
r = [loc; loc_b; loc_p];

mass = [m,m_bat,m_smallplane];

for i = 1:length(mass)
    for j = 1:2
       rm(i,j) = mass(i) * r(i,j) ;
    end
end
x_cg = sum(rm(:,1)) / totalmass;
y_cg = sum(rm(:,2)) / totalmass;
fprintf('Total Weight: %1.3f lbf. \n',totalmass*.00220462)
fprintf('X_CG: %1.3f inches from tip of fues \n',x_cg)
fprintf('Y_CG: %1.3f inches from center line of fues\n',y_cg)

fprintf('X_cg: %1.3f meters form tip.\n',x_cg*.0254)
fprintf('Total metric weight: %1.3f grams.',totalmass)
end
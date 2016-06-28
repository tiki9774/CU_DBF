clear all

% Battery location in global coord. Defaul is 0,4
battloc = [0 4];

m1 = [25.35 25.35 30 35 15 30 15 15 15 15 17 25 10 100 350 80 30 90];
m2 = [25.35 25.35 30 35 15 30 15 15 15 15 17 25 10 100 300 80 30 90];
loc = [12 -8.5; -12 -8.5; 0 2; 0 1; 0 1; 0 1; 20 -14; -20 -14; 27 -12; ...
    -27 -12; 0 0; 0 -13; 0 -13; 0 7; battloc; 0 3; 0 8; 0 -2];

% Total mass
mtot1 = sum(m1);
mtot2 = sum(m2);


xg1 = 0;
yg1 = 0;
xg2 = 0;
yg2 = 0;

% Calculate the x anf y loaction of the CM for mission 1 and 2
for i = 1:12
    xg1 = (1/mtot1) * m1(i)*loc(i,1) + xg1;
    yg1 = (1/mtot1) * m1(i)*loc(i,2) + yg1;
    xg2 = (1/mtot2) * m2(i)*loc(i,1) + xg2;
    yg2 = (1/mtot2) * m2(i)*loc(i,2) + yg2;
end
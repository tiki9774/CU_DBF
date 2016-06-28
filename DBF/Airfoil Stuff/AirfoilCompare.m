%% AirfoilCompare.m created by: Kevin Bieri
% Ranks the UIC airfoils using the output from AirfoilPerformance.m which
% is stored in airfoil results.txt
clear all;close all


%% Load airfoilresults.txt
fid = fopen('airfoilresults.txt');
C = textscan(fid,'%s %f %f %f %f','HeaderLines',1);
% filename S*cd_c (L/D)_c S*cm_c WF
fclose(fid);
name = C(1); % airfoil name
D = C{2}; % wing area*coefficient of drag during cruise
LD = C{3}; % L/D ratio in cruise
Cm = C{4}; % wing area * moment coefficient at cruise AoA
WF = C{5}; % weight factor
WF = WF./min(WF);
r = length(D);

%% Set up score criteria

% effective drag coefficient

% weights
wD = 0.20;
wLD = 0.45;
wCm = 0.0;
wWF = 0.35;

%% create score vector for drag coefficient product
% want to minimize
Ds = ceil(10*tiedrank(-D)/length(D));

%% create score vector for L/D in cruise
% want to maximize
LDs = ceil(10*tiedrank(LD)/length(LD));

%% create score vector for moment coefficient at cruise AoA
% want to minimize
Cms = ceil(10*tiedrank(-abs(Cm))/length(Cm));

%% create score vector for weight factor
% want to minimize
WFs = ceil(10*tiedrank(-WF)/length(WF));

%% use this to create 5 equal sized bins not equal population bins
% % find min and max values for each design feature
% minD = min(D);
% maxD = max(D);
% minLD = min(LD);
% maxLD = max(LD);
% minCm = min(abs(Cm));
% maxCm = max(abs(Cm));

% % create score vector for drag coefficient product
% D_range = linspace(minD,maxD,6);
% Dc = zeros(length(D),1);
% for i = 1:r % minimize
%     if D_range(1) <= abs(D(i)) && abs(D(i)) < D_range(2);
%         Dc(i) = 5*wD;
%     elseif D_range(2) <= abs(D(i)) && abs(D(i)) < D_range(3);
%         Dc(i) = 4*wD;
%     elseif D_range(3) <= abs(D(i)) && abs(D(i)) < D_range(4);
%         Dc(i) = 3*wD;
%     elseif D_range(4) <= abs(D(i)) && abs(D(i)) < D_range(5);
%         Dc(i) = 2*wD;
%     else
%         Dc(i) = 1*wD;
%     end
% end
% 
% % create score vector for L/D in cruise
% LD_range = linspace(minLD,maxLD,6);
% LDc = zeros(length(LD),1);
% for i = 1:r % maximize
%     if LD_range(1) <= abs(LD(i)) && abs(LD(i)) < LD_range(2);
%         LDc(i) = 1*wLD;
%     elseif LD_range(2) <= abs(LD(i)) && abs(LD(i)) < LD_range(3);
%         LDc(i) = 2*wLD;
%     elseif LD_range(3) <= abs(LD(i)) && abs(LD(i)) < LD_range(4);
%         LDc(i) = 3*wLD;
%     elseif LD_range(4) <= abs(LD(i)) && abs(LD(i)) < LD_range(5);
%         LDc(i) = 4*wLD;
%     else
%         LDc(i) = 5*wLD;
%     end
% end
% 
% % create score vector for moment coefficient at cruise AoA
% Cm_range = linspace(minCm,maxCm,6);
% Cmc = zeros(length(Cm),1);
% for i = 1:r % minimize
%     if Cm_range(1) <= abs(Cm(i)) && abs(Cm(i)) < Cm_range(2);
%         Cmc(i) = 5*wCm;
%     elseif Cm_range(2) <= abs(Cm(i)) && abs(Cm(i)) < Cm_range(3);
%         Cmc(i) = 4*wCm;
%     elseif Cm_range(3) <= abs(Cm(i)) && abs(Cm(i)) < Cm_range(4);
%         Cmc(i) = 3*wCm;
%     elseif Cm_range(4) <= abs(Cm(i)) && abs(Cm(i)) < Cm_range(5);
%         Cmc(i) = 2*wCm;
%     else
%         Cmc(i) = 1*wCm;
%     end
% end

%% find total score for each airfoil
score = zeros(r,1);
for n = 1:r
    score(n) = Ds(n)*wD + LDs(n)*wLD + Cms(n)*wCm + WFs(n)*wWF;
    if isnan(score(n))
        score(n) = 0;
    end
end

%% Sort airfoils by score

% Create cell array A of {score name S*cd_c Ds LD LDs Cm Cms WF WFs}
A = cell(r,10);
for n = 1:r
    A(n,:) = {score(n) name{1}{n} D(n) Ds(n) LD(n) LDs(n) Cm(n) Cms(n) ...
        WF(n) WFs(n)};
end

% sort A in ascending order by score (column 1)
B = sortrows(A,1);

% flip to descending order
B = flipud(B);

%% Output Results to SortedAirfoil.txt

% pull results out of cell array
for n = 1:r
    score(n) = B{n,1};
    name{1}{n} = B{n,2};
    D(n) = B{n,3};
    Ds(n) = B{n,4};
    LD(n) = B{n,5};
    LDs(n) = B{n,6};
    Cm(n) = B{n,7};
    Cms(n) = B{n,8};
    WF(n) = B{n,9};
    WFs(n) = B{n,10};
end

% create file
fid = fopen('SortedAirfoil.txt','w');

% write header
header = 'Filename  Score  S*cd_c  Dc  L/D_c  LDc  Cm  Cmc  WF  WFs';
fprintf(fid,'%s\n',header);

% write data
for n = 1:r
    if n ~= r
        fprintf(fid,'%s %6.2f %6.4f %4.0f %6.2f %4.0f %6.4f %4.0f %4.2f %4.0f \n',...
            name{1}{n},score(n),D(n),Ds(n),LD(n),LDs(n),Cm(n),Cms(n),...
            WF(n),WFs(n));
    else
        fprintf(fid,'%s %6.2f %6.4f %4.0f %6.2f %4.2f %6.4f %4.2f',...
            name{1}{n},score(n),D(n),Ds(n),LD(n),LDs(n),Cm(n),Cms(n));
    end
end

% close file
fclose(fid);
%% importairfoils.m  Written by: Kevin Bieri for CUDBF 2015
%  Reads in all airfoil files for use in constraint chart
%--------------------------------------------------------------------------

function importairfoils
% get x-foil results folder
results = dir('UI_airfoil_data');
n = length(results)-2;

% set-up export file
fidex = fopen('airfoil_data.txt','w');
% create file header
fprintf(fidex,'airfoil    clmax    cd0   cdc   clc   cm   m0   alpha0   alphaM\n');

index = 1;
for i = 1:n
    % x-foil results
    xfoil = results(i+2).name; % x-foil results file
    A = importdata(['./UI_Airfoil_data/' xfoil],' ',12); % file info
    
    % extract airfoil name
    skip = 0;
    j = 1;
    while true
        if j > numel(xfoil)
            skip = 1;
            break
        elseif ~strcmp('.',xfoil(j))
            j = j+1;
        else
            airfoil = xfoil(1:(j-1));
            break
        end
    end
    if skip == 1
        continue
    end
    
    % load corresponding coordinate file and extract coordinates
    filename = sprintf('%s%s',airfoil,'.dat');
    B = importdata(['./coord_seligFmt/' filename],' ',1);
    coord = B.data;
    x = coord(:,1);
    y = coord(:,2);

    % find top and bottom surface of airfoil
    front = find(min(x)==x,1);
    top = [x(1:front),y(1:front)];
    bot = flipud([x(front:end),y(front:end)]);
    if length(bot) < 10
        continue
    end
    
    % remove duplicate points in bot
    if length(unique(bot(:,1))) ~= length(bot(:,1))
        [C,ia,ic] = unique(bot(:,1),'stable');
        bot = bot(ia,:);
    end
    
    % find y-coordinate on bottom surface corresponding to each point on
    % top
    bot_interp = interp1(bot(:,1),bot(:,2),top(:,1));
    
    % find maximum airfoil thickness
    thick = top(:,2) - bot_interp;
    max_thick = max(thick);
    if max_thick < 0.1 % if max thickness is not greater than 10% discard airfoil
        continue
    end
    
    % find maximum airfoil camber
    camber = (top(:,2) + bot_interp)/2;
    max_camber = max(camber);
    if max_camber > 0.06 % if max camber is greater than 6% discard airfoil
        continue
    end

    if isstruct(A)
        if ~isempty(A)
            data = A.data; % airfoil data
            if ~isempty(data) & ~isnan(data)
                [r,c] = size(data);
                if c == 7 && r >= 12
                                
                % remove NaN cl rows
                while true
                    if isnan(data(end,2))
                        data(end,:) = [];
                    elseif isnan(data(1,2))
                        data(1,:) = [];
                    else
                        break
                    end
                end
                
                % remove results if AoA is never greater than 4
                if max(data(:,1)) > 6 && min(data(:,1))<0
                
                disp(i)
                x0 = find(max(data(:,2))==data(:,2),1); % index of max cl
                clmax = data(x0,2); % cl_max for the airfoil
                alphaM = data(x0,1); % AoA for cl_max
                
                % find zero lift drag coefficient
                x1 = find(abs(data(:,2))==min(abs(data(:,2))),1); % index of
                % cd0 (minimum lift approximates no lift)
                cd0 = data(x1,3);
                               
                % find cruise flight index (i.e. alpha = 2deg)
                x2 = find(min(abs(data(:,1)-2))==abs(data(:,1)-2),1);
                
                               
                % find cd, cl, cm for cruise
                cdc = data(x2,3);
                cl = data(x2,2);
                cm = data(x2,5);
                
                % find lift curve slope
                x3 = find(data(:,2)==min(data(:,2)),1); % cl_min
                x4 = find(data(:,2)==max(data(:,2)),1); % cl_max
                m0 = (data(x4-3,2)-data(x3+3,2))/(data(x4-3,1)...
                    -data(x3+3,1)); % lift slope [cl/deg]
                
                % find alpha L=0
                alpha0 = 2-cl/m0;
                                
                % print results
                fprintf(fidex,'%s %f %f %f %f %f %f %f %f\n',airfoil,clmax,cd0,...
                    cdc,cl,cm,m0,alpha0,alphaM);
                end
                index = index + 1;
                end

            end
        end
    end
end
end
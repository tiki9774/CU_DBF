%% known parameters
bh = 24; % horizontal tail span [in]
Sht = 144; % horizontal tail area [in]
bv = 6; % vertical tail height [in]
Svt = 110; % vertical tail area [in]
alpha0 = [-3 -3]; % zero-lift AoA for [root tip] [deg]
alpha = [4 1]; % AoA for [root tip] [deg]
a0 = 2*pi*[1 1]; % lift slope [root tip]
N = 10; % number of odd terms to use for PLLT

%% horizontal tail optimization

% taper range
h_taper = 0:0.01:1;

% run PLLT to estimate efficiency factor
h_e = zeros(length(h_taper),1); % horizontal tail efficiency
for i = 1:length(h_taper)
    % calculate root and tip chord
    cr = Sht/bh/(1+h_taper(i)); % root chord [in]
    ct = cr*h_taper(i); % tip chord [in]
    c = [cr ct];
    
    % calculate span efficiency
    [~,~,~,h_e(i),~] = PLLT(c,alpha0,alpha,a0,bh,N);
end

% find optimum taper ratio
[h_e_opt,h_index] = max(h_e);
h_taper_opt = h_taper(h_index); % optimum horizontal taper ratio
fprintf(1,'Horizontal taper: %.2f span efficiency: %.2f\n',h_taper_opt,...
    h_e_opt);

%% vertical tail coptimization

% taper range
v_taper = 0.63;%0:0.01:1;

% run PLLT to estimate efficiency factor
v_e = zeros(length(v_taper),1); % vertical tail efficiency
for i = 1:length(v_taper)
    % calculate root and tip chord
    cr = 2*Svt/bv/(1+v_taper(i)); % root chord [in]
    ct = cr*v_taper(i); % tip chord [in]
    c = [cr ct];
    
    % calculate span efficiency
    [~,~,~,v_e(i),~] = PLLT(c,alpha0,alpha,a0,bv,N);
end

% find optimum taper ratio
[v_e_opt,v_index] = max(v_e);
v_taper_opt = v_taper(v_index); % optimum vertical taper ratio
fprintf(1,'Vertical taper: %.2f span efficiency: %.2f\n',v_taper_opt,...
    v_e_opt);
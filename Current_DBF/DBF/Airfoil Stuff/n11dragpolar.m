% drag polars for top 5 airfoils

%% load x-foil results for n11
n11 = importdata(['./UI_Airfoil_data/' 'n11.txt'],' ',12); % file info

% extract data
data = n11.data;

% get drag coefficient data
cd = data(:,3);

% get lfit coefficient data
cl = data(:,2);

% plot cl vs cd
figure(1)
plot(cd,cl,'-r')
title('Top Airfoils: c_l vs. c_d')
grid on
xlabel('c_d')
ylabel('c_l')
hold on

%% load x-foil results for usnps4
usnps4 = importdata(['./UI_Airfoil_data/' 'usnps4.txt'],' ',12); % file info

% extract data
data = usnps4.data;

% get drag coefficient data
cd = data(:,3);

% get lfit coefficient data
cl = data(:,2);

% plot cl vs cd
plot(cd,cl,'-b')

%% load x-foil results for mh115
mh115 = importdata(['./UI_Airfoil_data/' 'mh115.txt'],' ',12); % file info

% extract data
data = mh115.data;

% get drag coefficient data
cd = data(:,3);

% get lfit coefficient data
cl = data(:,2);

% plot cl vs cd
plot(cd,cl,'-g')

%% load x-foil results for e216
e216 = importdata(['./UI_Airfoil_data/' 'e216.txt'],' ',12); % file info

% extract data
data = e216.data;

% get drag coefficient data
cd = data(:,3);

% get lfit coefficient data
cl = data(:,2);

% plot cl vs cd
plot(cd,cl,'-k')

%% load x-foil results for goe176
goe176 = importdata(['./UI_Airfoil_data/' 'goe176.txt'],' ',12); % file info

% extract data
data = goe176.data;

% get drag coefficient data
cd = data(:,3);

% get lfit coefficient data
cl = data(:,2);

% plot cl vs cd
plot(cd,cl,'-m')
hold off
legend('N-11','USNPS4','MH-115','Eppler 216','Gottingen 176','Location',...
    'SouthEast')
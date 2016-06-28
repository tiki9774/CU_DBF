%script for power vs V
close all;
clc;

set(0, 'defaulttextfontsize', 30);
set(0, 'defaultaxesfontsize', 25);
set(0, 'defaultlinelinewidth',10);

%small plane
%mission 3
e = .8;
AR = 7.56;
W = 2.133*9.8;
S = .307; %m^2
V = linspace(.1,40,100); %m/s
rho = 1.1787;%kg/m^2
cdo = 0.11;
k = 1/AR/pi/e;
P = @(V) 0.5*rho*V.^3*S*cdo + (k*W^2*2) / rho ./ V./S ;
for i = 1:length(V)
   Power(i) = P(V(i)); 
end
miss_power = P(20.73)
figure(1)
hold on;
plot([0 75], [304.3 304.3],'Color',[0 0 0])
axis([0,75,0,400]);
V = V ./ .3048;
plot(V,Power,'Color',[207/255, 184/255, 124/255])
plot(68,miss_power,'*')
title('Power Requirement For Mission 3')
xlabel('Cruise Velocity [ft/s]')
ylabel('Power Required [W]')
grid on 
axis([0,70,0,400])


%bigplane mission 2 empty big plane
e = .85;
AR = 4.19;
W = 2.625*9.8;
S = .557; %m^2
V = linspace(.1,25,100); %m/s

rho = 1.1787;%kg/m^2
cdo = 0.12;
k = 1/AR/pi/e;
P = @(V) 0.5*rho*V.^3*S*cdo + (k*W^2*2) / rho ./ V./S ;
for i = 1:length(V)
   Power(i) = P(V(i)); 
end
miss_power = P(17.68)
figure(2)
hold on;
plot([0 65], [378 378],'Color',[0 0 0])
V = V/.3048;
plot(V,Power,'Color',[207/255, 184/255, 124/255])
plot(58,miss_power,'*')
title('Power Requirement For Mission 2')
xlabel('Cruise Velocity [ft/s]')
ylabel('Power Required [W]')
grid on;
hold off;


%bigplane mission 1 loaded big plane
e = .85;
AR = 4.19;
W = 2*9.8;
S = .557; %m^2
V = linspace(.1,25,100); %m/s

rho = 1.1787;%kg/m^2
cdo = 0.2;
k = 1/AR/pi/e;
P = @(V) 0.5*rho*V.^3*S*cdo + (k*W^2*2) / rho ./ V./S ;

for i = 1:length(V)
 power(i) = P(V(i));
end
miss_power = P(15.15)

figure(3)
hold on;
plot([0 65], [378 378],'Color',[0 0 0])
V = V /.3048; 
plot(V,power,'Color',[207/255, 184/255, 124/255])
plot(49.7,miss_power,'*')
title('Power Requirement For Mission 1')
xlabel('Cruise Speed [ft/s]')
ylabel('Power Required [W]')
grid on
function bigplane_fit()
close all;
clc;
%Use american units
%Getting air foil data
set(0, 'defaulttextfontsize', 18);
set(0, 'defaultaxesfontsize', 15);
set(0, 'defaultlinelinewidth',3);
%center_foil = load('FX60_177');
%center_foil = load('NASA_Langley_MS0317');
%center_foil = load('NACA25125');
center_foil = load('NACA66021.dat');
wing_foil = load('NACA2414.dat');
big_x = 25*center_foil(:,1);
big_y = 25*center_foil(:,2);
big_z = zeros(length(big_y),1);

big_x_1 = 22*center_foil(:,1);
big_y_1 = 22*center_foil(:,2);
big_z_1 = 2*ones(length(big_y),1);
%naca 2414 airfoil sections
wing_x = wing_foil(:,1);
wing_y = wing_foil(:,2);
%small plane airfoil sections
airfoil_root = load('HS602.dat');
airfoil_midtip = load('HS520.dat');
%defiing rotation matrix for airfoil sections
T = @(phi)[cosd(phi) sind(phi) 0; -sind(phi) cosd(phi) 0; 0 0 1];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%offsets for big plane wing sections
offsetcenter = 0;
offset_body1 = 2; %in

offset_root = 6;
rootcord_b = 15;
rootangle = 4;

offset_mid = 11;
midspancord = 13;
midspanangle=4;%deg

offset_tip = 15;
tipspancord = 11;
offset_tip1 = 16;
tipspanangle = -4;%deg

sparradius = 0.5;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%small plane
%small plane fues
t = -4:.1:0; %in
y_t = -sqrt(4*(1-(t/4).^2));
y1 = [fliplr(y_t),-y_t];
y1(42) = [];
x1 =[ 0:-.1:-3.9, -4:.1:0];
x2 = [0 5.5];
y2 = [2 2];
x3 = [ 5.5:.1:7.4, 7.5:-.1:5.5];
t = 0:.1:2;
y_t = sqrt(4-t.^2);
y3 = [y_t,-fliplr(y_t)];
y3(21) = [];
x4 = [5.5 0];
y4 = [-2 -2];
y = [y1 y2 y3 y4];
x = [x1 x2 x3 x4]+8.5;
z = zeros(1,length(x));
templinex = [8.5 8.5];
templiney = [-3 3];
templinez = [0 0];

disp('motor clearence')
fprintf('Tip of small plane fues to tip fues big plane: %1.3f inches. \n',min(x));

%wing section for small plane
%importing airfoil sections
airfoil_rx = 10*airfoil_root(:,1);
airfoil_ry = 10*airfoil_root(:,2);
airfoil_mx = 8.5*airfoil_midtip(:,1);
airfoil_my = 8.5*airfoil_midtip(:,2);
airfoil_tx = 7*airfoil_midtip(:,1);
airfoil_ty = 7*airfoil_midtip(:,2);
%root
root_x = (airfoil_rx - 2)';
root_y = airfoil_ry';
root_z = ones(1,length(root_x))*2;
root_vec = [root_x;root_y;root_z];

for i = 1:length(root_vec)
   root_cord(i,:) = T(4)*root_vec(:,i); 
end
root_xs =  8.5+ root_cord(:,1);
root_ys = root_cord(:,2);
root_zs = root_cord(:,3);
%midspan
root_mx = (airfoil_mx - 1.7)';
root_my = airfoil_my';
root_mz = ones(1,length(root_mx))*16;
root_vec_m = [root_mx;root_my;root_mz];

for i = 1:length(root_vec_m)
   root_cord_m(i,:) = T(4)*root_vec_m(:,i); 
end
root_mxs =  6.5+4.5+1.7+ root_cord_m(:,1);
root_mys = root_cord_m(:,2);
root_mzs = root_cord_m(:,3);
%tip small plane
root_tx = (airfoil_tx - 1.4)';
root_ty = airfoil_ty';
root_tz = ones(1,length(root_tx))*30;
root_vec_t = [root_tx;root_ty;root_tz];

for i = 1:length(root_vec_t)
   root_cord_t(i,:) = T(-5)*root_vec_t(:,i); 
end
root_cord_t(:,1);
root_txs =  6.5+9+1.4+ root_cord_t(:,1);
root_tys = root_cord_t(:,2);
root_tzs = root_cord_t(:,3);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%bigplane "root" section

root_xb1 = rootcord_b*wing_x-sparradius;
root_yb1 = rootcord_b*wing_y ;
root_zb1 = 4*ones(length(root_xb1),1);
Z = [root_xb1,root_yb1,root_zb1]';
for i = 1:length(root_xb1)
root_b1(i,:) = T(rootangle)*Z(:,i);
end
root_xb1 = root_b1(:,1)+offset_root;
root_yb1 = root_b1(:,2);
root_zb1 = root_b1(:,3);

%midmidspan
root_xb2 = midspancord*wing_x-sparradius;
root_yb2 = midspancord*wing_y ;
root_zb2 = 16*ones(length(root_xb1),1);
Z = [root_xb2,root_yb2,root_zb2]';
for i = 1:length(root_xb2)
root_b2(i,:) = T(midspanangle)*Z(:,i);
end
root_xb2 = root_b2(:,1)+offset_mid;
root_yb2 = root_b2(:,2);
root_zb2 = root_b2(:,3);
%tiptip
root_xb3 = tipspancord*wing_x-sparradius;
root_yb3 = tipspancord*wing_y ;
root_zb3 = 30*ones(length(root_xb3),1);
Z = [root_xb3,root_yb3,root_zb3]';
for i = 1:length(root_xb3)
root_b3(i,:) = T(tipspanangle)*Z(:,i);
end
root_xb3 = root_b3(:,1)+offset_tip;
root_yb3 = root_b3(:,2);
root_zb3 = root_b3(:,3);


figure(1);

hold on;
grid on;
plot3(big_x, big_y,big_z)%centerfues
 plot3(offset_body1+big_x_1, big_y_1,big_z_1)%fues

 plot3(root_xb1,root_yb1,root_zb1)%"root"
 plot3(root_xb2,root_yb2,root_zb2)%midmidspan
 plot3(root_xb3,root_yb3,root_zb3)%tip
%plting small plane
 plot3(templinex,templiney,templinez)
plot3(x,y,z,'r')
 plot3(root_xs,root_ys,root_zs,'r')
 plot3(root_mxs,root_mys,root_mzs,'r')
  plot3(root_txs,root_tys,root_tzs,'r')
hold off
set(gca,'XMinorTick','on','YMinorTick','on')
axis equal

end
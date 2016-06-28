function mean_thickness_ratio = mean_thickness(airfoil)
% calculates the mean thickness ratio of an airfoil given its coordinate
% file
% Author: Kevin Bieri
% Created: Nov 25 2014
% Last Modified: Nov 25 2014

% load file
B = importdata(['./coord_seligFmt/' airfoil],' ',1);
coord = B.data;
x = coord(:,1);
y = coord(:,2);

% find top and bottom surface of airfoil
front = find(min(x)==x,1);
top = [x(1:front),y(1:front)];
bot = flipud([x(front:end),y(front:end)]);

% remove duplicate points in bot
if length(unique(bot(:,1))) ~= length(bot(:,1))
    [C,ia,ic] = unique(bot(:,1),'stable');
    bot = bot(ia,:);
end

% interpolate surfaces to find 100 evenly spaced points along the top and
% bottom surfaces
points = linspace(1,0,100);
top_interp = interp1(top(:,1),top(:,2),points,'pchip');
bot_interp = interp1(bot(:,1),bot(:,2),points,'pchip');

% plot airfoil
figure(1)
plot(points,top_interp,'b')
hold on
plot(points,bot_interp,'b')
hold off
axis('equal')

% find airfoil thickness
thick = top_interp - bot_interp;

% find mean airfoil thickness ratio
mean_thickness_ratio = mean(thick);
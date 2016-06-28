% constants
CVT = 0.06; % vertical tail volume coefficient
CHT = 0.50; % horizontal tail volume coefficient
AR = 4.55; % aspect ratio of the main wing
taper = 0.4405; % taper ratio of the main wing
Sref = 792; % reference wing area [in^2]

L = 1.5:0.01:30; % tail boom lengths [in]

% calculate weight of tail boom
W = zeros(length(L),1);
for i = 1:length(L)
    [W(i),~,~] = tail_sizing(L(i),Sref,AR,taper,CVT,CHT); % [lb]
end

% find optimal length
index = find(min(W)==W,1,'first');
L_opt = L(index); % optimal tail length
W_opt = W(index); % estimated weight of optimal length tail

% plot results
figure(1)
plot((L+4)./12,W,'-b','LineWidth',1.5);
hold on
plot((L_opt+4)/12,W_opt,'*r','MarkerSize',10);
hold off
xlabel('Length [ft]')
ylabel('Weight [lb]')
title('Tail and Boom Weight vs. Boom Length')
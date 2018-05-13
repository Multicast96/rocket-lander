% close all;
% 
% max_x = 3000;
% max_y = 500;
% curveity_x = 1;
% curveity_y = 2;
% 
% [x, y] = meshgrid(linspace(0,max_x), linspace(-max_y,max_y));
% f1 = 1 - logsig(curveity_x./(1-1/max_x) + curveity_y./(1-1/max_y));
% f = (1 - logsig(curveity_x./(1-x./max_x) + curveity_y./(1-abs(y./max_y)))) / f1;
% figure;
% surf(x, y, f);
% xlabel('height');
% ylabel('velocity');
% zlabel('reward');
% 
% figure;
% f1 = 1 - logsig(1./(1-1/max_x) + 1./(1-1/max_y));
% f = (1 - logsig(1./(1-x./max_x) + 1./(1-abs(y./max_y)))) / f1;
% surf(x, y, f);
% xlabel('height');
% ylabel('velocity');
% zlabel('reward');

close all;

h_max = 750;
h = linspace(0, h_max*2);
figure;
plot(h, 1 - logsig((h - h_max/2)./(h_max/6)));
grid on;

v_max = 300;
v = linspace(-2*v_max, 2*v_max);
figure;
plot(v, 1 - logsig((abs(v) - v_max/2)./(v_max/6)));
grid on;

t_max = 8;
t = linspace(-1, t_max + 4);
figure;
plot(t, logsig((t - t_max/2)./(t_max/6)));
grid on;
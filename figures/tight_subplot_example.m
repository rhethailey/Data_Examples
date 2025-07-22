% Clean example of paper fig w/ pdf export
% Rhet Haliey - 2024

clc, clear all, close all
% 

% insert your color pallette of choice
col = parula(4);

set(groot,'defaulttextinterpreter','latex');
set(groot,'defaultLegendInterpreter','latex');
set(groot,'defaultAxesTickLabelInterpreter','latex');
set(groot,'DefaultAxesFontSize',24);
set(groot, 'DefaultLineLineWidth', 3);
set(groot, 'DefaultAxesColorOrder', col); % lazy color changing


dt = .01;
t = 0:dt:2*pi;
y1 = sin(t);
y2 = sin(2*t);
x = cos(t);

dy1 = [0 diff(y1)/dt];
dy2 = [0 diff(y2)/dt];
dx = [0 diff(x)/dt];



figure(1)
clf(1)


set(figure(1), 'Units', 'Inches', 'Position', [1.5 0.25, 10, 6]); % [x, y, width, height]
% sizing of tight axes sublots
ha = tight_subplot(2,1,[.035 .035],[.15 .06],[.1 .05]);


% Plot bountds
xbounds = [0 2*pi];
ybounds = 1.5*[-1 1];
velbound = 2.5*[-1 1];



% position
axes(ha(1))
hold on

py1 = plot(t,y1);
py2 = plot(t,y2);
px = plot(t,x);

l = legend([py1, py2, px],{'sin(t)','sin(2t)','cos(t)'},'Location','northoutside',...
    'Orientation','horizontal');

ylabel("Position (m)")
xlim(xbounds)
ylim(ybounds)


% Velocity
axes(ha(2))
hold on
dpy1 = plot(t,dy1);
dpy2 = plot(t,dy2);
dpx = plot(t,dx);

ylabel("Velocity (m/s)")
ylim([velbound])
xlim(xbounds)
xlabel("Time (s)")


set(ha(:), 'XGrid', 'on', 'YGrid', 'on')  % For 2D plots
set(ha(1:2),'TickLabelInterpreter','latex')
set(ha(1:2), 'YTick', -2:1:2);  % Define x-tick positions
set(ha(1:2), 'YTickLabelMode', 'auto');  % Define x-tick positions
set(ha(1:2), 'XTick', 0:pi/2:2*pi);  % Define x-tick position
set(ha(2),'XTickLabel',{'0','$\pi/2$','$\pi$','$3\pi/2$','$2\pi$','$\pi/2$','$\pi$','$3\pi/2$','$2\pi$'})


%% save // LAZY
answer = questdlg('Do you want to save your figs?', ...
    'fig prompts', ...
    'yes','no','no');

switch answer
    case 'yes'
        
        pos = get(gcf, 'Position');
        set(gcf, 'PaperPositionMode', 'Auto', ...
            'PaperUnits', 'Inches', ...
            'PaperSize', [pos(3), pos(4)]);
        
        % Export to PDF with vector graphics and proper resolution
        % grabs current figure
        print(gcf, ['figs/tight_subplot_example'], '-dpdf', '-vector');

    case 'no'
end

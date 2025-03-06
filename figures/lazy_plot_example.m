clear all, clc


% insert your color pallette of choice
col = parula(4);

set(groot,'defaulttextinterpreter','latex');
set(groot,'defaultLegendInterpreter','latex');
set(groot,'defaultAxesTickLabelInterpreter','latex');
set(groot,'DefaultAxesFontSize',20);
set(groot, 'DefaultLineLineWidth', 3);
set(groot, 'DefaultAxesColorOrder', col); % lazy color changing

dt = .01;
t = 0:dt:2*pi;
y1 = sin(t);
y2 = sin(2*t);

x = cos(t);


figure(1)
clf(1)

set(figure(1), 'Units', 'Inches', 'Position', [1.5 0.25, 10, 6]); % [x, y, width, height]

hold on
grid on

py1 = plot(t,y1);
py2 = plot(t,y2);
px = plot(t,x);

l = legend([py1, py2, px],{'sin(t)','sin(2t)','cos(t)'},'Location','northoutside',...
    'Orientation','horizontal');
l.FontSize = 32;

yl = ylabel("Output");
xl = xlabel("Time [s]");

ax = gca;
ax.YAxis.FontSize = 32;
ax.XAxis.FontSize = 32;


xLIM = [0 2*pi];
yLIM = [-1 1]*1.25

xticks('auto')
yticklabels('auto')

set(ax, 'YTick', -1:.5:1);  % Define x-tick positions
set(ax, 'XTick', 0:pi/2:2*pi);  % Define x-tick position
set(ax,'XTickLabel',{'0','$\pi/2$','$\pi$','$3\pi/2$','$2\pi$','$\pi/2$','$\pi$','$3\pi/2$','$2\pi$'})
set(ax,'XLim',xLIM)
set(ax,'YLim',yLIM)


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
        print(gcf, ['figs/lazy_plot'], '-dpdf', '-vector');

    case 'no'
end

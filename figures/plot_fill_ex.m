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
num = 10;

xtrue = cos(t);
x = xtrue.*[ones(num,1)] + 0.2*randn(num,length(t));
xvar = std(x);
xmean = mean(x);

% 3 standard devations encompasses 99.7% of the data
xlb = xtrue - 3*xvar;
xub = xtrue + 3*xvar;


figure(1)
clf(1)

set(figure(1), 'Units', 'Inches', 'Position', [1.5 0.25, 10, 6]); % [x, y, width, height]

hold on
grid on

pxm = plot(t,xmean);
pxt = plot(t,xtrue);

fxstd = fill([t fliplr(t)], [xlb fliplr(xub)], col(1,:), ...
    'FaceAlpha', .25, 'EdgeColor', 'none'); 


l = legend([pxt pxm fxstd],{'x$_{true}$','x$_{mean}$','3$\sigma_x$'},'Location','northoutside',...
    'Orientation','horizontal');
l.FontSize = 32;

yl = ylabel("Output");
xl = xlabel("Time [s]");

ax = gca;
ax.YAxis.FontSize = 32;
ax.XAxis.FontSize = 32;


xLIM = [0 2*pi];
yLIM = [-1 1]*1.75

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
        print(gcf, ['figs/fill_ex'], '-dpdf', '-vector');

    case 'no'
end

function [x,y,r,theta] = phase_variable(indicator,t)
    x = indicator-mean(indicator);
    y = cumtrapz(t,x);
    figure
    scatter(x,y)
    theta = (atan2(y,x));
    r = vecnorm([x,y],2,2);
end
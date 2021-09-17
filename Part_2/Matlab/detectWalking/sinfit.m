function [B,X] = sinfit(x,y,degree)
    X = ones(length(x),1);
    for i = 1:degree
        X = [X,sin(i*x)];
    end
    B = (X'*X)^-1;
    B = B*X';
    B = B*y;
end
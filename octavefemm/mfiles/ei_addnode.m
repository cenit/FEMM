% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_addnode(x,y)
if (nargin==2)
    if (length(x)>1)
        if (length(y)~=length(x))
            error('invalid arguments to ei_addnode');
        end
        for k=1:length(x)
            callfemm(['ei_addnode(' , numc(x(k)) , num(y(k)) , ')']);
        end
    else callfemm(['ei_addnode(' , numc(x) , num(y) , ')']);
    end
elseif (nargin==1)
    callfemm(['ei_addnode(' , numc(x(1)) , num(x(2)) , ')']);
end


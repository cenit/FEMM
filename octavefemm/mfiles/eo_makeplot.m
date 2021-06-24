% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function eo_makeplot(plottype,numpoints,filename,fileformat)
if (nargin==4)
callfemm(['eo_makeplot(' , numc(plottype) , numc(numpoints) , quotec(filename) , num(fileformat) , ')' ]);
end
if (nargin==3)
callfemm(['eo_makeplot(' , numc(plottype) , numc(numpoints) , quote(filename) , ')' ]);
end
if (nargin==2)
callfemm(['eo_makeplot(' , numc(plottype) , num(numpoints) , ')' ]);
end


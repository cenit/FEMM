% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=co_gete(x,y)
if (nargin==2)
	z=co_getpointvalues(x,y); z=z(:,6:7);
elseif (nargin==1)
	z=co_getpointvalues(x); z=z(:,6:7);
end


% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=eo_getperm(x,y)
if (nargin==2)
	z=eo_getpointvalues(x,y); z=z(:,6:7);
elseif (nargin==1)
	z=eo_getpointvalues(x); z=z(:,6:7);
end


% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=eo_getv(x,y)
if (nargin==2)
	z=eo_getpointvalues(x,y); z=z(:,1);
elseif (nargn==1)
	z=eo_getpointvalues(x); z=z(:,1);
end


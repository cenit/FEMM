% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=eo_getd(x,y)
if (nargin==2)
	z=eo_getpointvalues(x,y); z=z(:,2:3);
elseif (nargin==1)
	z=eo_getpointvalues(x); z=z(:,2:3);
end


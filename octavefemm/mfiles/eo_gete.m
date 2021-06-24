% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=eo_gete(x,y)
if (nargin==2)
	z=eo_getpointvalues(x,y); z=z(:,4:5);
elseif (nargin==1)
	z=eo_getpointvalues(x); z=z(:,4:5);
end


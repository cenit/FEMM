% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=mo_getfill(x,y)
if (nargin==2)
	z=mo_getpointvalues(x,y); z=z(:,14);
elseif (nargin==1)
	z=mo_getpointvalues(x(1),x(2)); z=z(:,14);
end





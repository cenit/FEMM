% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=mo_getb(x,y)
if (nargin==2)
	z=mo_getpointvalues(x,y); z=z(:,2:3);
elseif (nargin==1)
	z=mo_getpointvalues(x); z=z(:,2:3);
end


% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=co_getv(x,y)
if (nargin==2)
	z=co_getpointvalues(x,y); z=z(:,1);
elseif (nargin==1)
	z=co_getpointvalues(x); z=z(:,1);
end


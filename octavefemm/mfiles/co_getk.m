% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=co_getk(x,y)
if (nargin==2)
	z=co_getpointvalues(x,y); z=z(:,4:5);
elseif (nargin==1)
	z=co_getpointvalues(x); z=z(:,4:5);
end


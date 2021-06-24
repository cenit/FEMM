% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=co_getj(x,y)
if (nargin==2)
	z=co_getpointvalues(x,y); z=z(:,2:3);
elseif (nargin==1)
	z=co_getpointvalues(x); z=z(:,2:3);
end


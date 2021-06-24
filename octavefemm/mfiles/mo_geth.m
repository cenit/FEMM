% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=mo_geth(x,y)
if (nargin)==2
	z=mo_getpointvalues(x,y); z=z(:,6:7);
elseif (nargin==1)
	z=mo_getpointvalues(x); z=z(:,6:7);
end


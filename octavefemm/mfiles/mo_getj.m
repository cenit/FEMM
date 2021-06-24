% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=mo_getj(x,y)
if (nargin==2)
	z=mo_getpointvalues(x,y); z=z(:,8:9)*[1;1];
elseif (nargin==1)
	z=mo_getpointvalues(x); z=z(:,8:9)*[1;1];
end


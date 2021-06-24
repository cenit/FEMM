% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=mo_getpe(x,y)
if (nargin==2)
	z=mo_getpointvalues(x,y); z=z(:,12);
elseif (nargin==1)
	z=mo_getpointvalues(x); z=z(:,12);
end


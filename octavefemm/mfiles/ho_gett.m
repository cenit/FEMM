% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function z=ho_gett(x,y)
if (nargin==2)
	z=ho_getpointvalues(x,y); z=z(:,1);
elseif (nargin==1)
	z=ho_getpointvalues(x); z=z(:,1);
end


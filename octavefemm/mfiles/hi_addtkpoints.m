% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_addtkpoints(name,bh,h)
if(nargin==3)
	hi_addtkpoints(name,[bh;h]');
elseif (nargin==2)
	n=size(bh); n=n(1);
	for k=1:n
      	hi_addtkpoint(name,bh(k,1),bh(k,2));
	end
end


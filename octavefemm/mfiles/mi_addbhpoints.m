% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_addbhpoints(name,bh,h)
if(nargin==3)
	mi_addbhpoints(name,[bh;h]');
elseif (nargin==2)
	n=size(bh); n=n(1);
	for k=1:n
      	mi_addbhpoint(name,bh(k,1),bh(k,2));
	end
end


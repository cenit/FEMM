function z=mo_getgapharmonics(bdryname,angle)
	if (nargin==1)
		z=callfemm(['mo_getgapharmonics(' , quote(bdryname) ')']);
	else
		z=callfemm(['mo_getgapharmonics(' , quotec(bdryname) , num(angle) , ')']);
	end

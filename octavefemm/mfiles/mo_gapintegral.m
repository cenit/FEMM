function z=mo_gapintegral(bdryname,inttype)
	z=callfemm(['mo_gapintegral(' , quotec(bdryname) , num(inttype) , ')']);

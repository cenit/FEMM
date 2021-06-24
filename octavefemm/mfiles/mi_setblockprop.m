% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_setblockprop(blockname,automesh,meshsize,incirc,magdir,group,turns)
try
	bStr=isa(magdir,'numeric');
catch
	bStr=isreal(magdir);
end
if (bStr==1)
callfemm(['mi_setblockprop(' , quotec(blockname) , numc(automesh) , numc(meshsize) , quotec(incirc) , numc(magdir) , numc(group) , num(turns) , ')']);
else
callfemm(['mi_setblockprop(' , quotec(blockname) , numc(automesh) , numc(meshsize) , quotec(incirc) , quotec(magdir) , numc(group) , num(turns) , ')']);
end


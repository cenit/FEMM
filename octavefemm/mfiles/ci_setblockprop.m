% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ci_setblockprop(blockname,automesh,meshsize,group)
callfemm(['ci_setblockprop(' , quotec(blockname) , numc(automesh) , numc(meshsize) , num(group) , ')']);


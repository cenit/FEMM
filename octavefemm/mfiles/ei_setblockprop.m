% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function ei_setblockprop(blockname,automesh,meshsize,group)
callfemm(['ei_setblockprop(' , quotec(blockname) , numc(automesh) , numc(meshsize) , num(group) , ')']);


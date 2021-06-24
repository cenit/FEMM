% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_addmaterial(name,mux,muy,hc,js,c,lamd,phih,lamfill,lamtype,phihx,phihy,nstr,dwire)

switch nargin
	case 0
		callfemm(['mi_addmaterial(',quote('Air'),')']);
	case 1
		callfemm(['mi_addmaterial(',quote(name),')']);
	case 2
		callfemm(['mi_addmaterial(',quotec(name),num(mux),')']);
	case 3
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),num(muy),')']);
	case 4
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),num(hc),')']);
	case 5
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),num(js),')']);
	case 6
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),num(c),')']);
	case 7
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),numc(c),num(lamd),')']);
	case 8
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),numc(c),numc(lamd),num(phih),')']);
	case 9
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),numc(c),numc(lamd),numc(phih),num(lamfill),')']);
	case 10
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),numc(c),numc(lamd),numc(phih),numc(lamfill),num(lamtype),')']);
	case 11
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),numc(c),numc(lamd),numc(phih),numc(lamfill),numc(lamtype),num(phihx),')']);
	case 12
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),numc(c),numc(lamd),numc(phih),numc(lamfill),numc(lamtype),numc(phihx),num(phihy),')']);
	case 13
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),numc(c),numc(lamd),numc(phih),numc(lamfill),numc(lamtype),numc(phihx),num(phihy),')']);
	case 14
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),numc(c),numc(lamd),numc(phih),numc(lamfill),numc(lamtype),numc(phihx),numc(phihy),numc(nstr),num(dwire),')']);
end



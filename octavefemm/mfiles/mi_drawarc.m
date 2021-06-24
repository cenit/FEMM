% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function mi_drawarc(p1,p2,p3,p4,p5,p6)
if (nargin==6)
	x1=p1; y1=p2; x2=p3; y2=p4; angle=p5; maxseg=p6;
elseif (nargin==4)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2); angle=p3; maxseg=p4;
elseif (nargin==3)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2); angle=p2; maxseg=p3;
end
	mi_addnode(x1,y1);
      mi_addnode(x2,y2);
      mi_addarc(x1,y1,x2,y2,angle,maxseg);


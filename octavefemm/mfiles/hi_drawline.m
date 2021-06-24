% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function hi_drawline(p1,p2,p3,p4)
if (nargin==4)
	x1=p1; y1=p2; x2=p3; y2=p4;
elseif (nargin==2)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2);
elseif (nargin==1)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2);
end
	hi_addnode(x1,y1);
	hi_addnode(x2,y2);
	hi_addsegment(x1,y1,x2,y2);


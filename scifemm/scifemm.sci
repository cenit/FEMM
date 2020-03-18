warning('off');
scifemm_path=get_absolute_file_path('scifemm.sci');

function openfemm(n)

    link(scifemm_path + "scilink.dll",["openfemm","call2femm","closefemm"],"c");
    call("openfemm","out",[80,1],1,"c");
    callfemm([ 'setcurrentdirectory(' , quote(pwd()) , ')' ]);
	if (argn(2)==1)
		if (n==0)
			bHide=0;
		else
			bHide=1;
		end
	else
		bHide=0;
	end
	if (bHide==0)
		callfemm('main_restore()');
	end
endfunction

function z=callfemm(x)
    u=call("call2femm",strcat(x),1,"c","out",[512,1],2,"c");
    z=0;
    if (part(u,1)~=' ')
        if (part(u,1)=='e')
			u=strcat(["FEMM returns: ",u]);
            error(u);
        else
            u=mtlb_strrep(u,'I','%i');
            z=evstr(u);
        end
    end
endfunction

function z=callfemm_noeval(x)
    u=call("call2femm",strcat(x),1,"c","out",[512,1],2,"c");
    z=0;
    if (part(u,1)~=' ')
        if (part(u,1)=='e')
			u=strcat(["FEMM returns: ",u]);
            error(u);
        end
    end
endfunction

function closefemm()
	call("closefemm","out",[80,1],1,"c");
	ulink();
endfunction

function opendocument(fn)
callfemm([ 'open(' , quote(fn) , ')' ]);
endfunction

function z=getlength(x)
    y=size(x);
	if (y(1)==1)
    	z=y(2);
	else
		z=y(1);
	end
endfunction

function s=complex2str(x)
    re=real(x);
    im=imag(x);
    if (im==0)
        s=sprintf('%.17g',re);
    elseif (im==1)
        if (re==0)
            s=sprintf('I');
        else
            s=sprintf('%.17g+I',re);
        end
    elseif (im==-1)
        if (re==0)
            s=sprintf('-I');
        else
            s=sprintf('%.17g-I',re);
        end
    elseif (im<0)
        if (re~=0)
            s=sprintf('%.17g-I*%.17g',re,abs(im));
        else
            s=sprintf('-I*%.17g',abs(im));
        end
    elseif (im>0)
        if (re~=0)
            s=sprintf('%.17g+I*%.17g',re,im);
        else
            s=sprintf('I*%.17g',im);
        end
    else
        s='';
    end
endfunction


function s=num(n)
    s=complex2str(n);
endfunction

function s=numc(n)
    s=[ complex2str(n), ',' ];
endfunction

function y=isstr(x)
	y=(type(x)==10)
endfunction

function s=quote(u)
    if (~isstr(u)) u=string(u); end
	v = strsubst(u,char([92,92]),char([92]));
	v = strsubst(v,char([92]),char([92,92]));
	s = char(34) + v + char(34);
endfunction

function s=quotec(u)
	s = quote(u) + ","
endfunction

function s=rawquote(v)
    s = char(34) + v + char(34);
endfunction

function newdocument(n)
	callfemm('create(' + num(n) + ')' );
endfunction 


function ci_addarc(x1,y1,x2,y2,angle,maxseg)
callfemm(['ci_addarc(' , numc(x1) , numc(y1) , numc(x2) , numc(y2) , numc(angle) , num(maxseg) , ')']);

endfunction

function ci_addblocklabel(x,y)
if (argn(2)==2)
	callfemm(['ci_addblocklabel(' , numc(x) , num(y) , ')' ]);
elseif (argn(2)==1)
	callfemm(['ci_addblocklabel(' , numc(x(1)) , num(x(2)) , ')' ]);
end

endfunction

function ci_addboundprop(pname,Vs,qs,c0,c1,fmt)
select argn(2)
	case 0 then
		callfemm('ci_addboundprop()');
	case 1 then
        callfemm(['ci_addboundprop(' , quote(pname) , ')' ]);
	case 2 then
        callfemm(['ci_addboundprop(' , quotec(pname) , num(Vs) , ')' ]);
	case 3 then
        callfemm(['ci_addboundprop(' , quotec(pname) , numc(Vs) , num(qs) , ')' ]);
	case 4 then
        callfemm(['ci_addboundprop(' , quotec(pname) , numc(Vs) , numc(qs) , num(c0) , ')' ]);
	case 5 then
        callfemm(['ci_addboundprop(' , quotec(pname) , numc(Vs) , numc(qs) , numc(c0) , num(c1) , ')' ]);
	else
        callfemm(['ci_addboundprop(' , quotec(pname) , numc(Vs) , numc(qs) , numc(c0) , numc(c1) , num(fmt), ')' ]);
end

endfunction

function ci_addconductorprop(pname,Tc,qc,ptype)
select argn(2)
	case 0 then
		callfemm('ci_addconductorprop()');
	case 1 then
		callfemm(['ci_addconductorprop(' , quote(pname) , ')' ]);
	case 2 then
		callfemm(['ci_addconductorprop(' , quotec(pname) , num(Tc) , ')' ]);
	case 3 then
		callfemm(['ci_addconductorprop(' , quotec(pname) , numc(Tc) , num(qc) , ')' ]);
	else
		callfemm(['ci_addconductorprop(' , quotec(pname) , numc(Tc) , numc(qc) , num(ptype) , ')' ]);
end

endfunction

function ci_addmaterial(pname,ox,oy,ex,ey,ltx,lty)
select argn(2)
	case 0 then 
		callfemm('ci_addmaterial()');
	case 1 then
		callfemm(['ci_addmaterial(' , quote(pname) ')' ]);
	case 2 then
		callfemm(['ci_addmaterial(' , quotec(pname) , num(ox) , ')' ]);
	case 3 then 
		callfemm(['ci_addmaterial(' , quotec(pname) , numc(ox) , num(oy) , ')' ]);
	case 4 then
		callfemm(['ci_addmaterial(' , quotec(pname) , numc(ox) , numc(oy) , num(ex) , ')' ]);
	case 5 then
		callfemm(['ci_addmaterial(' , quotec(pname) , numc(ox) , numc(oy) , numc(ex) , num(ey) , ')' ]);
	case 6 then
		callfemm(['ci_addmaterial(' , quotec(pname) , numc(ox) , numc(oy) , numc(ex) , numc(ey), num(ltx) , ')' ]);
	else
		callfemm(['ci_addmaterial(' , quotec(pname) , numc(ox) , numc(oy) , numc(ex) , numc(ey), numc(ltx) , num(lty) , ')' ]);
end

endfunction

function ci_addnode(x,y)
if (argn(2)==2)
    if (getlength(x)>1)
        if (getlength(y)~=getlength(x))
            error('invalid arguments to ci_addnode');
        end
        for k=1:getlength(x)
            callfemm(['ci_addnode(' , numc(x(k)) , num(y(k)) , ')']);
        end
    else callfemm(['ci_addnode(' , numc(x) , num(y) , ')']);
    end
elseif (argn(2)==1)
    callfemm(['ci_addnode(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function ci_addpointprop(pname,Tp,qp)
select argn(2)
	case 0 then
		callfemm('ci_addpointprop()')
	case 1 then
		callfemm(['ci_addpointprop(' , quote(pname) , ')']);
	case 2 then
		callfemm(['ci_addpointprop(' , quotec(pname) , num(Tp) , ')']);
	else
		callfemm(['ci_addpointprop(' , quotec(pname) , numc(Tp) , num(qp) , ')']);
end

endfunction

function ci_addsegment(x1,y1,x2,y2)
if (argn(2)==4)
	callfemm(['ci_addsegment(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);
elseif (argn(2)==2)
	callfemm(['ci_addsegment(' , numc(x1(1)) , numc(x1(2)) , numc(y1(1)) , num(y1(2)) , ')' ]);
elseif (argn(2)==1)
	callfemm(['ci_addsegment(' , numc(x1(1,1)) , numc(x1(1,2)) , numc(x1(2,1)) , num(x1(2,2)) , ')' ]);
end

endfunction

function ci_analyse(n)
if (argn(2)==1)
	callfemm(['ci_analyze(' , num(n) , ')' ]);
elseif (argn(2)==0)
	callfemm('ci_analyze()');
end

endfunction

function ci_analyze(n)
if (argn(2)==1)
	callfemm(['ci_analyze(' , num(n) , ')' ]);
elseif (argn(2)==0)
	callfemm('ci_analyze()');
end

endfunction

function ci_clearselected()
callfemm('ci_clearselected()');

endfunction

function ci_cleartkpoints(n)
callfemm(['ci_cleartkpoints(' , quote(n) , ')' ]);

endfunction

function ci_close()
callfemm('ci_close()');

endfunction

function ci_copyrotate(p1,p2,p3,p4)
if (argn(2)==4)
	bx=p1; by=p2; angle=p3; copies=p4;
elseif (argn(2)==3)
	bx=p1(1); by=p1(2); angle=p2; copies=p3;
end
callfemm(['ci_copyrotate(' , numc(bx) , numc(by) , numc(angle) , num(copies) , ')' ]);

endfunction

function ci_copyrotate2(p1,p2,p3,p4,p5)
if (argn(2)==5)
	bx=p1; by=p2; angle=p3; copies=p4; editaction=p5;
elseif (argn(2)==4)
	bx=p1(1); by=p1(2); angle=p2; copies=p3; editaction=p4;
end
callfemm(['ci_copyrotate(' , numc(bx) , numc(by) , numc(angle) , numc(copies) , num(editaction) , ')' ]);

endfunction

function ci_copytranslate(p1,p2,p3)
if (argn(2)==3)
	bx=p1; by=p2; copies=p3;
elseif (argn(2)==2)
	bx=p1(1); by=p1(2); copies=p2;
end
callfemm(['ci_copytranslate(' , numc(bx) , numc(by) , num(copies) , ')' ]);

endfunction

function ci_copytranslate2(p1,p2,p3,p4)
if (argn(2)==4)
	bx=p1; by=p2; copies=p3; editaction=p4;
elseif (argn(2)==3)
	bx=p1(1); by=p1(2); copies=p2; editaction=p3;
end
callfemm(['ci_copytranslate(' , numc(bx) , numc(by) , numc(copies) , num(editaction) , ')' ]);

endfunction

function z=ci_createmesh()
z=callfemm('ci_createmesh()');

endfunction

function ci_createradius(x,y,r)
callfemm(['ci_createradius(' , numc(x) , numc(y), num(r) , ')' ]);

endfunction

function ci_deleteboundprop(n)
callfemm(['ci_deleteboundprop(' , quote(n) , ')' ]);

endfunction

function ci_deleteconductor(n)
callfemm(['ci_deleteconductor(' , quote(n) , ')' ]);

endfunction

function ci_deletematerial(n)
callfemm(['ci_deletematerial(' , quote(n) , ')' ]);

endfunction

function ci_deletepointprop(n)
callfemm(['ci_deletepointprop(' , quote(n) , ')' ]);

endfunction

function ci_deleteselected()
callfemm('ci_deleteselected()');

endfunction

function ci_deleteselectedarcsegments()
callfemm('ci_deleteselectedarcsegments()');

endfunction

function ci_deleteselectedlabels()
callfemm('ci_deleteselectedlabels()');

endfunction

function ci_deleteselectednodes()
callfemm('ci_deleteselectednodes()');

endfunction

function ci_deleteselectedsegments()
callfemm('ci_deleteselectedsegments()');

endfunction

function ci_drawarc(p1,p2,p3,p4,p5,p6)
if (argn(2)==6)
	x1=p1; y1=p2; x2=p3; y2=p4; angle=p5; maxseg=p6;
elseif (argn(2)==4)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2); angle=p3; maxseg=p4;
elseif (argn(2)==3)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2); angle=p2; maxseg=p3;
end
	ci_addnode(x1,y1);
      ci_addnode(x2,y2);
      ci_addarc(x1,y1,x2,y2,angle,maxseg);

endfunction

function ci_drawline(p1,p2,p3,p4)
if (argn(2)==4)
	x1=p1; y1=p2; x2=p3; y2=p4;
elseif (argn(2)==2)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2);
elseif (argn(2)==1)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2);
end
	ci_addnode(x1,y1);
	ci_addnode(x2,y2);
	ci_addsegment(x1,y1,x2,y2);

endfunction

function ci_drawpolygon(p)
	ci_drawpolyline(p);
	n=getlength(p);
	ci_drawline(p(1,1),p(1,2),p(n,1),p(n,2));

endfunction

function ci_drawpolyline(p)
	n=getlength(p);
	for k = 1:(n-1)
		ci_drawline(p(k,1),p(k,2),p(k+1,1),p(k+1,2));
	end

endfunction

function ci_drawrectangle(p1,p2,p3,p4)
if (argn(2)==4)
	x1=p1; y1=p2; x2=p3; y2=p4;
elseif (argn(2)==2)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2);
elseif (argn(2)==1)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2);
end
ci_drawpolygon([x1,y1;x1,y2;x2,y2;x2,y1]);

endfunction

function z=ci_getmaterial(matname)
callfemm(['ci_getmaterial(' , quote(matname) , ')' ]);


endfunction

function vu=ci_getview()
    fname=tempname;
    callfemm(['ci_savebitmap(', quote(fname), ')']);
    vu=imread(fname,'bmp');
    system(['del ',rawquote(fname)]);

endfunction

function ci_hidegrid()
callfemm('ci_hidegrid()');

endfunction

function ci_hidenames()
callfemm('ci_shownames(0)');

endfunction

function ci_loadsolution()
callfemm('ci_loadsolution()');

endfunction

function ci_maximize()
	callfemm('ci_maximize()');

endfunction

function ci_minimize()
	callfemm('ci_minimize()');

endfunction

function ci_mirror(p1,p2,p3,p4)
if (argn(2)==4)
	x1=p1; y1=p2; x2=p3; y2=p4;
elseif (argn(2)==2)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2); 
elseif (argn(2)==1)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2);
end
callfemm(['ci_mirror(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);

endfunction

function ci_mirror2(p1,p2,p3,p4,p5)
if (argn(2)==5)
	x1=p1; y1=p2; x2=p3; y2=p4; editaction=p5;
elseif (argn(2)==3)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2); editaction=p3;
elseif (argn(2)==2)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2); editaction=p2;
end
callfemm(['ci_mirror(' , numc(x1) , numc(y1) , numc(x2) , numc(y2) , num(editaction) , ')' ]);

endfunction

function ci_modifyboundprop(pname,pnum,pval)
        if (pnum==0)
                callfemm(['ci_modifyboundprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
        else
                callfemm(['ci_modifyboundprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

endfunction

function ci_modifyconductorprop(pname,pnum,pval)
        if (pnum==0)
                callfemm(['ci_modifyconductorprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
	  else
                callfemm(['ci_modifyconductorprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

endfunction

function ci_modifymaterial(pname,pnum,pval)
        if (pnum==0)
                callfemm(['ci_modifymaterial(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
	  else
                callfemm(['ci_modifymaterial(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

endfunction

function ci_modifypointprop(pname,pnum,pval)
        if (pnum==0)
                callfemm(['ci_modifypointprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
        else
                callfemm(['ci_modifypointprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

endfunction

function ci_moverotate(p1,p2,p3)
if (argn(2)==3)
	bx=p1; by=p2; shiftangle=p3;
elseif (argn(2)==2)
	bx=p1(1); by=p1(2); shiftangle=p2;
end
callfemm(['ci_moverotate(' , numc(bx) , numc(by) , num(shiftangle) , ')' ]);

endfunction

function ci_movetranslate(bx,by)
if (argn(2)==2)
	callfemm(['ci_movetranslate(' , numc(bx) , num(by) ')' ]);
elseif (argn(2)==1)
	callfemm(['ci_movetranslate(' , numc(bx(1)) , num(bx(2)) ')' ]);
end

endfunction

function ci_movetranslate2(p1,p2,p3)
if (argn(2)==3)
	bx=p1; by=p2; editaction=p3;
elseif (argn(2)==2)
	bx=p1(1); by=p1(2); editaction=p2;
end
callfemm(['ci_movetranslate(' , numc(bx) , numc(by) , num(editaction) , ')' ]);

endfunction

function ci_probdef(units, ptype, frequency, precision, depth, minangle)
callfemm(['ci_probdef(' , quotec(units) , quotec(ptype) , numc(frequency) , numc(precision) , numc(depth) , num(minangle) , ')']);

endfunction

function ci_purgemesh()
callfemm('ci_purgemesh()');

endfunction

function ci_readdxf(docname)
callfemm(['ci_readdxf(' , quote(docname) , ')' ]);

endfunction

function ci_refreshview()
callfemm('ci_refreshview()');

endfunction

function ci_resize(nWidth,nHeight)
	callfemm(['ci_resize(', numc(nWidth), num(nHeight), ')' ]);

endfunction

function ci_restore()
	callfemm('ci_restore()');

endfunction

function ci_saveas(fn)
callfemm(['ci_saveas(' , quote(fn) , ')']);

endfunction

function ci_savebitmap(n)
callfemm(['ci_savebitmap(' , quote(n) , ')' ]);

endfunction

function ci_savedxf(docname)
callfemm_noeval(['ci_savedxf(' , quote(docname) , ')' ]);

endfunction

function ci_savemetafile(n)
callfemm(['ci_savemetafile(' , quote(n) , ')' ]);

endfunction

function ci_scale(p1,p2,p3)
if (argn(2)==3)
	bx=p1; by=p2; sc=p3;
elseif (argn(2)==2)
	bx=p1(1); by=p1(2); sc=p2;
end
callfemm(['ci_scale(' , numc(bx) , numc(by) , numc(sc) , ')' ]);

endfunction

function ci_scale2(p1,p2,p3,p4)
if (argn(2)==4)
	bx=p1; by=p2; sc=p3; ea=p4;
elseif (argn(2)==3)
	bx=p1(1); by=p1(2); sc=p2; ea=p3;
end
callfemm(['ci_scale(' , numc(bx) , numc(by) , numc(sc) , num(ea) , ')' ]);

endfunction

function z=ci_selectarcsegment(x,y)
if (argn(2)==2)
	z=callfemm(['ci_selectarcsegment(' , numc(x) , num(y) , ')']);
elseif (argn(2)==1)
	z=callfemm(['ci_selectarcsegment(' , numc(x(1)) , num(x(2)) , ')']);
end
 
endfunction

function ci_selectgroup(gr)
callfemm(['ci_selectgroup(' , num(gr) , ')' ]);

endfunction

function z=ci_selectlabel(x,y)
if (argn(2)==2)
	z=callfemm(['ci_selectlabel(' , numc(x) , num(y) , ')']);
elseif (argn(2)==1)
	z=callfemm(['ci_selectlabel(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function z=ci_selectnode(x,y) 
if (argn(2)==2)
	z=callfemm(['ci_selectnode(' , numc(x) , num(y) , ')']);
elseif (argn(2)==1)
	z=callfemm(['ci_selectnode(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function z=ci_selectsegment(x,y)
if (argn(2)==2)
	z=callfemm(['ci_selectsegment(' , numc(x) , num(y) , ')']);
elseif (argn(2)==1)
	z=callfemm(['ci_selectsegment(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function ci_setarcsegmentprop(maxsegdeg,propname,hide,group,incond)
callfemm(['ci_setarcsegmentprop(' , numc(maxsegdeg) , quotec(propname) , numc(hide) , numc(group) , quote(incond) , ')' ]);

endfunction

function ci_setblockprop(blockname,automesh,meshsize,group)
callfemm(['ci_setblockprop(' , quotec(blockname) , numc(automesh) , numc(meshsize) , num(group) , ')']);

endfunction

function ci_seteditmode(editmode)
callfemm(['ci_seteditmode(' , quote(editmode) , ')' ]);

endfunction

function ci_setfocus(docname)
callfemm(['ci_setfocus(' , quote(docname) , ')' ]);

endfunction

function ci_setgrid(density,ptype)
callfemm(['ci_setgrid(' , numc(density) , quote(ptype) , ')' ]);

endfunction

function z=ci_setgroup(n)
z=callfemm(['ci_setgroup(' , num(n) , ')' ]);



endfunction

function ci_setnodeprop(nodeprop,groupno,inconductor)
callfemm(['ci_setnodeprop(' , quotec(nodeprop) , numc(groupno) , quote(inconductor) , ')']);

endfunction

function ci_setsegmentprop(pn,es,am,hi,gr, inconductor)
callfemm(['ci_setsegmentprop(' , quotec(pn) , numc(es) , numc(am) , numc(hi) , numc(gr) , quote(inconductor) , ')']);

endfunction

function ci_showgrid()
callfemm('ci_showgrid()');

endfunction

function ci_showmesh()
callfemm('ci_showmesh()');

endfunction

function ci_shownames()
callfemm('ci_shownames(1)');

endfunction

function ci_snapgridoff()
callfemm('ci_gridsnap(""off"")');

endfunction

function ci_snapgridon()
callfemm('ci_gridsnap(""on"")');

endfunction

function ci_zoom(x1,y1,x2,y2)
if (argn(2)==4)
	callfemm(['ci_zoom(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);
elseif (argn(2)==2)
	callfemm(['ci_zoom(' , numc(x1(1)) , numc(x1(2)) , numc(y1(1)) , num(y1(2)) , ')' ]);
elseif (argn(2)==1)
	callfemm(['ci_zoom(' , numc(x1(1,1)) , numc(x1(1,2)) , numc(x1(2,1)) , num(x1(2,2)) , ')' ]);
end

endfunction

function ci_zoomin()
callfemm('ci_zoomin()');

endfunction

function ci_zoomnatural()
callfemm('ci_zoomnatural()');

endfunction

function ci_zoomout()
callfemm('ci_zoomout()');

endfunction

function co_addcontour(x,y)
if (argn(2)==2)
	callfemm(['co_addcontour(' , numc(x) , num(y) , ')' ]);
elseif (argn(2)==1)
	callfemm(['co_addcontour(' , numc(x(1)) , num(x(2)) , ')' ]);
end

endfunction

function co_bendcontour(tta,dtta)
callfemm(['co_bendcontour(' , numc(tta) , num(dtta) , ')' ]);

endfunction

function z=co_blockintegral(ptype)
z=callfemm(['co_blockintegral(' , num(ptype) , ')' ]);
        
endfunction

function co_clearblock()
callfemm('co_clearblock()');

endfunction

function co_clearcontour()
callfemm('co_clearcontour()');

endfunction

function co_close()
callfemm('co_close()');

endfunction

function z=co_getconductorproperties(pname)
z=callfemm(['co_getconductorproperties(' , quote(pname) , ')' ]);

endfunction

function z=co_gete(x,y)
if (argn(2)==2)
	z=co_getpointvalues(x,y); z=z(:,6:7);
elseif (argn(2)==1)
	z=co_getpointvalues(x); z=z(:,6:7);
end

endfunction

function z=co_getelement(n)
z=callfemm(['co_getelement(' , num(n) , ')' ]);


endfunction

function z=co_getj(x,y)
if (argn(2)==2)
	z=co_getpointvalues(x,y); z=z(:,2:3);
elseif (argn(2)==1)
	z=co_getpointvalues(x); z=z(:,2:3);
end

endfunction

function z=co_getk(x,y)
if (argn(2)==2)
	z=co_getpointvalues(x,y); z=z(:,4:5);
elseif (argn(2)==1)
	z=co_getpointvalues(x); z=z(:,4:5);
end

endfunction

function z=co_getnode(n)
z=callfemm(['co_getnode(' , num(n) , ')' ]);

endfunction

function z=co_getpointvalues(x,y)
if (argn(2)==2)
	n=getlength(x);
	if (getlength(y)~=n)
		error('must be same number of entries in X and Y arrays');
	end
	z=zeros(n,15);
	for k=1:n
		z(k,:)=callfemm(['co_getpointvalues(' , numc(x(k)) , num(y(k)) , ')' ]);
	end
elseif (argn(2)==1)
	n=size(x); n=n(1);
	z=zeros(n,15);
	for k=1:n
		z(k,:)=callfemm(['co_getpointvalues(' , numc(x(k,1)) , num(x(k,2)) , ')' ]);
	end
end

endfunction

function z=co_getprobleminfo()
z=callfemm('co_getprobleminfo()');

endfunction

function z=co_getv(x,y)
if (argn(2)==2)
	z=co_getpointvalues(x,y); z=z(:,1);
elseif (argn(2)==1)
	z=co_getpointvalues(x); z=z(:,1);
end

endfunction

function vu=co_getview()
    fname=tempname;
    callfemm(['co_savebitmap(', quote(fname), ')']);
    vu=imread(fname,'bmp');
    system(['del ',rawquote(fname)]);

endfunction

function co_groupselectblock(n)
if(argn(2)==1)
callfemm(['co_groupselectblock(' , num(n) , ')' ]);
else
callfemm('co_groupselectblock()');
end

endfunction

function co_hidecontourplot()
callfemm('co_hidecontourplot()');

endfunction

function co_hidedensityplot()
callfemm('co_hidedensityplot()');

endfunction

function co_hidegrid()
callfemm('co_hidegrid()');

endfunction

function co_hidemesh()
callfemm('co_hidemesh()');

endfunction

function co_hidenames()
callfemm('co_shownames(0)');

endfunction

function co_hidepoints()
callfemm('co_hidepoints()');

endfunction

function z=co_lineintegral(ptype)
z=callfemm(['co_lineintegral(' , num(ptype) , ')' ]);

endfunction

function co_makeplot(plottype,numpoints,filename,fileformat)
if (argn(2)==4)
callfemm(['co_makeplot(' , numc(plottype) , numc(numpoints) , quotec(filename) , num(fileformat) , ')' ]);
end
if (argn(2)==3)
callfemm(['co_makeplot(' , numc(plottype) , numc(numpoints) , quote(filename) , ')' ]);
end
if (argn(2)==2)
callfemm(['co_makeplot(' , numc(plottype) , num(numpoints) , ')' ]);
end

endfunction

function co_maximize()
	callfemm('co_maximize()');

endfunction

function co_minimize()
	callfemm('co_minimize()');

endfunction

function z=co_numelements()
z=callfemm('co_numelements()');

endfunction

function z=co_numnodes()
z=callfemm('co_numnodes()');

endfunction

function co_refreshview()
callfemm('co_refreshview()');

endfunction

function co_reload()
callfemm('co_reload()');

endfunction

function co_resize(nWidth,nHeight)
	callfemm(['co_resize(', numc(nWidth), num(nHeight), ')' ]);

endfunction

function co_restore()
	callfemm('co_restore()');

endfunction

function co_savebitmap(fn)
callfemm(['co_savebitmap(' , quote(fn) , ')' ]);

endfunction

function co_savemetafile(fn)
callfemm(['co_savemetafile(' , quote(fn) , ')' ]);

endfunction

function co_selectblock(x,y)
if (argn(2)==2)
	callfemm(['co_selectblock(' , numc(x) , num(y) , ')' ]);
elseif (argn(2)==1)
	callfemm(['co_selectblock(' , numc(x(1)) , num(x(2)) , ')' ]);
end

endfunction

function co_selectpoint(x,y)
if (argn(2)==2)
	callfemm(['co_selectpoint(' , numc(x) , num(y) , ')' ]);
elseif (argn(2)==1)
	callfemm(['co_selectpoint(' , numc(x(1)) , num(x(2)) , ')' ]);
end

endfunction

function co_seteditmode(mode)
callfemm(['co_seteditmode(' , quote(mode) , ')' ]);

endfunction

function co_setgrid(density,ptype)
callfemm(['co_setgrid(' , numc(density) , quote(ptype) , ')' ]);

endfunction

function co_showcontourplot(numcontours,al,au)
callfemm(['co_showcontourplot(' , numc(numcontours) , numc(al) , num(au) , ')' ]);

endfunction

function co_showdensityplot(legend,gscale,ptype,bu,bl)
callfemm(['co_showdensityplot(' , numc(legend) , numc(gscale) , numc(ptype) , numc(bu) , num(bl) , ')' ]);

endfunction

function co_showgrid()
callfemm('co_showgrid()');

endfunction

function co_showmesh()
callfemm('co_showmesh()');

endfunction

function co_shownames()
callfemm('co_shownames(1)');

endfunction

function co_showpoints()
callfemm('co_showpoints()');

endfunction

function co_showvectorplot(plottype,scalefactor)
if (argn(2)==2)
	callfemm(['co_showvectorplot(' , numc(plottype) , num(scalefactor) , ')' ]);
elseif (argn(2)==1)
	callfemm(['co_showvectorplot(' , num(plottype) , ')' ]);
end

endfunction

function co_smooth(flag)
callfemm(['co_smooth(' , quote(flag) , ')' ]);

endfunction

function co_smoothoff()
callfemm('co_smooth(""on"")');

endfunction

function co_smoothon()
callfemm('co_smooth(""on"")');

endfunction

function co_snapgrid(flag)
callfemm(['co_gridsnap(' , quote(flag) , ')' ]);

endfunction

function co_snapgridoff()
callfemm('co_gridsnap(""off"")');

endfunction

function co_snapgridon()
callfemm('co_gridsnap(""on"")');

endfunction

function co_zoom(x1,y1,x2,y2)
if (argn(2)==4)
	callfemm(['co_zoom(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);
elseif (argn(2)==2)
	callfemm(['co_zoom(' , numc(x1(1)) , numc(x1(2)) , numc(y1(1)) , num(y1(2)) , ')' ]);
elseif (argn(2)==1)
	callfemm(['co_zoom(' , numc(x1(1,1)) , numc(x1(1,2)) , numc(x1(2,1)) , num(x1(2,2)) , ')' ]);
end

endfunction

function co_zoomin()
callfemm('co_zoomin()');

endfunction

function co_zoomnatural()
callfemm('co_zoomnatural()');

endfunction

function co_zoomout()
callfemm('co_zoomout()');

endfunction

function ei_addarc(x1,y1,x2,y2,angle,maxseg)
callfemm(['ei_addarc(' , numc(x1) , numc(y1) , numc(x2) , numc(y2) , numc(angle) , num(maxseg) , ')']);

endfunction

function ei_addblocklabel(x,y)
if (argn(2)==2)
	callfemm(['ei_addblocklabel(' , numc(x) , num(y) , ')' ]);
elseif (argn(2)==1)
	callfemm(['ei_addblocklabel(' , numc(x(1)) , num(x(2)) , ')' ]);
end

endfunction

function ei_addboundprop(pname,vs,qs,c0,c1,fmt)
        callfemm(['ei_addboundprop(' , quotec(pname) , numc(vs) , numc(qs) , numc(c0) , numc(c1) , num(fmt) , ')' ]);

endfunction

function ei_addconductorprop(pname,vc,qc,ptype)
callfemm(['ei_addconductorprop(' , quotec(pname) , numc(vc) , numc(qc) , num(ptype) , ')' ]);

endfunction

function ei_addmaterial(pname,ex,ey,qv)
callfemm(['ei_addmaterial(' , quotec(pname) , numc(ex) , numc(ey) , num(qv) , ')' ]);

endfunction

function ei_addnode(x,y)
if (argn(2)==2)
    if (getlength(x)>1)
        if (getlength(y)~=getlength(x))
            error('invalid arguments to ei_addnode');
        end
        for k=1:getlength(x)
            callfemm(['ei_addnode(' , numc(x(k)) , num(y(k)) , ')']);
        end
    else callfemm(['ei_addnode(' , numc(x) , num(y) , ')']);
    end
elseif (argn(2)==1)
    callfemm(['ei_addnode(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function ei_addpointprop(pname,vp,qp)
callfemm(['ei_addpointprop(' , quotec(pname) , numc(vp) , num(qp) , ')']);

endfunction

function ei_addsegment(x1,y1,x2,y2)
if (argn(2)==4)
	callfemm(['ei_addsegment(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);
elseif (argn(2)==2)
	callfemm(['ei_addsegment(' , numc(x1(1)) , numc(x1(2)) , numc(y1(1)) , num(y1(2)) , ')' ]);
elseif (argn(2)==1)
	callfemm(['ei_addsegment(' , numc(x1(1,1)) , numc(x1(1,2)) , numc(x1(2,1)) , num(x1(2,2)) , ')' ]);
end

endfunction

function ei_analyse(n)
if (argn(2)==1)
	callfemm(['ei_analyze(' , num(n) , ')' ]);
elseif (argn(2)==0)
	callfemm('ei_analyze()');
end

endfunction

function ei_analyze(n)
if (argn(2)==1)
	callfemm(['ei_analyze(' , num(n) , ')' ]);
elseif (argn(2)==0)
	callfemm('ei_analyze()');
end

endfunction

function ei_attachouterspace()
callfemm('ei_attachouterspace()');

endfunction

function ei_clearselected()
callfemm('ei_clearselected()');

endfunction

function ei_close()
callfemm('ei_close()');

endfunction

function ei_copyrotate(p1,p2,p3,p4)
if (argn(2)==4)
	bx=p1; by=p2; angle=p3; copies=p4;
elseif (argn(2)==3)
	bx=p1(1); by=p1(2); angle=p2; copies=p3;
end
callfemm(['ei_copyrotate(' , numc(bx) , numc(by) , numc(angle) , num(copies) , ')' ]);

endfunction

function ei_copyrotate2(p1,p2,p3,p4,p5)
if (argn(2)==5)
	bx=p1; by=p2; angle=p3; copies=p4; editaction=p5;
elseif (argn(2)==4)
	bx=p1(1); by=p1(2); angle=p2; copies=p3; editaction=p4;
end
callfemm(['ei_copyrotate(' , numc(bx) , numc(by) , numc(angle) , numc(copies) , num(editaction) , ')' ]);

endfunction

function ei_copytranslate(p1,p2,p3)
if (argn(2)==3)
	bx=p1; by=p2; copies=p3;
elseif (argn(2)==2)
	bx=p1(1); by=p1(2); copies=p2;
end
callfemm(['ei_copytranslate(' , numc(bx) , numc(by) , num(copies) , ')' ]);

endfunction

function ei_copytranslate2(p1,p2,p3,p4)
if (argn(2)==4)
	bx=p1; by=p2; copies=p3; editaction=p4;
elseif (argn(2)==3)
	bx=p1(1); by=p1(2); copies=p2; editaction=p3;
end
callfemm(['ei_copytranslate(' , numc(bx) , numc(by) , numc(copies) , num(editaction) , ')' ]);

endfunction

function z=ei_createmesh()
z=callfemm('ei_createmesh()');

endfunction

function ei_createradius(x,y,r)
callfemm(['ei_createradius(' , numc(x) , numc(y), num(r) , ')' ]);

endfunction

function ei_defineouterspace(Zo,Ro,Ri)
callfemm(['ei_defineouterspace(' , numc(Zo) , numc(Ro) , num(Ri) , ')' ]);

endfunction

function ei_deleteboundprop(n)
callfemm(['ei_deleteboundprop(' , quote(n) , ')' ]);

endfunction

function ei_deleteconductor(n)
callfemm(['ei_deleteconductor(' , quote(n) , ')' ]);

endfunction

function ei_deletematerial(n)
callfemm(['ei_deletematerial(' , quote(n) , ')' ]);

endfunction

function ei_deletepointprop(n)
callfemm(['ei_deletepointprop(' , quote(n) , ')' ]);

endfunction

function ei_deleteselected()
callfemm('ei_deleteselected()');

endfunction

function ei_deleteselectedarcsegments()
callfemm('ei_deleteselectedarcsegments()');

endfunction

function ei_deleteselectedlabels()
callfemm('ei_deleteselectedlabels()');

endfunction

function ei_deleteselectednodes()
callfemm('ei_deleteselectednodes()');

endfunction

function ei_deleteselectedsegments()
callfemm('ei_deleteselectedsegments()');

endfunction

function ei_detachouterspace()
callfemm('ei_detachouterspace()');

endfunction

function ei_drawarc(p1,p2,p3,p4,p5,p6)
if (argn(2)==6)
	x1=p1; y1=p2; x2=p3; y2=p4; angle=p5; maxseg=p6;
elseif (argn(2)==4)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2); angle=p3; maxseg=p4;
elseif (argn(2)==3)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2); angle=p2; maxseg=p3;
end
	ei_addnode(x1,y1);
      ei_addnode(x2,y2);
      ei_addarc(x1,y1,x2,y2,angle,maxseg);

endfunction

function ei_drawline(p1,p2,p3,p4)
if (argn(2)==4)
	x1=p1; y1=p2; x2=p3; y2=p4;
elseif (argn(2)==2)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2);
elseif (argn(2)==1)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2);
end
	ei_addnode(x1,y1);
	ei_addnode(x2,y2);
	ei_addsegment(x1,y1,x2,y2);

endfunction

function ei_drawpolygon(p)
	ei_drawpolyline(p);
	n=getlength(p);
	ei_drawline(p(1,1),p(1,2),p(n,1),p(n,2));

endfunction

function ei_drawpolyline(p)
	n=getlength(p);
	for k = 1:(n-1)
		ei_drawline(p(k,1),p(k,2),p(k+1,1),p(k+1,2));
	end

endfunction

function ei_drawrectangle(p1,p2,p3,p4)
if (argn(2)==4)
	x1=p1; y1=p2; x2=p3; y2=p4;
elseif (argn(2)==2)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2);
elseif (argn(2)==1)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2);
end
ei_drawpolygon([x1,y1;x1,y2;x2,y2;x2,y1]);

endfunction

function z=ei_getmaterial(matname)
callfemm(['ei_getmaterial(' , quote(matname) , ')' ]);

endfunction

function ei_hidegrid()
callfemm('ei_hidegrid()');

endfunction

function ei_hidenames()
callfemm('ei_shownames(0)');

endfunction

function ei_loadsolution()
callfemm('ei_loadsolution()');

endfunction

function ei_maximize()
	callfemm('ei_maximize()');

endfunction

function ei_minimize()
	callfemm('ei_minimize()');

endfunction

function ei_mirror(p1,p2,p3,p4)
if (argn(2)==4)
	x1=p1; y1=p2; x2=p3; y2=p4;
elseif (argn(2)==2)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2); 
elseif (argn(2)==1)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2);
end
callfemm(['ei_mirror(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);

endfunction

function ei_mirror2(p1,p2,p3,p4,p5)
if (argn(2)==5)
	x1=p1; y1=p2; x2=p3; y2=p4; editaction=p5;
elseif (argn(2)==3)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2); editaction=p3;
elseif (argn(2)==2)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2); editaction=p2;
end
callfemm(['ei_mirror(' , numc(x1) , numc(y1) , numc(x2) , numc(y2) , num(editaction) , ')' ]);

endfunction

function ei_modifyboundprop(pname,pnum,pval)
        if (pnum==0)
                callfemm(['ei_modifyboundprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
        else
                callfemm(['ei_modifyboundprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

endfunction

function ei_modifyconductorprop(pname,pnum,pval)
        if (pnum==0)
                callfemm(['ei_modifyconductorprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
	  else
                callfemm(['ei_modifyconductorprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

endfunction

function ei_modifymaterial(pname,pnum,pval)
        if (pnum==0)
                callfemm(['ei_modifymaterial(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
	  else
                callfemm(['ei_modifymaterial(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

endfunction

function ei_modifypointprop(pname,pnum,pval)
        if (pnum==0)
                callfemm(['ei_modifypointprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
        else
                callfemm(['ei_modifypointprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

endfunction

function ei_moverotate(p1,p2,p3)
if (argn(2)==3)
	bx=p1; by=p2; shiftangle=p3;
elseif (argn(2)==2)
	bx=p1(1); by=p1(2); shiftangle=p2;
end
callfemm(['ei_moverotate(' , numc(bx) , numc(by) , num(shiftangle) , ')' ]);

endfunction

function ei_movetranslate(bx,by)
if (argn(2)==2)
	callfemm(['ei_movetranslate(' , numc(bx) , num(by) ')' ]);
elseif (argn(2)==1)
	callfemm(['ei_movetranslate(' , numc(bx(1)) , num(bx(2)) ')' ]);
end

endfunction

function ei_movetranslate2(p1,p2,p3)
if (argn(2)==3)
	bx=p1; by=p2; editaction=p3;
elseif (argn(2)==2)
	bx=p1(1); by=p1(2); editaction=p2;
end
callfemm(['ei_movetranslate(' , numc(bx) , numc(by) , num(editaction) , ')' ]);

endfunction

function ei_probdef(units, ptype, precision, depth, minangle)
callfemm(['ei_probdef(' , quotec(units) , quotec(ptype) , numc(precision) , numc(depth) , num(minangle) , ')']);

endfunction

function ei_purgemesh()
callfemm('ei_purgemesh()');

endfunction

function ei_readdxf(docname)
callfemm(['ei_readdxf(' , quote(docname) , ')' ]);

endfunction

function ei_refreshview()
callfemm('ei_refreshview()');

endfunction

function ei_resize(nWidth,nHeight)
	callfemm(['ei_resize(', numc(nWidth), num(nHeight), ')' ]);

endfunction

function ei_restore()
	callfemm('ei_restore()');

endfunction

function ei_saveas(fn)
callfemm(['ei_saveas(' , quote(fn) , ')']);

endfunction

function ei_savebitmap(n)
callfemm(['ei_savebitmap(' , quote(n) , ')' ]);

endfunction

function ei_savedxf(docname)
callfemm_noeval(['ei_savedxf(' , quote(docname) , ')' ]);

endfunction

function ei_savemetafile(n)
callfemm(['ei_savemetafile(' , quote(n) , ')' ]);

endfunction

function ei_scale(p1,p2,p3)
if (argn(2)==3)
	bx=p1; by=p2; sc=p3;
elseif (argn(2)==2)
	bx=p1(1); by=p1(2); sc=p2;
end
callfemm(['ei_scale(' , numc(bx) , numc(by) , numc(sc) , ')' ]);

endfunction

function ei_scale2(p1,p2,p3,p4)
if (argn(2)==4)
	bx=p1; by=p2; sc=p3; ea=p4;
elseif (argn(2)==3)
	bx=p1(1); by=p1(2); sc=p2; ea=p3;
end
callfemm(['ei_scale(' , numc(bx) , numc(by) , numc(sc) , num(ea) , ')' ]);

endfunction

function z=ei_selectarcsegment(x,y)
if (argn(2)==2)
	z=callfemm(['ei_selectarcsegment(' , numc(x) , num(y) , ')']);
elseif (argn(2)==1)
	z=callfemm(['ei_selectarcsegment(' , numc(x(1)) , num(x(2)) , ')']);
end
 
endfunction

function ei_selectgroup(gr)
callfemm(['ei_selectgroup(' , num(gr) , ')' ]);

endfunction

function z=ei_selectlabel(x,y)
if (argn(2)==2)
	z=callfemm(['ei_selectlabel(' , numc(x) , num(y) , ')']);
elseif (argn(2)==1)
	z=callfemm(['ei_selectlabel(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function z=ei_selectnode(x,y) 
if (argn(2)==2)
	z=callfemm(['ei_selectnode(' , numc(x) , num(y) , ')']);
elseif (argn(2)==1)
	z=callfemm(['ei_selectnode(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function z=ei_selectsegment(x,y)
if (argn(2)==2)
	z=callfemm(['ei_selectsegment(' , numc(x) , num(y) , ')']);
elseif (argn(2)==1)
	z=callfemm(['ei_selectsegment(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function ei_setarcsegmentprop(maxsegdeg,propname,hide,group,incond)
callfemm(['ei_setarcsegmentprop(' , numc(maxsegdeg) , quotec(propname) , numc(hide) , numc(group) , quote(incond) , ')' ]);

endfunction

function ei_setblockprop(blockname,automesh,meshsize,group)
callfemm(['ei_setblockprop(' , quotec(blockname) , numc(automesh) , numc(meshsize) , num(group) , ')']);

endfunction

function ei_seteditmode(editmode)
callfemm(['ei_seteditmode(' , quote(editmode) , ')' ]);

endfunction

function ei_setfocus(docname)
callfemm(['ei_setfocus(' , quote(docname) , ')' ]);

endfunction

function ei_setgrid(density,ptype)
callfemm(['ei_setgrid(' , numc(density) , quote(ptype) , ')' ]);

endfunction

function z=ei_setgroup(n)
z=callfemm(['ei_setgroup(' , num(n) , ')' ]);

endfunction

function ei_setnodeprop(nodeprop,groupno,inconductor)
callfemm(['ei_setnodeprop(' , quotec(nodeprop) , numc(groupno) , quote(inconductor) , ')']);

endfunction

function ei_setsegmentprop(pn,es,am,hi,gr, inconductor)
callfemm(['ei_setsegmentprop(' , quotec(pn) , numc(es) , numc(am) , numc(hi) , numc(gr) , quote(inconductor) , ')']);

endfunction

function ei_showgrid()
callfemm('ei_showgrid()');

endfunction

function ei_showmesh()
callfemm('ei_showmesh()');

endfunction

function ei_shownames()
callfemm('ei_shownames(1)');

endfunction

function ei_snapgridoff()
callfemm('ei_gridsnap(""off"")');

endfunction

function ei_snapgridon()
callfemm('ei_gridsnap(""on"")');

endfunction

function ei_zoom(x1,y1,x2,y2)
if (argn(2)==4)
	callfemm(['ei_zoom(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);
elseif (argn(2)==2)
	callfemm(['ei_zoom(' , numc(x1(1)) , numc(x1(2)) , numc(y1(1)) , num(y1(2)) , ')' ]);
elseif (argn(2)==1)
	callfemm(['ei_zoom(' , numc(x1(1,1)) , numc(x1(1,2)) , numc(x1(2,1)) , num(x1(2,2)) , ')' ]);
end

endfunction

function ei_zoomin()
callfemm('ei_zoomin()');

endfunction

function ei_zoomnatural()
callfemm('ei_zoomnatural()');

endfunction

function ei_zoomout()
callfemm('ei_zoomout()');

endfunction

function eo_addcontour(x,y)
if (argn(2)==2)
	callfemm(['eo_addcontour(' , numc(x) , num(y) , ')' ]);
elseif (argn(2)==1)
	callfemm(['eo_addcontour(' , numc(x(1)) , num(x(2)) , ')' ]);
end

endfunction

function eo_bendcontour(tta,dtta)
callfemm(['eo_bendcontour(' , numc(tta) , num(dtta) , ')' ]);

endfunction

function z=eo_blockintegral(ptype)
z=callfemm(['eo_blockintegral(' , num(ptype) , ')' ]);
        
endfunction

function eo_clearblock()
callfemm('eo_clearblock()');

endfunction

function eo_clearcontour()
callfemm('eo_clearcontour()');

endfunction

function eo_close()
callfemm('eo_close()');

endfunction

function z=eo_getconductorproperties(pname)
z=callfemm(['eo_getconductorproperties(' , quote(pname) , ')' ]);

endfunction

function z=eo_getd(x,y)
if (argn(2)==2)
	z=eo_getpointvalues(x,y); z=z(:,2:3);
elseif (argn(2)==1)
	z=eo_getpointvalues(x); z=z(:,2:3);
end

endfunction

function z=eo_gete(x,y)
if (argn(2)==2)
	z=eo_getpointvalues(x,y); z=z(:,4:5);
elseif (argn(2)==1)
	z=eo_getpointvalues(x); z=z(:,4:5);
end

endfunction

function z=eo_getelement(n)
z=callfemm(['eo_getelement(' , num(n) , ')' ]);

endfunction

function z=eo_getenergydensity(x,y)
if (argn(2)==2)
	z=eo_getpointvalues(x,y); z=z(:,8);
elseif (argn(2)==1)
	z=eo_getpointvalues(x); z=z(:,8);
end

endfunction

function z=eo_getnode(n)
z=callfemm(['eo_getnode(' , num(n) , ')' ]);

endfunction

function z=eo_getperm(x,y)
if (argn(2)==2)
	z=eo_getpointvalues(x,y); z=z(:,6:7);
elseif (argn(2)==1)
	z=eo_getpointvalues(x); z=z(:,6:7);
end

endfunction

function z=eo_getpointvalues(x,y)
if (argn(2)==2)
	n=getlength(x);
	if (getlength(y)~=n)
		error('must be same number of entries in X and Y arrays');
	end
	z=zeros(n,8);
	for k=1:n
		z(k,:)=callfemm(['eo_getpointvalues(' , numc(x(k)) , num(y(k)) , ')' ]);
	end
elseif (argn(2)==1)
	n=size(x); n=n(1);
	z=zeros(n,8);
	for k=1:n
		z(k,:)=callfemm(['eo_getpointvalues(' , numc(x(k,1)) , num(x(k,2)) , ')' ]);
	end
end

endfunction

function z=eo_getprobleminfo()
z=callfemm('eo_getprobleminfo()');

endfunction

function z=eo_getv(x,y)
if (argn(2)==2)
	z=eo_getpointvalues(x,y); z=z(:,1);
elseif (nargn==1)
	z=eo_getpointvalues(x); z=z(:,1);
end

endfunction

function eo_groupselectblock(n)
if(argn(2)==1)
callfemm(['eo_groupselectblock(' , num(n) , ')' ]);
else
callfemm('eo_groupselectblock()');
end

endfunction

function eo_hidecontourplot()
callfemm('eo_hidecontourplot()');

endfunction

function eo_hidedensityplot()
callfemm('eo_hidedensityplot()');

endfunction

function eo_hidegrid()
callfemm('eo_hidegrid()');

endfunction

function eo_hidemesh()
callfemm('eo_hidemesh()');

endfunction

function eo_hidenames()
callfemm('eo_shownames(0)');

endfunction

function eo_hidepoints()
callfemm('eo_hidepoints()');

endfunction

function z=eo_lineintegral(ptype)
z=callfemm(['eo_lineintegral(' , num(ptype) , ')' ]);

endfunction

function eo_makeplot(plottype,numpoints,filename,fileformat)
if (argn(2)==4)
callfemm(['eo_makeplot(' , numc(plottype) , numc(numpoints) , quotec(filename) , num(fileformat) , ')' ]);
end
if (argn(2)==3)
callfemm(['eo_makeplot(' , numc(plottype) , numc(numpoints) , quote(filename) , ')' ]);
end
if (argn(2)==2)
callfemm(['eo_makeplot(' , numc(plottype) , num(numpoints) , ')' ]);
end

endfunction

function eo_maximize()
	callfemm('eo_maximize()');

endfunction

function eo_minimize()
	callfemm('eo_minimize()');

endfunction

function z=eo_numelements()
z=callfemm('eo_numelements()');

endfunction

function z=eo_numnodes()
z=callfemm('eo_numnodes()');

endfunction

function eo_refreshview()
callfemm('eo_refreshview()');

endfunction

function eo_reload()
callfemm('eo_reload()');

endfunction

function eo_resize(nWidth,nHeight)
	callfemm(['eo_resize(', numc(nWidth), num(nHeight), ')' ]);


endfunction

function eo_restore()
	callfemm('eo_restore()');

endfunction

function eo_savebitmap(fn)
callfemm(['eo_savebitmap(' , quote(fn) , ')' ]);

endfunction

function eo_savemetafile(fn)
callfemm(['eo_savemetafile(' , quote(fn) , ')' ]);

endfunction

function eo_selectblock(x,y)
if (argn(2)==2)
	callfemm(['eo_selectblock(' , numc(x) , num(y) , ')' ]);
elseif (argn(2)==1)
	callfemm(['eo_selectblock(' , numc(x(1)) , num(x(2)) , ')' ]);
end

endfunction

function eo_selectpoint(x,y)
if (argn(2)==2)
	callfemm(['eo_selectpoint(' , numc(x) , num(y) , ')' ]);
elseif (argn(2)==1)
	callfemm(['eo_selectpoint(' , numc(x(1)) , num(x(2)) , ')' ]);
end

endfunction

function eo_seteditmode(mode)
callfemm(['eo_seteditmode(' , quote(mode) , ')' ]);

endfunction

function eo_setgrid(density,ptype)
callfemm(['eo_setgrid(' , numc(density) , quote(ptype) , ')' ]);

endfunction

function eo_showcontourplot(numcontours,al,au)
callfemm(['eo_showcontourplot(' , numc(numcontours) , numc(al) , num(au) , ')' ]);

endfunction

function eo_showdensityplot(legend,gscale,ptype,bu,bl)
callfemm(['eo_showdensityplot(' , numc(legend) , numc(gscale) , numc(ptype) , numc(bu) , num(bl) , ')' ]);

endfunction

function eo_showgrid()
callfemm('eo_showgrid()');

endfunction

function eo_showmesh()
callfemm('eo_showmesh()');

endfunction

function eo_shownames()
callfemm('eo_shownames(1)');

endfunction

function eo_showpoints()
callfemm('eo_showpoints()');

endfunction

function eo_showvectorplot(plottype,scalefactor)
if (argn(2)==2)
	callfemm(['eo_showvectorplot(' , numc(plottype) , num(scalefactor) , ')' ]);
elseif (argn(2)==1)
	callfemm(['eo_showvectorplot(' , num(plottype) , ')' ]);
end

endfunction

function eo_smooth(flag)
callfemm(['eo_smooth(' , quote(flag) , ')' ]);

endfunction

function eo_smoothoff()
callfemm('eo_smooth(""on"")');

endfunction

function eo_smoothon()
callfemm('eo_smooth(""on"")');

endfunction

function eo_snapgrid(flag)
callfemm(['eo_gridsnap(' , quote(flag) , ')' ]);

endfunction

function eo_snapgridoff()
callfemm('eo_gridsnap(""off"")');

endfunction

function eo_snapgridon()
callfemm('eo_gridsnap(""on"")');

endfunction

function eo_zoom(x1,y1,x2,y2)
if (argn(2)==4)
	callfemm(['eo_zoom(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);
elseif (argn(2)==2)
	callfemm(['eo_zoom(' , numc(x1(1)) , numc(x1(2)) , numc(y1(1)) , num(y1(2)) , ')' ]);
elseif (argn(2)==1)
	callfemm(['eo_zoom(' , numc(x1(1,1)) , numc(x1(1,2)) , numc(x1(2,1)) , num(x1(2,2)) , ')' ]);
end


endfunction

function eo_zoomin()
callfemm('eo_zoomin()');

endfunction

function eo_zoomnatural()
callfemm('eo_zoomnatural()');

endfunction

function eo_zoomout()
callfemm('eo_zoomout()');

endfunction

function hi_addarc(x1,y1,x2,y2,angle,maxseg)
callfemm(['hi_addarc(' , numc(x1) , numc(y1) , numc(x2) , numc(y2) , numc(angle) , num(maxseg) , ')']);

endfunction

function hi_addblocklabel(x,y)
if (argn(2)==2)
	callfemm(['hi_addblocklabel(' , numc(x) , num(y) , ')' ]);
elseif (argn(2)==1)
	callfemm(['hi_addblocklabel(' , numc(x(1)) , num(x(2)) , ')' ]);
end

endfunction

function hi_addboundprop(pname,fmt,Tset,qs,Tinf,h,bta)
select argn(2)
	case 0 then
		callfemm('hi_addboundprop()');
	case 1 then
        callfemm(['hi_addboundprop(' , quote(pname) , ')' ]);
	case 2 then
        callfemm(['hi_addboundprop(' , quotec(pname) , num(fmt) , ')' ]);
	case 3 then
        callfemm(['hi_addboundprop(' , quotec(pname) , numc(fmt) , num(Tset) , ')' ]);
	case 4 then
        callfemm(['hi_addboundprop(' , quotec(pname) , numc(fmt) , numc(Tset) , num(qs) , ')' ]);
	case 5 then
        callfemm(['hi_addboundprop(' , quotec(pname) , numc(fmt) , numc(Tset) , numc(qs) , num(Tinf) , ')' ]);
	case 6 then
        callfemm(['hi_addboundprop(' , quotec(pname) , numc(fmt) , numc(Tset) , numc(qs) , numc(Tinf) , num(h), ')' ]);
	else
        callfemm(['hi_addboundprop(' , quotec(pname) , numc(fmt) , numc(Tset) , numc(qs) , numc(Tinf) , numc(h), num(bta) , ')' ]);
end

endfunction

function hi_addconductorprop(pname,Tc,qc,ptype)
select argn(2)
	case 0 then
		callfemm('hi_addconductorprop()');
	case 1 then
		callfemm(['hi_addconductorprop(' , quote(pname) , ')' ]);
	case 2 then
		callfemm(['hi_addconductorprop(' , quotec(pname) , num(Tc) , ')' ]);
	case 3 then
		callfemm(['hi_addconductorprop(' , quotec(pname) , numc(Tc) , num(qc) , ')' ]);
	else
		callfemm(['hi_addconductorprop(' , quotec(pname) , numc(Tc) , numc(qc) , num(ptype) , ')' ]);
end

endfunction

function hi_addmaterial(pname,kx,ky,qv,kt)
select argn(2)
	case 0 then 
		callfemm('hi_addmaterial()');
	case 1 then
		callfemm(['hi_addmaterial(' , quote(pname) ')' ]);
	case 2 then
		callfemm(['hi_addmaterial(' , quotec(pname) , num(kx) , ')' ]);
	case 3 then 
		callfemm(['hi_addmaterial(' , quotec(pname) , numc(kx) , num(ky) , ')' ]);
	case 4 then
		callfemm(['hi_addmaterial(' , quotec(pname) , numc(kx) , numc(ky) , num(qv) , ')' ]);
	else
		callfemm(['hi_addmaterial(' , quotec(pname) , numc(kx) , numc(ky) , numc(qv) , num(kt) , ')' ]);
end

endfunction

function hi_addnode(x,y)
if (argn(2)==2)
    if (getlength(x)>1)
        if (getlength(y)~=getlength(x))
            error('invalid arguments to hi_addnode');
        end
        for k=1:getlength(x)
            callfemm(['hi_addnode(' , numc(x(k)) , num(y(k)) , ')']);
        end
    else callfemm(['hi_addnode(' , numc(x) , num(y) , ')']);
    end
elseif (argn(2)==1)
    callfemm(['hi_addnode(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function hi_addpointprop(pname,Tp,qp)
select argn(2)
	case 0 then
		callfemm('hi_addpointprop()')
	case 1 then
		callfemm(['hi_addpointprop(' , quote(pname) , ')']);
	case 2 then
		callfemm(['hi_addpointprop(' , quotec(pname) , num(Tp) , ')']);
	else
		callfemm(['hi_addpointprop(' , quotec(pname) , numc(Tp) , num(qp) , ')']);
end

endfunction

function hi_addsegment(x1,y1,x2,y2)
if (argn(2)==4)
	callfemm(['hi_addsegment(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);
elseif (argn(2)==2)
	callfemm(['hi_addsegment(' , numc(x1(1)) , numc(x1(2)) , numc(y1(1)) , num(y1(2)) , ')' ]);
elseif (argn(2)==1)
	callfemm(['hi_addsegment(' , numc(x1(1,1)) , numc(x1(1,2)) , numc(x1(2,1)) , num(x1(2,2)) , ')' ]);
end

endfunction

function hi_addtkpoint(name,b,h)
callfemm(['hi_addtkpoint(' , quotec(name) , numc(b) , num(h) , ')' ]);

endfunction

function hi_addtkpoints(name,bh,h)
if(argn(2)==3)
	hi_addtkpoints(name,[bh;h]');
elseif (argn(2)==2)
	n=size(bh); n=n(1);
	for k=1:n
      	hi_addtkpoint(name,bh(k,1),bh(k,2));
	end
end

endfunction

function hi_analyse(n)
if (argn(2)==1)
	callfemm(['hi_analyze(' , num(n) , ')' ]);
elseif (argn(2)==0)
	callfemm('hi_analyze()');
end

endfunction

function hi_analyze(n)
if (argn(2)==1)
	callfemm(['hi_analyze(' , num(n) , ')' ]);
elseif (argn(2)==0)
	callfemm('hi_analyze()');
end

endfunction

function hi_attachouterspace()
callfemm('hi_attachouterspace()');

endfunction

function hi_clearselected()
callfemm('hi_clearselected()');

endfunction

function hi_cleartkpoints(n)
callfemm(['hi_cleartkpoints(' , quote(n) , ')' ]);


endfunction

function hi_close()
callfemm('hi_close()');

endfunction

function hi_copyrotate(p1,p2,p3,p4)
if (argn(2)==4)
	bx=p1; by=p2; angle=p3; copies=p4;
elseif (argn(2)==3)
	bx=p1(1); by=p1(2); angle=p2; copies=p3;
end
callfemm(['hi_copyrotate(' , numc(bx) , numc(by) , numc(angle) , num(copies) , ')' ]);

endfunction

function hi_copyrotate2(p1,p2,p3,p4,p5)
if (argn(2)==5)
	bx=p1; by=p2; angle=p3; copies=p4; editaction=p5;
elseif (argn(2)==4)
	bx=p1(1); by=p1(2); angle=p2; copies=p3; editaction=p4;
end
callfemm(['hi_copyrotate(' , numc(bx) , numc(by) , numc(angle) , numc(copies) , num(editaction) , ')' ]);

endfunction

function hi_copytranslate(p1,p2,p3)
if (argn(2)==3)
	bx=p1; by=p2; copies=p3;
elseif (argn(2)==2)
	bx=p1(1); by=p1(2); copies=p2;
end
callfemm(['hi_copytranslate(' , numc(bx) , numc(by) , num(copies) , ')' ]);

endfunction

function hi_copytranslate2(p1,p2,p3,p4)
if (argn(2)==4)
	bx=p1; by=p2; copies=p3; editaction=p4;
elseif (argn(2)==3)
	bx=p1(1); by=p1(2); copies=p2; editaction=p3;
end
callfemm(['hi_copytranslate(' , numc(bx) , numc(by) , numc(copies) , num(editaction) , ')' ]);

endfunction

function z=hi_createmesh()
z=callfemm('hi_createmesh()');

endfunction

function hi_createradius(x,y,r)
callfemm(['hi_createradius(' , numc(x) , numc(y), num(r) , ')' ]);

endfunction

function hi_defineouterspace(Zo,Ro,Ri)
callfemm(['hi_defineouterspace(' , numc(Zo) , numc(Ro) , num(Ri) , ')' ]);

endfunction

function hi_deleteboundprop(n)
callfemm(['hi_deleteboundprop(' , quote(n) , ')' ]);

endfunction

function hi_deleteconductor(n)
callfemm(['hi_deleteconductor(' , quote(n) , ')' ]);

endfunction

function hi_deletematerial(n)
callfemm(['hi_deletematerial(' , quote(n) , ')' ]);

endfunction

function hi_deletepointprop(n)
callfemm(['hi_deletepointprop(' , quote(n) , ')' ]);

endfunction

function hi_deleteselected()
callfemm('hi_deleteselected()');

endfunction

function hi_deleteselectedarcsegments()
callfemm('hi_deleteselectedarcsegments()');

endfunction

function hi_deleteselectedlabels()
callfemm('hi_deleteselectedlabels()');

endfunction

function hi_deleteselectednodes()
callfemm('hi_deleteselectednodes()');

endfunction

function hi_deleteselectedsegments()
callfemm('hi_deleteselectedsegments()');

endfunction

function hi_detachouterspace()
callfemm('hi_detachouterspace()');

endfunction

function hi_drawarc(p1,p2,p3,p4,p5,p6)
if (argn(2)==6)
	x1=p1; y1=p2; x2=p3; y2=p4; angle=p5; maxseg=p6;
elseif (argn(2)==4)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2); angle=p3; maxseg=p4;
elseif (argn(2)==3)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2); angle=p2; maxseg=p3;
end
	hi_addnode(x1,y1);
      hi_addnode(x2,y2);
      hi_addarc(x1,y1,x2,y2,angle,maxseg);

endfunction

function hi_drawline(p1,p2,p3,p4)
if (argn(2)==4)
	x1=p1; y1=p2; x2=p3; y2=p4;
elseif (argn(2)==2)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2);
elseif (argn(2)==1)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2);
end
	hi_addnode(x1,y1);
	hi_addnode(x2,y2);
	hi_addsegment(x1,y1,x2,y2);

endfunction

function hi_drawpolygon(p)
	hi_drawpolyline(p);
	n=getlength(p);
	hi_drawline(p(1,1),p(1,2),p(n,1),p(n,2));

endfunction

function hi_drawpolyline(p)
	n=getlength(p);
	for k = 1:(n-1)
		hi_drawline(p(k,1),p(k,2),p(k+1,1),p(k+1,2));
	end

endfunction

function hi_drawrectangle(p1,p2,p3,p4)
if (argn(2)==4)
	x1=p1; y1=p2; x2=p3; y2=p4;
elseif (argn(2)==2)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2);
elseif (argn(2)==1)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2);
end
hi_drawpolygon([x1,y1;x1,y2;x2,y2;x2,y1]);


endfunction

function z=hi_getmaterial(matname)
callfemm(['hi_getmaterial(' , quote(matname) , ')' ]);

endfunction

function vu=hi_getview()
    fname=tempname;
    callfemm(['hi_savebitmap(', quote(fname), ')']);
    vu=imread(fname,'bmp');
    system(['del ',rawquote(fname)]);

endfunction

function hi_hidegrid()
callfemm('hi_hidegrid()');

endfunction

function hi_hidenames()
callfemm('hi_shownames(0)');

endfunction

function hi_loadsolution()
callfemm('hi_loadsolution()');

endfunction

function hi_maximize()
	callfemm('hi_maximize()');

endfunction

function hi_minimize()
	callfemm('hi_minimize()');

endfunction

function hi_mirror(p1,p2,p3,p4)
if (argn(2)==4)
	x1=p1; y1=p2; x2=p3; y2=p4;
elseif (argn(2)==2)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2); 
elseif (argn(2)==1)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2);
end
callfemm(['hi_mirror(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);

endfunction

function hi_mirror2(p1,p2,p3,p4,p5)
if (argn(2)==5)
	x1=p1; y1=p2; x2=p3; y2=p4; editaction=p5;
elseif (argn(2)==3)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2); editaction=p3;
elseif (argn(2)==2)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2); editaction=p2;
end
callfemm(['hi_mirror(' , numc(x1) , numc(y1) , numc(x2) , numc(y2) , num(editaction) , ')' ]);

endfunction

function hi_modifyboundprop(pname,pnum,pval)
        if (pnum==0)
                callfemm(['hi_modifyboundprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
        else
                callfemm(['hi_modifyboundprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

endfunction

function hi_modifyconductorprop(pname,pnum,pval)
        if (pnum==0)
                callfemm(['hi_modifyconductorprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
	  else
                callfemm(['hi_modifyconductorprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

endfunction

function hi_modifymaterial(pname,pnum,pval)
        if (pnum==0)
                callfemm(['hi_modifymaterial(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
	  else
                callfemm(['hi_modifymaterial(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

endfunction

function hi_modifypointprop(pname,pnum,pval)
        if (pnum==0)
                callfemm(['hi_modifypointprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
        else
                callfemm(['hi_modifypointprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

endfunction

function hi_moverotate(p1,p2,p3)
if (argn(2)==3)
	bx=p1; by=p2; shiftangle=p3;
elseif (argn(2)==2)
	bx=p1(1); by=p1(2); shiftangle=p2;
end
callfemm(['hi_moverotate(' , numc(bx) , numc(by) , num(shiftangle) , ')' ]);

endfunction

function hi_movetranslate(bx,by)
if (argn(2)==2)
	callfemm(['hi_movetranslate(' , numc(bx) , num(by) ')' ]);
elseif (argn(2)==1)
	callfemm(['hi_movetranslate(' , numc(bx(1)) , num(bx(2)) ')' ]);
end

endfunction

function hi_movetranslate2(p1,p2,p3)
if (argn(2)==3)
	bx=p1; by=p2; editaction=p3;
elseif (argn(2)==2)
	bx=p1(1); by=p1(2); editaction=p2;
end
callfemm(['hi_movetranslate(' , numc(bx) , numc(by) , num(editaction) , ')' ]);

endfunction

function hi_probdef(units, ptype, precision, depth, minangle, prevsoln, dt)
if (argn(2)==5)
	callfemm(['hi_probdef(' , quotec(units) , quotec(ptype) , numc(precision) , numc(depth) , num(minangle) , ')']);
elseif (argn(2)==7)
	callfemm(['hi_probdef(' , quotec(units) , quotec(ptype) , numc(precision) , numc(depth) , numc(minangle) , quotec(prevsoln), num(dt), ')']);
end

endfunction

function hi_purgemesh()
callfemm('hi_purgemesh()');

endfunction

function hi_readdxf(docname)
callfemm(['hi_readdxf(' , quote(docname) , ')' ]);

endfunction

function hi_refreshview()
callfemm('hi_refreshview()');

endfunction

function hi_resize(nWidth,nHeight)
	callfemm(['hi_resize(', numc(nWidth), num(nHeight), ')' ]);

endfunction

function hi_restore()
	callfemm('hi_restore()');

endfunction

function hi_saveas(fn)
callfemm(['hi_saveas(' , quote(fn) , ')']);

endfunction

function hi_savebitmap(n)
callfemm(['hi_savebitmap(' , quote(n) , ')' ]);

endfunction

function hi_savedxf(docname)
callfemm_noeval(['hi_savedxf(' , quote(docname) , ')' ]);

endfunction

function hi_savemetafile(n)
callfemm(['hi_savemetafile(' , quote(n) , ')' ]);

endfunction

function hi_scale(p1,p2,p3)
if (argn(2)==3)
	bx=p1; by=p2; sc=p3;
elseif (argn(2)==2)
	bx=p1(1); by=p1(2); sc=p2;
end
callfemm(['hi_scale(' , numc(bx) , numc(by) , numc(sc) , ')' ]);

endfunction

function hi_scale2(p1,p2,p3,p4)
if (argn(2)==4)
	bx=p1; by=p2; sc=p3; ea=p4;
elseif (argn(2)==3)
	bx=p1(1); by=p1(2); sc=p2; ea=p3;
end
callfemm(['hi_scale(' , numc(bx) , numc(by) , numc(sc) , num(ea) , ')' ]);

endfunction

function z=hi_selectarcsegment(x,y)
if (argn(2)==2)
	z=callfemm(['hi_selectarcsegment(' , numc(x) , num(y) , ')']);
elseif (argn(2)==1)
	z=callfemm(['hi_selectarcsegment(' , numc(x(1)) , num(x(2)) , ')']);
end
 
endfunction

function hi_selectgroup(gr)
callfemm(['hi_selectgroup(' , num(gr) , ')' ]);

endfunction

function z=hi_selectlabel(x,y)
if (argn(2)==2)
	z=callfemm(['hi_selectlabel(' , numc(x) , num(y) , ')']);
elseif (argn(2)==1)
	z=callfemm(['hi_selectlabel(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function z=hi_selectnode(x,y) 
if (argn(2)==2)
	z=callfemm(['hi_selectnode(' , numc(x) , num(y) , ')']);
elseif (argn(2)==1)
	z=callfemm(['hi_selectnode(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function z=hi_selectsegment(x,y)
if (argn(2)==2)
	z=callfemm(['hi_selectsegment(' , numc(x) , num(y) , ')']);
elseif (argn(2)==1)
	z=callfemm(['hi_selectsegment(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function hi_setarcsegmentprop(maxsegdeg,propname,hide,group,incond)
callfemm(['hi_setarcsegmentprop(' , numc(maxsegdeg) , quotec(propname) , numc(hide) , numc(group) , quote(incond) , ')' ]);

endfunction

function hi_setblockprop(blockname,automesh,meshsize,group)
callfemm(['hi_setblockprop(' , quotec(blockname) , numc(automesh) , numc(meshsize) , num(group) , ')']);

endfunction

function hi_seteditmode(editmode)
callfemm(['hi_seteditmode(' , quote(editmode) , ')' ]);

endfunction

function hi_setfocus(docname)
callfemm(['hi_setfocus(' , quote(docname) , ')' ]);

endfunction

function hi_setgrid(density,ptype)
callfemm(['hi_setgrid(' , numc(density) , quote(ptype) , ')' ]);

endfunction

function z=hi_setgroup(n)
z=callfemm(['hi_setgroup(' , num(n) , ')' ]);

endfunction

function hi_setnodeprop(nodeprop,groupno,inconductor)
callfemm(['hi_setnodeprop(' , quotec(nodeprop) , numc(groupno) , quote(inconductor) , ')']);

endfunction

function hi_setsegmentprop(pn,es,am,hi,gr, inconductor)
callfemm(['hi_setsegmentprop(' , quotec(pn) , numc(es) , numc(am) , numc(hi) , numc(gr) , quote(inconductor) , ')']);

endfunction

function hi_showgrid()
callfemm('hi_showgrid()');

endfunction

function hi_showmesh()
callfemm('hi_showmesh()');

endfunction

function hi_shownames()
callfemm('hi_shownames(1)');

endfunction

function hi_snapgridoff()
callfemm('hi_gridsnap(""off"")');

endfunction

function hi_snapgridon()
callfemm('hi_gridsnap(""on"")');

endfunction

function hi_zoom(x1,y1,x2,y2)
if (argn(2)==4)
	callfemm(['hi_zoom(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);
elseif (argn(2)==2)
	callfemm(['hi_zoom(' , numc(x1(1)) , numc(x1(2)) , numc(y1(1)) , num(y1(2)) , ')' ]);
elseif (argn(2)==1)
	callfemm(['hi_zoom(' , numc(x1(1,1)) , numc(x1(1,2)) , numc(x1(2,1)) , num(x1(2,2)) , ')' ]);
end

endfunction

function hi_zoomin()
callfemm('hi_zoomin()');

endfunction

function hi_zoomnatural()
callfemm('hi_zoomnatural()');

endfunction

function hi_zoomout()
callfemm('hi_zoomout()');

endfunction

function ho_addcontour(x,y)
if (argn(2)==2)
	callfemm(['ho_addcontour(' , numc(x) , num(y) , ')' ]);
elseif (argn(2)==1)
	callfemm(['ho_addcontour(' , numc(x(1)) , num(x(2)) , ')' ]);
end

endfunction

function ho_bendcontour(tta,dtta)
callfemm(['ho_bendcontour(' , numc(tta) , num(dtta) , ')' ]);

endfunction

function z=ho_blockintegral(ptype)
z=callfemm(['ho_blockintegral(' , num(ptype) , ')' ]);
        
endfunction

function ho_clearblock()
callfemm('ho_clearblock()');

endfunction

function ho_clearcontour()
callfemm('ho_clearcontour()');

endfunction

function ho_close()
callfemm('ho_close()');

endfunction

function z=ho_getconductorproperties(pname)
z=callfemm(['ho_getconductorproperties(' , quote(pname) , ')' ]);

endfunction

function z=ho_getelement(n)
z=callfemm(['ho_getelement(' , num(n) , ')' ]);

endfunction

function z=ho_getf(x,y)
if (argn(2)==2)
	z=ho_getpointvalues(x,y); z=z(:,2:3);
elseif (argn(2)==1)
	z=ho_getpointvalues(x); z=z(:,2:3);
end

endfunction

function z=ho_getg(x,y)
if (argn(2)==2)
	z=ho_getpointvalues(x,y); z=z(:,4:5);
elseif (argn(2)==1)
	z=ho_getpointvalues(x); z=z(:,4:5);
end

endfunction

function z=ho_getk(x,y)
if (argn(2)==2)
	z=ho_getpointvalues(x,y); z=z(:,6:7);
elseif (argn(2)==1)
	z=ho_getpointvalues(x); z=z(:,6:7);
end

endfunction

function z=ho_getnode(n)
z=callfemm(['ho_getnode(' , num(n) , ')' ]);

endfunction

function z=ho_getpointvalues(x,y)
if (argn(2)==2)
	n=getlength(x);
	if (getlength(y)~=n)
		error('must be same number of entries in X and Y arrays');
	end
	z=zeros(n,7);
	for k=1:n
		z(k,:)=callfemm(['ho_getpointvalues(' , numc(x(k)) , num(y(k)) , ')' ]);
	end
elseif (argn(2)==1)
	n=size(x); n=n(1);
	z=zeros(n,7);
	for k=1:n
		z(k,:)=callfemm(['ho_getpointvalues(' , numc(x(k,1)) , num(x(k,2)) , ')' ]);
	end
end

endfunction

function z=ho_getprobleminfo()
z=callfemm('ho_getprobleminfo()');

endfunction

function z=ho_gett(x,y)
if (argn(2)==2)
	z=ho_getpointvalues(x,y); z=z(:,1);
elseif (argn(2)==1)
	z=ho_getpointvalues(x); z=z(:,1);
end

endfunction

function vu=ho_getview()
    fname=tempname;
    callfemm(['ho_savebitmap(', quote(fname), ')']);
    vu=imread(fname,'bmp');
    system(['del ',rawquote(fname)]);

endfunction

function ho_groupselectblock(n)
if(argn(2)==1)
callfemm(['ho_groupselectblock(' , num(n) , ')' ]);
else
callfemm('ho_groupselectblock()');
end

endfunction

function ho_hidecontourplot()
callfemm('ho_hidecontourplot()');

endfunction

function ho_hidedensityplot()
callfemm('ho_hidedensityplot()');

endfunction

function ho_hidegrid()
callfemm('ho_hidegrid()');

endfunction

function ho_hidemesh()
callfemm('ho_hidemesh()');

endfunction

function ho_hidenames()
callfemm('ho_shownames(0)');

endfunction

function ho_hidepoints()
callfemm('ho_hidepoints()');

endfunction

function z=ho_lineintegral(ptype)
z=callfemm(['ho_lineintegral(' , num(ptype) , ')' ]);

endfunction

function ho_makeplot(plottype,numpoints,filename,fileformat)
if (argn(2)==4)
callfemm(['ho_makeplot(' , numc(plottype) , numc(numpoints) , quotec(filename) , num(fileformat) , ')' ]);
end
if (argn(2)==3)
callfemm(['ho_makeplot(' , numc(plottype) , numc(numpoints) , quote(filename) , ')' ]);
end
if (argn(2)==2)
callfemm(['ho_makeplot(' , numc(plottype) , num(numpoints) , ')' ]);
end

endfunction

function ho_maximize()
	callfemm('ho_maximize()');

endfunction

function ho_minimize()
	callfemm('ho_minimize()');

endfunction

function z=ho_numelements()
z=callfemm('ho_numelements()');

endfunction

function z=ho_numnodes()
z=callfemm('ho_numnodes()');

endfunction

function ho_refreshview()
callfemm('ho_refreshview()');

endfunction

function ho_reload()
callfemm('ho_reload()');

endfunction

function ho_resize(nWidth,nHeight)
	callfemm(['ho_resize(', numc(nWidth), num(nHeight), ')' ]);


endfunction

function ho_restore()
	callfemm('ho_restore()');

endfunction

function ho_savebitmap(fn)
callfemm(['ho_savebitmap(' , quote(fn) , ')' ]);

endfunction

function ho_savemetafile(fn)
callfemm(['ho_savemetafile(' , quote(fn) , ')' ]);

endfunction

function ho_selectblock(x,y)
if (argn(2)==2)
	callfemm(['ho_selectblock(' , numc(x) , num(y) , ')' ]);
elseif (argn(2)==1)
	callfemm(['ho_selectblock(' , numc(x(1)) , num(x(2)) , ')' ]);
end

endfunction

function ho_selectpoint(x,y)
if (argn(2)==2)
	callfemm(['ho_selectpoint(' , numc(x) , num(y) , ')' ]);
elseif (argn(2)==1)
	callfemm(['ho_selectpoint(' , numc(x(1)) , num(x(2)) , ')' ]);
end

endfunction

function ho_seteditmode(mode)
callfemm(['ho_seteditmode(' , quote(mode) , ')' ]);

endfunction

function ho_setgrid(density,ptype)
callfemm(['ho_setgrid(' , numc(density) , quote(ptype) , ')' ]);

endfunction

function ho_showcontourplot(numcontours,al,au)
callfemm(['ho_showcontourplot(' , numc(numcontours) , numc(al) , num(au) , ')' ]);

endfunction

function ho_showdensityplot(legend,gscale,ptype,bu,bl)
callfemm(['ho_showdensityplot(' , numc(legend) , numc(gscale) , numc(ptype) , numc(bu) , num(bl) , ')' ]);

endfunction

function ho_showgrid()
callfemm('ho_showgrid()');

endfunction

function ho_showmesh()
callfemm('ho_showmesh()');

endfunction

function ho_shownames()
callfemm('ho_shownames(1)');

endfunction

function ho_showpoints()
callfemm('ho_showpoints()');

endfunction

function ho_showvectorplot(plottype, scalefactor)
if (argn(2)==2)
	callfemm(['ho_showvectorplot(' , numc(plottype) , num(scalefactor) , ')' ]);
elseif (argn(2)==1)
	callfemm(['ho_showvectorplot(' , num(plottype) , ')' ]);
end

endfunction

function ho_smooth(flag)
callfemm(['ho_smooth(' , quote(flag) , ')' ]);

endfunction

function ho_smoothoff()
callfemm('ho_smooth(""on"")');

endfunction

function ho_smoothon()
callfemm('ho_smooth(""on"")');

endfunction

function ho_snapgrid(flag)
callfemm(['ho_gridsnap(' , quote(flag) , ')' ]);

endfunction

function ho_snapgridoff()
callfemm('ho_gridsnap(""off"")');

endfunction

function ho_snapgridon()
callfemm('ho_gridsnap(""on"")');

endfunction

function ho_zoom(x1,y1,x2,y2)
if (argn(2)==4)
	callfemm(['ho_zoom(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);
elseif (argn(2)==2)
	callfemm(['ho_zoom(' , numc(x1(1)) , numc(x1(2)) , numc(y1(1)) , num(y1(2)) , ')' ]);
elseif (argn(2)==1)
	callfemm(['ho_zoom(' , numc(x1(1,1)) , numc(x1(1,2)) , numc(x1(2,1)) , num(x1(2,2)) , ')' ]);
end


endfunction

function ho_zoomin()
callfemm('ho_zoomin()');

endfunction

function ho_zoomnatural()
callfemm('ho_zoomnatural()');

endfunction

function ho_zoomout()
callfemm('ho_zoomout()');


endfunction

function main_maximize()
	callfemm('main_maximize()');

endfunction

function main_minimize()
	callfemm('main_minimize()');

endfunction

function main_resize(nWidth,nHeight)
	callfemm(['main_resize(', numc(nWidth), num(nHeight), ')' ]);

endfunction

function main_restore()
	callfemm('main_restore()');

endfunction

function mi_addarc(x1,y1,x2,y2,angle,maxseg)
callfemm(['mi_addarc(' , numc(x1) , numc(y1) , numc(x2) , numc(y2) , numc(angle) , num(maxseg) , ')']);

endfunction

function mi_addbhpoint(name,b,h)
callfemm(['mi_addbhpoint(' , quotec(name) , numc(b) , num(h) , ')' ]);

endfunction

function mi_addbhpoints(name,bh,h)
if(argn(2)==3)
	mi_addbhpoints(name,[bh;h]');
elseif (argn(2)==2)
	n=size(bh); n=n(1);
	for k=1:n
      	mi_addbhpoint(name,bh(k,1),bh(k,2));
	end
end

endfunction

function mi_addblocklabel(x,y)
if (argn(2)==2)
	callfemm(['mi_addblocklabel(' , numc(x) , num(y) , ')' ]);
elseif (argn(2)==1)
	callfemm(['mi_addblocklabel(' , numc(x(1)) , num(x(2)) , ')' ]);
end

endfunction

function mi_addboundprop(pname,a0,a1,a2,phi,mu,sig,c0,c1,fmt,ia,oa)

select argn(2)
	case 0 then
		callfemm('mi_addboundprop()');
	case 1 then
        callfemm(['mi_addboundprop(' , quote(pname) , ')' ]);
	case 2 then
		callfemm(['mi_addboundprop(' , quotec(pname) , num(a0) , ')' ]);
	case 3 then
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , num(a1) , ')' ]);
	case 4 then
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , num(a2) ,  ')' ]);
	case 5 then
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , numc(a2) , num(phi)  , ')' ]);
	case 6 then
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , numc(a2) , numc(phi) , num(mu) , ')' ]);
	case 7 then
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , numc(a2) , numc(phi) , numc(mu) , num(sig) , ')' ]);
	case 8 then
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , numc(a2) , numc(phi) , numc(mu) , numc(sig) , num(c0) , ')' ]);
	case 9 then
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , numc(a2) , numc(phi) , numc(mu) , numc(sig) , numc(c0) , num(c1) , ')' ]);
	case 10 then
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , numc(a2) , numc(phi) , numc(mu) , numc(sig) , numc(c0) , numc(c1) , num(fmt) , ')' ]);
	case 11 then
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , numc(a2) , numc(phi) , numc(mu) , numc(sig) , numc(c0) , numc(c1) , numc(fmt) , num(ia), ')' ]);
	case 12 then
		callfemm(['mi_addboundprop(' , quotec(pname) , numc(a0) , numc(a1) , numc(a2) , numc(phi) , numc(mu) , numc(sig) , numc(c0) , numc(c1) , numc(fmt) , numc(ia), num(oa), ')' ]);
end 
endfunction

function mi_addcircprop(pname,ic,ptype)
callfemm(['mi_addcircprop(' , quotec(pname) , numc(ic) , num(ptype) , ')' ]);

endfunction

function mi_addmaterial(name,mux,muy,hc,js,c,lamd,phih,lamfill,lamtype,phihx,phihy,nstr,dwire)

select argn(2)
	case 0 then
		callfemm(['mi_addmaterial(',quote('Air'),')']);
	case 1 then
		callfemm(['mi_addmaterial(',quote(name),')']);
	case 2 then
		callfemm(['mi_addmaterial(',quotec(name),num(mux),')']);
	case 3 then
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),num(muy),')']);
	case 4 then
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),num(hc),')']);
	case 5 then
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),num(js),')']);
	case 6 then
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),num(c),')']);
	case 7 then
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),numc(c),num(lamd),')']);
	case 8 then
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),numc(c),numc(lamd),num(phih),')']);
	case 9 then
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),numc(c),numc(lamd),numc(phih),num(lamfill),')']);
	case 10 then
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),numc(c),numc(lamd),numc(phih),numc(lamfill),num(lamtype),')']);
	case 11 then 
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),numc(c),numc(lamd),numc(phih),numc(lamfill),numc(lamtype),num(phihx),')']);
	case 12 then
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),numc(c),numc(lamd),numc(phih),numc(lamfill),numc(lamtype),numc(phihx),num(phihy),')']);
	case 13 then 
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),numc(c),numc(lamd),numc(phih),numc(lamfill),numc(lamtype),numc(phihx),num(phihy),')']);
	case 14 then
		callfemm(['mi_addmaterial(',quotec(name),numc(mux),numc(muy),numc(hc),numc(js),numc(c),numc(lamd),numc(phih),numc(lamfill),numc(lamtype),numc(phihx),numc(phihy),numc(nstr),num(dwire),')']);
end


endfunction

function mi_addnode(x,y)
if (argn(2)==2)
    if (getlength(x)>1)
        if (getlength(y)~=getlength(x))
            error('invalid arguments to mi_addnode');
        end
        for k=1:getlength(x)
            callfemm(['mi_addnode(' , numc(x(k)) , num(y(k)) , ')']);
        end
    else callfemm(['mi_addnode(' , numc(x) , num(y) , ')']);
    end
elseif (argn(2)==1)
    callfemm(['mi_addnode(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function mi_addpointprop(pname,ap,jp)
callfemm(['mi_addpointprop(' , quotec(pname) , numc(ap), num(jp), ')']);

endfunction

function mi_addsegment(x1,y1,x2,y2)
if (argn(2)==4)
	callfemm(['mi_addsegment(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);
elseif (argn(2)==2)
	callfemm(['mi_addsegment(' , numc(x1(1)) , numc(x1(2)) , numc(y1(1)) , num(y1(2)) , ')' ]);
elseif (argn(2)==1)
	callfemm(['mi_addsegment(' , numc(x1(1,1)) , numc(x1(1,2)) , numc(x1(2,1)) , num(x1(2,2)) , ')' ]);
end

endfunction

function mi_analyse(n)
if (argn(2)==1)
    callfemm(['mi_analyze(' , num(n) , ')' ]);
elseif (argn(2)==0)
    callfemm('mi_analyze()');
end

endfunction

function mi_analyze(n)
if (argn(2)==1)
	callfemm(['mi_analyze(' , num(n) , ')' ]);
elseif (argn(2)==0)
	callfemm('mi_analyze()');
end

endfunction

function mi_attachouterspace()
callfemm('mi_attachouterspace()');

endfunction

function mi_clearbhpoints(n)
callfemm(['mi_clearbhpoints(' , quote(n) , ')' ]);

endfunction

function mi_clearselected()
callfemm(['mi_clearselected()']);

endfunction

function mi_close()
callfemm('mi_close()');

endfunction

function mi_copyrotate(p1,p2,p3,p4)
if (argn(2)==4)
	bx=p1; by=p2; angle=p3; copies=p4;
elseif (argn(2)==3)
	bx=p1(1); by=p1(2); angle=p2; copies=p3;
end
callfemm(['mi_copyrotate(' , numc(bx) , numc(by) , numc(angle) , num(copies) , ')' ]);

endfunction

function mi_copyrotate2(p1,p2,p3,p4,p5)
if (argn(2)==5)
	bx=p1; by=p2; angle=p3; copies=p4; editaction=p5;
elseif (argn(2)==4)
	bx=p1(1); by=p1(2); angle=p2; copies=p3; editaction=p4;
end
callfemm(['mi_copyrotate(' , numc(bx) , numc(by) , numc(angle) , numc(copies) , num(editaction) , ')' ]);

endfunction

function mi_copytranslate(p1,p2,p3)
if (argn(2)==3)
	bx=p1; by=p2; copies=p3;
elseif (argn(2)==2)
	bx=p1(1); by=p1(2); copies=p2;
end
callfemm(['mi_copytranslate(' , numc(bx) , numc(by) , num(copies) , ')' ]);

endfunction

function mi_copytranslate2(p1,p2,p3,p4)
if (argn(2)==4)
	bx=p1; by=p2; copies=p3; editaction=p4;
elseif (argn(2)==3)
	bx=p1(1); by=p1(2); copies=p2; editaction=p3;
end
callfemm(['mi_copytranslate(' , numc(bx) , numc(by) , numc(copies) , num(editaction) , ')' ]);

endfunction

function z=mi_createmesh()
z=callfemm('mi_createmesh()');

endfunction

function mi_createradius(x,y,r)
callfemm(['mi_createradius(' , numc(x) , numc(y), num(r) , ')' ]);


endfunction

function mi_defineouterspace(Zo,Ro,Ri)
callfemm(['mi_defineouterspace(' , numc(Zo) , numc(Ro) , num(Ri) , ')' ]);

endfunction

function mi_deleteboundprop(n)
callfemm(['mi_deleteboundprop(' , quote(n) , ')' ]);

endfunction

function mi_deletecircuit(n)
callfemm(['mi_deletecircuit(' , quote(n) , ')' ]);

endfunction

function mi_deletematerial(n)
callfemm(['mi_deletematerial(' , quote(n) , ')' ]);

endfunction

function mi_deletepointprop(n)
callfemm(['mi_deletepointprop(' , quote(n) , ')' ]);

endfunction

function mi_deleteselected()
callfemm(['mi_deleteselected()']);

endfunction

function mi_deleteselectedarcsegments()
callfemm(['mi_deleteselectedarcsegments()']);

endfunction

function mi_deleteselectedlabels()
callfemm(['mi_deleteselectedlabels()']);

endfunction

function mi_deleteselectednodes()
callfemm(['mi_deleteselectednodes()']);

endfunction

function mi_deleteselectedsegments()
callfemm(['mi_deleteselectedsegments()']);

endfunction

function mi_detachouterspace()
callfemm('mi_detachouterspace()');

endfunction

function mi_drawarc(p1,p2,p3,p4,p5,p6)
if (argn(2)==6)
	x1=p1; y1=p2; x2=p3; y2=p4; angle=p5; maxseg=p6;
elseif (argn(2)==4)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2); angle=p3; maxseg=p4;
elseif (argn(2)==3)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2); angle=p2; maxseg=p3;
end
	mi_addnode(x1,y1);
      mi_addnode(x2,y2);
      mi_addarc(x1,y1,x2,y2,angle,maxseg);

endfunction

function mi_drawline(p1,p2,p3,p4)
if (argn(2)==4)
	x1=p1; y1=p2; x2=p3; y2=p4;
elseif (argn(2)==2)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2);
elseif (argn(2)==1)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2);
end
	mi_addnode(x1,y1);
	mi_addnode(x2,y2);
	mi_addsegment(x1,y1,x2,y2);

endfunction

function mi_drawpolygon(p)
	mi_drawpolyline(p);
	n=getlength(p);
	mi_drawline(p(1,1),p(1,2),p(n,1),p(n,2));

endfunction

function mi_drawpolyline(p)
	n=getlength(p);
	for k = 1:(n-1)
		mi_drawline(p(k,1),p(k,2),p(k+1,1),p(k+1,2));
	end

endfunction

function mi_drawrectangle(p1,p2,p3,p4)
if (argn(2)==4)
	x1=p1; y1=p2; x2=p3; y2=p4;
elseif (argn(2)==2)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2);
elseif (argn(2)==1)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2);
end
mi_drawpolygon([x1,y1;x1,y2;x2,y2;x2,y1]);


endfunction

function z=mi_getmaterial(matname)
callfemm(['mi_getmaterial(' , quote(matname) , ')' ]);

endfunction

function mi_hidegrid()
callfemm('mi_hidegrid()');

endfunction

function mi_hidenames()
callfemm('mi_shownames(0)');

endfunction

function mi_loadsolution()
callfemm('mi_loadsolution()');


endfunction

function mi_maximize()
	callfemm('mi_maximize()');

endfunction

function mi_minimize()
	callfemm('mi_minimize()');

endfunction

function mi_mirror(p1,p2,p3,p4)
if (argn(2)==4)
	x1=p1; y1=p2; x2=p3; y2=p4;
elseif (argn(2)==2)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2); 
elseif (argn(2)==1)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2);
end
callfemm(['mi_mirror(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);

endfunction

function mi_mirror2(p1,p2,p3,p4,p5)
if (argn(2)==5)
	x1=p1; y1=p2; x2=p3; y2=p4; editaction=p5;
elseif (argn(2)==3)
	x1=p1(1); y1=p1(2); x2=p2(1); y2=p2(2); editaction=p3;
elseif (argn(2)==2)
	x1=p1(1,1); y1=p1(1,2); x2=p1(2,1); y2=p1(2,2); editaction=p2;
end
callfemm(['mi_mirror(' , numc(x1) , numc(y1) , numc(x2) , numc(y2) , num(editaction) , ')' ]);

endfunction

function mi_modifyboundprop(pname,pnum,pval)
	if (pnum==0)
      	callfemm(['mi_modifyboundprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
      else
		callfemm(['mi_modifyboundprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
      end

endfunction

function mi_modifycircprop(pname,pnum,pval)
        if (pnum==0)
                callfemm(['mi_modifycircprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
        else
                callfemm(['mi_modifycircprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
        end

endfunction

function mi_modifymaterial(pname,pnum,pval)
	if (pnum==0) 
		callfemm(['mi_modifymaterial(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
	else
		callfemm(['mi_modifymaterial(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
	end

endfunction

function mi_modifypointprop(pname,pnum,pval)
	if (pnum==0)
		callfemm(['mi_modifypointprop(' , quotec(pname) , numc(pnum) , quote(pval) , ')']);
	else
		callfemm(['mi_modifypointprop(' , quotec(pname) , numc(pnum) , num(pval) , ')']);
    end

endfunction

function mi_moverotate(p1,p2,p3)
if (argn(2)==3)
	bx=p1; by=p2; shiftangle=p3;
elseif (argn(2)==2)
	bx=p1(1); by=p1(2); shiftangle=p2;
end
callfemm(['mi_moverotate(' , numc(bx) , numc(by) , num(shiftangle) , ')' ]);

endfunction

function mi_movetranslate(bx,by)
if (argn(2)==2)
	callfemm(['mi_movetranslate(' , numc(bx) , num(by) ')' ]);
elseif (argn(2)==1)
	callfemm(['mi_movetranslate(' , numc(bx(1)) , num(bx(2)) ')' ]);
end

endfunction

function mi_movetranslate2(p1,p2,p3)
if (argn(2)==3)
	bx=p1; by=p2; editaction=p3;
elseif (argn(2)==2)
	bx=p1(1); by=p1(2); editaction=p2;
end
callfemm(['mi_movetranslate(' , numc(bx) , numc(by) , num(editaction) , ')' ]);

endfunction

function mi_probdef(freq, units, ptype, precision, depth, minangle, acsolver)
if (argn(2)==1)
callfemm(['mi_probdef(' , num(freq) , ')' ]);
elseif (argn(2)==2)
callfemm(['mi_probdef(' , numc(freq) , quote(units) , ')' ]);
elseif (argn(2)==3)
callfemm(['mi_probdef(' , numc(freq) , quotec(units) , quote(ptype) , ')' ]);
elseif (argn(2)==4)
callfemm(['mi_probdef(' , numc(freq) , quotec(units) , quotec(ptype) , num(precision) , ')' ]);
elseif (argn(2)==5)
callfemm(['mi_probdef(' , numc(freq) , quotec(units) , quotec(ptype) , numc(precision) , num(depth) , ')' ]);
elseif (argn(2)==6)
callfemm(['mi_probdef(' , numc(freq) , quotec(units) , quotec(ptype) , numc(precision) , numc(depth) , num(minangle) , ')' ]);
elseif (argn(2)==7)
callfemm(['mi_probdef(' , numc(freq) , quotec(units) , quotec(ptype) , numc(precision) , numc(depth) , numc(minangle) , num(acsolver) ')' ]);
end

endfunction

function mi_purgemesh()
callfemm('mi_purgemesh()');

endfunction

function mi_readdxf(docname)
callfemm(['mi_readdxf(' , quote(docname) , ')' ]);

endfunction

function mi_refreshview()
callfemm('mi_refreshview()');

endfunction

function mi_resize(nWidth,nHeight)
	callfemm(['mi_resize(', numc(nWidth), num(nHeight), ')' ]);

endfunction

function mi_restore()
	callfemm('mi_restore()');

endfunction

function mi_saveas(fn)
callfemm(['mi_saveas(' , quote(fn) , ')' ]);

endfunction

function mi_savebitmap(n)
callfemm(['mi_savebitmap(' , quote(n) , ')' ]);

endfunction

function mi_savedxf(docname)
callfemm_noeval(['mi_savedxf(' , quote(docname) , ')' ]);

endfunction

function mi_savemetafile(n)
callfemm(['mi_savemetafile(' , quote(n) , ')' ]);

endfunction

function mi_scale(p1,p2,p3)
if (argn(2)==3)
	bx=p1; by=p2; sc=p3;
elseif (argn(2)==2)
	bx=p1(1); by=p1(2); sc=p2;
end
callfemm(['mi_scale(' , numc(bx) , numc(by) , numc(sc) , ')' ]);

endfunction

function mi_scale2(p1,p2,p3,p4)
if (argn(2)==4)
	bx=p1; by=p2; sc=p3; ea=p4;
elseif (argn(2)==3)
	bx=p1(1); by=p1(2); sc=p2; ea=p3;
end
callfemm(['mi_scale(' , numc(bx) , numc(by) , numc(sc) , num(ea) , ')' ]);

endfunction

function z=mi_selectarcsegment(x,y)
if (argn(2)==2)
	z=callfemm(['mi_selectarcsegment(' , numc(x) , num(y) , ')']);
elseif (argn(2)==1)
	z=callfemm(['mi_selectarcsegment(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function mi_selectgroup(gr)
callfemm(['mi_selectgroup(' , num(gr) , ')' ]);

endfunction

function z=mi_selectlabel(x,y)
if (argn(2)==2)
	z=callfemm(['mi_selectlabel(' , numc(x) , num(y) , ')']);
elseif (argn(2)==1)
	z=callfemm(['mi_selectlabel(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function z=mi_selectnode(x,y) 
if (argn(2)==2)
	z=callfemm(['mi_selectnode(' , numc(x) , num(y) , ')']);
elseif (argn(2)==1)
	z=callfemm(['mi_selectnode(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function z=mi_selectsegment(x,y)
if (argn(2)==2)
	z=callfemm(['mi_selectsegment(' , numc(x) , num(y) , ')']);
elseif (argn(2)==1)
	z=callfemm(['mi_selectsegment(' , numc(x(1)) , num(x(2)) , ')']);
end

endfunction

function mi_setarcsegmentprop(maxsegdeg,propname,hide,group)
callfemm(['mi_setarcsegmentprop(' , numc(maxsegdeg) , quotec(propname) , numc(hide) , num(group) , ')' ]);

endfunction

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

endfunction

function mi_setcurrent(name,x)
mi_modifycircprop(name,1,x);

endfunction

function mi_seteditmode(editmode)
callfemm(['mi_seteditmode(' , quote(editmode) , ')' ]);

endfunction

function mi_setfocus(docname)
callfemm(['mi_setfocus(' , quote(docname) , ')' ]);

endfunction

function mi_setgrid(density,ptype)
callfemm(['mi_setgrid(' , numc(density) , quote(ptype) , ')' ]);

endfunction

function z=mi_setgroup(n)
z=callfemm(['mi_setgroup(' , num(n) , ')' ]);

endfunction

function mi_setnodeprop(nodeprop,groupno)
callfemm(['mi_setnodeprop(' , quotec(nodeprop) , num(groupno) , ')']);

endfunction

function mi_setsegmentprop(pn,es,am,hi,gr)
callfemm(['mi_setsegmentprop(' , quotec(pn) , numc(es) , numc(am) , numc(hi) , num(gr) , ')']);

endfunction

function mi_showgrid()
callfemm('mi_showgrid()');

endfunction

function mi_showmesh()
callfemm('mi_showmesh()');

endfunction

function mi_shownames()
callfemm('mi_shownames(1)');

endfunction

function mi_snapgridoff()
callfemm('mi_gridsnap(""off"")');

endfunction

function mi_snapgridon()
callfemm('mi_gridsnap(""on"")');

endfunction

function mi_zoom(x1,y1,x2,y2)
if (argn(2)==4)
	callfemm(['mi_zoom(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);
elseif (argn(2)==2)
	callfemm(['mi_zoom(' , numc(x1(1)) , numc(x1(2)) , numc(y1(1)) , num(y1(2)) , ')' ]);
elseif (argn(2)==1)
	callfemm(['mi_zoom(' , numc(x1(1,1)) , numc(x1(1,2)) , numc(x1(2,1)) , num(x1(2,2)) , ')' ]);
end

endfunction

function mi_zoomin()
callfemm('mi_zoomin()');

endfunction

function mi_zoomnatural()
callfemm('mi_zoomnatural()');

endfunction

function mi_zoomout()
callfemm('mi_zoomout()');

endfunction

function mo_addcontour(x,y)
if (argn(2)==2)
	callfemm(['mo_addcontour(' , numc(x) , num(y) , ')' ]);
elseif (argn(2)==1)
	callfemm(['mo_addcontour(' , numc(x(1)) , num(x(2)) , ')' ]);
end

endfunction

function mo_bendcontour(tta,dtta)
callfemm(['mo_bendcontour(' , numc(tta) , num(dtta) , ')' ]);

endfunction

function z=mo_blockintegral(ptype)
	z=callfemm(['mo_blockintegral(' , num(ptype) , ')' ]);

endfunction

function mo_clearblock()
callfemm('mo_clearblock()');

endfunction

function mo_clearcontour()
callfemm('mo_clearcontour()');

endfunction

function mo_close()
callfemm('mo_close()');

endfunction

function z=mo_geta(x,y)
if (argn(2)==2)
	z=mo_getpointvalues(x,y); z=z(:,1);
elseif (argn(2)==1)
	z=mo_getpointvalues(x); z=z(:,1);
end

endfunction

function z=mo_getb(x,y)
if (argn(2)==2)
	z=mo_getpointvalues(x,y); z=z(:,2:3);
elseif (argn(2)==1)
	z=mo_getpointvalues(x); z=z(:,2:3);
end

endfunction

function z=mo_getcircuitproperties(name)
	z=callfemm(['mo_getcircuitproperties(' , quote(name) , ')' ]);

endfunction

function z=mo_getconductivity(x,y);
if (argn(2)==2)
	z=mo_getpointvalues(x,y); z=z(:,4);
elseif (argn(2)==1)
	z=mo_getpointvalues(x); z=z(:,4);
end

endfunction

function z=mo_getelement(n)
z=callfemm(['mo_getelement(' , num(n) , ')' ]);

endfunction

function z=mo_getenergydensity(x,y);
if (argn(2)==2)
	z=mo_getpointvalues(x,y); z=z(:,5);
elseif (argn(2)==1)
	z=mo_getpointvalues(x); z=z(:,5);
end

endfunction

function z=mo_getfill(x,y)
if (argn(2)==2)
	z=mo_getpointvalues(x,y); z=z(:,14);
elseif (argn(2)==1)
	z=mo_getpointvalues(x(1),x(2)); z=z(:,14);
end




endfunction

function z=mo_geth(x,y)
if (argn(2))==2
	z=mo_getpointvalues(x,y); z=z(:,6:7);
elseif (argn(2)==1)
	z=mo_getpointvalues(x); z=z(:,6:7);
end

endfunction

function z=mo_getj(x,y)
if (argn(2)==2)
	z=mo_getpointvalues(x,y); z=z(:,8:9)*[1;1];
elseif (argn(2)==1)
	z=mo_getpointvalues(x); z=z(:,8:9)*[1;1];
end

endfunction

function z=mo_getmu(x,y)
if (argn(2)==2)
	z=mo_getpointvalues(x,y); z=z(:,10:11);
elseif (argn(2)==1)
	z=mo_getpointvalues(x); z=z(:,10:11);
end

endfunction

function z=mo_getnode(n)
z=callfemm(['mo_getnode(' , num(n) , ')' ]);

endfunction

function z=mo_getpe(x,y)
if (argn(2)==2)
	z=mo_getpointvalues(x,y); z=z(:,12);
elseif (argn(2)==1)
	z=mo_getpointvalues(x); z=z(:,12);
end

endfunction

function z=mo_getph(x,y)
if (argn(2)==2)
	z=mo_getpointvalues(x,y); z=z(:,13);
elseif (argn(2)==1)
	z=mo_getpointvalues(x(1),x(2)); z=z(:,13);
end

endfunction

function p=mo_getpointvalues(x,y)
global pointvaluesfilter
if (argn(2)==2)
	n=getlength(x);
	if (getlength(y)~=n)
		error('must be same number of entries in X and Y arrays');
	end
	z=zeros(n,14);
	for k=1:n
		z(k,:)=callfemm(['mo_getpointvalues(' , numc(x(k)) , num(y(k)) , ')']);
	end
elseif (argn(2)==1)
	n=size(x); n=n(1);
	z=zeros(n,14);
	for k=1:n
		z(k,:)=callfemm(['mo_getpointvalues(' , numc(x(k,1)) , num(x(k,2)) , ')']);
	end
else
	z=0;
end
if (getlength(z)>1)
	p=z; 	
else
	p=zeros(getlength(x),14);
end
endfunction

//function p=mo_getpointvalues(x,y)
//	p=callfemm(['mo_getpointvalues(' , numc(x) , num(y) , ')']);
//endfunction

function z=mo_getprobleminfo()
	z=callfemm('mo_getprobleminfo()');

endfunction

function mo_groupselectblock(n)
if(argn(2)==1)
callfemm(['mo_groupselectblock(' , num(n) , ')' ]);
else
callfemm(['mo_groupselectblock()' ]);
end

endfunction

function mo_hidecontourplot()
callfemm('mo_hidecontourplot()');

endfunction

function mo_hidedensityplot()
callfemm('mo_hidedensityplot()');

endfunction

function mo_hidegrid()
callfemm('mo_hidegrid()');

endfunction

function mo_hidemesh()
callfemm('mo_hidemesh()');

endfunction

function mo_hidenames()
callfemm('mo_shownames(0)');

endfunction

function mo_hidepoints()
callfemm('mo_hidepoints()');

endfunction

function z=mo_lineintegral(ptype)
	z=callfemm(['mo_lineintegral(' , num(ptype) , ')']);

endfunction

function mo_makeplot(plottype,numpoints,filename,fileformat)
if (argn(2)==4)
callfemm(['mo_makeplot(' , numc(plottype) , numc(numpoints) , quotec(filename) , num(fileformat) , ')' ]);
end
if (argn(2)==3)
callfemm(['mo_makeplot(' , numc(plottype) , numc(numpoints) , quote(filename) , ')' ]);
end
if (argn(2)==2)
callfemm(['mo_makeplot(' , numc(plottype) , num(numpoints) , ')' ]);
end

endfunction

function mo_maximize()
	callfemm('mo_maximize()');

endfunction

function mo_minimize()
	callfemm('mo_minimize()');

endfunction

function z=mo_numelements()
z=callfemm('mo_numelements()');

endfunction

function z=mo_numnodes()
z=callfemm('mo_numnodes()');

endfunction

function mo_refreshview()
callfemm('mo_refreshview()');

endfunction

function mo_reload()
callfemm('mo_reload()');

endfunction

function mo_resize(nWidth,nHeight)
	callfemm(['mo_resize(', numc(nWidth), num(nHeight), ')' ]);

endfunction

function mo_restore()
	callfemm('mo_restore()');

endfunction

function mo_savebitmap(fn)
callfemm(['mo_savebitmap(' , quote(fn) , ')' ]);

endfunction

function mo_savemetafile(fn)
callfemm(['mo_savemetafile(' , quote(fn) , ')' ]);

endfunction

function mo_selectblock(x,y)
if (argn(2)==2)
	callfemm(['mo_selectblock(' , numc(x) , num(y) , ')' ]);
elseif (argn(2)==1)
	callfemm(['mo_selectblock(' , numc(x(1)) , num(x(2)) , ')' ]);
end

endfunction

function mo_selectpoint(x,y)
if (argn(2)==2)
	callfemm(['mo_selectpoint(' , numc(x) , num(y) , ')' ]);
elseif (argn(2)==1)
	callfemm(['mo_selectpoint(' , numc(x(1)) , num(x(2)) , ')' ]);
end

endfunction

function mo_seteditmode(mode)
callfemm(['mo_seteditmode(' , quote(mode) , ')' ]);

endfunction

function mo_setweightingscheme(mode)
callfemm(['mo_setweightingscheme(' , quote(mode) , ')' ]);

endfunction

function mo_setgrid(density,ptype)
callfemm(['mo_setgrid(' , numc(density) , quote(ptype) , ')' ]);

endfunction

function mo_showcontourplot(numcontours,al,au,ptype)
callfemm(['mo_showcontourplot(' , numc(numcontours) , numc(al) ,  numc(au) , quote(ptype) , ')' ]);

endfunction

function mo_showdensityplot(legend,gscale,bu,bl,ptype)
callfemm(['mo_showdensityplot(' , numc(legend) , numc(gscale) , numc(bu) , numc(bl) , quote(ptype) , ')' ]);

endfunction

function mo_showgrid()
callfemm('mo_showgrid()');

endfunction

function mo_showmesh()
callfemm('mo_showmesh()');

endfunction

function mo_shownames()
callfemm('mo_shownames(1)');

endfunction

function mo_showpoints()
callfemm('mo_showpoints()');

endfunction

function mo_showvectorplot(plottype,scalefactor)
if (argn(2)==2)
	callfemm(['mo_showvectorplot(' , numc(plottype) , num(scalefactor) , ')' ]);
elseif (argn(2)==1)
	callfemm(['mo_showvectorplot(' , num(plottype) , ')' ]);
end

endfunction

function mo_smooth(flag)
callfemm(['mo_smooth(' , quote(flag) , ')' ]);

endfunction

function mo_smoothoff()
callfemm('mo_smooth(""on"")');

endfunction

function mo_smoothon()
callfemm('mo_smooth(""on"")');

endfunction

function mo_snapgrid(flag)
callfemm(['mo_gridsnap(' , quote(flag) , ')' ]);

endfunction

function mo_snapgridoff()
callfemm('mo_gridsnap(""off"")');

endfunction

function mo_snapgridon()
callfemm('mo_gridsnap(""on"")');

endfunction

function mo_zoom(x1,y1,x2,y2)
if (argn(2)==4)
	callfemm(['mo_zoom(' , numc(x1) , numc(y1) , numc(x2) , num(y2) , ')' ]);
elseif (argn(2)==2)
	callfemm(['mo_zoom(' , numc(x1(1)) , numc(x1(2)) , numc(y1(1)) , num(y1(2)) , ')' ]);
elseif (argn(2)==1)
	callfemm(['mo_zoom(' , numc(x1(1,1)) , numc(x1(1,2)) , numc(x1(2,1)) , num(x1(2,2)) , ')' ]);
end

endfunction

function mo_zoomin()
callfemm('mo_zoomin()');

endfunction

function mo_zoomnatural()
callfemm('mo_zoomnatural()');

endfunction

function mo_zoomout()
callfemm('mo_zoomout()');

endfunction

function mi_attachdefault()
callfemm('mi_attachdefault()');
endfunction

function mi_detachdefault()
callfemm('mi_detachdefault()');
endfunction

function ei_attachdefault()
callfemm('ei_attachdefault()');
endfunction

function ei_detachdefault()
callfemm('ei_detachdefault()');
endfunction

function hi_attachdefault()
callfemm('hi_attachdefault()');
endfunction

function hi_detachdefault()
callfemm('hi_detachdefault()');
endfunction

function ci_attachdefault()
callfemm('ci_attachdefault()');
endfunction

function ci_detachdefault()
callfemm('ci_detachdefault()');
endfunction


function ci_selectcircle(x,y,R,editmode)
if (argn(2)==3)
        callfemm(['ci_selectcircle(' , numc(x) , numc(y), num(R) , ')' ]);
elseif (argn(2)==4)
        callfemm(['ci_selectcircle(' , numc(x) , numc(y), numc(R), num(editmode) , ')' ]);
end
endfunction

function ei_selectcircle(x,y,R,editmode)
if (argn(2)==3)
        callfemm(['ei_selectcircle(' , numc(x) , numc(y), num(R) , ')' ]);
elseif (argn(2)==4)
        callfemm(['ei_selectcircle(' , numc(x) , numc(y), numc(R), num(editmode) , ')' ]);
end
endfunction

function hi_selectcircle(x,y,R,editmode)
if (argn(2)==3)
        callfemm(['hi_selectcircle(' , numc(x) , numc(y), num(R) , ')' ]);
elseif (argn(2)==4)
        callfemm(['hi_selectcircle(' , numc(x) , numc(y), numc(R), num(editmode) , ')' ]);
end
endfunction

function mi_selectcircle(x,y,R,editmode)
if (argn(2)==3)
        callfemm(['mi_selectcircle(' , numc(x) , numc(y), num(R) , ')' ]);
elseif (argn(2)==4)
        callfemm(['mi_selectcircle(' , numc(x) , numc(y), numc(R), num(editmode) , ')' ]);
end
endfunction

function ci_selectrectangle(x1,x1,x2,y2,editmode)
if (argn(2)==4)
        callfemm(['ci_selectrectangle(' , numc(x1) , numc(y1), numc(x2), num(y2) , ')' ]);
elseif (argn(2)==5)
        callfemm(['ci_selectrectangle(' , numc(x1) , numc(y1), numc(x2), numc(y2), num(editmode) , ')' ]);
end
endfunction

function ei_selectrectangle(x1,y1,x2,y2,editmode)
if (argn(2)==4)
        callfemm(['ei_selectrectangle(' , numc(x1) , numc(y1), numc(x2), num(y2) , ')' ]);
elseif (argn(2)==5)
        callfemm(['ei_selectrectangle(' , numc(x1) , numc(y1), numc(x2), numc(y2), num(editmode) , ')' ]);
end
endfunction

function hi_selectrectangle(x1,y1,x2,y2,editmode)
if (argn(2)==4)
        callfemm(['hi_selectrectangle(' , numc(x1) , numc(y1), numc(x2), num(y2) , ')' ]);
elseif (argn(2)==5)
        callfemm(['hi_selectrectangle(' , numc(x1) , numc(y1), numc(x2), numc(y2), num(editmode) , ')' ]);
end
endfunction

function mi_selectrectangle(x1,y1,x2,y2,editmode)
if (argn(2)==4)
        callfemm(['mi_selectrectangle(' , numc(x1) , numc(y1), numc(x2), num(y2) , ')' ]);
elseif (argn(2)==5)
        callfemm(['mi_selectrectangle(' , numc(x1) , numc(y1), numc(x2), numc(y2), num(editmode) , ')' ]);
end
endfunction

function mi_makeABC(n,R,x,y,bc)
if (argn(2)==5)
	callfemm(['mi_makeABC(' , numc(n) , numc(R), numc(x), numc(y), num(bc), ')' ]);
elseif (argn(2)==4)
        callfemm(['mi_makeABC(' , numc(n) , numc(R), numc(x), num(y) , ')' ]);
elseif (argn(2)==3)
        callfemm(['mi_makeABC(' , numc(n) , numc(R), num(x) , ')' ]);
elseif (argn(2)==2)
        callfemm(['mi_makeABC(' , numc(n) , num(R), ')' ]);
elseif (argn(2)==1)
        callfemm(['mi_makeABC(' , num(n) , ')' ]);
else
        callfemm('mi_makeABC()');
end
endfunction

function hi_makeABC(n,R,x,y,bc)
if (argn(2)==5)
	callfemm(['hi_makeABC(' , numc(n) , numc(R), numc(x), numc(y), num(bc), ')' ]);
elseif (argn(2)==4)
        callfemm(['hi_makeABC(' , numc(n) , numc(R), numc(x), num(y) , ')' ]);
elseif (argn(2)==3)
        callfemm(['hi_makeABC(' , numc(n) , numc(R), num(x) , ')' ]);
elseif (argn(2)==2)
        callfemm(['hi_makeABC(' , numc(n) , num(R), ')' ]);
elseif (argn(2)==1)
        callfemm(['hi_makeABC(' , num(n) , ')' ]);
else
        callfemm('hi_makeABC()');
end
endfunction

function ei_makeABC(n,R,x,y,bc)
if (argn(2)==5)
	callfemm(['ei_makeABC(' , numc(n) , numc(R), numc(x), numc(y), num(bc), ')' ]);
elseif (argn(2)==4)
        callfemm(['ei_makeABC(' , numc(n) , numc(R), numc(x), num(y) , ')' ]);
elseif (argn(2)==3)
        callfemm(['ei_makeABC(' , numc(n) , numc(R), num(x) , ')' ]);
elseif (argn(2)==2)
        callfemm(['ei_makeABC(' , numc(n) , num(R), ')' ]);
elseif (argn(2)==1)
        callfemm(['ei_makeABC(' , num(n) , ')' ]);
else
        callfemm('ei_makeABC()');
end
endfunction

function ci_makeABC(n,R,x,y,bc)
if (argn(2)==5)
	callfemm(['ci_makeABC(' , numc(n) , numc(R), numc(x), numc(y), num(bc), ')' ]);
elseif (argn(2)==4)
        callfemm(['ci_makeABC(' , numc(n) , numc(R), numc(x), num(y) , ')' ]);
elseif (argn(2)==3)
        callfemm(['ci_makeABC(' , numc(n) , numc(R), num(x) , ')' ]);
elseif (argn(2)==2)
        callfemm(['ci_makeABC(' , numc(n) , num(R), ')' ]);
elseif (argn(2)==1)
        callfemm(['ci_makeABC(' , num(n) , ')' ]);
else
        callfemm('ci_makeABC()');
end
endfunction

function smartmesh(n)
	callfemm(['smartmesh(' , num(n) , ')' ]);
endfunction

function mi_smartmesh(n)
	callfemm(['mi_smartmesh(' , num(n) , ')' ]);
endfunction

function ei_smartmesh(n)
	callfemm(['ei_smartmesh(' , num(n) , ')' ]);
endfunction

function hi_smartmesh(n)
	callfemm(['hi_smartmesh(' , num(n) , ')' ]);
endfunction

function ci_smartmesh(n)
	callfemm(['ci_smartmesh(' , num(n) , ')' ]);
endfunction

function mi_setprevious(fn,pt)
if (argn(2)==1)
	callfemm(['mi_setprevious(' , quote(fn) , ')' ]);
elseif (argn(2)==2)
	callfemm(['mi_setprevious(' , quotec(fn) , num(pt) , ')' ]);
end
endfunction

function z=mo_getgapb(bdryname,angle)
	z=callfemm(['mo_getgapb(' , quotec(bdryname) , num(angle) , ')']);
endfunction

function z=mo_getgapa(bdryname,angle)
	z=callfemm(['mo_getgapa(' , quotec(bdryname) , num(angle) , ')'])
endfunction

function z=mo_gapintegral(bdryname,inttype)
	z=callfemm(['mo_gapintegral(' , quotec(bdryname) , num(inttype) , ')'])
endfunction

function z=mo_getgapharmonics(bdryname,n)
	if (argn(2)==1)
		z=callfemm(['mo_gapintegral(' , quote(bdryname) , ')'])
	else
		z=callfemm(['mo_gapintegral(' , quotec(bdryname) , num(n) , ')'])
	end
endfunction

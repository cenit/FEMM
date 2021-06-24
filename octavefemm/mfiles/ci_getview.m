% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function vu=ci_getview()
    fname=tempname;
    callfemm(['ci_savebitmap(', quote(fname), ')']);
    vu=imread(fname,'bmp');
    system(['del ',rawquote(fname)]);


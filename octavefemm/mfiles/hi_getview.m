% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function vu=hi_getview()
    fname=tempname;
    callfemm(['hi_savebitmap(', quote(fname), ')']);
    vu=imread(fname,'bmp');
    system(['del ',rawquote(fname)]);


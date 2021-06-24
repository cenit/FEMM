% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function vu=ho_getview()
    fname=tempname;
    callfemm(['ho_savebitmap(', quote(fname), ')']);
    vu=imread(fname,'bmp');
    system(['del ',rawquote(fname)]);


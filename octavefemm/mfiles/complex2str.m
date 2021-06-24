% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

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


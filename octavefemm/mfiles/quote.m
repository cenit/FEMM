% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function s=quote(u)

try
	if (ischar(u)~=1) u=mat2str(u); end
catch
    if (isstr(u)~=1) u=mat2str(u); end
end

v=strrep(u,char([92,92]),char([92]));
v=strrep(v,char([92]),char([92,92]));
s= [ '"' , v , '"' ];


% ActiveFEMM (C)2006 David Meeker, dmeeker@ieee.org

function closefemm()

  global HandleToFEMM

  if (exist('actxserver'))
      global HandleToFEMM
      delete(HandleToFEMM);
      HandleToFEMM=0;
  else
      callfemm('quit()');
  end


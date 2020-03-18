FEMM 4.2 21Apr2019
David Meeker
dmeeker@ieee.org

21Apr2019: (Test Build)

* Start from previous solution for magnetics problems. To use this, specify a 
  file name in "Previous Solution" edit box in the Problem Definition dialog.
  Specify the "Prev Type" to be "None".  This works if the mesh has not changed
  from the last solution.  Good rotating problems with the "sliding band"
  boundary connecting the rotor and stator.  These problems often involve
  consideration of a sequence of problems with slightly different rotor angles
  and currents.  The "sliding band" approach lets the rotor move without
  modifying the mesh.
* Updated materials library for heat flow problems (thanks to Daniel Gheorghe)
* Updated hard magnetic materials in magnetics materials library (thanks to 
  Mike Devine). Includes new definitions for Sintered NdFeB, Bonded NdFeB,
  Sintered SmCo, and Alnico. See explanatory webpages on femm.info for sources
  and rationale.
* Fixed precision in exported DXF so that no precision is lost in DXF exports.
* Changed the algorithm that identifies limits for plotting flux density so that
  exterior regions (both Kelvin Transformation and IABC) are not considered.
  These exterior regions can have high flux densities that mask the flux density
  variations in the interior region of interest.
  
06Oct2018: (Test Build)

* Fixed the cause of an occasional error that happens running many runs in
  Mathematica.  Fkn.exe occasionally could not open a *.ans file to write over 
  older results.  The fix adds a retry if the file can't be opened.
* Fixed a bug with the computation of the "sliding band" incremental torque
  integral. The "previous solution" used to calculate the integral was not
  obtained correctly.
* For incremental runs, mesh temporary files never got deleted, leaving extra,
  spurious files hanging around. Code has been added to get rid of these
  files when they're not needed any more.  

25Feb2018:

* Added an "air gap boundary condition" that allows the rotor to be continuously
  moved without the mesh changing with rotor position. A number of new scripting
  functions have been added to calculate forces, torques, and field values of
  associated with an analytical solution in the air gap between the rotor and
  stator. Detailed examples to follow.
* Added file-by-file smartmesh attribute along with scripting commands for each
  input file type (mi_smartmesh, ei_smartmesh, hi_smartmesh, ci_smartmesh)
  to turn smartmesh on or off for a particular file.  The global smartmesh
  scripting function still works to turn smartmesh on or off across all problem
  types for a particular session.
* Moved to console version of Triangle 1.6 made from directly compiling Triangle
  source rather than wrapping it with a dialog.

03Dec2017: (Test Build)

* Fixed one more focus-stealing hole that took away focus during Lua
  script execution. As noted at https://tinyurl.com/y9uggja3 the OnInitDialog
  functions of triangle.exe, fkn.exe, belasolv.exe, csolv.exe, and hsolve.exe
  must return FALSE instead of TRUE, or else focus might be stolen.
  
24Nov2017: (Test Build)

* Changed to Triangle 1.6 instead of Triangle 1.3.  Version 1.6 is more
  robust and usually does not hang / crash if there are small angles
  in the input geometry.
* In cases where Triangle previously hung (displaying an error message)
  when an error occured, the program now terminates and indicates that
  an error has occured.  This is especially useful for Matlab/Octave
  scripts, which now get passed the error indication rather than having
  the script hang on the meshing error. 
* Fixed a number of additional instances of focus-stealing during script
  execution.  This makes it possible to do other things while FEMM is
  running a long (Matlab/Octave or Mathematica) script.
* Added a new optional parameter to openfemm in Matlab/Octave/Mathematica
  and Scilab scripting.  openfemm(1) starts FEMM fully minimized.  
* Calls to the analysis (mi_analyze, etc.) assume the state of the main 
  window.  For example, if a scripting run is started minimized with 
  openfemm(1), subsequent mi_analyze() calls will also be run
  minimized automatically.
* Fixed SciFEMM interface so that it works with Scilab 6.0.0

24Sep2017: (Test Build)

* Fixed mi_attachdefault, ei_attachdefault, hi_attachdefault, and
  ci_attachdefault functions, which didn't work correctly.

23Jun2016: (Test Build)

* Fixed sign error with off-diagonal term in permeability tensor for
  incremental permeability problems.

14Jun2016: (Test Build)

* Added a "frozen permeability" problem type that can be used to split up 
  field contributions between various excitation sources for nonlinear DC
  problems.  Also extends the "incremental permeability" formulation to DC
  problems.  The DC incremental results can be used as part of a general 
  time-transient solver, essentially providing the derivatives of flux
  linkage with respect to incremental changes in current and with 
  respect to small changes in position / orientation of moving parts.
* Dropped support for versions of Mathematica before version 5.0.  Versions
  prior to 5.0 do not include .NET/Link for interprocess communications,

12Jan2016:

* Fixed bug added in 01Nov2015 that messed up the reported permeability and
  field intensity for nonlinear time harmonic problems.

01Nov2015:

* Adds "incremental permeability" AC solver. An example application for
  this functionality is the analysis of the frequency-dependent impedance
  of speaker drivers about the DC operating point established by the 
  speaker's permanent magnet.
  - DC operating point specified as the "Previous Solution" in the problem
    definition or via the mi_setprevious(filename) command in Lua.
* Adds 10% and 15% Copper Clad Aluminum magnet wire material types. The
  material definition is intended to provide accurate estimates of proximity
  and skin effect losses across a wide range of frequencies with a bulk
  wire model (i.e. not every turn has to be individually modeled).
* Changes to some of the issues pointed out over the last couple years, e.g.
  - Change to InTriangle test to fix the issues that could occur if a 
    specified point is exactly on the line between two elements
  - Lua command to programmatically turn off "smart mesh" with the 
    smartmesh(state) Lua command.  State is 0 for no "smart mesh" and 1
	for "smart mesh".  Function has the same name in the Matlab interface
	and is named SmartMesh in the Mathematica implementation
  - Fixed bug where "mo_zoom", "eo_zoom", etc. didn't work right
  - Fixed an issue where the right energy / flux linkage was not reported
    for wound coils in AC magnetic problems if the frequency is very 
	small (e.g. <1μHz)
* For increased compatibility with Mathematica 10, the Mathematica interface
  has been changed to use .NET/Link when it is available (instead of
  MathLink).  .NET/Link invokes FEMM as an out-of-proc ActiveX server 
  (the same way that FEMM communicates with Matlab).

15Nov2013:

* Changed the way that errors are trapped in Matlab/Octave and Scilab
  implementations so that errors that would normally display as message
  boxes in a normal GUI session instead get returned as errors to
  Matlab/Octave or Scilab.  Errors can then be trapped, e.g. by using a
  try/catch block.

07Nov2013:

* Fixed instances of GetWindowLong and SetWindowLong which caused the x64
  build to crash when running on Linux via Wine.
* Fixes to eo_blockintegral and co_blockintegral functions. Previously wouldn't 
  allow integration (e.g. for Weighted Stress Tensor force) if the only selected 
  area was a conductor surface.

25Aug2013:

* Changed IABCs to support either a Dirichlet or Neumann outer edge.  This
  is useful for electrostatic problems.

04Aug2013:

* Added mi_selectcircle, mi_selectrectangle and friends to programmatically
  select regions.
* Changed .dxf import so that objects assigned layers are imported as
  grouped being in the same group.
* Added new "Improvised Asymptotic Boundary Condition" button and mi_makeABC
  and friends Lua funtions as an alternate way of solving unbounded
  problems.

11Apr2012:

* Can turn off "smart meshing" via a Preferences selection on the "General Attributes"
  tab by unchecking "use smart meshing"
* Fixed a newly introduced bug where an erroneous resistive loss is computed for AC
  problems in regions where conductivity = 0
* Fixed mi_readdxf problem described as Bug 18 in the bug tracker.

01Oct2011:

* Fixed error in reported flux linkage. Flux linkage for stranded regions 
  carrying zero current is not reported correctly for AC problems.
* The Lua "format" command did not work properly with complex number--it 
  stripped off the imaginary part of the number.  This is now fixed.
* The units reported for some heat flow block integral results were erroneous. 
  This has now been rectified.
* 64-bit version of FEMM 4.2 is now available.
* FEMM has been modified to allow multiple instance of FEMM to run at the 
  same time via ActiveX.  For example, This allows multiple instance of FEMM
  to be controlled by one instance of Matlab or Octave.
* FEMM 42 09Nov2010 asks for Mathematica integration when using the silent 
  install method. The installer script has been modified so that the silent
  install assumes that Mathematica is not available, letting the installation
  complete without requiring operator intervention.
* Default material.  A feature has been added which allows one block label to
  be designated as the default block label. Any unlabeled blocks are then
  assumed to be tagged by the default block label.
* In the current flow problem type, line plots of quantities normal and
  tangential to a user-define contour were messed up because the normal
  and tangential directions were computed incorrectly.  This is now fixed.
* The "default" mesh size has been changed.  In previous builds, using the
  default mesh size nearly always resulted in a mesh that was too coarse to
  give accurate results. The default mesh size has been changed so that
  specifying the default mesh size is adequate for most applications.
  Note: can use the <F3> and <F4> keys to uniformly refine and coarsen the
  mesh for the entire model with one keystroke.
* Added automatic refinement of the mesh near corners.  This refinement 
  improves convergence of results like force, stored energy, etc.
* Changed the way that the maximum flux density is computed for flux density
  plotting purposes.  With the automatic refinement of corners, small elements
  with high flux densities can appear in corners. The modified algorithm
  discounts these small corner elements when picking a maximum for the purposes
  of picking plot contours.
* Changed the key that is used to break out of Lua scripts to ESC from BREAK.
  Many keyboards don't have a BREAK key anymore, so it made sense to make this
  change.
* Changed the selection rectangle to a dotted line so that it would 
  render faster.
* Modified the DXF import to understand closed POLYLINE entities.  Previously,
  only open POLYLINE entities were supported.
* Fixed problem with functionality that creates rounded corners (i.e.
  the functionality invoked by mi_createradius) where the program would not
  allow a radius to be created if the intersection was between a line
  segment and an arc segment if the line segment laid along a ling that
  passed through the center of the circle associated with the arc segment.
* Default install directory changed to c:\femm42 to avoid directory
  permissions problems in Windows 7.
  
09Nov2010:

* Added a set of Scilab functions for interfacing with FEMM. These
  functions were tested using Scilab 5.2.2. The descriptions of these
  functions are identical to those described in the OctaveFEMM
  documentation. Example Scilab scripts are in the femm42/examples directory
  and have a .sce file extension and can be run by typing:
  exec('examplename.sce',-1)
  at the Scilab commandline. If you did not install FEMM to the default
  c:\Program Files\femm42 directory, you'll need to change the first line
  of the *.sce files that loads the FEMM library so that it points to the
  correct library location.

11Oct2010:

* Fixed bug in values of |H| reported in the Output Window for time harmonic
  magnetic problems.
* Fixed bug where in some plots, units of H given as A/m^2 instead of A/m
* Fixed error in mo_showvectorplot Matlab/Octave function.  Also fixed
  similar errors in co_showvectorplot, ho_showvectorplot, eo_showvectorplot
* Fixed messed-up definitions of the Lua functions ei_defineouterspace,
  ei_attachouterspace, and ei_detachouterspace
* Installer now prompts for whether or not Mathetmatica support is to be
  included. If Mathematica support is selected, a version of FEMM is
  installed that assumes the availability of ML32I2.DLL, a DLL installed by
  Mathematica. Otherwise, a version of FEMM is installed that doesn't need
  the Mathlink DLL.
* Re-wrote the GetIntersection routine that finds intersections between two
  line segments.  In some uncommon circumstances, the routine could create 
  extra points when the geometry was moved or rotated.
* Added extra Lua functions mi_getmaterial, ei_getmaterial, hi_getmaterial,
  and ci_getmaterial to fetch material definitions from the materials
  library on disk.  Analogous functions were also added to the Matlab/Octave
  and Mathematica interfaces.

02Nov2009:

* Added the Lua commands mi_setgroup, ei_setgroup, hi_setgroup, ci_setgroup
  that assign all selected items to the group number specified by the
  argument to the function.
* Fixed a bug that caused an incorrect permeability to be reported for
  nonlinear materials at points where the flux density is less than 10nT.
* Fixed bugs with ci_addconductor and ci_modifyconductor Lua functions.
* Fixed bug with CIAddMaterial function in MathFEMM

15Jul2009: Several minor changes have been made versus the 01Apr2009 release:

* Added the following Lua commands that allow direct access to finite
  element mesh information:
  mo_numnodes, mo_numelements, mo_getnode, mo_getelement,
  eo_numnodes, eo_numelements, eo_getnode, eo_getelement,
  ho_numnodes, ho_numelements, ho_getnode, ho_getelement,
  co_numnodes, co_numelements, co_getnode, co_getelement.
  There are Matlab/Octave and Mathematica analogs of these commands, too.
* Made a few more performance tweaks to the Mathematica interface.
* Fixed bug in computation of heat flux passing through a constant
  temperature-type "conductor property"
* Included a new selection of soft magnetic materials in the magnetic 
  materials library. The BH curves for these materials were obtained by
  digitizing the curves picutured in Figure 17, "Direct current 
  magnetization curves for various magnetic materials", Metals Handbook,
  8th ed, Vol. 1, p. 792.  These curves represent a wide variety of
  materials, and the curves are defined to very high flux levels at
  which all materials are fully saturated.

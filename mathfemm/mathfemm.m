If[Length[Contexts["MathFEMM`"]] >0,
        Print["MathFEMM is already loaded."];
        Abort[]
]

BeginPackage["MathFEMM`"]

MIMakeABC::usage =
"MIMakeABC[n,R,x,y] creates a series of circular shells that emulate the
impedance of an unbounded domain (i.e. an Inprovised Asymptotic Boundary
Condition).  The n parameter contains the number of shells to be used
(should be between 1 and 10), R is the radius of the solution domain, and
(x,y) denotes the center of the solution domain.  The bc parameter should
be specified as 0 for a Dirichlet outer edge or 1 for a Neumann outer edge.
If the function is called without all the parameters, the function makes up 
reasonable values for the missing parameters."

EIMakeABC::usage =
"EIMakeABC[n,R,x,y,bc] creates a series of circular shells that emulate the
impedance of an unbounded domain (i.e. an Inprovised Asymptotic Boundary
Condition).  The n parameter contains the number of shells to be used
(should be between 1 and 10), R is the radius of the solution domain, and
(x,y) denotes the center of the solution domain.  The bc parameter should
be specified as 0 for a Dirichlet outer edge or 1 for a Neumann outer edge.
If the function is called without all the parameters, the function makes up 
reasonable values for the missing parameters."

HIMakeABC::usage =
"HIMakeABC[n,R,x,y,bc] creates a series of circular shells that emulate the
impedance of an unbounded domain (i.e. an Inprovised Asymptotic Boundary
Condition).  The n parameter contains the number of shells to be used
(should be between 1 and 10), R is the radius of the solution domain, and
(x,y) denotes the center of the solution domain.  The bc parameter should
be specified as 0 for a Dirichlet outer edge or 1 for a Neumann outer edge.
If the function is called without all the parameters, the function makes up 
reasonable values for the missing parameters."

CIMakeABC::usage =
"CIMakeABC[n,R,x,y,bc] creates a series of circular shells that emulate the
impedance of an unbounded domain (i.e. an Inprovised Asymptotic Boundary
Condition).  The n parameter contains the number of shells to be used
(should be between 1 and 10), R is the radius of the solution domain, and
(x,y) denotes the center of the solution domain.  The bc parameter should
be specified as 0 for a Dirichlet outer edge or 1 for a Neumann outer edge.
If the function is called without all the parameters, the function makes up 
reasonable values for the missing parameters."


MISelectCircle::usage = 
"MISelectCircle[x,y,R,editmode] selects objects within a circle of radius
R centered at {x,y}.  If only x, y, and R paramters are given, the current
edit mode is used.  If the editmode parameter is used, 0 denotes nodes, 2
denotes block labels, 2 denotes segments, 3 denotes arcs, and 4 specifies
that all entity types are to be selected."

EISelectCircle::usage = 
"EISelectCircle[x,y,R,editmode] selects objects within a circle of radius
R centered at {x,y}.  If only x, y, and R paramters are given, the current
edit mode is used.  If the editmode parameter is used, 0 denotes nodes, 2
denotes block labels, 2 denotes segments, 3 denotes arcs, and 4 specifies
that all entity types are to be selected."

CISelectCircle::usage = 
"CISelectCircle[x,y,R,editmode] selects objects within a circle of radius
R centered at {x,y}.  If only x, y, and R paramters are given, the current
edit mode is used.  If the editmode parameter is used, 0 denotes nodes, 2
denotes block labels, 2 denotes segments, 3 denotes arcs, and 4 specifies
that all entity types are to be selected."

HISelectCircle::usage = 
"HISelectCircle[x,y,R,editmode] selects objects within a circle of radius
R centered at {x,y}.  If only x, y, and R paramters are given, the current
edit mode is used.  If the editmode parameter is used, 0 denotes nodes, 2
denotes block labels, 2 denotes segments, 3 denotes arcs, and 4 specifies
that all entity types are to be selected."

MISelectRectangle::usage = 
"MISelectRectangle[x1,y1,x2,y2,editmode] selects objects within a rectangle
defined by points {x1,y1} and {x2,y2}. If no editmode parameter is supplied,
the current edit mode is used.  If the editmode parameter is used, 0 denotes
nodes, 2 denotes block labels, 2 denotes segments, 3 denotes arcs, and 4 
specifies that all entity types are to be selected."

EISelectRectangle::usage = 
"EISelectRectangle[x1,y1,x2,y2,editmode] selects objects within a rectangle
defined by points {x1,y1} and {x2,y2}. If no editmode parameter is supplied,
the current edit mode is used.  If the editmode parameter is used, 0 denotes
nodes, 2 denotes block labels, 2 denotes segments, 3 denotes arcs, and 4 
specifies that all entity types are to be selected."

CISelectRectangle::usage = 
"CISelectRectangle[x1,y1,x2,y2,editmode] selects objects within a rectangle
defined by points {x1,y1} and {x2,y2}. If no editmode parameter is supplied,
the current edit mode is used.  If the editmode parameter is used, 0 denotes
nodes, 2 denotes block labels, 2 denotes segments, 3 denotes arcs, and 4 
specifies that all entity types are to be selected."

HISelectRectangle::usage = 
"HISelectRectangle[x1,y1,x2,y2,editmode] selects objects within a rectangle
defined by points {x1,y1} and {x2,y2}. If no editmode parameter is supplied,
the current edit mode is used.  If the editmode parameter is used, 0 denotes
nodes, 2 denotes block labels, 2 denotes segments, 3 denotes arcs, and 4 
specifies that all entity types are to be selected."

CIGetMaterial::usage =
"CIGetMaterial[\"materialname\" fetches the specified material from the
materials library."

EIGetMaterial::usage =
"EIGetMaterial[\"materialname\" fetches the specified material from the
materials library."

HIGetMaterial::usage =
"HIGetMaterial[\"materialname\" fetches the specified material from the
materials library."

MIGetMaterial::usage =
"MIGetMaterial[\"materialname\" fetches the specified material from the
materials library."

(********** Current Flow Input Functions **********************)
CIAddArc::usage =
"CIAddArc[x1,y1,x2,y2,angle,maxseg] adds an arc to the electrostatics
input geometry from the point nearst to {x1,y1} to the point nearest
to {x2,y2}.  The arc spans a number of degrees specified by angle. Since
FEMM approximates arcs by many line segments, the parameter maxseg
specifies the maximum number of degrees that is allowed to be spanned
by any one segment. Equivalent forms are:\n
CIAddArc[{x1,y1},{x2,y2},angle,maxseg]\n
CIAddArc[{{x1,y1},{x2,y2}},angle,maxseg]"

CIAddBlockLabel::usage = "CIAddBlockLabel[x,y] adds a block label at the
point {x,y}.  An equivalent form is:\n
CIAddBlockLabel[{x,y}]"

CIAddBoundProp::usage =
"CIAddBoundProp[\"boundpropname\", Vs, qs, c0, c1, bdryformat] adds a new boundary
property with name \"boundpropname\"\n
For a \"Fixed Voltage\" type boundary condition, set the
Vs parameter to the desired voltage and all other
parameters to zero. \n
To obtain a \"Mixed\"  type boundary condition, set \texttt{C1} and
c0 as required and bdryformat to 1. Set all other parameters to zero.\n
To obtain a prescribes surface current density, set qs to
the desired current density in A/m^2 and set bdryformat to 2.\n
For a \"Periodic\" boundary condition, set bdryformat to 3
and set all other parameters to zero.\n
For an \"Anti-Perodic\" boundary condition, set bdryformat
to 4 set all other parameters to zero."

CIAddConductorProp::usage =
"CIAddConductorProp[\"conductorname\",vc,qc,conductortype] adds a new
conductor property with name \"conductorname\" with either a prescribed
voltage or a prescribed total current. Set the unused property to zero. The
conductortype parameter is 0 for prescribed charge and 1 for prescribed
voltage."

CIAddMaterial::usage =
"CIAddMaterial[\"materialname\",ox,oy,ex,ey,ltx,lty] adds a new material with called
\"materialname\" with the material properties:\n
ox Electrical conductivity in the x- or r-direction.\n
oy Electrical conductivity in the y- or z-direction.\n
ex Relative electrical permittivity in the x- or r-direction.\n
ey Relative electrical permittivity in the y- or z-direction.\n
ltx Dielectric loss tangent in the x- or r-direction.\n
lty Dielectric loss tangent in the y- or z-direction."

CIAddNode::usage =
"CIAddNode[x,y] adds a new node at {x,y}. An equivalent form is:\n
CIAddNode[{x,y}]"

CIAddPointProp::usage =
"CIAddPointProp[\"pointpropname\",vp,qp] adds a new point property of name
\"pointpropname\" with either a specified voltage vp or point heat generation qp in units of J/m."

CIAddSegment::usage =
"CIAddSegment[x1,y1,x2,y2] add a new line segment from node closest to
{x1,y1} to node closest to {x2,y2}.  Equivalent forms are:\n
CIAddSegment[{x1,y1},{x2,y2}]\n
CIAddSegment[{{x1,y1},{x2,y2}}]"

CIAnalyze::usage =
"CIAnalyze[(flag)] runs the current solver. The flag parameter controls
whether the solver window is visible or minimized. For a visible window, either
specify no value for flag or specify 0. For a minimized window, flag should be
set to 1. An equivalent form is:\n
CIAnalyse[(flag)]"

CIAnalyse::usage =
"CIAnalyse[(flag)] runs the current solver. The flag parameter controls
whether the solver window is visible or minimized. For a visible window, either
specify no value for flag or specify 0. For a minimized window, flag should be
set to 1. An equivalent form is:\n
CIAnalyze[(flag)]"

CIClearSelected::usage =
"CIClearSelected[] clear all selected nodes, blocks, segments and arc segments."

CIClose::usage =
"CIClose[] closes the preprocessor window and destroys the current document."

CICopyRotate::usage =
"CICopyRotate[bx,by,angle,copies,(editaction)]\n
bx, by base point for rotation\n
angle angle by which the selected objects are incrementally shifted
to make each copy. This angle is measured in degrees.\n
copies number of copies to be produced from the selected objects \n
editaction 0-nodes, 1-segments, 2-block labels, 3-arcs, 4-group\n
An equivalent form is:\n
CICopyRotate[{bx,by},angle,copies,(editaction)]"

CICopyTranslate::usage =
"CICopyTranslate[dx,dy,copies,(editaction)]\n
{dx,dy} represents the distance by which the selected objects are to
be incrementally shifted.\n
copies specifies the number of copoies to be produced from the selected
objects\n
editaction 0-nodes, 1-segments, 2-block labels, 3-arcs, 4-group\n
An equivalent form is:\n
CICopyTranslate[{dx,dy},copies,(editaction)]"

CICreateMesh::usage =
"CICreateMesh[] runs triangle to create a mesh. Note that this is not a
necessary precursor of performing an analysis, as CIAnalyze[] will make
sure the mesh is up to date before running an analysis. Returns the number
of elements in the mesh."

CIDeleteBoundProp::usage =
"CIDeleteBoundProp[\"boundpropname\"] deletes the boundary property
named \"boundpropname\"."

CIDeleteConductor::usage =
"CIDeleteConductor[\"conductorname\"] deletes the conductor property
named \"conductorname\"."

CIDeleteMaterial::usage =
"CIDeleteMaterial[\"materialname\"] deletes the material property
named \"materialname\"."

CIDeletePointProp::usage =
"CIDeletePointProp[\"pointpropname\"] deletes the point property
named \"pointpropname\"."

CIDeleteSelected::usage =
"CIDeleteSelected[] deletes all selected objects."

CIDeleteSelectedArcSegments::usage =
"CIDeleteSelectedArcSegments[] deletes all selected arc segments."

CIDeleteSelectedLabels::usage =
"CIDeleteSelectedLabels[] deletes all selected block labels."

CIDeleteSelectedNodes::usage =
"CIDeleteSelectedNodes[] deletes all selected nodes."

CIDeleteSelectedSegments::usage =
"CIDeleteSelectedSegments[] deletes all selected segments."

CIDrawArc::usage =
"CIDrawArc[x1,y1,x2,y2,angle,maxseg] adds an arc to the current
input geometry by drawing points at {x1,y1} and {x2,y2} and then connecting
them with an arc segment. The arc spans a number of degrees specified by
angle. Since FEMM approximates arcs by many line segments, the parameter
maxseg specifies the maximum number of degrees that is allowed to be spanned
by any one segment. Equivalent forms are:\n
CIDrawArc[{x1,y1},{x2,y2},angle,maxseg]\n
CIDrawArc[{{x1,y1},{x2,y2}},angle,maxseg]"

CIDrawLine::usage =
"CIDrawLine[x1,y1,x2,y2] adds points at {x1,y1} and {x2,y2} and then adds
a segment connecting these two points.  Equivalent forms are:\n
CIDrawLine[{x1,y1},{x2,y2}]\n
CIDrawLine[{{x1,y1},{x2,y2}}]"

CIDrawPolygon::usage =
"CIDrawPolygon[{{x1,y2},...,{xn,yn}}] adds new node points at every listed
point and then draws a closed figure that connects the points"

CIDrawPolyLine::usage =
"CIDrawPolyLine[{{x1,y2},...,{xn,yn}}] draws a multi-segment line by adding
each of the points in the list and then adding segments that join the listed
points."

CIDrawRectangle::usage =
"CIDrawRectangle[x1,y1,x2,y2] adds nodes at {x1,y1}, {x1,y2}, {x2,y2} and
 {x2,y1} and joins them with new segments.  Equivalent forms are:\n
CIDrawRectangle[{x1,y1},{x2,y2}]\n
CIDrawRectangle[{{x1,y1},{x2,y2}}]"

CIGetView::usage =
"CIGetView[] grabs the current current input view and returns a
bitmapped graphics object.  This object can subsequently be displayed using
the Show[] command"

CIHideGrid::usage =
"CIHideGrid[] hides the current input grid points"

CIHideNames::usage =
"CIHideNames[] stops the names of the materials
associated with each block label from being displayed"

CILoadSolution::usage =
"CILoadSolution[] loads and displays the solution corresponding to the
current geometry."

CIMirror::usage =
"CIMirror[x1,y1,x2,y2,(editaction)] mirrors the selected objects about a line
passing through the points (x1,y1) and (x2,y2). Valid editaction entries are
0 for nodes, 1 for lines (segments), 2 for block labels, 3 for arc segments,
and 4 for groups. Equivalent forms are:\n
CIMirror[{x1,y1},{x2,y2},(editaction)]\n
CIMirror[{{x1,y1},{x2,y2}},(editaction)]"

CIModifyBoundProp::usage =
"CIModifyBoundProp[\"BdryName\",propnum,value] allows for modification
of a boundary property. The BC to be modified is specified by \"BdryName\".
The next parameter is the number of the property to be set. The last number
is the value to be applied to the specified property. The various properties
that can be modified are listed below:\n
0 - \"BdryName\" - Name of boundary property\n
1 - Vs - Fixed Voltage\n
2 - js - Prescribed current density\n
3 - c0 - External voltage\n
4 - c1 - heat transfer coefficient\n
5 - BdryFormat -  Type of boundary condition (0 = Prescribed Voltage; 1 = Mixed; 2 = Surface Current Density; 3 = Periodic; 4 = Antiperiodic)"

CIModifyConductorProp::usage =
"CIModifyConductorProp[\"ConductorName\",propnum,value] allows for
modification of a conductor property. The conductor property to be modified is
specified by \"ConductorName\". The next parameter is the number of the property
to be set. The last number is the value to be applied to the specified
property. The various properties that can be modified are listed below:\n
0 - \"ConductorName\" - Name of the conductor property\n
1 - Vc - Conductor Voltage\n
2 - jc - Total conductor current\n
3 - ConductorType - 0 = Prescribed current, 1 = Prescribed voltage"

CIModifyMaterial::usage =
"CIModifyMaterial[\"BlockName\",propnum,value] allows for modification
of a material's properties without redefining the entire material (e.g. so
that current can be modified from run to run). The material to be modified
is specified by \"BlockName\". The next parameter is the number of the property
to be set. The last number is the value to be applied to the specified property.
The various properties that can be modified are listed below:\n
0 - \"BlockName\" - Name of the material\n
1 - ox - x (or r) direction electrical conductivity\n
2 - oy - y (or z) direction electrical conductivity\n
1 - ox - x (or r) direction relative electrical permittivity\n
2 - oy - y (or z) direction relative electrical permittivity\n
1 - ox - x (or r) direction dielectric loss tangent\n
2 - oy - y (or z) direction dielectric loss tangent"

CIModifyPointProp::usage =
"CIModifyPointProp[\"PointName\",propnum,value] allows for modification
of a point property. The point property to be modified is specified by
\"PointName\". The next parameter is the number of the property to be set.
The last number is the value to be applied to the specified property. The
various properties that can be modified are listed below:\n
0 - PointName - Name of the point property\n
1 - Vp - Prescribed nodal voltage\n
2 - jp - Point current in A/m"

CIMoveRotate::usage =
"CIMoveRotate[bx,by,shiftangle,(editaction)]\n
bx, by – base point for rotation\n
shiftangle – angle in degrees by which the selected objects are
rotated.\n
editaction - 0  –nodes, 1 – lines (segments), 2 –block labels, 3 – arc segments, 4- group\n
An equivalent form is:\n
CIMoveRotate[{bx,by},shiftangle,(editaction)]"

CIMoveTranslate::usage =
"CIMoveTranslate[dx,dy,(editaction)]\n
dx,dy – distance by which the selected objects are shifted.\n
editaction - 0 –nodes, 1 – lines (segments), 2 –block labels, 3 – arc segments, 4- group\n
An equivalent form is:\n
CIMoveTranslate[{dx,dy},(editaction)]"

CIProbDef::usage =
"CIProbDef[units,type,frequency,precision,depth,minangle] changes the problem definition. The units
parameter specifies the units used for measuring length in the problem domain. Valid \"units\"
entries are \"inches\", \"millimeters\", \"centimeters\", \"mils\", \"meters\", and \"micrometers\".
Set problemtype to \"planar\" for a 2-D planar problem, or to \"axi\" for an axisymmetric
problem. The frequency parameter denotes the frequency of the analysis in Hz.
The precision parameter dictates the precision required by the solver. For example,
specifying 1.E-8 requires the RMS of the residual to be less than 10^(-8). The depth parameter,
represents the depth of the problem in the into-the-page direction for 2-D planar problems. The
minangle parameter is a constraint for the mesh generator. It specifies the smallest permissible
angle in triangles that compose the finite element mesh. A good value to choose is 30 degrees, but
smaller values may be needed for \"tough\" geometries that contain small angles."

CIPurgeMesh::usage = "CIPurgeMesh[] clears the mesh out of both the screen and memory."

CIReadDXF::usage = "CIReadDXF[\"filename\"] reads in geometry information a DXF file specified by \"filename\""

CISaveDXF::usage = "CISaveDXF[\"filename\"] saves geometry information in a DXF file specified by \"filename\""

CIRefreshView::usage = "CIRefreshView[] Redraws the current view."

CISaveAs::usage = "CISaveAs[\"filename\"] saves the file with name \"filename\". Note if you use a path you
must use two backslashes e.g. \"c:\\\\temp\\\\myfemmfile.fem\""

CISaveBitmap::usage = "CISaveBitmap[\"filename\"] saves a bitmapped screenshot of the current view to the file
specified by \"filename\", subject to the printf-type formatting explained previously for the CISaveAs command."

CISaveMetafile::usage = "CISaveMetafile[\"filename\"] saves an extended metafile screenshot of the current view to the file
specified by \"filename\", subject to the printf-type formatting explained previously for the CISaveAs command."

CIScale::usage = "CIScale[bx,by,scalefactor,(editaction)]\n
bx, by – base point for scaling\n
scalefactor – a multiplier that determines how much the selected objects are
scaled\n
editaction 0 –nodes, 1 – lines (segments), 2 –block labels, 3 – arc segments, 4- group\n
An equivalent for is:
CIScale[{bx,by},scalefactor,(editaction)]"

CISelectArcSegment::usage = "CISelectArcSegment[x,y] selects the arc segment closest to {x,y}.  An equivalent form is:\n
CISelectArcSegment[{x,y}]"

CISelectGroup::usage = "CISelectGroup[n] selects the nth group of nodes, segments, arc segments and block labels.
This function will clear all previously selected elements and leave the edit mode in 4 (group)"

CISelectLabel::usage = "CISelectLabel[x,y] selects the block label closest to {x,y}.  An equivalent form is:\n
CISelectLabel[{x,y}]"

CISelectNode::usage = "CISelectNode[x,y] selects the node closest to {x,y}.  An equivalent form is:\n
CISelectNode[{x,y}]"

CISelectSegment::usage = "CISelectSegment[x,y] selects the segment closest to {x,y}.  An equivalent form is:\n
CISelectSegment[{x,y}]"

CISetArcSegmentProp::usage =
"CISetArcSegmentProp[maxsegdeg, \"propname\", hide, groupno,\"inconductor\"]
sets the properties associated with the selected arc segments\n
maxsegdeg specifies that the arcs must be meshed with elements that span at
most maxsegdeg degrees per element\n
\"propname\" specifies the boundary property to be associated with the
selected arcs\n
hide: 0 = not hidden in post-processor, 1 == hidden in post processor\n
groupno is an integer specifying the group number of which the selected arcs
are to be members.\n
\"inconductor\" specifies the name of the conductor property with which the
selected arcs are to be associated. If the arcs is not to be part of a
conductor, this parameter can be specified as \"<None>\"."

CISetBlockProp::usage = "CISetBlockProp[\"blockname\",automesh,meshsize,groupno] sets the selected block
labels to have the properties:\n
Block property \"blockname\".\n
automesh: 0 = mesher defers to mesh size constraint defined in meshsize, 1 = mesher
automatically chooses the mesh density.\n
meshsize: size constraint on the mesh in the block marked by this label\n.
groupno: make selected members of specified group number"

CISetEditMode::usage =
"CISetEditMode[\"editmode\"] sets the current editmode to:\n
\"nodes\" – nodes\n
\"segments\" - line segments\n
\"arcsegments\" - arc segments\n
\"blocks\" - block labels\n
\"group\" - selected group\n
This command will affect all subsequent uses of the other editing commands, if they are used
without the editaction parameter."

CISetFocus::usage = "CISetFocus[\"documentname\"] switches the current input file upon
which scripting commands are to act. If more than one current input file is being edited
at a time, this command can be used to switch between files so that the mutiple files can be
operated upon programmatically . \"documentname\" should contain the name of the desired document
as it appears on the window’s title bar."

CISetGrid::usage = "CISetGrid[density,\"type\"] changes the grid spacing. The density parameter specifies
the space between grid points, and the \"type\" parameter is set to \"cart\" for Cartesian coordinates
or \"polar\" for polar coordinates."

CISetNodeProp::usage = "CISetNodeProp[\"propname\",groupno,\"inconductor\"] sets the selected nodes to have
the nodal property \"propname\" and group number groupno. The \"inconductor\" string
specifies which conductor the node belongs to. If the node doesn’t belong to a named conductor,
this parameter can be set to \"<None>\"."

CISetSegmentProp::usage =
"CISetSegmentProp[\"propname\",elementsize,automesh,hide,groupno,\"inconductor\"]
sets the select segments to have:\n
Boundary property \"propname\"\n
Local element size along segment no greater than elementsize\n
automesh: 0 = mesher defers to the element constraint defined by elementsize, 1 = mesher
automatically chooses mesh size along the selected segments\n
hide: 0 = not hidden in post-processor, 1 == hidden in post
processor\n
A member of group number groupno\n
A member of the conductor specified by the string \"inconductor\". If the segment is not
part of a conductor, this parameter can be specified as \"<None>\"."

CIShowGrid::usage = "CIShowGrid[] displays the grid points"

CIShowMesh::usage = "CIShowMesh[] displays the mesh"

CIShowNames::usage = "CIShowNames[] displays the material names associated with each block label"

CISnapGridOff::usage = "CISnapGridOff[] turns off snap-to-grid"

CISnapGridOn::usage = "CISnapGridOn[] turns on snap-to-grid"

CIZoom::usage = "CIZoom[x1,y1,x2,y2] Set the display area to be from the bottom left corner specified by
{x1,y1} to the top right corner specified by {x2,y2}. Equivalent forms are:\n
CIZoom[{x1,y1},{x2,y2}]\n
CIZoom[{{x1,y1},{x2,y2}}]"

CIZoomIn::usage = "CIZoomIn[] zooms out by a factor of 200%."

CIZoomNatural::usage = "CIZoomNatural[] zooms to a \"natural\" view with sensible extents."

CIZoomOut::usage = "CIZoomOut[] zooms out by a factor of 50%."

CIDefineOuterSpace::usage = "CIDefineOuterSpace[Zo,Ro,Ri] defines
an axisymmetric external region to be used in conjuction with the
Kelvin Transformation method of modeling unbounded problems.  The
Zo parameter is the z-location of the origin of the outer region,
the Ro parameter is the radius of the outer region, and the Ri
parameter is the radius of the inner region (i.e. the region of
interest). In the exterior region, the permeability varies as a function of
distance from the origin of the external region.  These parameters
are necessary to define the permeability variation in the external
region."

CIAttachOuterSpace::usage = "CIAttachOuterSpace[] marks all
selected block labels as members of the external region used for
modeling unbounded axisymmetric problems via the Kelvin
Transformation."

CIDetachOuterSpace::usage = "CIDetachOuterSpace[] undefines all
selected block labels as members of the external region used for
modeling unbounded axisymmetric problems via the Kelvin
Transformation."

CIAttachDefault::usage = "CIAttachDefault[] marks the
selected block label as the default block label.  This block label
is applied to any region that has not been explicitly labeled."

CIDetachDefault::usage = "CIDetachDefault[] undefines the default
attribute for the selected block label."

HIAttachDefault::usage = "HIAttachDefault[] marks the
selected block label as the default block label.  This block label
is applied to any region that has not been explicitly labeled."

HIDetachDefault::usage = "HIDetachDefault[] undefines the default
attribute for the selected block label."

EIAttachDefault::usage = "EIAttachDefault[] marks the
selected block label as the default block label.  This block label
is applied to any region that has not been explicitly labeled."

EIDetachDefault::usage = "EIDetachDefault[] undefines the default
attribute for the selected block label."

MIAttachDefault::usage = "MIAttachDefault[] marks the
selected block label as the default block label.  This block label
is applied to any region that has not been explicitly labeled."

MIDetachDefault::usage = "MIDetachDefault[] undefines the default
attribute for the selected block label."

CICreateRadius::usage = "CICreateRadius[x,y,z] turns a corner located at {x,y} into a curve of radius r.\n
An equivalent form is: CICreateRadius[{x,y},z]"


(********** Current Flow Output Functions **********************)

COAddContour::usage =
"COAddContour[x,y] adds the point {x,y} to the contour that is used for plotting values along lines and
for computing line integrals.  An equivalent form is:\n
COAddContour[{x,y}]"

COBendContour::usage =
"COBendContour[angle,anglestep] replaces the straight line formed by the last two
points in the contour by an arc that spans angle degrees. The arc is actually composed
of many straight lines, each of which is constrained to span no more than anglestep degrees.
The angle parameter can take on values from -180 to 180 degrees. The anglestep
parameter must be greater than zero. If there are less than two points defined in the contour,
this command is ignored."

COBlockIntegral::usage =
"COBlockIntegral[type] calculate a block integral for the selected
blocks. The type parameter can take on the following values:\n
     0   Real Power \n
     1   Reactive Power \n
     2   Apparent Power \n
     3   Time-Average Stored Energy \n
     4   Block cross-section area \n
     5   Block volume \n
     6   x (or r) direction Weighted Stress Tensor force, DC component \n
     7   y (or z) direction Weighted Stress Tensor force, DC component \n
     8   x (or r) direction Weighted Stress Tensor force, 2x frequency component \n
     9   y (or z) direction Weighted Stress Tensor force, 2x frequency component \n
     10  Weighted Stress Tensor torque, DC component \n
     11  Weighted Stress Tensor torque, 2x frequency component \n
Returns either a single value or a list of two values, depending on the specified type"

COClearBlock::usage = "COClearBlock[] clears the current block selection"

COClearContour::usage = "COClearContour[] clears the current contour selection"

COClose::usage = "COClose[] close the current postprocessor document and window"

COGetConductorProperties::usage = "COGetConductorProperties[\"conductor\"] returns properties for the
conductor property named \"conductor\". A list with two values is returned: The voltage of the specified
conductor, and the total current from the specified conductor."

COGetJ::usage = "COGetJ[x,y] returns a list with two elements containing the current density
at point {x,y}.  An equivalent form is:\n
COGetJ[{x,y}]"

COGetK::usage = "COGetK[x,y] returns a list with two elements containing the ac conductivity
at point {x,y}.  An equivalent form is:\n
COGetK[{x,y}]"

COGetE::usage = "COGetE[x,y] returns a list with two elements containing the 
electric field intensity at point {x,y}. An equivalent form is:\n
COGetE[{x,y}]"

COGetPointValues::usage =
"COGetPointValues[x,y] returns the various field values associated with the point at {x,y}. The return values,
in order, are:\n
V   & Voltage \n
Jx  & x- or r- direction component of current density \n
Jy  & y- or z- direction component of current density \n
Kx  & x- or r- direction component of AC conductivity \n
Ky  & y- or z- direction component of AC conductivity \n
Ex  & x- or r- direction component of electric field intensity \n
Ey  & y- or z- direction component of electric field intensity \n
ex  & x- or r- direction component of permittivity \n
ey  & y- or z- direction component of permittivity \n
Jdx & x- or r- direction component of displacement current density \n
Jdy & y- or z- direction component of displacement current density \n
ox  & x- or r- direction component of permittivity \n
oy  & y- or z- direction component of permittivity \n
Jcx & x- or r- direction component of conduction current density \n
Jcy & y- or z- direction component of conduction current density \n
An equivalent form is: \n
COGetPointValues[{x,y}]"

COGetProblemInfo::usage =
"COGetProblemInfo[] returns a list with four values: the Problem
Type (0 for planar and 1 for axisymmetric); the depth assumed for planar problems
in units of meters; the frequency in Hz; and the length unit used to draw the problem, represented in meters."

COGetV::usage = "COGetV[x,y] returns the voltage at point {x,y}. An equivalent form is:\n COGetV[{x,y}]"

COGetView::usage =
"COGetView[] grabs the current output view and returns a
bitmapped graphics object.  This object can subsequently be displayed using
the Show[] command"

COSnapGridOff::usage = "COSnapGridOff[] turns off snap-to-grid"

COSnapGridOn::usage = "COSnapGridOn[] turns on snap-to-grid"

COHideContourPlot::usage =
"COHideContourPlot[] hides the currently displayed contour plot"

COHideDensityPlot::usage =
"COHideDensityPlot[] hides the currently displayed density plot"

COHideGrid::usage =
"COHideGrid[] hides the grid points from view in the current output window"

COHideMesh::usage =
"COHideMesh[] hides the finite element mesh from in the current output view"

COHideNames::usage =
"COHideNames[] hides material names associated with block labels from
view in the current output window"

COHidePoints::usage =
"COHidePoints[] inhibits the display of input node points in the current
output window"

COLineIntegral::usage =
"COLineIntegral[type] calculate the line integral for the defined
contour. Valid type selections include:\n
 0  E.t \n
 1  J.n \n
 2  Contour length \n
 3  Average voltage over contour \n
 4  Force from stress tensor \n
 5  Torque from stress tensor \n
This integral returns either one value or a list of two values,
depending on the type of integral"

COMakePlot::usage =
"COMakePlot[PlotType,NumPoints,Filename,FileFormat]\n
This function allows programmatic access to FEMM's X-Y plot routines.
If only PlotType or only PlotType and NumPoints are specified, the command
is interpreted as a request to plot the requested plot type to the screen.
If, in addition, the Filename parameter is specified, the plot is instead
written to disk to the specified file name as an extended metafile. If the
FileFormat parameter is also, the command is instead
interpreted as a command to write the data to disk to the specfied file
name, rather than display it to make a graphical plot. Valid entries for
PlotType are: \n
 0   V (Voltage) \n
 1   |J| (Magnitude of current density) \n
 2   J.n (Normal current density) \n
 3   J.t (Tangential current density) \n
 4   |E (Magnitude of field intensity) \n
 5   E.n (Normal field intensity) \n
 6   E.t (Tangential field intensity) \n
 7   |Jc| (Magnitude of conduction current density) \n
 8   Jc.n (Normal conduction current density) \n
 9   Jc.t (Tangential conduction current density)} \n
 10  |Jd| (Magnitude of displacement current density) \n
 11  Jd.n (Normal displacement current density) \n
 12  Jd.t (Tangential displacement current density)"

CORefreshView::usage = "CORefreshView[] Redraws the current view."

COReload::usage = "COReload[] reloads the current output file."

COSaveBitmap::usage = "COSaveBitmap[\"filename\"] saves a bitmapped screenshot of the current view to the file
specified by \"filename\", subject to the printf-type formatting explained previously for the CISaveAs command."

COSaveMetafile::usage = "COSaveMetafile[\"filename\"] saves an extended metafile screenshot of the current view to the file
specified by \"filename\", subject to the printf-type formatting explained previously for the CISaveAs command."

COSelectBlock::usage = "COSelectBlock[x,y] select the block that contains point {x,y}.  An equivalent form is:\n
COSelectBlock[{x,y}]"

COSelectPoint::usage = "COSelectPoint[x,y] adds a contour point at the closest input point to {x,y}. If the selected
point and a previous selected points lie at the ends of an arcsegment, a contour is added
that traces along the arcsegment. The selectpoint command has the same functionality as
the left-button-click contour point selection when the program is running in interactive mode.
An equivalent form is:\n
COSelectPoint[{x,y}]"

COGroupSelectBlock::usage =
"COGroupSelectBlock[n] selects all of the blocks that are labeled by block labels that are
members of group n. If no number is specified (i.e. COGroupSelectBlock[]), all blocks
are selected."

COSetEditMode::usage =
"COSetEditMode[\"mode\"] sets the mode of the postprocessor to point, contour, or area mode.
Valid entries for mode are \"point\", \"contour\", and \"area\"."

COSetGrid::usage =
"COSetGrid[density,\"type\"] changes the grid spacing. The density parameter specifies
the space between grid points, and the type parameter is set to \"cart\" for Cartesian coordinates
or \"polar\" for polar coordinates."

COShowContourPlot::usage =
"COShowContourPlot[numcontours,lowerV,upperV] shows the V contour plot with
options:\n
numcontours - Number of equipotential lines to be plotted.\n
upperV - Upper limit for contours.\n
lowerV - Lower limit for contours.\n
If numcontours is -1 all parameters are ignored and default values are used"

COShowDensityPlot::usage =
"COShowDensityPlot[legend,gscale,type,upper,lower]\n
Shows the flux density plot with options:\n
legend - Set to 0 to hide the plot legend or 1 to show the plot legend.\n
gscale - Set to 0 for a colour density plot or 1 for a grey scale density plot.\n
upper - Sets the upper display limit for the density plot.\n
lower - Sets the lower display limit for the density plot.\n
type - Sets the type of density plot. Plot types include:\n
0  |V|\n
1  |Re(V)|   \n
2  |Im(V)|   \n
3  |J|       \n
4  |Re(J)|    \n
5  |Im(J)|    \n
6  |E|       \n
7  |Re(E)|    \n
8  |Im(E)|"

COShowGrid::usage = "COShowGrid[] displays the grid points"

COShowMesh::usage = "COShowMesh[] displays the mesh"

COShowNames::usage = "COShowNames[] displays the material names associated with each block label"

COShowPoints::usage = "COShowPoints[] displays the input node point as part of the output geometry"

COSmoothOff::usage = "COSmoothOff[] turns off smoothing of the D and E fields.  The D and E fields are then
displayed using values that are piecewise constant over each element."

COSmoothOn::usage = "COSmoothOn[] turns on smoothing of the D and E fields.  The D and E fields are then
displayed using a linear interpolation of the field from nodal values"

COZoom::usage = "COZoom[x1,y1,x2,y2] Set the display area to be from the bottom left corner specified by
{x1,y1} to the top right corner specified by {x2,y2}. Equivalent forms are:\n
COZoom[{x1,y1},{x2,y2}]\n
COZoom[{{x1,y1},{x2,y2}}]"

COZoomIn::usage = "COZoomIn[] zooms out by a factor of 200%."

COZoomNatural::usage = "COZoomNatural[] zooms to a \"natural\" view with sensible extents."

COZoomOut::usage = "COZoomOut[] zooms out by a factor of 50%."


(********** Heat Flow Input Functions **********************)
HIAddArc::usage =
"HIAddArc[x1,y1,x2,y2,angle,maxseg] adds an arc to the electrostatics
input geometry from the point nearst to {x1,y1} to the point nearest
to {x2,y2}.  The arc spans a number of degrees specified by angle. Since
FEMM approximates arcs by many line segments, the parameter maxseg
specifies the maximum number of degrees that is allowed to be spanned
by any one segment. Equivalent forms are:\n
HIAddArc[{x1,y1},{x2,y2},angle,maxseg]\n
HIAddArc[{{x1,y1},{x2,y2}},angle,maxseg]"

HIAddBlockLabel::usage = "HIAddBlockLabel[x,y] adds a block label at the
point {x,y}.  An equivalent form is:\n
HIAddBlockLabel[{x,y}]"

HIAddBoundProp::usage =
"HIAddBoundProp[\"boundpropname\", BdryFormat, Tset, qs, Tinf, h, beta] adds a new boundary
property with name \"boundpropname\"\n
For a \"Fixed Temperature\" type boundary condition, set the Tset parameter to the
desired temperature and all other parameters to zero.\n
To obtain a \"Heat Flux\" type boundary condition, set qs to be the heat flux density and
BdryFormat to 1. Set all other parameters to zero.\n
To obtain a convection boundary condition, set h to the desired heat transfer coefficient and
Tinf to the desired external temperature and set BdryFormat to 2.\n
For a Radiation boundary condition, set beta equal to the desired emmissivity and Tinf to the 
desired external temperature and set BdryFormat to 3.\n
For a \"Periodic\" boundary condition, set BdryFormat to 4 and set all other
parameters to zero.\n
For an \"Anti-Perodic\" boundary condition, set BdryFormat to 5 set all other
parameters to zero."

HIAddConductorProp::usage =
"HIAddConductorProp[\"conductorname\",Tc,qc,conductortype] adds a new
conductor property with name \"conductorname\" with either a prescribed
temperature or a prescribed total heat flux. Set the unused property to zero. The
conductortype parameter is 0 for prescribed charge and 1 for prescribed
temperature."

HIAddMaterial::usage =
"HIAddMaterial[\"materialname\",Kx,Ky,qv] adds a new material with called
\"materialname\" with the material properties:\n
Kx Thermal conductivity in the x- or r-direction.\n
Ky Thermal conductivity in the y- or z-direction.\n
qv Volume heat generation density in units of W/m^3\n
Kt Volumetric heat capacity in units of MJ/(m^3*K)"

HIAddNode::usage =
"HIAddNode[x,y] adds a new node at {x,y}. An equivalent form is:\n
HIAddNode[{x,y}]"

HIAddPointProp::usage =
"HIAddPointProp[\"pointpropname\",Tp,qp] adds a new point property of name
\"pointpropname\" with either a specified temperature Tp or point heat generation qp in units of J/m."

HIAddSegment::usage =
"HIAddSegment[x1,y1,x2,y2] add a new line segment from node closest to
{x1,y1} to node closest to {x2,y2}.  Equivalent forms are:\n
HIAddSegment[{x1,y1},{x2,y2}]\n
HIAddSegment[{{x1,y1},{x2,y2}}]"

HIAnalyze::usage =
"HIAnalyze[(flag)] runs the heat flow solver. The flag parameter controls
whether the solver window is visible or minimized. For a visible window, either
specify no value for flag or specify 0. For a minimized window, flag should be
set to 1. An equivalent form is:\n
HIAnalyse[(flag)]"

HIAnalyse::usage =
"HIAnalyse[(flag)] runs the heat flow solver. The flag parameter controls
whether the solver window is visible or minimized. For a visible window, either
specify no value for flag or specify 0. For a minimized window, flag should be
set to 1. An equivalent form is:\n
HIAnalyze[(flag)]"

HIClearSelected::usage =
"HIClearSelected[] clear all selected nodes, blocks, segments and arc segments."

HIClose::usage =
"HIClose[] closes the preprocessor window and destroys the current document."

HICopyRotate::usage =
"HICopyRotate[bx,by,angle,copies,(editaction)]\n
bx, by base point for rotation\n
angle angle by which the selected objects are incrementally shifted
to make each copy. This angle is measured in degrees.\n
copies number of copies to be produced from the selected objects \n
editaction 0-nodes, 1-segments, 2-block labels, 3-arcs, 4-group\n
An equivalent form is:\n
HICopyRotate[{bx,by},angle,copies,(editaction)]"

HICopyTranslate::usage =
"HICopyTranslate[dx,dy,copies,(editaction)]\n
{dx,dy} represents the distance by which the selected objects are to
be incrementally shifted.\n
copies specifies the number of copoies to be produced from the selected
objects\n
editaction 0-nodes, 1-segments, 2-block labels, 3-arcs, 4-group\n
An equivalent form is:\n
HICopyTranslate[{dx,dy},copies,(editaction)]"

HICreateMesh::usage =
"HICreateMesh[] runs triangle to create a mesh. Note that this is not a
necessary precursor of performing an analysis, as HIAnalyze[] will make
sure the mesh is up to date before running an analysis. Returns the number
of elements in the mesh."

HIDeleteBoundProp::usage =
"HIDeleteBoundProp[\"boundpropname\"] deletes the boundary property
named \"boundpropname\"."

HIDeleteConductor::usage =
"HIDeleteConductor[\"conductorname\"] deletes the conductor property
named \"conductorname\"."

HIDeleteMaterial::usage =
"HIDeleteMaterial[\"materialname\"] deletes the material property
named \"materialname\"."

HIDeletePointProp::usage =
"HIDeletePointProp[\"pointpropname\"] deletes the point property
named \"pointpropname\"."

HIDeleteSelected::usage =
"HIDeleteSelected[] deletes all selected objects."

HIDeleteSelectedArcSegments::usage =
"HIDeleteSelectedArcSegments[] deletes all selected arc segments."

HIDeleteSelectedLabels::usage =
"HIDeleteSelectedLabels[] deletes all selected block labels."

HIDeleteSelectedNodes::usage =
"HIDeleteSelectedNodes[] deletes all selected nodes."

HIDeleteSelectedSegments::usage =
"HIDeleteSelectedSegments[] deletes all selected segments."

HIDrawArc::usage =
"HIDrawArc[x1,y1,x2,y2,angle,maxseg] adds an arc to the heat flow
input geometry by drawing points at {x1,y1} and {x2,y2} and then connecting
them with an arc segment. The arc spans a number of degrees specified by
angle. Since FEMM approximates arcs by many line segments, the parameter
maxseg specifies the maximum number of degrees that is allowed to be spanned
by any one segment. Equivalent forms are:\n
HIDrawArc[{x1,y1},{x2,y2},angle,maxseg]\n
HIDrawArc[{{x1,y1},{x2,y2}},angle,maxseg]"

HIDrawLine::usage =
"HIDrawLine[x1,y1,x2,y2] adds points at {x1,y1} and {x2,y2} and then adds
a segment connecting these two points.  Equivalent forms are:\n
HIDrawLine[{x1,y1},{x2,y2}]\n
HIDrawLine[{{x1,y1},{x2,y2}}]"

HIDrawPolygon::usage =
"HIDrawPolygon[{{x1,y2},...,{xn,yn}}] adds new node points at every listed
point and then draws a closed figure that connects the points"

HIDrawPolyLine::usage =
"HIDrawPolyLine[{{x1,y2},...,{xn,yn}}] draws a multi-segment line by adding
each of the points in the list and then adding segments that join the listed
points."

HIDrawRectangle::usage =
"HIDrawRectangle[x1,y1,x2,y2] adds nodes at {x1,y1}, {x1,y2}, {x2,y2} and
 {x2,y1} and joins them with new segments.  Equivalent forms are:\n
HIDrawRectangle[{x1,y1},{x2,y2}]\n
HIDrawRectangle[{{x1,y1},{x2,y2}}]"

HIGetView::usage =
"HIGetView[] grabs the current heat flow input view and returns a
bitmapped graphics object.  This object can subsequently be displayed using
the Show[] command"

HIHideGrid::usage =
"HIHideGrid[] hides the heat flow input grid points"

HIHideNames::usage =
"HIHideNames[] stops the names of the materials
associated with each block label from being displayed"

HILoadSolution::usage =
"HILoadSolution[] loads and displays the solution corresponding to the
current geometry."

HIMirror::usage =
"HIMirror[x1,y1,x2,y2,(editaction)] mirrors the selected objects about a line
passing through the points (x1,y1) and (x2,y2). Valid editaction entries are
0 for nodes, 1 for lines (segments), 2 for block labels, 3 for arc segments,
and 4 for groups. Equivalent forms are:\n
HIMirror[{x1,y1},{x2,y2},(editaction)]\n
HIMirror[{{x1,y1},{x2,y2}},(editaction)]"

HIModifyBoundProp::usage =
"HIModifyBoundProp[\"BdryName\",propnum,value] allows for modification
of a boundary property. The BC to be modified is specified by \"BdryName\".
The next parameter is the number of the property to be set. The last number
is the value to be applied to the specified property. The various properties
that can be modified are listed below:\n
0 - \"BdryName\" - Name of boundary property\n
1 - BdryFormat -  Type of boundary condition (0 = Prescribed Temperature; 1 = Heat Flux; 2 = Convection; 3 = Radiation; 4 = Periodic; 5 = Antiperiodic)
2 - Tset - Fixed Temperature\n
3 - qs - Prescribed heat flux density\n
4 - Tinf - External temperature\n
5 - h - heat transfer coefficient\n
6 - beta - emmissivity"

HIModifyConductorProp::usage =
"HIModifyConductorProp[\"ConductorName\",propnum,value] allows for
modification of a conductor property. The conductor property to be modified is
specified by \"ConductorName\". The next parameter is the number of the property
to be set. The last number is the value to be applied to the specified
property. The various properties that can be modified are listed below:\n
0 - \"ConductorName\" - Name of the conductor property\n
1 - Tc - Conductor Temperature\n
2 - qc - Total conductor heat flux\n
3 - ConductorType - 0 = Prescribed heat flow, 1 = Prescribed temperature"

HIModifyMaterial::usage =
"HIModifyMaterial[\"BlockName\",propnum,value] allows for modification
of a material's properties without redefining the entire material (e.g. so
that current can be modified from run to run). The material to be modified
is specified by \"BlockName\". The next parameter is the number of the property
to be set. The last number is the value to be applied to the specified property.
The various properties that can be modified are listed below:\n
0 - \"BlockName\" - Name of the material\n
1 - Kx - x (or r) direction thermal conductivity\n
2 - Ky - y (or z) direction thermal conductivity\n
3 - qv - Volume heat generation\n
4 - Kt - Volumetric Heat Capacity"

HIModifyPointProp::usage =
"HIModifyPointProp[\"PointName\",propnum,value] allows for modification
of a point property. The point property to be modified is specified by
\"PointName\". The next parameter is the number of the property to be set.
The last number is the value to be applied to the specified property. The
various properties that can be modified are listed below:\n
0 - PointName - Name of the point property\n
1 - Tp - Prescribed nodal temperature\n
2 - qp - Point heat generation in C/m"

HIMoveRotate::usage =
"HIMoveRotate[bx,by,shiftangle,(editaction)]\n
bx, by – base point for rotation\n
shiftangle – angle in degrees by which the selected objects are
rotated.\n
editaction - 0  –nodes, 1 – lines (segments), 2 –block labels, 3 – arc segments, 4- group\n
An equivalent form is:\n
HIMoveRotate[{bx,by},shiftangle,(editaction)]"

HIMoveTranslate::usage =
"HIMoveTranslate[dx,dy,(editaction)]\n
dx,dy – distance by which the selected objects are shifted.\n
editaction - 0 –nodes, 1 – lines (segments), 2 –block labels, 3 – arc segments, 4- group\n
An equivalent form is:\n
HIMoveTranslate[{dx,dy},(editaction)]"

HIProbDef::usage =
"HIProbDef[units,type,precision,depth,minangle,prevsoln,dt] changes the problem definition. The units
parameter specifies the units used for measuring length in the problem domain. Valid \"units\"
entries are \"inches\", \"millimeters\", \"centimeters\", \"mils\", \"meters\", and \"micrometers\".
Set problemtype to \"planar\" for a 2-D planar problem, or to \"axi\" for an axisymmetric
problem. The precision parameter dictates the precision required by the solver. For example,
specifying 1.E-8 requires the RMS of the residual to be less than 10^(-8). The depth parameter,
represents the depth of the problem in the into-the-page direction for 2-D planar problems. The
minangle parameter is a constraint for the mesh generator. It specifies the smallest permissible
angle in triangles that compose the finite element mesh. A good value to choose is 30 degrees, but
smaller values may be needed for \"tough\" geometries that contain small angles.  For computing a
step of a time-transient problem, prevsoln is a string that can be used to specify the location
of a file containing the solution from the previous time step, and dt specifies the time between
solutions."

HIPurgeMesh::usage = "HIPurgeMesh[] clears the mesh out of both the screen and memory."

HIReadDXF::usage = "HIReadDXF[\"filename\"] reads in geometry information a DXF file specified by \"filename\""

HISaveDXF::usage = "HISaveDXF[\"filename\"] saves geometry information in a DXF file specified by \"filename\""

HIRefreshView::usage = "HIRefreshView[] Redraws the current view."

HISaveAs::usage = "HISaveAs[\"filename\"] saves the file with name \"filename\". Note if you use a path you
must use two backslashes e.g. \"c:\\\\temp\\\\myfemmfile.fem\""

HISaveBitmap::usage = "HISaveBitmap[\"filename\"] saves a bitmapped screenshot of the current view to the file
specified by \"filename\", subject to the printf-type formatting explained previously for the HISaveAs command."

HISaveMetafile::usage = "HISaveMetafile[\"filename\"] saves an extended metafile screenshot of the current view to the file
specified by \"filename\", subject to the printf-type formatting explained previously for the HISaveAs command."

HIScale::usage = "HIScale[bx,by,scalefactor,(editaction)]\n
bx, by – base point for scaling\n
scalefactor – a multiplier that determines how much the selected objects are
scaled\n
editaction 0 –nodes, 1 – lines (segments), 2 –block labels, 3 – arc segments, 4- group\n
An equivalent for is:
HIScale[{bx,by},scalefactor,(editaction)]"

HISelectArcSegment::usage = "HISelectArcSegment[x,y] selects the arc segment closest to {x,y}.  An equivalent form is:\n
HISelectArcSegment[{x,y}]"

HISelectGroup::usage = "HISelectGroup[n] selects the nth group of nodes, segments, arc segments and block labels.
This function will clear all previously selected elements and leave the edit mode in 4 (group)"

HISelectLabel::usage = "HISelectLabel[x,y] selects the block label closest to {x,y}.  An equivalent form is:\n
HISelectLabel[{x,y}]"

HISelectNode::usage = "HISelectNode[x,y] selects the node closest to {x,y}.  An equivalent form is:\n
HISelectNode[{x,y}]"

HISelectSegment::usage = "HISelectSegment[x,y] selects the segment closest to {x,y}.  An equivalent form is:\n
HISelectSegment[{x,y}]"

HISetArcSegmentProp::usage =
"HISetArcSegmentProp[maxsegdeg, \"propname\", hide, groupno,\"inconductor\"]
sets the properties associated with the selected arc segments\n
maxsegdeg specifies that the arcs must be meshed with elements that span at
most maxsegdeg degrees per element\n
\"propname\" specifies the boundary property to be associated with the
selected arcs\n
hide: 0 = not hidden in post-processor, 1 == hidden in post processor\n
groupno is an integer specifying the group number of which the selected arcs
are to be members.\n
\"inconductor\" specifies the name of the conductor property with which the
selected arcs are to be associated. If the arcs is not to be part of a
conductor, this parameter can be specified as \"<None>\"."

HISetBlockProp::usage = "HISetBlockProp[\"blockname\",automesh,meshsize,groupno] sets the selected block
labels to have the properties:\n
Block property \"blockname\".\n
automesh: 0 = mesher defers to mesh size constraint defined in meshsize, 1 = mesher
automatically chooses the mesh density.\n
meshsize: size constraint on the mesh in the block marked by this label\n.
groupno: make selected members of specified group number"

HISetEditMode::usage =
"HISetEditMode[\"editmode\"] sets the current editmode to:\n
\"nodes\" – nodes\n
\"segments\" - line segments\n
\"arcsegments\" - arc segments\n
\"blocks\" - block labels\n
\"group\" - selected group\n
This command will affect all subsequent uses of the other editing commands, if they are used
without the editaction parameter."

HISetFocus::usage = "HISetFocus[\"documentname\"] switches the heat flow input file upon
which scripting commands are to act. If more than one heat flow input file is being edited
at a time, this command can be used to switch between files so that the mutiple files can be
operated upon programmatically . \"documentname\" should contain the name of the desired document
as it appears on the window’s title bar."

HISetGrid::usage = "HISetGrid[density,\"type\"] changes the grid spacing. The density parameter specifies
the space between grid points, and the \"type\" parameter is set to \"cart\" for Cartesian coordinates
or \"polar\" for polar coordinates."

HISetNodeProp::usage = "HISetNodeProp[\"propname\",groupno,\"inconductor\"] sets the selected nodes to have
the nodal property \"propname\" and group number groupno. The \"inconductor\" string
specifies which conductor the node belongs to. If the node doesn’t belong to a named conductor,
this parameter can be set to \"<None>\"."

HISetSegmentProp::usage =
"HISetSegmentProp[\"propname\",elementsize,automesh,hide,groupno,\"inconductor\"]
sets the select segments to have:\n
Boundary property \"propname\"\n
Local element size along segment no greater than elementsize\n
automesh: 0 = mesher defers to the element constraint defined by elementsize, 1 = mesher
automatically chooses mesh size along the selected segments\n
hide: 0 = not hidden in post-processor, 1 == hidden in post
processor\n
A member of group number groupno\n
A member of the conductor specified by the string \"inconductor\". If the segment is not
part of a conductor, this parameter can be specified as \"<None>\"."

HIShowGrid::usage = "HIShowGrid[] displays the grid points"

HIShowMesh::usage = "HIShowMesh[] displays the mesh"

HIShowNames::usage = "HIShowNames[] displays the material names associated with each block label"

HISnapGridOff::usage = "HISnapGridOff[] turns off snap-to-grid"

HISnapGridOn::usage = "HISnapGridOn[] turns on snap-to-grid"

HIZoom::usage = "HIZoom[x1,y1,x2,y2] Set the display area to be from the bottom left corner specified by
{x1,y1} to the top right corner specified by {x2,y2}. Equivalent forms are:\n
HIZoom[{x1,y1},{x2,y2}]\n
HIZoom[{{x1,y1},{x2,y2}}]"

HIZoomIn::usage = "HIZoomIn[] zooms out by a factor of 200%."

HIZoomNatural::usage = "HIZoomNatural[] zooms to a \"natural\" view with sensible extents."

HIZoomOut::usage = "HIZoomOut[] zooms out by a factor of 50%."

HIDefineOuterSpace::usage = "HIDefineOuterSpace[Zo,Ro,Ri] defines
an axisymmetric external region to be used in conjuction with the
Kelvin Transformation method of modeling unbounded problems.  The
Zo parameter is the z-location of the origin of the outer region,
the Ro parameter is the radius of the outer region, and the Ri
parameter is the radius of the inner region (i.e. the region of
interest). In the exterior region, the permeability varies as a function of
distance from the origin of the external region.  These parameters
are necessary to define the permeability variation in the external
region."

HIAttachOuterSpace::usage = "HIAttachOuterSpace[] marks all
selected block labels as members of the external region used for
modeling unbounded axisymmetric problems via the Kelvin
Transformation."

HIDetachOuterSpace::usage = "HIDetachOuterSpace[] undefines all
selected block labels as members of the external region used for
modeling unbounded axisymmetric problems via the Kelvin
Transformation."

HICreateRadius::usage = "HICreateRadius[x,y,z] turns a corner located at {x,y} into a curve of radius r.\n
An equivalent form is: HICreateRadius[{x,y},z]"

HIAddTKPoint::usage = "HIAddTKPoint[\"materialname\",T,k] adds
the point {T,k} to the thermal conductivity vs. temperature curve for the material specified by
\"materialname\".  An equivalent form is:\n
HIAddTKPoint[\"materialname\",{T,k}]"

HIAddTKPoints::usage = "HIAddTKPoints[\"materialname\",list] adds
all of the points in list to the thermal conductivity vs. temperature curve for the material specified by
\"materialname\""

HIClearTKPoints::usage = "HIClearTKPoints[\"materialname\"] erases
all of the thermal conductivity points that have been defined for the material named
\"materialname\""


(********** Heat Flow Output Functions **********************)

HOAddContour::usage =
"HOAddContour[x,y] adds the point {x,y} to the contour that is used for plotting values along lines and
for computing line integrals.  An equivalent form is:\n
HOAddContour[{x,y}]"

HOBendContour::usage =
"HOBendContour[angle,anglestep] replaces the straight line formed by the last two
points in the contour by an arc that spans angle degrees. The arc is actually composed
of many straight lines, each of which is constrained to span no more than anglestep degrees.
The angle parameter can take on values from -180 to 180 degrees. The anglestep
parameter must be greater than zero. If there are less than two points defined in the contour,
this command is ignored."

HOBlockIntegral::usage =
"HOBlockIntegral[type] calculate a block integral for the selected
blocks. The type parameter can take on the following values:\n
0 - Average Temperature\n
1 - Block Cross-section\n
2 - Block Volume\n
3 - Average F over the block\n
4 - Average G over the block\n
returns either a single value or a list of two values, depending on the specified type"

HOClearBlock::usage = "HOClearBlock[] clears the current block selection"

HOClearContour::usage = "HOClearContour[] clears the current contour selection"

HOClose::usage = "HOClose[] close the current postprocessor document and window"

HOGetConductorProperties::usage = "HOGetConductorProperties[\"conductor\"] returns properties for the
conductor property named \"conductor\". A list with two values is returned: The temperature of the specified
conductor, and the total heat flux from the specified conductor."

HOGetF::usage = "HOGetD[x,y] returns a list with two elements containing the heat flux density
at point {x,y}.  An equivalent form is:\n
HOGetF[{x,y}]"

HOGetG::usage = "HOGetE[x,y] returns a list with two elements containing the temperature gradient
at point {x,y}.  An equivalent form is:\n
HOGetG[{x,y}]"

HOGetK::usage = "HOGetK[x,y] returns a list with two elements containing the thermal conductivity at point {x,y}.
An equivalent form is:\n
HOGetK[{x,y}]"

HOGetPointValues::usage =
"HOGetPointValues[x,y] returns the various field values associated with the point at {x,y}. The return values,
in order, are:\n
T - temperature \n
Fx - x- or r- direction component of heat flux density \n
Fy - y- or z- direction component of heat flux density \n
Gx - x- or r- direction component of temperature gradient \n
Gy - y- or z- direction component of temperature gradient \n
Kx - x- or r- direction component of thermal conductivity \n
Ky - y- or z- direction component of thermal conductivity \n
An equivalent form is: \n
HOGetPointValues[x,y]"

HOGetProblemInfo::usage =
"HOGetProblemInfo[] returns a list with three values: the Problem
Type (0 for planar and 1 for axisymmetric); the depth assumed for planar problems
in units of meters; and the length unit used to draw the problem, represented in meters."

HOGetT::usage = "HOGetT[x,y] returns the temperature at point {x,y}. An equivalent form is:\n HOGetT[{x,y}]"

HOGetView::usage =
"HOGetView[] grabs the current heat flow output view and returns a
bitmapped graphics object.  This object can subsequently be displayed using
the Show[] command"

HOSnapGridOff::usage = "HOSnapGridOff[] turns off snap-to-grid"

HOSnapGridOn::usage = "HOSnapGridOn[] turns on snap-to-grid"

HOHideContourPlot::usage =
"HOHideContourPlot[] hides the currently displayed contour plot"

HOHideDensityPlot::usage =
"HOHideDensityPlot[] hides the currently displayed density plot"

HOHideGrid::usage =
"HOHideGrid[] hides the grid points from view in the heat flow output window"

HOHideMesh::usage =
"HOHideMesh[] hides the finite element mesh from in the heat flow output view"

HOHideNames::usage =
"HOHideNames[] hides material names associated with block labels from
view in the heat flow output window"

HOHidePoints::usage =
"HOHidePoints[] inhibits the display of input node points in the heat flow
output window"

HOLineIntegral::usage =
"HOLineIntegral[type] calculate the line integral for the defined
contour. Valid type selections include:\n
0 - Temperature Difference (G·t) \n
1 - Heat Flux (F·n) \n
2 - Contour length \n
3 - Average temperature \n
This integral returns either one value or a list of two values,
depending on the type of integral"

HOMakePlot::usage =
"HOMakePlot[PlotType,NumPoints,Filename,FileFormat]\n
This function allows programmatic access to FEMM's X-Y plot routines.
If only PlotType or only PlotType and NumPoints are specified, the command
is interpreted as a request to plot the requested plot type to the screen.
If, in addition, the Filename parameter is specified, the plot is instead
written to disk to the specified file name as an extended metafile. If the
FileFormat parameter is also, the command is instead
interpreted as a command to write the data to disk to the specfied file
name, rather than display it to make a graphical plot. Valid entries for
PlotType are: \n
0 - T (temperature)\n
1 - |F| (Magnitude of heat flux density)\n
2 - F . n (Normal heat flux density)\n
3 - F . t (Tangential heat flux density)\n
4 - |G| (Magnitude of field intensity)\n
5 - G . n (Normal temperature gradient)\n
6 - G . t (Tangential temperature gradient)"

HORefreshView::usage = "HORefreshView[] Redraws the current view."

HOReload::usage = "HOReload[] reloads the current heat flow output file."

HOSaveBitmap::usage = "HOSaveBitmap[\"filename\"] saves a bitmapped screenshot of the current view to the file
specified by \"filename\", subject to the printf-type formatting explained previously for the HISaveAs command."

HOSaveMetafile::usage = "HOSaveMetafile[\"filename\"] saves an extended metafile screenshot of the current view to the file
specified by \"filename\", subject to the printf-type formatting explained previously for the HISaveAs command."

HOSelectBlock::usage = "HOSelectBlock[x,y] select the block that contains point {x,y}.  An equivalent form is:\n
HOSelectBlock[{x,y}]"

HOSelectPoint::usage = "HOSelectPoint[x,y] adds a contour point at the closest input point to {x,y}. If the selected
point and a previous selected points lie at the ends of an arcsegment, a contour is added
that traces along the arcsegment. The selectpoint command has the same functionality as
the left-button-click contour point selection when the program is running in interactive mode.
An equivalent form is:\n
HOSelectPoint[{x,y}]"

HOGroupSelectBlock::usage =
"HOGroupSelectBlock[n] selects all of the blocks that are labeled by block labels that are
members of group n. If no number is specified (i.e. HOGroupSelectBlock[]), all blocks
are selected."

HOSetEditMode::usage =
"HOSetEditMode[\"mode\"] sets the mode of the postprocessor to point, contour, or area mode.
Valid entries for mode are \"point\", \"contour\", and \"area\"."

HOSetGrid::usage =
"HOSetGrid[density,\"type\"] changes the grid spacing. The density parameter specifies
the space between grid points, and the type parameter is set to \"cart\" for Cartesian coordinates
or \"polar\" for polar coordinates."

HOShowContourPlot::usage =
"HOShowContourPlot[numcontours,lowerV,upperV] shows the V contour plot with
options:\n
numcontours - Number of equipotential lines to be plotted.\n
upperV - Upper limit for contours.\n
lowerV - Lower limit for contours.\n
If numcontours is -1 all parameters are ignored and default values are used"

HOShowDensityPlot::usage =
"HOShowDensityPlot[legend,gscale,type,upperD,lowerD]\n
Shows the flux density plot with options:\n
legend - Set to 0 to hide the plot legend or 1 to show the plot legend.\n
gscale - Set to 0 for a colour density plot or 1 for a grey scale density plot.\n
type - Sets the type of density plot. A value of 0 plots temperature, 1 plots the magnitude of D,
upperD - Sets the upper display limit for the density plot.\n
lowerD - Sets the lower display limit for the density plot.\n
and 2 plots the magnitude of E"

HOShowGrid::usage = "HOShowGrid[] displays the grid points"

HOShowMesh::usage = "HOShowMesh[] displays the mesh"

HOShowNames::usage = "HOShowNames[] displays the material names associated with each block label"

HOShowPoints::usage = "HOShowPoints[] displays the input node point as part of the output geometry"

HOSmoothOff::usage = "HOSmoothOff[] turns off smoothing of the D and E fields.  The D and E fields are then
displayed using values that are piecewise constant over each element."

HOSmoothOn::usage = "HOSmoothOn[] turns on smoothing of the D and E fields.  The D and E fields are then
displayed using a linear interpolation of the field from nodal values"

HOZoom::usage = "HOZoom[x1,y1,x2,y2] Set the display area to be from the bottom left corner specified by
{x1,y1} to the top right corner specified by {x2,y2}. Equivalent forms are:\n
HOZoom[{x1,y1},{x2,y2}]\n
HOZoom[{{x1,y1},{x2,y2}}]"

HOZoomIn::usage = "HOZoomIn[] zooms out by a factor of 200%."

HOZoomNatural::usage = "HOZoomNatural[] zooms to a \"natural\" view with sensible extents."

HOZoomOut::usage = "HOZoomOut[] zooms out by a factor of 50%."


(********** Electrostatics Input Functions **********************)
EIAddArc::usage =
"EIAddArc[x1,y1,x2,y2,angle,maxseg] adds an arc to the electrostatics
input geometry from the point nearst to {x1,y1} to the point nearest
to {x2,y2}.  The arc spans a number of degrees specified by angle. Since
FEMM approximates arcs by many line segments, the parameter maxseg
specifies the maximum number of degrees that is allowed to be spanned
by any one segment. Equivalent forms are:\n
EIAddArc[{x1,y1},{x2,y2},angle,maxseg]\n
EIAddArc[{{x1,y1},{x2,y2}},angle,maxseg]"

EIAddBlockLabel::usage = "EIAddBlockLabel[x,y] adds a block label at the
point {x,y}.  An equivalent form is:\n
EIAddBlockLabel[{x,y}]"

EIAddBoundProp::usage =
"EIAddBoundProp[\"boundpropname\", Vs,qs,c0,c1,BdryFormat] adds a new boundary
property with name \"boundpropname\"\n
For a \"Fixed Temperature\" type boundary condition, set the Vs parameter to the
desired voltage and all other parameters to zero.\n
To obtain a \"Mixed\" type boundary condition, set C1 and C0 as required and
BdryFormat to 1. Set all other parameters to zero.\n
To obtain a prescribes surface charge density, set qs to the desired charge
density in C/m2 and set BdryFormat to 2.\n
For a \"Periodic\" boundary condition, set BdryFormat to 3 and set all other
parameters to zero.\n
For an \"Anti-Perodic\" boundary condition, set BdryFormat to 4 set all other
parameters to zero."

EIAddConductorProp::usage =
"EIAddConductorProp[\"conductorname\",Vc,qc,conductortype] adds a new
conductor property with name \"conductorname\" with either a prescribed
voltage or a prescribed total charge. Set the unused property to zero. The
conductortype parameter is 0 for prescribed charge and 1 for prescribed
voltage."

EIAddMaterial::usage =
"EIAddMaterial[\"materialname\",ex,ey,qv] adds a new material with called
\"materialname\" with the material properties:\n
ex Relative permittivity in the x- or r-direction.\n
ey Relative permittivity in the y- or z-direction.\n
qv Volume charge density in units of C/m^3"

EIAddNode::usage =
"EIAddNode[x,y] adds a new node at {x,y}. An equivalent form is:\n
EIAddNode[{x,y}]"

EIAddPointProp::usage =
"EIAddPointProp[\"pointpropname\",Vp,qp] adds a new point property of name
\"pointpropname\" with either a specified potential Vp a point charge
density qp in units of C/m."

EIAddSegment::usage =
"EIAddSegment[x1,y1,x2,y2] add a new line segment from node closest to
{x1,y1} to node closest to {x2,y2}.  Equivalent forms are:\n
EIAddSegment[{x1,y1},{x2,y2}]\n
EIAddSegment[{{x1,y1},{x2,y2}}]"

EIAnalyze::usage =
"EIAnalyze[(flag)] runs the electrostatics solver. The flag parameter controls
whether the solver window is visible or minimized. For a visible window, either
specify no value for flag or specify 0. For a minimized window, flag should be
set to 1. An equivalent form is:\n
EIAnalyse[(flag)]"

EIAnalyse::usage =
"EIAnalyse[(flag)] runs the electrostatics solver. The flag parameter controls
whether the solver window is visible or minimized. For a visible window, either
specify no value for flag or specify 0. For a minimized window, flag should be
set to 1. An equivalent form is:\n
EIAnalyze[(flag)]"

EIClearSelected::usage =
"EIClearSelected[] clear all selected nodes, blocks, segments and arc segments."

EIClose::usage =
"EIClose[] closes the preprocessor window and destroys the current document."

EICopyRotate::usage =
"EICopyRotate[bx,by,angle,copies,(editaction)]\n
bx, by base point for rotation\n
angle angle by which the selected objects are incrementally shifted
to make each copy. This angle is measured in degrees.\n
copies number of copies to be produced from the selected objects \n
editaction 0-nodes, 1-segments, 2-block labels, 3-arcs, 4-group\n
An equivalent form is:\n
EICopyRotate[{bx,by},angle,copies,(editaction)]"


EICopyTranslate::usage =
"EICopyTranslate[dx,dy,copies,(editaction)]\n
{dx,dy} represents the distance by which the selected objects are to
be incrementally shifted.\n
copies specifies the number of copoies to be produced from the selected
objects\n
editaction 0-nodes, 1-segments, 2-block labels, 3-arcs, 4-group\n
An equivalent form is:\n
EICopyTranslate[{dx,dy},copies,(editaction)]"

EICreateMesh::usage =
"EICreateMesh[] runs triangle to create a mesh. Note that this is not a
necessary precursor of performing an analysis, as EIAnalyze[] will make
sure the mesh is up to date before running an analysis. Returns the number
of elements in the mesh."

EIDeleteBoundProp::usage =
"EIDeleteBoundProp[\"boundpropname\"] deletes the boundary property
named \"boundpropname\"."

EIDeleteConductor::usage =
"EIDeleteConductor[\"conductorname\"] deletes the conductor property
named \"conductorname\"."

EIDeleteMaterial::usage =
"EIDeleteMaterial[\"materialname\"] deletes the material property
named \"materialname\"."

EIDeletePointProp::usage =
"EIDeletePointProp[\"pointpropname\"] deletes the point property
named \"pointpropname\"."

EIDeleteSelected::usage =
"EIDeleteSelected[] deletes all selected objects."

EIDeleteSelectedArcSegments::usage =
"EIDeleteSelectedArcSegments[] deletes all selected arc segments."

EIDeleteSelectedLabels::usage =
"EIDeleteSelectedLabels[] deletes all selected block labels."

EIDeleteSelectedNodes::usage =
"EIDeleteSelectedNodes[] deletes all selected nodes."

EIDeleteSelectedSegments::usage =
"EIDeleteSelectedSegments[] deletes all selected segments."

EIDrawArc::usage =
"EIDrawArc[x1,y1,x2,y2,angle,maxseg] adds an arc to the electrostatics
input geometry by drawing points at {x1,y1} and {x2,y2} and then connecting
them with an arc segment. The arc spans a number of degrees specified by
angle. Since FEMM approximates arcs by many line segments, the parameter
maxseg specifies the maximum number of degrees that is allowed to be spanned
by any one segment. Equivalent forms are:\n
EIDrawArc[{x1,y1},{x2,y2},angle,maxseg]\n
EIDrawArc[{{x1,y1},{x2,y2}},angle,maxseg]"

EIDrawLine::usage =
"EIDrawLine[x1,y1,x2,y2] adds points at {x1,y1} and {x2,y2} and then adds
a segment connecting these two points.  Equivalent forms are:\n
EIDrawLine[{x1,y1},{x2,y2}]\n
EIDrawLine[{{x1,y1},{x2,y2}}]"

EIDrawPolygon::usage =
"EIDrawPolygon[{{x1,y2},...,{xn,yn}}] adds new node points at every listed
point and then draws a closed figure that connects the points"

EIDrawPolyLine::usage =
"EIDrawPolyLine[{{x1,y2},...,{xn,yn}}] draws a multi-segment line by adding
each of the points in the list and then adding segments that join the listed
points."

EIDrawRectangle::usage =
"EIDrawRectangle[x1,y1,x2,y2] adds nodes at {x1,y1}, {x1,y2}, {x2,y2} and
 {x2,y1} and joins them with new segments.  Equivalent forms are:\n
EIDrawRectangle[{x1,y1},{x2,y2}]\n
EIDrawRectangle[{{x1,y1},{x2,y2}}]"

EIGetView::usage =
"EIGetView[] grabs the current electrostatics input view and returns a
bitmapped graphics object.  This object can subsequently be displayed using
the Show[] command"

EIHideGrid::usage =
"EIHideGrid[] hides the electrostatics input grid points"

EIHideNames::usage =
"EIHideNames[] stops the names of the materials
associated with each block label from being displayed"

EILoadSolution::usage =
"EILoadSolution[] loads and displays the solution corresponding to the
current geometry."

EIMirror::usage =
"EIMirror[x1,y1,x2,y2,(editaction)] mirrors the selected objects about a line
passing through the points (x1,y1) and (x2,y2). Valid editaction entries are
0 for nodes, 1 for lines (segments), 2 for block labels, 3 for arc segments,
and 4 for groups. Equivalent forms are:\n
EIMirror[{x1,y1},{x2,y2},(editaction)]\n
EIMirror[{{x1,y1},{x2,y2}},(editaction)]"

EIModifyBoundProp::usage =
"EIModifyBoundProp[\"BdryName\",propnum,value] allows for modification
of a boundary property. The BC to be modified is specified by \"BdryName\".
The next parameter is the number of the property to be set. The last number
is the value to be applied to the specified property. The various properties
that can be modified are listed below:\n
0 - \"BdryName\" - Name of boundary property\n
1 - Vs - Fixed Voltage\n
2 - qs - Prescribed charge density\n
3 - c0 - Mixed BC parameter\n
4 - c1 - Mixed BC parameter\n
5 - BdryFormat -  Type of boundary condition (0 = Prescribed V;
1 = Mixed; 2 = Surface charge density; 3 = Periodic; 4 = Antiperiodic)"

EIModifyConductorProp::usage =
"EIModifyConductorProp[\"ConductorName\",propnum,value] allows for
modification of a conductor property. The conductor property to be modified is
specified by \"ConductorName\". The next parameter is the number of the property
to be set. The last number is the value to be applied to the specified
property. The various properties that can be modified are listed below:\n
0 - \"ConductorName\" - Name of the conductor property\n
1 - Vc - Conductor voltage\n
2 - qc - Total conductor charge\n
3 - ConductorType - 0 = Prescribed charge, 1 = Prescribed voltage"

EIModifyMaterial::usage =
"EIModifyMaterial[\"BlockName\",propnum,value] allows for modification
of a material's properties without redefining the entire material (e.g. so
that current can be modified from run to run). The material to be modified
is specified by \"BlockName\". The next parameter is the number of the property
to be set. The last number is the value to be applied to the specified property.
The various properties that can be modified are listed below:\n
0 - \"BlockName\" - Name of the material
1 - ex - x (or r) direction relative permittivity
2 - ey - y (or z) direction relative permittivity
3 - qs - Volume charge"

EIModifyPointProp::usage =
"EIModifyPointProp[\"PointName\",propnum,value] allows for modification
of a point property. The point property to be modified is specified by
\"PointName\". The next parameter is the number of the property to be set.
The last number is the value to be applied to the specified property. The
various properties that can be modified are listed below:\n
0 - PointName - Name of the point property\n
1 - Vp - Prescribed nodal voltage\n
2 - qp - Point charge density in C/m"

EIMoveRotate::usage =
"EIMoveRotate[bx,by,shiftangle,(editaction)]\n
bx, by – base point for rotation\n
shiftangle – angle in degrees by which the selected objects are
rotated.\n
editaction - 0  –nodes, 1 – lines (segments), 2 –block labels, 3 – arc segments, 4- group\n
An equivalent form is:\n
EIMoveRotate[{bx,by},shiftangle,(editaction)]"

EIMoveTranslate::usage =
"EIMoveTranslate[dx,dy,(editaction)]\n
dx,dy – distance by which the selected objects are shifted.\n
editaction - 0 –nodes, 1 – lines (segments), 2 –block labels, 3 – arc segments, 4- group\n
An equivalent form is:\n
EIMoveTranslate[{dx,dy},(editaction)]"

EIProbDef::usage =
"EIProbDef[units,type,precision,depth,minangle] changes the problem definition. The units
parameter specifies the units used for measuring length in the problem domain. Valid \"units\"
entries are \"inches\", \"millimeters\", \"centimeters\", \"mils\", \"meters\", and \"micrometers\".
Set problemtype to \"planar\" for a 2-D planar problem, or to \"axi\" for an axisymmetric
problem. The precision parameter dictates the precision required by the solver. For example,
specifying 1.E-8 requires the RMS of the residual to be less than 10^(-8). The depth parameter,
represents the depth of the problem in the into-the-page direction for 2-D planar problems. The
minangle parameter is a constraint for the mesh generator. It specifies the smallest permissible
angle in triangles that compose the finite element mesh. A good value to choose is 30 degrees, but
smaller values may be needed for \"tough\" geometries that contain small angles."

EIPurgeMesh::usage = "EIPurgeMesh[] clears the mesh out of both the screen and memory."

EIReadDXF::usage = "EIReadDXF[\"filename\"] reads in geometry information a DXF file specified by \"filename\""

EISaveDXF::usage = "EISaveDXF[\"filename\"] saves geometry information in a DXF file specified by \"filename\""

EIRefreshView::usage = "EIRefreshView[] Redraws the current view."

EISaveAs::usage = "EISaveAs[\"filename\"] saves the file with name \"filename\". Note if you use a path you
must use two backslashes e.g. \"c:\\\\temp\\\\myfemmfile.fem\""

EISaveBitmap::usage = "EISaveBitmap[\"filename\"] saves a bitmapped screenshot of the current view to the file
specified by \"filename\", subject to the printf-type formatting explained previously for the EISaveAs command."

EISaveMetafile::usage = "EISaveMetafile[\"filename\"] saves an extended metafile screenshot of the current view to the file
specified by \"filename\", subject to the printf-type formatting explained previously for the EISaveAs command."

EIScale::usage = "EIScale[bx,by,scalefactor,(editaction)]\n
bx, by – base point for scaling\n
scalefactor – a multiplier that determines how much the selected objects are
scaled\n
editaction 0 –nodes, 1 – lines (segments), 2 –block labels, 3 – arc segments, 4- group\n
An equivalent for is:
EIScale[{bx,by},scalefactor,(editaction)]"

EISelectArcSegment::usage = "EISelectArcSegment[x,y] selects the arc segment closest to {x,y}.  An equivalent form is:\n
EISelectArcSegment[{x,y}]"

EISelectGroup::usage = "EISelectGroup[n] selects the nth group of nodes, segments, arc segments and block labels.
This function will clear all previously selected elements and leave the edit mode in 4 (group)"

EISelectLabel::usage = "EISelectLabel[x,y] selects the block label closest to {x,y}.  An equivalent form is:\n
EISelectLabel[{x,y}]"

EISelectNode::usage = "EISelectNode[x,y] selects the node closest to {x,y}.  An equivalent form is:\n
EISelectNode[{x,y}]"

EISelectSegment::usage = "EISelectSegment[x,y] selects the segment closest to {x,y}.  An equivalent form is:\n
EISelectSegment[{x,y}]"

EISetArcSegmentProp::usage =
"EISetArcSegmentProp[maxsegdeg, \"propname\", hide, groupno,\"inconductor\"]
sets the properties associated with the selected arc segments\n
maxsegdeg specifies that the arcs must be meshed with elements that span at
most maxsegdeg degrees per element\n
\"propname\" specifies the boundary property to be associated with the
selected arcs\n
hide: 0 = not hidden in post-processor, 1 == hidden in post processor\n
groupno is an integer specifying the group number of which the selected arcs
are to be members.\n
\"inconductor\" specifies the name of the conductor property with which the
selected arcs are to be associated. If the arcs is not to be part of a
conductor, this parameter can be specified as \"<None>\"."

EISetBlockProp::usage = "EISetBlockProp[\"blockname\",automesh,meshsize,groupno] sets the selected block
labels to have the properties:\n
Block property \"blockname\".\n
automesh: 0 = mesher defers to mesh size constraint defined in meshsize, 1 = mesher
automatically chooses the mesh density.\n
meshsize: size constraint on the mesh in the block marked by this label\n.
groupno: make selected members of specified group number"

EISetEditMode::usage =
"EISetEditMode[\"editmode\"] sets the current editmode to:\n
\"nodes\" – nodes\n
\"segments\" - line segments\n
\"arcsegments\" - arc segments\n
\"blocks\" - block labels\n
\"group\" - selected group\n
This command will affect all subsequent uses of the other editing commands, if they are used
without the editaction parameter."

EISetFocus::usage = "EISetFocus[\"documentname\"] switches the electrostatics input file upon
which scripting commands are to act. If more than one electrostatics input file is being edited
at a time, this command can be used to switch between files so that the mutiple files can be
operated upon programmatically . \"documentname\" should contain the name of the desired document
as it appears on the window’s title bar."

EISetGrid::usage = "EISetGrid[density,\"type\"] changes the grid spacing. The density parameter specifies
the space between grid points, and the \"type\" parameter is set to \"cart\" for Cartesian coordinates
or \"polar\" for polar coordinates."

EISetNodeProp::usage = "EISetNodeProp[\"propname\",groupno,\"inconductor\"] sets the selected nodes to have
the nodal property \"propname\" and group number groupno. The \"inconductor\" string
specifies which conductor the node belongs to. If the node doesn’t belong to a named conductor,
this parameter can be set to \"<None>\"."

EISetSegmentProp::usage =
"EISetSegmentProp[\"propname\",elementsize,automesh,hide,groupno,\"inconductor\"]
sets the select segments to have:\n
Boundary property \"propname\"\n
Local element size along segment no greater than elementsize\n
automesh: 0 = mesher defers to the element constraint defined by elementsize, 1 = mesher
automatically chooses mesh size along the selected segments\n
hide: 0 = not hidden in post-processor, 1 == hidden in post
processor\n
A member of group number groupno\n
A member of the conductor specified by the string \"inconductor\". If the segment is not
part of a conductor, this parameter can be specified as \"<None>\"."

EIShowGrid::usage = "EIShowGrid[] displays the grid points"

EIShowMesh::usage = "EIShowMesh[] displays the mesh"

EIShowNames::usage = "EIShowNames[] displays the material names associated with each block label"

EISnapGridOff::usage = "EISnapGridOff[] turns off snap-to-grid"

EISnapGridOn::usage = "EISnapGridOn[] turns on snap-to-grid"

EIZoom::usage = "EIZoom[x1,y1,x2,y2] Set the display area to be from the bottom left corner specified by
{x1,y1} to the top right corner specified by {x2,y2}. Equivalent forms are:\n
EIZoom[{x1,y1},{x2,y2}]\n
EIZoom[{{x1,y1},{x2,y2}}]"

EIZoomIn::usage = "EIZoomIn[] zooms out by a factor of 200%."

EIZoomNatural::usage = "EIZoomNatural[] zooms to a \"natural\" view with sensible extents."

EIZoomOut::usage = "EIZoomOut[] zooms out by a factor of 50%."

EIDefineOuterSpace::usage = "EIDefineOuterSpace[Zo,Ro,Ri] defines
an axisymmetric external region to be used in conjuction with the
Kelvin Transformation method of modeling unbounded problems.  The
Zo parameter is the z-location of the origin of the outer region,
the Ro parameter is the radius of the outer region, and the Ri
parameter is the radius of the inner region (i.e. the region of
interest). In the exterior region, the permeability varies as a function of
distance from the origin of the external region.  These parameters
are necessary to define the permeability variation in the external
region."

EIAttachOuterSpace::usage = "EIAttachOuterSpace[] marks all
selected block labels as members of the external region used for
modeling unbounded axisymmetric problems via the Kelvin
Transformation."

EIDetachOuterSpace::usage = "EIDetachOuterSpace[] undefines all
selected block labels as members of the external region used for
modeling unbounded axisymmetric problems via the Kelvin
Transformation."

EICreateRadius::usage = "EICreateRadius[x,y,z] turns a corner located at {x,y} into a curve of radius r.\n
An equivalent form is: EICreateRadius[{x,y},z]"

(********** Electrostatics Output Functions **********************)

EOAddContour::usage =
"EOAddContour[x,y] adds the point {x,y} to the contour that is used for plotting values along lines and
for computing line integrals.  An equivalent form is:\n
EOAddContour[{x,y}]"

EOBendContour::usage =
"EOBendContour[angle,anglestep] replaces the straight line formed by the last two
points in the contour by an arc that spans angle degrees. The arc is actually composed
of many straight lines, each of which is constrained to span no more than anglestep degrees.
The angle parameter can take on values from -180 to 180 degrees. The anglestep
parameter must be greater than zero. If there are less than two points defined in the contour,
this command is ignored."

EOBlockIntegral::usage =
"EOBlockIntegral[type] calculate a block integral for the selected
blocks. The type parameter can take on the following values:\n
0 - Stored Energy\n
1 - Block Cross-section\n
2 - Block Volume\n
3 - Average D over the block\n
4 - Average E over the block\n
5 - Weighted Stress Tensor Force\n
6 - Weighted Stress Tensor Torque\n
returns either a single value or a list of two values, depending on the specified type"

EOClearBlock::usage = "EOClearBlock[] clears the current block selection"

EOClearContour::usage = "EOClearContour[] clears the current contour selection"

EOClose::usage = "EOClose[] close the current postprocessor document and window"

EOGetConductorProperties::usage = "EOGetConductorProperties[\"conductor\"] returns properties for the
conductor property named \"conductor\". A list with two values is returned: The voltage of the specified
conductor, and the charge carried on the specified conductor."

EOGetD::usage = "EOGetD[x,y] returns a list with two elements containing the electric flux density
at point {x,y}.  An equivalent form is:\n
EOGetD[{x,y}]"

EOGetE::usage = "EOGetE[x,y] returns a list with two elements containing the electric field intensity
at point {x,y}.  An equivalent form is:\n
EOGetE[{x,y}]"

EOGetEnergyDensity::usage = "EOGetEnergyDensity[x,y] returns the energy density, (E.D)/2,
at point {x,y}.  An equivalent form is:\n
EOGetEnergyDensity[{x,y}]"

EOGetPerm::usage = "EOGetPerm[x,y] returns a list with two elements containing the permittivity at point {x,y}.
An equivalent form is:\n
EOGetPerm[{x,y}]"

EOGetPointValues::usage =
"EOGetPointValues[x,y] returns the various field values associated with the point at {x,y}. The return values,
in order, are:\n
V - Voltage \n
Dx - x- or r- direction component of displacement \n
Dy - y- or z- direction component of displacement \n
Ex - x- or r- direction component of electric field intensity \n
Ey - y- or z- direction component of electric field intensity \n
ex - x- or r- direction component of permittivity \n
ey - y- or z- direction component of permittivity \n
nrg - electric field energy density \n
An equivalent form is: \n
EOGetPointValues[x,y]"

EOGetProblemInfo::usage =
"EOGetProblemInfo[] returns a list with three values: the Problem
Type (0 for planar and 1 for axisymmetric); the depth assumed for planar problems
in units of meters; and the length unit used to draw the problem, represented in meters."

EOGetV::usage = "EOGetV[x,y] returns the voltage at point {x,y}. An equivalent form is:\n EOGetV[{x,y}]"

EOGetView::usage =
"EOGetView[] grabs the current electrostatics output view and returns a
bitmapped graphics object.  This object can subsequently be displayed using
the Show[] command"

EOSnapGridOff::usage = "EOSnapGridOff[] turns off snap-to-grid"

EOSnapGridOn::usage = "EOSnapGridOn[] turns on snap-to-grid"

EOHideContourPlot::usage =
"EOHideContourPlot[] hides the currently displayed contour plot"

EOHideDensityPlot::usage =
"EOHideDensityPlot[] hides the currently displayed density plot"

EOHideGrid::usage =
"EOHideGrid[] hides the grid points from view in the electrostatics output window"

EOHideMesh::usage =
"EOHideMesh[] hides the finite element mesh from in the electrostatics output view"

EOHideNames::usage =
"EOHideNames[] hides material names associated with block labels from
view in the electrostatics output window"

EOHidePoints::usage =
"EOHidePoints[] inhibits the display of input node points in the electrostatics
output window"

EOLineIntegral::usage =
"EOLineIntegral[type] calculate the line integral for the defined
contour. Valid type selections include:\n
0 - E ·t \n
1 - D·n \n
2 - Contour length \n
3 - Force from stress tensor \n
4 - Torque from stress tensor \n
This integral returns either one value or a list of two values,
depending on the type of integral"

EOMakePlot::usage =
"EOMakePlot[PlotType,NumPoints,Filename,FileFormat]\n
This function allows programmatic access to FEMM's X-Y plot routines.
If only PlotType or only PlotType and NumPoints are specified, the command
is interpreted as a request to plot the requested plot type to the screen.
If, in addition, the Filename parameter is specified, the plot is instead
written to disk to the specified file name as an extended metafile. If the
FileFormat parameter is also, the command is instead
interpreted as a command to write the data to disk to the specfied file
name, rather than display it to make a graphical plot. Valid entries for
PlotType are: \n
0 - V (Voltage)\n
1 - |D| (Magnitude of flux density)\n
2 - D . n (Normal flux density)\n
3 - D . t (Tangential flux density)\n
4 - |E| (Magnitude of field intensity)\n
5 - E . n (Normal field intensity)\n
6 - E . t (Tangential field intensity)"

EORefreshView::usage = "EORefreshView[] Redraws the current view."

EOReload::usage = "EOReload[] reloads the current electrostatics output
file."

EOSaveBitmap::usage = "EOSaveBitmap[\"filename\"] saves a bitmapped screenshot of the current view to the file
specified by \"filename\", subject to the printf-type formatting explained previously for the EISaveAs command."

EOSaveMetafile::usage = "EOSaveMetafile[\"filename\"] saves an extended metafile screenshot of the current view to the file
specified by \"filename\", subject to the printf-type formatting explained previously for the EISaveAs command."

EOSelectBlock::usage = "EOSelectBlock[x,y] select the block that contains point {x,y}.  An equivalent form is:\n
EOSelectBlock[{x,y}]"

EOSelectPoint::usage = "EOSelectPoint[x,y] adds a contour point at the closest input point to {x,y}. If the selected
point and a previous selected points lie at the ends of an arcsegment, a contour is added
that traces along the arcsegment. The selectpoint command has the same functionality as
the left-button-click contour point selection when the program is running in interactive mode.
An equivalent form is:\n
EOSelectPoint[{x,y}]"

EOGroupSelectBlock::usage =
"EOGroupSelectBlock[n] selects all of the blocks that are labeled by block labels that are
members of group n. If no number is specified (i.e. EOGroupSelectBlock[]), all blocks
are selected."

EOSetEditMode::usage =
"EOSetEditMode[\"mode\"] sets the mode of the postprocessor to point, contour, or area mode.
Valid entries for mode are \"point\", \"contour\", and \"area\"."

EOSetGrid::usage =
"EOSetGrid[density,\"type\"] changes the grid spacing. The density parameter specifies
the space between grid points, and the type parameter is set to \"cart\" for Cartesian coordinates
or \"polar\" for polar coordinates."

EOShowContourPlot::usage =
"EOShowContourPlot[numcontours,lowerV,upperV] shows the V contour plot with
options:\n
numcontours - Number of equipotential lines to be plotted.\n
upperV - Upper limit for contours.\n
lowerV - Lower limit for contours.\n
If numcontours is -1 all parameters are ignored and default values are used"

EOShowDensityPlot::usage =
"EOShowDensityPlot[legend,gscale,type,upperD,lowerD]\n
Shows the flux density plot with options:\n
legend - Set to 0 to hide the plot legend or 1 to show the plot legend.\n
gscale - Set to 0 for a colour density plot or 1 for a grey scale density plot.\n
upperD - Sets the upper display limit for the density plot.\n
lowerD - Sets the lower display limit for the density plot.\n
type - Sets the type of density plot. A value of 0 plots voltage, 1 plots the magnitude of D,
and 2 plots the magnitude of E"

EOShowGrid::usage = "EOShowGrid[] displays the grid points"

EOShowMesh::usage = "EOShowMesh[] displays the mesh"

EOShowNames::usage = "EOShowNames[] displays the material names associated with each block label"

EOShowPoints::usage = "EOShowPoints[] displays the input node point as part of the output geometry"

EOSmoothOff::usage = "EOSmoothOff[] turns off smoothing of the D and E fields.  The D and E fields are then
displayed using values that are piecewise constant over each element."

EOSmoothOn::usage = "EOSmoothOn[] turns on smoothing of the D and E fields.  The D and E fields are then
displayed using a linear interpolation of the field from nodal values"

EOZoom::usage = "EOZoom[x1,y1,x2,y2] Set the display area to be from the bottom left corner specified by
{x1,y1} to the top right corner specified by {x2,y2}. Equivalent forms are:\n
EOZoom[{x1,y1},{x2,y2}]\n
EOZoom[{{x1,y1},{x2,y2}}]"

EOZoomIn::usage = "EOZoomIn[] zooms out by a factor of 200%."

EOZoomNatural::usage = "EOZoomNatural[] zooms to a \"natural\" view with sensible extents."

EOZoomOut::usage = "EOZoomOut[] zooms out by a factor of 50%."

(**************** Magnetics Input Functions ***************)
MIAddArc::usage =
"MIAddArc[x1,y1,x2,y2,angle,maxseg] adds an arc to the electrostatics
input geometry from the point nearst to {x1,y1} to the point nearest
to {x2,y2}.  The arc spans a number of degrees specified by angle. Since
FEMM approximates arcs by many line segments, the parameter maxseg
specifies the maximum number of degrees that is allowed to be spanned
by any one segment. Equivalent forms are:\n
MIAddArc[{x1,y1},{x2,y2},angle,maxseg]\n
MIAddArc[{{x1,y1},{x2,y2}},angle,maxseg]"

MIAddBHPoint::usage = "MIAddBHPoint[\"materialname\",b,h] adds
the point {b,h} to the BH curve for the material specified by
\"materialname\".  An equivalent form is:\n
MIAddBHPoint[\"materialname\",{b,h}]"

MIAddBHPoints::usage = "MIAddBHPoints[\"materialname\",list] adds
all of the points in list to the BH curve for the material specified by
\"materialname\""

MIAddBlockLabel::usage = "MIAddBlockLabel[x,y] adds a block label at the
point {x,y}.  An equivalent form is:\n
MIAddBlockLabel[{x,y}]"

MIAddBoundProp::usage =
"MIAddBoundProp[\"propname\",A0,A1,A2,Phi,Mu,Sig,c0,c1,BdryFormat,ia,oa]
adds a new boundary property with name \"propname\" \n
– For a \"Prescribed A\" type boundary condition, set the A0, A1, A2
 and Phi parameters as required. Set all other parameters to zero.\n
– For a \"Small Skin Depth\" type boundary condtion, set the Mu to
the desired relative permeability and Sig to the desired conductivity
in MS/m. Set BdryFormat to 1 and all other parameters to zero. \n
– To obtain a \"Mixed\" type boundary condition, set C1 and C0 as
required and BdryFormat to 2. Set all other parameters to zero. \n
– For a \"Strategic dual image\" boundary, set BdryFormat to 3 and set all
other parameters to zero.\n
– For a \"Periodic\" boundary condition, set BdryFormat to 4 and set all
other parameters to zero.\n
– For an \"Anti-Perodic\" boundary condition, set BdryFormat to 5 set all
other parameters to zero.
- For a \"Periodic Air Gap\", set BdryFormat to 6. Parameters ia and oa 
specify the inner and outer boundary angle, respectively.
- For an \"Anti-periodic Air Gap\", set BdryFormat to 7.  The same ia and
oa parameters also apply here."

MIAddCircProp::usage =
"MIAddCircProp[\"circuitname\",i,circuittype]
adds a new circuit property with name \"circuitname\" with a prescribed
current i. It is OK for the current to be complex-valued. The circuittype
parameter is 0 for a parallel-connected circuit and 1 for a
series-connected circuit."

MIAddMaterial::usage =
"MIAddMaterial[\"materialname\",mux,muy,Hc,J,Cduct,Lamd,Phihmax,lamfill,LamType,Phihx,Phihy,NStrands,WireD]\n
adds a new material with called \"materialname\" with the material
properties:\n
- mux - Relative permeability in the x- or r-direction (for linear materials)\n
- muy - Relative permeability in the y- or z-direction (for linear materials)\n
- Hc - Permanent magnet coercivity in Amps/Meter. \n
- J - Applied source current density in Amps/mm2. It is OK for J
to be complex-valued.\n
- Cduct - Electrical conductivity of the material in MS/m.\n
- Lamd - Lamination thickness in millimeters. \n
- Phihmax - Hysteresis lag angle in degrees, used for nonlinear BH
curves.\n
- Lamfill - Fraction of the volume occupied per lamination that is actually
filled with iron (Note that this parameter defaults to 1 the FEMM
preprocessor dialog box because, by default, iron completely fills the
volume)\n
- Lamtype - Set to 0 for \"Not laminated or laminated in plane\";
 1 for \"Laminated x or r\"; 2 for \"Laminated y or z\" \n
- Phihx - Hysteresis lag in degrees in the x-direction for linear
problems.\n
- Phihy - Hysteresis lag in degrees in the y-direction for linear problems.\n
- NStrands - number of strands per wire if the material is a wire.\n
- WireD - diameter of each strand in mm if the material is a wire."

MIAddNode::usage =
"MIAddNode[x,y] adds a new node at {x,y}. An equivalent form is:\n
MIAddNode[{x,y}]"

MIAddPointProp::usage =
"MIAddPointProp[\"pointpropname\",A,J] adds a new point property
of name \"pointpropname\" with either a specified potential A
in units of Webers/Meter or a point current J in units of Amps.
It is OK for either A or J to be complex-valued.
Set the unused parameter to 0."

MIAddSegment::usage =
"MIAddSegment[x1,y1,x2,y2] add a new line segment from node closest to
{x1,y1} to node closest to {x2,y2}.  Equivalent forms are:\n
MIAddSegment[{x1,y1},{x2,y2}]\n
MIAddSegment[{{x1,y1},{x2,y2}}]"

MIAnalyze::usage =
"MIAnalyze[(flag)] runs the magnetics solver. The flag parameter controls
whether the solver window is visible or minimized. For a visible window, either
specify no value for flag or specify 0. For a minimized window, flag should be
set to 1. An equivalent form is:\n
MIAnalyse[(flag)]"

MIAnalyse::usage =
"MIAnalyse[(flag)] runs the magnetics solver. The flag parameter controls
whether the solver window is visible or minimized. For a visible window, either
specify no value for flag or specify 0. For a minimized window, flag should be
set to 1. An equivalent form is:\n
MIAnalyze[(flag)]"

MIClearBHPoints::usage = "MIClearBHPoint[\"materialname\"] erases
all of the BH points that have been defined for the material named
\"materialname\""

MIClearSelected::usage =
"MIClearSelected[] clear all selected nodes, blocks, segments and arc segments."

MIClose::usage =
"MIClose[] closes the preprocessor window and destroys the current document."

MICopyRotate::usage =
"MICopyRotate[bx,by,angle,copies,(editaction)]\n
bx, by base point for rotation\n
angle angle by which the selected objects are incrementally shifted
to make each copy. This angle is measured in degrees.\n
copies number of copies to be produced from the selected objects \n
editaction 0-nodes, 1-segments, 2-block labels, 3-arcs, 4-group\n
An equivalent form is:\n
MICopyRotate[{bx,by},angle,copies,(editaction)]"

MICopyTranslate::usage =
"MICopyTranslate[dx,dy,copies,(editaction)]\n
{dx,dy} represents the distance by which the selected objects are to
be incrementally shifted.\n
copies specifies the number of copoies to be produced from the selected
objects\n
editaction 0-nodes, 1-segments, 2-block labels, 3-arcs, 4-group\n
An equivalent form is:\n
MICopyTranslate[{dx,dy},copies,(editaction)]"

MICreateMesh::usage =
"MICreateMesh[] runs triangle to create a mesh. Note that this is not a
necessary precursor of performing an analysis, as MIAnalyze[] will make
sure the mesh is up to date before running an analysis. Returns the number
of elements in the mesh."

MIDeleteBoundProp::usage =
"MIDeleteBoundProp[\"boundpropname\"] deletes the boundary property
named \"boundpropname\"."

MIDeleteCircuit::usage =
"MIDeleteCircuit[\"circuitname\"] deletes the circuit property
named \"circuitname\"."

MIDeleteMaterial::usage =
"MIDeleteMaterial[\"materialname\"] deletes the material property
named \"materialname\"."

MIDeletePointProp::usage =
"MIDeletePointProp[\"pointpropname\"] deletes the point property
named \"pointpropname\"."

MIDeleteSelected::usage =
"MIDeleteSelected[] deletes all selected objects."

MIDeleteSelectedArcSegments::usage =
"MIDeleteSelectedArcSegments[] deletes all selected arc segments."

MIDeleteSelectedLabels::usage =
"MIDeleteSelectedLabels[] deletes all selected block labels."

MIDeleteSelectedNodes::usage =
"MIDeleteSelectedNodes[] deletes all selected nodes."

MIDeleteSelectedSegments::usage =
"MIDeleteSelectedSegments[] deletes all selected segments."

MIDrawArc::usage =
"MIDrawArc[x1,y1,x2,y2,angle,maxseg] adds an arc to the electrostatics
input geometry by drawing points at {x1,y1} and {x2,y2} and then connecting
them with an arc segment. The arc spans a number of degrees specified by
angle. Since FEMM approximates arcs by many line segments, the parameter
maxseg specifies the maximum number of degrees that is allowed to be spanned
by any one segment. Equivalent forms are:\n
MIDrawArc[{x1,y1},{x2,y2},angle,maxseg]\n
MIDrawArc[{{x1,y1},{x2,y2}},angle,maxseg]"

MIDrawLine::usage =
"MIDrawLine[x1,y1,x2,y2] adds points at {x1,y1} and {x2,y2} and then adds
a segment connecting these two points.  Equivalent forms are:\n
MIDrawLine[{x1,y1},{x2,y2}]\n
MIDrawLine[{{x1,y1},{x2,y2}}]"

MIDrawPolygon::usage =
"MIDrawPolygon[{{x1,y2},...,{xn,yn}}] adds new node points at every listed
point and then draws a closed figure that connects the points"

MIDrawPolyLine::usage =
"MIDrawPolyLine[{{x1,y2},...,{xn,yn}}] draws a multi-segment line by adding
each of the points in the list and then adding segments that join the listed
points."

MIDrawRectangle::usage =
"MIDrawRectangle[x1,y1,x2,y2] adds nodes at {x1,y1}, {x1,y2}, {x2,y2} and
 {x2,y1} and joins them with new segments.  Equivalent forms are:\n
MIDrawRectangle[{x1,y1},{x2,y2}]\n
MIDrawRectangle[{{x1,y1},{x2,y2}}]"

MIGetView::usage =
"MIGetView[] grabs the current electrostatics input view and returns a
bitmapped graphics object.  This object can subsequently be displayed using
the Show[] command"

MIHideGrid::usage =
"MIHideGrid[] hides the electrostatics input grid points"

MIHideNames::usage =
"MIHideNames[] stops the names of the materials
associated with each block label from being displayed"

MILoadSolution::usage =
"MILoadSolution[] loads and displays the solution corresponding to the
current geometry."

MIMirror::usage =
"MIMirror[x1,y1,x2,y2,(editaction)] mirrors the selected objects about a line
passing through the points (x1,y1) and (x2,y2). Valid editaction entries are
0 for nodes, 1 for lines (segments), 2 for block labels, 3 for arc segments,
and 4 for groups. Equivalent forms are:\n
MIMirror[{x1,y1},{x2,y2},(editaction)]\n
MIMirror[{{x1,y1},{x2,y2}},(editaction)]"

MIModifyBoundProp::usage =
"MIModifyBoundProp[\"BdryName\",propnum,value] allows for modification
of a boundary property. The BC to be modified is specified by \"BdryName\". The next
parameter is the number of the property to be set. The last number is the value to be applied
to the specified property. The various properties that can be modified are listed below:\n
0 - \"BdryName\" - Name of boundary property \n
1 A0 - Prescribed A parameter\n
2 A1 - Prescribed A parameter\n
3 A2 - Prescribed A parameter\n
4 phi - Prescribed A phase\n
5 mu -  Small skin depth relative permeability \n
6 Cduct -  Small skin depth conductivity, MS/m \n
7 c0 - Mixed BC parameter \n
8 c1 - Mixed BC parameter \n
9 - BdryFormat.  Valid choices for boundary format include:\n
\t0 = Prescribed A\n
\t1 = Small skin depth\n
\t2 = Mixed\n
\t3 = Strategic Dual Image\n
\t4 = Periodic\n
\t5 = Antiperiodic
10 ia - Inner boundary angle for air gap element \n
11 oa - Outer boundary angle for air gap element"

MIModifyCircProp::usage =
"MIModifyCircProp[\"circname\",propnum,value] allows for modification
of a circuit property. The circuit property to be modified is specified by \"circname\".
The next parameter is the number of the property to be set. The last number is the value to
be applied to the specified property. The various properties that can be modified are listed
below:\n
0 - CircName - Name of the circuit property\n
1 - i - Total current. It is OK for i to be complex-valued\n
2 - CircType -  0 = Parallel, 1 = Series"

MIModifyMaterial::usage =
"MIModifyMaterial[\"BlockName\",propnum,value] allows for modification
of a material’s properties without redefining the entire material.
The material to be modified is specified by \"BlockName\".
The next parameter is the number of the property to be set. The last number is the value to
be applied to the specified property. The various properties that can be modified are listed
below:\n
0 - \"BlockName\" - Name of the material \n
1 - mux - x (or r) direction relative permeability \n
2 - muy - y (or z) direction relative permeability \n
3 - Hc - Coercivity, Amps/Meter \n
4 - J - Current density, MA/m2.  OK if J is complex-valued. \n
5 - cduct - Electrical conductivity, MS/m \n
6 - dlam - Lamination thickness, mm \n
7 - phihmax - Hysteresis lag angle for nonlinear problems, degrees \n
8 - LamFill - Iron fill fraction \n
9 - LamType - 0 = None/In plane, 1 = parallel to x, 2=parallel to y \n
10 - phihx - Hysteresis lag in x-direction for linear problems, degrees \n
11 - phihy - Hysteresis lag in y-direction for linear problems, degrees"

MIModifyPointProp::usage =
"MIModifyPointProp[\"PointName\",propnum,value] allows for modification of a point property.
The point property to be modified is specified by \"PointName\".
The next parameter is the number of the property to be set. The last number is the value to
be applied to the specified property. The various properties that can be modified are listed
below:\n
0 - PointName - Name of the point property \n
1 - A - Nodal potential, Weber/Meter\n
2 - J - Nodal current, Amps\n
It is OK if either A or J are complex-valued."

MIMoveRotate::usage =
"MIMoveRotate[bx,by,shiftangle,(editaction)]\n
bx, by – base point for rotation\n
shiftangle – angle in degrees by which the selected objects are
rotated.\n
editaction - 0  –nodes, 1 – lines (segments), 2 –block labels, 3 – arc segments, 4- group\n
An equivalent form is:\n
MIMoveRotate[{bx,by},shiftangle,(editaction)]"

MIMoveTranslate::usage =
"MIMoveTranslate[dx,dy,(editaction)]\n
dx,dy – distance by which the selected objects are shifted.\n
editaction - 0 –nodes, 1 – lines (segments), 2 –block labels, 3 – arc segments, 4- group\n
An equivalent form is:\n
MIMoveTranslate[{dx,dy},(editaction)]"

MIProbDef::usage =
"MIProbDef[freq,units,type,precision,depth,minangle,(acsolver)] changes the problem definition. The freq
parameter specifies the frequency at which the analysis is performed in Hz. The units
parameter specifies the units used for measuring length in the problem domain. Valid \"units\"
entries are \"inches\", \"millimeters\", \"centimeters\", \"mils\", \"meters\", and \"micrometers\".
Set problemtype to \"planar\" for a 2-D planar problem, or to \"axi\" for an axisymmetric
problem. The precision parameter dictates the precision required by the solver. For example,
specifying 1.E-8 requires the RMS of the residual to be less than 10^(-8). The depth parameter,
represents the depth of the problem in the into-the-page direction for 2-D planar problems. The
minangle parameter is a constraint for the mesh generator. It specifies the smallest permissible
angle in triangles that compose the finite element mesh. A good value to choose is 30 degrees, but
smaller values may be needed for \"tough\" geometries that contain small angles. The optional acsolver parameter
selects whether successive approximation (0) or Newton (1) method is to be used to solve AC problems"

MIPurgeMesh::usage = "MIPurgeMesh[] clears the mesh out of both the screen and memory."

MIReadDXF::usage = "MIReadDXF[\"filename\"] reads in geometry information a DXF file specified by \"filename\""

MISaveDXF::usage = "MISaveDXF[\"filename\"] saves geometry information in a DXF file specified by \"filename\""

MIRefreshView::usage = "MIRefreshView[] Redraws the current view."

MISaveAs::usage = "MISaveAs[\"filename\"] saves the file with name \"filename\". Note if you use a path you
must use two backslashes e.g. \"c:\\\\temp\\\\myfemmfile.fem\""

MISetPrevious::usage = "MISetPrevious[\"filename\", PrevType] defines the previous solution to be used as the
basis for an AC incremental permeability or Frozen Permeability solution.  The filename should include the .ans extension.
The PrevType parameter should be set to 1 for Incremental or 2 for Frozen"

MISaveBitmap::usage = "MISaveBitmap[\"filename\"] saves a bitmapped screenshot of the current view to the file
specified by \"filename\", subject to the printf-type formatting explained previously for the MISaveAs command."

MISaveMetafile::usage = "MISaveMetafile[\"filename\"] saves an extended metafile screenshot of the current view to the file
specified by \"filename\", subject to the printf-type formatting explained previously for the MISaveAs command."

MIScale::usage = "MIScale[bx,by,scalefactor,(editaction)]\n
bx, by – base point for scaling\n
scalefactor – a multiplier that determines how much the selected objects are
scaled\n
editaction 0 –nodes, 1 – lines (segments), 2 –block labels, 3 – arc segments, 4- group\n
An equivalent for is:
MIScale[{bx,by},scalefactor,(editaction)]"

MISelectArcSegment::usage = "MISelectArcSegment[x,y] selects the arc segment closest to {x,y}.  An equivalent form is:\n
MISelectArcSegment[{x,y}]"

MISelectGroup::usage = "MISelectGroup[n] selects the nth group of nodes, segments, arc segments and block labels.
This function will clear all previously selected elements and leave the edit mode in 4 (group)"

MISelectLabel::usage = "MISelectLabel[x,y] selects the block label closest to {x,y}.  An equivalent form is:\n
MISelectLabel[{x,y}]"

MISelectNode::usage = "MISelectNode[x,y] selects the node closest to {x,y}.  An equivalent form is:\n
MISelectNode[{x,y}]"

MISelectSegment::usage = "MISelectSegment[x,y] selects the segment closest to {x,y}.  An equivalent form is:\n
MISelectSegment[{x,y}]"

MISetArcSegmentProp::usage =
"MISetArcSegmentProp[maxsegdeg,\"propname\",hide,groupno] sets the selected arc
segments to:\n
– Meshed with elements that span at most maxsegdeg degrees per
element\n
– Boundary property \"propname\"\n
– hide: 0 = not hidden in post-processor, 1 == hidden in post
processor\n
– A member of group number groupno"

MISetBlockProp::usage =
"MISetBlockProp[\"blockname\",automesh,meshsize,\"incircuit\", magdirection,group,turns]\n
sets the selected block labels to have the properties:\n
– Block property \"blockname\".\n
– automesh: 0 = mesher defers to mesh size constraint defined in meshsize, 1 = mesher
automatically chooses the mesh density.\n
– meshsize: size constraint on the mesh in the block marked by this
label.\n
– Block is a member of the circuit named \"incircuit\" \n
– The magnetization is directed along an angle in measured in degrees denoted by the
parameter magdirection\n
– A member of group number group\n
– The number of turns associated with this label is denoted by
turns."

MISetCurrent::usage = "MISetCurrent[\"circname\",i] sets the current of the circuit named \"circname\" to
the value specified by i.  It is OK for the current to be complex-valued."

MISetEditMode::usage =
"MISetEditMode[\"editmode\"] sets the current editmode to:\n
\"nodes\" – nodes\n
\"segments\" - line segments\n
\"arcsegments\" - arc segments\n
\"blocks\" - block labels\n
\"group\" - selected group\n
This command will affect all subsequent uses of the other editing commands, if they are used
without the editaction parameter."

MISetFocus::usage = "MISetFocus[\"documentname\"] switches the electrostatics input file upon
which scripting commands are to act. If more than one electrostatics input file is being edited
at a time, this command can be used to switch between files so that the mutiple files can be
operated upon programmatically . \"documentname\" should contain the name of the desired document
as it appears on the window’s title bar."

MISetGrid::usage = "MISetGrid[density,\"type\"] changes the grid spacing. The density parameter specifies
the space between grid points, and the \"type\" parameter is set to \"cart\" for Cartesian coordinates
or \"polar\" for polar coordinates."

MISetNodeProp::usage = "MISetNodeProp[\"propname\",groupno] set the selected nodes to
have the nodal property \"propname\" and group number groupno."

MISetSegmentProp::usage =
"MISetSegmentProp[\"propname\",elementsize,automesh,hide,group] sets the
selected segments to have:\n
– Boundary property \"propname\" \n
– Local element size along segment no greater than elementsize \n
– automesh: 0 = mesher defers to the element constraint defined by elementsize, 1 =
mesher automatically chooses mesh size along the selected
segments \n
– hide: 0 = not hidden in post-processor, 1 == hidden in post
processor\n
– A member of group number group"

MIShowGrid::usage = "MIShowGrid[] displays the grid points"

MIShowMesh::usage = "MIShowMesh[] displays the mesh"

MIShowNames::usage = "MIShowNames[] displays the material names associated with each block label"

MISnapGridOff::usage = "MISnapGridOff[] turns off snap-to-grid"

MISnapGridOn::usage = "MISnapGridOn[] turns on snap-to-grid"

MIZoom::usage = "MIZoom[x1,y1,x2,y2] Set the display area to be from the bottom left corner specified by
{x1,y1} to the top right corner specified by {x2,y2}. Equivalent forms are:\n
MIZoom[{x1,y1},{x2,y2}]\n
MIZoom[{{x1,y1},{x2,y2}}]"

MIZoomIn::usage = "MIZoomIn[] zooms out by a factor of 200%."

MIZoomNatural::usage = "MIZoomNatural[] zooms to a \"natural\" view with sensible extents."

MIZoomOut::usage = "MIZoomOut[] zooms out by a factor of 50%."

MIDefineOuterSpace::usage = "MIDefineOuterSpace[Zo,Ro,Ri] defines
an axisymmetric external region to be used in conjuction with the
Kelvin Transformation method of modeling unbounded problems.  The
Zo parameter is the z-location of the origin of the outer region,
the Ro parameter is the radius of the outer region, and the Ri
parameter is the radius of the inner region (i.e. the region of
interest). In the exterior region, the permeability varies as a function of
distance from the origin of the external region.  These parameters
are necessary to define the permeability variation in the external
region."

MIAttachOuterSpace::usage = "MIAttachOuterSpace[] marks all
selected block labels as members of the external region used for
modeling unbounded axisymmetric problems via the Kelvin
Transformation."

MIDetachOuterSpace::usage = "MIDetachOuterSpace[] undefines all
selected block labels as members of the external region used for
modeling unbounded axisymmetric problems via the Kelvin
Transformation."

MICreateRadius::usage = "MICreateRadius[x,y,z] turns a corner located at {x,y} into a curve of radius r.\n
An equivalent form is: MICreateRadius[{x,y},z]"

(******* Magnetics Output Functions **************)
MOAddContour::usage =
"MOAddContour[x,y] adds the point {x,y} to the contour that is used for plotting values along lines and
for computinlocatedg line integrals.  An equivalent form is:\n
MOAddContour[{x,y}]"

MOBendContour::usage =
"MOBendContour[angle,anglestep] replaces the straight line formed by the last two
points in curvethe contour by an arc that spans angle degrees. The arc is actually composed
of many straight lines, each of which is constrained to span no more than anglestep degrees.
The angle parameter can take on values from -180 to 180 degrees. The anglestep
parameter must be greater than zero. If there are less than two points defined in the contour,
this command is ignored."

MOBlockIntegral::usage =
"MOBlockIntegral[type] calculates a block integral over the selected
blocks.  There is a single (possibly complex) return value. Valid type
specifications are:\n
0 - A·J \n
1 - A \n
2 - Magnetic field energy \n
3 - Hysteresis and/or lamination losses \n
4 - Resistive losses \n
5 - Block cross-section area \n
6 - Total losses \n
7 - Total current \n
8 - Integral of Bx (or Br) over block \n
9 - Integral of By (or Bz) over block \n
10 - Block volume \n
11 - x (or r) part of steady-state Lorentz force \n
12 - y (or z) part of steady-state Lorentz force \n
13 - x (or r) part of 2X Lorentz force \n
14 - y (or z) part of 2X Lorentz force \n
15 - Steady-state Lorentz torque \n
16 - 2X component of Lorentz torque \n
17 - Magnetic field coenergy \n
18 - x (or r) part of steady-state weighted stress tensor force \n
19 - y (or z) part of steady-state weighted stress tensor force \n
20 - x (or r) part of 2X weighted stress tensor force \n
21 - y (or z) part of 2X weighted stress tensor force \n
22 - Steady-state weighted stress tensor torque \n
23 - 2X component of weighted stress tensor torque \n
24 - R^2 (i.e. moment of inertia / density\n
25 - x (or r) part of 1X weighted stress tensor force \n
26 - y (or z) part of 1X weighted stress tensor force \n
27 - 1X weighted stress tensor torque \n
28 - x (or r) part of 1X Lorentz tensor force \n
29 - y (or z) part of 1X Lorentz force \n
30 - 1X Lorentz torque"

MOClearBlock::usage = "MOClearBlock[] clears the current block selection"

MOClearContour::usage = "MOClearContour[] clears the current contour selection"

MOClose::usage = "MOClose[] close the current postprocessor document and window"

MOGetA::usage = "MOGetA[x,y] returns the vector potential, A, for 2D planar
problems, and it returns flux, 2*Pi*r*A, for axisymmetric problems.  The
return value is possbly complex-valued. An equivalent form is:\n
MOGetA[{x,y}]"

MOGetB::usage = "MOGetB[x,y] returns a list with two elements containing the magnetic flux density
at point {x,y}.  An equivalent form is:\n
MOGetB[{x,y}]"

MOGetCircuitProperties::usage =
"MOGetCircuitProperties[\"circuit\"] is used primarily to obtain impedance
information associated with circuit properties. Properties are returned
for the circuit property named \"circuit\". A list of three values is
returned by the function. In order, the elements of this list are:\n
current -  the current carried by the circuit.\n
volts - the voltage drop across the circuit in the circuit.\n
flux - the circuit’s flux linkage\n
Any of these entries could possibly be complex-valued."

MOGetConductivity::usage =
"MOGetConductivity[x,y] returns the electrical conductivity at point {x,y}.
An equivalent form is:\n MOGetConductivity[{x,y}]"

MOGetEnergyDensity::usage =
"MOGetEnergyDensity[x,y] returns the energy density in the magnetic field
at point {x,y}. An equivalent form is:\n MOGetEnergyDensity[{x,y}]"

MOGetH::usage =
"MOGetH[x,y] returns a list with two elements containing the magnetic
field intensity at point {x,y}.  An equivalent form is:\n MOGetB[{x,y}]"

MOGetJ::usage =
"MOGetJ[x,y] returns the electric current density at point {x,y}.
An equivalent form is:\n MOGetJ[{x,y}]"

MOGetMu::usage =
"MOGetMu[x,y] returns a list with two elements containing the magnetic
permeability at point {x,y}.  An equivalent form is:\n MOGetMu[{x,y}]"

MOGetPe::usage =
"MOGetPe[x,y] returns the electrical (ohmic) loss density at point {x,y}.
An equivalent form is:\n MOGetMu[{x,y}]"

MOGetPh::usage =
"MOGetPh[x,y] returns the hysteresis and/or laminated eddy current loss
density at point {x,y}. An equivalent form is:\n MOGetPh[{x,y}]"

MOGetFill::usage =
"MOGetFill[x,y] returns the winding fill factor at point {x,y}. 
An equivalent form is:\n MOGetFill[{x,y}]"

MOGetPointValues::usage =
"MOGetPointValues[x,y] get the field values at {x,y}.  The function
returns a list of values which represent:\n
A - vector potential A, flux=2*Pi*r*A if axisymmetric\n
B1 - Bx if planar, Br if axisymmetric\n
B2 - By if planar, Bz if axisymmetric\n
Sig - conductivity\n
E - stored energy density\n
H1 - Hx if planar, Hr if axisymmetric\n
H2 - Hy if planar, Hz if axisymmetric\n
Je - Eddy current contribution to current density\n
Js - Source current contribution to current density\n
Mu1 - mux if planar, mur if axisymmetric\n
Mu2 - muy if planar, muz if axisymmetric\n
Pe - Power density dissipated through ohmic losses\n
Ph - Power density dissipated by hysteresis\n
ff - Winding fill factor"

MOGetProblemInfo::usage =
"MOGetProblemInfo[] returns a list of four values: the problem type (0 for
planar, 1 for axisymmetric); the analysis frequency in Hz; the depth of the
problem in meters; and the length unit used to draw the problem, represented in meters."

MOGetView::usage =
"MOGetView[] grabs the current electrostatics output view and returns a
bitmapped graphics object.  This object can subsequently be displayed using
the Show[] command"

MOSnapGridOff::usage = "MOSnapGridOff[] turns off snap-to-grid"

MOSnapGridOn::usage = "MOSnapGridOn[] turns on snap-to-grid"

MOHideContourPlot::usage =
"MOHideContourPlot[] hides the currently displayed contour plot"

MOHideDensityPlot::usage =
"MOHideDensityPlot[] hides the currently displayed density plot"

MOHideGrid::usage =
"MOHideGrid[] hides the grid points from view in the electrostatics output window"

MOHideMesh::usage =
"MOHideMesh[] hides the finite element mesh from in the electrostatics output view"

MOHideNames::usage =
"MOHideNames[] hides material names associated with block labels from
view in the electrostatics output window"

MOHidePoints::usage =
"MOHidePoints[] inhibits the display of input node points in the electrostatics
output window"

MOLineIntegral::usage =
"MOLineIntegral[type] calculates the line integral for the defined
contour.  The following types of line integral and the return
values are as follows:\n
0 - total B.n, average B.n over the contour \n
1 - total H.t, average H.t over the contour \n
2 - Contour length, surface area \n
3 - Stress Tensor Force: DC r/x force, DC y/z force, 2X r/x force, 2X y/z
force\n
4 - Stress Tensor Torque: DC torque, 2X torque \n
5 - total (B.n)^2, average (B.n)^2 \n
Typically returns two (possibly complex) values as results.
However, the Stress Tensor Force integral returns four values."

MOMakePlot::usage =
"MOMakePlot[PlotType,NumPoints,Filename,FileFormat]\n
This function allows programmatic access to FEMM's X-Y plot routines.
If only PlotType or only PlotType and NumPoints are specified, the command
is interpreted as a request to plot the requested plot type to the screen.
If, in addition, the Filename parameter is specified, the plot is instead
written to disk to the specified file name as an extended metafile. If the
FileFormat parameter is also, the command is instead
interpreted as a command to write the data to disk to the specfied file
name, rather than display it to make a graphical plot. Valid entries for
PlotType are: \n
0 - Potential \n
1 - |B| \n
2 B·n \n
3 - B·t \n
4 - |H| \n
5 - H ·n \n
6 - H ·t \n
7 - Jeddy \n
8 - Jsource+Jeddy"

MORefreshView::usage = "MORefreshView[] Redraws the current view."

MOReload::usage = "MOReload[] reloads the current electrostatics output
file."

MOSaveBitmap::usage = "MOSaveBitmap[\"filename\"] saves a bitmapped screenshot of the current view to the file
specified by \"filename\", subject to the printf-type formatting explained previously for the EISaveAs command."

MOSaveMetafile::usage = "MOSaveMetafile[\"filename\"] saves an extended metafile screenshot of the current view to the file
specified by \"filename\", subject to the printf-type formatting explained previously for the EISaveAs command."

MOSelectBlock::usage = "MOSelectBlock[x,y] select the block that contains point {x,y}.  An equivalent form is:\n
MOSelectBlock[{x,y}]"

MOSelectPoint::usage = "MOSelectPoint[x,y] adds a contour point at the closest input point to {x,y}. If the selected
point and a previous selected points lie at the ends of an arcsegment, a contour is added
that traces along the arcsegment. The selectpoint command has the same functionality as
the left-button-click contour point selection when the program is running in interactive mode.
An equivalent form is:\n
MOSelectPoint[{x,y}]"

MOGroupSelectBlock::usage =
"MOGroupSelectBlock[n] selects all of the blocks that are labeled by block labels that are
members of group n. If no number is specified (i.e. MOGroupSelectBlock[]), all blocks
are selected."

MOSetEditMode::usage =
"MOSetEditMode[\"mode\"] sets the mode of the postprocessor to point, contour, or area mode.
Valid entries for mode are \"point\", \"contour\", and \"area\"."

MOSetGrid::usage =
"MOSetGrid[density,\"type\"] changes the grid spacing. The density parameter specifies
the space between grid points, and the type parameter is set to \"cart\" for Cartesian coordinates
or \"polar\" for polar coordinates."

MOSetWeightingScheme::usage =
"MOSetWeightingScheme[density,\"type\"] changes weighting method used for Weighted Stress Tensor
force and torque integrals. Type 0 is recommended. This integral generated a line of elements in the
middle of the airgap between components, minimizing error.  Type 1 weights all elements equally
and uses all elements in the gap."

MOShowContourPlot::usage =
"MOShowContourPlot[numcontours,lowerA,upperA,type] shows the A contour plot
with options:\n
numcontours - Number of A equipotential lines to be plotted. \n
upperA - Upper limit for A contours. \n
lowerA - Lower limit for A contours. \n
type - Choice of \"real\", \"imag\", or \"both\" to show either
the real, imaginary of both real and imaginary components of A.\n
If numcontours is -1, all parameters are ignored and default
values are used"

MOShowDensityPlot::usage =
"MOShowDensityPlot[legend,gscale,upperB,lowerB,type]\n
shows the flux density plot with the options: \n
legend - Set to 0 to hide the plot legend or 1 to show the plot legend.\n
gscale - Set to 0 for a colour density plot or 1 for a grey scaledensity plot.\n
upperB - Sets the upper display limit for the density plot.\n
lowerB - Sets the lower display limit for the density plot.\n
type - Type of density plot to display. Valid entries are \"mag\", \"real\", and \"imag\"
for magnitude, real component, and imaginary component of B, respectively. Alternatively,
current density can be displayed by specifying \"jmag\", \"jreal\", and \"jimag\"
for magnitude, real component, and imaginary component of J, respectively.\n
If legend is set to -1 all parameters are ignored and default values are used"

MOShowGrid::usage = "MOShowGrid[] displays the grid points"

MOShowMesh::usage = "MOShowMesh[] displays the mesh"

MOShowNames::usage = "MOShowNames[] displays the material names associated with each block label"

MOShowPoints::usage = "MOShowPoints[] displays the input node point as part of the output geometry"

MOSmoothOff::usage = "MOSmoothOff[] turns off smoothing of the B and H fields.  The B and H fields are then
displayed using values that are piecewise constant over each element."

MOSmoothOn::usage = "MOSmoothOn[] turns on smoothing of the B and H fields.  The B and H fields are then
displayed using a linear interpolation of the field from nodal values"

MOZoom::usage = "MOZoom[x1,y1,x2,y2] Set the display area to be from the bottom left corner specified by
{x1,y1} to the top right corner specified by {x2,y2}. Equivalent forms are:\n
MOZoom[{x1,y1},{x2,y2}]\n
MOZoom[{{x1,y1},{x2,y2}}]"

MOZoomIn::usage = "MOZoomIn[] zooms out by a factor of 200%."

MOZoomNatural::usage = "MOZoomNatural[] zooms to a \"natural\" view with sensible extents."

MOZoomOut::usage = "MOZoomOut[] zooms out by a factor of 50%."

MOGroupSelectBlock::usage =
"MOGroupSelectBlock[n] selects all of the blocks that are labeled by block labels that are
members of group n. If no number is specified (i.e. MOGroupSelectBlock[]), all blocks
are selected."

MOGetGapB::usage =
"MOGetGapB[BdryName,angle] computes the radial and tangential flux density on the centerline of the
specified air gap element at the specified angle.  The angle is specified in degrees."

MOGetGapA::usage =
"MOGetGapA[BdryName,angle] computes the magnetic vector potential on the centerline of the
specified air gap element at the specified angle.  The angle is specified in degrees."

MOGapIntegral::usage = 
"MOGapIntegral[BdryName,inttype] computes an integral specifiedy by inttype over an air gap element specified
by BdryName. Values for inttype in are:\n
0 :: DC Torque\n
1 :: DC Force\n
2 :: Stored Energy\n
3 :: 2X Torque\n
4 :: 2X Force\n
5 :: Interaction Torque\n
6 :: Interaction Force"

MOGetGapHarmonics::usage = 
"MOGetGapHarmonics[BdryName,n] returns the acc, acs, brc, brs, btc, and bts.  These quantities represent the
components of vector potential, radial flux density, and tangential flux density along cos(n*theta) and
sin(n*theta) on the centerline of the specified air gap element at the specified angle.  The angle is 
specified in degrees.  Call with just the BdryName parameter to obtain the number of harmonics available."

(****** Functions that aren't associated with a ***)
(****** specific document type                  ***)

CloseFEMM::usage =
"CloseFEMM[] shuts down the external instance of FEMM and closes the associated MathLink connection."

OpenFEMM::usage =
"OpenFEMM[] starts an external instance of FEMM and establishes an associated MathLink connection.
OpenFEMM[1] starts FEMM minimized"

OpenDebug::usage = ""

HidePointProps::usage = "HidePointProps[] hides the Point Properties dialog from view"

ShowPointProps::usage = "ShowPointProps[] displays the Point Properties dialog"

HideConsole::usage = "HideConsole[] hides the FEMM Lua console from view"

ShowConsole::usage = "ShowConsole[] displays the FEMM Lua console"

Messagebox::usage = "MessageBox[\"string\"] displays a pop-up messagebox with the message \"string\""

NewDocument::usage = "NewDocument[doctype] creates a new preprocessor document and opens up
a new preprocessor window in FEMM. Specify doctype to be 0 for a magnetics problem, 1 for
an electrostatics problem, 2 for a heat flow problem, or 3 for a current flow problem.
Alternate form is Create[doctype]"

Create::usage = "Create[doctype] creates a new preprocessor document and opens up
a new preprocessor window in FEMM. Specify doctype to be 0 for a magnetics problem, 1 for
an electrostatics problem, 2 for a heat flow problem, or 3 for a current flow problem. 
Alternate form is NewDocument[doctype]"

OpenDocument::usage = "OpenDocument[\"filename\"] opens a document specified by \"filename\"."

Prompt::usage = "Prompt[\"string\"] displays a pop-up dialog with the message \"string\" displayed
on it with an edit box.  The return value of the funciton is the contents of the edit box."

MLPut::usage = "MLPut[\"string\"] sends \"string\" to FEMM's Lua interpreter via MathLink"

MLGet::usage = "MLGet[] receives data from FEMM via MathLink.  The data is always in the form
of a list, but the number of elements in the list is variable.  The items in the list could be
either numbers or strings"

MLGetNC::usage = "MLGetNC[] receives data from FEMM via MathLink.  The data is always in the form
of a list, but the number of elements in the list is variable.  The items in the list could be
either numbers or strings.  This function is the same as MLGet[], only the results are not passed
through Chop[]"

SetPathToFEMM::usage = "SetPathToFEMM[\"path\"] is used by the package to find the femm.exe, the FEMM executable."

GetPackageDirectory::usage = "GetPackageDirectory[] displays the directory in which the MathFEMM package resides."

AWG::usage = "AWG[awg] returns the wire diameter in mm for the specified AWG gauge"

IEC::usage = "IEC[iec] returns the wire diameter in mm for the specified IEC R-20 wire gauge"

SmartMesh::usage = "SmartMesh[state] sets session-wide Smart Meshing on if state=1 or off if state=0"
MISmartMesh::usage = "MISmartMesh[state] sets Smart Meshing on if state=1 or off if state=0 for the in focus magnetics problem"
EISmartMesh::usage = "EISmartMesh[state] sets Smart Meshing on if state=1 or off if state=0 for the in focus electrostatics problem"
HISmartMesh::usage = "HISmartMesh[state] sets Smart Meshing on if state=1 or off if state=0 for the in focus heat flow problem"
CISmartMesh::usage = "CISmartMesh[state] sets Smart Meshing on if state=1 or off if state=0 for the in focus current flow problem "

MainMinimize::usage = "MainMinimize[] minimizes the main FEMM window."
MainMaximize::usage = "MainMaximize[] maximizes the main FEMM window."
MainRestore::usage  = "MainRestore[] restores the main FEMM window from a minimized or maximized state."
MainResize::usage   = "MainResize[width,height] resizes the main FEMM window client area to width X height."

MIMinimize::usage = "MIMinimize[] minimizes the active magnetics input view."
MIMaximize::usage = "MIMaximize[] maximizes the active magnetics input view."
MIRestore::usage  = "MIRestore[] restores the active magnetics input view from a minimized or maximized state."
MIResize::usage   = "MIResize[width,height] resizes the active magnetics input client area to width X height."

MOMinimize::usage = "MOMinimize[] minimizes the active magnetics output view."
MOMaximize::usage = "MOMaximize[] maximizes the active magnetics output view."
MORestore::usage  = "MORestore[] restores the active magnetics output view from a minimized or maximized state."
MOResize::usage   = "MOResize[width,height] resizes the active magnetics output client area to width X height."

EIMinimize::usage = "EIMinimize[] minimizes the active electrostatics input view."
EIMaximize::usage = "EIMaximize[] maximizes the active electrostatics input view."
EIRestore::usage  = "EIRestore[] restores the active electrostatics input view from a minimized or maximized state."
EIResize::usage   = "EIResize[width,height] resizes the active electrostatics input client area to width X height."

EOMinimize::usage = "EOMinimize[] minimizes the active electrostatics output view."
EOMaximize::usage = "EOMaximize[] maximizes the active electrostatics output view."
EORestore::usage  = "EORestore[] restores the active electrostatics output view from a minimized or maximized state."
EOResize::usage   = "EOResize[width,height] resizes the active electrostatics output client area to width X height."

HIMinimize::usage = "HIMinimize[] minimizes the active electrostatics input view."
HIMaximize::usage = "HIMaximize[] maximizes the active electrostatics input view."
HIRestore::usage  = "HIRestore[] restores the active electrostatics input view from a minimized or maximized state."
HIResize::usage   = "HIResize[width,height] resizes the active electrostatics input client area to width X height."

HOMinimize::usage = "HOMinimize[] minimizes the active electrostatics output view."
HOMaximize::usage = "HOMaximize[] maximizes the active electrostatics output view."
HORestore::usage  = "HORestore[] restores the active electrostatics output view from a minimized or maximized state."
HOResize::usage   = "HOResize[width,height] resizes the active electrostatics output client area to width X height."

CIMinimize::usage = "CIMinimize[] minimizes the active electrostatics input view."
CIMaximize::usage = "CIMaximize[] maximizes the active electrostatics input view."
CIRestore::usage  = "CIRestore[] restores the active electrostatics input view from a minimized or maximized state."
CIResize::usage   = "CIResize[width,height] resizes the active electrostatics input client area to width X height."

COMinimize::usage = "COMinimize[] minimizes the active electrostatics output view."
COMaximize::usage = "COMaximize[] maximizes the active electrostatics output view."
CORestore::usage  = "CORestore[] restores the active electrostatics output view from a minimized or maximized state."
COResize::usage   = "COResize[width,height] resizes the active electrostatics output client area to width X height."

COShowVectorPlot::usage = "COShowVectorPlot[PlotType,ScaleFactor] or COShowVectorPlot[PlotType]"
MOShowVectorPlot::usage = "MOShowVectorPlot[PlotType,ScaleFactor] or MOShowVectorPlot[PlotType]"
EOShowVectorPlot::usage = "EOShowVectorPlot[PlotType,ScaleFactor] or EOShowVectorPlot[PlotType]"
HOShowVectorPlot::usage = "HOShowVectorPlot[PlotType,ScaleFactor] or HOShowVectorPlot[PlotType]"

MONumNodes::usage = "MONumNodes[] returns the number of nodes in the current magnetics output mesh"
MONumElements::usage = "MONumElements[] returns the number of elements in the current magnets output mesh"
MOGetElement::usage = 
"MOGetElement[n] returns the following proprerties for the nth element: \n
	1) Index of first element node\n
	2) Index of second element node\n
	3) Index of third element node\n
	4) x (or r) coordinate of the element centroid\n
	5) y (or z) coordinate of the element centroid\n
	6) element area using the length unit defined for the problem\n
	7) group number associated with the element\n"
MOGetNode::usage = "MOGetNode[n] returns the (x,y) or (r,z) position of the nth mesh node"

MONumNodes::usage = "MONumNodes[] returns the number of nodes in the in focus magnetics output mesh"
MONumElements::usage = "MONumElements[] returns the number of elements in the in focus magnets output mesh"
MOGetElement::usage = 
"MOGetElement[n] returns the following proprerties for the nth element: \n
	1) Index of first element node\n
	2) Index of second element node\n
	3) Index of third element node\n
	4) x (or r) coordinate of the element centroid\n
	5) y (or z) coordinate of the element centroid\n
	6) element area using the length unit defined for the problem\n
	7) group number associated with the element\n"
MOGetNode::usage = "MOGetNode[n] returns the (x,y) or (r,z) position of the nth mesh node"

HONumNodes::usage = "HONumNodes[] returns the number of nodes in the in focus heat flow output mesh"
HONumElements::usage = "HONumElements[] returns the number of elements in the in focus heat flow output mesh"
HOGetElement::usage = 
"HOGetElement[n] returns the following proprerties for the nth element: \n
	1) Index of first element node\n
	2) Index of second element node\n
	3) Index of third element node\n
	4) x (or r) coordinate of the element centroid\n
	5) y (or z) coordinate of the element centroid\n
	6) element area using the length unit defined for the problem\n
	7) group number associated with the element\n"
HOGetNode::usage = "HOGetNode[n] returns the (x,y) or (r,z) position of the nth mesh node"

EONumNodes::usage = "EONumNodes[] returns the number of nodes in the in focus electrostatics output mesh"
EONumElements::usage = "EONumElements[] returns the number of elements in the in focus electrostatics output mesh"
EOGetElement::usage = 
"EOGetElement[n] returns the following proprerties for the nth element: \n
	1) Index of first element node\n
	2) Index of second element node\n
	3) Index of third element node\n
	4) x (or r) coordinate of the element centroid\n
	5) y (or z) coordinate of the element centroid\n
	6) element area using the length unit defined for the problem\n
	7) group number associated with the element\n"
EOGetNode::usage = "EOGetNode[n] returns the (x,y) or (r,z) position of the nth mesh node"

CONumNodes::usage = "CONumNodes[] returns the number of nodes in the in focus current flow output mesh"
CONumElements::usage = "CONumElements[] returns the number of elements in the in focus current flow output mesh"
COGetElement::usage = 
"COGetElement[n] returns the following proprerties for the nth element: \n
	1) Index of first element node\n
	2) Index of second element node\n
	3) Index of third element node\n
	4) x (or r) coordinate of the element centroid\n
	5) y (or z) coordinate of the element centroid\n
	6) element area using the length unit defined for the problem\n
	7) group number associated with the element\n"
COGetNode::usage = "COGetNode[n] returns the (x,y) or (r,z) position of the nth mesh node"

MISetGroup::usage = "MISetGroup[n] sets the group of the selected entities to n"
EISetGroup::usage = "EISetGroup[n] sets the group of the selected entities to n"
HISetGroup::usage = "HISetGroup[n] sets the group of the selected entities to n"
CISetGroup::usage = "CISetGroup[n] sets the group of the selected entities to n"

(****************************************************)
(****** Beginning of actual function definitions ****)
(****************************************************)
Begin["`Private`"]

mlink = Null;
mlinkResult = Null;

PackDir = DirectoryName[StringReplace[$Input, "/" -> "\\"]];

Print["MathFEMM loaded at " <> DateString[]];

FEMMPath = "c:\\femm42\\bin\\femm.exe";

SetPathToFEMM[path_]:=Module[{},
        Export[ToFileName[PackDir,"path.txt"], path];
        FEMMPath=path;
]

GetPathToFEMM[]:=Module[{path},
        Off[Import::nffil];
        path=Import[ToFileName[PackDir, "path.txt"]];
        On[Import::nffil];
        If[path=!=$Failed,FEMMPath=path];
]

GetPackageDirectory[]:=PackDir;

(* beginning of the "real" functionality *)
OpenFEMM[bHide_] := Module[{},
	If[bHide==0,
		OpenFEMM[],
		(* Check to see if we're running a version that can call FEMM as an ActiveX server. *)
		bUseMathlink=False;
		If[Needs["NETLink`"] === $Failed,bUseMathlink=True];
		If[bUseMathlink,
			If[mlink === Null,
				GetPathToFEMM[];
				mlink = LinkLaunch[FEMMPath <> " -windowhide"];
				LinkWrite[mlink, "print(\"MathLink Connected\")"];
				LinkWrite[mlink, "setcompatibilitymode(0)"],
				Print["already connected!"];
				LinkWrite[mlink, "setcompatibilitymode(0)"]
			],
			If[mlink===Null,
				mlink=NETLink`CreateCOMObject["femm.ActiveFEMM"];
				mlink@call2femm["print('NETLink Connected')"],
				Print["already connected!"];
			];
		]
	]
]

OpenFEMM[] := Module[{},
	(* Check to see if we're running a version that can call FEMM as an ActiveX server. *)
	bUseMathlink=False;
	If[Needs["NETLink`"] === $Failed,bUseMathlink=True];
	If[bUseMathlink,
		If[mlink === Null,
			GetPathToFEMM[];
			mlink = LinkLaunch[FEMMPath];
			LinkWrite[mlink, "print(\"MathLink Connected\")"];
			LinkWrite[mlink, "setcompatibilitymode(0)"],
			Print["already connected!"];
			LinkWrite[mlink, "setcompatibilitymode(0)"]
		],
		If[mlink===Null,
			mlink=NETLink`CreateCOMObject["femm.ActiveFEMM"];
			mlink@call2femm["main_restore()"];
			mlink@call2femm["print('NETLink Connected')"],
			Print["already connected!"];
		];
	]
]
	
CloseFEMM[] :=
	If[bUseMathlink,
		If[mlink =!= Null,
			LinkWrite[mlink, "quit()"];
			Pause[1];
			LinkClose[mlink];
			mlink = Null; ,
			Print["no open link!"]],
		NETLink`ReleaseCOMObject[mlink];
		mlink=Null;
	]

MLPut[s_]:=If[bUseMathlink,
		If[mlink =!= Null,
			(* Clear out old data on the link that might be left over from a crash *)
			While[LinkReadyQ[mlink] != False, LinkRead[mlink]];
			LinkWrite[mlink,s],
			Print["not connected!"]
		],
		MLPutWait[s]
]

MLPutWait[s_]:=Module[{myResult,ss},
	If[bUseMathlink,
		If[mlink =!= Null,
			LinkWrite[mlink,s];
			MLPut["mlput(0)"];
			While[LinkReadyQ[mlink] == False,Pause[0.001]];
			MLGet[];,
			Print["not connected!"]
		],
		If[StringMatchQ[s,"mlput(*)"]==True,ss=StringTake[s,{7,StringLength[s]-1}],ss=s];
		ss=StringReplace[ss, "\\" -> "\\\\"];
		myResult=mlink@call2femm[ss];
		If[StringMatchQ[myResult, "error:*"]==True,
			Print[myResult];
			Beep[];
			FrontEndExecute[FrontEndToken["EvaluatorAbort"]],
			myResult=StringReplace[myResult, "e" -> "*^"];
			If[Length[StringPosition[myResult, "\n"]]<=1,
				(* Result is a scalar number; just evaluate it *)
				mlinkResult=ToExpression[myResult],
				(* Result is a vector; need to clean things up, then evaluate it *)
				myResult="{"<>StringReplace[StringTake[myResult, StringLength[myResult] - 1], "\n" -> ","]<>"}";
				mlinkResult=ToExpression[myResult];
			];
		]
	]
]

MLGet[]:=Module[{z},
		If[bUseMathlink,
			While[LinkReadyQ[mlink] == False];
			z=Chop[LinkRead[mlink],10^(-14)];
			If[Length[z]==1,z[[1]],z],
			Chop[mlinkResult]
		]
]

MLGetNC[]:=Module[{z},
		If[bUseMathlink,
			While[LinkReadyQ[mlink] == False];
			z=LinkRead[mlink];
			If[Length[z]==1,z[[1]],z],
			mlinkResult
		]
]

Quote[s_]:= "\"" <> ToString[s] <> "\""

QuoteC[s_]:= "\"" <> ToString[s] <> "\","

Num[n_]:=ToString[N[n],CForm]

NumC[n_]:=ToString[N[n],CForm] <> ","

NewDocument[doctype_]:=MLPut["create(" <> Num[doctype] <> ")"]

Create[doctype_]:=MLPut["create(" <> Num[doctype] <> ")"]

OpenDocument[fn_]:=MLPut["open(" <> Quote[fn] <> ")"]

ShowPointProps[]:=MLPut["showpointprops()"];

HidePointProps[]:=MLPut["hidepointprops()"];

ShowConsole[]:=MLPut["showconsole()"];

HideConsole[]:=MLPut["hideconsole()"];

Prompt[msg_]:=Module[{},
        MLPut["mlput(prompt(" <> Quote[msg] <> "))"];
        MLGet[]
]

Messagebox[msg_]:=MLPut["messagebox(" <> Quote[msg] <> ")" ]

MIGetView[]:=Module[{fname,vu},
    fname=Close[OpenTemporary[]];
    MLPutWait["mi_savebitmap(" <> ToString[InputForm[fname]] <> ")"];
    vu=Import[fname,"BMP"];
        ReadList["! del " <> fname, Word];
    vu
]

MOGetView[]:=Module[{fname,vu},
    fname=Close[OpenTemporary[]];
    MLPutWait["mo_savebitmap(" <> ToString[InputForm[fname]] <> ")"];
    vu=Import[fname,"BMP"];
    ReadList["! del " <> fname, Word];
    vu
]

EIGetView[]:=Module[{fname,vu},
    fname=Close[OpenTemporary[]];
    MLPutWait["ei_savebitmap(" <> ToString[InputForm[fname]] <> ")"];
    vu=Import[fname,"BMP"];
    ReadList["! del " <> fname, Word];
    vu
]

EOGetView[]:=Module[{fname,vu},
    fname=Close[OpenTemporary[]];
    MLPutWait["eo_savebitmap(" <> ToString[InputForm[fname]] <> ")"];
    vu=Import[fname,"BMP"];
    ReadList["! del " <> fname, Word];
    vu
]

HIGetView[]:=Module[{fname,vu},
    fname=Close[OpenTemporary[]];
    MLPutWait["hi_savebitmap(" <> ToString[InputForm[fname]] <> ")"];
    vu=Import[fname,"BMP"];
    ReadList["! del " <> fname, Word];
    vu
]

HOGetView[]:=Module[{fname,vu},
    fname=Close[OpenTemporary[]];
    MLPutWait["ho_savebitmap(" <> ToString[InputForm[fname]] <> ")"];
    vu=Import[fname,"BMP"];
    ReadList["! del " <> fname, Word];
    vu
]

CIGetView[]:=Module[{fname,vu},
    fname=Close[OpenTemporary[]];
    MLPutWait["ci_savebitmap(" <> ToString[InputForm[fname]] <> ")"];
    vu=Import[fname,"BMP"];
    ReadList["! del " <> fname, Word];
    vu
]

COGetView[]:=Module[{fname,vu},
    fname=Close[OpenTemporary[]];
    MLPutWait["co_savebitmap(" <> ToString[InputForm[fname]] <> ")"];
    vu=Import[fname,"BMP"];
    ReadList["! del " <> fname, Word];
    vu
]

MainMinimize[]:=MLPut["main_minimize()"]

MainMaximize[]:=MLPut["main_maximize()"]

MainRestore[]:=MLPut["main_restore()"]

MainResize[nWidth_,nHeight_]:=
        MLPut["main_resize(" <> NumC[nWidth] <> NumC[nHeight] <> ")"]

MIMinimize[]:=MLPut["mi_minimize()"]

MIMaximize[]:=MLPut["mi_maximize()"]

MIRestore[]:=MLPut["mi_restore()"]

MIResize[nWidth_,nHeight_]:=
        MLPut["mi_resize(" <> NumC[nWidth] <> Num[nHeight] <> ")"]

MOMinimize[]:=MLPut["mo_minimize()"]

MOMaximize[]:=MLPut["mo_maximize()"]

MORestore[]:=MLPut["mo_restore()"]

MOResize[nWidth_,nHeight_]:=
        MLPut["mo_resize(" <> NumC[nWidth] <> Num[nHeight] <> ")"]

EIMinimize[]:=MLPut["ei_minimize()"]

EIMaximize[]:=MLPut["ei_maximize()"]

EIRestore[]:=MLPut["ei_restore()"]

EIResize[nWidth_,nHeight_]:=
        MLPut["ei_resize(" <> NumC[nWidth] <> Num[nHeight] <> ")"]

EOMinimize[]:=MLPut["eo_minimize()"]

EOMaximize[]:=MLPut["eo_maximize()"]

EORestore[]:=MLPut["eo_restore()"]

EOResize[nWidth_,nHeight_]:=
        MLPut["eo_resize(" <> NumC[nWidth] <> Num[nHeight] <> ")"]

HOMinimize[]:=MLPut["ho_minimize()"]

HOMaximize[]:=MLPut["ho_maximize()"]

HORestore[]:=MLPut["ho_restore()"]

HOResize[nWidth_,nHeight_]:=
        MLPut["ho_resize(" <> NumC[nWidth] <> Num[nHeight] <> ")"]

HIMinimize[]:=MLPut["hi_minimize()"]

HIMaximize[]:=MLPut["hi_maximize()"]

HIRestore[]:=MLPut["hi_restore()"]

HIResize[nWidth_,nHeight_]:=
        MLPut["hi_resize(" <> NumC[nWidth] <> Num[nHeight] <> ")"]

COMinimize[]:=MLPut["co_minimize()"]

COMaximize[]:=MLPut["co_maximize()"]

CORestore[]:=MLPut["co_restore()"]

COResize[nWidth_,nHeight_]:=
        MLPut["ho_resize(" <> NumC[nWidth] <> Num[nHeight] <> ")"]

CIMinimize[]:=MLPut["ci_minimize()"]

CIMaximize[]:=MLPut["ci_maximize()"]

CIRestore[]:=MLPut["ci_restore()"]

CIResize[nWidth_,nHeight_]:=
        MLPut["hi_resize(" <> NumC[nWidth] <> Num[nHeight] <> ")"]

MIZoomNatural[]:=MLPut["mi_zoomnatural()"]

MIZoomOut[]:=MLPut["mi_zoomout()"]

MIZoomIn[]:=MLPut["mi_zoomin()"];

MIZoom[x1_,y1_,x2_,y2_]:=MLPut["mi_zoom(" <> NumC[x1] <> NumC[y1]
        <> NumC[x2] <> Num[y2] <> ")"]

MIZoom[{x1_,y1_},{x2_,y2_}]:=MIZoom[x1,y1,x2,y2]

MIZoom[{{x1_,y1_},{x2_,y2_}}]:=MIZoom[x1,y1,x2,y2]

MOZoomNatural[]:=MLPut["mo_zoomnatural()"]

MOZoomOut[]:=MLPut["mo_zoomout()"]

MOZoomIn[]:=MLPut["mo_zoomin()"];

MOZoom[x1_,y1_,x2_,y2_]:=MLPut["mo_zoom(" <> NumC[x1] <> NumC[y1]
        <> NumC[x2] <> Num[y2] <> ")"]

MOZoom[{x1_,y1_},{x2_,y2_}]:=MOZoom[x1,y1,x2,y2]

MOZoom[{{x1_,y1_},{x2_,y2_}}]:=MOZoom[x1,y1,x2,y2]

EIZoomNatural[]:=MLPut["ei_zoomnatural()"]

EIZoomOut[]:=MLPut["ei_zoomout()"]

EIZoomIn[]:=MLPut["ei_zoomin()"];

EIZoom[x1_,y1_,x2_,y2_]:=MLPut["ei_zoom(" <> NumC[x1] <> NumC[y1]
        <> NumC[x2] <> Num[y2] <> ")"]

EIZoom[{x1_,y1_},{x2_,y2_}]:=EIZoom[x1,y1,x2,y2]

EIZoom[{{x1_,y1_},{x2_,y2_}}]:=EIZoom[x1,y1,x2,y2]

EOZoomNatural[]:=MLPut["eo_zoomnatural()"]

EOZoomOut[]:=MLPut["eo_zoomout()"]

EOZoomIn[]:=MLPut["eo_zoomin()"];

EOZoom[x1_,y1_,x2_,y2_]:=MLPut["eo_zoom(" <> NumC[x1] <> NumC[y1]
        <> NumC[x2] <> Num[y2] <> ")"]

EOZoom[{x1_,y1_},{x2_,y2_}]:=EOZoom[x1,y1,x2,y2]

EOZoom[{{x1_,y1_},{x2_,y2_}}]:=EOZoom[x1,y1,x2,y2]

HIZoomNatural[]:=MLPut["hi_zoomnatural()"]

HIZoomOut[]:=MLPut["hi_zoomout()"]

HIZoomIn[]:=MLPut["hi_zoomin()"];

HIZoom[x1_,y1_,x2_,y2_]:=MLPut["hi_zoom(" <> NumC[x1] <> NumC[y1]
        <> NumC[x2] <> Num[y2] <> ")"]

HIZoom[{x1_,y1_},{x2_,y2_}]:=HIZoom[x1,y1,x2,y2]

HIZoom[{{x1_,y1_},{x2_,y2_}}]:=HIZoom[x1,y1,x2,y2]

HOZoomNatural[]:=MLPut["ho_zoomnatural()"]

HOZoomOut[]:=MLPut["ho_zoomout()"]

HOZoomIn[]:=MLPut["ho_zoomin()"];

HOZoom[x1_,y1_,x2_,y2_]:=MLPut["ho_zoom(" <> NumC[x1] <> NumC[y1]
        <> NumC[x2] <> Num[y2] <> ")"]

HOZoom[{x1_,y1_},{x2_,y2_}]:=HOZoom[x1,y1,x2,y2]

HOZoom[{{x1_,y1_},{x2_,y2_}}]:=HOZoom[x1,y1,x2,y2]

CIZoomNatural[]:=MLPut["ci_zoomnatural()"]

CIZoomOut[]:=MLPut["ci_zoomout()"]

CIZoomIn[]:=MLPut["ci_zoomin()"];

CIZoom[x1_,y1_,x2_,y2_]:=MLPut["ci_zoom(" <> NumC[x1] <> NumC[y1]
        <> NumC[x2] <> Num[y2] <> ")"]

CIZoom[{x1_,y1_},{x2_,y2_}]:=CIZoom[x1,y1,x2,y2]

CIZoom[{{x1_,y1_},{x2_,y2_}}]:=CIZoom[x1,y1,x2,y2]

COZoomNatural[]:=MLPut["co_zoomnatural()"]

COZoomOut[]:=MLPut["co_zoomout()"]

COZoomIn[]:=MLPut["co_zoomin()"];

COZoom[x1_,y1_,x2_,y2_]:=MLPut["co_zoom(" <> NumC[x1] <> NumC[y1]
        <> NumC[x2] <> Num[y2] <> ")"]

COZoom[{x1_,y1_},{x2_,y2_}]:=COZoom[x1,y1,x2,y2]

COZoom[{{x1_,y1_},{x2_,y2_}}]:=COZoom[x1,y1,x2,y2]


(********************************************************************)
MISaveAs[fn_] := MLPut["mi_saveas(" <> Quote[fn] <> ")"]

MISetPrevious[fn_] := MLPut["mi_setprevious(" <> Quote[fn] <> ")"]

MISetPrevious[fn_, prevtype_] := MLPut["mi_setprevious(" <> QuoteC[fn] <> Num[prevtype] <> ")"]

MICreateMesh[]:=Module[{},
	MLPut["mi_createmesh()"];
	MLGet[]
]

MIShowMesh[]:=MLPut["mi_showmesh()"]

MIPurgeMesh[]:=MLPut["mi_purgemesh()"]

MIProbDef[freq_] :=
        MLPut["mi_probdef(" <> Num[freq] <> ")"]
				
MIProbDef[freq_, units_, type_, precision_, depth_, minangle_] :=
        MLPut["mi_probdef(" <> NumC[freq] <> QuoteC[units] <>
                QuoteC[type] <> NumC[precision] <> NumC[depth] <>
                Num[minangle] <> ")"]

MIProbDef[freq_, units_, type_, precision_, depth_, minangle_,acsolver_] :=
        MLPut["mi_probdef(" <> NumC[freq] <> QuoteC[units] <>
                QuoteC[type] <> NumC[precision] <> NumC[depth] <>
                NumC[minangle] <> Num[acsolver] ")"]

MIAnalyze[]:=MLPutWait["mi_analyze()"];

MIAnalyse[]:=MIAnalyze[]

MIAnalyze[n_]:=MLPutWait["mi_analyze(" <> Num[n] <> ")"];

MIAnalyse[n_]:=MIAnalyze[n]

MILoadSolution[]:=MLPutWait["mi_loadsolution()"];

MIAddNode[x_,y_]:=MLPut["mi_addnode(" <> NumC[x] <> Num[y] <> ")"]

MIAddBlockLabel[x_,y_]:=MLPut["mi_addblocklabel(" <> NumC[x] <> Num[y] <> ")"]

MIAddSegment[x1_,y1_,x2_,y2_]:=MLPut["mi_addsegment(" <> NumC[x1] <> NumC[y1]
        <> NumC[x2] <> Num[y2] <> ")"]

MIAddArc[x1_,y1_,x2_,y2_,angle_,maxseg_]:=MLPut["mi_addarc(" <> NumC[x1] <> NumC[y1]
        <> NumC[x2] <> NumC[y2] <> NumC[angle] <> Num[maxseg] <> ")"]

MISelectNode[x_,y_]:=Module[{},
        MLPut["mlput(mi_selectnode(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[];
]
MISelectNode[{x_,y_}]:=MISelectNode[x,y]

MISelectLabel[x_,y_]:=Module[{},
        MLPut["mlput(mi_selectlabel(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[];
]
MISelectLabel[{x_,y_}]:=MISelectLabel[x,y]

MISelectSegment[x_,y_]:=Module[{},
        MLPut["mlput(mi_selectsegment(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[];
]
MISelectSegment[{x_,y_}]:=MISelectSegment[x,y]

MISelectArcSegment[x_,y_]:=Module[{},
        MLPut["mlput(mi_selectarcsegment(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[];
]
MISelectArcSegment[{x_,y_}]:=MISelectArcSegment[x,y]

MIClearSelected[]:=MLPut["mi_clearselected()"]

MISetNodeProp[nodeprop_,groupno_]:=MLPut["mi_setnodeprop(" <> QuoteC[nodeprop] <> Num[groupno] <> ")"]

MISetBlockProp[blockname_,automesh_,meshsize_,incirc_,magdir_,group_,turns_]:=Module[{},
	If[Head[magdir]==String,
        MLPut["mi_setblockprop(" <> QuoteC[blockname] <> NumC[automesh] <>
                NumC[meshsize] <> QuoteC[incirc] <> QuoteC[magdir] <>
                NumC[group] <> Num[turns] <> ")"];
		Return[]
	];
	MLPut["mi_setblockprop(" <> QuoteC[blockname] <> NumC[automesh] <>
		NumC[meshsize] <> QuoteC[incirc] <> NumC[magdir] <>
		NumC[group] <> Num[turns] <> ")"];
]

MISetSegmentProp[pn_,es_,am_,hi_,gr_]:=
        MLPut["mi_setsegmentprop(" <> QuoteC[pn] <> NumC[es] <>
                NumC[am] <> NumC[hi] <> Num[gr] <> ")"];

MISetArcSegmentProp[maxsegdeg_,propname_,hide_,group_]:=
        MLPut["mi_setarcsegmentprop(" <> NumC[maxsegdeg] <> QuoteC[propname]
        <> NumC[hide] <> Num[group] <> ")" ];

MIDeleteSelected[]:=MLPut["mi_deleteselected()"]

MIDeleteSelectedNodes[]:=MLPut["mi_deleteselectednodes()"]

MIDeleteSelectedLabels[]:=MLPut["mi_deleteselectedlabels()"]

MIDeleteSelectedSegments[]:=MLPut["mi_deleteselectedsegments()"]

MIDeleteSelectedArcSegments[]:=MLPut["mi_deleteselectedarcsegments()"]

MIAddMaterial[name_,mux_,muy_,Hc_,J_,c_,lamd_,phih_,lamfill_,lamtype_,phihx_,phihy_,NStrands_,WireD_]:=
        MLPut["mi_addmaterial(" <> QuoteC[name] <> NumC[mux] <> NumC[muy] <>
                NumC[Hc] <> NumC[J] <> NumC[c] <> NumC[lamd] <>
                NumC[phih] <> NumC[lamfill] <> NumC[lamtype] <> NumC[phihx] <>
                NumC[phihy] <> NumC[NStrands] <> Num[WireD] <> ")"]

MIAddMaterial[name_]:=
        MLPut["mi_addmaterial(" <> Quote[name] <> ")"]

MIAddMaterial[name_,mux_]:=
        MLPut["mi_addmaterial(" <> QuoteC[name] <> Num[mux] <> ")"]

MIAddMaterial[name_,mux_,muy_]:=
		MLPut["mi_addmaterial(" <> QuoteC[name] <> NumC[mux] <> Num[muy] <> ")"]

MIAddMaterial[name_,mux_,muy_,Hc_]:=
        MLPut["mi_addmaterial(" <> QuoteC[name] <> NumC[mux] <> NumC[muy] <>
                Num[Hc] <> ")"]

MIAddMaterial[name_,mux_,muy_,Hc_,J_]:=
        MLPut["mi_addmaterial(" <> QuoteC[name] <> NumC[mux] <> NumC[muy] <>
                NumC[Hc] <> Num[J] <> ")"]

MIAddMaterial[name_,mux_,muy_,Hc_,J_,c_]:=
        MLPut["mi_addmaterial(" <> QuoteC[name] <> NumC[mux] <> NumC[muy] <>
                NumC[Hc] <> NumC[J] <> Num[c] <> ")"]

MIAddMaterial[name_,mux_,muy_,Hc_,J_,c_,lamd_]:=
        MLPut["mi_addmaterial(" <> QuoteC[name] <> NumC[mux] <> NumC[muy] <>
                NumC[Hc] <> NumC[J]  <> NumC[c] <> Num[lamd] <> ")"]

MIAddMaterial[name_,mux_,muy_,Hc_,J_,c_,lamd_,phih_]:=
        MLPut["mi_addmaterial(" <> QuoteC[name] <> NumC[mux] <> NumC[muy] <>
                NumC[Hc] <> NumC[J] <> NumC[c] <> NumC[lamd] <> Num[phih] <> ")"]

MIAddMaterial[name_,mux_,muy_,Hc_,J_,c_,lamd_,phih_,lamfill_]:=
        MLPut["mi_addmaterial(" <> QuoteC[name] <> NumC[mux] <> NumC[muy] <>
                NumC[Hc] <> NumC[J] <> NumC[c] <> NumC[lamd] <>
                NumC[phih] <> Num[lamfill] <> ")"]

MIAddMaterial[name_,mux_,muy_,Hc_,J_,c_,lamd_,phih_,lamfill_,lamtype_]:=
        MLPut["mi_addmaterial(" <> QuoteC[name] <> NumC[mux] <> NumC[muy] <>
                NumC[Hc] <> NumC[J] <> NumC[c] <> NumC[lamd] <>
                NumC[phih] <> NumC[lamfill] <> Num[lamtype] <> ")"]

MIAddMaterial[name_,mux_,muy_,Hc_,J_,c_,lamd_,phih_,lamfill_,lamtype_,phihx_]:=
        MLPut["mi_addmaterial(" <> QuoteC[name] <> NumC[mux] <> NumC[muy] <>
                NumC[Hc] <> NumC[J] <> NumC[c] <> NumC[lamd] <>
                NumC[phih] <> NumC[lamfill] <> NumC[lamtype] <> NumC[phihx] <>
                ")"]

MIAddMaterial[name_,mux_,muy_,Hc_,J_,c_,lamd_,phih_,lamfill_,lamtype_,phihx_,phihy_]:=
        MLPut["mi_addmaterial(" <> QuoteC[name] <> NumC[mux] <> NumC[muy] <>
                NumC[Hc] <> NumC[J] <> NumC[c] <> NumC[lamd] <>
                NumC[phih] <> NumC[lamfill] <> NumC[lamtype] <> NumC[phihx] <>
                Num[phihy] <> ")"]

MIAddMaterial[name_,mux_,muy_,Hc_,J_,c_,lamd_,phih_,lamfill_,lamtype_,phihx_,phihy_,NStrands_]:=
        MLPut["mi_addmaterial(" <> QuoteC[name] <> NumC[mux] <> NumC[muy] <>
                NumC[Hc] <> NumC[J] <> NumC[c] <> NumC[lamd] <>
                NumC[phih] <> NumC[lamfill] <> NumC[lamtype] <> NumC[phihx] <>
                NumC[phihy] <> Num[NStrands] <>  ")"]

MIClose[]:=MLPut["mi_close()"]

MIAddPointProp[name_,a_,j_]:=
        MLPut["mi_addpointprop(" <> QuoteC[name] <> NumC[a]  <> Num[j] <> ")"]

MIAddPointProp[name_]:=
        MLPut["mi_addpointprop(" <> Quote[name] <> ")"]

MIAddPointProp[name_,a_,j_]:=
        MLPut["mi_addpointprop(" <> QuoteC[name] <> Num[a] <> ")"]

MIAddCircProp[name_,i_,type_]:=
        MLPut["mi_addcircprop(" <> QuoteC[name] <> NumC[i] <> Num[type] <> ")" ];

MIAddCircProp[name_]:=
        MLPut["mi_addcircprop(" <> Quote[name] <> ")" ];

MIAddCircProp[name_,i_]:=
        MLPut["mi_addcircprop(" <> QuoteC[name] <> Num[i] <> ")" ];

MIAddBoundProp[name_,A0_,A1_,A2_,phi_,mu_,sig_,c0_,c1_,fmt_]:=
        MLPut["mi_addboundprop(" <> QuoteC[name] <> NumC[A0] <> NumC[A1] <> NumC[A2] <> NumC[phi] <> NumC[mu] <> NumC[sig] <> NumC[c0] <> NumC[c1] <> Num[fmt] <> ")" ]

MIAddBoundProp[name_]:=
        MLPut["mi_addboundprop(" <> Quote[name] <> ")" ]

MIAddBoundProp[name_,A0_]:=
        MLPut["mi_addboundprop(" <> QuoteC[name] <> Num[A0] <> ")" ]

MIAddBoundProp[name_,A0_,A1_]:=
        MLPut["mi_addboundprop(" <> QuoteC[name] <> NumC[A0] <> Num[A1] <> ")" ]

MIAddBoundProp[name_,A0_,A1_,A2_]:=
        MLPut["mi_addboundprop(" <> QuoteC[name] <> NumC[A0] <> NumC[A1] <>
                Num[A2] <> ")" ]

MIAddBoundProp[name_,A0_,A1_,A2_,phi_]:=
        MLPut["mi_addboundprop(" <> QuoteC[name] <> NumC[A0] <> NumC[A1] <>
                NumC[A2] <> Num[phi] <> ")" ]

MIAddBoundProp[name_,A0_,A1_,A2_,phi_,mu_]:=
        MLPut["mi_addboundprop(" <> QuoteC[name] <> NumC[A0] <> NumC[A1] <>
                NumC[A2] <> NumC[phi] <> Num[mu] <> ")" ]

MIAddBoundProp[name_,A0_,A1_,A2_,phi_,mu_,sig_]:=
        MLPut["mi_addboundprop(" <> QuoteC[name] <> NumC[A0] <> NumC[A1] <>
                NumC[A2] <> NumC[phi] <> NumC[mu] <> Num[sig] <> ")" ]

MIAddBoundProp[name_,A0_,A1_,A2_,phi_,mu_,sig_,c0_]:=
        MLPut["mi_addboundprop(" <> QuoteC[name] <> NumC[A0] <> NumC[A1] <>
                NumC[A2] <> NumC[phi] <> NumC[mu] <> NumC[sig] <> Num[c0] <> ")" ]

MIAddBoundProp[name_,A0_,A1_,A2_,phi_,mu_,sig_,c0_,c1_]:=
        MLPut["mi_addboundprop(" <> QuoteC[name] <> NumC[A0] <> NumC[A1] <>
                NumC[A2] <> NumC[phi] <> NumC[mu] <> NumC[sig] <> NumC[c0] <>
                Num[c1] <> ")" ]

MIAddBoundProp[name_,A0_,A1_,A2_,phi_,mu_,sig_,c0_,c1_,fmt_,ia_]:=
        MLPut["mi_addboundprop(" <> QuoteC[name] <> NumC[A0] <> NumC[A1] <> NumC[A2] <> NumC[phi] <> NumC[mu] <> NumC[sig] <> NumC[c0] <> NumC[c1] <> NumC[fmt] <> Num[ia] <>  ")" ]
		
MIAddBoundProp[name_,A0_,A1_,A2_,phi_,mu_,sig_,c0_,c1_,fmt_,ia_,oa_]:=
        MLPut["mi_addboundprop(" <> QuoteC[name] <> NumC[A0] <> NumC[A1] <> NumC[A2] <> NumC[phi] <> NumC[mu] <> NumC[sig] <> NumC[c0] <> NumC[c1] <> NumC[fmt] <> NumC[ia] <> Num[oa] <> ")" ]

MIModifyMaterial[name_,num_,val_]:=Module[{},
        If[num==0,
                MLPut["mi_modifymaterial(" <> QuoteC[name] <> NumC[num] <> Quote[val] <> ")"],
                MLPut["mi_modifymaterial(" <> QuoteC[name] <> NumC[num] <> Num[val] <> ")"]
        ]
]

MIModifyBoundProp[name_,num_,val_]:=
        If[num==0,
                MLPut["mi_modifyboundprop(" <> QuoteC[name] <> NumC[num] <> Quote[val] <> ")"],
                MLPut["mi_modifyboundprop(" <> QuoteC[name] <> NumC[num] <> Num[val] <> ")"]
        ]

MIModifyPointProp[name_,num_,val_]:=Module[{},
        If[num==0,
                MLPut["mi_modifypointprop(" <> QuoteC[name] <> NumC[num] <> Quote[val] <> ")"],
                MLPut["mi_modifypointprop(" <> QuoteC[name] <> NumC[num] <> Num[val] <> ")"];
        ]
]

MIModifyCircProp[name_,num_,val_]:=Module[{},
        If[num==0,
                MLPut["mi_modifycircprop(" <> QuoteC[name] <> NumC[num] <> Quote[val] <> ")"],
                MLPut["mi_modifycircprop(" <> QuoteC[name] <> NumC[num] <> Num[val] <> ")"]
        ]
]

MISetCurrent[name_,i_]:=MIModifyCircProp[name,1,i]

MIDeleteMaterial[n_]:=MLPut["mi_deletematerial(" <> Quote[n] <> ")" ]

MIDeleteBoundProp[n_]:=MLPut["mi_deleteboundprop(" <> Quote[n] <> ")" ]

MIDeleteCircuit[n_]:=MLPut["mi_deletecircuit(" <> Quote[n] <> ")" ]

MIDeletePointProp[n_]:=MLPut["mi_deletepointprop(" <> Quote[n] <> ")" ]

MIMoveRotate[bx_,by_,shiftangle_,editaction_]:=
        MLPut["mi_moverotate(" <> NumC[bx] <> NumC[by] <> NumC[shiftangle] <>
                Num[editaction] <> ")" ]
MIMoveRotate[{bx_,by_},shiftangle_,editaction_]:=
        MIMoveRotate[bx,by,shiftangle,editaction]

MIMoveRotate[bx_,by_,shiftangle_]:=
        MLPut["mi_moverotate(" <> NumC[bx] <> NumC[by] <> Num[shiftangle] <> ")" ]
MIMoveRotate[{bx_,by_},shiftangle_]:=MIMoveRotate[bx,by,shiftangle]

MIMoveTranslate[bx_,by_,editaction_]:=
        MLPut["mi_movetranslate(" <> NumC[bx] <> NumC[by] <> Num[editaction] <> ")" ]

MIMoveTranslate[{bx_,by_},editaction_]:=MIMoveTranslate[bx,by,editaction]

MIMoveTranslate[bx_,by_]:=
        MLPut["mi_movetranslate(" <> NumC[bx] <> Num[by] <> ")" ]

MIMoveTranslate[{bx_,by_}]:=MIMoveTranslate[bx,by]

MICopyRotate[bx_,by_,angle_,copies_,editaction_]:=
        MLPut["mi_copyrotate(" <> NumC[bx] <> NumC[by] <> NumC[angle] <>
                NumC[copies] <> Num[editaction] <> ")" ]
MICopyRotate[{bx_,by_},angle_,copies_,editaction_]:=MICopyRotate[bx,by,angle,copies,editaction]

MICopyRotate[bx_,by_,angle_,copies_]:=
        MLPut["mi_copyrotate(" <> NumC[bx] <> NumC[by] <>
                NumC[angle] <> Num[copies] <> ")" ]
MICopyRotate[{bx_,by_},angle_,copies_]:=MICopyRotate[bx,by,angle,copies]

MICopyTranslate[bx_,by_,copies_,editaction_]:=
        MLPut["mi_copytranslate(" <> NumC[bx] <> NumC[by] <>
                NumC[copies] <> Num[editaction] <> ")" ]
MICopyTranslate[{bx_,by_},copies_,editaction_]:=MICopyTranslate[bx,by,copies,editaction]

MICopyTranslate[bx_,by_,copies_]:=
        MLPut["mi_copytranslate(" <> NumC[bx] <> NumC[by] <> Num[copies] <> ")" ]
MICopyTranslate[{bx_,by_},copies_]:=MICopyTranslate[bx,by,copies]


MIMirror[x1_,y1_,x2_,y2_,editaction_]:=
        MLPut["mi_mirror(" <> NumC[x1] <> NumC[y1] <> NumC[x2] <>
                NumC[y2] <> Num[editaction] <> ")" ]
MIMirror[{x1_,y1_},{x2_,y2_},editaction_]:=MIMirror[x1,y1,x2,y2,editaction]
MIMirror[{{x1_,y1_},{x2_,y2_}},editaction_]:=MIMirror[x1,y1,x2,y2,editaction]

MIMirror[x1_,y1_,x2_,y2_]:=
        MLPut["mi_mirror(" <> NumC[x1] <> NumC[y1] <> NumC[x2] <> Num[y2] <> ")" ]
MIMirror[{x1_,y1_},{x2_,y2_}]:=MIMirror[x1,y1,x2,y2,editaction]
MIMirror[{{x1_,y1_},{x2_,y2_}}]:=MIMirror[x1,y1,x2,y2]

MIScale[bx_,by_,sc_,ea_]:=
        MLPut["mi_scale(" <> NumC[bx] <> NumC[by] <> NumC[sc] <> Num[ea] <> ")" ]
MIScale[{bx_,by_},sc_,ea_]:=MIScale[bx,by,sc,ea]

MIScale[bx_,by_,sc_]:=
        MLPut["mi_scale(" <> NumC[bx] <> NumC[by] <> Num[sc] <> ")" ]
MIScale[{bx_,by_},sc_]:=MIScale[bx,by,sc]

MISetEditMode[editmode_]:=MLPut["mi_seteditmode(" <> Quote[editmode] <> ")" ]

MISelectGroup[gr_]:=MLPut["mi_selectgroup(" <> Num[gr] <> ")" ]

MISaveBitmap[n_]:=MLPut["mi_savebitmap(" <> Quote[n] <> ")" ]

MISaveMetafile[n_]:=MLPut["mi_savemetafile(" <> Quote[n] <> ")" ]

MIAddBHPoint[name_,b_,h_]:=MLPut["mi_addbhpoint(" <> QuoteC[name] <>
        NumC[b] <> Num[h] <> ")" ]

MIAddBHPoints[name_,bh_]:=Module[{k},
        For[k=1,k<=Length[bh],k++,
                MIAddBHPoint[name,bh[[k,1]],bh[[k,2]]]
        ]
]

MIClearBHPoints[n_]:=MLPut["mi_clearbhpoints(" <> Quote[n] <> ")" ]

MIRefreshView[]:=MLPut["mi_refreshview()"]

MIShowNames[]:=MLPut["mi_shownames(1)"];

MIHideNames[]:=MLPut["mi_shownames(0)"];

MIShowGrid[]:=MLPut["mi_showgrid()"]

MIHideGrid[]:=MLPut["mi_hidegrid()"]

MISnapGridOn[]:=MLPut["mi_gridsnap(\"on\")"]

MISnapGridOff[]:=MLPut["mi_gridsnap(\"off\")"]

MISetGrid[density_,type_]:=
        MLPut["mi_setgrid(" <> NumC[density] <> Quote[type] <> ")" ]

MISetFocus[docname_]:=MLPut["mi_setfocus(" <> Quote[docname] <> ")" ]

MIReadDXF[docname_]:=MLPut["mi_readdxf(" <> Quote[docname] <> ")" ]

MISaveDXF[docname_]:=MLPut["mi_savedxf(" <> Quote[docname] <> ")" ]

(****** Derived drawing functions ******)
MIAddNode[p_]:=MIAddNode[p[[1]],p[[2]]]
MIAddBlockLabel[p_]:=MIAddBlockLabel[p[[1]],p[[2]]]
MIAddSegment[p_,q_]:=MIAddSegment[p[[1]],p[[2]],q[[1]],q[[2]]]
MIAddSegment[p_]:=MIAddSegment[p[[1]],p[[2]]]
MIAddArc[p_,q_,angle_,maxseg_]:=MIAddArc[p[[1]],p[[2]],q[[1]],q[[2]],angle,maxseg]
MIAddArc[p_,angle_,maxseg_]:=MIAddArc[p[[1]],p[[2]],angle,maxseg]
MIDrawLine[x1_,y1_,x2_,y2_]:=Module[{},
        MIAddNode[x1,y1];
        MIAddNode[x2,y2];
        MIAddSegment[x1,y1,x2,y2]
]
MIDrawLine[p_,q_]:=MIDrawLine[p[[1]],p[[2]],q[[1]],q[[2]]]
MIDrawLine[p_]:=MIDrawLine[p[[1]],p[[2]]]
MIDrawPolyLine[p_]:=Module[{k},
        For[k=1,k<Length[p],k++,
                MIDrawLine[p[[k]],p[[k+1]]]
        ]
]
MIDrawPolygon[p_]:=Module[{},
        MIDrawPolyLine[p];
        MIDrawLine[p[[1]],p[[Length[p]]]]
]
MIDrawArc[x1_,y1_,x2_,y2_,angle_,maxseg_]:=Module[{},
        MIAddNode[x1,y1];
        MIAddNode[x2,y2];
        MIAddArc[x1,y1,x2,y2,angle,maxseg]
]
MIDrawArc[p_,q_,angle_,maxseg_]:=
        MIDrawArc[p[[1]],p[[2]],q[[1]],q[[2]],angle,maxseg]
MIDrawArc[p_,angle_,maxseg_]:=
        MIDrawArc[p[[1]],p[[2]],angle,maxseg]
MIDrawRectangle[x1_,y1_,x2_,y2_]:=MIDrawPolygon[{{x1,y1},{x1,y2},{x2,y2},{x2,y1}}]
MIDrawRectangle[p_,q_]:=MIDrawRectangle[p[[1]],p[[2]],q[[1]],q[[2]]]
MIDrawRectangle[p_]:=MIDrawRectangle[p[[1]],p[[2]]]
MICreateRadius[x_,y_,r_]:=MLPut["mi_createradius(" <> NumC[x] <> NumC[y] <> Num[r] <> ")" ]
MICreateRadius[{x_,y_},r_]:=MLPut["mi_createradius(" <> NumC[x] <> NumC[y] <> Num[r] <> ")" ]

(***************************************************)

MOGetPointValues[x_,y_]:=Module[{z},
        MLPut["mlput(mo_getpointvalues(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[]
]

MOGetA[x_,y_]:=MOGetPointValues[x,y][[1]]
MOGetB[x_,y_]:=MOGetPointValues[x,y][[{2,3}]]
MOGetH[x_,y_]:=MOGetPointValues[x,y][[{6,7}]]
MOGetJ[x_,y_]:=MOGetPointValues[x,y][[{8,9}]].{1,1}
MOGetMu[x_,y_]:=MOGetPointValues[x,y][[{10,11}]]
MOGetPe[x_,y_]:=MOGetPointValues[x,y][[12]]
MOGetPh[x_,y_]:=MOGetPointValues[x,y][[13]]
MOGetFill[x_,y_]:=MOGetPointValues[x,y][[14]]
MOGetConductivity[x_,y_]:=MOGetPointValues[x,y][[4]]
MOGetEnergyDensity[x_,y_]:=MOGetPointValues[x,y][[5]]

MOGetPointValues[{x_,y_}]:=MOGetPointValues[x,y]
MOGetA[{x_,y_}]:=MOGetPointValues[x,y][[1]]
MOGetB[{x_,y_}]:=MOGetPointValues[x,y][[{2,3}]]
MOGetH[{x_,y_}]:=MOGetPointValues[x,y][[{6,7}]]
MOGetJ[{x_,y_}]:=MOGetPointValues[x,y][[{8,9}]].{1,1}
MOGetMu[{x_,y_}]:=MOGetPointValues[x,y][[{10,11}]]
MOGetPe[{x_,y_}]:=MOGetPointValues[x,y][[12]]
MOGetPh[{x_,y_}]:=MOGetPointValues[x,y][[13]]
MOGetFill[{x_,y_}]:=MOGetPointValues[x,y][[14]]
MOGetConductivity[{x_,y_}]:=MOGetPointValues[x,y][[4]]
MOGetEnergyDensity[{x_,y_}]:=MOGetPointValues[x,y][[5]]

MOLineIntegral[type_]:=Module[{z,k,n,u},
        MLPut["mlput(mo_lineintegral(" <> Num[type] <> "))"];
        MLGet[]
]

MOBlockIntegral[type_]:=Module[{z},
        MLPut["mlput(mo_blockintegral(" <> Num[type] <> "))"];
        MLGet[]
]

MOMakePlot[plottype_,numpoints_,filename_,fileformat_]:=
        MLPut["mo_makeplot(" <> NumC[plottype] <> NumC[numpoints] <>
                QuoteC[filename] <> Num[fileformat] <> ")" ]

MOMakePlot[plottype_,numpoints_,filename_]:=
        MLPut["mo_makeplot(" <> NumC[plottype] <> NumC[numpoints] <>
                Quote[filename] <> ")" ]

MOMakePlot[plottype_,numpoints_]:=
        MLPut["mo_makeplot(" <> NumC[plottype] <> Num[numpoints] <> ")" ]

MOGetProblemInfo[]:=Module[{},
        MLPut["mlput(mo_getprobleminfo())"];
        MLGet[]
]

MOGetCircuitProperties[name_]:=Module[{z},
        MLPut["mlput(mo_getcircuitproperties(" <> Quote[name] <> "))" ];
        MLGet[]
]

MOSetEditMode[mode_]:=MLPut["mo_seteditmode(" <> Quote[mode] <> ")" ]

MOSetWeightingScheme[mode_]:=MLPut["mo_setweightingscheme(" <> Quote[mode] <> ")" ]

MOSelectBlock[x_,y_]:=MLPut["mo_selectblock(" <> NumC[x] <> Num[y] <> ")" ]
MOSelectBlock[{x_,y_}]:=MOSelectBlock[x,y]

MOGroupSelectBlock[n_]:=MLPut["mo_groupselectblock(" <> Num[n] <> ")" ]
MOGroupSelectBlock[]:=MLPut["mo_groupselectblock()" ]

MOAddContour[x_,y_]:=MLPut["mo_addcontour(" <> NumC[x] <> Num[y] <> ")" ]
MOAddContour[{x_,y_}]:=MOAddContour[x,y]

MOBendContour[tta_,dtta_]:=MLPut["mo_selectblock(" <> NumC[tta] <> Num[dtta] <> ")" ]

MOSelectPoint[x_,y_]:=MLPut["mo_selectpoint(" <> NumC[x] <> Num[y] <> ")" ]
MOSelectPoint[{x_,y_}]:=MOSelectPoint[x,y]

MOClearContour[]:=MLPut["mo_clearcontour()"]

MOClearBlock[]:=MLPut["mo_clearblock()"]

MOShowMesh[]:=MLPut["mo_showmesh()"]

MOHideMesh[]:=MLPut["mo_hidemesh()"]

MOShowPoints[]:=MLPut["mo_showpoints()"]

MOHidePoints[]:=MLPut["mo_hidepoints()"]

MOSmooth[flag]:=MLPut["mo_smooth(" <> Quote[flag] <> ")" ]

MOSmoothOn[]:=MLPut["mo_smooth(\"on\")"]

MOSmoothOff[]:=MLPut["mo_smooth(\"off\")"]

MOShowGrid[]:=MLPut["mo_showgrid()"]

MOHideGrid[]:=MLPut["mo_hidegrid()"]

MOSnapGrid[flag]:=MLPut["mo_gridsnap(" <> Quote[flag] <> ")" ]

MOSnapGridOn[]:=MLPut["mo_gridsnap(\"on\")"]

MOSnapGridOff[]:=MLPut["mo_gridsnap(\"off\")"]

MOSetGrid[density_,type_]:=
        MLPut["mo_setgrid(" <> NumC[density] <> Quote[type] <> ")" ]

MOHideDensityPlot[]:=MLPut["mo_hidedensityplot()"]

MOShowDensityPlot[legend_,gscale_,bu_,bl_,type_]:=
        MLPut["mo_showdensityplot(" <> NumC[legend] <> NumC[gscale] <>
                NumC[bu] <> NumC[bl] <> Quote[type] <> ")" ]

MOHideContourPlot[]:=MLPut["mo_hidecontourplot()"]

MOShowContourPlot[numcontours_,Al,Au,type]:=
        MLPut["mo_showcontourplot(" <> NumC[numcontours] <> NumC[Al] <>
                NumC[Au] <> Quote[type] <> ")" ]

MOClose[]:=MLPut["mo_close()"]

MORefreshView[]:=MLPut["mo_refreshview()"]

MOReload[]:=MLPutWait["mo_reload()"];

MOSaveBitmap[fn_]:=MLPut["mo_savebitmap(" <> Quote[fn] <> ")" ]

MOSaveMetafile[fn_]:=MLPut["mo_savemetafile(" <> Quote[fn] <> ")" ]

MOShowNames[flag]:=MLPut["mo_shownames(" <> Num[flag] <> ")" ]

MOShowNames[]:=MLPut["mo_shownames(1)"]

MOHideNames[]:=MLPut["mo_shownames(0)"]

(********************************************************************)
EISaveAs[fn_] := MLPut["ei_saveas(" <> Quote[fn] <> ")"]

EICreateMesh[]:=Module[{},
	MLPut["ei_createmesh()"];
	MLGet[]
]

EIShowMesh[]:=MLPut["ei_showmesh()"]

EIPurgeMesh[]:=MLPut["ei_purgemesh()"]

EIProbDef[units_, type_, precision_, depth_, minangle_] :=
        MLPut["ei_probdef(" <> QuoteC[units] <>
                QuoteC[type] <> NumC[precision] <> NumC[depth] <>
                Num[minangle] <> ")"]

EIAnalyze[]:=MLPutWait["ei_analyze()"];

EIAnalyse[]:=EIAnalyze[]

EIAnalyze[n_]:=MLPutWait["ei_analyze(" <> Num[n] <> ")"];

EIAnalyse[n_]:=EIAnalyze[n]

EILoadSolution[]:=MLPutWait["ei_loadsolution()"];

EIAddNode[x_,y_]:=MLPut["ei_addnode(" <> NumC[x] <> Num[y] <> ")"]

EIAddBlockLabel[x_,y_]:=MLPut["ei_addblocklabel(" <> NumC[x] <> Num[y] <> ")"]

EIAddSegment[x1_,y1_,x2_,y2_]:=MLPut["ei_addsegment(" <> NumC[x1] <> NumC[y1]
        <> NumC[x2] <> Num[y2] <> ")"]

EIAddArc[x1_,y1_,x2_,y2_,angle_,maxseg_]:=MLPut["ei_addarc(" <> NumC[x1] <> NumC[y1]
        <> NumC[x2] <> NumC[y2] <> NumC[angle] <> Num[maxseg] <> ")"]

EISelectNode[x_,y_]:=Module[{},
        MLPut["mlput(ei_selectnode(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[];
]
EISelectNode[{x_,y_}]:=EISelectNode[x,y]

EISelectLabel[x_,y_]:=Module[{},
        MLPut["mlput(ei_selectlabel(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[];
]
EISelectLabel[{x_,y_}]:=EISelectLabel[x,y]

EISelectSegment[x_,y_]:=Module[{},
        MLPut["mlput(ei_selectsegment(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[];
]
EISelectSegment[{x_,y_}]:=EISelectSegment[x,y]

EISelectArcSegment[x_,y_]:=Module[{},
        MLPut["mlput(ei_selectarcsegment(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[];
]
EISelectArcSegment[{x_,y_}]:=EISelectArcSegment[x,y]

EIClearSelected[]:=MLPut["ei_clearselected()"]

EISetNodeProp[nodeprop_,groupno_,inconductor_]:=
        MLPut["ei_setnodeprop(" <> QuoteC[nodeprop] <> NumC[groupno] <> Quote[inconductor] <> ")"]

EISetBlockProp[blockname_,automesh_,meshsize_,group_]:=
        MLPut["ei_setblockprop(" <> QuoteC[blockname] <> NumC[automesh] <>
                NumC[meshsize] <> Num[group] <> ")"]

EISetSegmentProp[pn_,es_,am_,hi_,gr_, inconductor_]:=
        MLPut["ei_setsegmentprop(" <> QuoteC[pn] <> NumC[es] <>
                NumC[am] <> NumC[hi] <> NumC[gr] <> Quote[inconductor] <> ")"];

EISetArcSegmentProp[maxsegdeg_,propname_,hide_,group_,incond_]:=
        MLPut["ei_setarcsegmentprop(" <> NumC[maxsegdeg] <> QuoteC[propname]
        <> NumC[hide] <> NumC[group] <> Quote[incond] <> ")" ]

EIDeleteSelected[]:=MLPut["ei_deleteselected()"]

EIDeleteSelectedNodes[]:=MLPut["ei_deleteselectednodes()"]

EIDeleteSelectedLabels[]:=MLPut["ei_deleteselectedlabels()"]

EIDeleteSelectedSegments[]:=MLPut["ei_deleteselectedsegments()"]

EIDeleteSelectedArcSegments[]:=MLPut["ei_deleteselectedarcsegments()"]

EIAddMaterial[name_,ex_,ey_,qv_]:=
        MLPut["ei_addmaterial(" <> QuoteC[name] <>
                NumC[ex] <> NumC[ey] <> Num[qv] <> ")" ]

EIAddMaterial[name_,ex_]:=
        MLPut["ei_addmaterial(" <> QuoteC[name] <>
                Num[ex] <> ")" ]

EIAddMaterial[name_,ex_,ey_]:=
        MLPut["ei_addmaterial(" <> QuoteC[name] <>
                NumC[ex] <> Num[ey] <> ")" ]

EIAddMaterial[name_]:=
        MLPut["ei_addmaterial(" <> Quote[name] <> ")" ]

EIClose[]:=MLPut["ei_close()"]

EIAddPointProp[name_,vp_,qp_]:=
        MLPut["ei_addpointprop(" <> QuoteC[name] <> NumC[vp] <> Num[qp] <> ")"]

EIAddPointProp[name_]:=
        MLPut["ei_addpointprop(" <> Quote[name] <> ")"]

EIAddPointProp[name_,vp_]:=
        MLPut["ei_addpointprop(" <> QuoteC[name] <> Num[vp] <> ")"]

EIAddConductorProp[name_,vc_,qc_,type_]:=
        MLPut["ei_addconductorprop(" <> QuoteC[name] <> NumC[vc] <> NumC[qc]
                <> Num[type] <> ")" ];

EIAddConductorProp[name_]:=
        MLPut["ei_addconductorprop(" <> Quote[name] <> ")" ];

EIAddConductorProp[name_,vc_]:=
        MLPut["ei_addconductorprop(" <> QuoteC[name] <> Num[vc] <> ")" ];

EIAddConductorProp[name_,vc_,qc_]:=
        MLPut["ei_addconductorprop(" <> QuoteC[name] <> NumC[vc] <> Num[qc]
                <> ")" ];

EIAddBoundProp[name_,vs_,qs_,c0_,c1_,fmt_]:=
        MLPut["ei_addboundprop(" <> QuoteC[name] <> NumC[vs] <> NumC[qs] <>
                NumC[c0] <> NumC[c1] <> Num[fmt] <> ")" ]

EIAddBoundProp[name_]:=
        MLPut["ei_addboundprop(" <> Quote[name] <> ")" ]

EIAddBoundProp[name_,vs_]:=
        MLPut["ei_addboundprop(" <> QuoteC[name] <> Num[vs] <> ")" ]

EIAddBoundProp[name_,vs_,qs_]:=
        MLPut["ei_addboundprop(" <> QuoteC[name] <> NumC[vs] <> Num[qs] <> ")" ]

EIAddBoundProp[name_,vs_,qs_,c0_]:=
        MLPut["ei_addboundprop(" <> QuoteC[name] <> NumC[vs] <> NumC[qs] <> Num[c0] <> ")" ]

EIAddBoundProp[name_,vs_,qs_,c0_,c1_]:=
        MLPut["ei_addboundprop(" <> QuoteC[name] <> NumC[vs] <> NumC[qs] <> NumC[c0] <> Num[c1] <> ")" ]

EIModifyMaterial[name_,num_,val_]:=
        If[num==0,
                MLPut["ei_modifymaterial(" <> QuoteC[name] <> NumC[num] <> Quote[val] <> ")"],
                MLPut["ei_modifymaterial(" <> QuoteC[name] <> NumC[num] <> Num[val] <> ")"]
        ]

EIModifyBoundProp[name_,num_,val_]:=
        If[num==0,
                MLPut["ei_modifyboundprop(" <> QuoteC[name] <> NumC[num] <> Quote[val] <> ")"],
                MLPut["ei_modifyboundprop(" <> QuoteC[name] <> NumC[num] <> Num[val] <> ")"]
        ]

EIModifyPointProp[name_,num_,val_]:=
        If[num==0,
                MLPut["ei_modifypointprop(" <> QuoteC[name] <> NumC[num] <> Quote[val] <> ")"],
                MLPut["ei_modifypointprop(" <> QuoteC[name] <> NumC[num] <> Num[val] <> ")"]
        ]

EIModifyConductorProp[name_,num_,val_]:=
        If[num==0,
                MLPut["ei_modifyconductorprop(" <> QuoteC[name] <> NumC[num] <> Quote[val] <> ")"],
                MLPut["ei_modifyconductorprop(" <> QuoteC[name] <> NumC[num] <> Num[val] <> ")"]
        ]

EIDeleteMaterial[n_]:=MLPut["ei_deletematerial(" <> Quote[n] <> ")" ]

EIDeleteBoundProp[n_]:=MLPut["ei_deleteboundprop(" <> Quote[n] <> ")" ]

EIDeleteConductor[n_]:=MLPut["ei_deleteconductor(" <> Quote[n] <> ")" ]

EIDeletePointProp[n_]:=MLPut["ei_deletepointprop(" <> Quote[n] <> ")" ]

EIMoveRotate[bx_,by_,shiftangle_,editaction_]:=
        MLPut["ei_moverotate(" <> NumC[bx] <> NumC[by] <> NumC[shiftangle] <>
                Num[editaction] <> ")" ]
EIMoveRotate[{bx_,by_},shiftangle_,editaction_]:=EIMoveRotate[bx,by,shiftangle,editaction]

EIMoveRotate[bx_,by_,shiftangle_]:=
        MLPut["ei_moverotate(" <> NumC[bx] <> NumC[by] <> Num[shiftangle] <> ")" ]
EIMoveRotate[{bx_,by_},shiftangle_]:=EIMoveRotate[bx,by,shiftangle]

EIMoveTranslate[bx_,by_,editaction_]:=
        MLPut["ei_movetranslate(" <> NumC[bx] <> NumC[by] <> Num[editaction] <> ")" ]
EIMoveTranslate[{bx_,by_},editaction_]:=EIMoveTranslate[bx,by,editaction]

EIMoveTranslate[bx_,by_]:=
        MLPut["ei_movetranslate(" <> NumC[bx] <> Num[by] <> ")" ]
EIMoveTranslate[{bx_,by_}]:=EIMoveTranslate[bx,by]

EICopyRotate[bx_,by_,angle_,copies_,editaction_]:=
        MLPut["ei_copyrotate(" <> NumC[bx] <> NumC[by] <> NumC[angle] <>
                NumC[copies] <> Num[editaction] <> ")" ]
EICopyRotate[{bx_,by_},angle_,copies_,editaction_]:=EICopyRotate[bx,by,angle,copies,editaction]

EICopyRotate[bx_,by_,angle_,copies_]:=
        MLPut["ei_copyrotate(" <> NumC[bx] <> NumC[by] <>
                NumC[angle] <> Num[copies] <> ")" ]
EICopyRotate[{bx_,by_},angle_,copies_]:=EICopyRotate[bx,by,angle,copies]

EICopyTranslate[bx_,by_,copies_,editaction_]:=
        MLPut["ei_copytranslate(" <> NumC[bx] <> NumC[by] <>
                NumC[copies] <> Num[editaction] <> ")" ]
EICopyTranslate[{bx_,by_},copies_,editaction_]:=EICopyTranslate[bx,by,copies,editaction]

EICopyTranslate[bx_,by_,copies_]:=
        MLPut["ei_copytranslate(" <> NumC[bx] <> NumC[by] <> Num[copies] <> ")" ]
EICopyTranslate[{bx_,by_},copies_]:=EICopyTranslate[bx,by,copies]

EIMirror[x1_,y1_,x2_,y2_,editaction_]:=
        MLPut["ei_mirror(" <> NumC[x1] <> NumC[y1] <> NumC[x2] <>
                NumC[y2] <> Num[editaction] <> ")" ]
EIMirror[{x1_,y1_},{x2_,y2_},editaction_]:=EIMirror[x1,y1,x2,y2,editaction]
EIMirror[{{x1_,y1_},{x2_,y2_}},editaction_]:=EIMirror[x1,y1,x2,y2,editaction]

EIMirror[x1_,y1_,x2_,y2_]:=
        MLPut["ei_mirror(" <> NumC[x1] <> NumC[y1] <> NumC[x2] <> Num[y2] <> ")" ]
EIMirror[{x1_,y1_},{x2_,y2_}]:=EIMirror[x1,y1,x2,y2]
EIMirror[{{x1_,y1_},{x2_,y2_}}]:=EIMirror[x1,y1,x2,y2]

EIScale[bx_,by_,sc_,ea_]:=
        MLPut["ei_scale(" <> NumC[bx] <> NumC[by] <> NumC[sc] <> Num[ea] <> ")" ]
EIScale[{bx_,by_},sc_,ea_]:=EIScale[bx,by,sc,ea]

EIScale[bx_,by_,sc_]:=
        MLPut["ei_scale(" <> NumC[bx] <> NumC[by] <> Num[sc] <> ")" ]
EIScale[{bx_,by_},sc_]:=EIScale[bx,by,sc]

EISetEditMode[editmode_]:=MLPut["ei_seteditmode(" <> Quote[editmode] <> ")" ]

EISelectGroup[gr_]:=MLPut["ei_selectgroup(" <> Num[gr] <> ")" ]

EISaveBitmap[n_]:=MLPut["ei_savebitmap(" <> Quote[n] <> ")" ]

EISaveMetafile[n_]:=MLPut["ei_savemetafile(" <> Quote[n] <> ")" ]

EIRefreshView[]:=MLPut["ei_refreshview()"]

EIShowNames[]:=MLPut["ei_shownames(1)"];

EIHideNames[]:=MLPut["ei_shownames(0)"];

EIShowGrid[]:=MLPut["ei_showgrid()"]

EIHideGrid[]:=MLPut["ei_hidegrid()"]

EISnapGridOn[]:=MLPut["ei_gridsnap(\"on\")"]

EISnapGridOff[]:=MLPut["ei_gridsnap(\"off\")"]

EISetGrid[density_,type_]:=
        MLPut["ei_setgrid(" <> NumC[density] <> Quote[type] <> ")" ]

EISetFocus[docname_]:=MLPut["ei_setfocus(" <> Quote[docname] <> ")" ]

EIReadDXF[docname_]:=MLPut["ei_readdxf(" <> Quote[docname] <> ")" ]

EISaveDXF[docname_]:=MLPut["ei_savedxf(" <> Quote[docname] <> ")" ]

(****** Derived drawing functions ******)
EIAddNode[p_]:=EIAddNode[p[[1]],p[[2]]]
EIAddBlockLabel[p_]:=EIAddBlockLabel[p[[1]],p[[2]]]
EIAddSegment[p_,q_]:=EIAddSegment[p[[1]],p[[2]],q[[1]],q[[2]]]
EIAddSegment[p_]:=EIAddSegment[p[[1]],p[[2]]]
EIAddArc[p_,q_,angle_,maxseg_]:=EIAddArc[p[[1]],p[[2]],q[[1]],q[[2]],angle,maxseg]
EIAddArc[p_,angle_,maxseg_]:=EIAddArc[p[[1]],p[[2]],angle,maxseg]
EIDrawLine[x1_,y1_,x2_,y2_]:=Module[{},
        EIAddNode[x1,y1];
        EIAddNode[x2,y2];
        EIAddSegment[x1,y1,x2,y2]
]
EIDrawLine[p_,q_]:=EIDrawLine[p[[1]],p[[2]],q[[1]],q[[2]]]
EIDrawLine[p_]:=EIDrawLine[p[[1]],p[[2]]]
EIDrawPolyLine[p_]:=Module[{k},
        For[k=1,k<Length[p],k++,
                EIDrawLine[p[[k]],p[[k+1]]]
        ]
]
EIDrawPolygon[p_]:=Module[{},
        EIDrawPolyLine[p];
        EIDrawLine[p[[1]],p[[Length[p]]]]
]
EIDrawArc[x1_,y1_,x2_,y2_,angle_,maxseg_]:=Module[{},
        EIAddNode[x1,y1];
        EIAddNode[x2,y2];
        EIAddArc[x1,y1,x2,y2,angle,maxseg]
]
EIDrawArc[p_,q_,angle_,maxseg_]:=
        EIDrawArc[p[[1]],p[[2]],q[[1]],q[[2]],angle,maxseg]
EIDrawArc[p_,angle_,maxseg_]:=
        EIDrawArc[p[[1]],p[[2]],angle,maxseg]
EIDrawRectangle[x1_,y1_,x2_,y2_]:=EIDrawPolygon[{{x1,y1},{x1,y2},{x2,y2},{x2,y1}}]
EIDrawRectangle[p_,q_]:=EIDrawRectangle[p[[1]],p[[2]],q[[1]],q[[2]]]
EIDrawRectangle[p_]:=EIDrawRectangle[p[[1]],p[[2]]]
EICreateRadius[x_,y_,r_]:=MLPut["ei_createradius(" <> NumC[x] <> NumC[y] <> Num[r] <> ")" ]
EICreateRadius[{x_,y_},r_]:=MLPut["ei_createradius(" <> NumC[x] <> NumC[y] <> Num[r] <> ")" ]

(***************************************************)

EOGetPointValues[x_,y_]:=Module[{z},
        MLPut["mlput(eo_getpointvalues(" <> NumC[x] <> Num[y] <> "))"];
        MLGetNC[]
]
EOGetPointValues[{x_,y_}]:=EOGetPointValues[x,y]

EOGetV[x_,y_]:=EOGetPointValues[x,y][[1]]
EOGetD[x_,y_]:=EOGetPointValues[x,y][[{2,3}]]
EOGetE[x_,y_]:=EOGetPointValues[x,y][[{4,5}]]
EOGetPerm[x_,y_]:=EOGetPointValues[x,y][[{6,7}]]
EOGetEnergyDensity[x_,y_]:=EOGetPointValues[x,y][[8]]

EOGetV[{x_,y_}]:=EOGetPointValues[x,y][[1]]
EOGetD[{x_,y_}]:=EOGetPointValues[x,y][[{2,3}]]
EOGetE[{x_,y_}]:=EOGetPointValues[x,y][[{4,5}]]
EOGetPerm[{x_,y_}]:=EOGetPointValues[x,y][[{6,7}]]
EOGetEnergyDensity[{x_,y_}]:=EOGetPointValues[x,y][[8]]

EOLineIntegral[type_]:=Module[{},
        MLPut["mlput(eo_lineintegral(" <> Num[type] <> "))"];
        MLGetNC[]
]

EOBlockIntegral[type_]:=Module[{},
        MLPut["mlput(eo_blockintegral(" <> Num[type] <> "))"];
        MLGetNC[]
]

EOMakePlot[plottype_,numpoints_,filename_,fileformat_]:=
        MLPut["eo_makeplot(" <> NumC[plottype] <> NumC[numpoints] <>
                QuoteC[filename] <> Num[fileformat] <> ")" ]

EOMakePlot[plottype_,numpoints_,filename_]:=
        MLPut["eo_makeplot(" <> NumC[plottype] <> NumC[numpoints] <>
                Quote[filename] <> ")" ]

EOMakePlot[plottype_,numpoints_]:=
        MLPut["eo_makeplot(" <> NumC[plottype] <> Num[numpoints] <> ")" ]

EOGetProblemInfo[]:=Module[{},
        MLPut["mlput(eo_getprobleminfo())"];
        MLGetNC[]
]

EOGetConductorProperties[name_]:=Module[{},
        MLPut["mlput(eo_getconductorproperties(" <> Quote[name] <> "))" ];
        MLGetNC[]
]

EOSetEditMode[mode_]:=MLPut["eo_seteditmode(" <> Quote[mode] <> ")" ]

EOSelectBlock[x_,y_]:=MLPut["eo_selectblock(" <> NumC[x] <> Num[y] <> ")" ]
EOSelectBlock[{x_,y_}]:=EOSelectBlock[x,y]

EOGroupSelectBlock[n_]:=MLPut["eo_groupselectblock(" <> Num[n] <> ")" ]
EOGroupSelectBlock[]:=MLPut["eo_groupselectblock()" ]

EOAddContour[x_,y_]:=MLPut["eo_addcontour(" <> NumC[x] <> Num[y] <> ")" ]
EOAddContour[{x_,y_}]:=EOAddContour[x,y]

EOBendContour[tta_,dtta_]:=MLPut["eo_selectblock(" <> NumC[tta] <> Num[dtta] <> ")" ]

EOSelectPoint[x_,y_]:=MLPut["eo_selectpoint(" <> NumC[x] <> Num[y] <> ")" ]
EOSelectPoint[{x_,y_}]:=EOSelectPoint[x,y]

EOClearContour[]:=MLPut["eo_clearcontour()"]

EOClearBlock[]:=MLPut["eo_clearblock()"]

EOShowMesh[]:=MLPut["eo_showmesh()"]

EOHideMesh[]:=MLPut["eo_hidemesh()"]

EOShowPoints[]:=MLPut["eo_showpoints()"]

EOHidePoints[]:=MLPut["eo_hidepoints()"]

EOSmooth[flag]:=MLPut["eo_smooth(" <> Quote[flag] <> ")" ]

EOSmoothOn[]:=MLPut["eo_smooth(\"on\")"]

EOSmoothOff[]:=MLPut["eo_smooth(\"off\")"]

EOShowGrid[]:=MLPut["eo_showgrid()"]

EOHideGrid[]:=MLPut["eo_hidegrid()"]

EOSnapGrid[flag]:=MLPut["eo_gridsnap(" <> Quote[flag] <> ")" ]

EOSnapGridOn[]:=MLPut["eo_gridsnap(\"on\")"]

EOSnapGridOff[]:=MLPut["eo_gridsnap(\"off\")"]

EOSetGrid[density_,type_]:=
        MLPut["eo_setgrid(" <> NumC[density] <> Quote[type] <> ")" ]

EOHideDensityPlot[]:=MLPut["eo_hidedensityplot()"]

EOShowDensityPlot[legend_,gscale_,type_,bu_,bl_]:=
        MLPut["eo_showdensityplot(" <> NumC[legend] <> NumC[gscale] <> NumC[type] <>
                NumC[bu] <> Num[bl] <> ")" ]

EOHideContourPlot[]:=MLPut["eo_hidecontourplot()"]

EOShowContourPlot[numcontours_,Al,Au]:=
        MLPut["eo_showcontourplot(" <> NumC[numcontours] <> NumC[Al] <>
                Num[Au] <> ")" ]

EOClose[]:=MLPut["eo_close()"]

EORefreshView[]:=MLPut["eo_refreshview()"]

EOReload[]:=MLPutWait["eo_reload()"];

EOSaveBitmap[fn_]:=MLPut["eo_savebitmap(" <> Quote[fn] <> ")" ]

EOSaveMetafile[fn_]:=MLPut["eo_savemetafile(" <> Quote[fn] <> ")" ]

EOShowNames[flag]:=MLPut["eo_shownames(" <> Num[flag] <> ")" ]

EOShowNames[]:=MLPut["eo_shownames(1)"]

EOHideNames[]:=MLPut["eo_shownames(0)"]

(********************************************************************)
HISaveAs[fn_] := MLPut["hi_saveas(" <> Quote[fn] <> ")"]

HICreateMesh[]:=Module[{},
	MLPut["hi_createmesh()"];
	MLGet[]
]

HIShowMesh[]:=MLPut["hi_showmesh()"]

HIPurgeMesh[]:=MLPut["hi_purgemesh()"]

HIProbDef[units_, type_, precision_, depth_, minangle_] :=
        MLPut["hi_probdef(" <> QuoteC[units] <>
                QuoteC[type] <> NumC[precision] <> NumC[depth] <>
                Num[minangle] <> ")"]

HIProbDef[units_, type_, precision_, depth_, minangle_, prevsoln_, dt_] :=
        MLPut["hi_probdef(" <> QuoteC[units] <>
                QuoteC[type] <> NumC[precision] <> NumC[depth] <>
                NumC[minangle] <> QuoteC[prevsoln] <> Num[dt] <> ")"]

HIAnalyze[]:=MLPutWait["hi_analyze()"];

HIAnalyse[]:=HIAnalyze[]

HIAnalyze[n_]:=MLPutWait["hi_analyze(" <> Num[n] <> ")"];

HIAnalyse[n_]:=HIAnalyze[n]

HILoadSolution[]:=MLPutWait["hi_loadsolution()"];

HIAddNode[x_,y_]:=MLPut["hi_addnode(" <> NumC[x] <> Num[y] <> ")"]

HIAddBlockLabel[x_,y_]:=MLPut["hi_addblocklabel(" <> NumC[x] <> Num[y] <> ")"]

HIAddSegment[x1_,y1_,x2_,y2_]:=MLPut["hi_addsegment(" <> NumC[x1] <> NumC[y1]
        <> NumC[x2] <> Num[y2] <> ")"]

HIAddArc[x1_,y1_,x2_,y2_,angle_,maxseg_]:=MLPut["hi_addarc(" <> NumC[x1] <> NumC[y1]
        <> NumC[x2] <> NumC[y2] <> NumC[angle] <> Num[maxseg] <> ")"]

HISelectNode[x_,y_]:=Module[{},
        MLPut["mlput(hi_selectnode(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[];
]
HISelectNode[{x_,y_}]:=HISelectNode[x,y]

HISelectLabel[x_,y_]:=Module[{},
        MLPut["mlput(hi_selectlabel(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[];
]
HISelectLabel[{x_,y_}]:=HISelectLabel[x,y]

HISelectSegment[x_,y_]:=Module[{},
        MLPut["mlput(hi_selectsegment(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[];
]
HISelectSegment[{x_,y_}]:=HISelectSegment[x,y]

HISelectArcSegment[x_,y_]:=Module[{},
        MLPut["mlput(hi_selectarcsegment(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[];
]
HISelectArcSegment[{x_,y_}]:=HISelectArcSegment[x,y]

HIClearSelected[]:=MLPut["hi_clearselected()"]

HISetNodeProp[nodeprop_,groupno_,inconductor_]:=
        MLPut["hi_setnodeprop(" <> QuoteC[nodeprop] <> NumC[groupno] <> Quote[inconductor] <> ")"]

HISetBlockProp[blockname_,automesh_,meshsize_,group_]:=
        MLPut["hi_setblockprop(" <> QuoteC[blockname] <> NumC[automesh] <>
                NumC[meshsize] <> Num[group] <> ")"]

HISetSegmentProp[pn_,es_,am_,hi_,gr_, inconductor_]:=
        MLPut["hi_setsegmentprop(" <> QuoteC[pn] <> NumC[es] <>
                NumC[am] <> NumC[hi] <> NumC[gr] <> Quote[inconductor] <> ")"];

HISetArcSegmentProp[maxsegdeg_,propname_,hide_,group_,incond_]:=
        MLPut["hi_setarcsegmentprop(" <> NumC[maxsegdeg] <> QuoteC[propname]
        <> NumC[hide] <> NumC[group] <> Quote[incond] <> ")" ]

HIDeleteSelected[]:=MLPut["hi_deleteselected()"]

HIDeleteSelectedNodes[]:=MLPut["hi_deleteselectednodes()"]

HIDeleteSelectedLabels[]:=MLPut["hi_deleteselectedlabels()"]

HIDeleteSelectedSegments[]:=MLPut["hi_deleteselectedsegments()"]

HIDeleteSelectedArcSegments[]:=MLPut["hi_deleteselectedarcsegments()"]

HIAddMaterial[name_,ex_,ey_,qv_,kt_]:=
        MLPut["hi_addmaterial(" <> QuoteC[name] <>
                NumC[ex] <> NumC[ey] <> NumC[qv] <> Num[kt] <> ")" ]

HIAddMaterial[name_,ex_,ey_,qv_]:=
        MLPut["hi_addmaterial(" <> QuoteC[name] <>
                NumC[ex] <> NumC[ey] <> Num[qv] <> ")" ]

HIAddMaterial[name_,ex_]:=
        MLPut["hi_addmaterial(" <> QuoteC[name] <>
                Num[ex] <> ")" ]

HIAddMaterial[name_,ex_,ey_]:=
        MLPut["hi_addmaterial(" <> QuoteC[name] <>
                NumC[ex] <> Num[ey] <> ")" ]

HIAddMaterial[name_]:=
        MLPut["hi_addmaterial(" <> Quote[name] <> ")" ]

HIClose[]:=MLPut["hi_close()"]

HIAddPointProp[name_,vp_,qp_]:=
        MLPut["hi_addpointprop(" <> QuoteC[name] <> NumC[vp] <> Num[qp] <> ")"]

HIAddPointProp[name_]:=
        MLPut["hi_addpointprop(" <> Quote[name] <> ")"]

HIAddPointProp[name_,vp_]:=
        MLPut["hi_addpointprop(" <> QuoteC[name] <> Num[vp] <> ")"]

HIAddConductorProp[name_,vc_,qc_,type_]:=
        MLPut["hi_addconductorprop(" <> QuoteC[name] <> NumC[vc] <> NumC[qc]
                <> Num[type] <> ")" ];

HIAddConductorProp[name_]:=
        MLPut["hi_addconductorprop(" <> Quote[name] <> ")" ];

HIAddConductorProp[name_,vc_]:=
        MLPut["hi_addconductorprop(" <> QuoteC[name] <> Num[vc] <> ")" ];

HIAddConductorProp[name_,vc_,qc_]:=
        MLPut["hi_addconductorprop(" <> QuoteC[name] <> NumC[vc] <> Num[qc]
                <> ")" ];

HIAddBoundProp[name_,fmt_,Tset_,qs_,Tinf_,h_,bta_]:=
        MLPut["hi_addboundprop(" <> QuoteC[name] <> NumC[fmt] <> NumC[Tset] <>
                NumC[qs] <> NumC[Tinf] <> NumC[h] <> Num[bta] <> ")" ]

HIAddBoundProp[name_]:=
        MLPut["hi_addboundprop(" <> Quote[name] <> ")" ]

HIAddBoundProp[name_,fmt_]:=
        MLPut["hi_addboundprop(" <> QuoteC[name] <> Num[fmt] <> ")" ]

HIAddBoundProp[name_,fmt_,Tset_]:=
        MLPut["hi_addboundprop(" <> QuoteC[name] <> NumC[fmt] <> Num[Tset] <> ")" ]

HIAddBoundProp[name_,fmt_,Tset_,qs_]:=
        MLPut["hi_addboundprop(" <> QuoteC[name] <> NumC[fmt] <> NumC[Tset] <> Num[qs] <> ")" ]

HIAddBoundProp[name_,fmt_,Tset_,qs_,Tinf_]:=
        MLPut["hi_addboundprop(" <> QuoteC[name] <> NumC[fmt] <> NumC[Tset] <> NumC[qs] <> Num[Tinf] <> ")" ]

HIAddBoundProp[name_,fmt_,Tset_,qs_,Tinf_,h_]:=
        MLPut["hi_addboundprop(" <> QuoteC[name] <> NumC[fmt] <> NumC[Tset] <> NumC[qs] <> NumC[Tinf] <> Num[h] <> ")" ]

HIModifyMaterial[name_,num_,val_]:=
        If[num==0,
                MLPut["hi_modifymaterial(" <> QuoteC[name] <> NumC[num] <> Quote[val] <> ")"],
                MLPut["hi_modifymaterial(" <> QuoteC[name] <> NumC[num] <> Num[val] <> ")"]
        ]

HIModifyBoundProp[name_,num_,val_]:=
        If[num==0,
                MLPut["hi_modifyboundprop(" <> QuoteC[name] <> NumC[num] <> Quote[val] <> ")"],
                MLPut["hi_modifyboundprop(" <> QuoteC[name] <> NumC[num] <> Num[val] <> ")"]
        ]

HIModifyPointProp[name_,num_,val_]:=
        If[num==0,
                MLPut["hi_modifypointprop(" <> QuoteC[name] <> NumC[num] <> Quote[val] <> ")"],
                MLPut["hi_modifypointprop(" <> QuoteC[name] <> NumC[num] <> Num[val] <> ")"]
        ]

HIModifyConductorProp[name_,num_,val_]:=
        If[num==0,
                MLPut["hi_modifyconductorprop(" <> QuoteC[name] <> NumC[num] <> Quote[val] <> ")"],
                MLPut["hi_modifyconductorprop(" <> QuoteC[name] <> NumC[num] <> Num[val] <> ")"]
        ]

HIDeleteMaterial[n_]:=MLPut["hi_deletematerial(" <> Quote[n] <> ")" ]

HIDeleteBoundProp[n_]:=MLPut["hi_deleteboundprop(" <> Quote[n] <> ")" ]

HIDeleteConductor[n_]:=MLPut["hi_deleteconductor(" <> Quote[n] <> ")" ]

HIDeletePointProp[n_]:=MLPut["hi_deletepointprop(" <> Quote[n] <> ")" ]

HIMoveRotate[bx_,by_,shiftangle_,editaction_]:=
        MLPut["hi_moverotate(" <> NumC[bx] <> NumC[by] <> NumC[shiftangle] <>
                Num[editaction] <> ")" ]
HIMoveRotate[{bx_,by_},shiftangle_,editaction_]:=HIMoveRotate[bx,by,shiftangle,editaction]

HIMoveRotate[bx_,by_,shiftangle_]:=
        MLPut["hi_moverotate(" <> NumC[bx] <> NumC[by] <> Num[shiftangle] <> ")" ]
HIMoveRotate[{bx_,by_},shiftangle_]:=HIMoveRotate[bx,by,shiftangle]

HIMoveTranslate[bx_,by_,editaction_]:=
        MLPut["hi_movetranslate(" <> NumC[bx] <> NumC[by] <> Num[editaction] <> ")" ]
HIMoveTranslate[{bx_,by_},editaction_]:=HIMoveTranslate[bx,by,editaction]

HIMoveTranslate[bx_,by_]:=
        MLPut["hi_movetranslate(" <> NumC[bx] <> Num[by] <> ")" ]
HIMoveTranslate[{bx_,by_}]:=HIMoveTranslate[bx,by]

HICopyRotate[bx_,by_,angle_,copies_,editaction_]:=
        MLPut["hi_copyrotate(" <> NumC[bx] <> NumC[by] <> NumC[angle] <>
                NumC[copies] <> Num[editaction] <> ")" ]
HICopyRotate[{bx_,by_},angle_,copies_,editaction_]:=HICopyRotate[bx,by,angle,copies,editaction]

HICopyRotate[bx_,by_,angle_,copies_]:=
        MLPut["hi_copyrotate(" <> NumC[bx] <> NumC[by] <>
                NumC[angle] <> Num[copies] <> ")" ]
HICopyRotate[{bx_,by_},angle_,copies_]:=HICopyRotate[bx,by,angle,copies]

HICopyTranslate[bx_,by_,copies_,editaction_]:=
        MLPut["hi_copytranslate(" <> NumC[bx] <> NumC[by] <>
                NumC[copies] <> Num[editaction] <> ")" ]
HICopyTranslate[{bx_,by_},copies_,editaction_]:=HICopyTranslate[bx,by,copies,editaction]

HICopyTranslate[bx_,by_,copies_]:=
        MLPut["hi_copytranslate(" <> NumC[bx] <> NumC[by] <> Num[copies] <> ")" ]
HICopyTranslate[{bx_,by_},copies_]:=HICopyTranslate[bx,by,copies]

HIMirror[x1_,y1_,x2_,y2_,editaction_]:=
        MLPut["hi_mirror(" <> NumC[x1] <> NumC[y1] <> NumC[x2] <>
                NumC[y2] <> Num[editaction] <> ")" ]
HIMirror[{x1_,y1_},{x2_,y2_},editaction_]:=HIMirror[x1,y1,x2,y2,editaction]
HIMirror[{{x1_,y1_},{x2_,y2_}},editaction_]:=HIMirror[x1,y1,x2,y2,editaction]

HIMirror[x1_,y1_,x2_,y2_]:=
        MLPut["hi_mirror(" <> NumC[x1] <> NumC[y1] <> NumC[x2] <> Num[y2] <> ")" ]
HIMirror[{x1_,y1_},{x2_,y2_}]:=HIMirror[x1,y1,x2,y2]
HIMirror[{{x1_,y1_},{x2_,y2_}}]:=HIMirror[x1,y1,x2,y2]

HIScale[bx_,by_,sc_,ea_]:=
        MLPut["hi_scale(" <> NumC[bx] <> NumC[by] <> NumC[sc] <> Num[ea] <> ")" ]
HIScale[{bx_,by_},sc_,ea_]:=HIScale[bx,by,sc,ea]

HIScale[bx_,by_,sc_]:=
        MLPut["hi_scale(" <> NumC[bx] <> NumC[by] <> Num[sc] <> ")" ]
HIScale[{bx_,by_},sc_]:=HIScale[bx,by,sc]

HISetEditMode[editmode_]:=MLPut["hi_seteditmode(" <> Quote[editmode] <> ")" ]

HISelectGroup[gr_]:=MLPut["hi_selectgroup(" <> Num[gr] <> ")" ]

HISaveBitmap[n_]:=MLPut["hi_savebitmap(" <> Quote[n] <> ")" ]

HISaveMetafile[n_]:=MLPut["hi_savemetafile(" <> Quote[n] <> ")" ]

HIRefreshView[]:=MLPut["hi_refreshview()"]

HIShowNames[]:=MLPut["hi_shownames(1)"];

HIHideNames[]:=MLPut["hi_shownames(0)"];

HIShowGrid[]:=MLPut["hi_showgrid()"]

HIHideGrid[]:=MLPut["hi_hidegrid()"]

HISnapGridOn[]:=MLPut["hi_gridsnap(\"on\")"]

HISnapGridOff[]:=MLPut["hi_gridsnap(\"off\")"]

HISetGrid[density_,type_]:=
        MLPut["hi_setgrid(" <> NumC[density] <> Quote[type] <> ")" ]

HISetFocus[docname_]:=MLPut["hi_setfocus(" <> Quote[docname] <> ")" ]

HIReadDXF[docname_]:=MLPut["hi_readdxf(" <> Quote[docname] <> ")" ]

HISaveDXF[docname_]:=MLPut["hi_savedxf(" <> Quote[docname] <> ")" ]

HIAddTKPoint[name_,b_,h_]:=MLPut["hi_addtkpoint(" <> QuoteC[name] <>
        NumC[b] <> Num[h] <> ")" ]

HIAddTKPoints[name_,bh_]:=Module[{k},
        For[k=1,k<=Length[bh],k++,
                HIAddTKPoint[name,bh[[k,1]],bh[[k,2]]]
        ]
]

HIClearTKPoints[n_]:=MLPut["hi_cleartkpoints(" <> Quote[n] <> ")" ]

(****** Derived drawing functions ******)
HIAddNode[p_]:=HIAddNode[p[[1]],p[[2]]]
HIAddBlockLabel[p_]:=HIAddBlockLabel[p[[1]],p[[2]]]
HIAddSegment[p_,q_]:=HIAddSegment[p[[1]],p[[2]],q[[1]],q[[2]]]
HIAddSegment[p_]:=HIAddSegment[p[[1]],p[[2]]]
HIAddArc[p_,q_,angle_,maxseg_]:=HIAddArc[p[[1]],p[[2]],q[[1]],q[[2]],angle,maxseg]
HIAddArc[p_,angle_,maxseg_]:=HIAddArc[p[[1]],p[[2]],angle,maxseg]
HIDrawLine[x1_,y1_,x2_,y2_]:=Module[{},
        HIAddNode[x1,y1];
        HIAddNode[x2,y2];
        HIAddSegment[x1,y1,x2,y2]
]
HIDrawLine[p_,q_]:=HIDrawLine[p[[1]],p[[2]],q[[1]],q[[2]]]
HIDrawLine[p_]:=HIDrawLine[p[[1]],p[[2]]]
HIDrawPolyLine[p_]:=Module[{k},
        For[k=1,k<Length[p],k++,
                HIDrawLine[p[[k]],p[[k+1]]]
        ]
]
HIDrawPolygon[p_]:=Module[{},
        HIDrawPolyLine[p];
        HIDrawLine[p[[1]],p[[Length[p]]]]
]
HIDrawArc[x1_,y1_,x2_,y2_,angle_,maxseg_]:=Module[{},
        HIAddNode[x1,y1];
        HIAddNode[x2,y2];
        HIAddArc[x1,y1,x2,y2,angle,maxseg]
]
HIDrawArc[p_,q_,angle_,maxseg_]:=
        HIDrawArc[p[[1]],p[[2]],q[[1]],q[[2]],angle,maxseg]
HIDrawArc[p_,angle_,maxseg_]:=
        HIDrawArc[p[[1]],p[[2]],angle,maxseg]
HIDrawRectangle[x1_,y1_,x2_,y2_]:=HIDrawPolygon[{{x1,y1},{x1,y2},{x2,y2},{x2,y1}}]
HIDrawRectangle[p_,q_]:=HIDrawRectangle[p[[1]],p[[2]],q[[1]],q[[2]]]
HIDrawRectangle[p_]:=HIDrawRectangle[p[[1]],p[[2]]]
HICreateRadius[x_,y_,r_]:=MLPut["hi_createradius(" <> NumC[x] <> NumC[y] <> Num[r] <> ")" ]
HICreateRadius[{x_,y_},r_]:=MLPut["hi_createradius(" <> NumC[x] <> NumC[y] <> Num[r] <> ")" ]

(***************************************************)

HOGetPointValues[x_,y_]:=Module[{z},
        MLPut["mlput(ho_getpointvalues(" <> NumC[x] <> Num[y] <> "))"];
        MLGetNC[]
]
HOGetPointValues[{x_,y_}]:=HOGetPointValues[x,y]

HOGetT[x_,y_]:=HOGetPointValues[x,y][[1]]
HOGetF[x_,y_]:=HOGetPointValues[x,y][[{2,3}]]
HOGetG[x_,y_]:=HOGetPointValues[x,y][[{4,5}]]
HOGetK[x_,y_]:=HOGetPointValues[x,y][[{6,7}]]

HOGetT[{x_,y_}]:=HOGetPointValues[x,y][[1]]
HOGetF[{x_,y_}]:=HOGetPointValues[x,y][[{2,3}]]
HOGetG[{x_,y_}]:=HOGetPointValues[x,y][[{4,5}]]
HOGetK[{x_,y_}]:=HOGetPointValues[x,y][[{6,7}]]

HOLineIntegral[type_]:=Module[{},
        MLPut["mlput(ho_lineintegral(" <> Num[type] <> "))"];
        MLGetNC[]
]

HOBlockIntegral[type_]:=Module[{},
        MLPut["mlput(ho_blockintegral(" <> Num[type] <> "))"];
        MLGetNC[]
]

HOMakePlot[plottype_,numpoints_,filename_,fileformat_]:=
        MLPut["ho_makeplot(" <> NumC[plottype] <> NumC[numpoints] <>
                QuoteC[filename] <> Num[fileformat] <> ")" ]

HOMakePlot[plottype_,numpoints_,filename_]:=
        MLPut["ho_makeplot(" <> NumC[plottype] <> NumC[numpoints] <>
                Quote[filename] <> ")" ]

HOMakePlot[plottype_,numpoints_]:=
        MLPut["ho_makeplot(" <> NumC[plottype] <> Num[numpoints] <> ")" ]

HOGetProblemInfo[]:=Module[{},
        MLPut["mlput(ho_getprobleminfo())"];
        MLGetNC[]
]

HOGetConductorProperties[name_]:=Module[{},
        MLPut["mlput(ho_getconductorproperties(" <> Quote[name] <> "))" ];
        MLGetNC[]
]

HOSetEditMode[mode_]:=MLPut["ho_seteditmode(" <> Quote[mode] <> ")" ]

HOSelectBlock[x_,y_]:=MLPut["ho_selectblock(" <> NumC[x] <> Num[y] <> ")" ]
HOSelectBlock[{x_,y_}]:=HOSelectBlock[x,y]

HOGroupSelectBlock[n_]:=MLPut["ho_groupselectblock(" <> Num[n] <> ")" ]
HOGroupSelectBlock[]:=MLPut["ho_groupselectblock()" ]

HOAddContour[x_,y_]:=MLPut["ho_addcontour(" <> NumC[x] <> Num[y] <> ")" ]
HOAddContour[{x_,y_}]:=HOAddContour[x,y]

HOBendContour[tta_,dtta_]:=MLPut["ho_selectblock(" <> NumC[tta] <> Num[dtta] <> ")" ]

HOSelectPoint[x_,y_]:=MLPut["ho_selectpoint(" <> NumC[x] <> Num[y] <> ")" ]
HOSelectPoint[{x_,y_}]:=HOSelectPoint[x,y]

HOClearContour[]:=MLPut["ho_clearcontour()"]

HOClearBlock[]:=MLPut["ho_clearblock()"]

HOShowMesh[]:=MLPut["ho_showmesh()"]

HOHideMesh[]:=MLPut["ho_hidemesh()"]

HOShowPoints[]:=MLPut["ho_showpoints()"]

HOHidePoints[]:=MLPut["ho_hidepoints()"]

HOSmooth[flag]:=MLPut["ho_smooth(" <> Quote[flag] <> ")" ]

HOSmoothOn[]:=MLPut["ho_smooth(\"on\")"]

HOSmoothOff[]:=MLPut["ho_smooth(\"off\")"]

HOShowGrid[]:=MLPut["ho_showgrid()"]

HOHideGrid[]:=MLPut["ho_hidegrid()"]

HOSnapGrid[flag]:=MLPut["ho_gridsnap(" <> Quote[flag] <> ")" ]

HOSnapGridOn[]:=MLPut["ho_gridsnap(\"on\")"]

HOSnapGridOff[]:=MLPut["ho_gridsnap(\"off\")"]

HOSetGrid[density_,type_]:=
        MLPut["ho_setgrid(" <> NumC[density] <> Quote[type] <> ")" ]

HOHideDensityPlot[]:=MLPut["ho_hidedensityplot()"]

HOShowDensityPlot[legend_,gscale_,type_,bu_,bl_]:=
        MLPut["ho_showdensityplot(" <> NumC[legend] <> NumC[gscale] <>
                NumC[type] <> NumC[bu] <> Num[bl] <> ")" ]

HOHideContourPlot[]:=MLPut["ho_hidecontourplot()"]

HOShowContourPlot[numcontours_,Al,Au]:=
        MLPut["ho_showcontourplot(" <> NumC[numcontours] <> NumC[Al] <>
                Num[Au] <> ")" ]

HOClose[]:=MLPut["ho_close()"]

HORefreshView[]:=MLPut["ho_refreshview()"]

HOReload[]:=MLPutWait["ho_reload()"];

HOSaveBitmap[fn_]:=MLPut["ho_savebitmap(" <> Quote[fn] <> ")" ]

HOSaveMetafile[fn_]:=MLPut["ho_savemetafile(" <> Quote[fn] <> ")" ]

HOShowNames[flag]:=MLPut["ho_shownames(" <> Num[flag] <> ")" ]

HOShowNames[]:=MLPut["ho_shownames(1)"]

HOHideNames[]:=MLPut["ho_shownames(0)"]

(********************************************************************)
CISaveAs[fn_] := MLPut["ci_saveas(" <> Quote[fn] <> ")"]

CICreateMesh[]:=Module[{},
	MLPut["ci_createmesh()"];
	MLGet[]
]

CIShowMesh[]:=MLPut["ci_showmesh()"]

CIPurgeMesh[]:=MLPut["ci_purgemesh()"]

CIProbDef[units_, type_, frequency_, precision_, depth_, minangle_] :=
        MLPut["ci_probdef(" <> QuoteC[units] <>
                QuoteC[type] <> NumC[frequency] <> NumC[precision] <> NumC[depth] <>
                Num[minangle] <> ")"]

CIAnalyze[]:=MLPutWait["ci_analyze()"];

CIAnalyse[]:=CIAnalyze[]

CIAnalyze[n_]:=MLPutWait["ci_analyze(" <> Num[n] <> ")"];

CIAnalyse[n_]:=CIAnalyze[n]

CILoadSolution[]:=MLPutWait["ci_loadsolution()"];

CIAddNode[x_,y_]:=MLPut["ci_addnode(" <> NumC[x] <> Num[y] <> ")"]

CIAddBlockLabel[x_,y_]:=MLPut["ci_addblocklabel(" <> NumC[x] <> Num[y] <> ")"]

CIAddSegment[x1_,y1_,x2_,y2_]:=MLPut["ci_addsegment(" <> NumC[x1] <> NumC[y1]
        <> NumC[x2] <> Num[y2] <> ")"]

CIAddArc[x1_,y1_,x2_,y2_,angle_,maxseg_]:=MLPut["ci_addarc(" <> NumC[x1] <> NumC[y1]
        <> NumC[x2] <> NumC[y2] <> NumC[angle] <> Num[maxseg] <> ")"]

CISelectNode[x_,y_]:=Module[{},
        MLPut["mlput(ci_selectnode(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[];
]
CISelectNode[{x_,y_}]:=CISelectNode[x,y]

CISelectLabel[x_,y_]:=Module[{},
        MLPut["mlput(ci_selectlabel(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[];
]
CISelectLabel[{x_,y_}]:=CISelectLabel[x,y]

CISelectSegment[x_,y_]:=Module[{},
        MLPut["mlput(ci_selectsegment(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[];
]
CISelectSegment[{x_,y_}]:=CISelectSegment[x,y]

CISelectArcSegment[x_,y_]:=Module[{},
        MLPut["mlput(ci_selectarcsegment(" <> NumC[x] <> Num[y] <> "))"];
        MLGet[];
]
CISelectArcSegment[{x_,y_}]:=CISelectArcSegment[x,y]

CIClearSelected[]:=MLPut["ci_clearselected()"]

CISetNodeProp[nodeprop_,groupno_,inconductor_]:=
        MLPut["ci_setnodeprop(" <> QuoteC[nodeprop] <> NumC[groupno] <> Quote[inconductor] <> ")"]

CISetBlockProp[blockname_,automesh_,meshsize_,group_]:=
        MLPut["ci_setblockprop(" <> QuoteC[blockname] <> NumC[automesh] <>
                NumC[meshsize] <> Num[group] <> ")"]

CISetSegmentProp[pn_,es_,am_,ci_,gr_, inconductor_]:=
        MLPut["ci_setsegmentprop(" <> QuoteC[pn] <> NumC[es] <>
                NumC[am] <> NumC[hi] <> NumC[gr] <> Quote[inconductor] <> ")"];

CISetArcSegmentProp[maxsegdeg_,propname_,hide_,group_,incond_]:=
        MLPut["ci_setarcsegmentprop(" <> NumC[maxsegdeg] <> QuoteC[propname]
        <> NumC[hide] <> NumC[group] <> Quote[incond] <> ")" ]

CIDeleteSelected[]:=MLPut["ci_deleteselected()"]

CIDeleteSelectedNodes[]:=MLPut["ci_deleteselectednodes()"]

CIDeleteSelectedLabels[]:=MLPut["ci_deleteselectedlabels()"]

CIDeleteSelectedSegments[]:=MLPut["ci_deleteselectedsegments()"]

CIDeleteSelectedArcSegments[]:=MLPut["ci_deleteselectedarcsegments()"]

CIAddMaterial[name_,ox_,oy_,ex_,ey_,ltx_,lty_]:=
        MLPut["ci_addmaterial(" <> QuoteC[name] <> NumC[ox] <> NumC[oy] <> NumC[ex] <> NumC[ey] <> NumC[ltx] <> Num[lty] <> ")" ]

CIAddMaterial[name_,ox_,oy_,ex_,ey_,ltx_]:=
        MLPut["ci_addmaterial(" <> QuoteC[name] <> NumC[ox] <> NumC[oy] <> NumC[ex] <> NumC[ey] <> Num[ltx] <> ")" ]

CIAddMaterial[name_,ox_,oy_,ex_,ey_]:=
        MLPut["ci_addmaterial(" <> QuoteC[name] <> NumC[ox] <> NumC[oy] <> NumC[ex] <> Num[ey] <> ")" ]

CIAddMaterial[name_,ox_,oy_,ex_]:=
        MLPut["ci_addmaterial(" <> QuoteC[name] <> NumC[ox] <> NumC[oy] <> Num[ex] <> ")" ]

CIAddMaterial[name_,ox_,oy_]:=
        MLPut["ci_addmaterial(" <> QuoteC[name] <> NumC[ox] <> Num[oy] <> ")" ]

CIAddMaterial[name_,ox_]:=
        MLPut["ci_addmaterial(" <> QuoteC[name] <> Num[ox] <> ")" ]

CIAddMaterial[name_]:=
        MLPut["ci_addmaterial(" <> Quote[name] <> ")" ]

CIClose[]:=MLPut["ci_close()"]

CIAddPointProp[name_,vp_,qp_]:=
        MLPut["ci_addpointprop(" <> QuoteC[name] <> NumC[vp] <> Num[qp] <> ")"]

CIAddPointProp[name_]:=
        MLPut["ci_addpointprop(" <> Quote[name] <> ")"]

CIAddPointProp[name_,vp_]:=
        MLPut["ci_addpointprop(" <> QuoteC[name] <> Num[vp] <> ")"]

CIAddConductorProp[name_,vc_,qc_,type_]:=
        MLPut["ci_addconductorprop(" <> QuoteC[name] <> NumC[vc] <> NumC[qc]
                <> Num[type] <> ")" ];

CIAddConductorProp[name_]:=
        MLPut["ci_addconductorprop(" <> Quote[name] <> ")" ];

CIAddConductorProp[name_,vc_]:=
        MLPut["ci_addconductorprop(" <> QuoteC[name] <> Num[vc] <> ")" ];

CIAddConductorProp[name_,vc_,qc_]:=
        MLPut["ci_addconductorprop(" <> QuoteC[name] <> NumC[vc] <> Num[qc]
                <> ")" ];

CIAddBoundProp[name_,fmt_,Tset_,qs_,Tinf_,h_,bta_]:=
        MLPut["ci_addboundprop(" <> QuoteC[name] <> NumC[fmt] <> NumC[Tset] <>
                NumC[qs] <> NumC[Tinf] <> NumC[h] <> Num[bta] <> ")" ]

CIAddBoundProp[name_]:=
        MLPut["ci_addboundprop(" <> Quote[name] <> ")" ]

CIAddBoundProp[name_,Vs_]:=
        MLPut["ci_addboundprop(" <> QuoteC[name] <> Num[Vs] <> ")" ]

CIAddBoundProp[name_,Vs_,qs_]:=
        MLPut["ci_addboundprop(" <> QuoteC[name] <> NumC[Vs] <> Num[qs] <> ")" ]

CIAddBoundProp[name_,Vs_,qs_,c0_]:=
        MLPut["ci_addboundprop(" <> QuoteC[name] <> NumC[Vs] <> NumC[qs] <> Num[c0] <> ")" ]

CIAddBoundProp[name_,Vs_,qs_,c0_,c1_]:=
        MLPut["ci_addboundprop(" <> QuoteC[name] <> NumC[Vs] <> NumC[qs] <> NumC[c0] <> Num[c1] <> ")" ]

CIAddBoundProp[name_,Vs_,qs_,c0_,c1_,fmt_]:=
        MLPut["ci_addboundprop(" <> QuoteC[name] <> NumC[Vs] <> NumC[qs] <> NumC[c0] <> NumC[c1] <> Num[fmt] <> ")" ]

CIModifyMaterial[name_,num_,val_]:=
        If[num==0,
                MLPut["ci_modifymaterial(" <> QuoteC[name] <> NumC[num] <> Quote[val] <> ")"],
                MLPut["ci_modifymaterial(" <> QuoteC[name] <> NumC[num] <> Num[val] <> ")"]
        ]

CIModifyBoundProp[name_,num_,val_]:=
        If[num==0,
                MLPut["ci_modifyboundprop(" <> QuoteC[name] <> NumC[num] <> Quote[val] <> ")"],
                MLPut["ci_modifyboundprop(" <> QuoteC[name] <> NumC[num] <> Num[val] <> ")"]
        ]

CIModifyPointProp[name_,num_,val_]:=
        If[num==0,
                MLPut["ci_modifypointprop(" <> QuoteC[name] <> NumC[num] <> Quote[val] <> ")"],
                MLPut["ci_modifypointprop(" <> QuoteC[name] <> NumC[num] <> Num[val] <> ")"]
        ]

CIModifyConductorProp[name_,num_,val_]:=
        If[num==0,
                MLPut["ci_modifyconductorprop(" <> QuoteC[name] <> NumC[num] <> Quote[val] <> ")"],
                MLPut["ci_modifyconductorprop(" <> QuoteC[name] <> NumC[num] <> Num[val] <> ")"]
        ]

CIDeleteMaterial[n_]:=MLPut["ci_deletematerial(" <> Quote[n] <> ")" ]

CIDeleteBoundProp[n_]:=MLPut["ci_deleteboundprop(" <> Quote[n] <> ")" ]

CIDeleteConductor[n_]:=MLPut["ci_deleteconductor(" <> Quote[n] <> ")" ]

CIDeletePointProp[n_]:=MLPut["ci_deletepointprop(" <> Quote[n] <> ")" ]

CIMoveRotate[bx_,by_,shiftangle_,editaction_]:=
        MLPut["ci_moverotate(" <> NumC[bx] <> NumC[by] <> NumC[shiftangle] <>
                Num[editaction] <> ")" ]
CIMoveRotate[{bx_,by_},shiftangle_,editaction_]:=CIMoveRotate[bx,by,shiftangle,editaction]

CIMoveRotate[bx_,by_,shiftangle_]:=
        MLPut["ci_moverotate(" <> NumC[bx] <> NumC[by] <> Num[shiftangle] <> ")" ]
CIMoveRotate[{bx_,by_},shiftangle_]:=CIMoveRotate[bx,by,shiftangle]

CIMoveTranslate[bx_,by_,editaction_]:=
        MLPut["ci_movetranslate(" <> NumC[bx] <> NumC[by] <> Num[editaction] <> ")" ]
CIMoveTranslate[{bx_,by_},editaction_]:=CIMoveTranslate[bx,by,editaction]

CIMoveTranslate[bx_,by_]:=
        MLPut["ci_movetranslate(" <> NumC[bx] <> Num[by] <> ")" ]
CIMoveTranslate[{bx_,by_}]:=CIMoveTranslate[bx,by]

CICopyRotate[bx_,by_,angle_,copies_,editaction_]:=
        MLPut["ci_copyrotate(" <> NumC[bx] <> NumC[by] <> NumC[angle] <>
                NumC[copies] <> Num[editaction] <> ")" ]
CICopyRotate[{bx_,by_},angle_,copies_,editaction_]:=CICopyRotate[bx,by,angle,copies,editaction]

CICopyRotate[bx_,by_,angle_,copies_]:=
        MLPut["ci_copyrotate(" <> NumC[bx] <> NumC[by] <>
                NumC[angle] <> Num[copies] <> ")" ]
CICopyRotate[{bx_,by_},angle_,copies_]:=CICopyRotate[bx,by,angle,copies]

CICopyTranslate[bx_,by_,copies_,editaction_]:=
        MLPut["ci_copytranslate(" <> NumC[bx] <> NumC[by] <>
                NumC[copies] <> Num[editaction] <> ")" ]
CICopyTranslate[{bx_,by_},copies_,editaction_]:=CICopyTranslate[bx,by,copies,editaction]

CICopyTranslate[bx_,by_,copies_]:=
        MLPut["ci_copytranslate(" <> NumC[bx] <> NumC[by] <> Num[copies] <> ")" ]
CICopyTranslate[{bx_,by_},copies_]:=CICopyTranslate[bx,by,copies]

CIMirror[x1_,y1_,x2_,y2_,editaction_]:=
        MLPut["ci_mirror(" <> NumC[x1] <> NumC[y1] <> NumC[x2] <>
                NumC[y2] <> Num[editaction] <> ")" ]
CIMirror[{x1_,y1_},{x2_,y2_},editaction_]:=CIMirror[x1,y1,x2,y2,editaction]
CIMirror[{{x1_,y1_},{x2_,y2_}},editaction_]:=CIMirror[x1,y1,x2,y2,editaction]

CIMirror[x1_,y1_,x2_,y2_]:=
        MLPut["ci_mirror(" <> NumC[x1] <> NumC[y1] <> NumC[x2] <> Num[y2] <> ")" ]
CIMirror[{x1_,y1_},{x2_,y2_}]:=CIMirror[x1,y1,x2,y2]
CIMirror[{{x1_,y1_},{x2_,y2_}}]:=CIMirror[x1,y1,x2,y2]

CIScale[bx_,by_,sc_,ea_]:=
        MLPut["ci_scale(" <> NumC[bx] <> NumC[by] <> NumC[sc] <> Num[ea] <> ")" ]
CIScale[{bx_,by_},sc_,ea_]:=CIScale[bx,by,sc,ea]

CIScale[bx_,by_,sc_]:=
        MLPut["ci_scale(" <> NumC[bx] <> NumC[by] <> Num[sc] <> ")" ]
CIScale[{bx_,by_},sc_]:=CIScale[bx,by,sc]

CISetEditMode[editmode_]:=MLPut["ci_seteditmode(" <> Quote[editmode] <> ")" ]

CISelectGroup[gr_]:=MLPut["ci_selectgroup(" <> Num[gr] <> ")" ]

CISaveBitmap[n_]:=MLPut["ci_savebitmap(" <> Quote[n] <> ")" ]

CISaveMetafile[n_]:=MLPut["ci_savemetafile(" <> Quote[n] <> ")" ]

CIRefreshView[]:=MLPut["ci_refreshview()"]

CIShowNames[]:=MLPut["ci_shownames(1)"];

CIHideNames[]:=MLPut["ci_shownames(0)"];

CIShowGrid[]:=MLPut["ci_showgrid()"]

CIHideGrid[]:=MLPut["ci_hidegrid()"]

CISnapGridOn[]:=MLPut["ci_gridsnap(\"on\")"]

CISnapGridOff[]:=MLPut["ci_gridsnap(\"off\")"]

CISetGrid[density_,type_]:=
        MLPut["ci_setgrid(" <> NumC[density] <> Quote[type] <> ")" ]

CISetFocus[docname_]:=MLPut["ci_setfocus(" <> Quote[docname] <> ")" ]

CIReadDXF[docname_]:=MLPut["ci_readdxf(" <> Quote[docname] <> ")" ]

CISaveDXF[docname_]:=MLPut["ci_savedxf(" <> Quote[docname] <> ")" ]

CIAddTKPoint[name_,b_,h_]:=MLPut["ci_addtkpoint(" <> QuoteC[name] <>
        NumC[b] <> Num[h] <> ")" ]

(****** Derived drawing functions ******)
CIAddNode[p_]:=CIAddNode[p[[1]],p[[2]]]
CIAddBlockLabel[p_]:=CIAddBlockLabel[p[[1]],p[[2]]]
CIAddSegment[p_,q_]:=CIAddSegment[p[[1]],p[[2]],q[[1]],q[[2]]]
CIAddSegment[p_]:=CIAddSegment[p[[1]],p[[2]]]
CIAddArc[p_,q_,angle_,maxseg_]:=CIAddArc[p[[1]],p[[2]],q[[1]],q[[2]],angle,maxseg]
CIAddArc[p_,angle_,maxseg_]:=CIAddArc[p[[1]],p[[2]],angle,maxseg]
CIDrawLine[x1_,y1_,x2_,y2_]:=Module[{},
        CIAddNode[x1,y1];
        CIAddNode[x2,y2];
        CIAddSegment[x1,y1,x2,y2]
]
CIDrawLine[p_,q_]:=CIDrawLine[p[[1]],p[[2]],q[[1]],q[[2]]]
CIDrawLine[p_]:=CIDrawLine[p[[1]],p[[2]]]
CIDrawPolyLine[p_]:=Module[{k},
        For[k=1,k<Length[p],k++,
                CIDrawLine[p[[k]],p[[k+1]]]
        ]
]
CIDrawPolygon[p_]:=Module[{},
        CIDrawPolyLine[p];
        CIDrawLine[p[[1]],p[[Length[p]]]]
]
CIDrawArc[x1_,y1_,x2_,y2_,angle_,maxseg_]:=Module[{},
        CIAddNode[x1,y1];
        CIAddNode[x2,y2];
        CIAddArc[x1,y1,x2,y2,angle,maxseg]
]
CIDrawArc[p_,q_,angle_,maxseg_]:=
        CIDrawArc[p[[1]],p[[2]],q[[1]],q[[2]],angle,maxseg]
CIDrawArc[p_,angle_,maxseg_]:=
        CIDrawArc[p[[1]],p[[2]],angle,maxseg]
CIDrawRectangle[x1_,y1_,x2_,y2_]:=CIDrawPolygon[{{x1,y1},{x1,y2},{x2,y2},{x2,y1}}]
CIDrawRectangle[p_,q_]:=CIDrawRectangle[p[[1]],p[[2]],q[[1]],q[[2]]]
CIDrawRectangle[p_]:=CIDrawRectangle[p[[1]],p[[2]]]
CICreateRadius[x_,y_,r_]:=MLPut["ci_createradius(" <> NumC[x] <> NumC[y] <> Num[r] <> ")" ]
CICreateRadius[{x_,y_},r_]:=MLPut["ci_createradius(" <> NumC[x] <> NumC[y] <> Num[r] <> ")" ]

(***************************************************)

COGetPointValues[x_,y_]:=Module[{z},
        MLPut["mlput(co_getpointvalues(" <> NumC[x] <> Num[y] <> "))"];
        MLGetNC[]
]
COGetPointValues[{x_,y_}]:=COGetPointValues[x,y]

COGetV[x_,y_]:=COGetPointValues[x,y][[1]]
COGetJ[x_,y_]:=COGetPointValues[x,y][[{2,3}]]
COGetK[x_,y_]:=COGetPointValues[x,y][[{4,5}]]
COGetE[x_,y_]:=COGetPointValues[x,y][[{6,7}]]

COGetV[{x_,y_}]:=COGetPointValues[x,y][[1]]
COGetJ[{x_,y_}]:=COGetPointValues[x,y][[{2,3}]]
COGetK[{x_,y_}]:=COGetPointValues[x,y][[{4,5}]]
COGetE[{x_,y_}]:=COGetPointValues[x,y][[{6,7}]]

COLineIntegral[type_]:=Module[{},
        MLPut["mlput(co_lineintegral(" <> Num[type] <> "))"];
        MLGetNC[]
]

COBlockIntegral[type_]:=Module[{},
        MLPut["mlput(co_blockintegral(" <> Num[type] <> "))"];
        MLGetNC[]
]

COMakePlot[plottype_,numpoints_,filename_,fileformat_]:=
        MLPut["co_makeplot(" <> NumC[plottype] <> NumC[numpoints] <>
                QuoteC[filename] <> Num[fileformat] <> ")" ]

COMakePlot[plottype_,numpoints_,filename_]:=
        MLPut["co_makeplot(" <> NumC[plottype] <> NumC[numpoints] <>
                Quote[filename] <> ")" ]

COMakePlot[plottype_,numpoints_]:=
        MLPut["co_makeplot(" <> NumC[plottype] <> Num[numpoints] <> ")" ]

COGetProblemInfo[]:=Module[{},
        MLPut["mlput(co_getprobleminfo())"];
        MLGetNC[]
]

COGetConductorProperties[name_]:=Module[{},
        MLPut["mlput(co_getconductorproperties(" <> Quote[name] <> "))" ];
        MLGetNC[]
]

COSetEditMode[mode_]:=MLPut["co_seteditmode(" <> Quote[mode] <> ")" ]

COSelectBlock[x_,y_]:=MLPut["co_selectblock(" <> NumC[x] <> Num[y] <> ")" ]
COSelectBlock[{x_,y_}]:=COSelectBlock[x,y]

COGroupSelectBlock[n_]:=MLPut["co_groupselectblock(" <> Num[n] <> ")" ]
COGroupSelectBlock[]:=MLPut["co_groupselectblock()" ]

COAddContour[x_,y_]:=MLPut["co_addcontour(" <> NumC[x] <> Num[y] <> ")" ]
COAddContour[{x_,y_}]:=COAddContour[x,y]

COBendContour[tta_,dtta_]:=MLPut["co_selectblock(" <> NumC[tta] <> Num[dtta] <> ")" ]

COSelectPoint[x_,y_]:=MLPut["co_selectpoint(" <> NumC[x] <> Num[y] <> ")" ]
COSelectPoint[{x_,y_}]:=COSelectPoint[x,y]

COClearContour[]:=MLPut["co_clearcontour()"]

COClearBlock[]:=MLPut["co_clearblock()"]

COShowMesh[]:=MLPut["co_showmesh()"]

COHideMesh[]:=MLPut["co_hidemesh()"]

COShowPoints[]:=MLPut["co_showpoints()"]

COHidePoints[]:=MLPut["co_hidepoints()"]

COSmooth[flag]:=MLPut["co_smooth(" <> Quote[flag] <> ")" ]

COSmoothOn[]:=MLPut["co_smooth(\"on\")"]

COSmoothOff[]:=MLPut["co_smooth(\"off\")"]

COShowGrid[]:=MLPut["co_showgrid()"]

COHideGrid[]:=MLPut["co_hidegrid()"]

COSnapGrid[flag]:=MLPut["co_gridsnap(" <> Quote[flag] <> ")" ]

COSnapGridOn[]:=MLPut["co_gridsnap(\"on\")"]

COSnapGridOff[]:=MLPut["co_gridsnap(\"off\")"]

COSetGrid[density_,type_]:=
        MLPut["co_setgrid(" <> NumC[density] <> Quote[type] <> ")" ]

COHideDensityPlot[]:=MLPut["co_hidedensityplot()"]

COShowDensityPlot[legend_,gscale_,type_,bu_,bl_]:=
        MLPut["co_showdensityplot(" <> NumC[legend] <> NumC[gscale] <>
                NumC[type] <> NumC[bu] <> Num[bl] <> ")" ]

COHideContourPlot[]:=MLPut["co_hidecontourplot()"]

COShowContourPlot[numcontours_,Al,Au]:=
        MLPut["co_showcontourplot(" <> NumC[numcontours] <> NumC[Al] <>
                Num[Au] <> ")" ]

COClose[]:=MLPut["co_close()"]

CORefreshView[]:=MLPut["co_refreshview()"]

COReload[]:=MLPutWait["co_reload()"];

COSaveBitmap[fn_]:=MLPut["co_savebitmap(" <> Quote[fn] <> ")" ]

COSaveMetafile[fn_]:=MLPut["co_savemetafile(" <> Quote[fn] <> ")" ]

COShowNames[flag]:=MLPut["co_shownames(" <> Num[flag] <> ")" ]

COShowNames[]:=MLPut["co_shownames(1)"]

COHideNames[]:=MLPut["co_shownames(0)"]


COShowVectorPlot[plottype_,scalefactor_]:=MLPut["co_showvectorplot(" <> NumC[plottype] <> Num[scalefactor] <> ")" ]
COShowVectorPlot[plottype_]:=MLPut["co_showvectorplot(" <> Num[plottype] <> ")" ]

MOShowVectorPlot[plottype_,scalefactor_]:=MLPut["mo_showvectorplot(" <> NumC[plottype] <> Num[scalefactor] <> ")" ]
MOShowVectorPlot[plottype_]:=MLPut["mo_showvectorplot(" <> Num[plottype] <> ")" ]

EOShowVectorPlot[plottype_,scalefactor_]:=MLPut["eo_showvectorplot(" <> NumC[plottype] <> Num[scalefactor] <> ")" ]
EOShowVectorPlot[plottype_]:=MLPut["eo_showvectorplot(" <> Num[plottype] <> ")" ]

HOShowVectorPlot[plottype_,scalefactor_]:=MLPut["ho_showvectorplot(" <> NumC[plottype] <> Num[scalefactor] <> ")" ]
HOShowVectorPlot[plottype_]:=MLPut["ho_showvectorplot(" <> Num[plottype] <> ")" ]


MIDefineOuterSpace[extZo_,extRo_,extRi_]:=MLPut["mi_defineouterspace(" <>
NumC[extZo] <> NumC[extRo] <> Num[extRi] <> ")" ]

MIAttachOuterSpace[]:=MLPut["mi_attachouterspace()"]

MIDetachOuterSpace[]:=MLPut["mi_detachouterspace()"]

MIAttachDefault[]:=MLPut["mi_attachdefault()"]

MIDetachDefault[]:=MLPut["mi_detachdefault()"]

EIAttachDefault[]:=MLPut["ei_attachdefault()"]

EIDetachDefault[]:=MLPut["ei_detachdefault()"]

HIAttachDefault[]:=MLPut["hi_attachdefault()"]

HIDetachDefault[]:=MLPut["hi_detachdefault()"]

CIAttachDefault[]:=MLPut["ci_attachdefault()"]

CIDetachDefault[]:=MLPut["ci_detachdefault()"]

EIDefineOuterSpace[extZo_,extRo_,extRi_]:=MLPut["ei_defineouterspace(" <>
NumC[extZo] <> NumC[extRo] <> Num[extRi] <> ")" ]

EIAttachOuterSpace[]:=MLPut["ei_attachouterspace()"]

EIDetachOuterSpace[]:=MLPut["ei_detachouterspace()"]

HIDefineOuterSpace[extZo_,extRo_,extRi_]:=MLPut["hi_defineouterspace(" <>
NumC[extZo] <> NumC[extRo] <> Num[extRi] <> ")" ]

HIAttachOuterSpace[]:=MLPut["hi_attachouterspace()"]

HIDetachOuterSpace[]:=MLPut["hi_detachouterspace()"]

CIDefineOuterSpace[extZo_,extRo_,extRi_]:=MLPut["ci_defineouterspace(" <>
NumC[extZo] <> NumC[extRo] <> Num[extRi] <> ")" ]

CIAttachOuterSpace[]:=MLPut["ci_attachouterspace()"]

CIDetachOuterSpace[]:=MLPut["ci_detachouterspace()"]


MONumNodes[]:=Module[{},
        MLPut["mlput(mo_numnodes())"];
        Round[MLGetNC[]]
]

MONumElements[]:=Module[{},
        MLPut["mlput(mo_numelements())"];
        Round[MLGetNC[]]
]

MOGetNode[n_]:=Module[{},
        MLPut["mlput(mo_getnode(" <> Num[n] <> "))"];
        MLGetNC[]
]

MOGetElement[n_]:=Module[{},
        MLPut["mlput(mo_getelement(" <> Num[n] <> "))"];
        Rationalize[MLGetNC[]]
]


CONumNodes[]:=Module[{},
        MLPut["mlput(co_numnodes())"];
        Round[MLGetNC[]]
]

CONumElements[]:=Module[{},
        MLPut["mlput(co_numelements())"];
        Round[MLGetNC[]]
]

COGetNode[n_]:=Module[{},
        MLPut["mlput(co_getnode(" <> Num[n] <> "))"];
        MLGetNC[]
]

COGetElement[n_]:=Module[{},
        MLPut["mlput(co_getelement(" <> Num[n] <> "))"];
        Rationalize[MLGetNC[]]
]


EONumNodes[]:=Module[{},
        MLPut["mlput(eo_numnodes())"];
        Round[MLGetNC[]]
]

EONumElements[]:=Module[{},
        MLPut["mlput(eo_numelements())"];
        Round[MLGetNC[]]
]

EOGetNode[n_]:=Module[{},
        MLPut["mlput(eo_getnode(" <> Num[n] <> "))"];
        MLGetNC[]
]

EOGetElement[n_]:=Module[{},
        MLPut["mlput(eo_getelement(" <> Num[n] <> "))"];
        Rationalize[MLGetNC[]]
]


HONumNodes[]:=Module[{},
        MLPut["mlput(ho_numnodes())"];
        Round[MLGetNC[]]
]

HONumElements[]:=Module[{},
        MLPut["mlput(ho_numelements())"];
        Round[MLGetNC[]]
]

HOGetNode[n_]:=Module[{},
        MLPut["mlput(ho_getnode(" <> Num[n] <> "))"];
        MLGetNC[]
]

HOGetElement[n_]:=Module[{},
        MLPut["mlput(ho_getelement(" <> Num[n] <> "))"];
        Rationalize[MLGetNC[]]
]


MISetGroup[n_]:=MLPut["mlput(mi_setgroup(" <> Num[n] <> "))"];

EISetGroup[n_]:=MLPut["mlput(ei_setgroup(" <> Num[n] <> "))"];

HISetGroup[n_]:=MLPut["mlput(hi_setgroup(" <> Num[n] <> "))"];

CISetGroup[n_]:=MLPut["mlput(ci_setgroup(" <> Num[n] <> "))"];

MIGetMaterial[fn_]:=MLPut["mi_getmaterial(" <> Quote[fn] <> ")"];

EIGetMaterial[fn_]:=MLPut["ei_getmaterial(" <> Quote[fn] <> ")"];

HIGetMaterial[fn_]:=MLPut["hi_getmaterial(" <> Quote[fn] <> ")"];

CIGetMaterial[fn_]:=MLPut["ci_getmaterial(" <> Quote[fn] <> ")"];

AWG[awg_]:=8.2514694*Exp[-0.115943*awg]

IEC[iec_]:=7.959159641581004*Exp[-0.11519673572274754*iec]

SmartMesh[n_]:=MLPut["smartmesh(" <> Num[n] <> ")"];
MISmartMesh[n_]:=MLPut["mi_smartmesh(" <> Num[n] <> ")"];
EISmartMesh[n_]:=MLPut["ei_smartmesh(" <> Num[n] <> ")"];
CISmartMesh[n_]:=MLPut["ci_smartmesh(" <> Num[n] <> ")"];
HISmartMesh[n_]:=MLPut["hi_smartmesh(" <> Num[n] <> ")"];

MISelectCircle[x_,y_,R_,ee_]:=MLPut["mi_selectcircle(" <> NumC[x] <> NumC[y] <> NumC[R] <> Num[ee] <> ")" ]; 
MISelectCircle[x_,y_,R_]:=MLPut["mi_selectcircle(" <> NumC[x] <> NumC[y] <> Num[R] <> ")" ]; 
MISelectRectangle[x1_,y1_,x2_,y2_,ee_]:=MLPut["mi_selectrectangle(" <> NumC[x1] <> NumC[y1] <> NumC[x2] <> NumC[y2] <> Num[ee] <> ")" ]; 
MISelectRectangle[x1_,y1_,x2_,y2_]:=MLPut["mi_selectrectangle(" <> NumC[x1] <> NumC[y1] <> NumC[x2] <> Num[y2] <> ")" ]; 

EISelectCircle[x_,y_,R_,ee_]:=MLPut["ei_selectcircle(" <> NumC[x] <> NumC[y] <> NumC[R] <> Num[ee] <> ")" ]; 
EISelectCircle[x_,y_,R_]:=MLPut["ei_selectcircle(" <> NumC[x] <> NumC[y] <> Num[R] <> ")" ]; 
EISelectRectangle[x1_,y1_,x2_,y2_,ee_]:=MLPut["ei_selectrectangle(" <> NumC[x1] <> NumC[y1] <> NumC[x2] <> NumC[y2] <> Num[ee] <> ")" ]; 
EISelectRectangle[x1_,y1_,x2_,y2_]:=MLPut["ei_selectrectangle(" <> NumC[x1] <> NumC[y1] <> NumC[x2] <> Num[y2] <> ")" ]; 

CISelectCircle[x_,y_,R_,ee_]:=MLPut["ci_selectcircle(" <> NumC[x] <> NumC[y] <> NumC[R] <> Num[ee] <> ")" ]; 
CISelectCircle[x_,y_,R_]:=MLPut["ci_selectcircle(" <> NumC[x] <> NumC[y] <> Num[R] <> ")" ]; 
CISelectRectangle[x1_,y1_,x2_,y2_,ee_]:=MLPut["ci_selectrectangle(" <> NumC[x1] <> NumC[y1] <> NumC[x2] <> NumC[y2] <> Num[ee] <> ")" ]; 
CISelectRectangle[x1_,y1_,x2_,y2_]:=MLPut["ci_selectrectangle(" <> NumC[x1] <> NumC[y1] <> NumC[x2] <> Num[y2] <> ")" ]; 

HISelectCircle[x_,y_,R_,ee_]:=MLPut["hi_selectcircle(" <> NumC[x] <> NumC[y] <> NumC[R] <> Num[ee] <> ")" ]; 
HISelectCircle[x_,y_,R_]:=MLPut["hi_selectcircle(" <> NumC[x] <> NumC[y] <> Num[R] <> ")" ]; 
HISelectRectangle[x1_,y1_,x2_,y2_,ee_]:=MLPut["hi_selectrectangle(" <> NumC[x1] <> NumC[y1] <> NumC[x2] <> NumC[y2] <> Num[ee] <> ")" ]; 
HISelectRectangle[x1_,y1_,x2_,y2_]:=MLPut["hi_selectrectangle(" <> NumC[x1] <> NumC[y1] <> NumC[x2] <> Num[y2] <> ")" ]; 

MIMakeABC[]:=MLPut["mi_makeABC()" ];
MIMakeABC[n_]:=MLPut["mi_makeABC(" <> Num[n] <> ")" ];
MIMakeABC[n_,R_]:=MLPut["mi_makeABC(" <> NumC[n] <> Num[R] <> ")" ];
MIMakeABC[n_,R_,x_]:=MLPut["mi_makeABC(" <> NumC[n] <> NumC[R] <> Num[x] <> ")" ];
MIMakeABC[n_,R_,x_,y_]:=MLPut["mi_makeABC(" <> NumC[n] <> NumC[R] <> NumC[x] <> Num[y] <> ")" ];
MIMakeABC[n_,R_,x_,y_,bc_]:=MLPut["mi_makeABC(" <> NumC[n] <> NumC[R] <> NumC[x] <> NumC[y] <> Num[bc] <> ")" ];

HIMakeABC[]:=MLPut["hi_makeABC()" ];
HIMakeABC[n_]:=MLPut["hi_makeABC(" <> Num[n] <> ")" ];
HIMakeABC[n_,R_]:=MLPut["hi_makeABC(" <> NumC[n] <> Num[R] <> ")" ];
HIMakeABC[n_,R_,x_]:=MLPut["hi_makeABC(" <> NumC[n] <> NumC[R] <> Num[x] <> ")" ];
HIMakeABC[n_,R_,x_,y_]:=MLPut["hi_makeABC(" <> NumC[n] <> NumC[R] <> NumC[x] <> Num[y] <> ")" ];
HIMakeABC[n_,R_,x_,y_,bc_]:=MLPut["hi_makeABC(" <> NumC[n] <> NumC[R] <> NumC[x] <> NumC[y] <> Num[bc] <> ")" ];

EIMakeABC[]:=MLPut["ei_makeABC()" ];
EIMakeABC[n_]:=MLPut["ei_makeABC(" <> Num[n] <> ")" ];
EIMakeABC[n_,R_]:=MLPut["ei_makeABC(" <> NumC[n] <> Num[R] <> ")" ];
EIMakeABC[n_,R_,x_]:=MLPut["ei_makeABC(" <> NumC[n] <> NumC[R] <> Num[x] <> ")" ];
EIMakeABC[n_,R_,x_,y_]:=MLPut["ei_makeABC(" <> NumC[n] <> NumC[R] <> NumC[x] <> Num[y] <> ")" ];
EIMakeABC[n_,R_,x_,y_,bc_]:=MLPut["ei_makeABC(" <> NumC[n] <> NumC[R] <> NumC[x] <> NumC[y] <> Num[bc] <> ")" ];

CIMakeABC[]:=MLPut["ci_makeABC()" ];
CIMakeABC[n_]:=MLPut["ci_makeABC(" <> Num[n] <> ")" ];
CIMakeABC[n_,R_]:=MLPut["ci_makeABC(" <> NumC[n] <> Num[R] <> ")" ];
CIMakeABC[n_,R_,x_]:=MLPut["ci_makeABC(" <> NumC[n] <> NumC[R] <> Num[x] <> ")" ];
CIMakeABC[n_,R_,x_,y_]:=MLPut["ci_makeABC(" <> NumC[n] <> NumC[R] <> NumC[x] <> Num[y] <> ")" ];
CIMakeABC[n_,R_,x_,y_,bc_]:=MLPut["ci_makeABC(" <> NumC[n] <> NumC[R] <> NumC[x] <> NumC[y] <> Num[bc] <> ")" ];

MOGetGapB[BdryName_,angle_]:= Module[{},
       MLPut["mlput(mo_getgapb(" <> QuoteC[BdryName] <> Num[angle] <> "))"];
       MLGet[]
]

MOGetGapA[BdryName_,angle_]:= Module[{},
       MLPut["mlput(mo_getgapa(" <> QuoteC[BdryName] <> Num[angle] <> "))"];
       MLGet[]
]

MOGapIntegral[BdryName_,inttype_]:=Module[{},
       MLPut["mlput(mo_gapintegral(" <> QuoteC[BdryName] <> Num[inttype] <> "))"];
       MLGet[]
]

MOGetGapHarmonics[BdryName_,n_]:=Module[{},
       MLPut["mlput(mo_getgapharmonics(" <> QuoteC[BdryName] <> Num[n] <> "))"];
       MLGet[]
]

MOGetGapHarmonics[BdryName_]:=Module[{},
       MLPut["mlput(mo_getgapharmonics(" <> Quote[BdryName] <> "))"];
       MLGet[]
]

End[];

SetAttributes[CloseFEMM,{Locked,Protected,ReadProtected}];
SetAttributes[EIAddArc,{Locked,Protected,ReadProtected}];
SetAttributes[EIAddBlockLabel,{Locked,Protected,ReadProtected}];
SetAttributes[EIAddBoundProp,{Locked,Protected,ReadProtected}];
SetAttributes[EIAddConductorProp,{Locked,Protected,ReadProtected}];
SetAttributes[EIAddMaterial,{Locked,Protected,ReadProtected}];
SetAttributes[EIAddNode,{Locked,Protected,ReadProtected}];
SetAttributes[EIAddPointProp,{Locked,Protected,ReadProtected}];
SetAttributes[EIAddSegment,{Locked,Protected,ReadProtected}];
SetAttributes[EIAnalyse,{Locked,Protected,ReadProtected}];
SetAttributes[EIAnalyze,{Locked,Protected,ReadProtected}];
SetAttributes[EIClearSelected,{Locked,Protected,ReadProtected}];
SetAttributes[EIClose,{Locked,Protected,ReadProtected}];
SetAttributes[EICopyRotate,{Locked,Protected,ReadProtected}];
SetAttributes[EICopyTranslate,{Locked,Protected,ReadProtected}];
SetAttributes[EICreateMesh,{Locked,Protected,ReadProtected}];
SetAttributes[EIDeleteBoundProp,{Locked,Protected,ReadProtected}];
SetAttributes[EIDeleteConductor,{Locked,Protected,ReadProtected}];
SetAttributes[EIDeleteMaterial,{Locked,Protected,ReadProtected}];
SetAttributes[EIDeletePointProp,{Locked,Protected,ReadProtected}];
SetAttributes[EIDeleteSelected,{Locked,Protected,ReadProtected}];
SetAttributes[EIDeleteSelectedArcSegments,{Locked,Protected,ReadProtected}];
SetAttributes[EIDeleteSelectedLabels,{Locked,Protected,ReadProtected}];
SetAttributes[EIDeleteSelectedNodes,{Locked,Protected,ReadProtected}];
SetAttributes[EIDeleteSelectedSegments,{Locked,Protected,ReadProtected}];
SetAttributes[EIDrawArc,{Locked,Protected,ReadProtected}];
SetAttributes[EIDrawLine,{Locked,Protected,ReadProtected}];
SetAttributes[EIDrawPolygon,{Locked,Protected,ReadProtected}];
SetAttributes[EIDrawPolyLine,{Locked,Protected,ReadProtected}];
SetAttributes[EIDrawRectangle,{Locked,Protected,ReadProtected}];
SetAttributes[EIGetView,{Locked,Protected,ReadProtected}];
SetAttributes[EIHideGrid,{Locked,Protected,ReadProtected}];
SetAttributes[EIHideNames,{Locked,Protected,ReadProtected}];
SetAttributes[EILoadSolution,{Locked,Protected,ReadProtected}];
SetAttributes[EIMirror,{Locked,Protected,ReadProtected}];
SetAttributes[EIModifyBoundProp,{Locked,Protected,ReadProtected}];
SetAttributes[EIModifyConductorProp,{Locked,Protected,ReadProtected}];
SetAttributes[EIModifyMaterial,{Locked,Protected,ReadProtected}];
SetAttributes[EIModifyPointProp,{Locked,Protected,ReadProtected}];
SetAttributes[EIMoveRotate,{Locked,Protected,ReadProtected}];
SetAttributes[EIMoveTranslate,{Locked,Protected,ReadProtected}];
SetAttributes[EIProbDef,{Locked,Protected,ReadProtected}];
SetAttributes[EIPurgeMesh,{Locked,Protected,ReadProtected}];
SetAttributes[EIReadDXF,{Locked,Protected,ReadProtected}];
SetAttributes[EISaveDXF,{Locked,Protected,ReadProtected}];
SetAttributes[HISaveDXF,{Locked,Protected,ReadProtected}];
SetAttributes[MISaveDXF,{Locked,Protected,ReadProtected}];
SetAttributes[CISaveDXF,{Locked,Protected,ReadProtected}];
SetAttributes[EIRefreshView,{Locked,Protected,ReadProtected}];
SetAttributes[EISaveAs,{Locked,Protected,ReadProtected}];
SetAttributes[EISaveBitmap,{Locked,Protected,ReadProtected}];
SetAttributes[EISaveMetafile,{Locked,Protected,ReadProtected}];
SetAttributes[EIScale,{Locked,Protected,ReadProtected}];
SetAttributes[EISelectArcSegment,{Locked,Protected,ReadProtected}];
SetAttributes[EISelectGroup,{Locked,Protected,ReadProtected}];
SetAttributes[EISelectLabel,{Locked,Protected,ReadProtected}];
SetAttributes[EISelectNode,{Locked,Protected,ReadProtected}];
SetAttributes[EISelectSegment,{Locked,Protected,ReadProtected}];
SetAttributes[EISetArcSegmentProp,{Locked,Protected,ReadProtected}];
SetAttributes[EISetBlockProp,{Locked,Protected,ReadProtected}];
SetAttributes[EISetEditMode,{Locked,Protected,ReadProtected}];
SetAttributes[EISetFocus,{Locked,Protected,ReadProtected}];
SetAttributes[EISetGrid,{Locked,Protected,ReadProtected}];
SetAttributes[EISetNodeProp,{Locked,Protected,ReadProtected}];
SetAttributes[EISetSegmentProp,{Locked,Protected,ReadProtected}];
SetAttributes[EIShowGrid,{Locked,Protected,ReadProtected}];
SetAttributes[EIShowMesh,{Locked,Protected,ReadProtected}];
SetAttributes[EIShowNames,{Locked,Protected,ReadProtected}];
SetAttributes[EISnapGridOff,{Locked,Protected,ReadProtected}];
SetAttributes[EISnapGridOn,{Locked,Protected,ReadProtected}];
SetAttributes[EIZoom,{Locked,Protected,ReadProtected}];
SetAttributes[EIZoomIn,{Locked,Protected,ReadProtected}];
SetAttributes[EIZoomNatural,{Locked,Protected,ReadProtected}];
SetAttributes[EIZoomOut,{Locked,Protected,ReadProtected}];
SetAttributes[EOAddContour,{Locked,Protected,ReadProtected}];
SetAttributes[EOBendContour,{Locked,Protected,ReadProtected}];
SetAttributes[EOBlockIntegral,{Locked,Protected,ReadProtected}];
SetAttributes[EOClearBlock,{Locked,Protected,ReadProtected}];
SetAttributes[EOClearContour,{Locked,Protected,ReadProtected}];
SetAttributes[EOClose,{Locked,Protected,ReadProtected}];
SetAttributes[EOGetConductorProperties,{Locked,Protected,ReadProtected}];
SetAttributes[EOGetD,{Locked,Protected,ReadProtected}];
SetAttributes[EOGetE,{Locked,Protected,ReadProtected}];
SetAttributes[EOGetEnergyDensity,{Locked,Protected,ReadProtected}];
SetAttributes[EOGetPerm,{Locked,Protected,ReadProtected}];
SetAttributes[EOGetPointValues,{Locked,Protected,ReadProtected}];
SetAttributes[EOGetProblemInfo,{Locked,Protected,ReadProtected}];
SetAttributes[EOGetV,{Locked,Protected,ReadProtected}];
SetAttributes[EOGetView,{Locked,Protected,ReadProtected}];
SetAttributes[EOGroupSelectBlock,{Locked,Protected,ReadProtected}];
SetAttributes[EOHideContourPlot,{Locked,Protected,ReadProtected}];
SetAttributes[EOHideDensityPlot,{Locked,Protected,ReadProtected}];
SetAttributes[EOHideGrid,{Locked,Protected,ReadProtected}];
SetAttributes[EOHideMesh,{Locked,Protected,ReadProtected}];
SetAttributes[EOHideNames,{Locked,Protected,ReadProtected}];
SetAttributes[EOHidePoints,{Locked,Protected,ReadProtected}];
SetAttributes[EOLineIntegral,{Locked,Protected,ReadProtected}];
SetAttributes[EOMakePlot,{Locked,Protected,ReadProtected}];
SetAttributes[EORefreshView,{Locked,Protected,ReadProtected}];
SetAttributes[EOReload,{Locked,Protected,ReadProtected}];
SetAttributes[EOSaveBitmap,{Locked,Protected,ReadProtected}];
SetAttributes[EOSaveMetafile,{Locked,Protected,ReadProtected}];
SetAttributes[EOSelectBlock,{Locked,Protected,ReadProtected}];
SetAttributes[EOSelectPoint,{Locked,Protected,ReadProtected}];
SetAttributes[EOSetEditMode,{Locked,Protected,ReadProtected}];
SetAttributes[EOSetGrid,{Locked,Protected,ReadProtected}];
SetAttributes[EOShowContourPlot,{Locked,Protected,ReadProtected}];
SetAttributes[EOShowDensityPlot,{Locked,Protected,ReadProtected}];
SetAttributes[EOShowGrid,{Locked,Protected,ReadProtected}];
SetAttributes[EOShowMesh,{Locked,Protected,ReadProtected}];
SetAttributes[EOShowNames,{Locked,Protected,ReadProtected}];
SetAttributes[EOShowPoints,{Locked,Protected,ReadProtected}];
SetAttributes[EOSmooth,{Locked,Protected,ReadProtected}];
SetAttributes[EOSmoothOff,{Locked,Protected,ReadProtected}];
SetAttributes[EOSmoothOn,{Locked,Protected,ReadProtected}];
SetAttributes[EOSnapGrid,{Locked,Protected,ReadProtected}];
SetAttributes[EOSnapGridOff,{Locked,Protected,ReadProtected}];
SetAttributes[EOSnapGridOn,{Locked,Protected,ReadProtected}];
SetAttributes[EOZoom,{Locked,Protected,ReadProtected}];
SetAttributes[EOZoomIn,{Locked,Protected,ReadProtected}];
SetAttributes[EOZoomNatural,{Locked,Protected,ReadProtected}];
SetAttributes[EOZoomOut,{Locked,Protected,ReadProtected}];
SetAttributes[HIAddArc,{Locked,Protected,ReadProtected}];
SetAttributes[HIAddBlockLabel,{Locked,Protected,ReadProtected}];
SetAttributes[HIAddBoundProp,{Locked,Protected,ReadProtected}];
SetAttributes[HIAddConductorProp,{Locked,Protected,ReadProtected}];
SetAttributes[HIAddMaterial,{Locked,Protected,ReadProtected}];
SetAttributes[HIAddNode,{Locked,Protected,ReadProtected}];
SetAttributes[HIAddPointProp,{Locked,Protected,ReadProtected}];
SetAttributes[HIAddSegment,{Locked,Protected,ReadProtected}];
SetAttributes[HIAnalyse,{Locked,Protected,ReadProtected}];
SetAttributes[HIAnalyze,{Locked,Protected,ReadProtected}];
SetAttributes[HIClearSelected,{Locked,Protected,ReadProtected}];
SetAttributes[HIClose,{Locked,Protected,ReadProtected}];
SetAttributes[HICopyRotate,{Locked,Protected,ReadProtected}];
SetAttributes[HICopyTranslate,{Locked,Protected,ReadProtected}];
SetAttributes[HICreateMesh,{Locked,Protected,ReadProtected}];
SetAttributes[HIDeleteBoundProp,{Locked,Protected,ReadProtected}];
SetAttributes[HIDeleteConductor,{Locked,Protected,ReadProtected}];
SetAttributes[HIDeleteMaterial,{Locked,Protected,ReadProtected}];
SetAttributes[HIDeletePointProp,{Locked,Protected,ReadProtected}];
SetAttributes[HIDeleteSelected,{Locked,Protected,ReadProtected}];
SetAttributes[HIDeleteSelectedArcSegments,{Locked,Protected,ReadProtected}];
SetAttributes[HIDeleteSelectedLabels,{Locked,Protected,ReadProtected}];
SetAttributes[HIDeleteSelectedNodes,{Locked,Protected,ReadProtected}];
SetAttributes[HIDeleteSelectedSegments,{Locked,Protected,ReadProtected}];
SetAttributes[HIDrawArc,{Locked,Protected,ReadProtected}];
SetAttributes[HIDrawLine,{Locked,Protected,ReadProtected}];
SetAttributes[HIDrawPolygon,{Locked,Protected,ReadProtected}];
SetAttributes[HIDrawPolyLine,{Locked,Protected,ReadProtected}];
SetAttributes[HIDrawRectangle,{Locked,Protected,ReadProtected}];
SetAttributes[HIGetView,{Locked,Protected,ReadProtected}];
SetAttributes[HIHideGrid,{Locked,Protected,ReadProtected}];
SetAttributes[HIHideNames,{Locked,Protected,ReadProtected}];
SetAttributes[HILoadSolution,{Locked,Protected,ReadProtected}];
SetAttributes[HIMirror,{Locked,Protected,ReadProtected}];
SetAttributes[HIModifyBoundProp,{Locked,Protected,ReadProtected}];
SetAttributes[HIModifyConductorProp,{Locked,Protected,ReadProtected}];
SetAttributes[HIModifyMaterial,{Locked,Protected,ReadProtected}];
SetAttributes[HIModifyPointProp,{Locked,Protected,ReadProtected}];
SetAttributes[HIMoveRotate,{Locked,Protected,ReadProtected}];
SetAttributes[HIMoveTranslate,{Locked,Protected,ReadProtected}];
SetAttributes[HIProbDef,{Locked,Protected,ReadProtected}];
SetAttributes[HIPurgeMesh,{Locked,Protected,ReadProtected}];
SetAttributes[HIReadDXF,{Locked,Protected,ReadProtected}];
SetAttributes[HIRefreshView,{Locked,Protected,ReadProtected}];
SetAttributes[HISaveAs,{Locked,Protected,ReadProtected}];
SetAttributes[HISaveBitmap,{Locked,Protected,ReadProtected}];
SetAttributes[HISaveMetafile,{Locked,Protected,ReadProtected}];
SetAttributes[HIScale,{Locked,Protected,ReadProtected}];
SetAttributes[HISelectArcSegment,{Locked,Protected,ReadProtected}];
SetAttributes[HISelectGroup,{Locked,Protected,ReadProtected}];
SetAttributes[HISelectLabel,{Locked,Protected,ReadProtected}];
SetAttributes[HISelectNode,{Locked,Protected,ReadProtected}];
SetAttributes[HISelectSegment,{Locked,Protected,ReadProtected}];
SetAttributes[HISetArcSegmentProp,{Locked,Protected,ReadProtected}];
SetAttributes[HISetBlockProp,{Locked,Protected,ReadProtected}];
SetAttributes[HISetEditMode,{Locked,Protected,ReadProtected}];
SetAttributes[HISetFocus,{Locked,Protected,ReadProtected}];
SetAttributes[HISetGrid,{Locked,Protected,ReadProtected}];
SetAttributes[HISetNodeProp,{Locked,Protected,ReadProtected}];
SetAttributes[HISetSegmentProp,{Locked,Protected,ReadProtected}];
SetAttributes[HIShowGrid,{Locked,Protected,ReadProtected}];
SetAttributes[HIShowMesh,{Locked,Protected,ReadProtected}];
SetAttributes[HIShowNames,{Locked,Protected,ReadProtected}];
SetAttributes[HISnapGridOff,{Locked,Protected,ReadProtected}];
SetAttributes[HISnapGridOn,{Locked,Protected,ReadProtected}];
SetAttributes[HIZoom,{Locked,Protected,ReadProtected}];
SetAttributes[HIZoomIn,{Locked,Protected,ReadProtected}];
SetAttributes[HIZoomNatural,{Locked,Protected,ReadProtected}];
SetAttributes[HIZoomOut,{Locked,Protected,ReadProtected}];
SetAttributes[HIAddTKPoint,{Locked,Protected,ReadProtected}];
SetAttributes[HIAddTKPoints,{Locked,Protected,ReadProtected}];
SetAttributes[HIClearTKPoints,{Locked,Protected,ReadProtected}];
SetAttributes[HOAddContour,{Locked,Protected,ReadProtected}];
SetAttributes[HOBendContour,{Locked,Protected,ReadProtected}];
SetAttributes[HOBlockIntegral,{Locked,Protected,ReadProtected}];
SetAttributes[HOClearBlock,{Locked,Protected,ReadProtected}];
SetAttributes[HOClearContour,{Locked,Protected,ReadProtected}];
SetAttributes[HOClose,{Locked,Protected,ReadProtected}];
SetAttributes[HOGetConductorProperties,{Locked,Protected,ReadProtected}];
SetAttributes[HOGetF,{Locked,Protected,ReadProtected}];
SetAttributes[HOGetG,{Locked,Protected,ReadProtected}];
SetAttributes[HOGetK,{Locked,Protected,ReadProtected}];
SetAttributes[HOGetPointValues,{Locked,Protected,ReadProtected}];
SetAttributes[HOGetProblemInfo,{Locked,Protected,ReadProtected}];
SetAttributes[HOGetT,{Locked,Protected,ReadProtected}];
SetAttributes[HOGetView,{Locked,Protected,ReadProtected}];
SetAttributes[HOGroupSelectBlock,{Locked,Protected,ReadProtected}];
SetAttributes[HOHideContourPlot,{Locked,Protected,ReadProtected}];
SetAttributes[HOHideDensityPlot,{Locked,Protected,ReadProtected}];
SetAttributes[HOHideGrid,{Locked,Protected,ReadProtected}];
SetAttributes[HOHideMesh,{Locked,Protected,ReadProtected}];
SetAttributes[HOHideNames,{Locked,Protected,ReadProtected}];
SetAttributes[HOHidePoints,{Locked,Protected,ReadProtected}];
SetAttributes[HOLineIntegral,{Locked,Protected,ReadProtected}];
SetAttributes[HOMakePlot,{Locked,Protected,ReadProtected}];
SetAttributes[HORefreshView,{Locked,Protected,ReadProtected}];
SetAttributes[HOReload,{Locked,Protected,ReadProtected}];
SetAttributes[HOSaveBitmap,{Locked,Protected,ReadProtected}];
SetAttributes[HOSaveMetafile,{Locked,Protected,ReadProtected}];
SetAttributes[HOSelectBlock,{Locked,Protected,ReadProtected}];
SetAttributes[HOSelectPoint,{Locked,Protected,ReadProtected}];
SetAttributes[HOSetEditMode,{Locked,Protected,ReadProtected}];
SetAttributes[HOSetGrid,{Locked,Protected,ReadProtected}];
SetAttributes[HOShowContourPlot,{Locked,Protected,ReadProtected}];
SetAttributes[HOShowDensityPlot,{Locked,Protected,ReadProtected}];
SetAttributes[HOShowGrid,{Locked,Protected,ReadProtected}];
SetAttributes[HOShowMesh,{Locked,Protected,ReadProtected}];
SetAttributes[HOShowNames,{Locked,Protected,ReadProtected}];
SetAttributes[HOShowPoints,{Locked,Protected,ReadProtected}];
SetAttributes[HOSmooth,{Locked,Protected,ReadProtected}];
SetAttributes[HOSmoothOff,{Locked,Protected,ReadProtected}];
SetAttributes[HOSmoothOn,{Locked,Protected,ReadProtected}];
SetAttributes[HOSnapGrid,{Locked,Protected,ReadProtected}];
SetAttributes[HOSnapGridOff,{Locked,Protected,ReadProtected}];
SetAttributes[HOSnapGridOn,{Locked,Protected,ReadProtected}];
SetAttributes[HOZoom,{Locked,Protected,ReadProtected}];
SetAttributes[HOZoomIn,{Locked,Protected,ReadProtected}];
SetAttributes[HOZoomNatural,{Locked,Protected,ReadProtected}];
SetAttributes[HOZoomOut,{Locked,Protected,ReadProtected}];
SetAttributes[HideConsole,{Locked,Protected,ReadProtected}];
SetAttributes[HidePointProps,{Locked,Protected,ReadProtected}];
SetAttributes[Messagebox,{Locked,Protected,ReadProtected}];
SetAttributes[MIAddArc,{Locked,Protected,ReadProtected}];
SetAttributes[MIAddBHPoint,{Locked,Protected,ReadProtected}];
SetAttributes[MIAddBHPoints,{Locked,Protected,ReadProtected}];
SetAttributes[MIAddBlockLabel,{Locked,Protected,ReadProtected}];
SetAttributes[MIAddBoundProp,{Locked,Protected,ReadProtected}];
SetAttributes[MIAddCircProp,{Locked,Protected,ReadProtected}];
SetAttributes[MIAddMaterial,{Locked,Protected,ReadProtected}];
SetAttributes[MIAddNode,{Locked,Protected,ReadProtected}];
SetAttributes[MIAddPointProp,{Locked,Protected,ReadProtected}];
SetAttributes[MIAddSegment,{Locked,Protected,ReadProtected}];
SetAttributes[MIAnalyse,{Locked,Protected,ReadProtected}];
SetAttributes[MIAnalyze,{Locked,Protected,ReadProtected}];
SetAttributes[MIClearBHPoints,{Locked,Protected,ReadProtected}];
SetAttributes[MIClearSelected,{Locked,Protected,ReadProtected}];
SetAttributes[MIClose,{Locked,Protected,ReadProtected}];
SetAttributes[MICopyRotate,{Locked,Protected,ReadProtected}];
SetAttributes[MICopyTranslate,{Locked,Protected,ReadProtected}];
SetAttributes[MICreateMesh,{Locked,Protected,ReadProtected}];
SetAttributes[MIDeleteBoundProp,{Locked,Protected,ReadProtected}];
SetAttributes[MIDeleteCircuit,{Locked,Protected,ReadProtected}];
SetAttributes[MIDeleteMaterial,{Locked,Protected,ReadProtected}];
SetAttributes[MIDeletePointProp,{Locked,Protected,ReadProtected}];
SetAttributes[MIDeleteSelected,{Locked,Protected,ReadProtected}];
SetAttributes[MIDeleteSelectedArcSegments,{Locked,Protected,ReadProtected}];
SetAttributes[MIDeleteSelectedLabels,{Locked,Protected,ReadProtected}];
SetAttributes[MIDeleteSelectedNodes,{Locked,Protected,ReadProtected}];
SetAttributes[MIDeleteSelectedSegments,{Locked,Protected,ReadProtected}];
SetAttributes[MIDrawArc,{Locked,Protected,ReadProtected}];
SetAttributes[MIDrawLine,{Locked,Protected,ReadProtected}];
SetAttributes[MIDrawPolygon,{Locked,Protected,ReadProtected}];
SetAttributes[MIDrawPolyLine,{Locked,Protected,ReadProtected}];
SetAttributes[MIDrawRectangle,{Locked,Protected,ReadProtected}];
SetAttributes[MIGetView,{Locked,Protected,ReadProtected}];
SetAttributes[MIHideGrid,{Locked,Protected,ReadProtected}];
SetAttributes[MIHideNames,{Locked,Protected,ReadProtected}];
SetAttributes[MILoadSolution,{Locked,Protected,ReadProtected}];
SetAttributes[MIMirror,{Locked,Protected,ReadProtected}];
SetAttributes[MIModifyBoundProp,{Locked,Protected,ReadProtected}];
SetAttributes[MIModifyCircProp,{Locked,Protected,ReadProtected}];
SetAttributes[MIModifyMaterial,{Locked,Protected,ReadProtected}];
SetAttributes[MIModifyPointProp,{Locked,Protected,ReadProtected}];
SetAttributes[MIMoveRotate,{Locked,Protected,ReadProtected}];
SetAttributes[MIMoveTranslate,{Locked,Protected,ReadProtected}];
SetAttributes[MIProbDef,{Locked,Protected,ReadProtected}];
SetAttributes[MIPurgeMesh,{Locked,Protected,ReadProtected}];
SetAttributes[MIReadDXF,{Locked,Protected,ReadProtected}];
SetAttributes[MIRefreshView,{Locked,Protected,ReadProtected}];
SetAttributes[MISaveAs,{Locked,Protected,ReadProtected}];
SetAttributes[MISaveBitmap,{Locked,Protected,ReadProtected}];
SetAttributes[MISaveMetafile,{Locked,Protected,ReadProtected}];
SetAttributes[MIScale,{Locked,Protected,ReadProtected}];
SetAttributes[MISelectArcSegment,{Locked,Protected,ReadProtected}];
SetAttributes[MISelectGroup,{Locked,Protected,ReadProtected}];
SetAttributes[MISelectLabel,{Locked,Protected,ReadProtected}];
SetAttributes[MISelectNode,{Locked,Protected,ReadProtected}];
SetAttributes[MISelectSegment,{Locked,Protected,ReadProtected}];
SetAttributes[MISetArcSegmentProp,{Locked,Protected,ReadProtected}];
SetAttributes[MISetBlockProp,{Locked,Protected,ReadProtected}];
SetAttributes[MISetCurrent,{Locked,Protected,ReadProtected}];
SetAttributes[MISetEditMode,{Locked,Protected,ReadProtected}];
SetAttributes[MISetFocus,{Locked,Protected,ReadProtected}];
SetAttributes[MISetGrid,{Locked,Protected,ReadProtected}];
SetAttributes[MISetNodeProp,{Locked,Protected,ReadProtected}];
SetAttributes[MISetSegmentProp,{Locked,Protected,ReadProtected}];
SetAttributes[MIShowGrid,{Locked,Protected,ReadProtected}];
SetAttributes[MIShowMesh,{Locked,Protected,ReadProtected}];
SetAttributes[MIShowNames,{Locked,Protected,ReadProtected}];
SetAttributes[MISnapGridOff,{Locked,Protected,ReadProtected}];
SetAttributes[MISnapGridOn,{Locked,Protected,ReadProtected}];
SetAttributes[MIZoom,{Locked,Protected,ReadProtected}];
SetAttributes[MIZoomIn,{Locked,Protected,ReadProtected}];
SetAttributes[MIZoomNatural,{Locked,Protected,ReadProtected}];
SetAttributes[MIZoomOut,{Locked,Protected,ReadProtected}];
SetAttributes[MLGet,{Locked,Protected,ReadProtected}];
SetAttributes[MLGetNC,{Locked,Protected,ReadProtected}];
SetAttributes[MLPut,{Locked,Protected,ReadProtected}];
SetAttributes[MLPutWait,{Locked,Protected,ReadProtected}];
SetAttributes[MOAddContour,{Locked,Protected,ReadProtected}];
SetAttributes[MOBendContour,{Locked,Protected,ReadProtected}];
SetAttributes[MOBlockIntegral,{Locked,Protected,ReadProtected}];
SetAttributes[MOClearBlock,{Locked,Protected,ReadProtected}];
SetAttributes[MOClearContour,{Locked,Protected,ReadProtected}];
SetAttributes[MOClose,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetA,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetB,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetCircuitProperties,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetConductivity,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetEnergyDensity,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetH,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetJ,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetMu,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetPe,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetPh,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetFill,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetPointValues,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetProblemInfo,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetView,{Locked,Protected,ReadProtected}];
SetAttributes[MOGroupSelectBlock,{Locked,Protected,ReadProtected}];
SetAttributes[MOHideContourPlot,{Locked,Protected,ReadProtected}];
SetAttributes[MOHideDensityPlot,{Locked,Protected,ReadProtected}];
SetAttributes[MOHideGrid,{Locked,Protected,ReadProtected}];
SetAttributes[MOHideMesh,{Locked,Protected,ReadProtected}];
SetAttributes[MOHideNames,{Locked,Protected,ReadProtected}];
SetAttributes[MOHidePoints,{Locked,Protected,ReadProtected}];
SetAttributes[MOLineIntegral,{Locked,Protected,ReadProtected}];
SetAttributes[MOMakePlot,{Locked,Protected,ReadProtected}];
SetAttributes[MORefreshView,{Locked,Protected,ReadProtected}];
SetAttributes[MOReload,{Locked,Protected,ReadProtected}];
SetAttributes[MOSaveBitmap,{Locked,Protected,ReadProtected}];
SetAttributes[MOSaveMetafile,{Locked,Protected,ReadProtected}];
SetAttributes[MOSelectBlock,{Locked,Protected,ReadProtected}];
SetAttributes[MOSelectPoint,{Locked,Protected,ReadProtected}];
SetAttributes[MOSetEditMode,{Locked,Protected,ReadProtected}];
SetAttributes[MOSetGrid,{Locked,Protected,ReadProtected}];
SetAttributes[MOShowContourPlot,{Locked,Protected,ReadProtected}];
SetAttributes[MOShowDensityPlot,{Locked,Protected,ReadProtected}];
SetAttributes[MOShowGrid,{Locked,Protected,ReadProtected}];
SetAttributes[MOShowMesh,{Locked,Protected,ReadProtected}];
SetAttributes[MOShowNames,{Locked,Protected,ReadProtected}];
SetAttributes[MOShowPoints,{Locked,Protected,ReadProtected}];
SetAttributes[MOSmooth,{Locked,Protected,ReadProtected}];
SetAttributes[MOSmoothOff,{Locked,Protected,ReadProtected}];
SetAttributes[MOSmoothOn,{Locked,Protected,ReadProtected}];
SetAttributes[MOSnapGrid,{Locked,Protected,ReadProtected}];
SetAttributes[MOSnapGridOff,{Locked,Protected,ReadProtected}];
SetAttributes[MOSnapGridOn,{Locked,Protected,ReadProtected}];
SetAttributes[MOZoom,{Locked,Protected,ReadProtected}];
SetAttributes[MOZoomIn,{Locked,Protected,ReadProtected}];
SetAttributes[MOZoomNatural,{Locked,Protected,ReadProtected}];
SetAttributes[MOZoomOut,{Locked,Protected,ReadProtected}];
SetAttributes[NewDocument,{Locked,Protected,ReadProtected}];
SetAttributes[Num,{Locked,Protected,ReadProtected}];
SetAttributes[NumC,{Locked,Protected,ReadProtected}];
SetAttributes[OpenDebug,{Locked,Protected,ReadProtected}];
SetAttributes[OpenDocument,{Locked,Protected,ReadProtected}];
SetAttributes[OpenFEMM,{Locked,Protected,ReadProtected}];
SetAttributes[Prompt,{Locked,Protected,ReadProtected}];
SetAttributes[Quote,{Locked,Protected,ReadProtected}];
SetAttributes[QuoteC,{Locked,Protected,ReadProtected}];
SetAttributes[ShowConsole,{Locked,Protected,ReadProtected}];
SetAttributes[ShowPointProps,{Locked,Protected,ReadProtected}];
SetAttributes[GetPathToFEMM,{Locked,Protected,ReadProtected}];
SetAttributes[SetPathToFEMM,{Locked,Protected,ReadProtected}];
SetAttributes[GetPackageDirectory,{Locked,Protected,ReadProtected}];
SetAttributes[MIDefineOuterSpace,{Locked,Protected,ReadProtected}];
SetAttributes[MIAttachOuterSpace,{Locked,Protected,ReadProtected}];
SetAttributes[MIDetachOuterSpace,{Locked,Protected,ReadProtected}];
SetAttributes[MIAttachDefault,{Locked,Protected,ReadProtected}];
SetAttributes[MIDetachDefault,{Locked,Protected,ReadProtected}];
SetAttributes[EIAttachDefault,{Locked,Protected,ReadProtected}];
SetAttributes[EIDetachDefault,{Locked,Protected,ReadProtected}];
SetAttributes[HIAttachDefault,{Locked,Protected,ReadProtected}];
SetAttributes[HIDetachDefault,{Locked,Protected,ReadProtected}];
SetAttributes[CIAttachDefault,{Locked,Protected,ReadProtected}];
SetAttributes[CIDetachDefault,{Locked,Protected,ReadProtected}];
SetAttributes[EIDefineOuterSpace,{Locked,Protected,ReadProtected}];
SetAttributes[EIAttachOuterSpace,{Locked,Protected,ReadProtected}];
SetAttributes[EIDetachOuterSpace,{Locked,Protected,ReadProtected}];
SetAttributes[HIDefineOuterSpace,{Locked,Protected,ReadProtected}];
SetAttributes[HIAttachOuterSpace,{Locked,Protected,ReadProtected}];
SetAttributes[HIDetachOuterSpace,{Locked,Protected,ReadProtected}];
SetAttributes[MICreateRadius,{Locked,Protected,ReadProtected}];
SetAttributes[EICreateRadius,{Locked,Protected,ReadProtected}];
SetAttributes[HICreateRadius,{Locked,Protected,ReadProtected}];
SetAttributes[CIDefineOuterSpace,{Locked,Protected,ReadProtected}];
SetAttributes[CIAttachOuterSpace,{Locked,Protected,ReadProtected}];
SetAttributes[CIDetachOuterSpace,{Locked,Protected,ReadProtected}];
SetAttributes[CIAddArc,{Locked,Protected,ReadProtected}];
SetAttributes[CIAddBlockLabel,{Locked,Protected,ReadProtected}];
SetAttributes[CIAddBoundProp,{Locked,Protected,ReadProtected}];
SetAttributes[CIAddConductorProp,{Locked,Protected,ReadProtected}];
SetAttributes[CIAddMaterial,{Locked,Protected,ReadProtected}];
SetAttributes[CIAddNode,{Locked,Protected,ReadProtected}];
SetAttributes[CIAddPointProp,{Locked,Protected,ReadProtected}];
SetAttributes[CIAddSegment,{Locked,Protected,ReadProtected}];
SetAttributes[CIAnalyse,{Locked,Protected,ReadProtected}];
SetAttributes[CIAnalyze,{Locked,Protected,ReadProtected}];
SetAttributes[CIClearSelected,{Locked,Protected,ReadProtected}];
SetAttributes[CIClose,{Locked,Protected,ReadProtected}];
SetAttributes[CICopyRotate,{Locked,Protected,ReadProtected}];
SetAttributes[CICopyTranslate,{Locked,Protected,ReadProtected}];
SetAttributes[CICreateMesh,{Locked,Protected,ReadProtected}];
SetAttributes[CIDeleteBoundProp,{Locked,Protected,ReadProtected}];
SetAttributes[CIDeleteConductor,{Locked,Protected,ReadProtected}];
SetAttributes[CIDeleteMaterial,{Locked,Protected,ReadProtected}];
SetAttributes[CIDeletePointProp,{Locked,Protected,ReadProtected}];
SetAttributes[CIDeleteSelected,{Locked,Protected,ReadProtected}];
SetAttributes[CIDeleteSelectedArcSegments,{Locked,Protected,ReadProtected}];
SetAttributes[CIDeleteSelectedLabels,{Locked,Protected,ReadProtected}];
SetAttributes[CIDeleteSelectedNodes,{Locked,Protected,ReadProtected}];
SetAttributes[CIDeleteSelectedSegments,{Locked,Protected,ReadProtected}];
SetAttributes[CIDrawArc,{Locked,Protected,ReadProtected}];
SetAttributes[CIDrawLine,{Locked,Protected,ReadProtected}];
SetAttributes[CIDrawPolygon,{Locked,Protected,ReadProtected}];
SetAttributes[CIDrawPolyLine,{Locked,Protected,ReadProtected}];
SetAttributes[CIDrawRectangle,{Locked,Protected,ReadProtected}];
SetAttributes[CIGetView,{Locked,Protected,ReadProtected}];
SetAttributes[CIHideGrid,{Locked,Protected,ReadProtected}];
SetAttributes[CIHideNames,{Locked,Protected,ReadProtected}];
SetAttributes[CILoadSolution,{Locked,Protected,ReadProtected}];
SetAttributes[CIMirror,{Locked,Protected,ReadProtected}];
SetAttributes[CIModifyBoundProp,{Locked,Protected,ReadProtected}];
SetAttributes[CIModifyConductorProp,{Locked,Protected,ReadProtected}];
SetAttributes[CIModifyMaterial,{Locked,Protected,ReadProtected}];
SetAttributes[CIModifyPointProp,{Locked,Protected,ReadProtected}];
SetAttributes[CIMoveRotate,{Locked,Protected,ReadProtected}];
SetAttributes[CIMoveTranslate,{Locked,Protected,ReadProtected}];
SetAttributes[CIProbDef,{Locked,Protected,ReadProtected}];
SetAttributes[CIPurgeMesh,{Locked,Protected,ReadProtected}];
SetAttributes[CIReadDXF,{Locked,Protected,ReadProtected}];
SetAttributes[CIRefreshView,{Locked,Protected,ReadProtected}];
SetAttributes[CISaveAs,{Locked,Protected,ReadProtected}];
SetAttributes[CISaveBitmap,{Locked,Protected,ReadProtected}];
SetAttributes[CISaveMetafile,{Locked,Protected,ReadProtected}];
SetAttributes[CIScale,{Locked,Protected,ReadProtected}];
SetAttributes[CISelectArcSegment,{Locked,Protected,ReadProtected}];
SetAttributes[CISelectGroup,{Locked,Protected,ReadProtected}];
SetAttributes[CISelectLabel,{Locked,Protected,ReadProtected}];
SetAttributes[CISelectNode,{Locked,Protected,ReadProtected}];
SetAttributes[CISelectSegment,{Locked,Protected,ReadProtected}];
SetAttributes[CISetArcSegmentProp,{Locked,Protected,ReadProtected}];
SetAttributes[CISetBlockProp,{Locked,Protected,ReadProtected}];
SetAttributes[CISetEditMode,{Locked,Protected,ReadProtected}];
SetAttributes[CISetFocus,{Locked,Protected,ReadProtected}];
SetAttributes[CISetGrid,{Locked,Protected,ReadProtected}];
SetAttributes[CISetNodeProp,{Locked,Protected,ReadProtected}];
SetAttributes[CISetSegmentProp,{Locked,Protected,ReadProtected}];
SetAttributes[CIShowGrid,{Locked,Protected,ReadProtected}];
SetAttributes[CIShowMesh,{Locked,Protected,ReadProtected}];
SetAttributes[CIShowNames,{Locked,Protected,ReadProtected}];
SetAttributes[CISnapGridOff,{Locked,Protected,ReadProtected}];
SetAttributes[CISnapGridOn,{Locked,Protected,ReadProtected}];
SetAttributes[CIZoom,{Locked,Protected,ReadProtected}];
SetAttributes[CIZoomIn,{Locked,Protected,ReadProtected}];
SetAttributes[CIZoomNatural,{Locked,Protected,ReadProtected}];
SetAttributes[CIZoomOut,{Locked,Protected,ReadProtected}];
SetAttributes[COAddContour,{Locked,Protected,ReadProtected}];
SetAttributes[COBendContour,{Locked,Protected,ReadProtected}];
SetAttributes[COBlockIntegral,{Locked,Protected,ReadProtected}];
SetAttributes[COClearBlock,{Locked,Protected,ReadProtected}];
SetAttributes[COClearContour,{Locked,Protected,ReadProtected}];
SetAttributes[COClose,{Locked,Protected,ReadProtected}];
SetAttributes[COGetConductorProperties,{Locked,Protected,ReadProtected}];
SetAttributes[COGetJ,{Locked,Protected,ReadProtected}];
SetAttributes[COGetK,{Locked,Protected,ReadProtected}];
SetAttributes[COGetE,{Locked,Protected,ReadProtected}];
SetAttributes[COGetPointValues,{Locked,Protected,ReadProtected}];
SetAttributes[COGetProblemInfo,{Locked,Protected,ReadProtected}];
SetAttributes[COGetV,{Locked,Protected,ReadProtected}];
SetAttributes[COGetView,{Locked,Protected,ReadProtected}];
SetAttributes[COGroupSelectBlock,{Locked,Protected,ReadProtected}];
SetAttributes[COHideContourPlot,{Locked,Protected,ReadProtected}];
SetAttributes[COHideDensityPlot,{Locked,Protected,ReadProtected}];
SetAttributes[COHideGrid,{Locked,Protected,ReadProtected}];
SetAttributes[COHideMesh,{Locked,Protected,ReadProtected}];
SetAttributes[COHideNames,{Locked,Protected,ReadProtected}];
SetAttributes[COHidePoints,{Locked,Protected,ReadProtected}];
SetAttributes[COLineIntegral,{Locked,Protected,ReadProtected}];
SetAttributes[COMakePlot,{Locked,Protected,ReadProtected}];
SetAttributes[CORefreshView,{Locked,Protected,ReadProtected}];
SetAttributes[COReload,{Locked,Protected,ReadProtected}];
SetAttributes[COSaveBitmap,{Locked,Protected,ReadProtected}];
SetAttributes[COSaveMetafile,{Locked,Protected,ReadProtected}];
SetAttributes[COSelectBlock,{Locked,Protected,ReadProtected}];
SetAttributes[COSelectPoint,{Locked,Protected,ReadProtected}];
SetAttributes[COSetEditMode,{Locked,Protected,ReadProtected}];
SetAttributes[COSetGrid,{Locked,Protected,ReadProtected}];
SetAttributes[COShowContourPlot,{Locked,Protected,ReadProtected}];
SetAttributes[COShowDensityPlot,{Locked,Protected,ReadProtected}];
SetAttributes[COShowGrid,{Locked,Protected,ReadProtected}];
SetAttributes[COShowMesh,{Locked,Protected,ReadProtected}];
SetAttributes[COShowNames,{Locked,Protected,ReadProtected}];
SetAttributes[COShowPoints,{Locked,Protected,ReadProtected}];
SetAttributes[COSmooth,{Locked,Protected,ReadProtected}];
SetAttributes[COSmoothOff,{Locked,Protected,ReadProtected}];
SetAttributes[COSmoothOn,{Locked,Protected,ReadProtected}];
SetAttributes[COSnapGrid,{Locked,Protected,ReadProtected}];
SetAttributes[COSnapGridOff,{Locked,Protected,ReadProtected}];
SetAttributes[COSnapGridOn,{Locked,Protected,ReadProtected}];
SetAttributes[COZoom,{Locked,Protected,ReadProtected}];
SetAttributes[COZoomIn,{Locked,Protected,ReadProtected}];
SetAttributes[COZoomNatural,{Locked,Protected,ReadProtected}];
SetAttributes[COZoomOut,{Locked,Protected,ReadProtected}];
SetAttributes[COShowVectorPlot,{Locked,Protected,ReadProtected}];
SetAttributes[MOShowVectorPlot,{Locked,Protected,ReadProtected}];
SetAttributes[HOShowVectorPlot,{Locked,Protected,ReadProtected}];
SetAttributes[EOShowVectorPlot,{Locked,Protected,ReadProtected}];
SetAttributes[MONumNodes,{Locked,Protected,ReadProtected}];
SetAttributes[MONumElements,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetNode,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetElement,{Locked,Protected,ReadProtected}];
SetAttributes[HONumNodes,{Locked,Protected,ReadProtected}];
SetAttributes[HONumElements,{Locked,Protected,ReadProtected}];
SetAttributes[HOGetNode,{Locked,Protected,ReadProtected}];
SetAttributes[HOGetElement,{Locked,Protected,ReadProtected}];
SetAttributes[EONumNodes,{Locked,Protected,ReadProtected}];
SetAttributes[EONumElements,{Locked,Protected,ReadProtected}];
SetAttributes[EOGetNode,{Locked,Protected,ReadProtected}];
SetAttributes[EOGetElement,{Locked,Protected,ReadProtected}];
SetAttributes[CONumNodes,{Locked,Protected,ReadProtected}];
SetAttributes[CONumElements,{Locked,Protected,ReadProtected}];
SetAttributes[COGetNode,{Locked,Protected,ReadProtected}];
SetAttributes[COGetElement,{Locked,Protected,ReadProtected}];
SetAttributes[MISetGroup,{Locked,Protected,ReadProtected}];
SetAttributes[EISetGroup,{Locked,Protected,ReadProtected}];
SetAttributes[HISetGroup,{Locked,Protected,ReadProtected}];
SetAttributes[CISetGroup,{Locked,Protected,ReadProtected}];
SetAttributes[MIGetMaterial,{Locked,Protected,ReadProtected}];
SetAttributes[EIGetMaterial,{Locked,Protected,ReadProtected}];
SetAttributes[HIGetMaterial,{Locked,Protected,ReadProtected}];
SetAttributes[CIGetMaterial,{Locked,Protected,ReadProtected}];
SetAttributes[MISelectCircle,{Locked,Protected,ReadProtected}];
SetAttributes[MISelectRectangle,{Locked,Protected,ReadProtected}];
SetAttributes[EISelectCircle,{Locked,Protected,ReadProtected}];
SetAttributes[EISelectRectangle,{Locked,Protected,ReadProtected}];
SetAttributes[CISelectCircle,{Locked,Protected,ReadProtected}];
SetAttributes[CISelectRectangle,{Locked,Protected,ReadProtected}];
SetAttributes[HISelectCircle,{Locked,Protected,ReadProtected}];
SetAttributes[HISelectRectangle,{Locked,Protected,ReadProtected}];
SetAttributes[MIMakeABC,{Locked,Protected,ReadProtected}];
SetAttributes[EIMakeABC,{Locked,Protected,ReadProtected}];
SetAttributes[CIMakeABC,{Locked,Protected,ReadProtected}];
SetAttributes[HIMakeABC,{Locked,Protected,ReadProtected}];
SetAttributes[SmartMesh,{Locked,Protected,ReadProtected}];
SetAttributes[MISmartMesh,{Locked,Protected,ReadProtected}];
SetAttributes[EISmartMesh,{Locked,Protected,ReadProtected}];
SetAttributes[HISmartMesh,{Locked,Protected,ReadProtected}];
SetAttributes[CISmartMesh,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetGapB,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetGapA,{Locked,Protected,ReadProtected}];
SetAttributes[MOGetGapHarmonics,{Locked,Protected,ReadProtected}];
SetAttributes[MOGapIntegral,{Locked,Protected,ReadProtected}];
EndPackage[]

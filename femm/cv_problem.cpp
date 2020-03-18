#include "stdafx.h"
#include "cv_problem.h"

/////////////////////////////////////////////////////////////////////////////
// CNode construction

CNode::CNode()
{
	x=0.;
	y=0.;
	IsSelected=FALSE;
	BoundaryMarker=-1;
	InConductor=-1;
	InGroup=0;
}

CComplex CNode::CC()
{
	return (x+I*y);
}

double CNode::GetDistance(double xo, double yo)
{
	return sqrt((x-xo)*(x-xo) + (y-yo)*(y-yo));
}

void CNode::ToggleSelect()
{
	if (IsSelected==TRUE) IsSelected=FALSE;
	else IsSelected=TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMeshNode construction

CMeshNode::CMeshNode()
{
	x=y=0;
	v=0;
	Q=FALSE;
	msk=0;
	IsSelected=FALSE;
}

CComplex CMeshNode::CC()
{
	return (x+I*y);
}

double CMeshNode::GetDistance(double xo, double yo)
{
	return sqrt((x-xo)*(x-xo) + (y-yo)*(y-yo));
}

/////////////////////////////////////////////////////////////////////////////
// CSegment construction

CSegment::CSegment()
{
	n0=0;
	n1=0;
	IsSelected=FALSE;
	BoundaryMarker=-1;
	InConductor=-1;
	InGroup=0;
}

void CSegment::ToggleSelect()
{
	if (IsSelected==TRUE) IsSelected=FALSE;
	else IsSelected=TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CArcSegment construction

CArcSegment::CArcSegment()
{
	n0=0;
	n1=0;
	IsSelected=FALSE;
	MaxSideLength=-1;
	ArcLength=90.;
	BoundaryMarker=-1;
	InConductor=-1;
	InGroup=0;
}

void CArcSegment::ToggleSelect()
{
	if (IsSelected==TRUE) IsSelected=FALSE;
	else IsSelected=TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CNode construction

CBlockLabel::CBlockLabel()
{
	x=0.;
	y=0.;
	MaxArea=0.;
	IsSelected=FALSE;
	InGroup=0;
	BlockType=-1;
	IsExternal=FALSE;
	IsDefault=FALSE;
}

void CBlockLabel::ToggleSelect()
{
	if (IsSelected==TRUE) IsSelected=FALSE;
	else IsSelected=TRUE;
}

double CBlockLabel::GetDistance(double xo, double yo)
{
	return sqrt((x-xo)*(x-xo) + (y-yo)*(y-yo));
}

CMaterialProp::CMaterialProp()
{		
		BlockName="New Material";
		Kx=0; Ky=0;
		ox=0; oy=0;
		ex=0; ey=0;
}

CBoundaryProp::CBoundaryProp()
{
		BdryName="New Boundary";
		BdryFormat=0;				// type of boundary condition we are applying
		V=0; qs=0;
		c0=0;
		c1=0;

		InConductor="<None>";
}

CPointProp::CPointProp()
{
		PointName="New Point Property";
		InConductor="<None>";
		V=0;
		qp=0;
}

CCircuit::CCircuit()
{
		CircName="New Circuit";
		V=0;
		q=0;
		CircType=0;
}

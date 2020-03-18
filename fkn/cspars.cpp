#include <stdafx.h>
#include <math.h>
#include <stdio.h>
#include "fkn.h"
#include "fknDlg.h"
#include "complex.h"
#include "spars.h"

#define MAXITER 1000000
#define KLUDGE
#define nrm(X) sqrt(Re(ConjDot(X,X)))

CComplexEntry::CComplexEntry()
{
	next=NULL;
	x=0;
	c=0;
}

CBigComplexLinProb::CBigComplexLinProb()
{
	n=0;
}

CBigComplexLinProb::~CBigComplexLinProb()
{
	if (n==0) return;

	int i;
	CComplexEntry *uo,*ui;

	free(b); free(P); free(R);
	free(V); free(U); free(Z);
	free(uu); free(vv);

	for(i=0;i<n;i++)
	{
		ui=M[i];
		do{
			uo=ui;
			ui=uo->next;
			delete uo;
		} while(ui!=NULL);
	}
	free(M);

	if (bNewton)
	{
		for(i=0;i<n;i++)
		{
			ui=Mh[i];
			do{
				uo=ui;
				ui=uo->next;
				delete uo;
			} while(ui!=NULL);
		}
		free(Mh);

		for(i=0;i<n;i++)
		{
			ui=Ma[i];
			do{
				uo=ui;
				ui=uo->next;
				delete uo;
			} while(ui!=NULL);
		}
		free(Ma);

		for(i=0;i<n;i++)
		{
			ui=Ms[i];
			do{
				uo=ui;
				ui=uo->next;
				delete uo;
			} while(ui!=NULL);
		}
		free(Ms);
	}
}

int CBigComplexLinProb::Create(int d, int bw, int nodes)
{
	int i;

	bdw=bw;
	NumNodes=nodes;
	b=(CComplex *)calloc(d,sizeof(CComplex));
	V=(CComplex *)calloc(d,sizeof(CComplex));
	P=(CComplex *)calloc(d,sizeof(CComplex));
	R=(CComplex *)calloc(d,sizeof(CComplex));
	U=(CComplex *)calloc(d,sizeof(CComplex));
	Z=(CComplex *)calloc(d,sizeof(CComplex));
	uu=(CComplex *)calloc(d,sizeof(CComplex));
	vv=(CComplex *)calloc(d,sizeof(CComplex));
	n=d;
	
	M=(CComplexEntry **)calloc(d,sizeof(CComplexEntry *));
	for(i=0;i<d;i++){
		M[i] = new CComplexEntry;
		M[i]->c = i;
	}

	bNewton=FALSE;

	return 1;
}

void CBigComplexLinProb::Put(CComplex v, int p, int q, int k)
{
	CComplexEntry *e,*l;
	int i;

	if(q<p){ 
		i=p; p=q; q=i; 
		if (k==1) v=conj(v);	// hermitian matrix
		if (k==3) v=-conj(v);	// antihermitian matrix
	}

	// allocate space for auxilliary matrices if they are actually needed
	if ((k>0) && (bNewton==FALSE))
	{
		bNewton=TRUE;

		Mh=(CComplexEntry **)calloc(n,sizeof(CComplexEntry *));
		for(i=0;i<n;i++){
			Mh[i] = new CComplexEntry;
			Mh[i]->c = i;
		}
		
		Ma=(CComplexEntry **)calloc(n,sizeof(CComplexEntry *));
		for(i=0;i<n;i++){
			Ma[i] = new CComplexEntry;
			Ma[i]->c = i;
		}

		Ms=(CComplexEntry **)calloc(n,sizeof(CComplexEntry *));
		for(i=0;i<n;i++){
			Ms[i] = new CComplexEntry;
			Ms[i]->c = i;
		}
	}

	switch(k)
	{
		case 1:
			e=Mh[p];
			break;
		case 2:
			e=Ms[p];
			break;
		case 3:
			e=Ma[p];
			break;
		default:
			e=M[p];
			break;
	}

	while((e->c < q) && (e->next != NULL))
	{
		l=e; 
		e=e->next;
	}

	if(e->c == q){
		e->x=v;
		return;
	}

	CComplexEntry *m = new CComplexEntry;
	
	if((e->next == NULL) && (q > e->c)){
		e->next = m;
		m->c = q;
		m->x = v;
	}
	else{
		l->next=m;
		m->next=e;
		m->c=q;
		m->x=v;
	}

	return;
}

CComplex CBigComplexLinProb::Get(int p, int q, int k)
{
	CComplexEntry *e;
	BOOL flip=FALSE;

	if(q<p){ int i; i=p; p=q; q=i; flip=TRUE; }

	switch(k)
	{
		case 1:
			if (bNewton==FALSE) return CComplex(0,0);
			e=Mh[p];
			break;
		case 2:
			if (bNewton==FALSE) return CComplex(0,0);
			e=Ms[p];
			break;
		case 3:
			if (bNewton==FALSE) return CComplex(0,0);
			e=Ma[p];
			break;
		default:
			e=M[p];
			break;
	}

	while((e->c < q) && (e->next != NULL)) e=e->next;

	if(e->c == q) 
	{
		if(flip)
		{
			if(k==1) return conj(e->x);		// case where matrix is hermitian...
			if(k==3) return -conj(e->x);	// case where matrix is anti-hermitian...
		}
		
		return e->x;
	}

	// if no entry in the list, this entry must be zero...
	return CComplex(0,0);
}

void CBigComplexLinProb::AddTo(CComplex v, int p, int q)
{
	Put(Get(p,q)+v,p,q);
}

void CBigComplexLinProb::MultA(CComplex *X, CComplex *Y, int k)
{
	int i;
	CComplexEntry *e;

	for(i=0;i<n;i++) Y[i]=0;

	// force the program to give the plain matrix multiply
	// if auxilliary matrices have not been built
	if ((!bNewton) && (k!=0)) k=0; 

	// Make the default call return the full multiply, including
	// the auxilliary matrix multiplies, when these matrices exist
	if ((bNewton) && (k==-1))
	{
		MultA(X,Y,0);
		MultA(X,uu,1);
		for(i=0;i<n;i++) Y[i]=Y[i]+uu[i];
		MultConjA(X,uu,2);
		MultA(X,vv,3);
		for(i=0;i<n;i++) Y[i]=Y[i]+conj(uu[i])+vv[i];
		return;
	}
	if ((bNewton) && (k==-2))
	{
		MultA(X,Y,0);
		MultConjA(X,uu,1);
		for(i=0;i<n;i++) Y[i]=Y[i]+uu[i];
		MultA(X,uu,2);
		MultConjA(X,vv,3);
		for(i=0;i<n;i++) Y[i]=Y[i]+uu[i];
		for(i=0;i<n;i++) Y[i]=Y[i]+conj(uu[i])-vv[i];
		return;
	}


	for(i=0;i<n;i++){
		switch (k)
		{
			case 1:
				Y[i]+=(Mh[i]->x*X[i]);
				e=Mh[i]->next;
				break;
			case 2:
				Y[i]+=(Ms[i]->x*X[i]);
				e=Ms[i]->next;
				break;
			case 3:
				Y[i]+=(Ma[i]->x*X[i]);
				e=Ma[i]->next;
				break;
			default:
				Y[i]+=(M[i]->x*X[i]);
				e=M[i]->next;
				break;
		}

		while(e!=NULL)
		{
			Y[i]+=(e->x*X[e->c]);
			if (k==1)   
				Y[e->c]+=(conj(e->x)*X[i]); // case in which the matrix is hermitian
			else if (k==3) 
				Y[e->c]+=(-conj(e->x)*X[i]); // case in which the matrix is antihermitian
			else        
				Y[e->c]+=(e->x*X[i]);             // case in which the matrix is complex-symmetric  
			e=e->next;
		}
	}
}

void CBigComplexLinProb::MultConjA(CComplex *X, CComplex *Y, int k)
{
	int i;
	CComplexEntry *e;

	for(i=0;i<n;i++) Y[i]=0;

	if ((k!=0) && (!bNewton)) k=0;

	for(i=0;i<n;i++){
		switch (k)
		{
			case 1:
				Y[i]+=(Mh[i]->x.Conj()*X[i]);
				e=Mh[i]->next;
				break;
			case 2:
				Y[i]+=(Ms[i]->x.Conj()*X[i]);
				e=Ms[i]->next;
				break;
			case 3:
				Y[i]+=(Ma[i]->x.Conj()*X[i]);
				e=Ma[i]->next;
				break;
			default:
				Y[i]+=(M[i]->x.Conj()*X[i]);
				e=M[i]->next;
				break;
		}
		while(e!=NULL)
		{
			Y[i]+=(e->x.Conj()*X[e->c]);
			if (k==1) 
				Y[e->c]+=(e->x*X[i]);   // case in which the matrix is hermitian
			if (k==3)
				Y[e->c]+=(-e->x*X[i]);   // case in which the matrix is antihermitian
			else 
				Y[e->c]+=(e->x.Conj()*X[i]); // case in which the matrix is complex-symmetric
			e=e->next;
		}
	}
}

void CBigComplexLinProb::MultAPPA(CComplex *X, CComplex *Y)
{
	int i;
	MultA(X,Z);
	MultPC(Z,Y);
	for(i=0;i<n;i++) Y[i].im=-Y[i].im;
	MultPC(Y,Z);
	MultA(Z,Y);
	for(i=0;i<n;i++) Y[i].im=-Y[i].im;
}

CComplex CBigComplexLinProb::Dot(CComplex *x, CComplex *y)
{
	int i;
	CComplex z;
	
	z=0;
	for(i=0;i<n;i++) z+=x[i]*y[i];
	
	return z;
}

CComplex CBigComplexLinProb::ConjDot(CComplex *x, CComplex *y)
{
	int i;
	CComplex z;
	
	z=0;
	for(i=0;i<n;i++) z+=x[i].Conj()*y[i];
	
	return z;
}

void CBigComplexLinProb::MultPC(CComplex *X, CComplex *Y)
{
	int i;

	// Jacobi preconditioner:
//	for(i=0;i<n;i++) Y[i]=X[i]/M[i]->x; return;


	// SSOR preconditioner
	CComplex c;
	CComplexEntry *e;

	c= LAMBDA*(2.-LAMBDA);
	for(i=0;i<n;i++) Y[i]=X[i]*c;
	
	// invert Lower Triangle;
	for(i=0;i<n;i++){
		Y[i]/= M[i]->x;
		e=M[i]->next;
		while(e!=NULL)
		{
			Y[e->c] -= e->x * Y[i] * LAMBDA;
			e=e->next;
		}	
	}

	for(i=0;i<n;i++) Y[i]*=M[i]->x;

	// invert Upper Triangle
	for(i=n-1;i>=0;i--){
		e=M[i]->next;
		while(e!=NULL)
		{
			Y[i] -= e->x * Y[e->c] * LAMBDA;
			e=e->next;
		}
		Y[i]/= M[i]->x;
	}

}

void CBigComplexLinProb::SetValue(int i, CComplex x)
{
    int k,fst,lst;
    CComplex z;
    	
	if(bdw==0){
		fst=0;
		lst=n;
	}
	else{
		fst=i-bdw; if (fst<0) fst=0;
		lst=i+bdw; if (lst>NumNodes) lst=NumNodes;
	}

	for(k=fst;k<n;k++)
    {
		if (k==lst) k=NumNodes;
		
		z=Get(k,i);
		if(z!=0){
			b[k]-=(z*x);
			if(i!=k) Put(CComplex(0,0),k,i);
		}

		if (bNewton)
		{
			z=Get(k,i,1);
			if(z!=0){
				if (i!=k) b[k]=b[k]-(z*x);
				Put(CComplex(0,0),k,i,1);
			}

			z=Get(k,i,2);
			if(z!=0){
				if (i!=k) b[k]=b[k]-(z*conj(x));
				Put(CComplex(0,0),k,i,2);
			}

			z=Get(k,i,3);
			if(z!=0){
//				if (i!=k) b[k]=b[k]-(-z*conj(x));
				if (i!=k) b[k]=b[k]-(z*conj(x));
				Put(CComplex(0,0),k,i,3);
			}
		}
    }
    b[i]=Get(i,i)*x;
}

void CBigComplexLinProb::Wipe()
{
	int i;
	CComplexEntry *e;

	for(i=0;i<n;i++){
		b[i]=0;
		e=M[i];
		do{
			e->x=0;
			e=e->next;
		} while(e!=NULL);
	}

	if (!bNewton) return;

	for(i=0;i<n;i++){
		e=Mh[i];
		do{
			e->x=0;
			e=e->next;
		} while(e!=NULL);
	}
	
	for(i=0;i<n;i++){
		e=Ma[i];
		do{
			e->x=0;
			e=e->next;
		} while(e!=NULL);
	}

	for(i=0;i<n;i++){
		e=Ms[i];
		do{
			e->x=0;
			e=e->next;
		} while(e!=NULL);
	}
}

void CBigComplexLinProb::AntiPeriodicity(int i, int j)
{
	int k,fst,lst,h;
	CComplex v1,v2,c;

#ifdef KLUDGE
	int tmpbdw=bdw;
	bdw=0;
#endif

	if (j<i) {k=j;j=i;i=k;}

	if(bdw==0){
		fst=0;
		lst=n;
	}
	else{
		fst=i-bdw; if (fst<0) fst=0;
		lst=j+bdw; if (lst>NumNodes-1) lst=NumNodes-1;
	}

	// contribution to A0 matrix
	for(k=fst;k<n;k++)
	{
		if((k!=i) && (k!=j))
		{
			v1=Get(k,i);
			v2=Get(k,j);
			if ((v1!=0) || (v2!=0)){
				c=(v1-v2)/2.;
				Put(c,k,i);
				Put(-c,k,j);
			}
		}
		if((k==i+bdw) && (k<j-bdw) && (bdw!=0)) k=j-bdw;
		else if(k==lst) k=NumNodes;
	}
	c=0.5*(Get(i,i)+Get(j,j));
	Put(c,i,i);
	Put(c,j,j);

	// contribution to RHS
	c=0.5*(b[i]-b[j]);
	b[i]=c;
	b[j]=-c;

    if(bNewton) for(h=1;h<=3;h++)
	{
		for(k=fst;k<n;k++)
		{
			if((k!=i) && (k!=j))
			{
				v1=Get(k,i,h);
				v2=Get(k,j,h);
				if ((v1!=0) || (v2!=0)){
					c=(v1-v2)/2.;
					Put(c,k,i,h);
					Put(-c,k,j,h);
				}
			}
			if((k==i+bdw) && (k<j-bdw) && (bdw!=0)) k=j-bdw;
			else if(k==lst) k=NumNodes;
		}
		c=(Get(i,i,h)-Get(i,j,h)-Get(j,i,h)+Get(j,j,h))/4.;
		Put(c,i,i,h);
		Put(-c,i,j,h);
		Put(c,j,j,h);
	}

#ifdef KLUDGE
	bdw=tmpbdw;
#endif
}

void CBigComplexLinProb::Periodicity(int i, int j)
{
	int k,fst,lst,h;
	CComplex v1,v2,c;

#ifdef KLUDGE
	int tmpbdw=bdw;
	bdw=0;
#endif

	if (j<i) {k=j;j=i;i=k;}
	
	if(bdw==0){
		fst=0;
		lst=n;
	}
	else{
		fst=i-bdw; if (fst<0) fst=0;
		lst=j+bdw; if (lst>NumNodes-1) lst=NumNodes-1;
	}

	for(k=fst;k<n;k++){
		if((k!=i) && (k!=j))
		{
			v1=Get(k,i);
			v2=Get(k,j);
			if ((v1!=0) || (v2!=0)) {
				c=(v1+v2)/2.;
				Put(c,k,i);
				Put(c,k,j);
			}	
		}
		if((k==i+bdw) && (k<j-bdw) && (bdw!=0)) k=j-bdw;
		else if(k==lst) k=NumNodes;
	}

	c=(Get(i,i)+Get(j,j))/2.;
	Put(c,i,i);
	Put(c,j,j);

	c=0.5*(b[i]+b[j]);
	b[i]=c;
	b[j]=c;

    if(bNewton) for(h=1;h<=3;h++)
	{
		for(k=fst;k<n;k++)
		{
			if((k!=i) && (k!=j))
			{
				v1=Get(k,i,h);
				v2=Get(k,j,h);
				if ((v1!=0) || (v2!=0)){
					c=(v1+v2)/2.;
					Put(c,k,i,h);
					Put(c,k,j,h);
				}
			}
			if((k==i+bdw) && (k<j-bdw) && (bdw!=0)) k=j-bdw;
			else if(k==lst) k=NumNodes;
		}
		c=(Get(i,i,h)+Get(i,j,h)+Get(j,i,h)+Get(j,j,h))/4.;
		Put(c,i,i,h);
		Put(c,i,j,h);
		Put(c,j,j,h);
	}
}

// Make into a Hermitian problem and solve.
// Just use for a few iterations to get a good starting point
// for the regular BiPCG, which can sometimes get initialized
// with a pathological starting point.
int CBigComplexLinProb::PCGSQStart()
{
	int i,k;
	CComplex res,res_new,del,rho,pAp;
	
	// quick check for most obvious sign of singularity;
	for(i=0;i<n;i++) if((M[i]->x.re==0) && (M[i]->x.im==0)){
		fprintf(stderr,"singular flag tripped.");
		return 0;
	}

	// Operate on RHS to scale for squared problem
	MultPC(b,Z);
	for(i=0;i<n;i++) Z[i].im=-Z[i].im;
	MultPC(Z,P);
	MultA(P,Z);
	for(i=0;i<n;i++) P[i]=Z[i].Conj();

	// initialize V with zeros;
	for(i=0;i<n;i++) V[i]=0;

	// form residual;
	MultAPPA(V,R);
	for(i=0;i<n;i++) R[i]=P[i]-R[i];
	
	// form initial search direction
	for(i=0;i<n;i++) P[i]=R[i];
	res=ConjDot(R,R);

	// do iteration;
	for(k=0;k<3;k++)
	{
		// step i)
		MultAPPA(P,U);
		pAp=ConjDot(P,U);
		del=res/pAp;

		// step ii)
		for(i=0;i<n;i++) V[i]+=(del*P[i]);
					
		// step iii)
		for(i=0;i<n;i++) R[i]-=(del*U[i]);
			
		// step iv)
		res_new=ConjDot(R,R);
		rho=res_new/res;
		res=res_new;

		// step v)
		for(i=0;i<n;i++) P[i]=R[i]+(rho*P[i]);

	} 

	return 1;
}		

// Complex-Symmetric Preconditioned BiCG
int CBigComplexLinProb::PBCGSolve(int flag)
{
	int i;
	CComplex res,res_new,del,rho,pAp;
	double er,normb;
	int prg2,prg1=0;
	
	// Initialize if required
	if(flag==FALSE){
		for(i=0;i<n;i++) V[i]=0;
	}

	// form residual;
	MultA(V,R);
	for(i=0;i<n;i++) R[i]=b[i]-R[i];
	normb=nrm(b);

	// initialize progress bar;
	er=nrm(R)/normb;
	prg1=(int) (20.*log10(er)/(log10(Precision)));
	TheView->m_prog1.SetPos(5*prg1);
	TheView->SetDlgItemText(IDC_FRAME1,"BiConjugate Gradient Solver");
	TheView->InvalidateRect(NULL, FALSE);
	TheView->UpdateWindow();

	// form initial search direction;
	MultPC(R,Z);
	for(i=0;i<n;i++) P[i]=Z[i];
	res=Dot(Z,R);

	// do iteration;
	do{
		// step i)
		MultA(P,U);
		pAp=Dot(P,U);
		del=res/pAp;

		// step ii)
		for(i=0;i<n;i++) V[i]+=(del*P[i]);
					
		// step iii)
		for(i=0;i<n;i++) R[i]-=(del*U[i]);
			
		// step iv)
		MultPC(R,Z);
		res_new=Dot(Z,R);
		rho=res_new/res;
		res=res_new;

		// step v)
		for(i=0;i<n;i++) P[i]=Z[i]+(rho*P[i]);

		er=nrm(R)/normb;

		// report progress
		prg2=(int) (20.*log10(er)/(log10(Precision)));
		if(prg2>prg1){
			prg1=prg2;
			prg2=(prg1*5);
			if(prg2>100) prg2=100;
			TheView->m_prog1.SetPos(prg2);
			TheView->InvalidateRect(NULL, FALSE);
			TheView->UpdateWindow();
		}

	} while(er>Precision); 

	return 1;
}

// BiCGSTAB for solving N-R iterations
int CBigComplexLinProb::BiCGSTAB(int flag)
{
	double er,normb;
	CComplex om,alf,rho1,rho2,bta;
	CComplex *P2,*R2,*Z2,*t;
	int i,j,k;
	CString out;

	P2=(CComplex *)calloc(n,sizeof(CComplex));
	Z2=(CComplex *)calloc(n,sizeof(CComplex));
	R2=(CComplex *)calloc(n,sizeof(CComplex));
	t =(CComplex *)calloc(n,sizeof(CComplex));

	// initialize progress bar;
	TheView->m_prog1.SetPos(0);
	int prg1=0;
	int prg2;
	TheView->SetDlgItemText(IDC_FRAME1,"BiCGSTAB Solver");
   
	if (flag==FALSE) for(i=0;i<n;i++) V[i]=0;

	MultA(V,R,-1);
	for(j=0;j<n;j++){
		R[j]=b[j]-R[j];
		R2[j]=R[j];
	}

	normb=nrm(b);

	for(k=0;k<MAXITER;k++)
	{
		rho1 = Re(ConjDot(R2,R));
		if (k==0){
			for(j=0;j<n;j++) P[j]=R[j];
		}
		else{
			bta=(rho1/rho2)*alf/om;
			for(j=0;j<n;j++) P[j] = R[j] + bta*(P[j] - om*U[j]);
		}
		MultPC(P,P2);
		MultA(P2,U,-1);
		alf=rho1/Re(ConjDot(R2,U));
		for(j=0;j<n;j++) Z[j]=R[j]-alf*U[j]; 
		MultPC(Z,Z2);
		MultA(Z2,t,-1);
		om=Re(ConjDot(t,Z))/Re(ConjDot(t,t));
		for(j=0;j<n;j++){
			V[j]=V[j]+alf*P2[j]+om*Z2[j];
			R[j]=Z[j]-om*t[j];
		}
		rho2 = rho1;
		er=nrm(R)/normb;

		// display progress to the user
		if (k==50*(k/50)){
			out.Format("BiCGSTAB Solver (%i,%g)",k,er);
			TheView->SetDlgItemText(IDC_FRAME1,out);
		}
		prg2=(int) (20.*log10(er)/(log10(Precision)));
		if(prg2>prg1){
			prg1=prg2;
			prg2=(prg1*5);
			if(prg2>100) prg2=100;
			TheView->m_prog1.SetPos(prg2);
			TheView->InvalidateRect(NULL, FALSE);
			TheView->UpdateWindow();
		}

		if (er<Precision) break;
	}
	free(P2);
	free(R2);
	free(Z2);
	free(t);

	if (er<Precision) return 1;
	return 0;
}

// ad-hoc iterative approach to solving non-symmetric N-R problem
int CBigComplexLinProb::KludgeSolve(int flag)
{
	
	int i,k; 
	double er,normb,c;
	CString out;
	CComplex *borig, *v, *r;

	borig=(CComplex *)calloc(n,sizeof(CComplex));
	v    =(CComplex *)calloc(n,sizeof(CComplex));
	r    =(CComplex *)calloc(n,sizeof(CComplex));

	// if flag is false, initialize V with zeros;
	if (flag==0) for(i=0;i<n;i++) V[i]=0;
	
	// get norm of RHS
	normb=nrm(b);
	
	// save original b vector
	for (i=0;i<n;i++){
		borig[i]=b[i];
		v[i]=V[i];
	}

	// form starting residual
	MultA(V,r,-1);
	for(i=0;i<n;i++) r[i]=b[i]-r[i];
	er=nrm(r)/normb;
	if (er<Precision) return 1;

	for(k=0;k<10;k++)
	{
		// modify RHS multiplying results of the previous
		// iteration by the A1 and A2 matrices
		MultA(V,P,1);
		MultConjA(V,U,2);
		MultA(V,R,3);
		for(i=0;i<n;i++) b[i]=borig[i] - P[i] - conj(U[i]) - R[i];
	
		PBCGSolve(TRUE);

		// adjust step length along the new direction
		// to result in the greatest reduction in error
		for(i=0;i<n;i++) P[i]= V[i]-v[i];
		MultA(P,U,-1);
		c=Re(ConjDot(r,U))/Re(ConjDot(U,U));
		for(i=0;i<n;i++)
		{
			V[i] = v[i] + c*P[i];
			r[i] = r[i] - c*U[i];
			v[i] = V[i];
		}

		er=nrm(r)/normb;
		if (er<Precision*10.) break;
	}
	free(borig); free(v); free(r);
	return 1;
}
	
// Entry point into linear solvers.
// Calls PCGSQStart to do a small number of iterations,
// moving the starting point for PBCG away from the 
// pathological starting points that can sometimes crop up.
int CBigComplexLinProb::PBCGSolveMod(int flag)
{
	// if this is a N-R iteration, call the appropriate solver
	if (bNewton) 
	//	return BiCGSTAB(flag);
		return KludgeSolve(flag);
	
	// Get starting point with a few iterations of CGNE;
	if(flag==FALSE){
		TheView->SetDlgItemText(IDC_FRAME1,"Initializing Solver");
		if (PCGSQStart()==0) return 0;
	}
	

	// call the complex-symmetric solver
	return PBCGSolve(2);
}	

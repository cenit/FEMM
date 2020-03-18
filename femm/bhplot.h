class CBHPlot
{
	public:
		// data members
		double **M;
		char **lbls;
		int NumRows;
		int NumCols;
		int NumPts;
		CComplex *Pts;
		
		// member functions
		CBHPlot();
		~CBHPlot();
		BOOL Create(int rows, int cols);
		void MakePlot(CDC *pDC, BOOL logscale=FALSE);
		BOOL ToDisk();

	private:
};
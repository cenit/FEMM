
class CFullMatrix{

	public:

		// data members
		double **M;			// Matrix on LHS
		double *b;			// vector on RHS
		long n;				// dimension of the matrix

		// member functions
		CFullMatrix();
		CFullMatrix(int d);
		~CFullMatrix();
		void Wipe();
		BOOL Create(int d);
		BOOL GaussSolve();

	private:

};

class CComplexFullMatrix{

    public:

        // data members
        CComplex **M;           // Matrix on LHS
        CComplex *b;            // vector on RHS
        int n;              // dimension of the matrix

        // member functions
        CComplexFullMatrix();
        CComplexFullMatrix(int d);
        ~CComplexFullMatrix();
        void Wipe();
        BOOL Create(int d);
        BOOL GaussSolve();

    private:

};

class CFullMatrix{

	public:

		// data members
		CComplex **M;			// Matrix on LHS
		CComplex *b;			// vector on RHS
		int n;				// dimension of the matrix

		// member functions
		CFullMatrix();
		CFullMatrix(int d);
		~CFullMatrix();
		void Wipe();
		BOOL Create(int d);
		BOOL GaussSolve();

	private:

};

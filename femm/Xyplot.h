class CXYPlot {
  public:
  // data members
  double** M;
  char** lbls;
  int NumRows;
  int NumCols;

  // member functions
  CXYPlot();
  ~CXYPlot();
  BOOL Create(int rows, int cols);
  void MakePlot(CDC* pDC, char* BoundingBox = NULL);
  BOOL ToDisk(int fileformat, CString ToFile = "");

  private:
};
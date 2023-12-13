# Compiler and flags
CXX        = icpc
CXXFLAGS   = -O3 -std=c++11
OMPFLAGS   = -qopenmp  # Enable OpenMP
MPICXX     = mpicxx    # MPI Compiler

# Executable names
EXEC_REGULAR = quicksort
EXEC_OMP     = quicksort_openmp
EXEC_MPI     = quicksort_mpi

# all target
all: $(EXEC_REGULAR) $(EXEC_OMP) $(EXEC_MPI)

# Regular Quicksort
$(EXEC_REGULAR): quicksort.cpp
	$(CXX) $(CXXFLAGS) -o $(EXEC_REGULAR) quicksort.cpp

# OpenMP Quicksort
$(EXEC_OMP): quicksort_openmp.cpp
	$(CXX) $(CXXFLAGS) $(OMPFLAGS) -o $(EXEC_OMP) quicksort_openmp.cpp

# MPI Quicksort
$(EXEC_MPI): quicksort_mpi.cpp
	$(MPICXX) $(CXXFLAGS) -o $(EXEC_MPI) quicksort_mpi.cpp

# Clean target
clean:
	rm -f $(EXEC_REGULAR) $(EXEC_OMP) $(EXEC_MPI) *.o *.d

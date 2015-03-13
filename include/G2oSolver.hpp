#ifndef SLAM_G2O_SOLVER_HPP
#define SLAM_G2O_SOLVER_HPP

#include "Solver.hpp"
#include "g2o/core/sparse_optimizer.h"

namespace slam
{	
	/**
	 * @class G2oSolver
	 * @author Sebastian Kasperski
	 * @date 03/13/15
	 * @file G2oSolver.hpp
	 * @brief A solver for graph otimization that uses the g2o-backend.
	 * See: https://github.com/RainerKuemmerle/g2o
	 */
	class G2oSolver : public Solver
	{
	public:
		G2oSolver(Logger* logger);
		~G2oSolver();
		
		void addNode(const VertexObject &v, int id);
		void addConstraint(const EdgeObject &e, int source, int target);
		void compute();
		void saveGraph(std::string filename);
		
		IdPoseVector getCorrections();
		
	protected:
		g2o::SparseOptimizer mOptimizer;
		IdPoseVector mCorrections;
	};
}

#endif
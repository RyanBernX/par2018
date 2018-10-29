/**
 * @file   scl.h
 * @author Ruo Li <rli@math.pku.edu.cn>
 * @date   Wed Jan 4 11:23:56 2012
 * 
 * @brief  scl(scalar conservation law)
 * 
 * 
 */

#include <AFEPack/EasyMesh.h>
#include <AFEPack/TemplateElement.h>
#include <AFEPack/Operator.h>
#include <AFEPack/DGFEMSpace.h>
#include <AFEPack/mpi/MPI_Controller.h>
#include <AFEPack/mpi/MPI_HGeometry.h>
#include <AFEPack/mpi/MPI_ULoadBalance.h>
#include <AFEPack/mpi/MPI_SyncProp.h>
#include <AFEPack/mpi/MPI_MemoryReclaimer.h>

#define DIM 2
#define CFL 0.25

typedef AFEPack::MPI::HGeometryForest<DIM> tree_t;
typedef AFEPack::MPI::BirdView<tree_t> ir_mesh_t;

using namespace AFEPack;
typedef RegularMesh<DIM> mesh_t;
typedef DGFEMSpace<double,DIM> fe_space_t;
typedef FEMFunction<double,DIM> fe_func_t;
typedef Element<double,DIM> element_t;

class SCL {
 private:
  tree_t tree;
  ir_mesh_t  * p_ir_mesh;
  fe_space_t * p_sp;

  TemplateGeometry<DIM>          tmp_geo_0, tmp_geo_1;
  CoordTransform<DIM,DIM>        crd_trs_0, crd_trs_1;
  TemplateDOF<DIM>               tmp_dof_0, tmp_dof_1;
  BasisFunctionAdmin<double,DIM> bas_fun_0, bas_fun_1;
  UnitOutNormal<DIM>             out_nrm_0, out_nrm_1;
  std::vector<TemplateElement<double,DIM> > tmp_ele;

  TemplateGeometry<DIM-1>        tmp_geo_2;
  CoordTransform<DIM-1,DIM>      crd_trs_2;
  std::vector<TemplateDGElement<DIM-1,DIM> > tmp_edge;

  Indicator<DIM> ind;

  fe_func_t u_h[DIM+2];
  fe_func_t du[DIM+2];

  double t;
  double dt;

 public:
  void run();

  void initialize();

  void read_mesh();
  void read_template();
  void build_fe_space();
  void initial_value();

  void step_forward();

  void time_step_length();
  void edge_flux();
  void update_u_h();

  void adapt_mesh();
  void indicator();

  void load_balance();

  void save_data(const char *);
  void boundary_value(std::vector<double>& u_val,
                      const AFEPack::Point<DIM>& p,
                      const std::vector<double>& un,
                      int bm) const;
};

/**
 * end of file
 * 
 */

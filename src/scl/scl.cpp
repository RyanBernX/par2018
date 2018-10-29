/**
 * @file   scl.cpp
 * @author Ruo Li <rli@math.pku.edu.cn>
 * @date   Wed Jan 4 11:48:58 2012
 * 
 * @brief  
 * 
 * 
 */

#include "scl.h"

#define GAMMA (1.4)

void _u_0(const Point<DIM>& p, 
          std::vector<double>& v) {
  double r = sqrt((p[0] - 1.0)*(p[0] - 1.0) + p[1]*p[1]);
  if (r > 0.5) {
    v[0] = 1.0;
    v[1] = 0.0;
    v[2] = 0.0;
    v[3] = 1.0e+05/(GAMMA - 1);
  } else {
    v[0] = 1.0;
    v[1] = 0.0;
    v[2] = 0.0;
    v[3] = 1.0e+07/(GAMMA - 1);
  }
}

void _f(const std::vector<double>& u, 
        std::vector<std::vector<double> >& f) {
  double rho = u[0];
  double V[DIM] = {u[1]/rho, u[2]/rho};
  double V2 = V[0]*V[0] + V[1]*V[1];
  double p = (GAMMA - 1)*(u[3] - 0.5*rho*V2);

  f[0][0] = rho*V[0];          f[0][1] = rho*V[1];
  f[1][0] = rho*V[0]*V[0] + p; f[1][1] = rho*V[0]*V[1];
  f[2][0] = rho*V[0]*V[1];     f[2][1] = rho*V[1]*V[1] + p;
  f[3][0] = V[0]*(u[3] + p);   f[3][1] = V[1]*(u[3] + p);
}

double _f_prime(const std::vector<double>& u) {
  double rho = u[0];
  double V[DIM] = {u[1]/rho, u[2]/rho};
  double V2 = V[0]*V[0] + V[1]*V[1];
  double p = (GAMMA - 1)*(u[3] - 0.5*rho*V2);

  double c = sqrt(GAMMA*p/rho);
  return sqrt(V2) + c;
}

void _F(const std::vector<double>& u_l,
        const std::vector<double>& u_r,
        const std::vector<double>& un,
        std::vector<double>& f) {
  std::vector<std::vector<double> > f_l(DIM+2, std::vector<double>(DIM));
  std::vector<std::vector<double> > f_r(DIM+2, std::vector<double>(DIM));

  _f(u_l, f_l); _f(u_r, f_r);

  double lambda = std::max(_f_prime(u_l), _f_prime(u_r));
  for (int j = 0;j < DIM+2;++ j) {
    f[j] = 0.5*((innerProduct(f_l[j], un) + innerProduct(f_r[j], un)) -
                lambda*(u_r[j] - u_l[j]));
  }
}

void SCL::read_mesh() {
  tree.set_communicator(MPI_COMM_WORLD);
  Migration::initialize(MPI_COMM_WORLD);

  p_ir_mesh = new ir_mesh_t(tree);
  AFEPack::MPI::load_mesh("D", tree, *p_ir_mesh, true);
  p_ir_mesh->semiregularize();
  p_ir_mesh->regularize();
}

void SCL::read_template() {
  tmp_geo_0.readData("triangle.tmp_geo");
  crd_trs_0.readData("triangle.crd_trs");
  tmp_dof_0.reinit(tmp_geo_0);
  tmp_dof_0.readData("triangle.0.tmp_dof");
  bas_fun_0.reinit(tmp_dof_0);
  bas_fun_0.readData("triangle.0.bas_fun");
  out_nrm_0.readData("triangle.out_nrm");

  tmp_geo_1.readData("twin_triangle.tmp_geo");
  crd_trs_1.readData("twin_triangle.crd_trs");
  tmp_dof_1.reinit(tmp_geo_1);
  tmp_dof_1.readData("twin_triangle.0.tmp_dof");
  bas_fun_1.reinit(tmp_dof_1);
  bas_fun_1.readData("twin_triangle.0.bas_fun");
  out_nrm_1.readData("twin_triangle.out_nrm");

  tmp_ele.resize(2);
  tmp_ele[0].reinit(tmp_geo_0, tmp_dof_0, crd_trs_0, bas_fun_0, out_nrm_0);
  tmp_ele[1].reinit(tmp_geo_1, tmp_dof_1, crd_trs_1, bas_fun_1, out_nrm_1);

  tmp_geo_2.readData("interval.tmp_geo");
  crd_trs_2.readData("interval.to2d.crd_trs");

  tmp_edge.resize(1);
  tmp_edge[0].reinit(tmp_geo_2, crd_trs_2);
}

void SCL::build_fe_space() {
  mesh_t& mesh = p_ir_mesh->regularMesh();
  p_sp = new fe_space_t(mesh, tmp_ele, tmp_edge);

  int n_ele = mesh.n_geometry(DIM);
  p_sp->element().resize(n_ele);
  for (int i = 0;i < n_ele;++ i) {
    const GeometryBM& geo = mesh.geometry(DIM, i);
    if (geo.n_vertex() == 3) {
      p_sp->element(i).reinit(*p_sp, i, 0);
    } else {
      p_sp->element(i).reinit(*p_sp, i, 1);
    }
  }
  p_sp->buildElement();
  p_sp->buildDof();
  p_sp->buildDofBoundaryMark();

  int n_edge = mesh.n_geometry(DIM-1);
  p_sp->dgElement().resize(n_edge);
  for (int i = 0;i < n_edge;++ i) {
    p_sp->dgElement(i).reinit(*p_sp, i, 0);
  }
  p_sp->buildDGElement();
}

void SCL::initial_value() {
  for (int i = 0;i < DIM+2;++ i) {
    u_h[i].reinit(*p_sp);
  }

  int n_dof = p_sp->n_dof();
  for (int i = 0;i < n_dof;++ i) {
    const Point<DIM>& ip = p_sp->dofInfo(i).interp_point;
    std::vector<double> v(DIM+2);
    _u_0(ip, v);
    for (int j = 0;j < DIM+2;++ j) {
      u_h[j](i) = v[j];
    }
  }

  t = 0.0;
}

void SCL::initialize() {
  read_template();
  read_mesh();
  build_fe_space();
  initial_value();

  for (u_int step = 0;step < 10;++ step) {
    adapt_mesh();
    load_balance();
    initial_value();
  }
}

void SCL::run() {
  u_int step = 0;
  do {
    step_forward();
    step += 1;
    if (step%10 == 1) {
      adapt_mesh();
      if (step%100 == 1) {
        load_balance();
      }
    }
    AFEPack::MPI::getControl();
  } while (true);
}

void SCL::step_forward() {
  time_step_length();
  edge_flux();
  update_u_h();

  t += dt;
  if (tree.rank() == 0) {
    std::cout << "t = " << t 
              << ", dt = " << dt
              << std::endl;
  }
}

void SCL::time_step_length() {
  mesh_t& mesh = p_ir_mesh->regularMesh();
  dt = 1.0e+08;

  fe_space_t::ElementIterator
    the_ele = p_sp->beginElement(),
    end_ele = p_sp->endElement();
  for (;the_ele != end_ele;++ the_ele) {
    const GeometryBM& geo = the_ele->geometry();

    const Point<DIM>& p0 = mesh.point(geo.vertex(0));
    const Point<DIM>& p1 = mesh.point(geo.vertex(1));
    const Point<DIM>& p2 = mesh.point(geo.vertex(geo.n_vertex()-1));

    double l[3] = {
      distance(p0, p1), distance(p1, p2), distance(p2, p0)
    };

    double min_l = *std::min_element(&l[0], &l[3]);

    std::vector<double> u_h_val(DIM+2);
    for (int j = 0;j < DIM+2;++ j) {
      u_h_val[j] = u_h[j].value(p0, *the_ele);
    }
    
    double f_u_prime = _f_prime(u_h_val);
    dt = std::min(dt, min_l/std::max(f_u_prime, 1.0e-08));
  }

  double rank_dt = dt;
  MPI_Allreduce(&rank_dt, &dt, 1, MPI_DOUBLE, 
                MPI_MIN, tree.communicator());
  dt *= CFL;
}


void SCL::edge_flux() {
  mesh_t& mesh = p_ir_mesh->regularMesh();

  property_id_t<std::vector<double> > pid_u_val;
  new_property_id(pid_u_val);
  fe_space_t::DGElementIterator
    the_edge = p_sp->beginDGElement(),
    end_edge = p_sp->endDGElement();
  for (;the_edge != end_edge;++ the_edge) {
    int bm = the_edge->boundaryMark();
    const element_t * p_neigh1 = the_edge->p_neighbourElement(1);
    if ((bm != 0) || (p_neigh1 != NULL)) continue;

    const GeometryBM& geo = the_edge->geometry();
    const Point<DIM>& p0 = mesh.point(geo.vertex(0));
    const Point<DIM>& p1 = mesh.point(geo.vertex(1));
    Point<DIM> p = midpoint(p0, p1);

    const element_t * p_neigh0 = the_edge->p_neighbourElement(0);

    std::vector<double> * p_u_val = the_edge->new_property(pid_u_val);
    p_u_val->resize(DIM+2);
    for (int j = 0;j < DIM+2;++ j) {
      (*p_u_val)[j] = u_h[j].value(p, *p_neigh0);
    }
  }
  AFEPack::MPI::PropSyncer<tree_t,std::vector<double> > syncer(tree, pid_u_val);
  syncer.sync(DIM-1);

  for (int j = 0;j < DIM+2;++ j) {
    du[j].reinit(*p_sp);
  }
  the_edge = p_sp->beginDGElement();
  for (;the_edge != end_edge;++ the_edge) {
    const GeometryBM& geo = the_edge->geometry();
    const Point<DIM>& p0 = mesh.point(geo.vertex(0));
    const Point<DIM>& p1 = mesh.point(geo.vertex(1));
    double l = distance(p0, p1);

    Point<DIM> p = midpoint(p0, p1);

    const element_t * p_neigh0 = the_edge->p_neighbourElement(0);
    const element_t * p_neigh1 = the_edge->p_neighbourElement(1);

    std::vector<double> un = unitOutNormal(p, *p_neigh0, *the_edge);

    std::vector<double> u_h_val0(DIM+2);
    for (int j = 0;j < DIM+2;++ j) {
      u_h_val0[j] = u_h[j].value(p, *p_neigh0);
    }

    std::vector<double> u_h_val1(DIM+2);
    if (p_neigh1 == NULL) { /// 使用边界条件
      int bm = the_edge->boundaryMark();
      if (bm == 0) {
        u_h_val1 = *(the_edge->get_property(pid_u_val));
      } else {
        u_h_val1 = u_h_val0;
        boundary_value(u_h_val1, p, un, bm);
      }
    } else {
      for (int j = 0;j < DIM+2;++ j) {
        u_h_val1[j] = u_h[j].value(p, *p_neigh1);
      }
    }

    std::vector<double> flux(DIM+2);
    _F(u_h_val0, u_h_val1, un, flux);

    for (int j = 0;j < DIM+2;++ j) {
      double a = l*flux[j];

      const std::vector<int>& ele_dof0 = p_neigh0->dof();
      du[j](ele_dof0[0]) -= a;

      if (p_neigh1 != NULL) {
        const std::vector<int>& ele_dof1 = p_neigh1->dof();
        du[j](ele_dof1[0]) += a;
      }
    }
  }
}

void SCL::update_u_h() {
  mesh_t& mesh = p_ir_mesh->regularMesh();

  fe_space_t::ElementIterator
    the_ele = p_sp->beginElement(),
    end_ele = p_sp->endElement();
  for (;the_ele != end_ele;++ the_ele) {
    const GeometryBM& geo = the_ele->geometry();

    const Point<DIM>& p0 = mesh.point(geo.vertex(0));
    const Point<DIM>& p1 = mesh.point(geo.vertex(1));
    const Point<DIM>& p2 = mesh.point(geo.vertex(geo.n_vertex()-1));
    
    double v = ((p1[0] - p0[0])*(p2[1] - p0[1]) - 
                (p1[1] - p0[1])*(p2[0] - p0[0]))/2.0;

    const std::vector<int>& ele_dof = the_ele->dof();
    for (int j = 0;j < DIM+2;++ j) {
      u_h[j](ele_dof[0]) += dt*du[j](ele_dof[0])/v;
    }
  }
}

void SCL::boundary_value(std::vector<double>& u_val,
                         const Point<DIM>& p,
                         const std::vector<double>& un,
                         int bm) const {
  double a = u_val[1]*un[0] + u_val[2]*un[1];
  u_val[1] -= 2*a*un[0];
  u_val[2] -= 2*a*un[1];
}

void SCL::save_data(const char * args) {
  if (tree.rank() == 0) 
    std::cout << "Saving data ..." << std::endl;
  const char * var_name[] = { "rho", "rhou", "rhov", "E" };
  char filename[256];
  for (int j = 0;j < DIM+2;++ j) {
    sprintf(filename, "%s_%d.dx", var_name[j], tree.rank());
    u_h[j].writeOpenDXData(filename, 1);
  }
}

void SCL::indicator() {
  mesh_t& mesh = p_ir_mesh->regularMesh();
  ind.reinit(mesh);

  property_id_t<std::vector<double> > pid_u_val;
  new_property_id(pid_u_val);
  fe_space_t::DGElementIterator
    the_edge = p_sp->beginDGElement(),
    end_edge = p_sp->endDGElement();
  for (;the_edge != end_edge;++ the_edge) {
    int bm = the_edge->boundaryMark();
    const element_t * p_neigh1 = the_edge->p_neighbourElement(1);
    if ((bm != 0) || (p_neigh1 != NULL)) continue;

    const GeometryBM& geo = the_edge->geometry();
    const Point<DIM>& p0 = mesh.point(geo.vertex(0));
    const Point<DIM>& p1 = mesh.point(geo.vertex(1));
    Point<DIM> p = midpoint(p0, p1);

    const element_t * p_neigh0 = the_edge->p_neighbourElement(0);

    std::vector<double> * p_u_val = the_edge->new_property(pid_u_val);
    p_u_val->resize(DIM+2);
    for (int j = 0;j < DIM+2;++ j) {
      (*p_u_val)[j] = u_h[j].value(p, *p_neigh0);
    }
  }
  AFEPack::MPI::PropSyncer<tree_t,std::vector<double> > syncer(tree, pid_u_val);
  syncer.sync(DIM-1);

  the_edge = p_sp->beginDGElement();
  for (;the_edge != end_edge;++ the_edge) {
    const GeometryBM& geo = the_edge->geometry();
    const Point<DIM>& p0 = mesh.point(geo.vertex(0));
    const Point<DIM>& p1 = mesh.point(geo.vertex(1));
    double l = distance(p0, p1);

    Point<DIM> p = midpoint(p0, p1);

    const element_t * p_neigh0 = the_edge->p_neighbourElement(0);
    const element_t * p_neigh1 = the_edge->p_neighbourElement(1);

    std::vector<double> un = unitOutNormal(p, *p_neigh0, *the_edge);

    std::vector<double> u_h_val0(DIM+2);
    for (int j = 0;j < DIM+2;++ j) {
      u_h_val0[j] = u_h[j].value(p, *p_neigh0);
    }
    std::vector<double> u_h_val1(DIM+2);
    if (p_neigh1 == NULL) { /// 使用边界条件
      int bm = the_edge->boundaryMark();
      if (bm == 0) {
        u_h_val1 = *(the_edge->get_property(pid_u_val));
      } else {
        u_h_val1 = u_h_val0;
        boundary_value(u_h_val1, p, un, bm);
      }
    } else {
      for (int j = 0;j < DIM+2;++ j) {
        u_h_val1[j] = u_h[j].value(p, *p_neigh1);
      }
    }
    
    double edge_ind = 0.0;
    for (int j = 0;j < DIM+2;++ j) {
      edge_ind += fabs(u_h_val0[j] - u_h_val1[j]);
    }
    edge_ind *= l*l;

    ind[p_neigh0->index()] += edge_ind;
    if (p_neigh1 != NULL) {
      ind[p_neigh1->index()] += edge_ind;
    }
  }
}

void SCL::adapt_mesh() {
  indicator(); /// 计算自适应指示子

  double convergence_order = 1.0;
  double adapt_tolerence = 5.0e+01;

  ir_mesh_t * old_ir_mesh = p_ir_mesh;
  fe_space_t * old_fem_space = p_sp;
  fe_func_t old_u_h[DIM+2];
  for (int j = 0;j < DIM+2;++ j) {
    old_u_h[j] = u_h[j];
  }

  p_ir_mesh = new ir_mesh_t(*old_ir_mesh);
  MeshAdaptor<DIM> mesh_adaptor(*p_ir_mesh);
  mesh_adaptor.convergenceOrder() = convergence_order;
  mesh_adaptor.refineStep() = 1;
  mesh_adaptor.setIndicator(ind);
  mesh_adaptor.tolerence() = adapt_tolerence;
  mesh_adaptor.adapt();

  /**< 正则化自适应后的网格 */
  p_ir_mesh->semiregularize();
  p_ir_mesh->regularize(false);

  /**< 建立新的有限元空间和将解更新到新的空间 */
  build_fe_space();
  for (int j = 0;j < DIM+2;++ j) {
    u_h[j].reinit(*p_sp);
    Operator::L2Interpolate(old_u_h[j], u_h[j]);
  }

  /**< 释放旧的网格、有限元空间以及解的内存 */
  delete old_fem_space; old_fem_space = NULL;
  delete old_ir_mesh; old_ir_mesh = NULL;
}

void SCL::load_balance()
{
  /// 然后对数据进行重新分区存储
  AFEPack::MPI::HLoadBalance<tree_t> hlb(tree);
  hlb.config(*p_ir_mesh); /// 使用 ir_mesh 进行负载计算
  hlb.partition(tree.n_rank()); /// 重新分区

  AFEPack::MPI::BirdViewSet<tree_t> bvs;
  bvs.add(*p_ir_mesh); /// 存储时对 ir_mesh 也进行存储

  Migration::clear_data_buffer(tree); /// 清除不用的数据
  Migration::data_id_t id[DIM+2];
  const char * var_name[] = {
    "rho", "rho_u", "rho_v", "E"
  };
  for (int j = 0;j < DIM+2;++ j) {
    id[j] = Migration::name_to_id(var_name[j]);
    if (! Migration::is_valid(id[j])) { /// 获取数据ID，如果没有则新建一个
      id[j] = Migration::register_data_name(var_name[j]);
    }
    Migration::export_fe_func(u_h[j], id[j]); /// 将有限元函数放到几何遗传树上
  }

  if (tree.rank() == 0) {
    std::cerr << "Rebalance the loading ... " << std::endl;
    int result;
    result = system("rm -rf .hlb.bak; mv .hlb .hlb.bak");
  }
  MPI_Barrier(tree.communicator()); /// 同步以后
  hlb.save_data(".hlb", bvs); /// 存储数据

  /// 清除内存
  delete p_sp; /// 所以这两个变量都直接删除
  p_ir_mesh->clear(); /// 网格和几何遗传树会重新载入
  tree.clear(); /// 因此清除其中的数据空间

  /// 最后将数据重新载入
  hlb.load_data(".hlb", bvs, true); /// 载入数据
  p_ir_mesh->semiregularize(); /// 重造正则网格
  p_ir_mesh->regularize(false);

  build_fe_space(); /// 重建有限元空间
  for (int j = 0;j < DIM+2;++ j) {
    u_h[j].reinit(*p_sp); /// 和有限元函数
    Migration::import_fe_func(u_h[j], id[j]); /// 将有限元函数从几何遗传树上读出
  }
  Migration::clear_data_buffer(tree); /// 将几何遗传树上的数据清除

  AFEPack::MPI::MemoryReclaimer<tree_t> mr(tree);
  mr.addIrregularMesh(*p_ir_mesh);
  mr.reclaim();
}

/**
 * end of file
 * 
 */

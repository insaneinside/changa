/*****************************************************************************
 * $Source$
 * $Author$
 * $Date$
 * $Revision$
 *****************************************************************************/

/**
 * \addtogroup CkLdb
*/
/*@{*/

#ifndef _MULTISTEPLB_H_
#define _MULTISTEPLB_H_

#define MCLBMS          // multistepping enabled
#define MCLB_ORBSMOOTH  // orbsmooth for large steps
#define MCLB_RR         // round robin otherwise

#include "CentralLB.h"

#include "MapStructures.h"
#include "ScaleTranMapBG.h"
#include "ScaledORBMapBG.h"

#include "MultistepLB.decl.h"

void CreateMultistepLB();
BaseLB * AllocateMultistepLB();

//**************************************
// ORB3DLB functions
//**************************************
static int comparx(const void *a, const void *b){
  const TPObject *ta = reinterpret_cast<const TPObject*>(a);
  const TPObject *tb = reinterpret_cast<const TPObject*>(b);
  return ta->centroid.x < tb->centroid.x ? -1 : ta->centroid.x > tb->centroid.x ? 1 : 0;
}
static int compary(const void *a, const void *b){
  const TPObject *ta = reinterpret_cast<const TPObject*>(a);
  const TPObject *tb = reinterpret_cast<const TPObject*>(b);
  return ta->centroid.y < tb->centroid.y ? -1 : ta->centroid.y > tb->centroid.y ? 1 : 0;
}
static int comparz(const void *a, const void *b){
  const TPObject *ta = reinterpret_cast<const TPObject*>(a);
  const TPObject *tb = reinterpret_cast<const TPObject*>(b);
  return ta->centroid.z < tb->centroid.z ? -1 : ta->centroid.z > tb->centroid.z ? 1 : 0;
}

static int pcx(const void *a, const void *b){
  const Node *ta = reinterpret_cast<const Node*>(a);
  const Node *tb = reinterpret_cast<const Node*>(b);
  return ta->x < tb->x ? -1 : ta->x > tb->x ? 1 : 0;
}
static int pcy(const void *a, const void *b){
  const Node *ta = reinterpret_cast<const Node*>(a);
  const Node *tb = reinterpret_cast<const Node*>(b);
  return ta->y < tb->y ? -1 : ta->y > tb->y ? 1 : 0;
}
static int pcz(const void *a, const void *b){
  const Node *ta = reinterpret_cast<const Node*>(a);
  const Node *tb = reinterpret_cast<const Node*>(b);
  return ta->z < tb->z ? -1 : ta->z > tb->z ? 1 : 0;
}
//**************************************


class LightweightLDStats1 {
  public:
  int n_objs;
  int n_migrateobjs;
  CkVec<LDObjData> objData;

  void pup(PUP::er &p);
};


class MultistepLB : public CentralLB {
private:
  bool firstRound;
  bool haveTPCentroids;
  ComparatorFn compares[NDIMS];
  ComparatorFn pc[NDIMS];

  TaggedVector3D *tpCentroids;
  CkReductionMsg *tpmsg;
  int nrecvd;
  CkVec<int> *mapping;

  int procsPerNode;

  CkVec<LightweightLDStats1> savedPhaseStats;       // stats saved from previous phases
  
  bool QueryBalanceNow(int step);
  //int prevPhase;

  unsigned int determinePhase(unsigned int activeRung);
  void makeActiveProcessorList(BaseLB::LDStats *stats, int numActiveObjs);
  void mergeInstrumentedData(int phase, BaseLB::LDStats *phaseStats);
  bool havePhaseData(int phase); 
  void printData(BaseLB::LDStats &stats, int phase, int *revObjMap);
public:
  MultistepLB(const CkLBOptions &);
  MultistepLB(CkMigrateMessage *m):CentralLB(m) { 
    lbname = "MultistepLB"; 
    compares[0] = comparx;
    compares[1] = compary;
    compares[2] = comparz;

    pc[0] = pcx;
    pc[1] = pcy;
    pc[2] = pcz;

  }
    
  void work(BaseLB::LDStats* stats);
  void receiveCentroids(CkReductionMsg *msg);
  //ScaleTranMapBG map;

public:/* <- Sun CC demands Partition be public for ComputeLoad to access it. */

  class Partition {
  public:
    int refno;
    double load;			// total load in this set
    int origin[3];			// box coordinates
    int corner[3];
    int  count;				// number of objects in this partition
    int node, mapped;
    CkVec<int>   bkpes;			// background processors
  public:
    Partition(): refno(0), load(0.0), node(-1), mapped(0) {};
  };

private:  
  struct ComputeLoad {
    int id;
    int v[3];
    double load;
    int  refno;
    double  tv;
    Partition * partition;
  };
  
  
  struct VecArray {
    int v;
    int id;
  };
  
  enum {XDIR=0, YDIR, ZDIR};
  
public:
  double overLoad;
  
//**************************************
// ORB3DLB functions
//**************************************
//
  void work2(BaseLB::LDStats* stats, int count, int phase, int prevPhase);
  void greedy(BaseLB::LDStats* stats, int count, int phase, int prevPhase);
  void directMap(TPObject *tp, int ntp, Node *nodes);
  void map(TPObject *tp, int ntp, int nn, Node *procs, int xs, int ys, int zs, int dim);
  int nextDim(int dim, int xs, int ys, int zs);
  TPObject *partitionEvenLoad(TPObject *tp, int &ntp);
  Node *halveNodes(Node *start, int np);

  void pup(PUP::er &p);
};

#endif /* _MultistepLB */

/*@}*/

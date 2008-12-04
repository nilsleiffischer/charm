#ifndef _VERBOSE_H
#define _VERBOSE_H

#include <stdio.h>
#include <errno.h>

#include "trace.h"
#include "envelope.h"
#include "register.h"
#include "trace-common.h"

/**
 *  TraceControlPoints is a template for an instrumentation module making use of
 *    the tracing framework hooks provided in Charm++.
 *
 *  Only the more common hooks are listened to in this module.
 */
class TraceControlPoints : public Trace {
 public:

  double lastBeginExecuteTime;
  int lastbeginMessageSize;

  /** The amount of time spent executing entry methods */
  double totalEntryMethodTime;

  /** The start of the idle region */
  double lastBeginIdle;
  
  /** The amount of time spent idle */
  double totalIdleTime;
  
  /** The time we last rest the idle/entry totals */
  double lastResetTime;

 public:
  TraceControlPoints(char **argv);
  
  // a user event has just occured
  void userEvent(int eventID);
  // a pair of begin/end user event has just occured
  void userBracketEvent(int eventID, double bt, double et);
  
  // "creation" of message(s) - message Sends
  void creation(envelope *, int epIdx, int num=1);
  void creationMulticast(envelope *, int epIdx, int num=1, int *pelist=NULL);
  void creationDone(int num=1);
  
  void messageRecv(char *env, int pe);
  
  // **************************************************************
  // begin/end execution of a Charm++ entry point
  // NOTE: begin/endPack and begin/endUnpack can be called in between
  //       a beginExecute and its corresponding endExecute.
  void beginExecute(envelope *);
  void beginExecute(CmiObjId *tid);
  void beginExecute(
		    int event,   // event type defined in trace-common.h
		    int msgType, // message type
		    int ep,      // Charm++ entry point id
		    int srcPe,   // Which PE originated the call
		    int ml,      // message size
		    CmiObjId* idx);    // index
  void endExecute(void);
  
  // begin/end idle time for this pe
  void beginIdle(double curWallTime);
  void endIdle(double curWallTime);
  
  // begin/end of execution
  void beginComputation(void);
  void endComputation(void);
  
  /* Memory tracing */
  void malloc(void *where, int size, void **stack, int stackSize);
  void free(void *where, int size);
  
  // do any clean-up necessary for tracing
  void traceClose();




  // ==================================================================
  // The following methods are not required for a tracing module

  /** reset the idle time and entry method execution time accumulators */
  void resetTimings();

  /** What fraction of the time is spent idle */
  double idleRatio(){
    return (totalIdleTime) / (CmiWallTimer() - lastResetTime);
  }

};


TraceControlPoints *localControlPointTracingInstance();


#endif

/*@}*/

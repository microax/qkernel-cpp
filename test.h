// TEST.H	 Definition file for TEST.
//-----------------------------------------------------------------------------
// History:
// --------
// 07/07/96 M. Gill	Initial creation.
//-----------------------------------------------------------------------------
#ifndef TEST_H
#define TEST_H

class TEST_DAEMON : public CDaemon
{
public:

  TEST_DAEMON(const char *appcde, const char *logdir);
  void CreateAndStartMainThreads(void);

};
#endif









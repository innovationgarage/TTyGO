#define MAX_TASKS 6

class SchedulerCls;

class Task {
  friend SchedulerCls;
  protected:
    unsigned long next_tick;

    void init(SchedulerCls *scheduler);
    virtual void setup();
    virtual void loop();

  public:
    void delay(int ms);
    
//  private:
    SchedulerCls *sched;
};

class SchedulerCls {
  public:
     SchedulerCls();
     void start(Task *task);
     void begin();
     
//  private:
    int nr_tasks;
    Task *tasks[MAX_TASKS];
};

extern SchedulerCls Scheduler;

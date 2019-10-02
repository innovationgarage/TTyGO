#ifndef USE_ESP8266SCHEDULER

void Task::init(SchedulerCls *scheduler)
{
  // Serial.print("Task.init() for "); Serial.print(scheduler->nr_tasks); Serial.print("("); Serial.print((int) this); Serial.print(")\n"); Serial.flush();
  sched = scheduler;
  next_tick = 0;
}

void Task::delay(int ms)
{
  next_tick = millis() + ms;
}

SchedulerCls::SchedulerCls()
{
  nr_tasks = 0;
}

void SchedulerCls::start(Task *task)
{
  tasks[nr_tasks] = task;
  // Serial.print("Running init for "); Serial.print(nr_tasks); Serial.print("("); Serial.print((int) task); Serial.print(")\n");
  task->init(this);
  nr_tasks++;
}

void SchedulerCls::begin()
{
   int idx;
   unsigned long next_tick, now;

   // Serial.print("Scheduler.begin\n"); Serial.flush();
   for (idx = 0; idx < nr_tasks; idx++)
   {
     // Serial.print("Scheduler.begin."); Serial.print(idx); Serial.print("\n"); Serial.flush();
     tasks[idx]->setup();
   }

   millis();
   while (1)
   {
     for (idx = 0; idx < nr_tasks; idx++)
     {
       if (tasks[idx]->next_tick <= now)
       {
         tasks[idx]->loop();
       }
     }
     next_tick = tasks[0]->next_tick;
     for (idx = 0; idx < nr_tasks; idx++)
     {
       if (tasks[idx]->next_tick < next_tick)
       {
         next_tick = tasks[idx]->next_tick;
       }
     }
     now = millis();
     if (next_tick > now)
     {
       // Serial.print("Scheduler.delay:"); Serial.print(next_tick - now); Serial.print("\n"); Serial.flush();
       delay(next_tick - now);
       now = millis();
     }
   }
}

SchedulerCls Scheduler;

#endif
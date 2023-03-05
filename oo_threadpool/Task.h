/* File Name Task.h
 * Author 泷fengyoukai
 * Github https://github.com/1750252467
 * Created Time 2023-03-04 18:47
 * Last Modified 2023-03-04 18:49
 */
#ifndef __TASK_H__
#define __TASK_H__


class Task{
public:
    Task()
    {
    }

    virtual void process() = 0;

private:

};

#endif // __TASK_H__

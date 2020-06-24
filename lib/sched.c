#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *  Search through 'envs' for a runnable environment ,
 *  in circular fashion statrting after the previously running env,
 *  and switch to the first such environment found.
 *
 * Hints:
 *  The variable which is for counting should be defined as 'static'.
 */

extern struct Env_list env_sched_list[];
extern struct Env *curenv;

void sched_yield(void)
{
	static int cur_pri = 1;
	static int cur_index = 0;
	struct Env *next_env;
	int find = 0;
	cur_pri--;
	if (cur_pri <= 0 || curenv == NULL 
        || curenv->env_status != ENV_RUNNABLE)
	{
		find = 0;
		while(1)
		{
			if (LIST_EMPTY(&env_sched_list[cur_index]))
			{
				cur_index = !cur_index;
				break;
			}
			next_env = LIST_FIRST(&env_sched_list[cur_index]);
			if (next_env->env_status == ENV_RUNNABLE)
			{
				find = 1;
				break;
			}
			LIST_REMOVE(next_env, env_sched_link);
			LIST_INSERT_HEAD(&env_sched_list[!cur_index], next_env, env_sched_link);
		}
		if (!find)
		{
			while (1)
			{
				if (LIST_EMPTY(&env_sched_list[cur_index]))
				{
					panic("^^^^^^^^No env is RUNNABLE!^^^^^^^^");
				}
				next_env = LIST_FIRST(&env_sched_list[cur_index]);
				if (next_env->env_status == ENV_RUNNABLE)
				{
					find = 1;
					break;
				}
				LIST_REMOVE(next_env, env_sched_link);
				LIST_INSERT_HEAD(&env_sched_list[!cur_index], next_env, env_sched_link);
			}
		}
		LIST_REMOVE(next_env, env_sched_link);
		LIST_INSERT_HEAD(&env_sched_list[!cur_index], next_env, env_sched_link);
		cur_pri = next_env->env_pri;
		env_run(next_env);
		panic("^^^^^^^^sched yield jump faild^^^^^^^^");
	}
	env_run(curenv);
	panic("^^^^^^^^sched yield reached end^^^^^^^^");
}
This module provides an API for queued asynchronous jobs that have a high chance of failing and
subsequentely need to be retried.

If you want to use this module, you need to implement 2 things. First of all job_scheduler_retry_job_scheduler_retry_info() 
allows you to define your queues and the way you want to retry items. See job_scheduler_retry_api.inc for documentation 
on that hook.

Secondly, you need to add items to your queues. Here is an example of that:

We want to call your_module_your_method($param1, $param2);
If that method succeeds, we want to hand the job over to 

    $job = array(
      'callback_main_function' => 'your_module_your_method',
      'callback_main_function_arguments' => array ($param1, $param2),
      'callback_success_function' => 'your_module_your_method_success',
      'callback_error_function' => 'your_module_your_method_success_error',
      'something' => 'You can add items to the job that you need later',
      // If you install the advanced_queues module, all items in the advanced_queue table
      // have a title field. You can show that field in views, which can be handy
      'title' => $unique_id,
    );
    // firstqueue is defined by you in your implementation of hook_job_scheduler_retry_info()
    $queue = DrupalQueue::get('firstqueue');
    $queue->createItem($job);


This module is a pure API module. If there is no module that requires you to install this, then there is no use for you to do so.

This module requires job_scheduler.
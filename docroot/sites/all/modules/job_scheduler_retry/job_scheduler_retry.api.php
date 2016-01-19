<?php

/**
 * @file
 * Hooks and samples provided by job_scheduler_retry
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provide a reschedule queue.
 *
 * Items in the arrau:
 * - title
 *   Display name
 * - name
 *   Internal name
 * - Description
 *   Additional text
 * - retries
 *   How often will items be put back into this queue
 * - next_queue
 *   After the number of retries > retries the item will be moved to
 *   this queue. This way you can first retry a few times every 5 minutes,
 *   then retry every hour, twice a day and then move items to oblivion, the
 *   default endpoint
 * - max_jobs
 *   Maximum amount of jobs processed in 1 run
 * - crontab
 *   crontab style scheduling of this queue. See job_scheduler documentation for exact format
 * - weight
 *   Order in which this queue is processes. This can be relevant as job_scheduler, who calls our jobs
 *   will timeout after a certain time, and then skip all of our queues that have not been processed yet
 *
 */
function job_scheduler_retry_job_scheduler_retry_info() {
  return array (
      "oblivion" => array (
          'title' => 'Oblivion',
          'name' => "oblivion",
          'description' => 'Job runs never',
          'retries' => 0,
          'next_queue' => 'oblivion',
          'max_jobs' => 1000,
          'crontab' => '* * * * */2011', // never
          'weight' => 100,
      ),
  );
}


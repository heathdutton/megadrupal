<?php
/**
 * @file
 * Contains the $_callback function, timing and description for this cron job.
 */

 /*
  * Possible  time patterns: (See README.md for current list)
  * 'every 10 minutes'
  * 'every 2 days'
  * 'every 2 weeks
  * 'every 3 months'
  * 'every 2 years'
  * 'every 10 days after 16:00'
  * 'on the 6th of every 1 month'
  * 'on the 15th of every 1 months after 16:00'
  * 'on the 4th of July'
  * 'on the 4th of July after 14:00'
  *
  */
$_time_pattern = '';

// Just a description of what the cron does.  Used by Elysia Cron.
$_description = t('This does...');


/**
 * Anything done within this function will be executed when the cron job runs.
 *
 * @return bool
 *   TRUE if the cron function ran, FALSE if it did not.
 */
$_callback = function ()  {
  // Do whatever processing you want here.
  // If you consider it successful, Return TRUE.
  // If it was unsuccessful return FALSE.


  return FALSE;
};


// You can place other functions here that will only be available to this cron.
// Namespace them to the codit_crons_{cron_job_name} to prevent fatal naming
// collisions.

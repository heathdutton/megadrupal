<?php

/**
 * @file
 * Provide example code for creating a custom daemon.
 *
 * @see DrushDaemon
 * @see hook_daemon_api_info().
 */

/**
 * This is an Example Daemon to show you how to create your own Daemon.
 *
 * All it does is sleep so as far as getting any work done, it is not
 * very useful.
 *
 * Note that for your custom Daemon you should create a class similar to this
 * one where you spawn daemon-specific tasks in the execute_tasks() function.
 * Additionally, you must implement hook_daemon_api_info() outside of this
 * class (preferably in your .module file) to tell the Daemon API about your
 * custom daemon.
 */
class DrushDaemonExample extends DrushDaemon {

  // OPTIONAL: Set how often in seconds your execute_task() should be called.
  // Keep in mind that this time does not include the amount of time spent
  // executing your tasks. For example, if you set this to 5 seconds and you
  // have 2 tasks in your execute_tasks() function, each of which take 15
  // seconds, then your loop will iterate (and thus your execute_task()
  // function will be called again) before your tasks finish.
  // CODING STANDARDS: Can't change this variable to lowerCamel since it
  // inherits from a library class.
  protected $loop_interval = 20;

  // OPTIONAL: Set how often the Daemon should restart itself; Default 8hrs.
  // CODING STANDARDS: Can't change this variable to lowerCamel since it
  // inherits from a library class.
  protected $auto_restart_interval = 28800;

  /**
   * This is where your Daemon actually executes the tasks/jobs you want done.
   *
   * This function will be called once per loop iteration.
   *
   * @param int $iteration_number
   *   This is an integer stating the current iteration of the loop you are on.
   */
  protected function executeTask($iteration_number) {

    $nap_length = rand(1, 20);

    // Use $this->log($message) to log output to both the screen (unless
    // Daemonized) and to the log file. Make sure to tell your users what is
    // going on -especially if it might take awhile.
    $this->log('I am really tired *Yawns*! I\'m going to task myself with a '
      . $nap_length . ' second nap...');

    // Now we want to do some work. Note that the Daemon API does not support
    // the PHP-Daemon Task or Worker API's at this time due to a conflict with
    // Drupal/Drush.
    //
    // It is good practice to actually execute your commands in a separate
    // custom function (in this case task_nap() ) for readability & abstraction.
    // Simply call that function(s) here.
    $this->taskNap($nap_length);

    // It's also good practice to log when the task completes.
    $this->log('Now that I\'ve completed all my work, I\'m taking a '
      . $this->loop_interval
      . ' second break until the loop comes around again *smiles*.');

  }

  /**
   * Sleeps for the specified amount of time.
   *
   * This function has been created for demonstration purposes. Do not
   * implement this function in your custom daemon.
   *
   * @param int $nap_length
   *   An integer specifying how long to sleep.
   */
  protected function taskNap($nap_length) {

    // Note the second argument to the $this-Log function (ie: NAP). This
    // allows you to specify a label in the log file making it more readable.
    // It can be any string you like however it will be truncated at 12 chars.
    $this->log('Commensing ' . $nap_length . ' second nap.', 'NAP');

    // This is our made up task and mostly just shows how variables can be
    // passed from the execute_task() function.
    sleep($nap_length);

    // Once, again, it's good practice to let the user know you've
    // achomplished the task asked of you.
    $this->log('Waking up...', 'NAP');
  }
}

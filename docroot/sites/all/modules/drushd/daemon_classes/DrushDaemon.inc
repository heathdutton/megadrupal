<?php

/**
 * @file
 * Implements drupal-specific daemon functionality.
 */

/**
 * Extends the PHP-Daemon Core class to work with Drupal.
 *
 * This is the main class for the Daemon API and provides Drupal specific
 * settings and implements defaults for the Daemon API to make implementation
 * of Daemons easier.
 *
 * When creating a custom Daemon you should extend this class and implement your
 * own execute_tasks() method. This method is called for each iteration of the
 * loop and is where you should implement the task/job you wish your daemon to
 * carry out. Make sure to break it down to only handle the functionality you
 * want executed in a single loop iteration.
 *
 * KNOWN BUG: Can't get the CoreDaemon to realize we are already a daemon. This
 * is a bug in the PHP-Daemon library where we are not allowed to set the
 * private $this->daemon variable in the parent (Core_Daemon) class except by
 * passing the -d option to the script. Unfortunately drush using this option
 * for debugging so we actually can't use it. This bug will be fixed once
 * PHP-Daemon v2.1.1 is released since you can use the new $this->set method. I
 * could use the development version I guess but that somehow seems wrong...
 */
class DrushDaemon extends \Core_Daemon {

  // How often in seconds your execute() should be called.
  // CODING STANDARDS: Can't change this variable to lowerCamel since it
  // inherits from a library class.
  protected $loop_interval = 20;

  // How often should the Daemon restart itself; Default 8hrs.
  // CODING STANDARDS: Can't change this variable to lowerCamel since it
  // inherits from a library class.
  protected $auto_restart_interval = 28800;

  // Instructions
  // CODING STANDARDS: Can't change this variable to lowerCamel since it
  // inherits from a library class.
  protected $install_instructions = array(
    'Run tha Daemon using drush daemon start [daemon machine-name]. ',
    'See drush daemon --help for more information.',
  );

  // The current number of iterations performed.
  protected $numIterations = 0;

  // Wether or not the daemon is currently executing a task.
  protected $hibernating = TRUE;

  /**
   * Tell the PHP Simple Daemon API about any plugins.
   *
   * The only plugin we're using is a simple file-based lock
   * to prevent 2 instances from running.
   *
   * CODING STANDARDS: Can't change this method name to lowerCamel since it
   *   inherits from a library class.
   */
  protected function setup_plugins() {

    // Create a Lock File.
    $this->files_dir = file_directory_temp();
    $this->plugin(
      'Lock_File',
      new Core_Lock_File($this, array('path' => $this->files_dir))
    );
    $this->Lock_File->daemon_name = $this->setDaemonName();
  }

  /**
   * This is where you implement any once-per-execution setup code.
   *
   * CODING STANDARDS: Can't change this method name to lowerCamel since it
   *   inherits from a library class.
   */
  protected function setup() {

    // Set the name of the daemon for use in lock/log/status files.
    $this->setDaemonName();

  }

  /**
   * This is where you implement the tasks you want your daemon to perform.
   *
   * This method is called at the frequency defined by loop_interval.
   *
   * CODING STANDARDS: Can't change this method name to lowerCamel since it
   *   inherits from a library class.
   */
  protected function execute() {

    // Keep track of the number of iterations.
    $this->numIterations++;

    // Pull the Daemon out of hibernating to execute a task.
    $this->hibernating = FALSE;
    $this->setStatus();

    // Execute the Task. This function should be overridden in custom daemons.
    $this->executeTask($this->numIterations);

    // Put the daemon back to sleep.
    $this->setStatus();
    $this->hibernating = TRUE;
  }

  /**
   * Where Task(s) should be executed in custom daemons.
   *
   * This gets executed once per loop iteration and should be overridden by
   * your own class in order to execute your own task(s).
   */
  protected function executeTask($iteration_number) {
    $this->log('Your task will be run here when you call your custom daemon.'
      . ' Simply execute "drush daemon start [daemon-machine-name]".');
  }

  /**
   * Dynamically build the file name for the log file.
   *
   * CODING STANDARDS: Can't change this method name to lowerCamel since it
   *   inherits from a library class.
   *
   * @return string
   *   Log Filename including path
   */
  public function log_file() {
    $this->setDaemonName();
    return $this->files_dir . DIRECTORY_SEPARATOR . $this->daemon_name . '_' . date('Ymd') . '.log';
  }

  /**
   * Return the name of the file containing the status of this daemon.
   *
   * CODING STANDARDS: Can't change this method name to lowerCamel since it
   *   inherits from a library class.
   *
   * @return string
   *   Log Filename including path
   */
  public function status_file() {
    $this->setDaemonName();
    return $this->files_dir . DIRECTORY_SEPARATOR . $this->daemon_name . '.status';
  }

  /**
   * We want to record the status of the Daemon.
   *
   * The following information will be included in a serialized array:
   *   - PID: The PID of the main process
   *   - Hibernating: TRUE if your daemon is hibernating (ie: execute_task()
   *       is not currently executing) and FALSE otherwise
   *   - Number of Iterations: The number of times execute_task() has been
   *       executed since the Daemon was last started
   *   - Current Log File: The name including the path of the current log file
   *
   * To include additional daemon-specific settings for use in reports, override
   * get_status_details().
   */
  protected function setStatus() {

    $status_details = $this->getStatusDetails();
    file_put_contents(
      $this->status_file(),
      serialize($status_details)
    );

  }

  /**
   * Override to include additional daemon-specific settings for use in reports.
   *
   * @return array
   *   An array of status details where the key should be a human-readable
   *   name for your detail and the value should be the current state.
   */
  protected function getStatusDetails() {
    $status_details = array();

    $status_details['PID'] = getmypid();
    $status_details['Hibernating'] = $this->hibernating;
    $status_details['Number of Iterations'] = $this->numIterations;
    $status_details['Current Log File'] = $this->log_file();

    return $status_details;
  }

  /**
   * Handle command line arguments.
   *
   * We don't handle command-line arguments within the module since they
   * are already handled in the drush commands. Therefore just set default here.
   *
   * CODING STANDARDS: Can't change this method name to lowerCamel since it
   *   inherits from a library class.
   */
  protected function getopt() {

    // We always run in daemon-mode.
    $this->daemon = TRUE;

    // No ability to change at this point.
    $this->recover_workers = FALSE;
    $this->debug_workers = FALSE;
    $this->verbose = FALSE;
  }

  /**
   * Uniquely name this daemon to be specific to the drupal site running it.
   *
   * This is used to name the Lock, status & log files. Using get_class
   * ensures that these files have different names based on the Daemon &
   * allows more than 1 to run at once.
   */
  protected function setDaemonName() {

    if (!isset($this->daemon_name)) {

      // Grab the database name of the current drupal site.
      $db = reset($GLOBALS['databases']);
      $db = reset($db);
      $drupal_site = $db['database'];

      // Set the Daemon name to include the sanitized drupal URL and
      // the current daemon class.
      $this->daemon_name = $drupal_site . '-' . get_class($this);
    }

    return $this->daemon_name;
  }
}

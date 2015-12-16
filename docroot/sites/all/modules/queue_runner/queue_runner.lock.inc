<?php

class QueueRunnerFLockException extends Exception {}

class QueueRunnerFLockExistsException extends QueueRunnerFLockException {}

/**
 * Utility class for managing locks using flock().
 */
class QueueRunnerFLock {

  // Typically the name of the script or function creating the lock.
  protected $lock_name;
  protected $filename;
  // Directory where PID files will be stored.
  protected $lock_dir;
  protected $fp;

  protected function __construct($lock_name, $lock_dir) {
    $this->lock_name = preg_replace('/[^A-Za-z0-9_\.\-]/', '_', $lock_name);
    $this->lock_dir = $lock_dir;
    $this->clear();
  }

  protected function clear() {
    $this->filename = '';
    $this->fp = FALSE;
  }

  /**
   * Create the lock or throw an exception; does not block.
   *
   * On success, writes the current process id into the file.
   */
  protected function lock() {
    if ($this->fp !== FALSE) {
      throw new QueueRunnerFLockException("lock is not re-lockable");
    }
    $this->filename = $this->lock_dir . '/' . $this->lock_name . '.lock';
    // Open the file for writing but do NOT truncate it since we might read it.
    $this->fp = @fopen($this->filename, 'a');
    if ($this->fp === FALSE) {
      throw new QueueRunnerFLockException("cannot open lock file {$this->filename}");
    }
    $lock = @flock($this->fp, LOCK_EX|LOCK_NB);
    if ($lock === FALSE) {
      fclose($this->fp);
      $pid = @file_get_contents($this->filename);
      $msg = "{$this->filename} is locked by pid $pid";
      $this->clear();
      throw new QueueRunnerFLockExistsException($msg);
    }
    if (ftruncate($this->fp, 0) === FALSE || fwrite($this->fp, getmypid()) === FALSE) {
      fclose($this->fp);
      $msg = "cannot truncate or write to {$this->filename}";
      $this->clear();
      throw new QueueRunnerFLockException($msg);
    }
  }

  /**
   * Remove the lock.
   */
  function release() {
    if ($this->fp !== FALSE) {
      // PHP 5.3.2 >= requires you to unlock it with flock.
      flock($this->fp, LOCK_UN);
      $result = fclose($this->fp);
      if ($result === FALSE) {
        throw new QueueRunnerFLockException("Could not unlock {$this->filename}");
      }
      // Do not unlink the file! Doing so allows a race condition. See
      // http://world.std.com/~swmcd/steven/tech/flock.html for an explanation.
    }
    $this->clear();
  }

  /**
   * Retrieve the file descriptor holding the lock.
   */
  public function fd() {
    // This is a hack, but PHP does not seem to provide a better way.
    $pid = getmypid();
    $data = explode("\n", shell_exec("lsof -a -p $pid -F f {$this->filename}"));
    return substr($data[1], 1);
  }

  /**
   * Create an exclusive system-wide lock for a given script name.
   *
   * Retry at random intervals a specified number of seconds, then fail.
   *
   * Call $lock->release() to release the lock, or just exit the process.
   *
   * @param string $lock_name
   *   A name for our lock, such as a script name.
   * @param string $lock_dir
   *   The directory to lock.
   * @param integer $timeout
   *   The maximum number of seconds to wait for the lock. 0 means to try only
   *   once.
   *
   * @return
   *   The lock object on which to call release().
   * @throws
   *   QueueRunnerFLockException() if the lock is currently held or can't be created.
   */
  public static function create($lock_name, $lock_dir = '/tmp', $timeout = 0) {
    $lock = new QueueRunnerFLock($lock_name, $lock_dir);
    $timeout_time = microtime(TRUE) + $timeout;
    while (TRUE) {
      try {
        $lock->lock();
        return $lock;
      }
      catch (QueueRunnerFLockExistsException $e) {
        if (function_exists('drush_print')) {
          drush_print($e->getMessage());
        }
        if (empty($timeout) || (microtime(TRUE) > $timeout_time)) {
          throw $e;
        }
        else {
          // Sleep between .01s and 1s.
          usleep(rand(10000, 1000000));
        }
      }
    }
  }
}


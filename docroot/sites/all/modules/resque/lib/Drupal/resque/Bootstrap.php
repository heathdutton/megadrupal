<?php
/**
 * @file
 * Contains Drupal\resque\Bootstrap.
 */

namespace Drupal\resque;

use Resque_Job;

class Bootstrap {
  /**
   * Bootstraps drupal after child gets forked and is ready to perform.
   *
   * @param Resque_Job $job
   *   The job that is ready to be performed.
   */
  public static function afterFork(Resque_Job $job) {
    require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
    $_SERVER['REMOTE_ADDR'] = '127.0.0.1';
    $_SERVER['REQUEST_METHOD'] = 'GET';
    $_SERVER['HTTP_USER_AGENT'] = 'RESQUE_WORKER';
    chdir(DRUPAL_ROOT);
    drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  }
}

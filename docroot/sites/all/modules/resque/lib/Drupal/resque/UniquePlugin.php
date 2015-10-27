<?php
/**
 * @file
 * Contains Drupal\resque\UniquePlugin.
 */

namespace Drupal\resque;

use Resque as Php_Resque;
use Resque_Job;

/**
 * Using event hooks defined in the php-resque library.
 */
class UniquePlugin {
  /**
   * Check the job has a unique instance.
   *
   * @param string $class
   *   String containing the name of scheduled job.
   * @param array $arguments
   *   Array of arguments supplied to the job.
   * @param string $queue
   *   String containing the name of the queue the job was added to.
   *
   * @return bool
   *   Queue up the job if another with the same key doesn't exist.
   */
  public static function beforeEnqueue($class, array $arguments, $queue) {
    if (Php_Resque::redis()->exists($arguments['drupal_unique_key'])) {
      return FALSE;
    }

    Php_Resque::redis()->set($arguments['drupal_unique_key'], '1');

    return TRUE;
  }

  /**
   * Clear the unique queue after the job has performed.
   *
   * @param Resque_Job $job
   *   The job that failed.
   */
  public static function afterPerform(Resque_Job $job) {
    if (Php_Resque::redis()->exists($job->payload['args'][0]['drupal_unique_key'])) {
      Php_Resque::redis()->del($job->payload['args'][0]['drupal_unique_key']);
    }
  }

  /**
   * Clear the unique queue after the job has failed.
   *
   * @param object $exception
   *   Exception that occurred.
   * @param Resque_Job $job
   *   The job that failed.
   */
  public static function onFailure($exception, Resque_Job $job) {
    if (Php_Resque::redis()->exists($job->payload['args'][0]['drupal_unique_key'])) {
      Php_Resque::redis()->del($job->payload['args'][0]['drupal_unique_key']);
    }
  }
}

<?php
/**
 * @file
 * Contains Drupal\resque\Job.
 */

namespace Drupal\resque;

/**
 * Abstract class that implements Resque interface.
 */
abstract class Job implements JobInterface {
  /**
   * {@inheritdoc}
   */
  public function setup() {
    // Do nothing.
  }
}

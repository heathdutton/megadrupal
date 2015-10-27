<?php
/**
 * @file
 * Contains Drupal\resque\Base.
 */

namespace Drupal\resque;

class Base extends Job {
  /**
   * {@inheritdoc}
   */
  public function perform() {
    $function = $this->args['worker_callback'];
    unset($this->args['worker_callback']);
    $function($this->args);
  }
}

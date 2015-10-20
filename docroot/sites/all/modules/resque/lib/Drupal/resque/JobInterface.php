<?php
/**
 * @file
 * Contains Drupal\resque\JobInterface.
 */

namespace Drupal\resque;

/**
 * Implement this interface to use your own classes to run your jobs.
 *
 * Modeled after @link https://github.com/chrisboulton/php-resque @endlink
 * and @link https://github.com/resque/resque @endlink.
 */
interface JobInterface {
  /**
   * This will always run before the perform method.
   */
  public function setUp();

  /**
   * This is the method where you can use your jobs arguments to perform tasks.
   */
  public function perform();
}

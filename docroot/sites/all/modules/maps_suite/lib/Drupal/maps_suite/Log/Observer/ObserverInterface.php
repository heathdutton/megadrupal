<?php

/**
 * @file
 * Defines the MaPS Suite Log Observer interface.
 */

namespace Drupal\maps_suite\Log\Observer;

use Drupal\maps_suite\Log;

/**
 * MaPS Suite Log Observer interface.
 */
interface ObserverInterface {

  /**
   * Dispatch the log information on any desired service, as email or database log...
   */
  public function dispatchLog(Log\LogInterface $log);

  /**
   * Dispatch log information, assuming the log related operations are not entirely
   * performed.
   *
   * @see ObserverInterface::dispatchLog()
   */
  public function dispatchPartialLog(Log\LogInterface $log);

}

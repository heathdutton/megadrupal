<?php

/**
 * @file
 * Defines the MaPS Suite Log Observer Watchdog class.
 */

namespace Drupal\maps_suite\Log\Observer;

use Drupal\maps_suite\Log;

/**
 * MaPS Suite Log Overver Watchdog class.
 *
 * This class allow to dispatch log messages through Drupal watchdog system.
 */
class Watchdog implements ObserverInterface {

  /**
   * @inheritdoc
   */
  public function dispatchLog(Log\LogInterface $log) {
    watchdog('maps_suite', $log->getIntroductionMessage(), array(), WATCHDOG_DEBUG, $log->getUrl());
  }

  /**
   * @inheritdoc
   */
  public function dispatchPartialLog(Log\LogInterface $log) {
  	if (variable_get('maps_suite_log_oberver_watchdog_partial', FALSE)) {
      watchdog('maps_suite', '!message<hr />Percentage of completion: @percentage %', array('!message' => $log->getIntroductionMessage(), '@percentage' => number_format(100 * $log->getPercentage(), 0)), WATCHDOG_DEBUG, $log->getPath());
    }
  }

}

<?php

/**
 * @file
 * Defines the MaPS Suite Log related classes and interfaces.
 */

namespace Drupal\maps_suite\Log\Observer;

use Drupal\maps_suite\Log;

/**
 * MaPS Suite Log Overver Mail class.
 *
 * This class allow to dispatch log messages through Drupal mail system.
 */
abstract class Mail implements ObserverInterface {

  /**
   * @inheritdoc
   */
  final public function dispatchPartialLog(Log\LogInterface $log) {
    // Do not spam the Webmaster mail box, since partial logs number may be very
    // high, especially on mapping operations.
    return FALSE;
  }

}

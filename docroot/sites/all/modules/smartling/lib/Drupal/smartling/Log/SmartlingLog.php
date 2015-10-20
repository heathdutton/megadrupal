<?php

/**
 * @file
 * Smartling log.
 */

namespace Drupal\smartling\Log;

/**
 * Class SmartlingLog.
 */
class SmartlingLog extends DevNullLogger {

  public $settings;

  public function __construct($settings) {
    $this->settings = $settings;
  }

  public function log($level, $message, array $context = array(), $ignore_settings = FALSE) {
    if (!$ignore_settings && !$this->settings->getLogMode()) {
      return FALSE;
    }

    $link = '';
    if (isset($context['entity_link'])) {
      $link = $context['entity_link'];
      unset($context['entity_link']);
    }

    watchdog('smartling', $message, $context, $level, $link);
    return TRUE;
  }

}

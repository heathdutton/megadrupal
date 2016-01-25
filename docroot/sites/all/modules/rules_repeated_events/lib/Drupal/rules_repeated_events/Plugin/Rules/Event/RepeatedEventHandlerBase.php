<?php

/**
 * @file
 * Contains TwitterEventHandlerBase.
 */

namespace Drupal\rules_repeated_events\Plugin\Rules\Event;

use Drupal\fluxservice\Rules\EventHandler\CronEventHandlerBase;
use Drupal\rules_repeated_events\Rules\RulesPluginHandlerBase;

/**
 * Cron-based base class for Contextio event handlers.
 */
abstract class RepeatedEventHandlerBase extends CronEventHandlerBase {

  /**
   * Returns info-defaults for contextio plugin handlers.
   */
  public static function getInfoDefaults() {
    return RulesPluginHandlerBase::getInfoDefaults();
  }

  /**
   * Rules contextio integration access callback.
   */
  public static function integrationAccess($type, $name) {
    return fluxservice_access_by_plugin('rules_repeated_events');
  }

  /**
   * {@inheritdoc}
   */
  public function getEventNameSuffix() {
    return drupal_hash_base64(serialize($this->getSettings()));
  }

}

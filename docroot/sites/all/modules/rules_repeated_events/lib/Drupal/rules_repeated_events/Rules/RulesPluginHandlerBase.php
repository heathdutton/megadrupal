<?php

/**
 * @file
 * Contains RulesPluginHandlerBase.
 */

namespace Drupal\rules_repeated_events\Rules;

use Drupal\fluxservice\Rules\FluxRulesPluginHandlerBase;

/**
 * Base class for contextio Rules plugin handler.
 */
abstract class RulesPluginHandlerBase extends FluxRulesPluginHandlerBase {

  /**
   * Returns info-defaults for contextio plugin handlers.
   */
  public static function getInfoDefaults() {
    return array(
      'category' => 'repeated_events',
      'access callback' => array(get_called_class(), 'integrationAccess'),
    );
  }

  /**
   * Rules plugin access callback.
   */
  public static function integrationAccess($type, $name) {
    return fluxservice_access_by_plugin('rules_repeated_events');
  }
}

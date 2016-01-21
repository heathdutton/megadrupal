<?php

/**
 * @file
 * Contains RulesPluginHandlerBase.
 */

namespace Drupal\fluxcontextio\Rules;

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
      'category' => 'fluxcontextio',
      'access callback' => array(get_called_class(), 'integrationAccess'),
    );
  }

  /**
   * Rules contextio integration access callback.
   */
  public static function integrationAccess($type, $name) {
    return fluxservice_access_by_plugin('fluxcontextio');
  }

  /**
   * Returns info suiting for contextio service account parameters.
   */
  public static function getAccountParameterInfo() {
    return array(
      'type' => 'fluxservice_account',
      'bundle' => 'fluxcontextio',
      'label' => t('Context.io account'),
      'description' => t('The Context.io account under which this shall be executed.'),
    );
  }
}

<?php
/**
 * @file
 * Provide an accordion style plugin for Views.
 * This file is autoloaded by Views.
 */

/**
 * Implements hook_views_plugin().
 */
function views_universal_accordion_views_plugins() {
  return array(
    'style' => array(
      'views_universal_accordion' => array(
        'title' => t('Accordion'),
        'theme' => 'views_view_universal_accordion',
        'help' => t('Display an accordion with the results. Non-image fields will be used as the header and trigger.'),
        'handler' => 'ViewsUniversalAccordionStylePlugin',
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses options' => TRUE,
        'uses fields' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}

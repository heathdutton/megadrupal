<?php

/**
 * Implements hook_views_plugins().
 */
function views_expost_views_plugins() {
  return array(
    'exposed_form' => array(
      'post_exposed_filters' => array(
        'title' => t('ExPost Form (Extends BEF if available)'),
        'help' => t('Allow exposed filters forms in views to use the method POST instead of GET'),
        'handler' => 'views_expost_exposed_form_plugin',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'parent' => 'basic',
      ),
    ),
  );
}

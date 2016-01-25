<?php

/**
 * Implements hook_views_plugins().
 */
function views_exposed_autohide_views_plugins() {
  return array(
    'exposed_form' => array(
      'views_exposed_autohide' => array(
        'title' => t('Autohide exposed filters'),
        'help' => t('Hide exposed filters if the view result count is low'),
        'handler' => 'views_exposed_autohide_plugin_exposed_form_autohide',
        'uses options' => TRUE,
        'parent' => 'basic',
        'help topic' => 'exposed-form-basic',
      ),
    ),
  );
}

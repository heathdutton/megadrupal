<?php

/**
 * Implements hook_views_plugins().
 */
function references_tabs_views_plugins() {
  return array(
    'style' => array(
      'references_tabs_tabs' => array(
        'title' => t('Tabs'),
        'help' => t('Display the nodes in tabs.'),
        'handler' => 'references_tabs_views_handler',
        'base' => array('node'),
        'uses row plugin' => TRUE,
        'uses grouping' => FALSE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}


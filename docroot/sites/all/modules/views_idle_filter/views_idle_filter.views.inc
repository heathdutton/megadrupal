<?php

/**
 * @file
 * Views Idle Filter views integration.
 */

/**
 * Implements hook_views_data().
 */
function views_idle_filter_views_data() {
  return array(
    'views' => array(
      'idle_filter' => array(
        'title' => t('Show filter label'),
        'help' => t('Shows filter label only, does not filter anything.'),
        'filter' => array(
          'handler' => 'views_idle_filter_handler',
        ),
      ),
    ),
  );
}

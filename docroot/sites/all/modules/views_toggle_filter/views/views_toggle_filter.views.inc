<?php

/**
 * @file
 * Views Toggle Filter views integration.
 */

/**
 * Implements hook_views_data().
 */
function views_toggle_filter_views_data() {
  return array(
    'views' => array(
      'toggle_filter' => array(
        'title' => t('Toggle filter'),
        'help' => t('Shows or hides other exposed filters.'),
        'filter' => array(
          'handler' => 'views_toggle_filter_handler',
        ),
      ),
    ),
  );
}

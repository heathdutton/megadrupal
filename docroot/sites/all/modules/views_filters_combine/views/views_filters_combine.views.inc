<?php
/**
 * @file
 * Views hooks for the Views filters combine module.
 */

/**
 * Implements hook_views_data().
 */
function views_filters_combine_views_data() {
  $data = array();
  $data['views_filters_combine']['table']['group'] = t('Global');
  $data['views_filters_combine']['table']['join'] = array(
    '#global' => array(),
  );

  $data['views_filters_combine']['filters_combine'] = array(
    'title' => t('Combine filters'),
    'help' => t('Use other filters with the value of this filter.'),
    'filter' => array(
      'handler' => 'views_filters_combine_handler_filter',
    ),
  );

  return $data;
}

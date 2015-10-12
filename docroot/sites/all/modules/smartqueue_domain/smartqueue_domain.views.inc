<?php

/**
 * Implements hook_views_data().
 */
function smartqueue_domain_views_data() {
  $data = array();
  $data['nodequeue_nodes']['in_domain'] = array(
    'title' => t('In Domain'),
    'help' => t('Filter to ensure a node IS or IS NOT in the current Domain'),
    'filter' => array(
      'handler' => 'smartqueue_domain_handler_filter_in_domain',
      'label' => t('In Domain'),
    ),
  );
  return $data;
}

/**
 * Implements hook_views_pre_view().
 *
 * Fixes filter handler name previously misspelled as 'in_domian'.
 *
 * See https://drupal.org/node/1965786#comment-7285168 (and the whole comment
 * thread generally) for reasoning behind why this has been done this way
 * instead of implementing hook_update_N().
 *
 * @see smartqueue_domain_views_data()
 */
function smartqueue_domain_views_pre_view(&$view) {
  $filters = $view->display_handler->get_option('filters');
  if (isset($filters['in_domian'])) {
    $filters['in_domain'] = $filters['in_domian'];
    $filters['in_domain']['id'] = 'in_domain';
    $filters['in_domain']['field'] = 'in_domain';
    unset($filters['in_domian']);
    $view->display_handler->set_option('filters', $filters);
  }
}

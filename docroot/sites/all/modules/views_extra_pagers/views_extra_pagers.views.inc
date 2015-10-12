<?php

/**
 * Implements hook_views_plugins().
 */
function views_extra_pagers_views_plugins() {
  return array(
    'pager' => array(
      'full_all' => array(
        'title' => t('Paged output, full pager with link to all items'),
        'help' => t('Paged output, full Drupal style with link to all items'),
        'handler' => 'views_extra_pager_full_all',
        'help topic' => 'pager-full',
        'uses options' => TRUE,
        'parent' => 'full',
      ),
      'mini_all' => array(
        'title' => t('Paged output, mini pager with link to all items'),
        'help' => t('Use the mini pager output with link to all items.'),
        'handler' => 'views_extra_pager_mini_all',
        'help topic' => 'pager-mini',
        'uses options' => TRUE,
        'parent' => 'mini',
      ),
    ),
  );
}

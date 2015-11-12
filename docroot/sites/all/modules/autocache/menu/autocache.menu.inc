<?php

/**
 * Implements a menu callback
 */
function autocache_settings_form() {

  $form = array();

  $form['autocache_page_tracking_mode'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Track page paths accurately'), 
    '#default_value' => variable_get('autocache_page_tracking_mode', FALSE), 
    '#description' => t('This allows Autocache to clear page caches as accurately as possible. Use only in smaller sites as the tracking table might get flooded easily.'),
  );

  $form['autocache_auto'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Clear caches automatically'), 
    '#default_value' => variable_get('autocache_auto', TRUE), 
    '#description' => t('This is the beef of the module. By turning this off you will have to use the Rules module to clear the caches with the autocache  flushing action.'),
  );

  $form['autocache_debug'] = array(
    '#type' => 'checkbox', 
    '#title' => t('Show debug information'), 
    '#default_value' => variable_get('autocache_debug', FALSE), 
    '#description' => t('Display what cids are cleared when e.g. a node is updated. Also displays the cids used on the current page. It also shows you time it takes to collect the information required to clear the cache tables and the actual time it takes to clear cache tables.'),
  );

  $form['#submit'][] = 'autocache_settings_form_submit';

  return system_settings_form($form);
}

/**
 * Submit handler
 */
function autocache_settings_form_submit($form, &$form_state) {

  cache_clear_all();

  $varnish = module_exists('varnish');

  // Clear varnish
  if ($varnish) {
    varnish_purge_all_pages();
    drupal_set_message(t('For your convenience the caches in drupal and varnish have been cleared'));
  } else {
    drupal_set_message(t('For your convenience the caches in drupal have been cleared'));
  }
}

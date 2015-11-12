<?php

/**
 * @file
 * Admin form callbacks for the Cache Bypass Path module.
 */

/**
 * Form callback to regenerate the hash url.
 */
function cache_bypass_path_regenerate_hash_form($form, &$form_state) {
  $chash = variable_get('cache_bypass_path_hash');
  $cache_path = url("cbp/{$chash}", array('absolute' => TRUE));
  $form['current_path'] = array(
    '#prefix' => '<div><p>' . t('The current cache bypass path is:') . '</p>',
    '#markup' => l($cache_path, $cache_path),
    '#suffix' => '</div>',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Regenerate'),
  );
  return $form;
}

/**
 * Submit callback for the regnerate hash bypass url form.
 */
function cache_bypass_path_regenerate_hash_form_submit($form, &$form_state) {
  module_load_include('inc', 'cache_bypass_path');
  $chash = cache_bypass_path_reset_hash();
  $cache_path = url("cbp/{$chash}", array('absolute' => TRUE));
  drupal_set_message(t('The cache bypass path has been regnerated. It is now at !path.', array('!path' => l($cache_path, $cache_path))), 'status');
}

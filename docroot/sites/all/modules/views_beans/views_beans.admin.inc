<?php
/**
 * @file
 * Administration pages callbacks.
 */

/**
 * Views Beans Admin form callback.
 */
function views_beans_settings($form, &$form_state) {
  drupal_set_message(t('Changing this form requires clearing all caches.'));
  $options = _views_beans_get_categories(FALSE);

  $form['views_beans_bean_types'] = array(
    '#title' => t('Views Panes Categories for Bean Types'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => variable_get('views_beans_bean_types', array()),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = '_views_beans_clear_caches';
  return $form;
}
/**
 * Submit callback to clear bean caches.
 */
function _views_beans_clear_caches($form, &$form_state) {
  bean_bean_cache_clear();
  cache_clear_all('plugins:bean:types', 'cache');
  variable_set('menu_rebuild_needed', TRUE);
}
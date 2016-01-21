<?php

/**
 * @file
 * Administration
 */

/**
 * SimpleMeta settings form builder.
 */
function simplemeta_settings_form($form, &$form_state) {
  $form['simplemeta_form_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Add Meta Tags Form'),
    '#description' => t('If enabled, form will appear on pages'),
    '#default_value' => variable_get('simplemeta_form_enable', TRUE),
    '#return_value' => TRUE,
  );
  $form['simplemeta_language_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Language Support'),
    '#description' => t('If enabled, language selectbox will appear on the form. Otherwise all created metadata will be language-neutral i.e. global for all languages'),
    '#default_value' => variable_get('simplemeta_language_enable', FALSE),
    '#return_value' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Menu callback; show list of saved SimpleMeta configurations. 
 * 
 * @return string
 *   themed output
 */
function simplemeta_meta_list() {
  $query = db_select('simplemeta', 's')
    ->extend('PagerDefault')
    ->fields('s')
    ->orderBy('s.sid', 'DESC')->limit(20);
  $result = $query->execute();

  $items = array();
  while ($meta = $result->fetchObject()) {
    $meta->data = unserialize($meta->data);
    $items[] = $meta;
  }

  return theme('simplemeta_meta_list', array('items' => $items)) . theme('pager');
}
/**
 * Menu callback for creating new SimpleMeta configurations.
 * 
 * @return array
 *   form structure to render
 */
function simplemeta_add() {
  $meta = new stdClass();
  $meta->data = array();
  $meta->language = '';
  return drupal_get_form('simplemeta_meta_form', $meta);
}

/**
 * SimpleMeta deletion confirmation form builder.
 */
function simplemeta_meta_delete_confirm($form, &$form_state, $meta) {
  $form['sid'] = array(
    '#type' => 'value',
    '#value' => $meta->sid,
  );

  return confirm_form($form,
    t('Are you sure you want to delete Simplemeta %sid?', array('%sid' => $meta->sid)),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/content/simplemeta/' . $meta->sid . '/edit',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * SimpleMeta deletion confirmation form submit callback.
 */
function simplemeta_meta_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    simplemeta_meta_delete($form_state['values']['sid']);
    cache_clear_all('*', 'cache_simplemeta', TRUE);
  }
  $form_state['redirect'] = 'admin/content/simplemeta/list';
}

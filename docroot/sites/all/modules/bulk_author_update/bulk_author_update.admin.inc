<?php
/**
 * @file
 * Bulk author update configuration form.
 */

/**
 * Bulk author update configurations settings form.
 */
function bulk_author_update_admin_config_form() {
  // Set Title.
  drupal_set_title(t('Bulk Author Update'));

  $form  = array();
  $form['bulk_author_update_display_records'] = array(
    '#type' => 'textfield',
    '#title' => 'Records per page',
    '#description' => t('In dashboard number of records visible per page'),
    '#default_value' => variable_get('bulk_author_update_display_records', 50),
  );
  $form['note'] = array(
    '#type' => 'item',
    '#markup' => "<b>" . t('Choose content types to update the author') . "<b><br/>",
  );
  $ctypes = node_type_get_types();
  if (count($ctypes) > 0) {
    foreach ($ctypes as $ct) {
      $content_type_names = $ct->name;
      $content_type = $ct->type;
      $form['bulk_author_update_' . $content_type] = array(
        '#type' => 'checkbox',
        '#title' => check_plain($content_type_names),
        '#default_value' => variable_get('bulk_author_update_' . $content_type),
      );
    }
  }
  return system_settings_form($form);
}

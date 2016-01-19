<?php
/**
 * @file
 * Pages for taxonomy nested_intervals settings and more.
 */

/**
 * Form build for the settings form.
 *
 * @see taxonomy_nested_intervals_rebuild_submit()
 * @ingroup forms
 */
function taxonomy_nested_intervals_settings_form() {
  $form = array();

  $form['taxonomy_nested_intervals_rebuild_safe_sibling'] = array(
    '#title' => t('Safe sibling rebuild'),
    '#description' => t('Rebuild tree with safe sibling enumeration (slower, but more precise).'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('taxonomy_nested_intervals_rebuild_safe_sibling', TAXONOMY_NESTED_INTERVALS_REBUILD_SAFE_SIBLING),
  );
  $form['taxonomy_nested_intervals_override_term_pages'] = array(
    '#title' => t('Override term pages'),
    '#description' => t('Override taxonomy/term/%term pages (and feed pages also) supporting the depth modifier argument.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('taxonomy_nested_intervals_override_term_pages', TAXONOMY_NESTED_INTERVALS_OVERRIDE_TERM_PAGES),
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#description' => t('This section contains VERY advanced settings.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['taxonomy_nested_intervals_decimals'] = array(
    '#title' => t('Number of decimals for numeric type.'),
    '#description' => t('Number of decimals for numeric type. Remember to update your DB schema manually!'),
    '#type' => 'textfield',
    '#default_value' => variable_get('taxonomy_nested_intervals_decimals', TAXONOMY_NESTED_INTERVALS_DECIMALS),
  );
  $form['advanced']['taxonomy_nested_intervals_delete_chunksize'] = array(
    '#title' => t('Size of delete chunks.'),
    '#description' => t('Number of rows to delete at a time, when clearing out tables. 0 = clear all in one query.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('taxonomy_nested_intervals_delete_chunksize', TAXONOMY_NESTED_INTERVALS_DELETE_CHUNKSIZE),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'taxonomy_nested_intervals_system_settings_form_submit_custom';
  return $form;
}

/**
 * Submit handler for rebuild menu after saving.
 *
 * Changing the override term pages setting requires rebuild of menu.
 * Since we have no proper atomic way of detecting whether or not the
 * setting has actually changed (theoretical race-condition), we always
 * rebuild when saving these settings.
 */
function taxonomy_nested_intervals_system_settings_form_submit_custom($form, &$form_state) {
  // If a rebuild is already in progress, chances are that our new settings
  // is not part of the rebuild. We therefore wait for the rebuild to finish
  // and then rebuild again.
  if (!lock_acquire('menu_rebuild')) {
    lock_wait('menu_rebuild');
    if (!lock_acquire('menu_rebuild')) {
      drupal_set_message(t("Could not rebuild menu. Please rebuild menu / clear cache to make the term pages override take effect."), 'error');
      return;
    }
  }
  menu_rebuild();
}

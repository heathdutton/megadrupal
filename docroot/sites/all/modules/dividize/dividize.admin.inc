<?php

/**
 * @file
 * Module administration pages.
 */

/**
 * Returns with the general configuration form.
 */
function dividize_admin_settings($form, &$form_state) {
  $form['dividize_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Apply dividize to the following elements'),
    '#default_value' => variable_get('dividize_selector', 'table'),
    '#description' => t('A comma-separated list of jQuery selectors to apply dividize. Defaults to select to apply dividize to all <code>&lt;table&gt;</code> elements.'),
  );

  $form['dividize_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Window width breakpoint'),
    '#default_value' => variable_get('dividize_width', 768),
    '#description' => t("Dividize will be enabled only if the width of user's Web browser window is smaller than the below value."),
    '#maxlength' => 4,
    '#size' => 4,
    '#field_suffix' => 'px',
  );

  $form['dividize_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Apply the provided CSS'),
    '#default_value' => variable_get('dividize_css', TRUE),
    '#description' => t('Simple CSS to display div as a table.'),
  );

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dividize settings'),
  );

  $form['settings']['dividize_classes'] = array(
    '#type' => 'textfield',
    '#title' => t('Classes'),
    '#default_value' => variable_get('dividize_classes', 'dividize-processed'),
    '#description' => t('Add any extra classes to converted div.'),
  );

  $form['settings']['dividize_remove_headers'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove headers'),
    '#default_value' => variable_get('dividize_remove_headers', FALSE),
    '#description' => t('Remove/leave headers div.'),
  );

  $form['settings']['dividize_add_label_headers'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add label headers'),
    '#default_value' => variable_get('dividize_add_label_headers', TRUE),
    '#description' => t('Add headers to each cell.'),
  );

  $form['settings']['dividize_hide_labels'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide labels'),
    '#default_value' => variable_get('dividize_hide_labels', TRUE),
    '#description' => t('Hide the labelHeader by default.'),
  );

  $form['settings']['dividize_preserve_events'] = array(
    '#type' => 'checkbox',
    '#title' => t('Preserve events'),
    '#default_value' => variable_get('dividize_preserve_events', TRUE),
    '#description' => t('Save events cell content and restore after conversion.'),
  );

  $form['settings']['dividize_preserve_dim'] = array(
    '#type' => 'checkbox',
    '#title' => t('Preserve dim'),
    '#default_value' => variable_get('dividize_preserve_dim', TRUE),
    '#description' => t('Try to keep cell widths & heights.'),
  );

  $form['settings']['dividize_enable_alt_rows'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable alt rows'),
    '#default_value' => variable_get('dividize_enable_alt_rows', TRUE),
    '#description' => t('Enable alternating rows.'),
  );

  return system_settings_form($form);
}

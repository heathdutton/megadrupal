<?php
/**
 * @file
 * Form builder. Configure webform pager.
 *
 * @ingroup forms
 * @see system_settings_form()
 */

/**
 * Creates settings form for webform_pager.
 */
function webform_pager_admin_settings() {
  // Show page numbers in multi-page webforms.
  $form['webform_pager_show_page_numbers'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show page count'),
    '#description' => t('Show current page and total amount of pages on enabled webforms.'),
    '#options' => array(
      0 => 0,
      1 => 1,
    ),
    '#required' => FALSE,
    '#default_value' => variable_get('webform_pager_show_page_numbers', 1),
  );

  // Show percentages on multi-page webforms.
  $form['webform_pager_show_percentages'] = array(
    '#type' => 'checkbox',
    '#title' => t('Percentages'),
    '#description' => t('Show percentages on webforms with pager.'),
    '#options' => array(
      0 => 0,
      1 => 1,
    ),
    '#default_value' => variable_get('webform_pager_show_percentages', 0),
  );

  // Show page slide on multi-page webforms.
  $form['webform_pager_show_page_names'] = array(
    '#type' => 'checkbox',
    '#title' => t('Page names'),
    '#description' => t('Show page names with pager.'),
    '#options' => array(
      0 => 0,
      1 => 1,
    ),
    '#default_value' => variable_get('webform_pager_show_page_names', 0),
  );

  return system_settings_form($form);
}

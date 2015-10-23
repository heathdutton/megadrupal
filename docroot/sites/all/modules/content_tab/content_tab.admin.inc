<?php

/**
 * @file
 * Administration page callbacks for the content tab module.
 */

/**
 * Form builder. Configure annotations.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function content_tab_admin_settings() {

  $pager_options = drupal_map_assoc(range(5, 50, 5));

  $content_tab_page_column_options = array(
    'created' => t('Created'),
    'changed' => t('Changed'),
    'published' => t('Published'),
    'sticky' => t('Sticky'),
    'promoted' => t('Promoted to front page'),
    'operations' => t('Operations'),
  );

  $form['content_tab']['content_tab_weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#description' => t('Select the weight to place the content tab on the /user page.'),
    '#default_value' => variable_get('content_tab_weight', -5),
  );
  $form['content_tab']['content_tab_page_columns'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Table columns'),
    '#description' => t('Select the columns, you wish to display.'),
    '#options' => $content_tab_page_column_options,
    '#default_value' => variable_get('content_tab_page_columns',
      array_keys($content_tab_page_column_options)),
  );
  $form['content_tab']['content_tab_pager'] = array(
    '#type' => 'select',
    '#title' => t('Pager settings'),
    '#description' => t('Enter the number of rows to be displayed on the page.'),
    '#options' => $pager_options,
    '#default_value' => variable_get('content_tab_pager', 10),
  );
  return system_settings_form($form);
}

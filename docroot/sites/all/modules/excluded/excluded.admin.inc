<?php
/**
 * @file
 * Excluded Module Administrative screen.
 */

/**
 * Excluded settings form
 */
function excluded_settings() {
  $form = array();

  $form['excluded_list'] = array(
    '#type' => 'container',
    '#attributes' => array(
        'class' => array(
          'excluded-list',
        ),
      ), 
    );
  $form['excluded_list']['excluded_javascript_custom']  = array(
    '#type' => 'textarea',
    '#title' => t('JavaScript'),
    '#description' => t('Enter the full path of the file you would like to exclude'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#default_value' => variable_get('excluded_javascript_custom', ''),
  );
  $form['excluded_list']['excluded_css_custom']  = array(
    '#type' => 'textarea',
    '#title' => t('CSS'),
    '#description' => t('Enter the full path of the file you would like to exclude'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#default_value' => variable_get('excluded_css_custom', ''),
  );

  return system_settings_form($form);
}
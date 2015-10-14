<?php

/**
 * @file
 * Administration function
 */

/**
 * Form callback: administration form
 */
function ckeditor_tabber_admin_form() {
  $form = array();
  $form['ckeditor_tabber_semantic_tabs_elements'] = array(
    '#type'          => 'textfield',
    '#title'         => t('jQuery selector for elements to display as tabs'),
    '#description'   => t('Enter a valid jQuery selector like <em>dl, dl.ckeditor-tabber, dl.my-class</em>.'),
    '#default_value' => variable_get('ckeditor_tabber_semantic_tabs_elements', 'dl.ckeditor-tabber')
  );
  return system_settings_form($form);
}

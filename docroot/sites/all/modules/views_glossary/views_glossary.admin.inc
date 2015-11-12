<?php

/**
 * @file
 * In this file we will create admin form of module.
 */

/**
 * This function we will create admin form.
 */
function views_glossary_admin_settings() {
  $form['views_glossary_name'] = array(
    '#title' => t('Views Name'),
    '#description' =>
    t('Enter Views Machine Name that you want to apply views glossary layout Separated by comma.'),
    '#type' => 'textarea',
    '#default_value' => variable_get('views_glossary_name'),
  );
  $active = array(0 => t('Selectbox'), 1 => t('Textbox'));
  $form['views_glossary_go_to_page'] = array(
    '#title' => t('Choose Box'),
    '#description' => t('Choose Selectbox or Textbox for GO TO PER PAGE.'),
    '#type' => 'radios',
    '#default_value' => variable_get('views_glossary_go_to_page', 0),
    '#options' => $active,
  );
  return system_settings_form($form);
}

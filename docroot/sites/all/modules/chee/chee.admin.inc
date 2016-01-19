<?php
/**
 * @file
 * CHEE admin file.
 */

/**
 * Settings form.
 */
function chee_settings_form($form, &$form_state) {
  $form = array();

  $options = chee_list_themes();

  $form['code-highlight'] = array(
    '#type' => 'fieldset',
    '#title' => t('Code highlight theme'),
  );

  $form['code-highlight']['chee_front_theme'] = array(
    '#type' => 'select',
    '#title' => t('Front-end'),
    '#options' => $options,
    '#default_value' => variable_get('chee_front_theme', 'default'),
  );

  $form['code-highlight']['chee_back_theme'] = array(
    '#type' => 'select',
    '#title' => t('Back-end'),
    '#options' => $options,
    '#default_value' => variable_get('chee-back-theme', 'tomorrow-night-eighties'),
  );

  $form['chee_linebreaks'] = array(
    '#type' => 'checkbox',
    '#title' => t('Line Breaks'),
    '#description' => t('If your code container relies on `&lt;br&gt;` tags instead of line breaks check this option to make Chee use `&lt;br&gt;` in the output'),
    '#default_value' => variable_get('chee-linebreaks', TRUE),
  );

  $form['chee_tab_replace'] = array(
    '#type' => 'textfield',
    '#title' => t('Replace TAB characters'),
    '#description' => t("You can replace TAB ('x09') characters used for indentation in your code with some fixed number of spaces or with a `&lt;span&gt;` to give them special styling"),
    '#default_value' => variable_get('chee_tab_replace', '  '),
  );

  return system_settings_form($form);
}

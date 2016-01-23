<?php
/**
 * @file
 * Define admin settings form
 */

/**
 * Defines the title case settings form.
 */
function title_case_settings_form($form, &$form_state) {
  $form['title_case_selectors'] = array(
    '#type' => 'textarea',
    '#title' => t('Selectors'),
    '#default_value' => variable_get('title_case_selectors', ''),
    '#description' => t("Enter the JQuery selectors, which string content will be converted to Title Case (<em>one per line</em>)."),
  );

  $form['title_case_exceptions'] = array(
    '#type' => 'textarea',
    '#title' => t('Exceptions'),
    '#default_value' => variable_get('title_case_exceptions', ''),
    '#description' => t("Enter string exceptions to the title case conversion (<em>one per line and no spaces</em>)."),
  );

  return system_settings_form($form);
}

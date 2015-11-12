<?php

/**
 * @file
 * Plugin to provide an relationship from month number to month name.
 */

$plugin = array(
  'title' => t('Month name from month number'),
  'keyword' => 'month_name',
  'description' => t('Creates month name from month number as a string context.'),
  'required context' => new ctools_context_required(t('Month number'), 'string'),
  'context' => 'months_month_from_number_context',
  'edit form' => 'months_month_from_number_settings_form',
  'defaults' => array(
    'case' => 'nominative',
    'custom_case' => '',
    'capitalize' => 'none',
  ),
);

function months_month_from_number_get_months() {
  return array('', 'January', 'February', 'March', 'April', 'May', 'June', 'July', 'August', 'September', 'October', 'November', 'December');
}

/**
 * Return a new context based on an existing context.
 */
function months_month_from_number_context($context, $conf) {
  if (empty($context->data)) {
    return ctools_context_create_empty('string', NULL);
  }

  $month_number = intval(trim($context->data));
  if (empty($month_number) || ($month_number > 12)) {
    return ctools_context_create_empty('string', NULL);
  }

  $case = $conf['case'];
  $custom_case = trim($conf['custom_case']);
  if (!empty($custom_case)) {
    $case = drupal_strtolower($custom_case);
  }

  $months = months_month_from_number_get_months();
  $month_name = $months[$month_number];

  global $language;
  $langcode = isset($language->language) ? $language->language : 'en';

  if ($langcode === 'en' && $case === 'possessive') {
    $month_name = $month_name . "'s";
  }

  $month_name = t($month_name, array(), array('context' => 'Month case: ' . $case));

  $capitalize = $conf['capitalize'];

  if ($capitalize === 'first') {
    $month_name = drupal_ucfirst($month_name);
  }
  elseif ($capitalize === 'whole') {
    $month_name = drupal_strtoupper($month_name);
  }

  return ctools_context_create('string', $month_name);
}

/**
 * Settings form for the relationship.
 */
function months_month_from_number_settings_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $cases = array(
    'nominative' => t('Nominative case'),
    'possessive' => t('Possessive case'),
    'subjective' => t('Subjective case'),
  );

  $caps = array(
    'none' => t('None'),
    'first' => t('First letter'),
    'whole' => t('Whole word'),
  );

  $form['case'] = array(
    '#title' => t('Grammar case'),
    '#type' => 'select',
    '#options' => $cases,
    '#default_value' => $conf['case'],
    '#description' => t('Select the grammar case for the month name or set the other case below.'),
  );

  $form['custom_case'] = array(
    '#title' => t('Other grammar case'),
    '#type' => 'textfield',
    '#default_value' => $conf['custom_case'],
    '#description' => t('Set the grammar case for the month name if the required case is not in the selection list. Use ENGLISH word as the case name.'),
  );

  $form['capitalize'] = array(
    '#title' => t('Capitalization'),
    '#type' => 'select',
    '#options' => $caps,
    '#default_value' => $conf['capitalize'],
  );

  $form['help'] = array(
    '#type' => 'item',
    '#title' => t('Translation help'),
    '#markup' => t('Translations of the month names use context "Month case: selected_case".'),
  );

  $form['buttons']['return']['#submit'][] = 'months_month_from_number_settings_form_add_translation';

  return $form;
}

function months_month_from_number_settings_form_add_translation(&$form, &$form_state) {
  $case = '';
  if (!empty($form_state['values']['case'])) {
    $case = $form_state['values']['case'];
  }
  if (!empty($form_state['values']['custom_case'])) {
    $case = $form_state['values']['custom_case'];
  }
  if (empty($case)) {
    return;
  }
  foreach (months_month_from_number_get_months() as $month_name) {
    // Add records to locales_source table.
    t($month_name, array(), array('context' => 'Month case: ' . $case));
  }
}

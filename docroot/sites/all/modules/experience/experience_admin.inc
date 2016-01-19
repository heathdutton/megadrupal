<?php

/**
 * @file
 * Experience administration code.
 */

/**
 * Helper function for experience_field_widget_settings_form().
 *
 * @see experience_field_widget_settings_form_validate()
 */
function _experience_field_widget_settings_form($field, $instance) {
  $widget = $instance['widget'];
  $settings = $widget['settings'];

  $form = array(
    '#element_validate' => array('experience_field_widget_settings_form_validate'),
  );

  $form['label_position'] = array(
    '#type' => 'value',
    '#value' => $settings['label_position'],
  );

  $form['include_fresher'] = array(
    '#type' => 'value',
    '#value' => $settings['include_fresher'],
  );

  $form['year_start'] = array(
    '#type' => 'value',
    '#value' => $settings['year_start'],
  );

  $form['year_end'] = array(
    '#type' => 'value',
    '#value' => $settings['year_end'],
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#fieldset' => 'experience_format',
    '#weight' => 9,
  );

  $options = array('above' => t('Above'), 'within' => t('Within'));
  $description = t("The location of experience part labels, like 'Year', 'Month'. 'Above' displays the label as titles above each experience part. 'Within' inserts the label as the first option in the select list.");

  $form['advanced']['label_position'] = array(
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => $settings['label_position'],
    '#title' => t('Position of experience part labels'),
    '#description' => $description,
  );

  $form['advanced']['include_fresher'] = array(
    '#type' => 'checkbox',
    '#default_value' => $settings['include_fresher'],
    '#title' => t('Include fresher option'),
  );

  $form['advanced']['year_start'] = array(
    '#type' => 'select',
    '#options' => range(0, 99),
    '#default_value' => $settings['year_start'],
    '#title' => t('Starting year'),
  );

  $form['advanced']['year_end'] = array(
    '#type' => 'select',
    '#options' => range(0, 99),
    '#default_value' => $settings['year_end'],
    '#title' => t('Ending year'),
  );

  $context = array(
    'field' => $field,
    'instance' => $instance,
  );
  drupal_alter('experience_field_widget_settings_form', $form, $context);

  return $form;
}

/**
 * Form validation handler for _experience_field_widget_settings_form().
 */
function experience_field_widget_settings_form_validate(&$form, &$form_state) {
  // The widget settings are in the wrong place in the form because of #tree on
  // the top level.
  $settings = $form_state['values']['instance']['widget']['settings'];
  $settings = array_merge($settings, $settings['advanced']);
  unset($settings['advanced']);
  form_set_value(array('#parents' => array('instance', 'widget', 'settings')), $settings, $form_state);

  $widget = &$form_state['values']['instance']['widget'];

  if ($widget['settings']['year_start'] > $widget['settings']['year_end']) {
    form_set_error('instance][widget][settings][advanced][year_start', t('The End year must be greater than the Start year.'));
    form_set_error('instance][widget][settings][advanced][year_end');
  }
}

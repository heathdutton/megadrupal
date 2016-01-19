<?php

/**
 * @file
 * The functions declaring the tutorfield field and its widget.
 *
 * For code readability, all the field (and widget) declaring functions are
 * moved here. The exception is tutorfield_field_formatter_view(), containing
 * most of the intersting logic, which is kept in tutorfield.module.
 */

/**
 * Implements hook_field_info().
 */
function tutorfield_field_info() {
  return array(
    'tutorfield' => array(
      'label' => t('Tutor question'),
      'description' => t('Displays a script-generated question to users.'),
      'default_widget' => 'tutorfield_widget',
      'default_formatter' => 'tutorfield_formatter',
    ),
  );
}

/**
 * Implements hook_field_is_empty().
 */
function tutorfield_field_is_empty($item, $field) {
  if ($field['type'] == 'tutorfield') {
    return empty($item['question']);
  }
}

/**
 * Implements hook_field_instance_settings_form().
 */
function tutorfield_field_instance_settings_form($field, $instance) {
  if ($field['type'] == 'tutorfield') {
    $settings = &$instance['settings'];
    $form['delta handling'] = array(
      '#type' => 'radios',
      '#title' => t('Question progression'),
      '#description' => t('Select how multiple-value fields should be handled.'),
      '#options' => array(
        'random smart' => t('Smart random: Select a random question, keep until correctly answered, then randomly pick another question'),
        'random' => t('Random: Select a random question, keep until correctly answered'),
        'progress' => t('Progress: Start from the top, go to next when correctly answered, loop when completed'),
        'progress once' => t('Progress once: Start from the top, go to next when correctly answered, stop at the end'),
        'test' => t('Test: Start from the top, go to next even if answer is incorrect, stop at the end'),
      ),
      '#default_value' => isset($settings['delta handling']) ? $settings['delta handling'] : 'random smart',
    );
    return $form;
  }
}

/**
 * Implements hook_field_widget_info().
 */
function tutorfield_field_widget_info() {
  return array(
    'tutorfield_widget' => array(
      'label' => t('Question selector'),
      'description' => t('Drop-down selection of available scripted questions.'),
      'field types' => array('tutorfield'),
      'multiple values' => FIELD_BEHAVIOR_DEFAULT,
      'default value' => FIELD_BEHAVIOR_DEFAULT,
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function tutorfield_field_widget_form($form, $form_state, $field, $instance, $langcode, $items, $delta, $element) {
  $base = $element;
  // Add a drop-down for selecting a scripted question.
  if ($instance['widget']['type'] == 'tutorfield_widget') {
    $element['question'] = array(
      '#type' => 'select',
      '#options' => tutor_question_labels($instance['required']),
      '#default_value' => isset($items[$delta]['question']) ? $items[$delta]['question'] : NULL,
    ) + $base;
  }
  return $element;
}

/**
 * Implements hook_field_formatter_info().
 */
function tutorfield_field_formatter_info() {
  return array(
    'tutorfield_formatter' => array(
      'label' => t('Question form'),
      'field types' => array('tutorfield'),
    ),
  );
}

<?php

/**
 * @file
 * Work days widget.
 */

/**
 * Implements hook_field_widget_info().
 */
function work_days_field_widget_info() {
  return array(
    'work_days' => array(
      'label' => t('Work days'),
      'field types' => array('work_days'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_NONE,
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function work_days_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  $calendar = array(
    '#type' => 'html_tag',
    '#tag' => 'div',
    '#value' => ' ',
    '#attributes' => array(
      'id' => drupal_html_id('work-days-calendar'),
      'class' => array(
        'work-days-calendar',
      )
    ),
  );

  $element['work_days']['date'] = array(
    '#title' => $instance['label'],
    '#markup' => drupal_render($calendar),
    '#type' => 'markup',
    '#theme_wrappers' => array(
      'form_element'
    ),
  );

  $element['work_days']['additions'] = array(
    '#type' => 'textarea',
    '#attributes' => array(
      'class' => array(
        'work-days-calendar-additions'
      ),
    ),
    '#resizable' => FALSE,
    '#default_value' => !empty($items[$delta]['additions']) ? $items[$delta]['additions'] : '[]',
    '#attached' => array(
      'library' => array(array('system', 'ui')),
      'library' => array(array('system', 'ui.datepicker')),
      'js' => array(
        drupal_get_path('module', 'work_days') . '/misc/jquery-ui.multidatespicker.js',
        drupal_get_path('module', 'work_days') . '/misc/widget.js',
      ),
      'css' => array(
        drupal_get_path('module', 'work_days') . '/misc/widget.css',
      )
    )
  );

  $element['work_days']['exclusions'] = array(
    '#type' => 'textarea',
    '#attributes' => array(
      'class' => array(
        'work-days-calendar-exclusions'
      ),
    ),
    '#resizable' => FALSE,
    '#default_value' => !empty($items[$delta]['exclusions']) ? $items[$delta]['exclusions'] : '[]',
  );

  return $element;
}

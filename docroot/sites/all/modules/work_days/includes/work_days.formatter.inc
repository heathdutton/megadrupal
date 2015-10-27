<?php

/**
 * @file
 * Work days formatter.
 */

/**
 * Implements hook_field_formatter_info().
 */
function work_days_field_formatter_info() {
  return array(
    'work_days_calendar' => array(
      'label' => t('Calendar'),
      'field types' => array('work_days'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function work_days_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  if ($display['type'] == 'work_days_calendar') {
    foreach ($items as $delta => $item) {
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

      $element[$delta]['work_days']['date'] = array(
        '#markup' => drupal_render($calendar),
        '#type' => 'markup',
        '#theme_wrappers' => array(
          'form_element'
        ),
      );

      $element[$delta]['work_days']['additions'] = array(
        '#type' => 'textarea',
        '#attributes' => array(
          'class' => array(
            'work-days-calendar-additions'
          ),
        ),
        '#resizable' => FALSE,
        '#value' => !empty($items[$delta]['additions']) ? $items[$delta]['additions'] : '[]',
        '#attached' => array(
          'library' => array(array('system', 'ui')),
          'library' => array(array('system', 'ui.datepicker')),
          'js' => array(
            drupal_get_path('module', 'work_days') . '/misc/jquery-ui.multidatespicker.js',
            drupal_get_path('module', 'work_days') . '/misc/widget.js',
            array(
              'type' => 'setting',
              'data' => array('workDays' => array('disableSelection' => TRUE)),
            )
          ),
          'css' => array(
            drupal_get_path('module', 'work_days') . '/misc/widget.css',
          )
        )
      );

      $element[$delta]['work_days']['exclusions'] = array(
        '#type' => 'textarea',
        '#attributes' => array(
          'class' => array(
            'work-days-calendar-exclusions'
          ),
        ),
        '#resizable' => FALSE,
        '#value' => !empty($items[$delta]['exclusions']) ? $items[$delta]['exclusions'] : '[]',
      );
    }
  }

  return $element;
}

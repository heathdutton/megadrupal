<?php
/**
 * @file
 * Administration functions for nepali_calendar.module.
 */

/**
 * Implements hook_form().
 */
function nepali_calendar_admin_settings_form($node, &$form_state) {
  global $base_url;

  $form = array();

  $form['overview'] = array(
    '#markup' => t('Converts Gregorian calendar (A.D.) to Bikram Sambat (B.S.) and vice-versa.'),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );

  $form['general'] = array(
    '#type' => 'vertical_tabs',
    '#default_tab' => 'edit-general-nepali-date',
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'nepali_calendar') . '/assets/js/nepali_calendar.admin.js' => array(
          'type' => 'file',
        ),
      ),
    ),
  );

  $form['general_nepali_date'] = array(
    '#group' => 'general',
    '#type' => 'fieldset',
    '#title' => t('Nepali date'),
    '#description' => t('<a href="@block">Nepali date</a> block is created automatically when "Nepali calendar" module is installed.', array('@block' => $base_url . '/admin/structure/block')),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );

  $form['general_nepali_date']['nepali_calendar_nepali_date_format'] = array(
    '#group' => 'general',
    '#type' => 'select',
    '#title' => t('Nepali date format'),
    '#description' => t('Date format that will be displayed in the user screen.'),
    '#options' => nepali_calendar_nepali_date_format_array(),
    '#default_value' => variable_get('nepali_calendar_nepali_date_format', 5),
  );

  $form['general_nepali_date']['nepali_calendar_show_date_label'] = array(
    '#group' => 'general',
    '#type' => 'checkbox',
    '#title' => t('Show date label'),
    '#description' => t('If checked, a date label "<strong>@date_label: </strong>" will be prepended to the date.', array('@date_label' => t('Date'))),
    '#default_value' => variable_get('nepali_calendar_show_date_label', 1),
  );

  $form['general_nepal_time'] = array(
    '#group' => 'general',
    '#type' => 'fieldset',
    '#title' => t('Nepal time'),
  );

  $form['general_nepal_time']['nepali_calendar_show_nepal_time'] = array(
    '#group' => 'general',
    '#type' => 'checkbox',
    '#title' => t('Show Nepal time'),
    '#description' => t('If checked, Nepal time will be appended next to date.<br />E.g., 2067, Falgun 29 <strong>20:00</strong>'),
    '#default_value' => variable_get('nepali_calendar_show_nepal_time', 0),
  );

  $form['general_nepal_time']['nepali_calendar_nepal_time_format'] = array(
    '#group' => 'general',
    '#type' => 'select',
    '#title' => t('Select time format'),
    '#options' => array(
      0 => t('08:00 am/pm (12-hour format)'),
      1 => t('08:00 AM/PM (12-hour format)'),
      2 => t('20:00 (24-hour format)'),
    ),
    '#default_value' => variable_get('nepali_calendar_nepal_time_format', 0),
    '#states' => array(
      'invisible' => array(
        ':input[name="nepali_calendar_show_nepal_time"]' => array('checked' => FALSE),
      ),
    ),
  );

  return system_settings_form($form);
}

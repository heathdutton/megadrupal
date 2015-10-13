<?php

/**
 * @file
 * Nameday block settings
 */
function nameday_settings() {
  $form = array();
  $form['nameday_show_date'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show date in the name day'),
    '#default_value' => variable_get('nameday_show_date', TRUE),
    '#description' => 'If this checked, the date is displayed before the name days and holidays.'
  );
  $form['nameday_show_holiday'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show holiday in the name day, if any.'),
    '#default_value' => variable_get('nameday_show_holiday', TRUE),
  );
  $form['nameday_date_format'] = array(
    "#type" => 'select',
    '#title' => t('Date format'),
    '#options' => array(
      'small' => t('Short date format'),
      'medium' => t('Medium date format'),
      'large' => t('Long date format'),
      'custom' => t('Custom date format'),
    ),
    '#default_value' => variable_get('nameday_date_format', 'small'),
    '#description' => t('If you want to show the date, You can choose the format of it. You can set it up in the <a href="@url">Date and time settings</a>', array('@url' => url('admin/settings/date-time'))),
  );
  $form['nameday_date_format_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom date format'),
    '#default_value' => variable_get('nameday_date_format_custom', ''),
    '#description' => t('If You choose the custom date format here You can define the date format. See the <a href="@url">PHP manual</a> for available options.', array('@url' => 'http://php.net/manual/function.date.php')),
  );
  return system_settings_form($form);
}
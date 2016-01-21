<?php

/**
 * @file
 * Definition of nepali_calendar_views_handler_field_date.
 */

/**
 * A handler to provide proper displays for Nepali dates.
 *
 * @ingroup views_field_handlers
 */
class nepali_calendar_views_handler_field_date extends views_handler_field_date {
  /**
   * Provide options for this handler.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['nepali_calendar_views_date_format'] = array(
      'default' => 5,
    );

    $options['nepali_calendar_views_show_time'] = array(
      'default' => 0,
    );

    $options['nepali_calendar_views_time_format'] = array(
      'default' => 0,
    );

    return $options;
  }

  /**
   * Provide a options form for this handler.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $date = nepali_calendar_getdate_ne(time());

    // New date format for Nepali date.
    $form['date_format']['#options']['nepali_calendar'] = t('Nepali calendar');

    // Sub-format for Nepali date.
    $form['nepali_calendar_views_date_format'] = array(
      '#type' => 'select',
      '#title' => t('Nepali date format'),
      '#description' => t('Date format that will be displayed in the user screen.'),
      '#options' => nepali_calendar_nepali_date_format_array(),
      '#dependency' => array('edit-options-date-format' => array('nepali_calendar')),
      '#default_value' => isset($this->options['nepali_calendar_views_date_format']) ? $this->options['nepali_calendar_views_date_format'] : 5,
    );

    $form['nepali_calendar_views_show_time'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show Nepal time'),
      '#description' => t('If checked, Nepal time will be appended next to date.'),
      '#dependency' => array('edit-options-date-format' => array('nepali_calendar')),
      '#default_value' => isset($this->options['nepali_calendar_views_show_time']) ? $this->options['nepali_calendar_views_show_time'] : 0,
    );

    $form['nepali_calendar_views_time_format'] = array(
      '#type' => 'select',
      '#title' => t('Select time format'),
      '#options' => array(
        0 => t('08:00 am/pm (12-hour format)'),
        1 => t('08:00 AM/PM (12-hour format)'),
        2 => t('20:00 (24-hour format)'),
      ),
      '#dependency' => array('edit-options-nepali-calendar-views-show-time' => array(1)),
      '#default_value' => isset($this->options['nepali_calendar_views_time_format']) ? $this->options['nepali_calendar_views_time_format'] : 0,
    );
  }

  /**
   * Render the field.
   *
   * @param $values
   *   The values retrieved from the database.
   */
  function render($values) {
    // Datetime field value.
    $value = $values->{$this->field_alias};

    $date = nepali_calendar_getdate_ne($value);
    $date_format = nepali_calendar_date_format($date, $this->options['nepali_calendar_views_date_format']);

    if ($this->options['nepali_calendar_views_show_time'] == 1) {
      $hours12 = date('h', mktime($date['hours'], $date['minutes'], $date['seconds'], $date['mon'], $date['mday'], $date['year']));
      $hours24 = date('H', mktime($date['hours'], $date['minutes'], $date['seconds'], $date['mon'], $date['mday'], $date['year']));
      $minutes = date('i', mktime($date['hours'], $date['minutes'], $date['seconds'], $date['mon'], $date['mday'], $date['year']));
      $am_pm = (0 == strcasecmp(date('a', mktime($date['hours'], $date['minutes'], $date['seconds'], $date['mon'], $date['mday'], $date['year'])), 'am')) ? t('am') : t('pm');

      switch ($this->options['nepali_calendar_views_time_format']) {
        case 0:
          $date_format .= ' ' . nepali_calendar_translate_digit($hours12) . ':' . nepali_calendar_translate_digit($minutes) . ' ' . $am_pm;
          break;

        case 1:
          $date_format .= ' ' . nepali_calendar_translate_digit($hours12) . ':' . nepali_calendar_translate_digit($minutes) . ' ' . strtoupper($am_pm);
          break;

        case 2:
          $date_format .= ' ' . nepali_calendar_translate_digit($hours24) . ':' . nepali_calendar_translate_digit($minutes);
          break;
      }
    }

    if ($this->options['date_format'] == 'nepali_calendar') {
      return $date_format;
    }
    else {
      return parent::render($values);
    }
  }
}

<?php

/**
 * @file
 * Contains help classes to perform field_timer_text formatter related actions.
 */

/**
 * Base class for field_timer_text formatter.
 */
abstract class field_timer_text_base extends field_timer_formatter_base {
  /**
   * @inheritdoc
   */
  public function enableFormatter() {
    return TRUE;
  }

  /**
   * @inheritdoc
   */
  final public function getFormatterDefinition() {
    return array(
      'label' => t('Text'),
      'description' => t('Simple text timer and countdown.'),
    );
  }


  /**
   * @inheritdoc
   */
  public function getDefaultSettings() {
    return array(
      'type' => 'auto',
      'granularity' => 2,
    );
  }

  /**
   * @inheritdoc
   */
  public function getSettingsSummary($settings, $field) {
    $summary = array();
    $types = $this->formatterTypes();
    $summary[] = t('Type: %type', array('%type' => $types[$settings['type']]));
    $summary[] = t('Granularity: %granularity', array('%granularity' => $settings['granularity']));

    return $summary;
  }

  /**
   * @inheritdoc
   */
  public function getSettingsForm($settings, $field) {
    $settings_form = array();
    $settings_form['type'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#options' => $this->formatterTypes(),
      '#default_value' => $settings['type'],
    );

    $settings_form['granularity'] = array(
      '#type' => 'select',
      '#title' => t('Granularity'),
      '#options' => $this->formatterGranularity(),
      '#default_value' => $settings['granularity'],
    );

    return $settings_form;
  }

  public function renderItems($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
    $elements = array();
    $settings = $display['settings'];

    foreach ($items as $delta => $item) {
      $elements[$delta] = array(
        '#type' => 'markup',
        '#markup' => $this->renderItem($settings['type'], $settings['granularity'], $this->getTimestamp($item, $field)),
      );
    }

    return $elements;
  }

  /**
   * @inheritdoc
   */
  public function getFormatterName() {
    return 'field_timer_text';
  }

  /**
   * Render field item.
   */
  protected function renderItem($type, $granularity, $timestamp) {
    $time = '';
    // Check what we should display - timer or countdown.
    switch ($type) {
      case 'auto':
        $interval = REQUEST_TIME - $timestamp;
        $sign = ($interval > 0) ? '+' : '-';
        $time = $sign . format_interval(abs($interval), $granularity);
        break;

      case 'timer':
        $time = format_interval(REQUEST_TIME - $timestamp, $granularity);
        break;

      case 'countdown':
        $time = format_interval($timestamp - REQUEST_TIME, $granularity);
        break;
    }

    return $time;
  }

  protected function formatterTypes() {
    return array(
      'auto' => t('Auto'),
      'timer' => t('Timer'),
      'countdown' => t('Countdown'),
    );
  }

  protected function formatterGranularity() {
    $options = range(0, 7);
    unset($options[0]);
    return $options;
  }

}

/**
 * Class for field_timer_text formatter and field_timer field type.
 */
class field_timer_text_field_timer extends field_timer_text_base {

  /**
   * @inheritdoc
   */
  public function getFieldType() {
    return 'field_timer';
  }

  /**
   * @inheritdoc
   */
  protected function getTimestamp($item, $field, $key = 'timestamp') {
    return $item[$key];
  }

}

/**
 * Base class for field_timer_text formatter and date field types.
 */
abstract class field_timer_text_date_field_base extends field_timer_text_base {

  /**
   * @inheritdoc
   */
  public function getDefaultSettings() {
    return parent::getDefaultSettings() + array(
      'title' => '',
      'title2' => '',
      'type2' => 'auto',
      'granularity2' => 2,
      'fromto' => 'both',
    );
  }

  /**
   * @inheritdoc
   */
  public function getSettingsSummary($settings, $field) {
    $summary = array();
    if (!empty($field['settings']['todate'])) {
      $types = $this->formatterTypes();
      $displays = $this->formatterDisplays();
      $summary[] = t('Type for Start date: %type', array('%type' => $types[$settings['type']]));
      $summary[] = t('Granularity for Start date: %granularity', array('%granularity' => $settings['granularity']));
      $summary[] = t('Title for Start date: %title', array('%title' => $settings['title']));
      $summary[] = t('Type for End date: %type', array('%type' => $types[$settings['type2']]));
      $summary[] = t('Granularity for End date: %granularity', array('%granularity' => $settings['granularity2']));
      $summary[] = t('Title for End date: %title', array('%title' => $settings['title2']));
      $summary[] = t('Display: %display', array('%display' => $displays[$settings['fromto']]));
    }
    else {
      $summary[] = t('Title: %title', array('%title' => $settings['title']));
      $summary = array_merge($summary, parent::getSettingsSummary($settings, $field));
    }

    return $summary;
  }

  /**
   * @inheritdoc
   */
  public function getSettingsForm($settings, $field) {
    $settings_form = parent::getSettingsForm($settings, $field);

    $settings_form['title'] = array(
      '#type' => 'textfield',
      '#title' => empty($field['settings']['todate']) ? t('Title') : t('Title for Start date'),
      '#default_value' => $settings['title'],
      '#weight' => -10,
    );

    if (!empty($field['settings']['todate'])) {
      $settings_form['type']['#title'] = t('Type for Start date');
      $settings_form['granularity']['#title'] = t('Granularity for Start date');
      $settings_form['title2'] = array(
        '#type' => 'textfield',
        '#title' => t('Title for End date'),
        '#default_value' => $settings['title2'],
      );

      $settings_form['type2'] = array(
        '#type' => 'select',
        '#title' => t('Type for End date'),
        '#options' => $this->formatterTypes(),
        '#default_value' => $settings['type2'],
      );

      $settings_form['granularity2'] = array(
        '#type' => 'select',
        '#title' => t('Granularity for End date'),
        '#options' => $this->formatterGranularity(),
        '#default_value' => $settings['granularity2'],
      );

      $settings_form['fromto'] = array(
        '#type' => 'select',
        '#title' => t('Display'),
        '#options' => $this->formatterDisplays(),
        '#default_value' => $settings['fromto'],
      );
    }

    return $settings_form;
  }

  /**
   * @inheritdoc
   */
  public function renderItems($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
    $settings = $display['settings'];
    $elements = ($settings['fromto'] != 'to') ? parent::renderItems($entity_type, $entity, $field, $instance, $langcode, $items, $display) : array();

    foreach ($items as $delta => $item) {
      if ($settings['fromto'] != 'to' && !empty($settings['title'])) {
        $elements[$delta]['#markup'] = '<span class="field-timer-start-date-title">'
        . $settings['title'] . '</span> ' . $elements[$delta]['#markup'];
      }
      if ($settings['fromto'] == 'both' && !empty($field['settings']['todate'])) {
        $elements[$delta]['#markup'] = '<div class="field-timer-start-date">'
        . $elements[$delta]['#markup'] . '</div>';
      }
      if ($settings['fromto'] != 'from' && !empty($field['settings']['todate'])) {
        $title = (!empty($settings['title2'])) ? '<span class="field-timer-end-date-title">' . $settings['title2'] . '</span> ' : '';
        $time = $this->renderItem($settings['type2'], $settings['granularity2'], $this->getTimestamp($item, $field, 'value2'));
        if ($settings['fromto'] == 'to') {
          $elements[$delta] = array(
            '#type' => 'markup',
            '#markup' => $title . $time,
          );
        }
        else {
          $elements[$delta]['#markup'] .= '<div class="field-timer-end-date">'
          . $title . $time . '</div>';
        }
      }
    }

    return $elements;
  }

  protected function formatterDisplays() {
    return array(
      'both' => t('Both Start and End dates'),
      'from' => t('Start date only'),
      'to' => t('End date only'),
    );
  }

}

/**
 * Class for field_timer_text formatter and datestamp field type.
 */
class field_timer_text_datestamp extends field_timer_text_date_field_base {

  /**
   * @inheritdoc
   */
  public function getFieldType() {
    return 'datestamp';
  }
  /**
   * @inheritdoc
   */
  protected function getTimestamp($item, $field, $key = 'value') {
    return $item[$key];
  }
}

/**
 * Class for field_timer_text formatter and date field type.
 */
class field_timer_text_date extends field_timer_text_date_field_base {

  /**
   * @inheritdoc
   */
  public function getFieldType() {
    return 'date';
  }

  /**
   * @inheritdoc
   */
  protected function getTimestamp($item, $field, $key = 'value') {
    $timezone_db = date_get_timezone_db($field['settings']['tz_handling']);
    $db_format = date_type_format($field['type']);
    $date = new DateObject($item[$key], $timezone_db, $db_format);
    return $date->getTimestamp();
  }

}

/**
 * Class for field_timer_text formatter and datetime field type.
 */
class field_timer_text_datetime extends field_timer_text_date_field_base {

  /**
   * @inheritdoc
   */
  public function getFieldType() {
    return 'datetime';
  }

  /**
   * @inheritdoc
   */
  protected function getTimestamp($item, $field, $key = 'value') {
    $timezone_db = date_get_timezone_db($field['settings']['tz_handling']);
    $db_format = date_type_format($field['type']);
    $date = new DateObject($item[$key], $timezone_db, $db_format);
    return $date->getTimestamp();
  }

}

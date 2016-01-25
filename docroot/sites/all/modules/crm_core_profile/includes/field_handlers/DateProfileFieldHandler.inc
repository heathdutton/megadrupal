<?php

/**
 * @file
 * Date fields handler.
 */

/**
 * Class DateProfileFieldHandler.
 */
class DateProfileFieldHandler extends DefaultProfileFieldHandler {

  public function renderField($field_name, $fields_config, &$form, &$form_state) {
    $field_element =& $form[$field_name][LANGUAGE_NONE][0];
    $entity_type = $field_element['#entity_type'];
    $bundle = $field_element['#bundle'];

    // Store the field prefix.
    $field_prefix = $entity_type . '_' . $bundle . '_';
    $profile_field = $field_prefix . $field_name;

    if (array_key_exists($profile_field, $fields_config)) {
      $config = $fields_config[$profile_field];
      // Apply default field value.
      if (!empty($config['default'])) {
        $field = field_info_field($field_name);
        $timezone = date_get_timezone($field['settings']['tz_handling']);
        $default_value = '';
        $strtotime = @strtotime($config['default']);
        if ($strtotime) {
          $date = new DateObject($strtotime, $timezone);
          switch ($field['type']) {
            case DATE_ISO:
              $default_value = $date->format(DATE_FORMAT_ISO);
              break;

            case DATE_UNIX:
              $default_value = $date->format(DATE_FORMAT_UNIX);
              break;

            case DATE_DATETIME:
              $default_value = $date->format(DATE_FORMAT_DATETIME);
              break;
          }
        }
        $field_element['#default_value']['value'] = $default_value;
      }
    }

    // Only default values need help rendering, call parent renderField.
    parent::renderField($field_name, $fields_config, $form, $form_state);
  }
}

/**
 * Class DateTimeProfileFieldHandler.
 */
class DateTimeProfileFieldHandler extends DateProfileFieldHandler {

}

/**
 * Class DateStampProfileFieldHandler.
 */
class DateStampProfileFieldHandler extends DateProfileFieldHandler {

}

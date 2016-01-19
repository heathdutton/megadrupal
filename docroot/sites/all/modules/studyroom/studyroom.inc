<?php

/**
 * @file
 * Contains studyroom class.
 */

/**
 * Studyroom class for studyroom system.
 */
class Studyroom {
  /**
   * Return an array of minutes.
   */
  public function validateDurations($form, &$form_state, $entity_type, $entity = NULL) {
    $values = $form_state['values'];
    $min_field = 'min_duration';
    $max_field = 'max_duration';
    $site_level = FALSE;

    if (isset($values['studyroom_' . $min_field])) {
      $min_field = 'studyroom_' . $min_field;
      $max_field = 'studyroom_' . $max_field;
      $site_level = TRUE;
    }

    if (empty($values[$min_field])) {
      form_set_value($form['rules'][$min_field], NULL, $form_state);
    }

    if (empty($values[$max_field])) {
      form_set_value($form['rules'][$max_field], NULL, $form_state);
    }

    // Return if both fields empty.
    if (empty($values[$min_field]) && empty($values[$max_field])) {
      return;
    }

    // Only check if both have value.
    if (!empty($values[$min_field]) && !empty($values[$max_field])) {
      if ($values[$min_field] > $values[$max_field]) {
        form_set_error($max_field, t('The maximum duration must be larger that the minimum duration.'));
      }
    }

    if ($entity_type == 'studyroom') {
      return;
    }

    $min_duration = $this->getMinDuration($entity);
    $max_duration = $this->getMaxDuration($entity);

    if (!empty($values[$min_field]) && isset($max_duration)) {
      if ($values[$min_field] > $max_duration) {
        form_set_error($min_field, t('The minimum duration must be less than the maximum duration of %time.',
          array('%time' => format_interval($max_duration))));
        return;
      }
    }

    if (!empty($values[$max_field]) && isset($min_duration)) {
      if ($values[$max_field] < $min_duration) {
        form_set_error($max_field, t('The maximum duration must be larger that the minimum duration of %time.',
          array('%time' => format_interval($min_duration))));
        return;
      }
    }
  }

  /**
   * Return the mininum duration based on values set.
   */
  public function getMinDuration($entity = NULL, $resv_type = NULL, $parent = FALSE) {
    if (isset($resv_type) && !$resv_type->public) {
      return variable_get('studyroom_time_increment', '15') * 60;
    }

    if (!$parent && isset($entity->min_duration)) {
      return $entity->min_duration;
    }

    if (isset($entity->type)) {
      $entity_type = studyroom_space_location_load($entity->type);
      if (isset($entity_type->min_duration)) {
        return $entity_type->min_duration;
      }
    }

    return variable_get('studyroom_min_duration', NULL);
  }

  /**
   * Return the maximum duration based on values set.
   */
  public function getMaxDuration($entity = NULL, $resv_type = NULL, $parent = FALSE) {
    if (isset($resv_type) && !$resv_type->public) {
      // Return 24 hours as seconds.
      return '86400';
    }

    if (!$parent && isset($entity->max_duration)) {
      return $entity->max_duration;
    }

    if (isset($entity->type)) {
      $entity_type = studyroom_space_location_load($entity->type);
      if (isset($entity_type->max_duration) && $entity_type->max_duration) {
        return $entity_type->max_duration;
      }
    }

    return variable_get('studyroom_max_duration', NULL);
  }

  /**
   * Return the maximum duration based on values set.
   */
  public function getMaxDaysAdvance($entity = NULL, $resv_type = NULL, $parent = FALSE) {
    if (isset($resv_type) && !$resv_type->public) {
      return '9999';
    }

    if (!$parent && isset($entity->max_days_advance)) {
      return $entity->max_days_advance;
    }

    if (isset($entity->type)) {
      $entity_type = studyroom_space_location_load($entity->type);
      if ($entity_type->max_days_advance) {
        return $entity_type->max_days_advance;
      }
    }

    return variable_get('studyroom_max_days_advance', NULL);
  }

  /**
   * Return default value for minimum duration.
   */
  public function defaultMinDuration() {
    return '900';
  }

  /**
   * Return default value for maximum duration.
   */
  public function defaultMaxDuration() {
    return '14400';
  }

  /**
   * Return default value for maximum days in advance.
   */
  public function defaultMaxDayAdvance() {
    return '14';
  }

  /**
   * Build array of options for minimum duration.
   *
   * @return array
   *   array of options formated for use in select form element
   */
  public function optionsMinDuration($default = TRUE) {
    $intervals = array(900, 1800, 3600, 7200, 10800, 14400);
    if ($default) {
      return array('' => '- Default -') + drupal_map_assoc($intervals, 'format_interval');
    }
    return drupal_map_assoc($intervals, 'format_interval');
  }

  /**
   * Build array of options for maximum duration.
   *
   * @return array
   *   array of options formated for use in select form element
   */
  public function optionsMaxDuration($default = TRUE) {
    $intervals = array(3600, 7200, 10800, 14400, 21600, 32400, 43200, 64800, 86400);
    if ($default) {
      return array('' => '- Default -') + drupal_map_assoc($intervals, 'format_interval');
    }
    return drupal_map_assoc($intervals, 'format_interval');
  }

  /**
   * Build array of options for maximum day in advance.
   *
   * @return array
   *   array of options formated for use in select form element
   */
  public function optionsMaxDayAdvance($default = TRUE) {
    $intervals = array(1, 2, 3, 4, 5, 6, 7, 14, 21, 30, 60, 90, 120, 180, 365);
    if ($default) {
      return array('' => '- Default -') + drupal_map_assoc($intervals, 'studyroom_format_day');
    }
    return drupal_map_assoc($intervals, 'studyroom_format_day');
  }

  /**
   * Return an array of minutes.
   */
  public function durationMinutes() {
    $increment = variable_get('studyroom_time_increment', '15');
    $increment = $increment * 60;
    $range = range($this->min_duration, $this->max_duration, $increment);

    // Remove minutes from everything over 4 hours.
    if ($this->entity_type->increment_limits['minutes']) {
      foreach ($range as $key => $seconds) {
        if ($seconds < 14440) {
          continue;
        }
        if (($seconds % 3600) != 0) {
          unset($range[$key]);
        }
      }
    }

    // Remove hours above 8 except 12, 18, and 24 hours.
    if ($this->entity_type->increment_limits['hours_large']) {
      foreach ($range as $key => $seconds) {
        if ($seconds <= 28800) {
          continue;
        }
        if (!in_array($seconds, array('43200', '64800', '86400'))) {
          unset($range[$key]);
        }
      }
    }

    sort($range);
    return $range;
  }
}


/**
 * Extends controller for generating extra fields to include more options.
 */
class StudyroomExtraFieldsController extends EntityDefaultExtraFieldsController {
  /**
   * Generates the display field info for a given property.
   */
  protected function generateExtraFieldInfo($name, $property_info) {
    $info = array(
      'label' => $property_info['label'],
      'weight' => isset($property_info['weight']) ? $property_info['weight'] : 0,
      'visible' => isset($property_info['visible']) ? $property_info['visible'] : TRUE,
    );
    if (!empty($property_info['description'])) {
      $info['description'] = $property_info['description'];
    }
    return $info;
  }
}

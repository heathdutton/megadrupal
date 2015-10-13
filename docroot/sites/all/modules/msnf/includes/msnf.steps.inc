<?php

/**
 * @file
 * Step crud functions.
 */

/**
 * Returns all steps for a content type.
 *
 * @return <array>
 *   All steps including associated fields in a structured array.
 */
function msnf_steps($content_type = '', $sorted = FALSE, $reset = FALSE) {
  global $language;
  static $steps, $steps_sorted;
  if (!isset($steps) || $reset) {
    if (($cached = cache_get('msnf_step_data:' . $language->language)) !== 0 && !empty($cached->data) && (time() < $cached->expire)) {
      $data = $cached->data;
      $steps = $data['steps'];
      $steps_sorted = $data['steps_sorted'];
    }
    else {
      $result = db_query('SELECT * FROM {msnf_step} ORDER BY weight, step_name');
      $steps = array();
      $steps_sorted = array();
      while ($step = db_fetch_array($result)) {
        $step['settings'] = unserialize($step['settings']);
        $step['fields'] = array();
        $step['groups'] = array();

        // Allow external modules to translate field step strings.
        $step_strings = array(
          'label' => $step['label'],
          'form_description' => $step['settings']['form']['description'],
          'form_button_label_previous' => $step['settings']['form']['button_label']['previous'],
          'form_button_label_next' => $step['settings']['form']['button_label']['next'],
          'form_description' => $step['settings']['form']['description'],
        );
        drupal_alter('msnf_step_strings', $step_strings, $step['type_name'], $step['step_name']);
        $step['label'] = $step_strings['label'];
        $step['settings']['form']['description'] = $step_strings['form_description'];
        $step['settings']['form']['button_label']['previous'] = $step_strings['form_button_label_previous'];
        $step['settings']['form']['button_label']['next'] = $step_strings['form_button_label_next'];

        // Load non-CCK fields and add them to the list.
        $extra_fields = msnf_extra_fields($step['type_name']);
        $step_fields = msnf_step_get_fields($step['type_name'], $step['step_name']);
        foreach ($extra_fields as $field_name => $field) {
          if (isset($step_fields[$field_name])) {
            // We do not have a real widget here but this doesn't matter.
            $field_strings = array(
              'widget_label' => $field['label'],
              'widget_description' => $field['description'],
            );

            // Allow external modules to translate field strings
            // (using hook_content_field_strings()!).
            drupal_alter('content_field_strings', $field_strings, $field['type_name'], $field_name);
            $field['label'] = $field_strings['widget_label'];
            $field['description'] = $field_strings['widget_description'];

            $step['fields'][$field_name] = $field;
          }
        }
        $steps[$step['type_name']][$step['step_name']] = $step;
        $steps_sorted[$step['type_name']][] = &$steps[$step['type_name']][$step['step_name']];
      }
      // Load fields from content.module (if installed).
      if (module_exists('content')) {
        $result = db_query('SELECT nfi.*, ms.step_name FROM {msnf_step} ms ' .
   'INNER JOIN {msnf_step_fields} msf ON msf.type_name = ms.type_name AND msf.step_name = ms.step_name ' .
   'INNER JOIN {' . content_instance_tablename() . '} nfi ON nfi.field_name = msf.field_name AND nfi.type_name = msf.type_name ' .
   'WHERE nfi.widget_active = 1 ORDER BY nfi.weight');
        while ($field = db_fetch_array($result)) {
          $field_strings = array(
            'widget_label' => $field['label'],
            'widget_description' => $field['description'],
          );

          // Allow external modules to translate field strings
          // (using hook_content_field_strings()!).
          drupal_alter('content_field_strings', $field_strings, $field['type_name'], $field['field_name']);
          $field['label'] = $field_strings['widget_label'];
          $field['description'] = $field_strings['widget_description'];

          $steps[$field['type_name']][$field['step_name']]['fields'][$field['field_name']] = $field;
        }
      }
      // Load fieldgroups from fieldgroup.module (if installed).
      if (module_exists('fieldgroup')) {
        $result = db_query('SELECT cg.*, ms.step_name FROM {msnf_step} ms ' .
   'INNER JOIN {msnf_step_fields} msf ON msf.type_name = ms.type_name AND msf.step_name = ms.step_name ' .
   'INNER JOIN {' . fieldgroup_tablename() . '} cg ON cg.group_name = msf.field_name AND cg.type_name = msf.type_name ' .
   'ORDER BY cg.weight');
        while ($group = db_fetch_array($result)) {
          $group_strings = array(
            'widget_label' => $group['label'],
            'widget_description' => $group['description'],
          );

          // Allow external modules to translate field strings
          // (using hook_content_field_strings()!).
          drupal_alter('content_field_strings', $group_strings, $group['type_name'], $group['group_name']);
          $group['label'] = $group_strings['widget_label'];
          $group['description'] = $group_strings['widget_description'];

          $steps[$group['type_name']][$group['step_name']]['groups'][$group['group_name']] = $group;
        }
      }
      cache_set('msnf_step_data:' . $language->language, array('steps' => $steps, 'steps_sorted' => $steps_sorted));
    }
  }
  if (empty($content_type)) {
    return $steps;
  }
  elseif (empty($steps) || empty($steps[$content_type])) {
    return array();
  }
  return $sorted ? $steps_sorted[$content_type] : $steps[$content_type];
}

/**
 * Return a list of all available step types.
 */
function msnf_step_types() {
  $types = array('standard' => t('Standard step'));
  // Allow other modules to add new step_types.
  $types = array_merge($types, module_invoke_all('msnf_step_types'));
  return $types;
}

/**
 * Provide labels for all steps.
 */
function _msnf_steps_label($content_type) {
  $steps = msnf_steps($content_type);

  $labels[''] = '<' . t('none') . '>';
  foreach ($steps as $step_name => $step) {
    $labels[$step_name] = t($step['label']);
  }
  return $labels;
}

/**
 * Saves a step for a content-type.
 *
 * @param <string> $type_name
 *   Name of content type to associate the form step with.
 * @param <array> $step
 *   Step informations to save.
 */
function msnf_save_step($type_name, $step) {
  $steps = msnf_steps($type_name);

  // Allow other modules to intervene when the step is saved.
  foreach (module_implements('msnf_save_step') as $module) {
    $function = $module . '_msnf_save_step';
    $function($step);
  }

  $result = SAVED_NEW;
  if (!isset($steps[$step['step_name']])) {
    // Accept step name from programmed submissions if valid.
    db_query('INSERT INTO {msnf_step} (step_type, type_name, step_name, label, settings, weight)' .
      " VALUES ('%s', '%s', '%s', '%s', '%s', %d)", $step['step_type'], $type_name, $step['step_name'], $step['label'], serialize($step['settings']), $step['weight']);
    cache_clear_all('msnf_step_data:', 'cache', TRUE);
  }
  else {
    db_query("UPDATE {msnf_step} SET step_type = '%s', label = '%s', settings = '%s', weight = %d " .
              "WHERE type_name = '%s' AND step_name = '%s'",
              $step['step_type'], $step['label'], serialize($step['settings']), $step['weight'], $type_name, $step['step_name']);
    cache_clear_all('msnf_step_data:', 'cache', TRUE);
    $result = SAVED_UPDATED;
  }
  // Update field and group mapping.
  foreach (array('fields', 'groups') as $item_type) {
    if (isset($step[$item_type])) {
      foreach ($step[$item_type] as $item_name => $item) {
        $values = array(
          'type_name' => $type_name,
          'step' => $step['step_name'],
          "field_name" => $item_name,
          'weight' => $item['weight'],
        );
        msnf_step_update_fields($values);
      }
    }
  }
  return $result;
}

/**
 * Update field mapping for steps.
 */
function msnf_step_update_fields($form_values) {
  if ($form_values['step']) {
    // Parent is a fieldgroup!
    if (strpos($form_values['step'], 'group_') > -1) {
      // If fieldgroup module is not enabled at this point, do nothing.
      if (module_exists('fieldgroup')) {
        // Prepare form values for fieldgroup update.
        $form_values['group'] = $form_values['step'];
        // Update field parenting.
        fieldgroup_update_fields($form_values);
        // Remove field from msnf_step_fields (if it exists there).
        db_query("DELETE FROM {msnf_step_fields} WHERE type_name = '%s' AND field_name = '%s'", $form_values['type_name'], $form_values['field_name']);
      }
    }
    // Parent is a step.
    else {
      // Get existing entry.
      $default = _msnf_field_get_step($form_values['type_name'], $form_values['field_name']);
      if ($default) {
        db_query("UPDATE {msnf_step_fields} SET step_name = '%s', weight = %d WHERE type_name = '%s' AND field_name = '%s'", $form_values['step'], $form_values['weight'], $form_values['type_name'], $form_values['field_name']);
      }
      else {
        db_query("INSERT INTO {msnf_step_fields} (type_name, step_name, field_name, weight) VALUES ('%s', '%s', '%s', %d)", $form_values['type_name'], $form_values['step'], $form_values['field_name'], $form_values['weight']);
      }
      if (module_exists('fieldgroup')) {
        // Remove field from fieldgroup table.
        db_query('DELETE FROM {' . fieldgroup_fields_tablename() . "} WHERE type_name = '%s' AND field_name = '%s'", $form_values['type_name'], $form_values['field_name']);
      }
    }
  }
  else {
    // Remove field from tables.
    if (module_exists('fieldgroup')) {
      db_query('DELETE FROM {' . fieldgroup_fields_tablename() . "} WHERE type_name = '%s' AND field_name = '%s'", $form_values['type_name'], $form_values['field_name']);
    }
    db_query("DELETE FROM {msnf_step_fields} WHERE type_name = '%s' AND field_name = '%s'", $form_values['type_name'], $form_values['field_name']);
  }
  cache_clear_all('msnf_step_data:', 'cache', TRUE);
}

/**
 * Delete a single step.
 *
 * @param <string> $content_type
 *   Name of content type.
 * @param <string> $step_name
 *   Name of step to delete.
 */
function msnf_step_delete($content_type, $step_name) {
  db_query("DELETE FROM {msnf_step} WHERE  type_name = '%s' AND step_name = '%s'", $content_type, $step_name);
  db_query("DELETE FROM {msnf_step_fields} WHERE  type_name = '%s' AND step_name = '%s'", $content_type, $step_name);
  cache_clear_all('msnf_step_data:', 'cache', TRUE);
}

/**
 * Provide default settings for a form step.
 *
 * @param $step_type
 *   Type of step to return the default settings for.
 *
 * @return <array>
 *   Default settings for the given step type.
 */
function msnf_step_default_settings($step_type) {
  $settings = array(
    'form' => array(
      'style' => 'default',
      'description' => '',
      'button_label' => array(
        'next' => t('Next'),
        'previous' => t('Previous'),
      ),
    ),
  );
  // Allow other modules to add new default settings.
  return array_merge($settings, module_invoke_all('msnf_step_default_settings', $step_type));
}

/**
 * API for step name validation.
 *
 * Pulled into separate function to be re-usable.
 */
function msnf_step_validate_name($step, $type_name) {
  $errors = array();

  // No label.
  if (!$step['label']) {
    $errors['label'][] = t('You need to provide a label.');
  }

  // No step name.
  if (!$step['step_name']) {
    $errors['step_name'][] = t('You need to provide a step name.');
  }
  // Group name validation.
  else {
    $step_name = $step['step_name'];
    $step['step_type'] = !empty($step['step_type']) ? $step['step_type'] : 'standard';

    // Add the 'step_' prefix.
    if (drupal_substr($step_name, 0, 6) != 'step_') {
      $step_name = 'step_' . $step_name;
    }

    // Invalid field name.
    if (!preg_match('!^step_[a-z0-9_]+$!', $step_name)) {
      $errors['step_name'][] = t('The step name %step_name is invalid. The name must include only lowercase unaccentuated letters, numbers, and underscores.', array('%step_name' => $step_name));
    }
    if (drupal_strlen($step_name) > 32) {
      $errors['step_name'][] = t('The step name %step_name is too long. The name is limited to 32 characters, including the "step_" prefix.', array('%step_name' => $step_name));
    }

    // Group name already exists.
    $steps = msnf_steps($type_name);
    if (isset($steps[$step_name])) {
      $errors['step_name'][] = t('The step name %step_name already exists.', array('%step_name' => $step_name));
    }
    if (empty($errors['step_name'])) {
      $step['step_name'] = $step_name;
    }
  }
  return array('step_name' => $step['step_name'], 'errors' => $errors);
}

/**
 * Helper function to get the step name for a field.
 *
 * @param <string> $content_type
 *   Content type the field belongs to.
 * @param <string> $field_name
 *   Name of field.
 *
 * @return <mixed>
 *   Name of step the field belongs to or FALSE if the field doesn't belong to
 *   a form step.
 */
function _msnf_field_get_step($content_type, $field_name) {
  return db_result(db_query("SELECT step_name FROM {msnf_step_fields} WHERE type_name = '%s' AND field_name = '%s'", $content_type, $field_name));
}

/**
 * Function to get a steps data.
 *
 * @param <string> $type_name
 *   Name of content type the step belongs to.
 * @param <string> $step_name
 *   Name of step to load.
 */
function msnf_step_get_step($type_name, $step_name) {
  return db_fetch_array(db_query("SELECT * FROM {msnf_step} WHERE type_name = '%s' AND step_name = '%s'", array($type_name, $step_name)));
}

/**
 * Function to return a list of field names for a form step of a content type.
 *
 * @param <string> $content_type
 *   Name of content type.
 * @param <string> $step_name
 *   Name of form step.
 *
 * @return <array>
 *   List of field names for the form step.
 */
function msnf_step_get_fields($content_type, $step_name) {
  $fields = array();
  $result = db_query("SELECT * FROM {msnf_step_fields} WHERE type_name = '%s' AND step_name = '%s'", array($content_type, $step_name));

  while (($field = db_fetch_array($result)) !== FALSE) {
    $fields[$field['field_name']] = $field;
  }

  return $fields;
}

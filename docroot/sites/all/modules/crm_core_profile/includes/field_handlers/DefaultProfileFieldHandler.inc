<?php

/**
 * @file
 * Default profile fields handler.
 */

/**
 * Interface for handling the generation of fields within CRM Core Profiles.
 */
interface ProfileFieldHandlerInterface {

  /**
   * This function is triggered when a field is being used in a profile.
   *
   * @param $form array: the array strucutre for the form we are rendering
   * @param $profile: the crm core profile being used to generate the form.
   */
  public function renderField($field_config, $profile, &$form, &$form_state);

  /**
   * This function is triggered when a CRM Core Profile is being validated.
   */
  public function validateField($field_config, $profile, &$form, &$form_state);

  /**
   * This function is triggered when in afer build callback of CRM Core Profile page.
   */
  public function afterBuild($field_name, $profile, &$form, &$form_state);

  /**
   * This function is triggered when a CRM Core Profile page is being submitted.
   */
  public function submitField($field_name, $profile, &$form, &$form_state);

  public function renderFieldConfig(&$form, $entity_type, $entity_label, $bundle, $field_name);
}

/**
 * Default class for rendering a form field in CRM Core Profile.
 *
 * @param array $field: Field being rendered
 * @param array $field_info: Field info for the field
 * @param array $profile: a list of fields associated with the profile
 * @param array $form: The form array we are adding the fields to
 */
class DefaultProfileFieldHandler implements ProfileFieldHandlerInterface {

  /**
   * This function is triggered when a field is being used in a profile.
   */
  public function renderField($field_name, $profile, &$form, &$form_state) {
    // Construct the field element key.
    $profile_field = $form['#entity_type'] . '_' . $form['#bundle'] . '_' . $field_name;

    if (array_key_exists($profile_field, $profile)) {
      // Apply default field value.
      if (!empty($profile[$profile_field]['default'])) {
        $field = field_info_field($field_name);
        switch ($field['type']) {
          case 'list_integer':
          case 'list_float':
          case 'list_text':
            $default = array();
            if (isset($field['settings']['allowed_values'])) {
              $default[$profile[$profile_field]['default']] = $field['settings']['allowed_values'][$profile[$profile_field]['default']];
            }
            else {
              $default[$profile[$profile_field]['default']] = $profile[$profile_field]['default'];
            }
            $form[$field_name][LANGUAGE_NONE]['#default_value'] = $default;
            break;

          default:
            $form[$field_name][LANGUAGE_NONE][0]['value']['#default_value'] = $profile[$profile_field]['default'];
            if (module_exists('token')) {
              // Extending field length to use tokens.
              $form[$field_name][LANGUAGE_NONE][0]['value']['#maxlength'] = 128;
              // Also we must remove validation, because tokens obviously won't
              // be valid.
              $form[$field_name][LANGUAGE_NONE][0]['value']['#element_validate'] = array();
            }
            // If the field is disabled, then reset the #default_value.
            // This is needed for some fields, where the tokens are leading
            // to validation errors.
            if (empty($profile[$profile_field]['status'])) {
              // Unset #default_value, we will reset it in #submit handler.
              unset($form[$field_name][LANGUAGE_NONE][0]['value']['#default_value']);
            }
        }
      }

      // Hide "hidden" field.
      if (!empty($profile[$profile_field]['hidden'])) {
        $form[$field_name]['#attributes']['class'][] = 'element-hidden';
      }
    }

    // Remove disabled field from profile by denying access and removing
    // associated field element validators.
    if (!array_key_exists($profile_field, $profile) || empty($profile[$profile_field]['status'])) {
      $form[$field_name]['#access'] = FALSE;
      $form[$field_name]['#element_validate'] = array();
    }
  }

  /**
   * This function is triggered when a CRM Core Profile is being validated.
   */
  public function validateField($field_config, $profile, &$form, &$form_state) {
  }

  /**
   * This function is triggered when in afer build callback of CRM Core Profile page.
   */
  public function afterBuild($field_name, $profile, &$form, &$form_state) {
  }

  /**
   * This function is triggered when a CRM Core Profile is being submitted.
   */
  public function submitField($field_name, $profile, &$form, &$form_state) {
  }

  public function renderFieldConfig(&$form, $entity_type, $entity_label, $bundle, $field_name) {
    $field_config = array();

    if (isset($form['#profile'])) {
    $profile = $form['#profile'];
    }

    $field_instance = field_info_instance($entity_type, $field_name, $bundle);
    $default_field_config = array(
      'entity_type' => $entity_type,
      'bundle' => $bundle,
      'field_name' => $field_name,
      'field_item' => '',
      'field_label' => $field_instance['label'],
      'weight' => _crm_core_profile_default_weight(),
      'handler' => get_class($this),
    );
    $field_separator = _crm_core_profile_field_separator($default_field_config);
    if (isset($profile->fields[$field_separator])) {
      $field_config = $profile->fields[$field_separator];
    }
    $field_config += $default_field_config;

    _crm_core_profile_new_form_fields($form, $entity_label, $field_config);
  }
}

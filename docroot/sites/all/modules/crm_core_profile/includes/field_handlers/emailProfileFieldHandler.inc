<?php

/**
 * Default class for rendering a form field in CRM Core Profile.
 *
 * @param array $field
 *   Field to render config for.
 * @param array $field_info
 *   Field info.
 * @param array $form
 *   Form to be modified
 */
class emailProfileFieldHandler extends DefaultProfileFieldHandler {

  /**
   * This function is triggered when a field is being used in a profile.
   */
  public function renderField($field_name, $profile, &$form, &$form_state) {
    // Construct the field element key.
    $profile_field = $form['#entity_type'] . '_' . $form['#bundle'] . '_' . $field_name;

    if (array_key_exists($profile_field, $profile)) {
      // Apply default field value.
      if ($profile[$profile_field]['default']) {
        $form[$field_name][LANGUAGE_NONE][0]['email']['#default_value'] = $profile[$profile_field]['default'];
      }
    }

    // Only default values need help rendering, call parent renderField.
    parent::renderField($field_name, $profile, $form, $form_state);
  }
}

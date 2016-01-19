<?php

/**
 * @file
 * CRM Core Activity handler class.
 */

/**
 * CRM Core Activity handler class.
 */
class CRMCoreActivityHandler extends ProfileEntityHandler {

  /**
   * Returns entity info.
   */
  public function entityInfo() {
    return array(
      'entity' => 'crm_core_activity',
      'entity_label' => t('CRM Core Activity'),
    );
  }

  /**
   * Validate handler for profile edit form.
   *
   * Ensures that activity title enabled.
   */
  public function editFormValidate(&$form, &$form_state, $profile) {

    $title_valid = TRUE;

    if (!empty($profile['fields'])) {
      // Checking activity title.
      $field_config = array(
        'entity_type' => 'crm_core_activity',
        'field_name' => 'title',
        'bundle' => $profile['entities']['crm_core_activity']['bundle'],
      );
      $separator = _crm_core_profile_field_separator($field_config);

      // Validation fails only in case the field is hidden end have no default.
      if (!empty($profile['fields'][$separator])
        && !empty($profile['fields'][$separator]['status'])
        && !empty($profile['fields'][$separator]['hidden'])
        && empty($profile['fields'][$separator]['default'])) {
        $title_valid = FALSE;
      }
    }

    if (!$title_valid) {
      form_set_error('', t('You must set default value for activity title if configuring this field as hidden, in other case user will not be able to submit profile form.'));
    }
  }

  /**
   * Assoc array of meta fields which should be handled.
   */
  public function entityProperties() {
    return array(
      'title' => t('CRM Core Activity Title'),
    );
  }

  /**
   * Used to handle submissions from the given entity type.
   */
  public function submitEntity($profile, $form, &$form_state) {
    $activity =& $form_state['crm_core_activity'];

    // Execute submitField method of all field handlers for the activity.
    _crm_core_profile_attach_handler('submitField', 'crm_core_activity', $activity, $profile->fields, $form, $form_state);

    // Perform activity form submit.
    field_attach_submit('crm_core_activity', $activity, $form, $form_state);

    // Title config.
    $title_separator = 'crm_core_activity_' . $activity->bundle . '_title';
    // If activity title field disabled use bundle label.
    if (!isset($profile->fields[$title_separator])
      || empty($profile->fields[$title_separator]['status'])
      || empty($form_state['values'][$title_separator])) {
      $info = entity_get_info('crm_core_activity');
      $activity->title = $info['bundles'][$activity->bundle]['label'];
    }
    else {
      $activity->title = $form_state['values'][$title_separator];
    }
  }

  /**
   * Save entity handler.
   */
  public function saveEntity($profile, $form, &$form_state) {
    // Get the activity information stored in the profile.
    $activity = $form_state['crm_core_activity'];

    // Field_activity_participants.
    $contact = isset($form_state['crm_core_contact']) ? $form_state['crm_core_contact'] : FALSE;
    if (isset($contact) && isset($contact->contact_id)) {
      $activity->field_activity_participants[LANGUAGE_NONE][]['target_id'] = $contact->contact_id;
    }
    else {
      // @todo What should we do if there is no contact supplied?
      $activity->field_activity_participants[LANGUAGE_NONE][]['target_id'] = 1;
    }

    // Save the contact record.
    crm_core_activity_save($activity);
  }

  /**
   * Responses to profile rendering.
   */
  public function execute($profile, $entity_type, $preload, &$form, &$form_state) {

    $entity_config = $profile->entities[$entity_type];
    // We are never going to be loading an existing meeting as part of a
    // profile. This means we can always create a new meeting of the specified
    // type.
    $activity = entity_create($entity_type, array(
      'type' => $entity_config['bundle'],
      'bundle' => $entity_config['bundle'],
    ));

    $form_state[$entity_type] = $activity;

    $entity_properties = $this->entityProperties();
    foreach ($entity_properties as $property_key => $property_label) {
      $field_separator = $entity_type . '_' . $entity_config['bundle'] . '_' . $property_key;
      if (array_key_exists($field_separator, $profile->fields) && $profile->fields[$field_separator]['status']) {
        $field_config = $profile->fields[$field_separator];
        $activity_type = crm_core_activity_types($field_config['bundle']);
        $property_label = t('@activity title', array('@activity' => $activity_type->label));
        switch ($field_config['field_name']) {
          case 'title':
            $form[$field_separator] = array(
              '#type' => empty($field_config['hidden']) ? 'textfield' : 'hidden',
              '#title' => $property_label,
              '#required' => TRUE,
              '#weight' => $field_config['weight'],
              '#default_value' => $field_config['default'],
            );
            break;
        }
      }
    }

    // Attach activity form to the profile page.
    field_attach_form($entity_type, $activity, $form, $form_state);

    // Execute renderField method of all field handlers for the contact.
    _crm_core_profile_attach_handler('renderField', $entity_type, $activity, $profile->fields, $form, $form_state);
  }
}

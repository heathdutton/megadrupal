<?php

/**
 * @file
 * Contact entity handler.
 */

/**
 * Profile entity handlers are used to add fields from different entities into a single
 * CRM Core Profile.
 */
class CRMCoreContactHandler extends ProfileEntityHandler {

  public function entityInfo() {
    return array(
      'entity' => 'crm_core_contact',
      'entity_label' => t('CRM Core Contact'),
    );
  }

  /**
   * Assoc array of meta fields which should be handled.
   *
   * @return array
   */
  public function entityProperties() {
    return array();
  }

  public function entityPropertiesValidate($property) {
  }

  public function entityPropertiesSubmit($property) {
  }

  /**
   * Provides configuration information for the entity within CRM Core Profile.
   */
  public function configEntityForm($profile, &$form) {

    $entity_info = $this->entityInfo();

    $entity_type = $entity_info['entity'];
    $entity_label = $entity_info['entity_label'];
    $entity_config = isset($profile->entities[$entity_type]) ? $profile->entities[$entity_type] : array();

    // handles contact matching for contact entities submitted through crm core profile
    if (module_exists('crm_core_match')) {
    
      $form['match'] = array(
        '#type' => 'fieldset',
        '#title' => t('Contact Matching'),
        '#collapsible' => TRUE,
      );
  
      $form['match']['description_wrapper'] = array(
        '#type' => 'container',
      );
      $form['match']['description_wrapper']['description'] = array(
        '#markup' => t('You can control how CRM Core identifies duplicate contacts when this form is submitted.'),
      );
  
      $form['match']['contact_match'] = array(
        '#type' => 'radios',
        '#title' => t('Type of contact matching for this profile'),
        '#title_display' => 'invisible',
        '#options' => array(
          'default' => t('Apply default contact matching rules to this profile'),
          'no_matching' => t('Do not apply contact matching rules to this profile'),
          'selected' => t('Select matching engines to apply'),
        ),
        '#default_value' => $profile->contact_match,
      );
  
      $engines = crm_core_match_get_engines();
      $options = array();
      foreach ($engines as $engine) {
        $options[$engine->getMachineName()] = $engine->getName();
      }
      $form['match']['contact_match_engines'] = array(
        '#type' => 'select',
        '#title' => t('Selected matching engines'),
        '#title_display' => 'invisible',
        '#options' => $options,
        '#multiple' => TRUE,
        '#default_value' => $profile->contact_match_engines,
        '#states' => array(
          'visible' => array(
            ':input[name="contact_match"]' => array('value' => 'selected'),
          ),
        ),
      );
    }    
    
    // handles form prepopulation in CRM Core Profile
    $form['prepopulate_wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t('Prepopulate %entity Forms Values', array('%entity' => $entity_label)),
      '#collapsible' => TRUE,
    );

    $form['prepopulate_wrapper']['description_wrapper'] = array(
      '#type' => 'container',
    );
    $prepopulate_description = 'When a user is logged in, CRM Core Profile can prepopulate this form using contact information.';
    $form['prepopulate_wrapper']['description_wrapper']['description'] = array(
      '#markup' => t($prepopulate_description),
    );

    $form['prepopulate_wrapper'][$entity_type] = array(
      '#type' => 'container',
      '#tree' => TRUE,
    );

    $form['prepopulate_wrapper'][$entity_type]['prepopulate'] = array(
      '#type' => 'checkbox',
      '#title' => t('Prepopulate form values within this profile'),
      '#default_value' => isset($entity_config['prepopulate']) ? $entity_config['prepopulate'] : FALSE,
    );

    // Set options for prepopulation. crm_core_user_sync must be on if we are
    // going to prepopulate based on the current user.
    $prepopulate_options = array();
    if (module_exists('crm_core_user_sync')) {
      $prepopulate_options['current_user'] = t('Currently logged in user');
      $prepopulate_options['token_user'] = t('A user account passed as a token through the URL');
    }
    $prepopulate_options['token_contact'] = t('A contact id passed as a token through the URL');

    $form['prepopulate_wrapper'][$entity_type]['prepopulate_type'] = array(
      '#type' => 'radios',
      '#title' => t('Type of prepopulation for this profile'),
      '#title_display' => 'invisible',
      '#options' => $prepopulate_options,
      '#default_value' => isset($entity_config['prepopulate_type']) ? $entity_config['prepopulate_type'] : '',
      '#states' => array(
        'visible' => array(
          ':input[name="' . $entity_type . '[prepopulate]"]' => array('checked' => TRUE),
        ),
      ),
    );
  }
  
  /**
   * @see ProfileEntityHandler::configEntityFormValidate()
   */
  public function configEntityFormValidate(&$form, &$form_state) {

    $values = $form_state['values'];
    $entity_info = $this->entityInfo();
    $entity_type = $entity_info['entity'];
    $entity_config = $form_state['values'][$entity_type];

    if (!empty($values['contact_match']) && $values['contact_match'] == 'selected' && empty($values['contact_match_engines'])) {
      form_set_error('contact_match_engines', t('You must select at least one matching engine.'));
    }

    // Prepopulation is possible only with page defined.
    if ($entity_config['prepopulate'] && $entity_config['prepopulate_type'] == 'token_user' && !$values['page']) {
      form_set_error($entity_type, t('To use prepopulation from token feature you must specify a valid page path with token within it.'));
      form_set_error('page');
      form_set_error('page_path');
    }

    if ($values['page'] && !empty($values['page_path'])) {
      $page_path = $values['page_path'] = trim($values['page_path'], '/ ');
      $path_arr = explode('/', $page_path);
      $count = 0;
      foreach ($path_arr as $part) {
        if ($part == '%user' || $part == '%crm_core_contact') {
          $count++;
        }
      }

      // If prepopulation is set we need to check for a token in the path.
      if ($entity_config['prepopulate'] && $entity_config['prepopulate_type'] == 'token_user' && $count > 1) {
        form_set_error('page_path', t('Check your path. There should be only one "%user" token.'));
      }
      elseif ($entity_config['prepopulate'] && $entity_config['prepopulate_type'] == 'token_contact' && $count > 1) {
        form_set_error('page_path', t('Check your path. There should be only one "%user" token.'));
      }
      elseif (!$entity_config['prepopulate'] && $count > 0) {
        form_set_error('page_path', t('Check your path. There should be no "%contact" or "%user" token, unless you are prepopulating the form.'));
      }
    }
  }

  /**
   * Used to validate entity submissions from CRM Core Profile.
   */
  public function validateEntity($profile, $form, &$form_state) {
  }

  /**
   * Used to handle submissions from the given entity type.
   */
  public function submitEntity($profile, $form, &$form_state) {

    $presave = & drupal_static(get_class($this) . __FUNCTION__);

    if (!isset($presave->processed)) {
      $presave = new stdClass();
      $presave->modules = module_implements('crm_core_profile_contact_presave');
      $presave->processed = 1;
    }

    // Get the contact information stored in the profile.
    $contact = &$form_state['crm_core_contact'];

    // Execute submitField method of all field handlers for the contact.
    _crm_core_profile_attach_handler('submitField', 'crm_core_contact', $contact, $profile->fields, $form, $form_state);

    // Perform contact form submit.
    field_attach_submit('crm_core_contact', $contact, $form, $form_state);

    // Now we decide what to do with the contact.
    if ($profile->contact_match != '' && $profile->contact_match != 'no_matching') {
      _crm_core_profile_check_matches($contact, $form_state);
    }

    // Before saving a contact through a profile, give other modules a chance
    // to modify the contact record.
    $values = array(
      'contact' => &$contact,
    );
    foreach ($presave->modules as $module) {
      module_invoke($module, 'crm_core_profile_contact_presave', $values);
    }

    // Save the contact record.
    crm_core_contact_save($contact);
  }

  public function execute($profile, $entity_type, $preload, &$form, &$form_state) {

    global $user;

    $entity_config = $profile->entities[$entity_type];
    
    // We need to handle prepopulation differently,depending on the
    // kind that is selected.
    $prepopulate = isset($profile->entities[$entity_type]['prepopulate']) ? $profile->entities[$entity_type]['prepopulate'] : FALSE;
    $prepopulate_type = isset($profile->entities[$entity_type]['prepopulate_type']) ? $profile->entities[$entity_type]['prepopulate_type'] : '';
    if ($prepopulate == 1) {

      switch ($prepopulate_type) {

        case "current_user":
          $contact = crm_core_user_sync_get_contact_from_uid($user->uid);
          $this->checkContact($contact, $entity_config);
          break;

        case "token_user":
          $contact = crm_core_user_sync_get_contact_from_uid($preload->uid);
          $this->checkContact($contact, $entity_config);
          break;

        case "token_contact":
          $contact = $preload;
          $this->checkContact($contact, $entity_config);
          break;
      }
    }

    $this->checkContact($contact, $entity_config);

    $form_state[$entity_type] = $contact;

    // Attach contact form to the profile page.
    field_attach_form($entity_type, $contact, $form, $form_state);

    // Execute renderField method of all field handlers for the contact.
    _crm_core_profile_attach_handler('renderField', $entity_type, $contact, $profile->fields, $form, $form_state);
    
  }

  /**
   * Checks that contact exists.
   *
   * @param object|NULL $contact
   *   CRM Core Contact, if so.
   * @param object $entity_config
   *   CRM Core Contact profile configuration.
   */
  public function checkContact(&$contact, $entity_config) {
    if (empty($contact)) {
      $contact = entity_create('crm_core_contact', array(
        'type' => $entity_config['bundle'],
        'bundle' => $entity_config['bundle'],
      ));
    }
    else {
      $contact->type = $entity_config['bundle'];
      $contact->bundle = $entity_config['bundle'];
    }
  }

  /**
   * Save entity handler.
   */
  public function saveEntity($profile, $form, &$form_state) {
    // Lets save contact one more time,
    // in case there were some token replacements.
    $form_state['crm_core_contact']->save();
  }
}


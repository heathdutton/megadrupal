<?php

/**
 * {@inheritdoc}
 */
class FlexiformCiviCrmPhone extends FlexiformFormEntityLegacy {


  function __construct(FlexiformFormEntityManagerInterface $manager, $namespace, $getter, $settings = array()) {
    parent::__construct($manager, $namespace, $getter, $settings);

    $this->settings += $settings;
  }

  /**
   * {@inheritdoc}
   */
  public function getEntity() {
    if(empty($this->settings['location_type'])) {
      throw new Exception('Location type ID is required to load a civicrm_phone entity.');
    }
    $base_entity = $this->manager->getBaseEntity();
    $location_type_id = $this->settings['location_type'];

    // Get the address based on the contact_id that we'll get from the base_entity which should be the user.
    $entity = entity_load(
      'civicrm_phone',
      false,
      array(
        'contact_id' => $base_entity->contact_id,
        'location_type_id' => $location_type_id
      )
    );
    // If the user doesn't have a record for this entity yet,
    // we need to stub out a blank one that can be handled via an insert.
    if(!empty($entity)) {
      $entity = reset($entity);
    }
    else {
      $entity = new stdClass();
      $entity->id = 0;
      $entity->is_new = true;
      $entity->location_type_id = $location_type_id;
      $entity->contact_id = $base_entity->contact_id;
    }
    return $entity;
  }

  /**
   * {@inheritdoc}
   */
  public function configForm($form, &$form_state) {

    $form['settings'] = array(
      '#type' => 'container',
      '#tree' => TRUE,
      'location_type' => array(
        '#type' => 'radios',
        '#required' => TRUE,
        '#options' => self::getLocationTypes(),
        '#title' => t('Location type'),
        '#description' => t('Select the location type.'),
        '#default_value' => $this->settings['location_type'],
      )
    );

    $form['actions'] = array(
      '#type' => 'actions',
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Save Settings'),
      )
    );
    return $form;
  }

  private static function getLocationTypes() {
    civicrm_initialize();
    return CRM_Core_PseudoConstant::get('CRM_Core_DAO_Address', 'location_type_id', array(), 'validate');
  }

  public function saveEntity($entity) {
    // Was the phone number cleared out?
    // Then delete the entity instead of saving changes
    // because the "phone" column is required.
    if($entity->id && empty($entity->phone)) {
      entity_delete('civicrm_phone', $entity->id);
    }
    // Only save if the phone number is not empty.
    else if(!empty($entity->phone)) {
      entity_save('civicrm_phone', $entity);
    }
  }
}

<?php

/**
 * {@inheritdoc}
 */
class FlexiformCiviCrmWebsite extends FlexiformFormEntityLegacy {

  function __construct(FlexiformFormEntityManagerInterface $manager, $namespace, $getter, $settings = array()) {
    parent::__construct($manager, $namespace, $getter, $settings);
    if(empty($settings['website_type'])) {
      // Setup a default website type.
      $settings['website_type'] = 1; /* Home */
    }
    $this->settings += $settings;
  }

  /**
   * {@inheritdoc}
   */
  public function getEntity() {
    $getter = $this->getter;
    $base_entity = $this->manager->getBaseEntity();
    $entities = $this->manager->getEntitiesRaw();
    $website_type_id = $this->settings['website_type'];
    // Get the website based on the contact_id that we'll get from the base_entity which should be the user.
    $entity = entity_load(
            'civicrm_website', 
            false, 
            array(
                'contact_id' => $base_entity->contact_id, 
                'website_type_id' => $website_type_id)
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
      $entity->website_type_id = $website_type_id;
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
      'website_type' => array(
        '#type' => 'radios',
        '#required' => TRUE,
        '#options' => self::getWebsiteTypes(),
        '#title' => t('Website type'),
        '#description' => t('Select the website type.'),
        '#default_value' => $this->settings['website_type'],
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

  private static function getWebsiteTypes() {
    civicrm_initialize();
    return CRM_Core_PseudoConstant::get('CRM_Core_DAO_Website', 'website_type_id', array(), 'validate');
  }

  public function saveEntity($entity) {
    entity_save('civicrm_website', $entity);
  }
}

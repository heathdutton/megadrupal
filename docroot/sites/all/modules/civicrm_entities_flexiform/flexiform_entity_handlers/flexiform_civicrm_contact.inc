<?php

/**
 * {@inheritdoc}
 */
class FlexiformCiviCrmContact extends FlexiformFormEntityLegacy {

  function __construct(FlexiformFormEntityManagerInterface $manager, $namespace, $getter, $settings = array()) {
    parent::__construct($manager, $namespace, $getter, $settings);
    $this->settings += $settings;
  }

  /**
   * {@inheritdoc}
   */
  public function getEntity() {
    $base_entity = $this->manager->getBaseEntity();
    $contact = entity_load('civicrm_contact', array($base_entity->contact_id));
    return reset($contact);
  }

  /**
   * {@inheritdoc}
   */
  public function configForm($form, &$form_state) {
    return $form;
  }

  public function saveEntity($entity) {
    $test = 'ss';
    entity_save('civicrm_contact', $entity);
  }

}

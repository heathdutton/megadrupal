<?php

/**
 * @file
 * Default entity handlers for CRM Core Profile.
 */

/**
 * Profile entity handlers are used to add fields from different entities into a single
 * CRM Core Profile.
 */
abstract class ProfileEntityHandler {

  /** 
   * Declares what entities this handler controls within CRM Core Profile.
   * 
   * Returns an array containing the following keys:
   * - entity: Required. The machine name for the entity.
   * - entity_label: Required. A human readable name for the entity, used in settings pages.
   * - add_bundles: Optional. Values are TRUE / FALSE. Tells CRM Core Profile whether or not 
   *     to use bundles when processing the entity. Use this when you do not want users
   *     selecting specific fields for an entity.
   * - edit_callback: Optional. A method in the handler for allowing users to edit this entity
   *     in place of the standard bundle / fields selector. If add bundles is false, and this 
   *     key is not provided, CRM Core Profile will not allow users to add any components to 
   *     the profile. Depending on your situation, this may or may not be a good thing.
   */
  public function entityInfo() {
  }

  /**
   * Validate handler for profile edit form.
   */
  public function editFormValidate(&$form, &$form_state, $profile) {}

  /**
   * Provides the interface for configuring the entity within CRM Core Profile.
   *
   * This function should pass back enough information for CRM Core Profile
   * to add fields from the entity to a profile.
   *
   * Method MUST add element to form with key equals to entity type and #tree
   * property set to TRUE. This element can be wrapped in fieldset for UI
   * consistence.
   */
  public function configEntityForm($profile, &$form) {
  }

  public function configEntityFormValidate(&$form, &$form_state) {
  }

  /**
   * Registers an entity with CRM Core Profile.
   *
   * This function should pass back an array with enough information for CRM Core Profile
   * to know how to store information about the entity in a form and retrieve it when
   * the profile is submitted.
   *
   * the array should contain the following keys:
   * - entity_name: the entity type name.
   *    Example: contact_type
   * - entity_holder: the $form_state variable for holding the entity.
   *    Example: $form_state['contact']
   *
   * @return array
   *  An associative array containing necessary field values
   */
  public function registerEntity() {
    return;
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
  }

  /**
   * Used to save entities from the given entity type.
   *
   * This uses a different function for saving, for proper
   * token support.
   */
  public function saveEntity($profile, $form, &$form_state) {
  }

  /**
   * Renders an entity property configuration row.
   *
   * @return
   *   The field_config used to render the row.
   */
  public function renderPropertyConfig(&$form, $entity_type, $entity_label, $bundle, $field_name) {
    $field_config = array();

    if (isset($form['#profile'])) {
    $profile = $form['#profile'];
    }

    $entity_properties = $this->entityProperties();
    $property_label = $entity_properties[$field_name];

    $default_field_config = array(
      'entity_type' => $entity_type,
      'bundle' => $bundle,
      'field_name' => $field_name,
      'field_item' => '',
      'field_label' => $property_label,
    );
    $field_separator = _crm_core_profile_field_separator($default_field_config);
    if (isset($profile->fields[$field_separator])) {
      $field_config = $profile->fields[$field_separator];
    }
    $field_config += array('handler' => get_class($this));
    $field_config += $default_field_config;

    _crm_core_profile_new_form_fields($form, $entity_label, $field_config);
    return $field_config;
  }

  public function renderField($field_config, $profile, &$form, &$form_state) {
  }

  /**
   * This function is triggered when a CRM Core Profile is being validated.
   */
  public function validateField($field_config, $profile, &$form, &$form_state) {
  }

  /**
   * This function is triggered when a CRM Core Profile is being submitted.
   */
  public function submitField($field_config, $profile, &$form, &$form_state) {
  }
}

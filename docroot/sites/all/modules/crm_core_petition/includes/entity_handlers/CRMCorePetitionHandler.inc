<?php
/**
 * @file
 * Entity handler for CRM Core Petition.
 */

/**
 * Provides signatures counter field for profile forms.
 */
class CRMCorePetitionHandler extends ProfileEntityHandler {

  /**
   * Returns entity info in array.
   */
  public function entityInfo() {
    return array(
      'entity' => 'crm_core_petition',
      'entity_label' => t('CRM Core Petition'),
      'add_bundles' => FALSE,
      'edit_callback' => 'renderEditForm',
    );
  }

  /**
   * Assoc array of meta fields which should be handled.
   *
   * @return array
   *   Array of entity properties.
   */
  public function entityProperties() {
    return array(
      'signatures_counter' => t('Petition signature count'),
    );
  }

  /**
   * Entity edit form builder.
   */
  public function renderEditForm(&$form, $profile, $entity_handler, $entity_info, $entity_label) {
    $entity_type = $entity_info['entity'];
    $entity_bundle = $entity_info['entity'];
    if (isset($form['entities'][$entity_type])) {
      $form['entities'][$entity_type]['status']['#description'] = t('Check this box to add commerce items to your profile.');
      $form['entities'][$entity_type]['bundle']['#value'] = $entity_bundle;
      $form['entities'][$entity_type]['bundle']['#type'] = 'hidden';
      _crm_core_profile_add_field_container($form, $entity_type, TRUE);
      $entity_config = isset($profile->entities[$entity_type]) ? $profile->entities[$entity_type] : array();
      $entity_config['always_display'] = TRUE;
      _crm_core_profile_fields_selector($form, $entity_type, $entity_config, $entity_handler, $entity_label);
    }
  }

  /**
   * Renders property configuration form.
   */
  public function renderPropertyConfig(&$form, $entity_type, $entity_label, $bundle, $field_name) {

    // Call the parent function.
    $field_config = parent::renderPropertyConfig($form, $entity_type, $entity_label, $bundle, $field_name);
    $field_separator = _crm_core_profile_field_separator($field_config);
    $row =& $form['fields'][$field_separator];

    // Override the row for our properties.
    unset($row['hidden']);
    unset($row['default']);

    $row['description_wrapper'] = array(
      '#type' => 'container',
    );
    $row['description_wrapper']['description'] = array(
      '#markup' => t('Use this field to place petition signatures counter in your profile form.'),
    );
  }

  /**
   * Adds commerce items to a profile.
   *
   * @param object $profile
   *   CRM Core Profile.
   * @param string $entity_type
   *   Entity type.
   * @param object $preload
   *   Preloaded user object.
   * @param array $form
   *   Structured form array to update.
   * @param array $form_state
   *   Form state.
   */
  public function execute($profile, $entity_type, $preload, &$form, &$form_state) {

    // Get the configuration for the profile.
    $entity_config = $profile->entities[$entity_type];
    // Lets render our counter.
    if (isset($profile->fields['crm_core_petition_crm_core_petition_signatures_counter'])) {
      $field_config = $profile->fields['crm_core_petition_crm_core_petition_signatures_counter'];
      // Counter enabled?
      if (!empty($field_config['status'])) {
        $petition = menu_get_object();
        // We are viewing petition node.
        if (is_object($petition) && $petition->type == 'cmcp_petition') {
          $form['crm_core_petition_counter_display'] = array(
            '#weight' => $field_config['weight'],
            '#type' => 'item',
            '#markup' => theme('petition_counter', array('nid' => $petition->nid)),
          );
        }
      }
    }
  }
}

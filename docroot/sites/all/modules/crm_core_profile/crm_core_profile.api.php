<?php

/**
 * @file
 * Describes hooks used in CRM Core Profile.
 */

/**
 * Registers an entity handler within CRM Core Profile.
 *
 * This hook is used whenever you want to register an entity handler with CRM
 * Core Profile. The function should pass back an associative array, where the
 * key is a unique name for the entity handler, and the value is the name of the
 * object used to handle the entity.
 */
function hook_crm_core_profile_register_entity() {
  return array(
    'commerce_product' => 'CommerceProfileHandler',
  );
}

/**
 * Fires just before a contact is saved through a CRM Core Profile.
 *
 * This hook is necessary because of the way CRMCoreContactEntity->match works.
 * When checking for duplicates, CRM Core provides the ability to modify
 * a contact record during the matching process. CRM Core Profile will update
 * any contact record values that are passed into the form, but it will not
 * necessarily carry over any changes to contact records that have been made
 * via CRMCoreContactEntity->match.
 *
 * This scenario will not affect profiles that are prepopulated, or profiles
 * that do not use contact matching.
 *
 * Of course, this hook can be used for other reasons as well. Use it any time
 * you need to modify a value being stored as part of a contact record before
 * it is saved. For instance, if you need a store the source of a contact, this
 * might be a good way to do it.
 *
 * This hook takes an associative array of values, which includes the contact
 * record passed by reference. Any modifications to the contact record will be
 * saved when the contact is saved through the profile.
 *
 * @param array $values
 *   An associative array, including a key for the contact record passed by
 * reference.
 */
function hook_crm_core_profile_contact_presave(&$values) {
  $contact = $values['contact'];

  // When modifying a contact record, you could store the modifications in
  // a static variable.
  $my_custom_changes = &drupal_static(__FUNCTION__);

  // And check to make sure they are staged to be committed. This would
  // eliminate any issues with modifications that are being made to
  // existing contacts.
  if (isset($my_custom_changes[$values['contact']->contact_id]) && !isset($values['contact']->my_custom_changes)) {
    $values['contact']->my_custom_changes = $my_custom_changes[$values['contact']->contact_id];
  }
}

/**
 * Fires before token replacement in fields gathered by profile form submission.
 *
 * See token_replace() for $context description.
 *
 * @param array $context
 *   Array of entities keyed by entity specific token type.
 *
 * @see token_replace()
 */
function hook_crm_core_profile_before_token_replace_alter(&$context) {
  if (isset($context['commerce-product'])) {
    unset($context['commerce-product']);
  }
}

/**
 * Fires before profile form is generated to allow inject data.
 *
 * @param object $profile
 *   CRM Core Profile.
 * @param array $data
 *   CRM Core Profile data which will be placed
 * in $form_state['crm_core_profile_data'].
 */
function hook_crm_core_profile_data_alter(&$profile, &$data) {
  $cart_items = array(
    'key' => array(
      'title' => t('Item label'),
      // Decimal amount representing item price.
      'amount' => 100,
    ),
  );

  $data['commerce_items_cart_items'] = $cart_items;
}

/**
 * Fires before all entity handlers executes entity_save().
 *
 * @param object $profile
 *   CRM Core Profile.
 * @param array $form
 *   Complete CRM Core Profile form.
 * @param array $form_state
 *   Form state with all the values and entities.
 */
function hook_crm_core_profile_pre_entity_save_alter(&$profile, &$form, &$form_state) {
}

/**
 * Fires after all entity handlers executes entity_save().
 *
 * @param object $profile
 *   CRM Core Profile.
 * @param array $form
 *   Complete CRM Core Profile form.
 * @param array $form_state
 *   Form state with all the values and entities.
 */
function hook_crm_core_profile_post_entity_save_alter(&$profile, &$form, &$form_state) {
}

/**
 * Fires right before profile get saved to DB.
 *
 * Useful to store additional setting added
 * in hook_form_crm_core_profile_settings_form_alter() or alter the profile
 * itself.
 *
 * @param object $profile
 *   Complete CRM Core Profile object.
 */
function hook_crm_core_profile_pre_save_alter(&$profile) {
}

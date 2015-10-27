<?php

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_redhen_default_rules_configuration() {
  $rules = array();

  /**
   * Define Rule to Create the RedHen Contact for the Order Owner if one does not
   * already exist.
   */
  $contact_types = redhen_contact_type_options_list();

  // If no contact types defined, set a warning message.
  if(!(isset($contact_types))) {
    drupal_set_message(t('There are no RedHen Contact Types defined. This module will not work until you do so <a href="/admin/structure/redhen/contact_types">here.</a>'), 'warning');
  }

  // Get the first Contact type in the list and use it as the default.
  if (isset($contact_types)) {
    reset($contact_types);
    $default_contact_type = key($contact_types);
  }

  $rule = rules_reaction_rule();
  $rule->label = 'If Customer Does Not Have a RedHen Contact, Create One';
  $rule->weight = 3;
  $rule->event('commerce_checkout_complete')
    // If user does not have a RedHen Contact
    ->condition(rules_condition('commerce_redhen_rules_does_user_have_contact', array(
      'user:select' => 'commerce-order:owner',
      ))->negate())
    // Bring in the Billing Profile and Address field from the Order
    ->condition('entity_has_field', array(
      'entity:select' => 'commerce-order',
      'field' => 'commerce_customer_billing',
      ))
    ->condition('entity_has_field', array(
      'entity:select' => 'commerce-order:commerce-customer-billing',
      'field' => 'commerce_customer_address',
      ))
    // Create the a Contact entity if all conditions are met
    ->action('entity_create', array(
      'type' => 'redhen_contact',
      'param_type' => isset($default_contact_type) ? $default_contact_type : NULL,
      'param_first_name:select' => 'commerce-order:commerce-customer-billing:commerce-customer-address:first-name',
      'param_last_name:select' => 'commerce-order:commerce-customer-billing:commerce-customer-address:last-name',
      'param_redhen_state' => 1,
      'entity_created:label' => t('Contact'),
      'entity_created:var' => 'contact',
      ))
    ->action('commerce_redhen_rules_set_email_for_contact', array(
      'contact:select' => 'contact',
      'email:select' => 'commerce-order:mail',
      ))
    ->action('entity_save', array(
      'data:select' => 'contact',
      'immediate' => 1,
      ));

  $rules['commerce_redhen_create_redhen_contact'] = $rule;

  /**
   * Define Rule Component that creates a RedHen Membership based on the product
   * that has been purchased.
   */

  $rule = rule(array(
    'line_item' => array(
      'label' => t('Line Item'),
      'type' => 'commerce_line_item',
    ),
    'user' => array(
      'label' => t('User'),
      'type' => 'user',
    ),
  ));
  $rule->label = 'If Product Grants Membership, Create the RedHen Membership';
  $rule->condition('entity_has_field', array(
    'entity:select' => 'line-item',
    'field' => 'commerce_product',
    ))
    // Ensure the product has the custom fields and they have values
    ->condition('entity_has_field', array(
      'entity:select' => 'line-item:commerce-product',
        'field' => 'commerce_redhen_membership',
    ))
    ->condition(rules_condition('data_is_empty', array(
      'data:select' => 'line-item:commerce-product:commerce-redhen-membership',
    ))->negate())
    ->condition('entity_has_field', array(
      'entity:select' => 'line-item:commerce-product',
      'field' => 'commerce_redhen_interval',
    ))
    ->condition(rules_or()
      ->condition(rules_condition('entity_has_field', array(
        'entity:select' => 'line-item',
        'field' => 'commerce_redhen_organization_ref',
      ))->negate())
      ->condition(rules_and()
        ->condition('entity_has_field', array(
          'entity:select' => 'line-item',
          'field' => 'commerce_redhen_organization_ref',
        ))
        ->condition('data_is_empty', array(
          'data:select' => 'line-item:commerce-redhen-organization-ref',
        ))
      )
    )
    // Pass in the Product and Order Owner, and create the Membership if all conditions pass.
    ->action('commerce_redhen_rules_create_membership_from_product', array(
      'line_item:select' => 'line-item',
      'product:select' => 'line-item:commerce-product',
      'user:select' => 'user',
      ));

  $rules['commerce_redhen_create_membership_from_product_component'] = $rule;

  /**
   * Define Rule Component that creates a RedHen Membership for an Organization based
   * on the product that has been purchased.
   */

  $rule = rule(array(
    'line_item' => array(
      'label' => t('Line Item'),
      'type' => 'commerce_line_item',
    ),
    'user' => array(
      'label' => t('User'),
      'type' => 'user',
    ),
  ));
  $rule->label = 'If Product Grants Membership, Create the RedHen Membership for Organization';
  $rule->condition('entity_has_field', array(
    'entity:select' => 'line-item',
    'field' => 'commerce_redhen_organization_ref',
  ))
    ->condition('entity_has_field', array(
    'entity:select' => 'line-item',
    'field' => 'commerce_product',
  ))
    // Ensure the product has the custom fields and they have values
    ->condition('entity_has_field', array(
      'entity:select' => 'line-item:commerce-product',
      'field' => 'commerce_redhen_membership',
    ))
    ->condition(rules_condition('data_is_empty', array(
      'data:select' => 'line-item:commerce-product:commerce-redhen-membership',
    ))->negate())
    ->condition('entity_has_field', array(
      'entity:select' => 'line-item:commerce-product',
      'field' => 'commerce_redhen_interval',
    ))
    // Pass in the Product and Order Owner, and create the Membership if all conditions pass.
    ->action('commerce_redhen_rules_create_membership_for_organization_from_product', array(
      'line_item:select' => 'line-item',
      'product:select' => 'line-item:commerce-product',
      'user:select' => 'user',
      'organization:select' => 'line-item:commerce-redhen-organization-ref'
    ))
    ->condition(rules_condition('data_is_empty', array(
      'data:select' => 'line-item:commerce-redhen-organization-ref',
    ))->negate());

  $rules['commerce_redhen_create_membership_for_org_from_product_component'] = $rule;


  /**
   * Define Rule to Create the RedHen Membership for the Order Owner.
   */

  $rule = rules_reaction_rule();
  $rule->label = 'Create RedHen Memberships After Checkout';
  $rule->weight = 5;
  $rule->event('commerce_checkout_complete');

  $loop = rules_loop(array(
    'list:select' => 'commerce-order:commerce-line-items',
    'item:var' => 'list_item',
    'item:label' => t('Current list item'),
    'item:type' => 'commerce_line_item',
  ))
    ->action('component_commerce_redhen_create_membership_from_product_component', array(
      'line_item:select' => 'list-item',
      'user:select' => 'commerce-order:owner',
    ))
    ->action('component_commerce_redhen_create_membership_for_org_from_product_component', array(
      'line_item:select' => 'list-item',
      'user:select' => 'commerce-order:owner',
    ));

  $rule->action($loop);

  $rules['commerce_redhen_create_memberships_checkout'] = $rule;

  return $rules;
}

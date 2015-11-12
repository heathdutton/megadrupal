<?php

/**
 * Implements hook_rules_action_info().
 */
function commerce_redhen_rules_action_info() {
  return array(
    'commerce_redhen_rules_create_membership_from_product' => array(
      'label' => t('Create a RedHen Membership Based on Product Data'),
      'parameter' => array(
        'line_item' => array(
          'type' => 'commerce_line_item',
          'label' => t('Line Item'),
          'description' => t('The Line item with the quantity ordered'),
        ),
        'product' => array(
          'type' => 'commerce_product',
          'label' => t('Product'),
          'description' => t('A product set up to grant RedHen Memberships. Configure them at <a href="/admin/commerce/config/redhen">Commerce RedHen Settings</a>.')
        ),
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
          'description' => t('The user who should get the membership.'),
        ),
      ),
      'group' => t('RedHen'),
    ),
    'commerce_redhen_rules_create_membership_for_organization_from_product' => array(
      'label' => t('Create a RedHen Membership for Organization Based on Product Data'),
      'parameter' => array(
        'line_item' => array(
          'type' => 'commerce_line_item',
          'label' => t('Line Item'),
          'description' => t('The Line item with the quantity ordered'),
        ),
        'product' => array(
          'type' => 'commerce_product',
          'label' => t('Product'),
          'description' => t('A product set up to grant RedHen Memberships. Configure them at <a href="/admin/commerce/config/redhen">Commerce RedHen Settings</a>.')
        ),
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
          'description' => t('The user who should get the membership.'),
        ),
        'organization' => array(
          'type' => 'redhen_org',
          'label' => t('Organization'),
          'description' => t('The organization that should get the membership'),
        ),
      ),
      'group' => t('RedHen'),
    ),
    'commerce_redhen_rules_set_email_for_contact' => array(
      'label' => t('Set the default Email for a Redhen Contact'),
      'parameter' => array(
        'contact' => array(
          'type' => 'redhen_contact',
          'label' => t('Contact'),
        ),
        'email' => array(
          'type' => 'text',
          'label' => t('Email'),
          'description' => t('Primary email for the Contact.'),
        ),
      ),
      'group' => t('RedHen'),
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 */
function commerce_redhen_rules_condition_info() {
  return array(
    'commerce_redhen_rules_does_user_have_contact' => array(
      'label' => t('User Has a RedHen Contact Record'),
      'parameter' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
          'description' => t('The user account to check.'),
        ),
      ),
      'group' => t('RedHen'),
    ),
  );
}

/**
 * Implementation of hook_rules_event_info()
 */
function commerce_redhen_rules_event_info() {
  return array(
    'commerce_redhen_rules_granting_redhen_membership' => array(
      'label' => t('RedHen Membership granted during checkout'),
      'module' => 'commerce_redhen',
      'variables' => array(
        'membership' => array(
          'type' => 'redhen_membership',
          'label' => t('RedHen Membership'),
          'description' => t('RedHen Membership Granted via Commerce RedHen')
        ),
        'product' => array(
          'type' => 'commerce_product',
          'label' => t('Product'),
          'description' => t('A product set up to grant RedHen Memberships. Configure them at <a href="/admin/commerce/config/redhen">Commerce RedHen Settings</a>.')
        ),
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
          'description' => t('The user who should get the membership.'),
        ),
      ),
      'group' => t('RedHen'),
    ),
  );
}


/**
 * Action callback for custom rules action. Creates a membership based on the product
 * and user.
 */
function commerce_redhen_rules_create_membership_from_product($line_item, $product, $user, $create_contact, $contact_type) {
  $product_wrapper = entity_metadata_wrapper('commerce_product', $product);
  $membership_type = $product_wrapper->commerce_redhen_membership->name->value();
  if ($product_wrapper->commerce_redhen_interval->value() == NULL || $product_wrapper->commerce_redhen_interval->interval->value() == '0') {
    $interval = NULL;
  }
  else {
    $interval = '+' . $product_wrapper->commerce_redhen_interval->interval->value() . ' ' . $product_wrapper->commerce_redhen_interval->period->value();
  }

  if ($contact = redhen_contact_user_contact($user)) {
    for ($i = 0; $i < $line_item->quantity; $i++) {
      $membership = entity_create('redhen_membership', array('bundle' => $membership_type));
      $membership->type = $membership_type;
      $membership->entity_type = 'redhen_contact';
      $membership->entity_id = $contact->contact_id;
      $membership->start_date = strtotime('-1 second'); // So it is sure to add the role to the user.
      $membership->expire_date = !empty($interval) ? strtotime($interval) : $interval;
      $membership->membership_state = REDHEN_MEMBERSHIP_ACTIVE;
      redhen_membership_save($membership);
      rules_invoke_event('commerce_redhen_rules_granting_redhen_membership', $membership, $product, $user);
    }
  } else {
    // If there is no contact, we need to alert them.
    drupal_set_message(t('You do not yet have a Contact record associated with your account. Please contact the site administrator.'), 'error');
  }
}

/**
 * Action callback for custom rules action. Creates a membership for an organization based on the product
 * and user.
 */
function commerce_redhen_rules_create_membership_for_organization_from_product($line_item, $product, $user, $organization, $create_contact, $contact_type) {
  $product_wrapper = entity_metadata_wrapper('commerce_product', $product);
  $membership_type = $product_wrapper->commerce_redhen_membership->name->value();
  if ($product_wrapper->commerce_redhen_interval->value() == NULL || $product_wrapper->commerce_redhen_interval->interval->value() == '0') {
    $interval = NULL;
  }
  else {
    $interval = '+' . $product_wrapper->commerce_redhen_interval->interval->value() . ' ' . $product_wrapper->commerce_redhen_interval->period->value();
  }

  if (!empty($organization)) {
    for ($i = 0; $i < $line_item->quantity; $i++) {
      $membership = entity_create('redhen_membership', array('bundle' => $membership_type));
      $membership->type = $membership_type;
      $membership->entity_type = 'redhen_org';
      $membership->entity_id = $organization->org_id;
      $membership->start_date = strtotime('-1 second'); // So it is sure to add the role to the user.
      $membership->expire_date = !empty($interval) ? strtotime($interval) : $interval;
      $membership->membership_state = REDHEN_MEMBERSHIP_ACTIVE;
      redhen_membership_save($membership);
      rules_invoke_event('commerce_redhen_rules_granting_redhen_membership', $membership, $product, $user);
    }
  }
  else {
    // If there is no contact, we need to alert them.
    drupal_set_message(t('You do not have an Organization record selected for your order. Please contact the site administrator.'), 'error');
  }
}

function commerce_redhen_rules_set_email_for_contact($contact, $email) {
  redhen_contact_property_email_set($contact, '', $email);
}

/**
 * Defines the options list for the Create Contact setting.
 */
function commerce_redhen_contact_rules_options_list() {
  return array(
    0 => t('No'),
    1 => t('Yes'),
  );
}

/**
 * Condition callback for checking if a user has a RedHen Contact
 */

function commerce_redhen_rules_does_user_have_contact($user) {
  if (redhen_contact_user_contact($user)) {
    return TRUE;
  }

  return FALSE;
}

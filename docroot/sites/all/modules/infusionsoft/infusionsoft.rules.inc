<?php
/**
 * @file
 * Rules for Infusionsoft Integration
 */

/**
 * Implements hook_rules_event_info().
 */
function infusionsoft_rules_event_info() {
  $items = array(
    'infusionsoft_contact_add' => array(
      'label' => t('After creating a new Infusionsoft contact'),
      'group' => t('Infusionsoft'),
      'variables' => array(
        'user' => array('type' => 'user', 'label' => 'Drupal User'),
      ),
    ),
	'infusionsoft_contact_update' => array(
      'label' => t('After updating an Infusionsoft contact'),
      'group' => t('Infusionsoft'),
      'variables' => array(
        'user' => array('type' => 'user', 'label' => 'Drupal User'),
      ),
    ),
  );

  return $items;
}

/**
 * Implements hook_rules_action_info().
 */
function infusionsoft_rules_action_info() {
  $actions = array();

  $actions['infusionsoft_opt_in'] = array(
    'label' => t('Opt in a Contact to the Email Service'),
    'parameter' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('The Drupal User'),
      ),
	  'opt_in_reason' => array(
	    'type' => 'text',
		'label' => t('Contact opt-in reason'),
	  ),
    ),
    'group' => t('Infusionsoft'),
    'callbacks' => array(
      'execute' => 'infusionsoft_rules_opt_in',
    ),
  );

  $actions['infusionsoft_add_contact'] = array(
    'label' => t('Add a Contact'),
    'parameter' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('The Drupal User'),
      ),
    ),
    'group' => t('Infusionsoft'),
    'callbacks' => array(
      'execute' => 'infusionsoft_rules_add_contact',
    ),
  );

  $actions['infusionsoft_rules_delete_contact'] = array(
    'label' => t('Delete a Contact'),
    'parameter' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('The Drupal User'),
      ),
    ),
    'group' => t('Infusionsoft'),
    'callbacks' => array(
      'execute' => 'infusionsoft_rules_delete_contact',
    ),
  );

  $actions['infusionsoft_add_contact_group'] = array(
    'label' => t('Add a Contact to a Group'),
    'parameter' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('The Drupal User'),
      ),
      'infusionsoft_group_id' => array(
        'type' => 'integer',
        'label' => t('The Infusionsoft Group'),
        'options list' => 'infusionsoft_groups_options',
      ),
    ),
    'group' => t('Infusionsoft'),
    'callbacks' => array(
      'execute' => 'infusionsoft_rules_add_to_group',
    ),
  );

  $actions['infusionsoft_remove_contact_group'] = array(
    'label' => t('Remove a Contact from a Group'),
    'parameter' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('The Drupal User'),
      ),
      'infusionsoft_group_id' => array(
        'type' => 'integer',
        'label' => t('The Infusionsoft Group'),
        'options list' => 'infusionsoft_groups_options',
      ),
    ),
    'group' => t('Infusionsoft'),
    'callbacks' => array(
      'execute' => 'infusionsoft_rules_remove_from_group',
    ),
  );

  $actions['infusionsoft_add_contact_campaign'] = array(
    'label' => t('Add a Contact to a campaign'),
    'parameter' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('The Drupal User'),
      ),
      'campaign' => array(
        'type' => 'integer',
        'label' => t('The Infusionsoft Campaign'),
        'options list' => 'infusionsoft_campaigns_options',
      ),
    ),
    'group' => t('Infusionsoft'),
    'callbacks' => array(
      'execute' => 'infusionsoft_rules_add_to_campaign',
    ),
  );


  return $actions;
}

/**
 * Rules action: Opt in User to Email Service
 *
 * @param $account
 */
function infusionsoft_rules_opt_in($account, $reason) {
  infusionsoft_contact_opt_in($account->mail, $reason);
}

/**
 * Rules action: Add a contact
 */
function infusionsoft_rules_add_contact($account) {
  infusionsoft_contact_id($account);
}

/**
 * Rules action: Delete a contact
 */
function infusionsoft_rules_delete_contact($account) {
  if (!empty($account->field_infusionsoft_id[LANGUAGE_NONE][0]['value'])) {
    $infusionsoft_id = $account->field_infusionsoft_id[LANGUAGE_NONE][0]['value'];
    infusionsoft_contact_delete($infusionsoft_id);
  }
}

/**
 * Rules action: adds a user to a group
 */
function infusionsoft_rules_add_to_group($account, $group_id) {
  infusionsoft_user_op('add', 'group', infusionsoft_contact_id($account), $group_id);
}

/**
 * Rules action: remove a user from a group
 */
function infusionsoft_rules_remove_from_group($account, $group_id) {
  infusionsoft_user_op('remove', 'group', infusionsoft_contact_id($account), $group_id);
}

/**
 * Rules action: adds a user to a campaign.
 */
function infusionsoft_rules_add_to_campaign($account, $campaign_id) {
  infusionsoft_user_op('add', 'campaign', infusionsoft_contact_id($account), $campaign_id);
}

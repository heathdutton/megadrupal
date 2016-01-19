<?php
/**
 * @file
 * Describe hooks provided by the Paywall field module.
 */

/**
 * Alter the message displayed on a given paywall field.
 *
 * Use any html markup for $message.
 *
 * @param string $message
 *   The paywall message to change.
 * @param string $paywall_name
 *   The field name of the paywall to change.
 * @param string $entity_type
 *   The type of entity the paywall is being rendered on.
 * @param string $entity_id
 *   The entity the paywall is being rendered on.
 */
function hook_field_paywall_message_alter(&$message, $paywall_name, $entity_type, $entity_id) {
  if ($paywall_name == 'field_custom_paywall') {
    $message = t('You cannot read this content, !click here to sign up now.', array(
      '!click' => l(t('Click'), 'user/signup'),
    ));
  }
}

/**
 * Alter the roles the paywall is triggered by.
 *
 * @param array $roles
 *   The paywall trigger roles to change.
 * @param string $paywall_name
 *   The field name of the paywall to change.
 * @param string $entity_type
 *   The type of entity the paywall is being rendered on.
 * @param string $entity_id
 *   The entity the paywall is being rendered on.
 */
function hook_field_paywall_roles_alter(&$roles, $paywall_name, $entity_type, $entity_id) {
  if ($paywall_name == 'field_custom_paywall' && $entity_type == 'node') {
    $node = node_load($entity_id);

    // Give anonymous users access.
    if ($node->type == 'article') {
      $roles[1] = 0;
    }
  }
}

/**
 * Alter the fields hidden by the paywall.
 *
 * @param array $fields_hidden
 *   The fields the paywall hides.
 * @param string $paywall_name
 *   The field name of the paywall to change.
 * @param string $entity_type
 *   The type of entity the paywall is being rendered on.
 * @param string $entity_id
 *   The entity the paywall is being rendered on.
 */
function hook_field_paywall_fields_hidden_alter(&$fields_hidden, $paywall_name, $entity_type, $entity_id) {
  if ($paywall_name == 'field_custom_paywall' && $entity_type == 'node') {
    $node = node_load($entity_id);

    // Hide the body field on page nodes.
    if ($node->type == 'page') {
      $fields_hidden['field_body'] = 1;
    }
  }
}

/**
 * Alter whether or not the paywall should be enabled.
 *
 * @param bool $enabled
 *   Whether or not the paywall should be enabled.
 * @param string $paywall_name
 *   The field name of the paywall to change.
 * @param string $entity_type
 *   The type of entity the paywall is being rendered on.
 * @param string $entity_id
 *   The entity the paywall is being rendered on.
 */
function hook_field_paywall_enabled_alter(&$enabled, $paywall_name, $entity_type, $entity_id) {
  if ($paywall_name == 'field_custom_paywall' && $entity_type == 'node') {
    $node = node_load($entity_id);

    // Disable paywall for promoted articles.
    if ($node->type == 'article' && $node->promoted == 1) {
      $enabled = FALSE;
    }
  }
}

/**
 * Alter all the final paywall settings before the paywall is activated.
 *
 * @param array $paywall_settings
 *   All the processed settings of the paywall.
 * @param string $paywall_name
 *   The field name of the paywall to change.
 * @param string $entity_type
 *   The type of entity the paywall is being rendered on.
 * @param string $entity_id
 *   The entity the paywall is being rendered on.
 */
function hook_field_paywall_paywall_alter(&$paywall_settings, $paywall_name, $entity_type, $entity_id) {
  if ($paywall_name == 'field_custom_paywall' && $entity_type == 'node') {
    $node = node_load($entity_id);

    if ($paywall_settings['enabled'] == 1 && $node->type == 'page') {
      watchdog('field_paywall', 'Paywall activated for user.');
    }
  }
}

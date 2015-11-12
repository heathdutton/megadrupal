<?php

/**
 * @file
 * Plugin to provide access control based upon role membership.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("User: role UUID"),
  'description' => t('Control access by role UUID.'),
  'callback' => 'uuidrole_role_uuid_access_check',
  'default' => array('uuids' => array()),
  'settings form' => 'uuidrole_role_uuid_access_settings',
  'settings form submit' => 'uuidrole_role_uuid_access_settings_submit',
  'summary' => 'uuidrole_role_uuid_access_summary',
  'required context' => new ctools_context_required(t('User'), 'user'),
);

/**
 * Settings form for the 'by role' access plugin
 */
function uuidrole_role_uuid_access_settings($form, &$form_state, $conf) {
  $form['settings']['uuids'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Role'),
    '#default_value' => $conf['uuids'],
    '#options' => uuidrole_roles(),
    '#description' => t('Only the checked roles will be granted access.'),
  );
  return $form;
}

/**
 * Compress the roles allowed to the minimum.
 */
function uuidrole_role_uuid_access_settings_submit($form, &$form_state) {
  $form_state['values']['settings']['uuids'] = array_keys(array_filter($form_state['values']['settings']['uuids']));
}

/**
 * Check for access.
 */
function uuidrole_role_uuid_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  if (empty($context) || empty($context->data) || !isset($context->data->roles)) {
    return FALSE;
  }

  $roles = array_keys($context->data->uuidroles);
  return (bool) array_intersect($conf['uuids'], $roles);
}

/**
 * Provide a summary description based upon the checked roles.
 */
function uuidrole_role_uuid_access_summary($conf, $context) {
  if (!isset($conf['uuids'])) {
    $conf['uuids'] = array();
  }
  $roles = uuidrole_roles();

  $names = array();
  foreach (array_filter($conf['uuids']) as $uuid) {
    $names[] = check_plain($roles[$uuid]);
  }

  if (empty($names)) {
    return t('@identifier can have any role', array('@identifier' => $context->identifier));
  }

  return format_plural(count($names), '@identifier has role UUID "@roles"', '@identifier has one of "@roles"', array('@roles' => implode(', ', $names), '@identifier' => $context->identifier));
}


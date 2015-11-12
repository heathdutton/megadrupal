<?php

/**
 * @file
 * Provides settings pages for the CAS Attributes module.
 */

/**
 * Administrative settings form.
 */
function cas_roles_admin_settings() {
  // Get current settings, and add default options.
  $sync_every_login = variable_get('cas_roles_sync_every_login', 1);
  $behavior = variable_get('cas_roles_behavior', CAS_ROLES_MATCH_REGEX);
  $roles = variable_get('cas_roles_roles');
  $require_a_role_create = variable_get('cas_roles_require_a_role_create', FALSE);
  $require_a_role_login = variable_get('cas_roles_require_a_role_login', FALSE);
  $relations = variable_get('cas_roles_relations', array('2' => NULL));

  $form['cas_roles_sync_every_login'] = array(
    '#type' => 'radios',
    '#title' => t('Fetch CAS Roles'),
    '#default_value' => $sync_every_login,
    '#options' => array(
      0 => t('only when a CAS account is created (i.e., the first login of a CAS user).'),
      1 => t('every time a CAS user logs in.'),
    ),
    '#weight' => -10,
  );

  $form['cas_roles_behavior'] = array(
    '#type' => 'radios',
    '#title' => t('CAS vs Drupal roles'),
    '#default_value' => $behavior,
    '#options' => array(
      CAS_ROLES_DRUPAL_ROLES_ONLY => t("Only assign roles which are present in drupal and match, remove user roles not present in CAS."),
      CAS_ROLES_CREATE_NEW_ROLES => t("Create roles which don't exits in Drupal, remove user roles not present in CAS."),
      CAS_ROLES_MATCH_REGEX => t("Match roles with regular expressions."),
    ),
    '#weight' => -9,
  );

  $form['cas_roles_roles'] = array(
    '#type' => 'textfield',
    '#title' => t('Attribute for roles'),
    '#default_value' => isset($roles) ? $roles : '',
    '#size' => 50,
    '#description' => t('CAS Attributes may be arrays, use the tokens syntax starting with "[cas:attribute:" global tokens are also replaced.<br />Example: [cas:attribute:drupal_roles]'),
    '#weight' => -8,
  );

  $form['cas_roles_require_a_role_create'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require at least one CAS managed role for new users'),
    '#default_value' => $require_a_role_create,
    '#description' => t('New users will only be added to the system if at least one CAS managed role matches (one of the fields below which is not empty)'),
    '#weight' => -7,
  );

  $form['cas_roles_require_a_role_login'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require at least one CAS managed role for any user to log in'),
    '#default_value' => $require_a_role_login,
    '#description' => t('Users will be denied access if not at least one CAS managed role matches (one of the fields below which is not empty)'),
    '#weight' => -7,
  );

  $form['cas_roles_relations'] = array(
    '#type' => 'fieldset',
    '#title' => t('CAS roles mappings'),
    'relations' => array(),
    '#tree' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="cas_roles_behavior"]' => array('value' => CAS_ROLES_MATCH_REGEX),
      ),
    ),
    '#weight' => -6,
  );

  $form['cas_roles_relations']['help'] = array(
    '#markup' => t('Regular expression to map <a href="@url">user roles</a>. The role is assigned if one of the roles in the attribute array matches the expression. An empty field means the role is not administrated by CAS.', array('@url' => url('admin/people/permissions/roles'))),
  );

  $authenticated_role = user_roles(TRUE);
  $authenticated_role = $authenticated_role[DRUPAL_AUTHENTICATED_RID];
  $form['cas_roles_relations'][$authenticated_role] = array(
    '#type' => 'textfield',
    '#title' => t('authenticated user'),
    '#default_value' => isset($relations[$authenticated_role]) ? $relations[$authenticated_role] : '',
    '#size' => 50,
    '#maxlength' => 1024,
    '#element_validate' => array('_cas_roles_preg_validate'),
    '#states' => array(
      'visible' => array(
        array(':input[name="cas_roles_require_a_role_create"]' => array('checked' => TRUE)),
        array(':input[name="cas_roles_require_a_role_login"]' => array('checked' => TRUE)),
      ),
    ),
    '#description' => t('The "authenticated user" role will be assigned to all users that are allowed to log in.'),
  );

  foreach (cas_roles_cutsom_user_roles() as $rid => $role) {
    $form['cas_roles_relations'][$role] = array(
      '#type' => 'textfield',
      '#title' => $role,
      '#default_value' => isset($relations[$role]) ? $relations[$role] : '',
      '#size' => 50,
      '#maxlength' => 1024,
      '#element_validate' => array('_cas_roles_preg_validate'),
    );
  }

  return system_settings_form($form);
}

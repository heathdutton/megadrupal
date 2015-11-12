<?php

/**
 * @file
 * Administrative settings for user taxonomy terms.
 */

/**
 * Form constructor for the private taxonomy form.
 */
function private_taxonomy_form($form, &$form_state) {
  $form = array();

  $form['deprecate'] = array(
    '#type' => 'fieldset',
    '#title' => t('Disable changes to core widgets'),
  );

  $form['deprecate']['private_taxonomy_deprecate'] = array(
    '#type' => 'checkbox',
    '#title' => '&nbsp;' . t("Leave unchecked if you require restricting terms to the user's terms in the core select and checkbox widgets."),
    '#default_value' => variable_get('private_taxonomy_deprecate', 0),
  );

  $form['new_users'] = array(
    '#type' => 'fieldset',
    '#title' => t('New users'),
  );

  $form['new_users']['private_taxonomy_default_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => '&nbsp;' . t('Enable creation of default terms for new users.'),
    '#default_value' => variable_get('private_taxonomy_default_enabled', 0),
  );

  $form['install'] = array(
    '#type' => 'fieldset',
    '#title' => t('Existing users'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['install']['private_taxonomy_user_name'] = array(
    '#type' => 'textfield',
    '#title' => t('User terms to clone'),
    '#size' => 15,
    '#default_value' => variable_get('private_taxonomy_user_name', 'admin'),
    '#description' => t('User name with terms that are to be cloned for other users.'),
  );

  $user_name = variable_get('private_taxonomy_user_name', 'admin');
  $sql = 'SELECT COUNT(*) FROM {user_term} user_term
    LEFT JOIN {users} users ON user_term.uid=users.uid
    WHERE users.name!=:name';
  if (db_query($sql, array(':name' => $user_name))->fetchField() == 0) {
    $form['install']['existing_users'] = array(
      '#type' => 'button',
      '#value' => t('Create default taxonomies'),
      '#prefix' => '<p>Create default taxonomies for existing users.<br/>',
      '#suffix' => '</p>',
    );

    drupal_add_js(drupal_get_path('module', 'private_taxonomy') .
                  '/private_taxonomy.js');
  }
  else {
    $form['install']['installed'] = array(
      '#markup' => t('Users with private terms already exist.'),
    );
  }

  return system_settings_form($form);
}

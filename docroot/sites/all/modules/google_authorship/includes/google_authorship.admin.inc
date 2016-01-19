<?php

/**
 * @file
 * Defines admin configuration page for the Google Authorship module
 * */

/**
 * Adds an administration page for this module.
 */
function google_authorship_admin() {
  $form = array();

  if (module_exists('metatag')) {
    $form['google_authorship_use_metatag'] = array(
      '#type' => 'checkbox',
      '#title' => t('Only use the Metatag module?'),
      '#default_value' => variable_get('google_authorship_use_metatag', 0),
      '#description' => t('Enable this in order to use the Metatag module exclusively to add Google Authorship metadata info to node pages and not rewrite username links. This is helpful if your theme overrides the default author and date display.'),
    );
  }

  $form['google_authorship_use_head'] = array(
    '#type' => 'checkbox',
    '#title' => t('Put Google Authorship Information in page head instead of relinking the author username on node pages?'),
    '#default_value' => variable_get('google_authorship_use_head', 0),
    '#description' => t('Enable this if you want to still be able to link from usernames to user account pages. This is also useful if your theme overrides the username link. NOTE: It is best NOT to use this with the Metatag option selected above, as it will create duplicate links. Please also ensure that Metatag is not already adding author information that may conflict with this.'),
  );

  $form['google_authorship_use_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Link to the author\'s user page as bio?'),
    '#default_value' => variable_get('google_authorship_use_user', 0),
    '#description' => t('Enable this if you use user accounts for bio pages. This will use "rel=\'author\'" in the author\'s user account link and add a "rel=\'me\' to the user account page head.'),
  );

  $form['google_authorship_registration_disp'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide Google+ ID field on user registration form?'),
    '#default_value' => variable_get('google_authorship_registration_disp', 0),
    '#description' => t('Enable this if you do not want users to be able to input their Google+ ID when registering a new user account.'),
  );

  $bundles = array();

  foreach (field_info_bundles('node') as $bundle => $bundle_info) {
    $bundles[$bundle] = $bundle_info['label'];
  }

  $form['google_authorship_bundles'] = array(
    '#type' => 'select',
    '#title' => t('Display Google Authorship metadata on:'),
    '#default_value' => variable_get('google_authorship_bundles'),
    '#description' => t('This determines which bundles will have Google Authorship metadata added to their markup. Making no selection will display Google Authorship for every bundle. Hold Control (PC) or Command (Mac) key down to select multiple bundles.'),
    '#options' => $bundles,
    '#multiple' => TRUE,
  );

  $roles = array();

  foreach (user_roles(TRUE) as $role_id => $role_name) {
    $roles[$role_id] = $role_name;
  }

  $form['google_authorship_disp_for_roles'] = array(
    '#type' => 'select',
    '#title' => t('Display Google Authorship for authors with these roles:'),
    '#default_value' => variable_get('google_authorship_disp_for_roles'),
    '#description' => t('This determines which authors will have Google Authorship metadata added to their markup, based on the author\'s role. Making no selection will display Google Authorship for every role. Hold Control (PC) or Command (Mac) key down to select multiple bundles.'),
    '#options' => $roles,
    '#multiple' => TRUE,
  );

  return system_settings_form($form);
}
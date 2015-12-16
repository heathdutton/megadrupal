<?php
/**
 * @file
 * Administration page for the 'Fieldset helper' module.
 */
/**
 * Administration page for the 'Fieldset helper' module.
 */
function fieldset_helper_admin_settings() {

  // General
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );

  // General: Node forms (add and edit) state
  $form['general']['fieldset_helper_node_form_state'] = array(
    '#type' => 'checkbox',
    '#title' => t("Save fieldset state on node add (%node_add) and node edit (%node_edit) as one page (node/form).", array('%node_add' => 'node/add/*', '%node_edit' => 'node/*/edit')),
    '#default_value' => variable_get('fieldset_helper_node_form_state', 1),
  );

  // General: Default collapsible pages
  $form['general']['fieldset_helper_default_collapsible_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Default fieldsets to be collapsible on specific pages'),
    '#default_value' => variable_get('fieldset_helper_default_collapsible_pages', '*'),
    '#description' => t("To enable collapsible fieldsets for every page enter just the wildcard character '*'") . '<br/>' .
      t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %modules for the modules page and %user_settings for the user settings page.", array('%modules' => 'admin/modules', '%users_settings' => 'admin/user/settings')),
  );

  // General: Default collapsible pages
  $form['general']['fieldset_helper_default_collapsed_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Default fieldsets to be collapsed on specific pages'),
    '#default_value' => variable_get('fieldset_helper_default_collapsed_pages', 'admin/modules'),
    '#description' => t("To collapse all fieldsets for every page enter just the wildcard character '*'") . '<br/>' .
      t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %modules for the modules page and %user_settings for the user settings page.", array('%modules' => 'admin/modules', '%users_settings' => 'admin/user/settings')),
  );

  // General: Toggle all links.
  $form['general']['fieldset_helper_toggle_all_pages'] = array(
    '#type' => 'textarea',
    '#title' => t("Add 'Expand all | Collapse all' links on specified pages"),
    '#default_value' =>   variable_get('fieldset_helper_toggle_all_pages', 'admin/modules
admin/modules/list'),
    '#description' => t("Enter just the wildcard character '*' to includes 'Expand all | Collapse all' links on every page with a collapsible fieldset.") . '<br/>' .
      t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %modules for the modules page and %user_settings for the user settings page.", array('%modules' => 'admin/modules', '%users_settings' => 'admin/user/settings')),
  );

  // General: Toggle all minimum.
  $form['general']['fieldset_helper_toggle_all_minimum'] = array(
    '#type' => 'select',
    '#title' => t("Minimum number of collapsible fieldsets to show 'Expand all | Collapse all' links"),
    '#description' => t('Non-tabified fieldsets do not count towards this minimum.'),
    '#options' => array(1 => t('No minimum')) + drupal_map_assoc(range(2, 10)),
    '#default_value' => variable_get('fieldset_helper_toggle_all_minimum', 2),
  );

  // Global
  $form['global'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global states'),
  );

  // Global: Pages
  $form['global']['fieldset_helper_global_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => variable_get('fieldset_helper_global_pages', ''),
    '#description' => t('Allows collapsible fieldset state to be saved across multiple pages.') . '<br/></br>' .
      t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %node_add for the node add form and %node_edit for the node edit form.", array('%node_add' => 'node/add/*', '%node_edit' => 'node/*/edit')),
  );

  // Global: Pages
  $form['global']['fieldset_helper_global_ids'] = array(
    '#type' => 'textarea',
    '#title' => t('Fieldset ids'),
    '#default_value' => variable_get('fieldset_helper_global_ids', ''),
    '#description' => t("Allows a collapsible fieldset's state to be saved globally. Very useful for CCK fieldgroups.") . '<br/></br>' .
      t("Enter one id per line. The '*' character is a wildcard."),
  );

  // Cookie duration
  $form['cookie'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cookie duration'),
    '#description' => t("The 'Fieldset helper' module saves the current fieldset state in a client-side browser cookie.") . ' ' .
      t("By default, this is a session cookie which is cleared when the user closes the browser.") . ' ' .
      t("Changing the 'Cookie duration' to a non-zero number makes the cookie persist even after the browser is closed and restarted."),
  );

  $form['cookie']['fieldset_helper_cookie_duration'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie duration'),
    '#default_value' => variable_get('fieldset_helper_cookie_duration', 0),
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t("For how many days should the cookie persist after the client browser is closed?"),
    '#field_suffix' => t('days'),
  );

  // Advanced
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Advanced:
  $form['advanced']['fieldset_helper_disable_state_anonymous'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable server-side state management for anonymous users.'),
    '#default_value' => variable_get('fieldset_helper_disable_state_anonymous', 0),
    '#description' => t("Check this to prevent Drupal's page caching or an alternative page caching mechanism (ie Reverse Proxy, CDN, etc...) from caching pages with modified fieldset states.") . '<br/>' .
      t('Anonymous users will still be able to save their collapsible fieldset state using only client-side JavaScript.'),
  );
  // Clear fieldset id lookup
  $form['lookup_ids'] = array(
    '#type' => 'fieldset',
    '#title' => t('Clear lookup ids'),
    '#description' => t("The 'Fieldset helper' module creates a lookup table for all the collapsible fieldset ids discovered on your website.") . ' ' .
      t("Clearing this list will affect the saved collapsible fieldset states for any user who is currently logged in.") . ' ' .
      t("This list should only need to be cleared if a larger number of forms and/or fieldsets on your website have been modified."),
  );

  $form['lookup_ids']['clear_fieldset_id_lookup'] = array(
    '#type' => 'submit',
    '#value' => t('Clear fieldset lookup ids'),
    '#submit' => array('fieldset_helper_clear_fieldset_id_lookup_submit'),
  );

  $form['#submit'][] = 'fieldset_helper_admin_settings_submit';

  return system_settings_form($form);
}
/**
 * Implementation of FORM_ID_validate().
 */
function fieldset_helper_admin_settings_validate($form, &$form_state) {
  // Check that cookie duration is a valid number.
  if ( preg_match( '/^\d+$/', $form_state['values']['fieldset_helper_cookie_duration']) === 0 ) {
    form_set_error('fieldset_helper_cookie_duration', t('Cookie duration must be a valid number.'));
  }
}
/**
 * Implementation of FORM_ID_submit().
 */
function fieldset_helper_admin_settings_submit($form, &$form_state) {
  fieldset_helper_state_manager_clear_lookup_ids();
}

/**
 * Clear the fieldset helper state manager table
 */
function fieldset_helper_clear_fieldset_id_lookup_submit() {
  fieldset_helper_state_manager_clear_lookup_ids();
}


////////////////////////////////////////////////////////////////////////////////
// Test form
////////////////////////////////////////////////////////////////////////////////

/**
 * Test page for the 'Fieldset helper' module.
 */
function fieldset_helper_test() {
  $output = '';

  // Test FAPI fieldsets
  $output .= '<h3>' . t('Test collapsible fieldsets associated with a FAPI form') . '</h3>';
  $output .= render(drupal_get_form('fieldset_helper_test_form'));

  // Test unassociated fieldsets
  $output .= '<h3>' . t('Test a collapsible fieldset that is not associated with a form or node') . '</h3>';
  $element = array(
    '#type' => 'fieldset',
    '#title' => t('The un-associated fieldset'),
    '#value' => '<div>' . t('Testing the un-associated fieldset') . '</div>',
    '#collapsible' => TRUE,
  );
  $output .= theme('fieldset', array('element' => $element));

  // Test unassociated fieldsets
  $output .= '<h3>' . t('Test a collapsible fieldset that is just plain html') . '</h3>';
  $output .= '<fieldset class="collapsible form-wrapper"><legend><span class="fieldset-legend">The plain html fieldset</legend><div class="fieldset-wrapper">';
  $output .= t('Testing a fieldset that is plain html');
  $output .= '<fieldset class="collapsible form-wrapper"><legend><span class="fieldset-legend">A nested plain html fieldset</span></legend><div class="fieldset-wrapper">' . t('Testing a nested fieldset that is plain html') . '</div></fieldset>';
  $output .= '</div></fieldset>';
  return $output;
}

/**
 * Test form for the 'Fieldset helper' module.
 */
function fieldset_helper_test_form() {
  // Fieldset
  $form['default_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default fieldset'),
  );

  $form['default_fieldset']['textfield'] = array(
    '#type' => 'textfield',
    '#title' => t('Text field'),
  );

  // Collapsible
  $form['collapsible'] = array(
    '#type' => 'fieldset',
    '#title' => t('Collapsible fieldset'),
    '#collapsible' => TRUE,
  );

  $form['collapsible']['textfield'] = array(
    '#type' => 'textfield',
    '#title' => t('Text field'),
  );

  // Collapsed
  $form['collapsed'] = array(
    '#type' => 'fieldset',
    '#title' => t('Collapsed fieldset'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['collapsed']['textfield'] = array(
    '#type' => 'textfield',
    '#title' => t('Text field'),
  );

  // Nested collapsible
  $form['nested'] = array(
    '#type' => 'fieldset',
    '#title' => t('Nested fieldsets'),
    '#collapsible' => TRUE,
  );

  $form['nested']['textfield'] = array(
    '#type' => 'textfield',
    '#title' => t('Text field'),
  );

  // Nested collapsed
  $form['nested']['collapsed'] = array(
    '#type' => 'fieldset',
    '#title' => t('Nested collapsed fieldset'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['nested']['collapsed'] ['textfield'] = array(
    '#type' => 'textfield',
    '#title' => t('Text field'),
  );

  return $form;
}

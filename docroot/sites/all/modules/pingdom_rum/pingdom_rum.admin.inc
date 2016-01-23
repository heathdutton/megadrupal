<?php

/**
 * @file
 * Administrative callbacks for the Pingdom RUM module
 */

/**
 * Creates the form components for a settings page.
 */
function pingdom_rum_admin_settings($form, &$form_state) {
  $form = array();
  $form['account'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('General account settings'),
  );

  $form['account']['pingdom_rum_project_id'] = array(
    '#type' => 'textfield',
    '#title' => t('The project ID given to you by Pingdom for your site'),
    '#default_value' => variable_get('pingdom_rum_project_id', ""),
    '#description' => t('The Project ID given you by Pingdom. This is usually 24 characters long, and will only comprise hexadecimal characters. See the README file for more information.'),
    '#required' => TRUE,
  );

  $form['scope_title'] = array(
    '#type' => 'item',
    '#title' => t('Pingdom RUM monitoring scope'),
  );

  $form['scope'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'pingdom_rum') . '/pingdom_rum.admin.js'),
    ),
  );

  $form['scope']['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $visibility = variable_get('pingdom_rum_visibility_pages', 0);

  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.",
    array(
      '%blog' => 'blog',
      '%blog-wildcard' => 'blog/*',
      '%front' => '<front>',
    )
  );

  $form['scope']['page_vis_settings']['pingdom_rum_visibility_pages'] = array(
    '#type' => 'radios',
    '#title' => t('Add Pingdom RUM monitoring code to specific pages'),
    '#options' => array(
      t('Every page except the listed pages'),
      t('The listed pages only'),
    ),
    '#default_value' => $visibility,
  );

  $form['scope']['page_vis_settings']['pingdom_rum_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#title_display' => 'invisible',
    '#default_value' => variable_get('pingdom_rum_pages', ''),
    '#description' => $description,
    '#rows' => 10,
  );

  $form['scope']['role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
  );

  $form['scope']['role_vis_settings']['pingdom_rum_roles_type'] = array(
    '#type' => 'radios',
    '#title' => t('Add Pingdom RUM monitoring code for specific roles'),
    '#options' => array(
      t('Add to the selected roles only'),
      t('Add to every role except the selected ones'),
    ),
    '#default_value' => variable_get('pingdom_rum_roles_type', 0),
  );

  $role_options = array_map('check_plain', user_roles());

  $form['scope']['role_vis_settings']['pingdom_rum_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => variable_get('pingdom_rum_roles', array()),
    '#options' => $role_options,
    '#description' => t('If none of the roles are selected, all users will be tracked. If a user has any of the roles checked, that user will be tracked (or excluded, depending on the setting above).'),
  );

  return system_settings_form($form);
}

/**
 * Validate the user input.
 *
 * Check is only a hex string, otherwise would be an injection vulnerability.
 */
function pingdom_rum_admin_settings_validate($form, &$form_state) {
  $new_value = $form_state['values']['pingdom_rum_project_id'];
  if (!ctype_xdigit($new_value)) {
    form_set_error('pingdom_rum_project_id',
      t('You must enter a hexadecimal string'));
  }
}

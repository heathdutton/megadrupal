<?php

/**
 * @file
 * Administrative page callbacks for the TNS Gallup module.
 */

/**
 * Form constructor for the admin settings form.
 *
 * @see tns_gallup_menu()
 *
 * @ingroup forms
 */
function tns_gallup_admin_settings_form($form_state) {
  $form = array();

  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => FALSE,
  );

  $form['account']['tns_gallup_site_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Site ID'),
    '#default_value' => variable_get('tns_gallup_site_id', ''),
    '#description' => t('Your site ID as provided by TNS Gallup'),
  );

  // Render the role overview.
  $form['role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role specific visibility settings'),
    '#collapsible' => TRUE,
  );

  $roles = user_roles();
  $role_options = array();
  foreach ($roles as $rid => $name) {
    $role_options[$rid] = $name;
  }
  $form['role_vis_settings']['tns_gallup_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Role specific visibility'),
    '#default_value' => variable_get('tns_gallup_roles', array()),
    '#options' => $role_options,
    '#description' => t('Include script only for the selected role(s). If you select none of the roles, then all roles will see the script. If a user has any of the roles checked, the script will be included for the user.'),
  );

  // Page specific visibility configurations.
  $form['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific visibility settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $access = user_access('use PHP for TNS Gallup visibility');
  $visibility = variable_get('tns_gallup_visibility', 0);
  $pages = variable_get('tns_gallup_pages', '');

  if ($visibility == 2 && !$access) {
    $form['page_vis_settings'] = array();
    $form['page_vis_settings']['visibility'] = array('#type' => 'value', '#value' => 2);
    $form['page_vis_settings']['pages'] = array('#type' => 'value', '#value' => $pages);
  }
  else {
    $options = array(t('Add to every page except the listed pages.'), t('Add to the listed pages only.'));
    $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.",
                   array(
                     '%blog' => 'blog',
                     '%blog-wildcard' => 'blog/*',
                     '%front' => '<front>',
                   ));

    if (module_exists('php') && $access) {
      $options[] = t('Add if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $description .= ' ' . t('If you choose the PHP-mode, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    $form['page_vis_settings']['tns_gallup_visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Add script to specific pages'),
      '#options' => $options,
      '#default_value' => $visibility,
    );
    $form['page_vis_settings']['tns_gallup_pages'] = array(
      '#type' => 'textarea',
      '#title' => t('Pages'),
      '#default_value' => $pages,
      '#description' => $description,
      '#wysiwyg' => FALSE,
    );
  }

  return system_settings_form($form);
}

/**
 * Validate the TNS Gallup form values.
 *
 * While an empty site id is valid we should warn the user that this disables
 * the global functionality.
 */
function tns_gallup_admin_settings_form_validate($form, &$form_state) {
  $form_values = $form_state['values'];
  if (empty($form_values['tns_gallup_site_id'])) {
    drupal_set_message(t("You didn't define a Site ID. The TNS Gallup script won't be written to the page."), 'warning');
  }
}

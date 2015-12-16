<?php
/**
 * @file
 * Tynt admin configuration options.
 */

/**
 * Menu callback for the String Overrides module to display its administration.
 */
function tynt_admin_settings() {
  $form['account'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('General Settings'),
    '#collapsible' => FALSE,
  );

  // Tynt Site GUID.
  $form['account']['tynt_site_guid'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Site GUID'),
    '#description'   => t("Enter your site guid found on <a href='@tynt'>Tynt</a> under 'Your API Key'.", array('@tynt' => 'http://www.tynt.com/api')),
    '#default_value' => variable_get('tynt_site_guid', ''),
  );

  $form['role_vis_settings'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Role Specific Tracking Settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );

  $form['role_vis_settings']['tynt_roles'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Add tracking for specific roles'),
    '#default_value' => variable_get('tynt_roles', array()),
    '#options'       => user_roles(),
    '#description'   => t('Add tracking only for the selected role(s). If none of the roles are selected, all users will be tracked. If a user has any of the roles checked, that user will be tracked.'),
  );

  $form['page_vis_settings'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Page Specific Tracking Settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );

  $access     = user_access('use PHP for tracking visibility');
  $visibility = variable_get('tynt_visibility', 0);
  $pages      = variable_get('tynt_pages', '');

  if ($visibility == 2 && !$access) {
    $form['page_vis_settings'] = array();

    $form['page_vis_settings']['visibility'] = array('#type' => 'value', '#value' => 2);
    $form['page_vis_settings']['pages']      = array('#type' => 'value', '#value' => $pages);
  }
  else {
    $options     = array(t('Add to every page except the listed pages.'), t('Add to the listed pages only.'));
    $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array(
      '%blog'          => 'blog',
      '%blog-wildcard' => 'blog/*',
      '%front'         => '<front>',
    ));

    if ($access) {
      $options[]   = t('Add if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $description .= ' ' . t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }

    $form['page_vis_settings']['tynt_visibility'] = array(
      '#type'          => 'radios',
      '#title'         => t('Add tracking to specific pages'),
      '#options'       => $options,
      '#default_value' => $visibility,
    );

    $form['page_vis_settings']['tynt_pages'] = array(
      '#type'          => 'textarea',
      '#title'         => t('Pages'),
      '#default_value' => $pages,
      '#description'   => $description,
      '#wysiwyg'       => FALSE,
    );
  }
  return system_settings_form($form);
}

/**
 * Validate callback for settings form.
 */
function tynt_admin_settings_validate($form, &$form_state) {

  // Trim some text area values.
  $form_state['values']['tynt_pages'] = trim($form_state['values']['tynt_pages']);
}

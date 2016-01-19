<?php

/**
 * @file
 * Administrative page callbacks for the snapengage module.
 */

/**
 * Form constructor for the admin settings form.
 *
 * @see snapengage_menu()
 * @ingroup forms
 */
function snapengage_admin_settings_form($form_state) {
  $form = array();

  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => FALSE,
  );

  $form['account']['snapengage_widget_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Widget ID'),
    '#default_value' => variable_get('snapengage_widget_id', ''),
    '#description' => t('Your widget ID. It will look somewhat like this: "abcdefgh-1234-ijkl-5678-mnopqrstuvwx". You can find it at <a href="!url">!url</a>.', array('!url' => 'https://secure.snapengage.com/widget')),
  );
$form['account']['snapengage_widget_language'] = array(
  '#type' => 'select',
    '#title' => t('Widget language'),
    '#default_value' => variable_get('snapengage_widget_language', 'default'),
    '#required' => TRUE,
    '#options' => array(
      'default' => t('Default language of site. With URL detection.'),
      'user' => t('User\'s default language'),
    ),
    '#description' => t('Select how to choose language.'),
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
  $form['role_vis_settings']['snapengage_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Role specific visibility'),
    '#default_value' => variable_get('snapengage_roles', array()),
    '#options' => $role_options,
    '#description' => t('Show widget only for the selected role(s). If you select none of the roles, then all roles will see the widget. If a user has any of the roles checked, the widget will be visible to the user.'),
  );

  // Page specific visibility configurations.
  $form['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific visibility settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $access = user_access('use PHP for SnapEngage visibility');
  $visibility = variable_get('snapengage_visibility', 0);
  $pages = variable_get('snapengage_pages', '');

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
      )
    );

    if (module_exists('php') && $access) {
      $options[] = t('Add if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $description .= ' ' . t('If you choose the PHP-mode, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    $form['page_vis_settings']['snapengage_visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Add widget to specific pages'),
      '#options' => $options,
      '#default_value' => $visibility,
    );
    $form['page_vis_settings']['snapengage_pages'] = array(
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
 * Validate the snapengage form values.
 *
 * While an empty widget id is valid we should warn the user that this disables
 * the global functionality.
 */
function snapengage_admin_settings_form_validate($form, &$form_state) {
  $form_values = $form_state['values'];
  if (empty($form_values['snapengage_widget_id'])) {
    drupal_set_message(t("You didn't define a Widget ID. The SnapEngage script won't be written to the page."), 'warning');
  }
  else{
  check_plain($form_values['snapengage_widget_id']);
  }
}

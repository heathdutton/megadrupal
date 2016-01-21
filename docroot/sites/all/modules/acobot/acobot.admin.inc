<?php

/**
 * @file
 * Configuration form for Acobot settings.
 */

/**
 * Settings form.
 */
function acobot_admin_settings_form($form_state) {
  $form = array();

  $form['acobot_token'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('acobot_token', ''),
    '#title' => t('Installation Key'),
    '#description' => t('To get your installation key, log in to your Acobot
                        account and choose "My web - Installation."
                        <a href="@acobot-register" target="_blank">Sign up</a>
                        if you haven\'t.',
                        array('@acobot-register' => 'http://acobot.com/user/register')),
  );

  // Visibility settings.
  $form['visbility'] = array(
    '#type' => 'vertical_tabs',
  );

  $php_access = user_access('use PHP for Acobot visibility');
  $visibility = variable_get('acobot_visibility_pages', 0);
  $pages = variable_get('acobot_pages', ACOBOT_PAGES);

  $form['visbility']['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  if ($visibility == 2 && !$php_access) {
    $form['visbility']['page_vis_settings'] = array();
    $form['visbility']['page_vis_settings']['visibility'] = array('#type' => 'value', '#value' => 2);
    $form['visbility']['page_vis_settings']['pages'] = array('#type' => 'value', '#value' => $pages);
  }
  else {
    $options = array(
      t('Every page except the listed pages'),
      t('The listed pages only'),
    );
    $description = t("Specify pages by using their paths. Enter one path per line.
                     The '*' character is a wildcard. Example paths are %blog for
                     the blog page and %blog-wildcard for every personal blog.
                     %front is the front page.",
                     array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

    if (module_exists('php') && $php_access) {
      $options[] = t('Pages on which this PHP code returns <code>TRUE</code> (experts only)');
      $title = t('Pages or PHP code');
      $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    else {
      $title = t('Pages');
    }
    $form['visbility']['page_vis_settings']['acobot_visibility_pages'] = array(
      '#type' => 'radios',
      '#title' => $title,
      '#options' => $options,
      '#default_value' => $visibility,
    );
    $form['visbility']['page_vis_settings']['acobot_pages'] = array(
      '#type' => 'textarea',
      '#title' => $title,
      '#title_display' => 'invisible',
      '#default_value' => $pages,
      '#description' => $description,
      '#rows' => 10,
    );
  }

  // Render the role overview.
  $form['visbility']['role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
  );

  $form['visbility']['role_vis_settings']['acobot_visibility_roles'] = array(
    '#type' => 'radios',
    '#title' => t('Show chat box for specific roles'),
    '#options' => array(
      t('Selected roles only'),
      t('Every role except the selected ones'),
    ),
    '#default_value' => variable_get('acobot_visibility_roles', 0),
  );

  $role_options = array_map('check_plain', user_roles());
  $form['visbility']['role_vis_settings']['acobot_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => variable_get('acobot_roles', array()),
    '#options' => $role_options,
    '#description' => t('If none of the roles are selected, all users will see the chat box. If a user has any of the roles checked, that user will see the chat box (or excluded, depending on the setting above).'),
  );

  return system_settings_form($form);
}

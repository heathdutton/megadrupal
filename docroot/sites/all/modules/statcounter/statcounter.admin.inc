<?php

/**
 * @file
 * Administrative page callbacks for the statcounter module.
 */

/**
 * Implements hook_admin_settings() for configuring the module.
 */
function statcounter_admin_settings_form($form_state) {
  // General settings.
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );

  $form['account']['statcounter_project_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Statcounter Project ID'),
    '#default_value' => variable_get('statcounter_project_id', ''),
    '#size' => 15,
    '#maxlength' => 20,
    '#required' => TRUE,
    '#description' => t('The Project ID number is unique. Copy this number from the <strong>Statcounter Configuration Pages</strong>, or extract this number from the <strong>Statcounter Code</strong> and enter it into this field.'),
  );

  $form['account']['statcounter_security_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Statcounter Security Code'),
    '#default_value' => variable_get('statcounter_security_code', ''),
    '#size' => 15,
    '#maxlength' => 20,
    '#required' => FALSE,
    '#description' => t('The Security Code is unique. Copy this number from the <strong>Statcounter Configuration Pages</strong>, or extract this number from the <strong>Statcounter Code</strong> and enter it into this field.'),
  );

  $form['account']['statcounter_invisible_tracking'] = array(
    '#type' => 'checkbox',
    '#title' => t('No Counter or Button'),
    '#description' => t('If checked, this option allows your site to be tracked without a counter or button appearing on your webpages.'),
    '#default_value' => variable_get('statcounter_invisible_tracking', 1),
    );

  $form['account']['statcounter_js_scope'] = array(
    '#type' => 'select',
    '#title' => t('Choose region to insert the Statcounter button or counter'),
    '#description' => t("If you are tracking administration pages as well, be sure to choose a region that also exists in the administration theme.<br /><strong>Warning:</strong> Adding the Statcounter tracking code to the Page bottom or Footer region is recommended for performance reasons."),
    '#options' => system_region_list(variable_get('theme_default', 'bartik')),
    '#default_value' => variable_get('statcounter_js_scope', 'page_bottom'),
    '#states' => array(
      'visible' => array(   // action to take.
        ':input[name="statcounter_invisible_tracking"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Visibility settings.
  $form['tracking_title'] = array(
    '#type' => 'item',
    '#title' => t('Tracking scope'),
  );
  $form['tracking'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'statcounter') . '/statcounter.admin.js'),
    ),
  );

  // Page specific visibility configurations.
  $php_access = user_access('use PHP for tracking visibility');
  $visibility = variable_get('statcounter_visibility_pages', 0);
  $pages = variable_get('statcounter_pages', '');

  $form['tracking']['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  if ($visibility == 2 && !$php_access) {
    $form['tracking']['page_vis_settings'] = array();
    $form['tracking']['page_vis_settings']['visibility'] = array('#type' => 'value', '#value' => 2);
    $form['tracking']['page_vis_settings']['pages'] = array('#type' => 'value', '#value' => $pages);
  }
  else {
    $options = array(
      t('Every page except the listed pages'),
      t('The listed pages only')
    );
    $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

    if (module_exists('php') && $php_access) {
      $options[] = t('Pages on which this PHP code returns <code>TRUE</code> (experts only)');
      $title = t('Pages or PHP code');
      $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    else {
      $title = t('Pages');
    }
    $form['tracking']['page_vis_settings']['statcounter_visibility_pages'] = array(
      '#type' => 'radios',
      '#title' => t('Add tracking to specific pages'),
      '#options' => $options,
      '#default_value' => $visibility,
    );
    $form['tracking']['page_vis_settings']['statcounter_pages'] = array(
      '#type' => 'textarea',
      '#title' => $title,
      '#title_display' => 'invisible',
      '#default_value' => $pages,
      '#description' => $description,
      '#rows' => 10,
    );
  }

  // Render the role overview.
  $form['tracking']['role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
  );

  $form['tracking']['role_vis_settings']['statcounter_visibility_roles'] = array(
    '#type' => 'radios',
    '#title' => t('Add tracking for specific roles'),
    '#options' => array(
      t('Add to the selected roles only'),
      t('Add to every role except the selected ones'),
    ),
    '#default_value' => variable_get('statcounter_visibility_roles', 0),
  );

  $role_options = array_map('check_plain', user_roles());
  $form['tracking']['role_vis_settings']['statcounter_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => variable_get('statcounter_roles', array()),
    '#options' => $role_options,
    '#description' => t('If none of the roles are selected, all users will be tracked. If a user has any of the roles checked, that user will be tracked (or excluded, depending on the setting above).'),
  );

  // Standard tracking configurations.
  $form['tracking']['user_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Users'),
  );
  $t_permission = array('%permission' => t('opt-in or out of tracking'));
  $form['tracking']['user_vis_settings']['statcounter_custom'] = array(
    '#type' => 'radios',
    '#title' => t('Allow users to customize tracking on their account page'),
    '#options' => array(
      t('No customization allowed'),
      t('Tracking on by default, users with %permission permission can opt out', $t_permission),
      t('Tracking off by default, users with %permission permission can opt in', $t_permission)
    ),
    '#default_value' => variable_get('statcounter_custom', 0),
  );

  // Privacy specific configurations.
  $form['tracking']['privacy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Privacy'),
  );
  $form['tracking']['privacy']['statcounter_privacy_donottrack'] = array(
    '#type' => 'checkbox',
    '#title' => t('Universal web tracking opt-out'),
    '#description' => t('If enabled and your Statcounter server receives the <a href="http://donottrack.us/">Do-Not-Track</a> header from the client browser, the Statcounter server will not track the user. Compliance with Do Not Track could be purely voluntary, enforced by industry self-regulation, or mandated by state or federal law. Please accept your visitors privacy. If they have opt-out from tracking and advertising, you should accept their personal decision.'),
    '#default_value' => variable_get('statcounter_privacy_donottrack', 1),
  );

  // Advanced feature configurations.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['advanced']['statcounter_cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Locally cache tracking code file'),
    '#description' => t('If checked, the tracking code file is retrieved from the Statcounter site and cached locally. It is updated daily to ensure updates to tracking code are reflected in the local copy.'),
    '#default_value' => variable_get('statcounter_cache', 0),
    );

  return system_settings_form($form);
}

function statcounter_admin_settings_form_validate($form, &$form_state) {
  // Trim some text area values.
  $form_state['values']['statcounter_pages'] = trim($form_state['values']['statcounter_pages']);

  if (!preg_match('/^\d{1,}$/', $form_state['values']['statcounter_project_id'])) {
    form_set_error('statcounter_project_id', t('A valid Statcounter Project ID is an integer only.'));
  }
  // Delete obsolete local cache file.
  if (empty($form_state['values']['statcounter_cache']) && $form['advanced']['statcounter_cache']['#default_value']) {
    statcounter_clear_js_cache();
  }
}

<?php
/**
 * @file Admin page callbacks for vcita module.
 */

/**
 * Callback function to link accounts.
 */
function vcita_callback($form, &$form_state) {
  $get_string = drupal_get_query_parameters();
  if (!(empty($get_string['uid']))) {
    $vcita_account = $get_string['uid'];
    $vcita_email = $get_string['email'];
    variable_set('vcita_account', $vcita_account);
    variable_set('vcita_email', $vcita_email);
    drupal_set_message(t('Your Drupal instance is connected to your vCita account with the login @email', array('@email' => $vcita_email)));
    drupal_goto('admin/config/services/vcita');
  }
  else {
    watchdog('vcita', 'error when connecting to vcita and linking account', NULL, WATCHDOG_ERROR);
    $error_msg = t('Error received when trying to create account or log in');
    $form['vcita'] = array(
      '#type' => 'markup',
      '#markup' => '<div id="error_message">' . $error_msg . '</div>',
    );
    return $form;
  }
}
/**
 * Main configuration form.
 *
 * @param array $form
 *   Form array to be rendered.
 *
 * @return array
 *   Array containing the form to be rendered.
 */
function vcita_admin_settings_form(array $form) {
  // Path prefix needed for the vcita buttons.
  $module_path = drupal_get_path('module', 'vcita');
  // Link needed to configure vcita widget on vcita.com.
  $widget_text = t('Edit LiveSite widget text and colors');
  $widget_path = 'https://www.vcita.com/my/widgets';
  $img_array = array(
    'path' => url($module_path . '/images/config_widget.png'),
    'alt' => $widget_text,
    'attributes' => array('class' => 'image', 'id' => 'widget_img'),
  );
  $widget_img = theme_image($img_array);
  $widget_link = l($widget_img . '&nbsp;' . $widget_text, $widget_path, array('html' => TRUE));
  // Link needed to change account settings on vcita.com.
  $settings_text = t('Settings');
  $settings_path = 'https://www.vcita.com/settings/business';
  $img_array = array(
    'path' => url($module_path . '/images/settings.png'),
    'alt' => $settings_text,
    'attributes' => array('class' => 'image', 'id' => 'widget_img'),
  );
  $settings_img = theme_image($img_array);
  $settings_link = l($settings_img . '&nbsp;' . $settings_text, $settings_path, array('html' => TRUE));
  $form['vcita'] = array(
    '#type' => 'vertical_tabs',
  );
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Account Settings'),
    '#collapsible' => FALSE,
    '#group' => 'vcita',
  );
  $form['account']['email'] = array(
    '#title' => t('Email'),
    '#type' => 'textfield',
    '#size' => 30,
    '#description' => t('Enter the email used for your vCita account or leave empty to create a new account.'),
    '#maxlength' => 60,
    '#default_value' => variable_get('vcita_email', ''),
    '#element_validate' => array('_vcita_email_validator'),
  );
  $form['account']['link_account'] = array(
    '#title' => t('Create or Link Account'),
    '#type' => 'submit',
    '#value' => t('Create or Link Account'),
    '#submit' => array('_vcita_link_account'),
  );
  $form['account']['widgets'] = array(
    '#type' => 'markup',
    '#markup' => $widget_link,
    '#prefix' => '<div>',
    '#suffix' => '</div>',
  );
  $form['account']['settings'] = array(
    '#type' => 'markup',
    '#markup' => $settings_link,
    '#prefix' => '<div>',
    '#suffix' => '</div>',
  );
  // Node type settings.
  $form['node_type_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node Type Settings'),
    '#collapsible' => TRUE,
    '#group' => 'vcita',
  );
  $ntypes = node_type_get_names();
  $form['node_type_settings']['vcita_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show the vCita widget only for these content types'),
    '#default_value' => variable_get('vcita_node_types', array()),
    '#options' => $ntypes,
    '#description' => t('Show the vCita widget for the content types selected. If no options are selected, the widget will be shown on all pages'),
  );
  // Role specific settings.
  $form['role_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role Specific Settings'),
    '#collapsible' => TRUE,
    '#group' => 'vcita',
  );
  $role_options = array_map('check_plain', user_roles());
  $form['role_settings']['vcita_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Exclude these roles from being shown the vCita widget'),
    '#default_value' => variable_get('vcita_roles', array()),
    '#options' => $role_options,
    '#description' => t('Remove the vCita widget for the roles selected. If no options are selected, all roles will see the widget.'),
  );
  // Page specific settings.
  $form['page_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page Specific Settings'),
    '#collapsible' => TRUE,
    '#group' => 'vcita',
  );
  $visible = variable_get('vcita_visible', 0);
  $vc_pages = array(
    'admin',
    'admin/*',
    'user/*/*',
    'node/add*',
    'node/*/*',
  );
  $vcita_pages = variable_get('vcita_pages', implode("\n", $vc_pages));
  $options = array(t('Add to every page except the listed pages.'),
    t('Add to the listed pages only.'));
  $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.",
    array(
      '%blog' => 'blog',
      '%blog-wildcard' => 'blog/*',
      '%front' => '<front>',
    ));
  $form['page_settings']['vcita_visible'] = array(
    '#type' => 'radios',
    '#title' => t('Add script to specific pages'),
    '#options' => $options,
    '#default_value' => $visible,
  );
  $form['page_settings']['vcita_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => $vcita_pages,
    '#description' => $description,
    '#wysiwyg' => FALSE,
  );
  $form['#submit'][] = 'vcita_admin_settings_form_submit';
  return system_settings_form($form);
}
/**
 * Implements hook_form_submit().
 */
function vcita_admin_settings_form_submit($form, &$form_state) {
  // Capture old email setting - no change, no need for action.
  $old_email = variable_get('vcita_email', '');
  // Identifying ourselves to VCita as the Drupal module in case a new account needs to be created.
  $invite_code = 'rf-bfb314eb8f5a48d4';
  // Callback that will process the result from the vcita login.
  $callback_page = url('vcita_callback', array('absolute' => TRUE));
  $vcita_api_path = 'http://www.vcita.com/integrations/plugin/drupal/new';
  if (empty($form_state['values']['email'])) {
    // Set options for fresh invite.
    $vcita_call_options = array(
      'query' => array(
        'callback' => $callback_page,
        'invite' => $invite_code,
        'lang' => 'us_EN',
      ),
    );
  }
  else {
    // Set options for login
    variable_set('vcita_email', $form_state['values']['email']);
    $vcita_call_options = array(
      'query' => array(
        'callback' => $callback_page,
        'email' => $form_state['values']['email'],
        'lang' => 'us_EN',
      ),
    );
  }
  // Decide whether a redirect is needed at all.
  if (!($old_email == $form_state['values']['email']) || (empty($old_email))) {
    $form_state['redirect'] = array(
      'path' => $vcita_api_path,
      'options' => $vcita_call_options,
    );
  }
}
/**
 * Validator for email field.
 */
function _vcita_email_validator($element, &$form_state, $form) {
  if ((!empty($element['#value'])) && (!valid_email_address($element['#value']))) {
    form_error($element, t('Valid email is required to link your account. Leave empty to create a new vCita account.'));
  }
}
/**
 * Links the Drupal install with an existing vCita account.
 */
function _vcita_link_account($form, &$form_state) {
  // Identifying ourselves to VCita as the Drupal module in case a new account needs to be created.
  $invite_code = 'rf-bfb314eb8f5a48d4';
  // Callback that will process the result from the vcita login.
  $callback_page = url('vcita_callback', array('absolute' => TRUE));
  $vcita_api_path = 'http://www.vcita.com/integrations/plugin/drupal/new';
  if (!empty($form_state['values']['email'])) {
    variable_set('vcita_email', $form_state['values']['email']);
    $vcita_call_options = array(
      'query' => array(
        'callback' => $callback_page,
        'email' => $form_state['values']['email'],
        'lang' => 'us_EN',
      ),
    );
  }
  else {
    $vcita_call_options = array(
      'query' => array(
        'callback' => $callback_page,
        'invite' => $invite_code,
        'lang' => 'us_EN',
      ),
    );
  }
  drupal_goto($vcita_api_path, $vcita_call_options);
}

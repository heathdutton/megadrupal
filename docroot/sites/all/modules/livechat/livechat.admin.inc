<?php

/**
 * @file
 * Administration pages for the LiveChat module.
 */

/**
 * Settings form.
 */
function livechat_admin_settings_form($form_state) {
  // Warn user if no license has been installed.
  if (!livechat_is_installed()) {
    $license_warning = t('A LiveChat license has not been installed. You can do
       so on the <a href="!url">@install</a> tab.', array(
         '!url' => url('admin/config/services/livechat/install'),
         '@install' => t('Install'),
       ));
    drupal_set_message($license_warning, 'warning', FALSE);
  }

  // Warn user if no role has the "use livechat" permission.
  $use_livechat_roles = user_roles(FALSE, 'use livechat');
  if (empty($use_livechat_roles)) {
    $permissions_warning = t('No roles have been given access to the LiveChat
       widget. See <a href="!url">the module\'s permissions</a>.', array(
         '!url' => url('admin/people/permissions', array(
         'fragment' => 'module-livechat',
         'query' => array('destination' => current_path()),
      )),
       ));
    drupal_set_message($permissions_warning, 'warning', FALSE);
  }

  $form['path'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visibility'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['path']['livechat_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Show LiveChat on'),
    '#options' => array(
      LIVECHAT_VISIBILITY_NOTLISTED => t('All pages except those listed'),
      LIVECHAT_VISIBILITY_LISTED => t('Only the listed pages'),
    ),
    '#default_value' => variable_get('livechat_visibility', LIVECHAT_VISIBILITY_NOTLISTED),
  );

  $form['path']['livechat_pages'] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get('livechat_pages', LIVECHAT_VISIBILITY_DEFAULT_PAGES),
    '#description' => t("Specify pages by using their paths. Enter one path per
      line. The '*' character is a wildcard. Example paths are %blog for the blog
      page and %blog-wildcard for every personal blog. %front is the
      front page.", array(
        '%blog' => 'blog',
        '%blog-wildcard' => 'blog/*',
        '%front' => '<front>',
      )),
  );

  $form['path']['livechat_exclude_system_paths'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable LiveChat on common system paths (recommended)'),
    '#description' => t('LiveChat will not trigger on the following paths: %paths', array(
      '%paths' => str_replace("\n", ', ', LIVECHAT_VISIBILITY_SYSTEM_PATHS),
    )),
    '#default_value' => variable_get('livechat_exclude_system_paths', 1),
  );
  $form['livechat_group'] = array(
    '#type' => 'textfield',
    '#title' => 'Group Id',
    '#description' => 'If you are using LiveChat on more than one website,
       enter the id of the website here',
    '#default_value' => variable_get('livechat_group', ''),
  );

  return system_settings_form($form);
}

/**
 * License installation form.
 */
function livechat_admin_license_form($form_state) {
  $module_dir = drupal_get_path('module', 'livechat');
  drupal_add_css($module_dir . '/admin/css/livechat.css');
  drupal_add_js($module_dir . '/admin/js/livechat.js');

  if (livechat_is_installed()) {
    $form['tracking_code'] = array(
      '#type' => 'item',
      '#markup' => '<div class="messages installed_ok">' . t('LiveChat
         is installed.') . '</div>',
    );

    $form['actions']['#type'] = 'actions';
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
    );
  }
  else {
    $download_text = t('Download LiveChat for your desktop/mobile and start chatting
      with your customers!');
    $download_link = l(t('Download application'),
      'http://www.livechatinc.com/product/', array(
        'attributes' => array('external' => TRUE),
      )
    );

    $form['download_app'] = array(
      '#type' => 'item',
      '#markup' => '<p>' . $download_text . '</p><p class="btn">' . $download_link . '</p>',
    );

    // New account / Already have a license forms.
    $form['choose_form'] = array(
      '#type' => 'item',
      '#markup' =>
      '<div id="lc_logo"></div>' . '<h3>Already have a LiveChat account?</h3>' .
      '<ul id="choose_form">' . '<li><input type="radio" name="choose_form"
         id="choose_form_1" checked="checked"> <label for="choose_form_1">Yes,
         I already have a LiveChat account</label></li>' . '<li><input type="radio"
          name="choose_form" id="choose_form_0"><label for="choose_form_0">No,
          I want to create one</label></li>' . '</ul>',
    );

    // General settings.
    $form['general'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#prefix' => '<div id="livechat_already_have"><h3>Account details</h3>',
      '#suffix' => '</div>',
    );

    $form['general']['livechat_login'] = array(
      '#type' => 'textfield',
      '#title' => t('LiveChat login'),
      '#default_value' => '',
      '#size' => 30,
      '#maxlength' => 100,
      // Handled by JavaScript validator.
      '#required' => FALSE,
    );

    $form['general']['license_number'] = array(
      '#type' => 'hidden',
      '#value' => '0',
    );

    $form['general']['ajax_message'] = array(
      '#type' => 'item',
      '#markup' => '<p class="ajax_message"></p>',
    );

    // New account form.
    $form['new_account'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#prefix' => '<div id="livechat_new_account"><h3>Create a new LiveChat
account</h3>',
      '#suffix' => '</div>',
    );

    $form['new_account']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Full name'),
      '#size' => 30,
      '#maxlength' => 60,
      // Handled by JavaScript validator.
      '#required' => FALSE,
    );

    $form['new_account']['email'] = array(
      '#type' => 'textfield',
      '#title' => t('E-mail'),
      '#size' => 30,
      '#maxlength' => 70,
      // Handled by JavaScript validator.
      '#required' => FALSE,
    );

    $form['new_account']['password'] = array(
      '#type' => 'password',
      '#title' => t('Password'),
      '#size' => 30,
      '#maxlength' => 70,
      // Handled by JavaScript validator.
      '#required' => FALSE,
    );

    $form['new_account']['password_retype'] = array(
      '#type' => 'password',
      '#title' => t('Retype password'),
      '#size' => 30,
      '#maxlength' => 70,
      // Handled by JavaScript validator.
      '#required' => FALSE,
    );

    $form['new_account']['ajax_message'] = array(
      '#type' => 'item',
      '#markup' => '<p class="ajax_message"></p>',
    );

    $form['actions']['#type'] = 'actions';
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }

  return $form;
}

/**
 * Validation callback for license installation form.
 */
function livechat_admin_license_form_validate($form, &$form_state) {
  // Validate the license number.
  if ($form_state['values']['op'] === t('Save')) {
    if (!livechat_validate_license($form_state['input']['license_number'])) {
      form_set_error('livechat_license_number', t('LiveChat license number is
        incorrect.'));
    }
  }
}

/**
 * Submit callback for license installation form.
 */
function livechat_admin_license_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] === t('Remove')) {
    // Forget license if the "Reset" button was clicked.
    variable_del('livechat_license');
  }
  else {
    // Save the license that was returned by the admin JavaScript.
    variable_set('livechat_license', $form_state['input']['license_number']);
  }
}

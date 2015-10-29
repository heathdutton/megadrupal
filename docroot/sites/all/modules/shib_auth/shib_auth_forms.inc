<?php
/**
 * @file
 * Drupal forms of the Shibboleth authentication module.
 */

/**
 * Form constructor for the shib_auth admin general page
 *
 * @ingroup forms
 */
function shib_auth_admin_general($form, &$form_state) {
  $form = array();

  $form['shib_handler_settings'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Shibboleth handler settings'),
    '#weight'        => -10,
    '#collapsible'   => FALSE,
  );

  $form['shib_attribute_settings'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Attribute settings'),
    '#weight'        => -5,
    '#collapsible'   => FALSE,
  );

  $form['shib_handler_settings']['shib_auth_full_handler_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Shibboleth login handler URL'),
    '#default_value' => shib_auth_config('full_handler_url'),
    '#description'   => t('The URL can be absolute or relative to the server base url: http://www.example.com/Shibboleth.sso/DS; /Shibboleth.sso/DS'),
  );

  $form['shib_handler_settings']['shib_auth_full_logout_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Shibboleth logout handler URL'),
    '#default_value' => shib_auth_config('full_logout_url'),
    '#description'   => t('The URL can be absolute or relative to the server base url: http://www.example.com/Shibboleth.sso/Logout; /Shibboleth.sso/Logout'),
  );

  $form['shib_handler_settings']['shib_auth_link_text'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Shibboleth login link text'),
    '#default_value' => shib_auth_config('link_text'),
    '#description'   => t('The text of the login link. You can change this text on the Shibbolet login block settings form too!'),
  );

  $form['shib_handler_settings']['shib_auth_force_https'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Force HTTPS on login'),
    '#description'   => t('The user will be redirected to HTTPS'),
    '#default_value' => shib_auth_config('force_https'),
  );

  $form['shib_attribute_settings']['shib_auth_username_variable'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Server variable for username'),
    '#default_value' => shib_auth_config('username_variable'),
  );

  $form['shib_attribute_settings']['shib_auth_email_variable'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Server variable for e-mail address'),
    '#default_value' => shib_auth_config('email_variable'),
  );

  $form['shib_attribute_settings']['shib_auth_define_username'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('User-defined usernames'),
    '#description'   => t('Allow users to set their Drupal usernames at first Shibboleth login. Note that disabling this option only prevents new users from registering their own username. Existing user-defined usernames will remain valid.'),
    '#default_value' => shib_auth_config('define_username'),
  );

  $form['shib_attribute_settings']['shib_auth_enable_custom_mail'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('User-defined e-mail addresses'),
    '#description'   => t('Ask users to set their contact email address at first login. Disabling this option will override contact address with the one, which was received from IdP. (In this case, missing e-mail address will result in fatal error.)'),
    '#default_value' => shib_auth_config('enable_custom_mail'),
  );

  $form['shib_attribute_settings']['shib_auth_account_linking'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Account linking'),
    '#description'   => t('Allow locally authenticated users to link their Drupal accounts to federated logins. Note that disabling this option only prevents from creating/removing associations, existing links will remain valid.'),
    '#default_value' => shib_auth_config('account_linking'),
  );

  $form['shib_attribute_settings']['shib_auth_account_linking_text'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Shibboleth account linking text'),
    '#default_value' => shib_auth_config('account_linking_text'),
    '#description'   => t('The text of the link providing account linking shown on the user settings form.'),
  );

  $form['shib_attribute_debug'] = array(
    '#type'          => 'fieldset',
    '#title'         => 'Debugging options',
    '#weight'        => -1,
  );

  $form['shib_attribute_debug']['shib_auth_debug_state'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable DEBUG mode.'),
    '#default_value' => shib_auth_config('debug_state'),
  );

  $form['shib_attribute_debug']['shib_auth_debug_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('DEBUG path prefix'),
    '#default_value' => shib_auth_config('debug_url'),
    '#description'   => t('For example use \'user/\' for display DEBUG messages on paths \'user/*\'!')
  );

  return system_settings_form($form);
} // function shib_auth_admin_general()

/**
 * Form constructor for the shib_auth custom form.
 *
 * Generate the custom e-mail and username provider form
 *
 * @see shib_auth_custom_data_validate()
 * @see shib_auth_custom_data_submit()
 * @ingroup forms
 */
function shib_auth_custom_data($form, &$form_state) {
  global $user;

  //drupal_set_message("Old user, accept, consentver: ".(int)$existing_authmap.",".(int)$consent_accepted.",".(int)$existing_authmap['consentver'], 'error');
  $form = array();
  //Decide if the user is already registered, but a new consent version is present
  $authmap_username = db_select('shib_authmap', 'c')
                          ->fields('c')
                          ->condition('targeted_id', shib_auth_getenv(shib_auth_config('username_variable')), '=')
                          ->execute()
                          ->fetchAssoc();
  //if the user already registered, display a message about consent version change
  if ($authmap_username) {
    $form['consent'] = array(
        '#value' => t('The terms of use document has been modified since your last login with id @targetedid. Please read it carefully, and click on Submit if you accept the new version. Version you have accepted: @accepted - document version: @documentver', array('@targetedid' => $authmap_username['targeted_id'], '@accepted' => $authmap_username['consentver'], '@documentver' => shib_auth_config('terms_ver') )),
      );
  }
  //register a new user with the data given here
  else {
    //username textfield is writable, if define_username variable is true
    if (shib_auth_config('define_username')) {
      $form['custom_username'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Desired username'),
        '#default_value' => shib_auth_getenv(shib_auth_config('username_variable')),
        '#size'          => 60,
      );
    }
    //username textfield is not writable, if define_username variable is false
    else {
      $form['custom_usernamem'] = array(
        '#markup' => t('<b>Username:</b> <br />'),
      );

      $form['custom_username'] = array(
        '#markup' => shib_auth_getenv(shib_auth_config('username_variable')),
        '#suffix' => '<br />',
      );
    }

    if (shib_auth_config('enable_custom_mail')) {
      //mail textfield is writable, if define_username variable is true
      $form['custom_mail'] = array(
        '#type'          => 'textfield',
        '#title'         => t('E-mail'),
        '#default_value' => shib_auth_getenv(shib_auth_config('email_variable')),
        '#size'          => 60,
      );
    }
    //mail textfield is not writable, if define_username variable is false
    else {
      $form['custom_mailm'] = array(
        '#markup' => t('<b>E-mail:</b> <br />'),
      );

      $form['custom_mail'] = array(
        '#markup' => shib_auth_getenv(shib_auth_config('email_variable')),
        '#suffix' => '<br />',
      );
    }
  }
  //display terms of use link, if it is required by admin
  if (shib_auth_config('terms_accept')) {
    $form['accept'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('I accept the <a href="@link" target="_blank">terms and conditions</a>', array('@link' => url(shib_auth_config('terms_url')))),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
  );

  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
} // function shib_auth_custom_data()

/**
 * Form validation handler for shib_auth_custom_data()
 */
function shib_auth_custom_data_validate($form, &$form_state) {
  if (shib_auth_config('enable_custom_mail')) {
    if ($form_state['values']['custom_mail'] == '') {
      form_set_error('', t('You have to fill the \'E-mail\' field.'));
    }
  }

  if (shib_auth_config('define_username')) {
    if ($form_state['values']['custom_username'] == '') {
      form_set_error('', t('You have to fill the \'Username\' field.'));
    }
  }

  if (shib_auth_config('terms_accept')) {
    if ($form_state['values']['accept'] == FALSE) {
      form_set_error('', t('You have to accept Terms of Use to proceed.'));
    }
  }
} // shib_auth_custom_email_validate()

/**
 * This function prevents drupal loading a cached page after shibboleth login
 */
function shib_auth_login() {
  $queries = $_GET;
  $queries['q'] = drupal_substr($_GET['q'], 11);

  if ($queries['q']!='shib_link') {
    drupal_goto('', array('query' => $queries));
  }
  else {
    drupal_goto('shib_login/user');
  }
}

/**
 * This function manages account linking
 */
function shib_auth_account_link() {
    $_SESSION['shib_auth_account_linking'] = TRUE;
    drupal_goto(shib_auth_generate_login_url());
}

/**
 * Form constructor for the shib_auth admin advanced page
 *
 * @ingroup forms
 */
function shib_auth_admin_advanced() {
  $form = array();

  $form['shib_handler_settings'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Advanced SAML2 settings'),
    '#weight'        => 0,
    '#collapsible'   => FALSE,
  );

  $form['shib_handler_settings']['shib_auth_is_passive'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable passive authentication'),
    '#description'   => t('Enable passive authentication'),
    '#default_value' => shib_auth_config('is_passive'),
  );

  $form['shib_handler_settings']['shib_auth_forceauthn'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable forced authentication'),
    '#description'   => t('Force users to re-authenticate'),
    '#default_value' => shib_auth_config('forceauthn'),
  );

  $form['shib_auth_auto_destroy_session'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Strict shibboleth session checking'),
    '#weight'        => -2,
  );

  $form['shib_auth_auto_destroy_session']['shib_auth_auto_destroy_session'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Destroy Drupal session when the Shibboleth session expires.'),
    '#default_value' => shib_auth_config('auto_destroy_session'),
  );

  $form['shib_login_settings'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Login settings'),
    '#weight'        => -1,
  );

  $form['shib_login_settings']['shib_auth_login_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t("URL to redirect to after login"),
    '#default_value' => shib_auth_config('login_url'),
    '#description'   => t("The URL can be absolute or relative to the server base url. The relative paths will be automatically extended with the site base URL. If this value is empty than the user will be redirected to the originally requested page.")
  );

  $form['shib_logout_settings'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Logout settings'),
    '#weight'        => -1,
  );

  $form['shib_logout_settings']['shib_auth_logout_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t("URL to redirect to after logout"),
    '#default_value' => shib_auth_config('logout_url'),
    '#description'   => t("The URL can be absolute or relative to the server base url. The relative paths will be automatically extended with the site base URL. If you are using SLO, this setting is probably useless (depending on the IdP)")
  );

  $form['shib_terms_conditions_settings'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Terms of use settings'),
    '#weight'        => -2,
    '#collapsible'   => FALSE,
  );

  $form['shib_terms_conditions_settings']['shib_auth_terms_accept'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Force users to accept Terms of Use'),
    '#description'   => t('Require acceptance of Terms of Use every time it changes'),
    '#default_value' => shib_auth_config('terms_accept'),
  );

  $form['shib_terms_conditions_settings']['shib_auth_terms_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('URL of the document'),
    '#default_value' => shib_auth_config('terms_url'),
    '#description'   => t('Please refence local content with e.g. "node/1", or use an external link.'),
  );

  $form['shib_terms_conditions_settings']['shib_auth_terms_ver'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Document version'),
    '#default_value' => shib_auth_config('terms_ver'),
    '#size'          => 4,
  );

  return system_settings_form($form);
}



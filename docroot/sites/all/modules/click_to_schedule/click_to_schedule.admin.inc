<?php

/**
 * @file
 * Module configuration pages for the Click to Schedule module.
 */

/**
 * Page callback: Builds main administration page.
 *
 * If a TimeTrade account has already been logged in to through this site, shows
 * a page indicating that. Otherwise shows a form to log in or a link to the
 * create account page.
 *
 * @return array
 *   A renderable array.
 */
function click_to_schedule_admin() {
  $path = libraries_get_path('fancybox');
  $files = click_to_schedule_set_files();
  drupal_add_js($path . '/' . $files['js']);
  drupal_add_js(CLICK_TO_SCHEDULE_PATH . '/click_to_schedule.js');
  drupal_add_css($path . '/' . $files['css']);
  drupal_add_css(CLICK_TO_SCHEDULE_PATH . '/click_to_schedule.css');

  // Check if user has account and credentials are stored locally.
  if (variable_get('click_to_schedule_email', '') && variable_get('click_to_schedule_password', '')) {
    $subscription_type = click_to_schedule_is_entitled('AppointmentRouter') ? 'paid' : 'free';
    // Show existing account page.
    $render = array(
      'logged_in' => array(
        '#prefix' => '<div class="main">',
        'verified' => array(
          '#markup' => '<h2>' . t('TimeTrade account credentials verified') . '</h2>',
        ),
        'email' => array(
          '#markup' => '<p>' . t('Logged in using') . ' ' . variable_get('click_to_schedule_email', '') . '</p>',
        ),
        'log out' => array(
          '#theme' => 'link',
          '#text' => t('Sign out of this TimeTrade account'),
          '#path' => 'admin/config/user-interface/click-to-schedule/log-out',
          '#options' => array('html' => FALSE, 'attributes' => array()),
        ),
        '#suffix' => '</div>',
      ),
      'offsite_content' => array(
        '#prefix' => '<div class="offsite">',
        'offsite_copy' => array(
          '#markup' => '<iframe id="logged-in-iframe" width="560" height="840" frameborder="0" src="http://www.timetrade.com/sites/default/files/freeAcct_Existing.html"></iframe>',
        ),
        'links' => array(
          '#theme' => 'links',
          '#links' => array(
            'learn_more' => array(
              'title' => t('Learn more...'),
              'href' => 'http://player.vimeo.com/video/14251501?title=0&amp;byline=0&amp;portrait=0&amp;color=ffffff',
              'attributes' => array(
                'class' => array('timetrade-admin-link', 'iframe', 'video'),
              ),
            ),
          ),
          '#attributes' => array('class' => array('timetrade-admin-links')),
        ),
        'go_to_timetrade' => array(
          '#theme' => 'link',
          '#text' => '<button type="button">' . t('Go to my TimeTrade account') . '</button>',
          '#path' => 'http://www.timetrade.com/login',
          '#options' => array(
            'html' => TRUE,
            'attributes' => array(
              'target' => '_blank',
              'class' => array('timetrade-admin-button'),
            ),
          ),
        ),
        '#suffix' => '</div>',
      ),
      'footer' => array(
        '#prefix' => '<div class="timetrade-admin-footer">',
        '#markup' => t('Powered by') . '<span class="timetrade"> TimeTrade</span>',
        '#suffix' => '</div>',
      ),
    );

    if ($subscription_type == 'paid') {
      unset($render['offsite_content']['offsite_copy']);
      unset($render['offsite_content']['links']);
    }
  }
  else {
    // Show login form.
    $render = array(
      'heading' => array(
        '#markup' => '<h2>' . t('Connect your TimeTrade account to Drupal') . '</h2>',
      ),
      'form' => drupal_get_form('click_to_schedule_connect_existing_account'),
      'offsite_content' => array(
        '#prefix' => '<div class="offsite">',
        'create_account' => array(
          '#theme' => 'link',
          '#text' => t('Create FREE Account'),
          '#path' => 'admin/config/user-interface/click-to-schedule/create-free-account',
          '#options' => array(
            'html' => TRUE,
            'attributes' => array(
              'id' => 'timetrade-create-account',
              'class' => array('timetrade-admin-button', 'button'),
            ),
          ),
        ),
        'offsite_copy' => array(
          '#markup' => '<iframe id="" width="560" height="550" frameborder="0" src="http://www.timetrade.com/sites/default/files/freeAcct_First.html"></iframe>',
        ),
        'links' => array(
          '#theme' => 'links',
          '#links' => array(
            'learn_more' => array(
              'title' => t('Learn more...'),
              'href' => 'http://player.vimeo.com/video/35010741?title=0&amp;byline=0&amp;portrait=0&amp;color=ffffff',
              'attributes' => array(
                'class' => array('timetrade-admin-link', 'iframe', 'video'),
              ),
            ),
          ),
          '#attributes' => array('class' => array('timetrade-admin-links')),
        ),
        '#suffix' => '</div>',
      ),
      'footer' => array(
        '#prefix' => '<div class="timetrade-admin-footer">',
        '#markup' => t('Powered by') . '<span class="timetrade"> TimeTrade</span>',
        '#suffix' => '</div>',
      ),
    );
  }

  return $render;
}

/**
 * Form constructor for logging in to existing TimeTrade account.
 *
 * @see click_to_schedule_connect_existing_account_submit()
 * @ingroup forms
 */
function click_to_schedule_connect_existing_account($form, &$form_state) {
  global $user;

  $form['existing_account'] = array(
    '#markup' => '<h3>' . t('Existing account') . '</h3>',
  );

  $form['email'] = array(
    '#title' => t('E-mail'),
    '#type' => 'textfield',
    '#default_value' => $user->mail,
  );
  $form['password'] = array(
    '#title' => t('Password'),
    '#type' => 'password',
  );
  $form['links'] = array(
    '#theme' => 'links',
    '#links' => array(
      'reminder' => array(
        'title' => t('Forgot password'),
        'href' => CLICK_TO_SCHEDULE_TIMETRADE_SERVER . 'td/login.jsp?passwordReset=1',
        'attributes' => array('target' => '_blank', 'class' => array('timetrade-admin-link')),
      ),
    ),
    '#attributes' => array('class' => array('timetrade-admin-links')),
  );
  $form['connect'] = array(
    '#value' => t('Connect'),
    '#type' => 'submit',
  );

  return $form;
}

/**
 * Form submission handler for click_to_schedule_connect_existing_account().
 */
function click_to_schedule_connect_existing_account_submit($form, &$form_state) {
  // Send login info to TimeTrade.
  $return_code = click_to_schedule_start_session($form_state['values']['email'], _click_to_schedule_md5_base64($form_state['values']['password']));

  if (isset($return_code)) {
    if ($return_code < 0) {
      // On failure go to login failed page.
      drupal_goto('admin/config/user-interface/click-to-schedule/retry-login', array('query' => array('email' => $form_state['values']['email'])));
    }
    else {
      $entitled = click_to_schedule_is_entitled('RoundRobinRouting');
      if ($entitled) {
        // On success store email & pw; go to existing free account page.
        variable_set('click_to_schedule_email', $form_state['values']['email']);
        variable_set('click_to_schedule_password', _click_to_schedule_md5_base64($form_state['values']['password']));
        drupal_goto('admin/config/user-interface/click-to-schedule');
      }
      elseif ($entitled === FALSE) {
        drupal_set_message(t('Your TimeTrade account does not have appointment routing enabled. Please upgrade to the Workgroup edition.'), 'warning');
      }
    }
  }
}

/**
 * Page callback: Form constructor for retrying TimeTrade login.
 *
 * Path: admin/config/user-interface/click-to-schedule/retry-login
 *
 * @see click_to_schedule_connect_existing_account_submit()
 * @ingroup forms
 */
function click_to_schedule_connect_retry_login($form, &$form_state) {
  $path = libraries_get_path('fancybox');
  $files = variable_get('click_to_schedule_fancybox_files', array());
  $query_params = drupal_get_query_parameters();

  $form['#attached']['js'] = array(
    $path . '/' . $files['js'],
    CLICK_TO_SCHEDULE_PATH . '/click_to_schedule.js',
  );
  $form['#attached']['css'] = array(
    $path . '/' . $files['css'],
    CLICK_TO_SCHEDULE_PATH . '/click_to_schedule.css',
  );

  $form['heading'] = array(
    '#markup' => '<h2>' . t('Unable to connect to your TimeTrade account. Please re-enter your credentials.') . '</h2>',
  );
  $form['email'] = array(
    '#prefix' => '<div id="retry-login-container">',
    '#title' => t('E-mail'),
    '#type' => 'textfield',
    // Value from previous form.
    '#default_value' => $query_params['email'],
  );
  $form['password'] = array(
    '#title' => t('Password'),
    '#type' => 'password',
  );
  $form['links'] = array(
    '#theme' => 'links',
    '#links' => array(
      'support' => array(
        'title' => t('Support'),
        'href' => 'http://www.timetrade.com',
        'attributes' => array('target' => '_blank', 'class' => array('timetrade-admin-link')),
      ),
      'reminder' => array(
        'title' => t('Forgot password'),
        'href' => CLICK_TO_SCHEDULE_TIMETRADE_SERVER . 'td/login.jsp?passwordReset=1',
        'attributes' => array('target' => '_blank', 'class' => array('timetrade-admin-link')),
      ),
    ),
    '#attributes' => array('class' => array('timetrade-admin-links')),
  );
  $form['connect'] = array(
    '#value' => t('Connect'),
    '#type' => 'submit',
    '#suffix' => '</div>',
  );
  $form['footer'] = array(
    '#prefix' => '<div class="timetrade-admin-footer">',
    '#markup' => t('Powered by') . '<span class="timetrade"> TimeTrade</span>',
    '#suffix' => '</div>',
  );

  $form['#submit'][] = 'click_to_schedule_connect_existing_account_submit';

  return $form;
}

/**
 * Page callback: Form constructor for creating a new TimeTrade account.
 *
 * Path: admin/config/user-interface/click-to-schedule/create-free-account
 *
 * @see click_to_schedule_create_free_account_validate()
 * @see click_to_schedule_create_free_account_submit()
 * @ingroup forms
 */
function click_to_schedule_create_free_account($form, &$form_state) {
  global $user;

  $path = libraries_get_path('fancybox');
  $files = variable_get('click_to_schedule_fancybox_files', array());

  $form['#attached']['js'] = array(
    $path . '/' . $files['js'],
    CLICK_TO_SCHEDULE_PATH . '/click_to_schedule.js',
  );
  $form['#attached']['css'] = array(
    $path . '/' . $files['css'],
    CLICK_TO_SCHEDULE_PATH . '/click_to_schedule.css',
  );

  $form['heading'] = array(
    '#markup' => '<h2>' . t('Create a FREE TimeTrade Account') . '</h2>',
  );
  $form['fname'] = array(
    '#title' => t('First Name'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['lname'] = array(
    '#title' => t('Last Name'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['organization'] = array(
    '#title' => t('Organization'),
    '#type' => 'textfield',
    '#required' => TRUE,
  );

  $form['timezone'] = array(
    '#type' => 'select',
    '#title' => t('Time Zone'),
    '#default_value' => date_default_timezone_get(),
    '#options' => system_time_zones(),
    '#attributes' => array('class' => array('timezone-detect')),
  );

  $form['email'] = array(
    '#title' => t('E-mail'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => $user->mail,
  );
  $form['password'] = array(
    '#description' => t('Passwords must be at least 6 characters and be a combination of at least two of the following: lowercase letters, capital letters, numbers, and special characters.'),
    '#type' => 'password_confirm',
    '#required' => TRUE,
  );
  $form['create_account'] = array(
    '#value' => t('Create My Account'),
    '#type' => 'submit',
  );

  $form['footer'] = array(
    '#prefix' => '<div class="timetrade-admin-footer">',
    '#markup' => t('Powered by') . '<span class="timetrade"> TimeTrade</span>',
    '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Form validation handler for click_to_schedule_create_free_account().
 *
 * @see click_to_schedule_create_free_account_submit()
 */
function click_to_schedule_create_free_account_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('Enter a valid email address.'));
  }

  $MIN_LEN = 6;
  if (drupal_strlen($form_state['values']['password']) < $MIN_LEN) {
    form_set_error('password', t('Password must be at least six characters long.'));
  }

  $has_capital = FALSE;
  $has_lower = FALSE;
  $has_special = FALSE;
  $has_number = FALSE;
  // Iterate through password characters using preg_split(), available since PHP
  // 4.
  $chars = preg_split('//', $form_state['values']['password'], -1, PREG_SPLIT_NO_EMPTY);
  foreach ($chars as $char) {
    if (ctype_upper($char)) {
      $has_capital = TRUE;
    }
    elseif (ctype_lower($char)) {
      $has_lower = TRUE;
    }
    elseif (ctype_punct($char)) {
      $has_special = TRUE;
    }
    elseif (ctype_digit($char)) {
      $has_number = TRUE;
    }
  }

  if ($has_lower + $has_capital + $has_special + $has_number < 2) {
    form_set_error('password', t('Passwords must be a combination of two of the following: numbers, lowercase letters, uppercase letters, and special characters (!@#$%^&*?_~).'));
  }
}

/**
 * Form submission handler for click_to_schedule_create_free_account().
 *
 * @see click_to_schedule_create_free_account_validate()
 */
function click_to_schedule_create_free_account_submit($form, &$form_state) {
  // Send account info to TimeTrade.
  $client = new JsonRpcClient(
      CLICK_TO_SCHEDULE_TIMETRADE_SERVER . 'td/json/baseApi',
      '2.0'
  );

  $user = new stdClass();
  $user->firstName = $form_state['values']['fname'];
  $user->lastName = $form_state['values']['lname'];
  $user->emailAddress = $form_state['values']['email'];
  $user->password = _click_to_schedule_md5_base64($form_state['values']['password']);
  $user->organization = $form_state['values']['organization'];
  $user->timezone = $form_state['values']['timezone'];
  $user->registrationSource = 'DrupalOrg';
  $user->productCode = 'W-F-BASIC';

  try {
    $response = $client->call('baseApi.createAndActivateAccount', array($user));

    if (isset($response['result'])) {
      $return_code = $response['result'];
      if ($return_code > 0) {
        switch ($return_code) {
          case 1:
            $message = t('Invalid password. Please check your entries or !url',
                         array('!url' => l(t('contact support.'), 'mailto: help@timetrade.com?subject=Account Registration', array('absolute' => TRUE))));
            break;

          case 4:
            $message = t('Invalid first name. Please check your entries or !url',
                         array('!url' => l(t('contact support.'), 'mailto: help@timetrade.com?subject=Account Registration', array('absolute' => TRUE))));
            break;

          case 5:
            $message = t('Invalid last name. Please check your entries or !url',
                         array('!url' => l(t('contact support.'), 'mailto: help@timetrade.com?subject=Account Registration', array('absolute' => TRUE))));
            break;

          case 7:
            $message = t('Invalid email address. Please check your entries or !url',
                         array('!url' => l(t('contact support.'), 'mailto: help@timetrade.com?subject=Account Registration', array('absolute' => TRUE))));
            break;

          case 8:
            $message = t('A TimeTrade account already exists for this email address. Please !url to TimeTrade',
                         array('!url' => l(t('sign in'), 'admin/config/user-interface/click-to-schedule')));
            break;

          case 9:
            $message = t('A server error occurred. Please try again or !url',
                         array('!url' => l(t('contact support.'), 'mailto: help@timetrade.com?subject=Account Registration', array('absolute' => TRUE))));
            break;

          case 10:
            $message = t('Invalid timezone. Please try again or !url',
                         array('!url' => l(t('contact support.'), 'mailto: help@timetrade.com?subject=Account Registration', array('absolute' => TRUE))));
            break;
        }
        drupal_set_message(t('Signup failed: !message', array('!message' => filter_xss($message))), 'error');
      }
      else {
        // If success go to logged-in page.
        variable_set('click_to_schedule_email', $form_state['values']['email']);
        variable_set('click_to_schedule_password', _click_to_schedule_md5_base64($form_state['values']['password']));
        drupal_goto('admin/config/user-interface/click-to-schedule');
      }
    }
    elseif (isset($response['error'])) {
      drupal_set_message(t('Could not connect to TimeTrade: !message', array('!message' => filter_xss($response['error']['msg']))), 'error');
    }
  } catch (JsonRpcClientHttpException $e) {
    drupal_set_message(t('Error connecting to TimeTrade: !message', array('!message' => filter_xss($e->getMessage()))), 'error');
  }
}

/**
 * Menu callback: Log out of TimeTrade account.
 *
 * Path: admin/config/user-interface/click-to-schedule/log-out
 *
 * @see click_to_schedule_menu()
 */
function click_to_schedule_log_out() {
  variable_del('click_to_schedule_email');
  variable_del('click_to_schedule_password');
  drupal_goto('admin/config/user-interface/click-to-schedule');
}

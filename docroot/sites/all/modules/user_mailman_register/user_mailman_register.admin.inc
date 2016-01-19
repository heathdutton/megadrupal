<?php
/**
 * @file
 * Administration pages for User Mailman Register.
 */


/**
 * Menu callback for the admin listings form.
 */
function user_mailman_register_adminlist_form() {
  drupal_set_title(t('Mailman Mailing Lists'));

  $form['add_new'] = array(
    '#type' => 'markup',
    '#prefix' => '<p>&raquo; ',
    '#markup' => l(t('Add new mailing list'), 'admin/structure/user_mailman_register/add', array('query' => drupal_get_destination())),
    '#suffix' => '</p>',
  );
  $header = array(
    array('data' => t('ID'), 'field' => 'lid', 'sort' => 'asc'),
    array('data' => t('Name'), 'field' => 'name'),
    array('data' => t('User invite'), 'field' => 'user_invite'),
    array('data' => t('User notify'), 'field' => 'user_notify'),
    array('data' => t('List admins notify'), 'field' => 'user_admin_notify'),
    array('data' => t('Allow unsub'), 'field' => 'allow_unsubscribe'),
    array('data' => t('Allow temp disable'), 'field' => 'allow_temp_disable'),
    array('data' => t('Allow digest'), 'field' => 'allow_digest'),
    array('data' => t('Default new reg'), 'field' => 'defaultnewreg'),
    array('data' => t('Role sync'), 'field' => 'role_sync'),
    array('data' => t('Actions')),
  );
  $rows = array();
  $query = db_select('mailman_lists', 'ml')
    ->extend('PagerDefault')
    ->extend('TableSort');
  $query
    ->fields('ml', array(
        'lid',
        'name',
        'user_invite',
        'user_notify',
        'user_admin_notify',
        'allow_unsubscribe',
        'allow_temp_disable',
        'allow_digest',
        'defaultnewreg',
        'role_sync',
        'webadmin',
        'webpass',
      ))
    ->orderByHeader($header);
  $count_query = $query->countQuery();
  $query->setCountQuery($count_query);
  $result = $query->execute();
  foreach ($result as $list) {
    $row = (array) $list;
    unset($row['webadmin']);
    unset($row['webpass']);
    $cmd = _user_mailman_register_allowed_list($list) ? t('Edit') : t('Activate');
    $row[] = l(t('@cmd', array('@cmd' => $cmd)), 'admin/structure/user_mailman_register/edit/' . $row['lid']) . '<br />' . l(t('Delete'), 'admin/structure/user_mailman_register/delete/' . $row['lid']);
    $rows[] = $row;
  }
  $form['mailing_lists'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('There are no mailing lists configured.'),
  );
  $form['mailing_lists_pager'] = array('#theme' => 'pager');
  $form['num_lists'] = array(
    '#type' => 'hidden',
    '#value' => $count_query->execute()->fetchField(),
  );

  return $form;
}


/**
 * Menu callback for the admin settings form.
 */
function user_mailman_register_adminsettings_form() {
  drupal_set_title(t('Mailman Mailing Lists'));

  $perms = user_role_permissions(array(DRUPAL_ANONYMOUS_RID => DRUPAL_ANONYMOUS_RID));
  if (isset($perms[DRUPAL_ANONYMOUS_RID]['access user_mailman_register'])) {
    $form['anonymous_warning'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="messages warning">',
      '#markup' => t('Warning: anonymous users have the ability to subscribe to mailing lists without registering. To prevent this, turn off permission "access user_mailman_register" for anonymous users in !permissions_url.', array('!permissions_url' => l(t('anonymous user permissions'), 'admin/people/permissions/' . DRUPAL_ANONYMOUS_RID))),
      '#suffix' => '</div>',
    );
  }
  $form['user_mailman_register_checkinterval'] = array(
    '#type' => 'textfield',
    '#title' => t('Mailman connection interval'),
    '#size' => 5,
    '#maxlength' => 5,
    '#required' => TRUE,
    '#default_value' => variable_get('user_mailman_register_checkinterval', 180),
    '#description' => t("How often (in minutes) the user subscription form can connect to the mailman server to check his/her subscription status. During this interval, subscription status will be retrieved from the User Mailman Register stored status of the last connection to the mailman server. Set to 0 to connect every time users check their subscription form.")
  );
  $form['user_mailman_register_hourly_threshold'] = array(
    '#type' => 'select',
    '#title' => t('Hourly threshold'),
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50)),
    '#default_value' => variable_get('user_mailman_register_hourly_threshold', 1),
    '#description' => t('The maximum number of subscription form submissions a user can perform per hour and per list. This affects all submissions for anonymous users and list invitation submissions for every user.'),
  );
  $form['user_mailman_register_required'] = array(
    '#type' => 'checkbox',
    '#title' => t('A subscription is required'),
    '#default_value' => variable_get('user_mailman_register_required', 0),
    '#description' => t('If checked, user must subscribe to at least one mailing list.'),
  );
  $form['user_mailman_register_registerform'] = array(
    '#type' => 'checkbox',
    '#title' => t('Visible in user registration form'),
    '#default_value' => variable_get('user_mailman_register_registerform', 0),
    '#description' => t('If checked, a "Mailing Lists Subscriptions" section will be displayed in the new user registration form. You will also have to grant %permission to "anonymous user" (to register) and "authenticated user" (to manage their own subscription) in the !permissions_url for each applicable mailing list.', array('%permission' => '"can subscribe to <listname>"', '!permissions_url' => l(t('User Mailman Register permissions'), 'admin/user/permissions', array('fragment' => 'module-user_mailman_register')))),
  );
  $form['user_mailman_register_ownform'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display in own separate category'),
    '#default_value' => variable_get('user_mailman_register_ownform', 1),
    '#description' => t('If checked, mailing list subscription is displayed in a separate category on the <em>My account</em> page, otherwise it will be displayed in the user account edit form.'),
  );
  if (module_exists('profile')) {
    $form['user_mailman_register_profilename'] = array(
      '#type' => 'textfield',
      '#title' => t('Profile module'),
      '#size' => 50,
      '#default_value' => variable_get('user_mailman_register_profilename', ''),
      '#description' => t('Enter the name(s) of the !profile-url field(s) from which to retrieve the display name for the user. You can enter multiple fields separated by commas; eg: profile_firstname, profile_lastname. Optional.', array('!profile-url' => l(t('Profile'), 'admin/user/profile'))),
    );
  }
  $form['user_mailman_register_mailonerrors'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail on errors'),
    '#size' => 50,
    '#default_value' => variable_get('user_mailman_register_mailonerrors', ''),
    '#description' => t('If not empty, a warning mail will be sent to these addresses when a user can not modify his subscriptions because of a mailman server connection failure. Multiple emails should be comma separated. Optional.'),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'user_mailman_register_adminsettings_form_submit';
  return $form;
}


/**
 * Validation function for the admin settings form
 */
function user_mailman_register_adminsettings_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['user_mailman_register_checkinterval'])) {
    form_set_error('user_mailman_register_checkinterval', 'Not a valid number.');
  }
  if (isset($form['user_mailman_register_profilename'])) {
    $profile_string = $form_state['values']['user_mailman_register_profilename'];
    if ($profile_string) {
      $profile_array = explode(',', $profile_string);
      $profile_fields = db_query('SELECT name, title FROM {profile_field}')->fetchAllKeyed();
      foreach ($profile_array as $name) {
        $name = trim($name);
        if (!isset($profile_fields[$name])) {
          form_set_error('user_mailman_register_profilename', t('%name is not a valid profile name.', array('%name' => $name)));
        }
      }
    }
  }
  $mail_string = $form_state['values']['user_mailman_register_mailonerrors'];
  if ($mail_string) {
    $mail_array = explode(',', $mail_string);
    foreach ($mail_array as $mail) {
      $mail = trim($mail);
      if (!valid_email_address($mail)) {
        form_set_error('user_mailman_register_mailonerror', t('%mail is not a valid email address.', array('%mail' => $mail)));
      }
    }
  }
}


/**
 * Submission function for the admin settings form
 */
function user_mailman_register_adminsettings_form_submit($form, &$form_state) {
  // If we've changed the value of user_mailman_register_ownform we need to
  // rebuild the menus to make the Mailing Lists tab appear/disappear from the
  // user page.
  menu_rebuild();
}


/**
 * Menu callback for editing an individual mailing list or creating a new one.
 *
 * @param $lid
 *   ID of the existing list, or 0 for a new one.
 */
function user_mailman_register_adminedit_form($form, &$form_state, $lid = NULL) {
  $form = array();

  if (!$lid) {
    drupal_set_title(t('Add new mailing list'));
    $list = (object) array(
      'lid' => 0,
      'name' => '',
      'command' => '',
      'admin' => '',
      'web' => '',
      'webarch' => '',
      'webadmin' => '',
      'webpass' => '',
      'version' => 0,
      'user_invite' => 0,
      'user_notify' => 0,
      'user_admin_notify' => 0,
      'allow_unsubscribe' => 0,
      'allow_temp_disable' => 0,
      'allow_digest' => 0,
      'description' => '',
      'defaultnewreg' => 0,
      'role_sync' => 0,
    );
  }
  else {
    $list = _user_mailman_register_get_list($lid);
    if (!$list) {
      drupal_goto('admin/structure/user_mailman_register');
    }
    drupal_set_title(t('Edit mailing list %list', array('%list' => $list->name)), PASS_THROUGH);
    // If list exists, then we'll add a descriptive header to the form.
    $form['list_header'] = array(
      '#theme' => 'table',
      '#header' => array(
        array('data' => t('ID')),
        array('data' => t('Name')),
        array('data' => t('Admin URL')),
        array('data' => t('Status')),
      ),
      '#rows' => array(
        array(
          'lid' => $list->lid ? $list->lid : '-',
          'name' => $list->name,
          'command' => $list->webadmin,
          'status' => (_user_mailman_register_allowed_list($list)) ? t('Enabled') : t('Disabled'),
        )
      ),
    );
  }

  // Mailman list managment

  $form['mailman_admin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mailman settings'),
    '#tree' => TRUE,
  );

  $form['mailman_admin']['name'] = array(
    '#title' => 'Mailing List name',
    '#default_value' => !empty($list->name) ? $list->name : '',
    '#type' => 'textfield',
    '#required' => TRUE,
  );
  $form['mailman_admin']['command'] = array( // TBD, this is a holdover from MM
    '#title' => t("Mailing List 'request' email address"),
    '#default_value' => !empty($list->command) ? $list->command : '',
    '#description' => t('Email commands are sent here.'),
    '#type' => 'textfield',
//    '#required' => TRUE,
  );
  $form['mailman_admin']['admin'] = array( // TBD, this is a holdover from MM
    '#title' => t("Mailing List 'admin' email address"),
    '#default_value' => !empty($list->admin) ? $list->admin : '',
    '#description' => t('(Optional. admin is used for BCC).'),
    '#type' => 'textfield',
  );
  $form['mailman_admin']['web'] = array(
    '#title' => t('Mailing list web address for users'),
    '#default_value' => !empty($list->web) ? $list->web : '',
    '#description' => t('http://... Leave empty if hidden.'),
    '#type' => 'textfield',
  );
  $form['mailman_admin']['webarch'] = array(
    '#title' => t('Mailing list web archive address for users'),
    '#default_value' => !empty($list->webarch) ? $list->webarch : '',
    '#description' => t('http://... Leave empty if hidden.'),
    '#type' => 'textfield',
  );

  $form['mailman_admin']['webadmin'] = array(
    '#title' => t('Admin web page'),
    '#default_value' => $list->webadmin,
    '#description' => t("Mailman admin web page. The url has to be avaiable from your drupal web server ip. It's highly recommended to use the https protocol if your server supports it. Set empty to disable this list. Example: https://www.mysite.com/cgi-bin/mailman/admin/mylist ."),
    '#type' => 'textfield',
    '#required' => FALSE,
  );
  $form['mailman_admin']['webpass'] = array(
    '#title' => t('Admin web password'),
    '#description' => t('Mailman administrator web password.'),
    '#type' => 'textfield',
    '#default_value' => $list->webpass,
    '#size' => 15,
  );
  $form['mailman_admin']['version'] = array(
    '#title' => t('Mailman version'),
    '#description' => t('Select your mailman software version (it should be displayed on the mailman web interface footer). Try different settings if you realize that digest and nomail subscription does not work correctly.'),
    '#type' => 'radios',
    '#required' => TRUE,
    '#default_value' => $list->version,
    '#options' => array(t('2.1.9 or prior'), t('2.1.10 or later')),
  );
  $form['mailman_admin']['user_invite'] = array(
    '#title' => t('Only invite users'),
    '#default_value' => $list->user_invite,
    '#description' => t('Just invite new users instead of subscribing them?'),
    '#type' => 'checkbox',
  );
  $form['mailman_admin']['user_notify'] = array(
    '#title' => t('Notify users'),
    '#default_value' => $list->user_notify,
    '#description' => t('Send welcome messages to the new subscribee?'),
    '#type' => 'checkbox',
  );
  $form['mailman_admin']['user_admin_notify'] = array(
    '#title' => t('Notify mailman administrators'),
    '#default_value' => $list->user_admin_notify,
    '#description' => t('Send notifications of new subscription to the list owner?'),
    '#type' => 'checkbox',
  );

  // Drupal list managment

  $form['drupal_admin'] = array(
    '#type' => 'fieldset',
    '#title' => t('List interface settings'),
    '#tree' => TRUE,
  );

  $form['drupal_admin']['role_sync'] = array(
    '#title' => t('Sync with roles'),
    '#default_value' => $list->role_sync,
    '#description' => t('Subscribe and/or unsubscribe users to this list according to their current role and list permissions. This action will be performed everytime a user role is changed.'),
    '#type' => 'radios',
    '#required' => TRUE,
    '#options' => _user_mailman_register_sync_options(),
  );

  $form['drupal_admin']['defaultnewreg'] = array(
    '#type' => 'radios',
    '#title' => t('Default choice'),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => $list->defaultnewreg,
    '#required' => TRUE,
    '#description' => t('This will be the default choice in the user subscription form.'),
  );
  $form['drupal_admin']['allow_unsubscribe'] = array(
    '#title' => t('Allow unsubscribe'),
    '#default_value' => $list->allow_unsubscribe,
    '#description' => t('Allow users to unsubscribe from the list? Unchecked will set <b>Default choice</b> option to <em>Yes</em> and force subscription to this list.'),
    '#type' => 'checkbox',
  );
  $form['drupal_admin']['allow_temp_disable'] = array(
    '#title' => t('Allow temporary disable'),
    '#default_value' => $list->allow_temp_disable,
    '#description' => t('Allow users to temporarily disable delivery?'),
    '#type' => 'checkbox',
  );
  $form['drupal_admin']['allow_digest'] = array(
    '#title' => t('Allow digest'),
    '#default_value' => $list->allow_digest,
    '#description' => t('Allow users to subscribe in digest mode?'),
    '#type' => 'checkbox',
  );
  $form['drupal_admin']['description'] = array(
    '#title' => t('Description'),
    '#default_value' => $list->description,
    '#description' => t('A brief description of this mailing list. This text will be displayed in the subscriptions page.'),
    '#type' => 'textarea',
  );
  $form['list'] = array(
    '#type' => 'value',
    '#value' => $list,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#suffix' => l(t('Cancel'), 'admin/structure/user_mailman_register/settings'),
  );
  return $form;
}


/**
 * Validation function for edit form for an individual mailing list
 */
function user_mailman_register_adminedit_form_validate($form, &$form_state) {
  $web = $form_state['values']['mailman_admin']['web'];
  if (!empty($web) && !valid_url($web, 1)) {
    form_set_error('web', t('%web is not a valid url', array('%web' => $web)));
  }
  $webarch = $form_state['values']['mailman_admin']['webarch'];
  if (!empty($webarch) && !valid_url($webarch, 1)) {
    form_set_error('webarch', t('%webarch is not a valid url', array('%webarch' => $webarch)));
  }
  $webadmin = $form_state['values']['mailman_admin']['webadmin'];
  if (!empty($webadmin) && !valid_url($webadmin, 1)) {
    form_set_error('webadmin', t('%webadmin is not a valid url', array('%webadmin' => $webadmin)));
  }
}


/**
 * Submission function for edit form for an individual mailing list
 */
function user_mailman_register_adminedit_form_submit($form, &$form_state) {
  $list = (object) array(
    'lid' => $form_state['values']['list']->lid,
    'name' => $form_state['values']['mailman_admin']['name'],
    'command' => $form_state['values']['mailman_admin']['command'],
    'admin' => $form_state['values']['mailman_admin']['admin'],
    'web' => $form_state['values']['mailman_admin']['web'],
    'webarch' => $form_state['values']['mailman_admin']['webarch'],
    'webadmin' => $form_state['values']['mailman_admin']['webadmin'],
    'webpass' => $form_state['values']['mailman_admin']['webpass'],
    'version' => $form_state['values']['mailman_admin']['version'],
    'user_invite' => $form_state['values']['mailman_admin']['user_invite'],
    'user_notify' => $form_state['values']['mailman_admin']['user_notify'],
    'user_admin_notify' => $form_state['values']['mailman_admin']['user_admin_notify'],
    'allow_unsubscribe' => $form_state['values']['drupal_admin']['allow_unsubscribe'],
    'allow_temp_disable' => $form_state['values']['drupal_admin']['allow_temp_disable'],
    'allow_digest' => $form_state['values']['drupal_admin']['allow_digest'],
    'description' => $form_state['values']['drupal_admin']['description'],
    'defaultnewreg' => $form_state['values']['drupal_admin']['defaultnewreg'],
    'role_sync' => $form_state['values']['drupal_admin']['role_sync'],
  );


  if ($list->lid) {
    // Update existing mailing list record
    if (drupal_write_record('mailman_lists', $list, 'lid')) {
      $action = t('updated');
      $watchdog = WATCHDOG_NOTICE;
    }
    else {
      $action = t('not updated');
      $watchdog = WATCHDOG_ERROR;
    }
  }
  else {
    // Create new mailing list record
    unset($list->lid);
    if (drupal_write_record('mailman_lists', $list)) {
      $action = t('created');
      $watchdog = WATCHDOG_NOTICE;
    }
    else {
      $action = t('not created');
      $watchdog = WATCHDOG_ERROR;
    }
  }
  $log = '%name list settings @action';
  $msg = t($log, array('%name' => $list->name, '@action' => $action));
  drupal_set_message($msg);
  watchdog('user mailman reg', $log, array('%name' => $list->name, '@action' => $action), $watchdog);
  $form_state['redirect'] = 'admin/structure/user_mailman_register';
}


/**
 * Menu callback for removal of an existing mailing list
 */
function user_mailman_register_admindelete_form($form, &$form_state, $lid) {
  $list = _user_mailman_register_get_list($lid);
  if (!$list) {
    drupal_goto('admin/structure/user_mailman_register');
  }
  $form['list'] = array(
    '#type' => 'value',
    '#value' => $list,
  );
  return confirm_form($form,
    t('Confirm mailing list removal'),
    'admin/structure/user_mailman_register',
    t('Are you sure you want to remove mailing list %name? This action cannot be undone.', array('%name' => $list->name)),
    t('Approve'),
    t('Cancel')
  );
}


/**
 * Submission function for removal of an existing mailing list.
 */
function user_mailman_register_admindelete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $list = $form_state['values']['list'];
    $ret = db_delete('mailman_lists')
      ->condition('lid', $list->lid)
      ->execute();
    drupal_set_message(t('Mailing list %name name has been removed.', array('%name' => $list->name)));
    $form_state['redirect'] = 'admin/structure/user_mailman_register';
  }
}



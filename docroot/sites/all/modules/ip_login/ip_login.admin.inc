<?php
/**
 * @file
 * Sets up the IP Login admin page settings.
 */

/**
 * Returns the settings form
 */
function ip_login_admin_settings() {
  $form['ip_address'] = array(
    '#prefix' => '<div class="messages status">',
    '#description' => t('Users for which IP Login is enabled are listed below.'),
    '#markup' => t('According to Drupal\'s <code>ip_address()</code> function <strong>your IP address is <code>@ip_address</code></strong>.', array('@ip_address' => ip_address())),
    '#suffix' => ' ' . t('Tip: You can find your computer\'s external IP address at <a href="http://www.whatsmyip.org/">www.whatsmyip.org</a>.') . '</div>'
  );
  $form['ip_login_users'] = array(
    '#markup' => t(
      '<p><strong>Note:</strong> To manage users, please see the <a href="!link">list of users with IP Login enabled</a>.</p>',
      array('!link' => '/admin/people/ip_login')
    )
  );
  $form['ip_login_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('IP Login enabled'),
    '#description' => t('Check to enable IP Login, uncheck to disable. If unchecked no user will be able to login via IP address.'),
    '#default_value' => variable_get('ip_login_enabled', 1)
  );
  $form['ip_login_suppress_messages'] = array(
    '#type' => 'checkbox',
    '#title' => t('Suppress IP Login messages'),
    '#description' => t('Check to prevent IP Login showing any login-related messages.'),
    '#default_value' => variable_get('ip_login_suppress_messages', 0)
  );

  $form['ip_login_destination'] = array(
    '#type' => 'textfield',
    '#title' => t('Destination after successful login by IP'),
    '#description' => t('<strong>Leave blank to send user back to their originally requested page</strong>, or enter the Drupal path users should be redirected to when automatically logged in. For example "user" or "&lt;front&gt;" etc.'),
    '#default_value' => variable_get('ip_login_destination', '')
  );

  $form['login_page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Login page link'),
    '#description' => t('A link with text from the <em>Login page link text</em> to log the user in automatically will be added to the login page unless it is blank.'),
  );
  $form['login_page']['ip_login_link_login_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Login page link text'),
    '#description' => t('Text for link on login page. Blank to remove link, defaults to "Log in automatically"'),
    '#default_value' => t(variable_get('ip_login_link_login_page', 'Log in automatically')),
  );
  $form['login_page']['ip_login_link_login_page_help'] = array(
    '#type' => 'textfield',
    '#title' => t('Login page help text'),
    '#description' => t("Help text under IP Login link on login page. Defaults to 'Your computer's IP address has been matched and validated.' and will not appear if <em>Login page link text</em> is blank."),
    '#default_value' => t(variable_get('ip_login_link_login_page_help', "Your computer's IP address has been matched and validated.")),
  );
  $form['login_page']['ip_login_link_login_page_weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#delta' => 20,
    '#default_value' => variable_get('ip_login_link_login_page_weight', -10),
    '#description' => t('Optional. The position within the login page form for this link.'),
  );

  $form['login_block'] = array(
    '#type' => 'fieldset',
    '#title' => t('Login block link'),
    '#description' => t('A link with text from the <em>Login page link text</em> to log the user in automatically will be added to the login page unless it is blank.'),
  );
  $form['login_block']['ip_login_link_login_block'] = array(
    '#type' => 'textfield',
    '#title' => t('Login blocks link text'),
    '#description' => t('Text for link on "User login" and "Log in by IP link" blocks. Use blank to remove link, defaults to "Log in automatically"'),
    '#default_value' => t(variable_get('ip_login_link_login_block', 'Log in automatically')),
    '#required' => FALSE,
  );
  $form['login_block']['ip_login_link_login_block_weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#delta' => 20,
    '#default_value' => variable_get('ip_login_link_login_block_weight', -10),
    '#description' => t('Optional. The position within the login block form for this link.'),
  );

  // Make auto-login message editable.
  $form['other_messages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Messages'),
    '#description' => t('You can alter other messages here and include tokens listed below.'),
  );
  $message_default = 'Account [user:name] does not have permission to log out once automatically logged in. You have been logged back in.';
  $form['other_messages']['ip_login_logged_back_in'] = array(
    '#type' => 'textarea',
    '#title' => t('User automatically logged back after attempting logout without permission'),
    '#description' => t('Text for the message "' . $message_default . '"'),
    '#default_value' => t(variable_get('ip_login_logged_back_in', $message_default)),
    '#cols' => 100,
    '#rows' => 5,
    '#required' => FALSE,
  );
  $form['other_messages']['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('user'),
    '#global_types' => TRUE,
    '#click_insert' => TRUE
  );
  // allows ip check on certain pages only
  $form['active_pages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Path specific IP logins'),
    '#description' => t('<p>You can choose specific paths that IP Login will or will not try to log in a user. <strong>Please note:</strong></p><ul><li>No alias or system path lookup is performed, so you will need to have both paths listed if you want IP logins to happen on both paths  - e.g. alias "about/this-site" and system path "node/123" would both need to be added.</li><li>This will not affect users with <code>can log in as another user</code> permission who have already logged out and have a cookie.</li></ul>'),
    '#collapsible' => FALSE,
  );
  $access = user_access('use PHP for block visibility');
  $pages = variable_get('ip_login_active_pages', '');
  if ($pages) {
    $check_mode = substr($pages, 0, 1);
    $pages = substr($pages, 1);
  }
  else {
    $check_mode = '';
  }
  if ($check_mode == 2 && !$access) {
    $form['active_pages'] = array();
    $form['active_pages']['check_mode'] = array('#type' => 'value', '#value' => 2);
    $form['active_pages']['check_pages'] = array('#type' => 'value', '#value' => $pages);
  }
  else {
    $options = array(t('Login by IP on every page except the listed <strong>paths</strong>.'), t('Login by IP only on the listed <strong>paths</strong>.'));
    $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page. Note you must also enter system paths (e.g. 'node/123') for aliases if you want IP logins to happen there too.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

    if ($access) {
      $options[] = t('login by IP if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $description .= ' ' . t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    $form['active_pages']['check_mode'] = array(
      '#type' => 'radios',
      '#title' => t('Login by IP on specific paths'),
      '#options' => $options,
      '#default_value' => $check_mode,
    );
    $form['active_pages']['check_pages'] = array(
      '#type' => 'textarea',
      '#title' => t('Pages'),
      '#default_value' => $pages,
      '#description' => $description,
    );
  }
  $form['#submit'][] = 'ip_login_admin_settings_submit';
  return system_settings_form($form);
}


/**
 * Returns a list of IP login-enabled users
 */
function ip_login_user_list() {
  // add handy header
  $output = '<div class="messages status">';
  $output .= t('According to Drupal\'s <code>ip_address()</code> function <strong>your IP address is <code>@ip_address</code></strong>. ', array('@ip_address' => ip_address()));
  $output .= t('Tip: You can find your computer\'s external IP address at <a href="http://www.whatsmyip.org/">www.whatsmyip.org</a>.') . '</div>';
  // add table of IP-enabled users
  $output .= t(
    '<p>Users for which IP Login is enabled are listed below. Login behaviour is controlled <a href="!link">IP Login settings</a>.</p>',
    array('!link' => '/admin/config/people/ip_login')
  );
  $output .= _ip_login_user_list_table();
  return $output;
}

/**
 * Submit handler for admin settings form
 */
function ip_login_admin_settings_submit($form, &$form_state) {
  if ($form_state['values']['check_pages']) {
    variable_set('ip_login_active_pages', $form_state['values']['check_mode'] . $form_state['values']['check_pages']);
  }
  else {
    variable_set('ip_login_active_pages', '');
  }
}

/**
 * Generates a table of IP-enabled users
 */
function _ip_login_user_list_table() {
  // get all IP-login enabled users and display in a table on the admin page
  $rows = array();
  $result = db_query(
    "SELECT u.name, u.uid, ip.ip_match
     FROM {users} u
     INNER JOIN {ip_login_user} ip ON ip.uid = u.uid
     ORDER BY ip.ip_match");
  foreach ($result as $row) {
    $rows[] = array(
      $row->name,
      $row->ip_match,
      l(
        t('edit'),
        'user/' . $row->uid . '/edit',
        array(
          'query' => array('destination' => 'admin/people/ip_login'),
          'fragment' => 'ip_login'
        )
      )
    );
  }
  $header = array('Username', 'IP ranges', 'Operations');
  if (isset($rows[0])) {
    $table = theme('table', array('header' => $header, 'rows' => $rows));
    $table .= l(
      t('Edit a user account to add more IP addresses.'),
      'admin/people/people'
      );
    return $table;
  }
  else {
    return t('There are presently no users with IP login ranges set.') . ' ' .
      l(
        t('Edit a user account to add some IP addresses'),
        'admin/people/people'
      );
  }
}

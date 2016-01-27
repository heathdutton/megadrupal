<?php

/**
 * @file
 * Configuration file for the protected_node module.
 */

/**
 * Protected node fieldset default behavior: smart.
 *
 * When editing a node that can be protected and the protected fieldset is
 * added, open it if the node is already protected. Leave closed otherwise.
 */
define('PROTECTED_NODE_FIELDSET_SMART', 0);

/**
 * Protected node fieldset default behavior: open.
 *
 * When editing a node that can be protected, always show the fieldset as
 * opened.
 */
define('PROTECTED_NODE_FIELDSET_OPEN', 1);

/**
 * Protected node fieldset default behavior: closed.
 *
 * When editing a node that can be protected, always show the fieldset as
 * closed.
 */
define('PROTECTED_NODE_FIELDSET_CLOSE', 2);


/**
 * Helper function.
 *
 * The default email subject sent to users to whom you give access to
 * a protected node.
 */
function protected_node_email_subject() {
  return t("Please visit my website...");
}

/**
 * Helper function.
 *
 * The default email body sent to users to whom you give access to
 * a protected node.
 */
function protected_node_email_body() {
  return t("Dear friend,\r\n\r\nI just created a new page on my website and wanted to invite you to view it.\r\n\r\nThe page is protected by the following password: [node:password]\r\n\r\nThank you.\r\n[site:name]");
}

/**
 * Actual implementation of the protected_node_menu() function.
 *
 * Moved here to optimize the module (menu are cached and the definitions
 * are only rarely necessary.)
 */
function protected_node_menu_array() {
  $items = array();

  $items['admin/content/protected'] = array(
    'title' => 'Protected',
    'description' => 'Find and manage content.',
    'page callback' => 'protected_node_content_list',
    'access arguments' => array('access protected node overview page'),
    'type' => MENU_LOCAL_TASK,
    'file' => 'protected_node.admin.inc',
    'weight' => -9,
  );
  $items['admin/config/content/protected_node'] = array(
    'title' => 'Protected node',
    'description' => 'Edit the global Protected node settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('protected_node_admin_settings'),
    'access arguments' => array('administer site configuration'),
    'file' => 'protected_node.settings.inc',
  );
  $items['protected-node'] = array(
    'title' => 'Protected page - Enter Password',
    'description' => 'Here you can enter the password for protected pages',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('protected_node_enterpassword'),
    'access callback' => 'protected_node_access_callback',
    'type' => MENU_CALLBACK,
    'file' => 'protected_node.redirect.inc',
  );
  $items['protected-nodes'] = array(
    'title' => 'Protected page - Enter Password',
    'description' => 'Here you can enter the password for protected pages',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('protected_node_enter_any_password'),
    'access arguments' => array('access protected node password form'),
    'type' => MENU_CALLBACK,
    'file' => 'protected_node.fork.inc',
  );

  return $items;
}

/**
 * Actual implementation of the protected_node_form_alter() function.
 *
 * It was moved here to optimize the module (so the .module is smaller)
 * This is really only necessary when some edits a node.
 *
 * @param[in,out] $form
 *   The form to be altered with the protected node field set.
 */
function protected_node_node_form_alter(&$form) {
  $protection = variable_get('protected_node_protection_' . $form['type']['#value'], PROTECTED_NODE_PROTECTION_PROTECTABLE);
  if ($protection == PROTECTED_NODE_PROTECTION_NEVER) {
    // Never protecting, do nothing.
    return;
  }

  // Turn off the auto-complete feature on this form since it could otherwise
  // pre-fill the password with the wrong parameter.
  $form['#attributes']['autocomplete'] = 'off';

  $node_type = node_type_get_type($form['type']['#value']);
  $fieldset_mode = variable_get('protected_node_fieldset_' . $node_type->type, PROTECTED_NODE_FIELDSET_SMART);
  switch ($fieldset_mode) {
    case PROTECTED_NODE_FIELDSET_OPEN:
      $collapsed = FALSE;
      break;

    case PROTECTED_NODE_FIELDSET_CLOSE:
      $collapsed = TRUE;
      break;

    // Case PROTECTED_NODE_FIELDSET_SMART.
    default:
      if ($protection == PROTECTED_NODE_PROTECTION_ALWAYS
      || ($protection == PROTECTED_NODE_PROTECTION_PROTECTED && empty($form['nid']['#value']))) {
        $collapsed = FALSE;
      }
      else {
        $collapsed = empty($form['#node']->protected_node_is_protected);
      }
      break;
  }
  $form['protected_node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Password protect this @node', array('@node' => $node_type->name)),
    '#collapsible' => TRUE,
    '#collapsed' => $collapsed,
    '#group' => 'additional_settings',
  );
  if ($protection == PROTECTED_NODE_PROTECTION_ALWAYS) {
    // Always protected, don't show anything to the user.
    $form['protected_node']['protected_node_is_protected'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );
  }
  else {
    // Define the current default value.
    if (empty($form['nid']['#value'])) {
      // A new node is being created, use the protection mode to determine the
      // default.
      // No need to test for NEVER, we're already out of this function in that
      // case.
      // No need to test for ALWAYS, we went through the previous level if() in
      // that case.
      $protected = $protection == PROTECTED_NODE_PROTECTION_PROTECTED;
    }
    else {
      $protected = !empty($form['#node']->protected_node_is_protected);
    }
    $form['protected_node']['protected_node_is_protected'] = array(
      '#type' => 'checkbox',
      '#title' => t('This @node is protected', array('@node' => $node_type->name)),
      '#description' => t('Select this checkbox to password protect this page.'),
      '#default_value' => $protected,
    );
  }

  if (isset($form['#node']->protected_node_show_title)) {
    $show_title = $form['#node']->protected_node_show_title;
  }
  else {
    $show_title = variable_get('protected_node_show_node_titles', FALSE);
  }

  // When custom description text is set there is no use for the Show Title
  // checkbox.
  $global_text = variable_get('protected_node_description_' . $form['#bundle'], '');
  $content_type_text = variable_get('protected_node_description', '');
  if (empty($global_text) && empty($content_type_text)) {
    $form['protected_node']['protected_node_show_title'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show Title'),
      '#description' => t('Show the title to users when requesting the password.'),
      '#default_value' => $show_title,
    );
  }
  else {
    $form['protected_node']['protected_node_show_title'] = array(
      '#type' => 'hidden',
      '#default_value' => $show_title,
    );
  }

  // Define a password field unless only the global password should be used.
  switch (variable_get('protected_node_use_global_password', PROTECTED_NODE_PER_NODE_PASSWORD)) {
    case PROTECTED_NODE_PER_NODE_PASSWORD:
    case PROTECTED_NODE_PER_NODE_AND_GLOBAL_PASSWORD:
      $form['protected_node']['protected_node_passwd'] = array(
        '#type' => 'password_confirm',
        '#description' => t('Enter a password here to protect this @node or set a global password on <a href="@settings_page">the settings page</a>. Changing the password will prevent all the users who knew the old password from accessing this page.', array(
          '@node' => $node_type->name, '@settings_page' => url('admin/config/content/protected_node'))),
        '#size' => 20,
        '#after_build' => array('protected_node_autocomplete_off'),
      );

      break;

  }

  if (variable_get('protected_node_allow_hint', FALSE)) {
    if (isset($form['#node']->protected_node_hint)) {
      $hint = $form['#node']->protected_node_hint;
    }
    else {
      $hint = '';
    }
    $form['protected_node']['protected_node_hint'] = array(
      '#type' => 'textarea',
      '#title' => t('Password hint'),
      '#description' => t('Enter a password hint. This information is displayed in the password form using the [node:password-hint] token.'),
      '#default_value' => $hint,
      '#cols' => 45,
      '#rows' => 5,
    );
  }

  // Email support.
  $email_activation = variable_get('protected_node_email_activation', FALSE);
  if ($email_activation) {
    // Random password text.
    $random_password_help_text = '';
    if (variable_get('protected_node_random_password', FALSE)) {
      $random_password_help_text = '<br />' . t('<strong>Additional Note</strong>: If a password has not previously been saved for this node, you may leave the password field blank to generate a random password. In this case, you must enter at least one email address so someone knows the password.');
    }

    if (isset($form['#node']->protected_node_emails)) {
      $previous_emails = $form['#node']->protected_node_emails;
    }
    else {
      $previous_emails = '';
    }

    $form['protected_node']['protected_node_emails'] = array(
      '#type' => 'textarea',
      '#title' => t('Emails'),
      '#description' => t('Enter a list of email addresses separated by newlines and/or commas. At the time you save the page, each one of those users will be sent a notification email.')
      . '<br />'
      . t('<strong>WARNING</strong>: for an email to be sent you MUST (1) protect the node; (2) re-enter the password; (3) publish the page.')
      . '<br />'
      . t('It is necessary to re-enter the password because it is otherwise encrypted in the database.')
      . $random_password_help_text,
      '#default_value' => $previous_emails,
      '#cols' => variable_get('protected_node_email_box_width', '10'),
      '#rows' => variable_get('protected_node_email_box_height', '2'),
    );
  }
}

/**
 * Define the settings form.
 *
 * @return array
 *   $form The settings form
 */
function protected_node_admin_settings() {

  // Total.
  $total_count = db_select('node')
    ->fields('node', array('nid'))
    ->countQuery()
    ->execute()
    ->fetchField();

  // Protected.
  $protected_count = db_select('protected_nodes')
    ->fields('protected_nodes', array('nid'))
    ->condition('protected_node_is_protected', 1)
    ->countQuery()
    ->execute()
    ->fetchField();

  // Title.
  $title_count = db_select('protected_nodes')
    ->fields('protected_nodes', array('nid'))
    ->condition('protected_node_is_protected', 1)
    ->condition('protected_node_show_title', 1)
    ->countQuery()
    ->execute()
    ->fetchField();

  // Global.
  $global_count = db_select('protected_nodes')
    ->fields('protected_nodes', array('nid'))
    ->condition('protected_node_is_protected', 1)
    ->condition('protected_node_passwd', '')
    ->countQuery()
    ->execute()
    ->fetchField();

  $unprotected_count = $total_count - $protected_count;
  $hiding_count = $protected_count - $title_count;
  $node_password_count = $protected_count - $global_count;

  // Any nodes?
  if ($total_count > 0) {
    // Statistics.
    $form['protected_node_stats'] = array(
      '#type' => 'fieldset',
      '#title' => t('Protected node statistics'),
      '#collapsible' => TRUE,
    );

    $column_labels = array(
      array('data' => t('Label'), 'style' => 'text-align:center'),
      array('data' => t('Count'), 'style' => 'text-align:center'),
      array('data' => t('Percent'), 'style' => 'text-align:center'),
    );

    $rows = array();
    $rows[] = array(
      t('Total nodes'),
      array(
        'data' => $total_count,
        'style' => 'text-align:right',
      ),
      array(
        'data' => t('100%'),
        'style' => 'text-align:right',
      ),
    );
    $rows[] = array(
      t('Unprotected nodes'),
      array(
        'data' => $unprotected_count,
        'style' => 'text-align:right',
      ),
      array(
        'data' => round($unprotected_count * 100 / ($total_count), 2) . '%',
        'style' => 'text-align:right',
      ),
    );
    $rows[] = array(
      t('Protected nodes'),
      array(
        'data' => $protected_count,
        'style' => 'text-align:right',
      ),
      array(
        'data' => round($protected_count * 100 / ($total_count), 2) . '%',
        'style' => 'text-align:right',
      ),
    );
    if ($protected_count > 0) {
      $rows[] = array(
        t('Showing title'),
        array(
          'data' => $title_count,
          'style' => 'text-align:right',
        ),
        array(
          'data' => round($title_count * 100 / $protected_count, 2) . '%',
          'style' => 'text-align:right',
        ),
      );
      $rows[] = array(
        t('Hiding title'),
        array(
          'data' => $hiding_count,
          'style' => 'text-align:right',
        ),
        array(
          'data' => round($hiding_count * 100 / $protected_count, 2) . '%',
          'style' => 'text-align:right',
        ),
      );
      $rows[] = array(
        t('Global passwords'),
        array(
          'data' => $global_count,
          'style' => 'text-align:right',
        ),
        array(
          'data' => round($global_count * 100 / $protected_count, 2) . '%',
          'style' => 'text-align:right',
        ),
      );
      $rows[] = array(
        t('Node passwords'),
        array(
          'data' => $node_password_count,
          'style' => 'text-align:right',
        ),
        array(
          'data' => round($node_password_count * 100 / $protected_count, 2) . '%',
          'style' => 'text-align:right',
        ),
      );
    }

    $form['protected_node_stats']['protected_node_statistics'] = array(
      '#type' => 'markup',
      '#markup' => theme('table', array(
          'header' => $column_labels,
          'rows' => $rows,
        )),
    );
  }

  // Security related options.
  $form['protected_node_security'] = array(
    '#type' => 'fieldset',
    '#title' => t('Protected node security'),
    '#collapsible' => TRUE,
  );

  $form['protected_node_security']['protected_node_use_global_password'] = array(
    '#type' => 'select',
    '#title' => t('Global password handling'),
    '#default_value' => variable_get('protected_node_use_global_password', PROTECTED_NODE_PER_NODE_PASSWORD),
    '#options' => array(
      PROTECTED_NODE_PER_NODE_PASSWORD => t('Per node password'),
      PROTECTED_NODE_PER_NODE_AND_GLOBAL_PASSWORD => t('Per node password or Global password'),
      PROTECTED_NODE_GLOBAL_PASSWORD => t('Global password only'),
    ),
    '#description' => t("Selecting the global password option makes it possible to protect nodes using one password which must be provided in the 'Global password' fields below.
      <br><strong>WARNING:</strong> Removing the global password after it has been assigned to nodes will make those nodes inaccessible except for the user with UID: 1 and the author of the node."),
  );

  $form['protected_node_security']['protected_node_global_password_field'] = array(
    '#type' => 'password_confirm',
    '#title' => t('Global password'),
    '#description' => t('The default password for all nodes. This password is necessary for global password handling.'),
  );

  $form['protected_node_security']['protected_node_show_password_strength'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show password strength in nodes'),
    '#default_value' => variable_get('protected_node_show_password_strength', TRUE),
    '#description' => t('When checked, show the password strength on nodes being edited. This will prevent the use of weak passwords.'),
  );

  $form['protected_node_security']['protected_node_show_node_titles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show node titles by default'),
    '#default_value' => variable_get('protected_node_show_node_titles', FALSE),
    '#description' => t('Whether the node title should be shown by default. This setting can be overridden when creating and editing nodes.'),
  );

  $form['protected_node_security']['protected_node_allow_hint'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow author to enter a password hint'),
    '#default_value' => variable_get('protected_node_allow_hint', FALSE),
    '#description' => t('By default, the password is kept as secret as possible. This option allows the author to enter a hint about the password of a node. The hint can later be shown using the [node:password-hint] token.'),
  );

  $form['protected_node_auto_email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Protected node email support'),
    '#description' => t('This setting allows users who have access to protect a node to send an email about the protected node.'),
    '#collapsible' => TRUE,
  );

  $form['protected_node_auto_email']['protected_node_email_activation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable protected node email support'),
    '#default_value' => variable_get('protected_node_email_activation', FALSE),
    '#description' => t('Check this box to enable the protected node email support (NOTE: Random password support available within this feature).'),
  );

  $form['protected_node_auto_email']['protected_node_email_box_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Email box width'),
    '#size' => 10,
    '#default_value' => variable_get('protected_node_email_box_width', '10'),
    '#description' => t('Enter the width (number of columns) of the email box text area. Must be a positive integer. The default value is 10.'),
    '#states' => array(
      'visible' => array(
        ':input[name="protected_node_email_activation"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['protected_node_auto_email']['protected_node_email_box_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Email box height'),
    '#size' => 10,
    '#default_value' => variable_get('protected_node_email_box_height', '2'),
    '#description' => t('Enter the height (number of rows) of the email box text area. Must be a positive integer. The default value is 2.'),
    '#states' => array(
      'visible' => array(
        ':input[name="protected_node_email_activation"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['protected_node_auto_email']['protected_node_email_from'] = array(
    '#type' => 'textfield',
    '#title' => t('From email address'),
    '#default_value' => variable_get('protected_node_email_from', ''),
    '#description' => t('Enter the email address used in the email header. By default, [site-mail] (@mail) is used, if defined.', array('@mail' => variable_get('site_mail', '<undefined>'))),
    '#states' => array(
      'visible' => array(
        ':input[name="protected_node_email_activation"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['protected_node_auto_email']['protected_node_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Email subject'),
    '#default_value' => variable_get('protected_node_email_subject', protected_node_email_subject()),
    '#description' => t('Enter the subject of the email. You may enter tokens in this field. Remember that [user-name] will be the author name.'),
    '#states' => array(
      'visible' => array(
        ':input[name="protected_node_email_activation"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['protected_node_auto_email']['protected_node_email_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Email content'),
    '#rows' => 15,
    '#default_value' => variable_get('protected_node_email_body', protected_node_email_body()),
    '#description' => t('Enter the body of the email. You may enter tokens in this field. Remember that [user-name] will be the author name.'),
    '#states' => array(
      'visible' => array(
        ':input[name="protected_node_email_activation"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['protected_node_auto_email']['protected_node_random_password'] = array(
    '#type' => 'checkbox',
    '#title' => t('Generate a random password if necessary'),
    '#default_value' => variable_get('protected_node_random_password', FALSE),
    '#description' => t('When this flag is set, saving a protected node without re-entering the password will randomize a password and send it to your users. You may also add your email address to get notified of the password. (It otherwise will be stored in the database encrypted.)'),
    '#states' => array(
      'visible' => array(
        ':input[name="protected_node_email_activation"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['protected_node_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Protected node form'),
    '#collapsible' => TRUE,
  );

  $form['protected_node_form']['protected_node_cancel'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always add a cancel link'),
    '#default_value' => variable_get('protected_node_cancel', 0),
    '#description' => t('Whether a cancel link should be added to the password form. If checked and we do not have a back link, then the cancel is set to &lt;front&gt; instead.'),
  );

  $form['protected_node_form']['protected_node_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Password page title'),
    '#default_value' => variable_get('protected_node_title', t('Protected page -- Enter password')),
    '#description' => t('Enter the title of the protected node page. No HTML allowed. You can use node type tokens provided by the token module if installed.'),
  );

  $form['protected_node_form']['protected_node_info'] = array(
    '#type' => 'textarea',
    '#title' => t('Password page general information'),
    '#default_value' => variable_get('protected_node_info', ''),
    '#description' => t('Enter general information for the protected node page. HTML is allowed. You can use node type tokens provided by the token module if installed.'),
  );

  $form['protected_node_form']['protected_node_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Password page description (inside the field set with the password form)'),
    '#default_value' => variable_get('protected_node_description', ''),
    '#description' => t('Enter custom description for the protected node page. This description is displayed inside the fieldset with the password form. HTML is accepted. You can use node type tokens provided by the token module if installed.'),
  );

  $form['protected_node_form']['protected_node_password_label'] = array(
    '#type' => 'textfield',
    '#title' => t('Password field label on password page'),
    '#default_value' => variable_get('protected_node_password_label', ''),
    '#description' => t('Enter the text for the password label appearing on the password page. The default (when empty) is the node type name followed by the word "password". You can use tokens provided by the token module if installed.'),
  );

  if (module_exists('token')) {
    $vars = array(
      'token_types' => array('global', 'node', 'user'),
    );
    $form['protected_node_form']['protected_node_tokens'] = array(
      '#value' => theme('token_tree', $vars),
      '#description' => t("WARNING: the user tokens should only be used if anonymous users cannot access protected nodes; otherwise the result may not be what you'd expect."),
    );
  }

  // Flood control options.
  $form['protected_node_flood_control'] = array(
    '#type' => 'fieldset',
    '#title' => t('Protected node flood control'),
    '#collapsible' => TRUE,
  );

  $form['protected_node_flood_control']['protected_node_failed_password_ip_limit'] = array(
    '#type' => 'select',
    '#title' => t('Failed password (IP) limit'),
    '#options' => _protected_node_get_failed_password_ip_limit_options(),
    '#default_value' => variable_get('protected_node_failed_password_ip_limit', 50),
    '#description' => t('Defines the number of attempts per user during a limited time window.'),
  );

  $form['protected_node_flood_control']['protected_node_failed_password_ip_window'] = array(
    '#type' => 'select',
    '#title' => t('Failed password (IP) window'),
    '#options' => _protected_node_get_failed_password_ip_window_options(),
    '#default_value' => variable_get('protected_node_failed_password_ip_window', 3600),
    '#description' => t('Defines the time window (in seconds) for the allowed number of attempts.'),
  );

  // View modes.
  $node_info = entity_get_info('node');
  $view_modes = array();
  foreach ($node_info['view modes'] as $id => $item) {
    $view_modes[$id] = $item['label'];
  }
  asort($view_modes);

  $form['protected_node_view_modes'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Display Suite view modes'),
  );
  $form['protected_node_view_modes']['protected_node_checked_view_modes'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Enabled view modes'),
    '#description' => t('Only check passwords for nodes that are rendered with the selected view modes.'),
    '#options' => $view_modes,
    '#default_value' => variable_get('protected_node_checked_view_modes', _protected_node_get_default_checked_view_modes()),
  );

  // A few actions to do some "mass work".
  $form['protected_node_actions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Protected node actions'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Clear Sessions button.
  $form['protected_node_actions']['protected_node_clear_sessions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Clear sessions'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Click on this button to reset all the Protected Node password sessions of your website. This means all the users who entered a password in the past will now have to log back in to review their node protection.'),
  );
  $form['protected_node_actions']['protected_node_clear_sessions']['protected_node_clear_sessions_button'] = array(
    '#type' => 'submit',
    '#value' => t('Clear sessions'),
    '#submit' => array('protected_node_action_clear_sessions'),
  );

  // Reset passwords button.
  $form['protected_node_actions']['protected_node_reset_passwords'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reset passwords'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('WARNING: this feature sets a password on nodes that were otherwise expected to use the global password.'),
  );
  $form['protected_node_actions']['protected_node_reset_passwords']['protected_node_reset_passwords_password'] = array(
    '#type' => 'password_confirm',
    '#title' => t('New Protected Nodes password'),
    '#description' => t('Enter the new password that will be assigned to all existing protected nodes.'),
  );
  $form['protected_node_actions']['protected_node_reset_passwords']['protected_node_reset_passwords_button'] = array(
    '#type' => 'submit',
    '#value' => t('Reset all existing passwords'),
    '#submit' => array('protected_node_action_reset_passwords'),
  );

  // Remove passwords button.
  $form['protected_node_actions']['protected_node_remove_passwords'] = array(
    '#type' => 'fieldset',
    '#title' => t('Use global password'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => '<span style="color: red;">' . t('WARNING: this function deletes all the individual passwords. Make sure you have authorized the use of global password.') . '</span>',
  );
  $form['protected_node_actions']['protected_node_remove_passwords']['protected_node_remove_passwords_button'] = array(
    '#type' => 'submit',
    '#value' => t('Use global password on ALL nodes'),
    '#submit' => array('protected_node_action_remove_passwords'),
  );

  // Remove protection button.
  $form['protected_node_actions']['protected_node_remove_protection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Remove protection'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => '<span style="color: red;">' . t('WARNING: this function unprotects all your currently protected nodes! The password is not modifiable.') . '</span>',
  );
  $form['protected_node_actions']['protected_node_remove_protection']['protected_node_remove_protection_button'] = array(
    '#type' => 'submit',
    '#value' => t('Remove password protection from all nodes'),
    '#submit' => array('protected_node_action_unprotected_all'),
  );

  // Protect ALL nodes buttons.
  $form['protected_node_actions']['protected_node_protect_nodes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Protect nodes'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('WARNING: this function protects ALL your nodes making them accessible only when the password is known. Nodes that do not yet have a password will make use of the global password. You may use the Reset password feature to change that password afterwards.'),
  );
  $form['protected_node_actions']['protected_node_protect_nodes']['protected_node_protect_all_nodes_button'] = array(
    '#type' => 'submit',
    '#value' => t('Protect ALL nodes'),
    '#submit' => array('protected_node_action_protect_all_nodes'),
  );

  // Re-protect nodes buttons.
  $form['protected_node_actions']['protected_node_protect_password'] = array(
    '#type' => 'fieldset',
    '#title' => t('Restore protection'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('WARNING: this function re-protects the nodes that had a password and got unprotected.'),
  );
  $form['protected_node_actions']['protected_node_protect_password']['protected_node_protect_nodes_with_password_button'] = array(
    '#type' => 'submit',
    '#value' => t('Protect nodes that already have a password'),
    '#submit' => array('protected_node_action_protect_nodes_with_password'),
  );

  $form['#validate'][] = '_protected_node_admin_settings_validate';
  $form['#submit'][] = '_protected_node_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Validate the submitted settings form from.
 *
 * This function verifies that a global password is defined if the user
 * defines that the global password should be used by default.
 */
function _protected_node_admin_settings_validate($form, &$form_state) {
  // Make sure that a password is included if a global password is required
  // and none was ever submitted before.
  switch ($form_state['values']['protected_node_use_global_password']) {
    case PROTECTED_NODE_PER_NODE_AND_GLOBAL_PASSWORD:
    case PROTECTED_NODE_GLOBAL_PASSWORD:
      if (!$form_state['values']['protected_node_global_password_field']
       && !variable_get('protected_node_global_password', '')) {
        form_error($form['protected_node_security']['protected_node_global_password_field'],
          t('A global password is required to make use of the global password functionality.'));
      }
      break;

  }

  // Email box width.
  $width = $form_state['values']['protected_node_email_box_width'];
  if (!is_numeric($width)) {
    form_set_error('protected_node_email_box_width', t('You must enter a number for the width of the email box.'));
  }
  else {
    if ($width < 0) {
      form_set_error('protected_node_email_box_width', t('The width of the email box must be positive.'));
    }
  }

  // Email box height.
  $height = $form_state['values']['protected_node_email_box_height'];
  if (!is_numeric($height)) {
    form_set_error('protected_node_email_box_height', t('You must enter a number for the height of the email box.'));
  }
  else {
    if ($height < 0) {
      form_set_error('protected_node_email_box_height', t('The height of the email box must be positive.'));
    }
  }

  if ($form_state['values']['protected_node_email_from']) {
    if (!valid_email_address($form_state['values']['protected_node_email_from'])) {
      form_error($form['protected_node_auto_email']['protected_node_email_from'],
        t('The From email address is not valid.'));
    }
  }
}

/**
 * Submit handler.
 *
 * Submitting the form requires a little bit of work for the password.
 * We need to encrypt it.
 */
function _protected_node_admin_settings_submit($form, &$form_state) {
  $passwd = $form_state['values']['protected_node_global_password_field'];
  if ($passwd) {
    variable_set('protected_node_global_password', hash('sha256', $passwd));
    unset($form_state['values']['protected_node_global_password_field']);
    variable_del('protected_node_global_password_field');
  }
  // Never reset the password...
}

/**
 * Clear all the sessions.
 *
 * This function sets the session variable date to now. Any session that was
 * created before then is then considered out of date and will be ignored.
 */
function protected_node_action_clear_sessions($form, &$form_state) {
  variable_set('protected_node_session_timelimit', REQUEST_TIME);
  drupal_set_message(t('All the Protected Node sessions were cleared.'));
}

/**
 * Reset all the passwords.
 *
 * This function assigns the specified password to all the existing protected
 * nodes.
 */
function protected_node_action_reset_passwords($form, &$form_state) {
  $passwd = $form_state['values']['protected_node_reset_passwords_password'];
  if ($passwd) {
    db_update('protected_nodes')
      ->fields(array(
        'protected_node_passwd' => hash('sha256', $passwd),
      ))
      ->execute();
    variable_set('protected_node_session_timelimit', REQUEST_TIME);
    drupal_set_message(t('All protected nodes are now publicly visible unless protected by another module.'));
  }
  else {
    form_error($form['protected_node_actions']['protected_node_reset_passwords']['protected_node_reset_passwords_password'],
      t('Enter a new password to reset all the protected node passwords.'));
  }
}

/**
 * Remove all the passwords.
 *
 * This function sets all the password to the empty string which has the effect
 * of resetting the password to use the global password on all nodes.
 */
function protected_node_action_remove_passwords($form, &$form_state) {
  db_update('protected_nodes')
    ->fields(array(
      'protected_node_passwd' => '',
    ))
    ->condition('protected_node_passwd', '', '<>')
    ->execute();
}

/**
 * Remove all the protections.
 *
 * This function sets all the protected_node_is_protected to 0.
 */
function protected_node_action_unprotected_all($form, &$form_state) {
  db_update('protected_nodes')
    ->fields(array(
      'protected_node_is_protected' => 0,
    ))
    ->condition('protected_node_is_protected', 1)
    ->execute();
}

/**
 * Protect all nodes.
 *
 * This function adds to the protected nodes list all the existing nodes
 * and marks ALL nodes as protected.
 */
function protected_node_action_protect_all_nodes($form, &$form_state) {
  // First protect all nodes that already are in our table.
  db_update('protected_nodes')
    ->fields(array(
      'protected_node_is_protected' => 1,
    ))
    ->condition('protected_node_is_protected', 0)
    ->execute();

  // Add never protected nodes to protected_nodes.
  $nodes = db_select('node')
    ->fields('node', array('nid'))
    ->execute();

  foreach ($nodes as $node) {
    $nid = $node->nid;

    $nid_in_protected_node = db_select('protected_nodes')
      ->fields('protected_nodes', array('nid'))
      ->condition('nid', $nid)
      ->execute()
      ->fetchfield();

    if (!$nid_in_protected_node) {
      // Add the node in protected_nodes
      db_insert('protected_nodes')
        ->fields(array(
          'nid' => $nid,
          'protected_node_is_protected' => '1',
          'protected_node_show_title' => $form_state['values']['protected_node_show_node_titles'],
        ))
        ->execute();
    }
  }
}

/**
 * Make sure all nodes with a password are protected.
 *
 * It is possible to unprotect a node and still have a password on it. This
 * function restores those nodes protection (i.e. sets the is_protected to 1).
 */
function protected_node_action_protect_nodes_with_password($form, &$form_state) {
  db_update('protected_nodes')
    ->fields(array(
      'protected_node_is_protected' => 1,
    ))
    ->condition('protected_node_is_protected', 0)
    ->condition('protected_node_passwd', '', '<>')
    ->execute();
  drupal_set_message(t('All nodes with an existing password (excluding the global password) are now protected.'));
}

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Add protected node options on node type form.
 *
 * Adds a fieldset to each node type to let user decide whether
 * a node type is always protected, is never protected, or is protect-able.
 * If protect-able, then one can choose whether the protected node field set
 * should be always open, always closed, or smart (i.e. open if the node is
 * protected.)
 *
 * @param[in,out] $form
 *   The node type form where our field set is added.
 */
function protected_node_node_type_form_alter(&$form) {
  $form['protected_node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Protected node settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'additional_settings',
  );

  $options = array(
    PROTECTED_NODE_PROTECTION_NEVER => t('Never protected'),
    PROTECTED_NODE_PROTECTION_PROTECTABLE => t('Protectable (default is unprotected)'),
    PROTECTED_NODE_PROTECTION_PROTECTED => t('Protectable (default is protected)'),
    PROTECTED_NODE_PROTECTION_ALWAYS => t('Always protected'),
  );
  $form['protected_node']['protected_node_protection'] = array(
    '#type' => 'radios',
    '#title' => t('Protected mode for nodes of this type'),
    '#default_value' => variable_get('protected_node_protection_' . $form['#node_type']->type, PROTECTED_NODE_PROTECTION_PROTECTABLE),
    '#options' => $options,
    '#description' => t('Select the protection mode for nodes of this type:<ul>
      <li>Never protected &mdash; the nodes cannot be protected</li>
      <li>Protectable &mdash; lets users choose whether the node is protected, defaults to protected or unprotected.</li>
      <li>Always protected &mdash; the node is automatically protected, the author has no choice.</li>
    </ul>'),
  );

  $form['protected_node']['protected_node_node_type_password_field'] = array(
    '#type' => 'password_confirm',
    '#description' => t('Enter a node type password. This password is the default for all the nodes of this type.'),
    '#size' => 20,
    '#after_build' => array('protected_node_autocomplete_off'),
    '#title' => t('A default (global) password for nodes of this type'),
  );

  $options = array(
    PROTECTED_NODE_FIELDSET_OPEN => t('Always open'),
    PROTECTED_NODE_FIELDSET_SMART => t('Smart mode (Open when protected)'),
    PROTECTED_NODE_FIELDSET_CLOSE => t('Always closed'),
  );
  $form['protected_node']['protected_node_fieldset'] = array(
    '#type' => 'radios',
    '#title' => t('How to show the protected node fieldset'),
    '#default_value' => variable_get('protected_node_fieldset_' . $form['#node_type']->type, PROTECTED_NODE_FIELDSET_SMART),
    '#options' => $options,
    '#description' => t('Select whether the "Protected node" field set should be opened or closed when editing a node.'),
  );

  $form['protected_node']['protected_node_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Password page description (inside the field set with the password form)'),
    '#default_value' => variable_get('protected_node_description_' . $form['#node_type']->type, ''),
    '#description' => t('Enter custom description for the protected node page of this content type to override the global setting. This description is displayed inside the fieldset with the password form. HTML is accepted. You can use node type tokens provided by the token module if installed.'),
  );

  $form['#validate'][] = '_protected_node_node_type_validate';
  $form['#submit'][] = '_protected_node_node_type_submit';
}

/**
 * Implements the validation of the protected node node type form.
 *
 * This function ensures that the password is encrypted.
 */
function _protected_node_node_type_validate($form, &$form_state) {
  if (!empty($form_state['values']['protected_node_node_type_password_field'])) {
    $form_state['values']['protected_node_node_type_password'] = hash('sha256', $form_state['values']['protected_node_node_type_password_field']);
  }
  else {
    $node_type = trim($form_state['values']['type']);
    $old_node_type = isset($form_state['values']['old_type']) ? $form_state['values']['old_type'] : '';
    if ($node_type != $old_node_type) {
      $passwd = variable_get('protected_node_node_type_password_' . $old_node_type, '');
      if ($passwd) {
        // Save only if available, we don't need an empty password.
        $form_state['values']['protected_node_node_type_password'] = $passwd;
      }
      variable_del('protected_node_node_type_password_' . $old_node_type);
    }
  }
  unset($form_state['values']['protected_node_node_type_password_field']);
}

/**
 * Do a little extra work on submission.
 */
function _protected_node_node_type_submit($form, &$form_state) {
  // Make doubly sure the unencrypted field doesn't get saved
  // (it should already be removed in the validate() function).
  unset($form_state['values']['protected_node_node_type_password_field']);

  // Delete the current password_field variable if it exists.
  $node_type = trim($form_state['values']['type']);
  variable_del('protected_node_node_type_password_field_' . $node_type);

  if (!empty($form_state['values']['old_type'])) {
    // Delete the old password field variable if it exists.
    $old_node_type = $form_state['values']['old_type'];
    variable_del('protected_node_node_type_password_field_' . $old_node_type);
  }
}

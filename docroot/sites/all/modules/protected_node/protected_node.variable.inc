<?php

/**
 * @file
 * Variable information.
 */

/**
 * Implements hook_variable_group_info().
 */
function protected_node_variable_group_info() {
  $groups['protected_node'] = array(
    'title' => t('Protected node'),
    'description' => t('Configuration for the protected node module.'),
    'access' => 'administer site configuration',
    'path' => array('admin/config/content/protected_node'),
  );

  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function protected_node_variable_info($options) {
  module_load_include('settings.inc', 'protected_node');

  // Global settings.
  $variables['protected_node_use_global_password'] = array(
    'type' => 'select',
    'title' => t('Global password handling', array(), $options),
    'description' => t("Selecting the global password option makes it possible to protect nodes using one password which must be provided in the 'Global password' fields below.", array(), $options) .
      t('<br><strong>WARNING:</strong> Removing the global password after it has been assigned to nodes will make those nodes inaccessible except for the user with UID: 1 and the author of the node.', array(), $options),
    'option' => array(
      PROTECTED_NODE_PER_NODE_PASSWORD => t('Per node password', array(), $options),
      PROTECTED_NODE_PER_NODE_AND_GLOBAL_PASSWORD => t('Per node password or Global password', array(), $options),
      PROTECTED_NODE_GLOBAL_PASSWORD => t('Global password only', array(), $options),
    ),
    'default' => PROTECTED_NODE_PER_NODE_PASSWORD,
    'group' => 'protected_node',
  );
  $variables['protected_node_global_password'] = array(
    'type' => 'string',
    'title' => t('Global password', array(), $options),
    'description' => t('The default password for all nodes. This password is necessary for global password handling.', array(), $options),
    'default' => '',
    'group' => 'protected_node',
  );
  $variables['protected_node_show_password_strength'] = array(
    'type' => 'boolean',
    'title' => t('Show password strength in nodes', array(), $options),
    'description' => t('When checked, show the password strength on nodes being edited. This will prevent the use of weak passwords.', array(), $options),
    'default' => TRUE,
    'group' => 'protected_node',
  );
  $variables['protected_node_show_node_titles'] = array(
    'type' => 'boolean',
    'title' => t('Show node titles by default', array(), $options),
    'description' => t('Whether the node title should be shown by default. This setting can be overridden when creating and editing nodes.', array(), $options),
    'default' => FALSE,
    'group' => 'protected_node',
  );
  $variables['protected_node_allow_hint'] = array(
    'type' => 'boolean',
    'title' => t('Allow author to enter a password hint', array(), $options),
    'description' => t('By default, the password is kept as secret as possible. This option allows the author to enter a hint about the password of a node. The hint can later be shown using the [node:password-hint] token.', array(), $options),
    'default' => FALSE,
    'group' => 'protected_node',
  );

  // Email features.
  $variables['protected_node_email_activation'] = array(
    'type' => 'boolean',
    'title' => t('Enable protected node email support', array(), $options),
    'description' => t('Check this box to enable the protected node email support (NOTE: Random password support available within this feature).', array(), $options),
    'default' => FALSE,
    'group' => 'protected_node',
  );
  $variables['protected_node_email_box_width'] = array(
    'type' => 'number',
    'title' => t('Email box width', array(), $options),
    'description' => t('Enter the width (number of columns) of the email box text area. Must be a positive integer. The default value is 10.', array(), $options),
    'default' => 10,
    'group' => 'protected_node',
  );
  $variables['protected_node_email_box_height'] = array(
    'type' => 'number',
    'title' => t('Email box height', array(), $options),
    'description' => t('Enter the height (number of rows) of the email box text area. Must be a positive integer. The default value is 2.', array(), $options),
    'default' => 2,
    'group' => 'protected_node',
  );
  $variables['protected_node_email_from'] = array(
    'type' => 'mail_address',
    'title' => t('From email address', array(), $options),
    'description' => t('Enter the email address used in the email header. By default, [site-mail] (@mail) is used, if defined.', array('@mail' => variable_get('site_mail', '<undefined>')), $options),
    'default' => '',
    'group' => 'protected_node',
  );
  $variables['protected_node_email_subject'] = array(
    'type' => 'string',
    'title' => t('Email subject', array(), $options),
    'description' => t('Enter the subject of the email. You may enter tokens in this field. Remember that [user-name] will be the author name.', array(), $options),
    'default callback' => protected_node_email_subject(),
    'group' => 'protected_node',
  );
  $variables['protected_node_email_body'] = array(
    'type' => 'text',
    'title' => t('Email content', array(), $options),
    'description' => t('Enter the body of the email. You may enter tokens in this field. Remember that [user-name] will be the author name.', array(), $options),
    'default callback' => protected_node_email_body(),
    'group' => 'protected_node',
  );
  $variables['protected_node_random_password'] = array(
    'type' => 'boolean',
    'title' => t('Generate a random password if necessary', array(), $options),
    'description' => t('When this flag is set, saving a protected node without re-entering the password will randomize a password and send it to your users.
      You may also add your email address to get notified of the password. (It otherwise will be stored in the database encrypted.)', array(), $options),
    'default' => FALSE,
    'group' => 'protected_node',
  );

  // Protected node form.
  $variables['protected_node_cancel'] = array(
    'type' => 'boolean',
    'title' => t('Always add a cancel link', array(), $options),
    'description' => t('Whether a cancel link should be added to the password form. If checked and we do not have a back link, then the cancel is set to &lt;front&gt; instead.', array(), $options),
    'default' => FALSE,
    'group' => 'protected_node',
  );
  $variables['protected_node_title'] = array(
    'type' => 'string',
    'title' => t('Password page title', array(), $options),
    'description' => t('Enter the title of the protected node page. No HTML allowed. You can use node type tokens provided by the token module if installed.', array(), $options),
    'default' => t('Protected page -- Enter password', array(), $options),
    'group' => 'protected_node',
  );
  $variables['protected_node_info'] = array(
    'type' => 'text',
    'title' => t('Password page general information', array(), $options),
    'description' => t('Enter general information for the protected node page. HTML is accepted. You can use node type tokens provided by the token module if installed.', array(), $options),
    'default' => '',
    'group' => 'protected_node',
  );
  $variables['protected_node_description'] = array(
    'type' => 'text',
    'title' => t('Password page description (inside the field set with the password form)', array(), $options),
    'description' => t('Enter custom description for the protected node page. This description is displayed inside the fieldset with the password form. HTML is allowed. You can use node type tokens provided by the token module if installed.', array(), $options),
    'default' => '',
    'group' => 'protected_node',
  );
  $variables['protected_node_password_label'] = array(
    'type' => 'string',
    'title' => t('Password field label on password page', array(), $options),
    'description' => t('Enter the text for the password label appearing on the password page. The default (when empty) is the node type name followed by the word "password". You can use tokens provided by the token module if installed.', array(), $options),
    'default' => '',
    'group' => 'protected_node',
  );

  // Flood control.
  $variables['protected_node_failed_password_ip_limit'] = array(
    'type' => 'select_number',
    'title' => t('Failed password (IP) limit', array(), $options),
    'description' => t('Defines the number of attempts per user during a limited time window.', array(), $options),
    'option callback' => _protected_node_get_failed_password_ip_limit_options(),
    'default' => 50,
    'group' => 'protected_node',
  );
  $variables['protected_node_failed_password_ip_window'] = array(
    'type' => 'select_number',
    'title' => t('Failed password (IP) window', array(), $options),
    'description' => t('Defines the time window (in seconds) for the allowed number of attempts.', array(), $options),
    'option callback' => _protected_node_get_failed_password_ip_window_options(),
    'default' => 3600,
    'group' => 'protected_node',
  );

  // View modes.
  $node_info = entity_get_info('node');
  $view_modes = array();
  foreach ($node_info['view modes'] as $id => $item) {
    $view_modes[$id] = $item['label'];
  }
  asort($view_modes);

  $variables['protected_node_checked_view_modes'] = array(
    'type' => 'select',
    'title' => t('Enabled view modes', array(), $options),
    'description' => t('Only check passwords for nodes that are rendered with the selected view modes.', array(), $options),
    'option' => $view_modes,
    'default callback' => _protected_node_get_default_checked_view_modes(),
    'group' => 'protected_node',
  );

  // Node type variables.
  $variables['protected_node_protection_[node_type]'] = array(
    'type' => 'multiple',
    'title' => t('Protected mode for nodes of this type', array(), $options),
    'description' => t('Select the protection mode for nodes of this type:<ul><li>Never protected &mdash; the nodes cannot be protected</li>
      <li>Protectable &mdash; lets users choose whether the node is protected, defaults to protected or unprotected.</li>
      <li>Always protected &mdash; the node is automatically protected, the author has no choice.</li></ul>', array(), $options),
    'repeat' => array(
      'type' => 'enable',
      'options' => array(
        PROTECTED_NODE_PROTECTION_NEVER => t('Never protected', array(), $options),
        PROTECTED_NODE_PROTECTION_PROTECTABLE => t('Protectable (default is unprotected)', array(), $options),
        PROTECTED_NODE_PROTECTION_PROTECTED => t('Protectable (default is protected)', array(), $options),
        PROTECTED_NODE_PROTECTION_ALWAYS => t('Always protected', array(), $options),
      ),
      'default' => PROTECTED_NODE_PROTECTION_PROTECTABLE,
    ),
    'group' => 'node_type_settings',
  );
  $variables['protected_node_node_type_password_[node_type]'] = array(
    'type' => 'multiple',
    'title' => t('A default (global) password for nodes of this type', array(), $options),
    'description' => t('Enter a node type password. This password is the default for all the nodes of this type.', array(), $options),
    'repeat' => array(
      'type' => 'string',
      'default' => '',
    ),
    'group' => 'node_type_settings',
  );
  $variables['protected_node_fieldset_[node_type]'] = array(
    'type' => 'multiple',
    'title' => t('How to show the protected node fieldset', array(), $options),
    'description' => t('Select whether the "Protected node" field set should be opened or closed when editing a node.', array(), $options),
    'repeat' => array(
      'type' => 'enable',
      'options' => array(
        PROTECTED_NODE_FIELDSET_OPEN => t('Always open', array(), $options),
        PROTECTED_NODE_FIELDSET_SMART => t('Smart mode (Open when protected)', array(), $options),
        PROTECTED_NODE_FIELDSET_CLOSE => t('Always closed', array(), $options),
      ),
      'default' => PROTECTED_NODE_FIELDSET_SMART,
    ),
    'group' => 'node_type_settings',
  );
  $variables['protected_node_description_[node_type]'] = array(
    'type' => 'multiple',
    'title' => t('Password page description (inside the field set with the password form)', array(), $options),
    'description' => t('Enter custom description for the protected node page of this content type to override the global setting. This description is displayed inside the fieldset with the password form. HTML is accepted. You can use node type tokens provided by the token module if installed.', array(), $options),
    'repeat' => array(
      'type' => 'text',
      'default' => '',
    ),
    'group' => 'node_type_settings',
  );

  return $variables;
}

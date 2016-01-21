<?php

/**
 * @file
 * Admin settings for conversejs.module
 */

/**
 * Form builder for admin/config/services/conversejs.
 */
function conversejs_admin_page($form, &$form_state) {
  $form['conversejs_bosh_server'] = array(
    '#type' => 'textfield',
    '#title' => t('BOSH server address'),
    '#description' => t('Please enter the http address of the BOSH server.') . ' ' . t('<strong>(Supports tokens)</strong>'),
    '#default_value' => conversejs_get_params('conversejs_bosh_server'),
    '#required' => TRUE,
  );

  $form['conversejs_use_proxy'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use built-in proxy'),
    '#description' => t("Connect to BOSH server using built-in proxy to overcome Same Origin Policy restrictions.<br /><strong>Note: this option is useful only when the BOSH server is hosted on another domain other than drupal. If BOSH domain is same as drupal domain, you should disable this option for better performance.</strong>"),
    '#default_value' => conversejs_get_params('conversejs_use_proxy'),
    '#required' => FALSE,
  );

  $form['conversejs_proxy_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Proxy settings'),
    '#states' => array(
      'visible' => array(
        ':input[name="conversejs_use_proxy"]' => array('checked' => TRUE),
      ),
    ),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['conversejs_proxy_settings']['conversejs_proxy_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Built-in proxy address'),
    '#description' => t('Please enter the path to http_bind.php file. You <strong>should not</strong> change this unless you moved this file to another place Enter the path relative to drupal root: @root', array(
        '@root' => url('', array('absolute' => TRUE)),
      )),
    '#default_value' => conversejs_get_params('conversejs_proxy_address'),
    '#required' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="conversejs_use_proxy"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="conversejs_use_proxy"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['conversejs_autologin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto login'),
    '#description' => t('If enabled, the user will be automatically logged-in in chat room when (s)he opens any page in the site.'),
    '#default_value' => conversejs_get_params('conversejs_autologin'),
    '#required' => FALSE,
  );

  $form['conversejs_autologin_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Autologin settings'),
    '#description' => t('These informations will be used to automatically attach a loged in XMPP session to every page. You can use tokens in all text fields.'),
    '#states' => array(
      'visible' => array(
        ':input[name="conversejs_autologin"]' => array('checked' => TRUE),
      ),
    ),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['conversejs_autologin_settings']['conversejs_xmpp_user'] = array(
    '#type' => 'textfield',
    '#title' => t('XMPP user name'),
    '#description' => t('Please enter the xmpp username for auto login.') . ' ' . t('<strong>(Supports tokens)</strong>'),
    '#default_value' => conversejs_get_params('conversejs_xmpp_user'),
    '#required' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="conversejs_autologin"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="conversejs_autologin"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['conversejs_autologin_settings']['conversejs_xmpp_password'] = array(
    '#type' => 'textfield',
    '#title' => t('XMPP password'),
    '#description' => t('Please enter the xmpp password for auto login.') . ' ' . t('<strong>(Supports tokens)</strong>'),
    '#default_value' => conversejs_get_params('conversejs_xmpp_password'),
    '#required' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="conversejs_autologin"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="conversejs_autologin"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['conversejs_autologin_settings']['conversejs_xmpp_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('XMPP domain'),
    '#description' => t('Please enter the xmpp domain name. e.g. example.com') . ' ' . t('<strong>(Supports tokens)</strong>'),
    '#default_value' => conversejs_get_params('conversejs_xmpp_domain'),
    '#required' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="conversejs_autologin"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="conversejs_autologin"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['conversejs_autologin_settings']['conversejs_xmpp_resource'] = array(
    '#type' => 'textfield',
    '#title' => t('XMPP resource name'),
    '#description' => t('Please enter the xmpp resource name. A rendom number will be apended to it when page loads to maintain uniqueness of resource names.') . ' ' . t('<strong>(Supports tokens)</strong>'),
    '#default_value' => conversejs_get_params('conversejs_xmpp_resource'),
    '#required' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="conversejs_autologin"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="conversejs_autologin"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['conversejs_autologin_settings']['conversejs_param_fullname'] = array(
    '#type' => 'textfield',
    '#title' => t("User's full name"),
    '#description' => t('You can specify the fullname of the currently logged in user, otherwise the user’s vCard will be fetched.') . ' ' . t('<strong>(Supports tokens)</strong>'),
    '#default_value' => conversejs_get_params('conversejs_param_fullname'),
    '#required' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="conversejs_autologin"]' => array('checked' => TRUE),
      ),
    ),
  );

  if (module_exists('token')) {
    $form['token_tree'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('user'),
    );
  }
  else {
    $form['token_tree'] = array(
      '#markup' => '<p>' . t('Enable the <a href="@drupal-token">Token module</a> to view the available token browser.', array('@drupal-token' => 'http://drupal.org/project/token')) . '</p>',
    );
  }

  $form['conversejs_visibility_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visibility settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $access = user_access('use PHP for block visibility');
  $options = array(t('Show on every page except the listed pages.'), t('Show on only the listed pages.'));
  $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.",
    array(
      '%blog' => 'blog',
      '%blog-wildcard' => 'blog/*',
      '%front' => '<front>',
    ));

  if ($access) {
    $options[] = t('Show if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
    $description .= ' ' . t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
  }

  $form['conversejs_visibility_settings']['conversejs_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Enable and show converse.js chat room on specific pages'),
    '#options' => $options,
    '#default_value' => conversejs_get_params('conversejs_visibility'),
  );

  $form['conversejs_visibility_settings']['conversejs_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => conversejs_get_params('conversejs_pages'),
    '#description' => $description,
  );

  return system_settings_form($form);
}

/**
 * Form validation callback.
 */
function conversejs_admin_page_validate($form, &$form_state) {
  if ($form_state['values']['conversejs_autologin']) {
    if (empty($form_state['values']['conversejs_xmpp_user'])) {
      form_set_error('conversejs_xmpp_user', t('Please enter XMPP user name.'));
    }
    if (empty($form_state['values']['conversejs_xmpp_password'])) {
      form_set_error('conversejs_xmpp_password', t('Please enter XMPP password.'));
    }
    if (empty($form_state['values']['conversejs_xmpp_domain'])) {
      form_set_error('conversejs_xmpp_domain', t('Please enter XMPP domain name.'));
    }
    if (empty($form_state['values']['conversejs_xmpp_resource'])) {
      form_set_error('conversejs_xmpp_resource', t('Please enter XMPP resource name.'));
    }
  }

  if ($form_state['values']['conversejs_use_proxy']) {
    if (empty($form_state['values']['conversejs_proxy_address'])) {
      form_set_error('conversejs_proxy_address', t('Please enter proxy address.'));
    }
  }
}

/**
 * Form builder for admin/config/services/conversejs/advanced.
 */
function conversejs_admin_page_advanced($form, &$form_state) {
  // Converse.js parameters.
  $form['conversejs_help'] = array(
    '#markup' => t('<p>Configure converse.js parameters. <strong>This is for advanced users.</strong></p><p>Please do not change anything if you are not sure what are you doing.</p>'),
  );

  $form['conversejs_basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic parameters'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Basic parameters. You can customize these options to fit you needs.'),
  );

  $form['conversejs_advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced parameters'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Advnaced parameters. Use these parameters with care.'),
  );

  $form['conversejs_basic']['conversejs_param_allow_contact_requests'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow contact requests'),
    '#description' => t('Allow users to add one another as contacts. If this is unchecked, the Add a contact widget, Contact Requests and Pending Contacts roster sections will all not appear. Additionally, all incoming contact requests will be ignored.'),
    '#default_value' => conversejs_get_params('conversejs_param_allow_contact_requests'),
  );

  $form['conversejs_basic']['conversejs_param_allow_muc'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow multi-user chat'),
    '#description' => t('Allow multi-user chat (muc) in chatrooms. Setting this to false will remove the Chatrooms tab from the control box.'),
    '#default_value' => conversejs_get_params('conversejs_param_allow_muc'),
  );

  $form['conversejs_basic']['conversejs_param_animate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable animations'),
    '#description' => t('Show animations, for example when opening and closing chat boxes.'),
    '#default_value' => conversejs_get_params('conversejs_param_animate'),
  );

  $form['conversejs_basic']['conversejs_param_auto_list_rooms'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatically list rooms'),
    '#description' => t('If checked, and the XMPP server on which the current user is logged in, supports multi-user chat, then a list of rooms on that server will be fetched.<br />Not recommended for servers with lots of chat rooms.<br />For each room on the server a query is made to fetch further details (e.g. features, number of occupants etc.), so on servers with many rooms this option will create lots of extra connection traffic.'),
    '#default_value' => conversejs_get_params('conversejs_param_auto_list_rooms'),
  );

  $form['conversejs_basic']['conversejs_param_auto_reconnect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatic reconnect'),
    '#description' => t('Automatically reconnect to the XMPP server if the connection drops unexpectedly.'),
    '#default_value' => conversejs_get_params('conversejs_param_auto_reconnect'),
  );

  $form['conversejs_basic']['conversejs_param_auto_subscribe'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatic subscription'),
    '#description' => t('If checked, the user will automatically subscribe back to any contact requests.'),
    '#default_value' => conversejs_get_params('conversejs_param_auto_subscribe'),
  );

  $form['conversejs_advanced']['conversejs_param_allow_otr'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow Off-the-record encryption'),
    '#description' => t('Allow !otr', array('!otr' => l(t('OTR (Off-the-record encryption)'), 'https://otr.cypherpunks.ca/'))),
    '#default_value' => conversejs_get_params('conversejs_param_allow_otr'),
  );

  $form['conversejs_advanced']['conversejs_param_use_otr_by_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Off-the-record encryption by default'),
    '#description' => t('If checked, Converse.js will automatically try to initiate an OTR (off-the-record) encrypted chat session every time you open a chat box.'),
    '#default_value' => conversejs_get_params('conversejs_param_use_otr_by_default'),
    '#states' => array(
      'visible' => array(
        ':input[name="conversejs_param_allow_otr"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['conversejs_advanced']['conversejs_param_cache_otr_key'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cache OTR key'),
    '#description' => t("Let the OTR (Off-the-record encryption) private key be cached in your browser’s session storage.<br />The browser’s session storage persists across page loads but is deleted once the tab or window is closed.<br />If this option is unchecked, a new OTR private key will be generated for each page load. While more inconvenient, this is a much more secure option.<br /><strong>Note:</strong> A browser window’s session storage is accessible by all javascript that is served from the same domain. So if there is malicious javascript served by the same server (or somehow injected via an attacker), then they will be able to retrieve your private key and read your all the chat messages in your current session. Previous sessions however cannot be decrypted."),
    '#default_value' => conversejs_get_params('conversejs_param_cache_otr_key'),
    '#states' => array(
      'visible' => array(
        ':input[name="conversejs_param_allow_otr"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['conversejs_advanced']['conversejs_param_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable debugging'),
    '#description' => t('If checked, debugging output will be logged to the browser console.'),
    '#default_value' => conversejs_get_params('conversejs_param_debug'),
  );

  $form['conversejs_advanced']['conversejs_param_expose_rid_and_sid'] = array(
    '#type' => 'checkbox',
    '#title' => t('Expose rid and sid !!Advaned!!'),
    '#description' => t('Allow the prebind tokens, RID (request ID) and SID (session ID), to be exposed globally via the API. This allows other scripts served on the same page to use these values.<br /><strong>Beware</strong>: a malicious script could use these tokens to assume your identity and inject fake chat messages.'),
    '#default_value' => conversejs_get_params('conversejs_param_expose_rid_and_sid'),
  );

  $form['conversejs_basic']['conversejs_param_hide_muc_server'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide muc server'),
    '#description' => t('Hide the server input field of the form inside the Room panel of the controlbox. Useful if you want to restrict users to a specific XMPP server of your choosing.'),
    '#default_value' => conversejs_get_params('conversejs_param_hide_muc_server'),
  );

  $form['conversejs_advanced']['conversejs_param_i18n_locale'] = array(
    '#type' => 'textfield',
    '#title' => t('i18n locale code'),
    '#description' => t('Specify the locale/language. The language must be in the locales object. Refer to ./locale/locales.js to see which locales are supported. Note: If you want to change this, you should also go to Libraries placement section and add appropriate locale files to libraries.'),
    '#default_value' => conversejs_get_params('conversejs_param_i18n_locale'),
    '#required' => TRUE,
    '#size' => 2,
  );

  $form['conversejs_basic']['conversejs_param_show_controlbox_by_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show controlbox by default'),
    '#description' => t('The “controlbox” refers to the special chatbox containing your contacts roster, status widget, chatrooms and other controls.<br />By default this box is hidden and can be toggled by clicking on any element in the page with class toggle-online-users.<br />If this options is checked, the controlbox will by default be shown upon page load.'),
    '#default_value' => conversejs_get_params('conversejs_param_show_controlbox_by_default'),
  );

  $form['conversejs_advanced']['conversejs_param_show_call_button'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show call button'),
    '#description' => t('Enable to display a call button on the chatbox toolbar.<br />When the call button is pressed, it will emit an event that can be used by a third-party library to initiate a call.<br />Please refer to converse.js documentation for more information.'),
    '#default_value' => conversejs_get_params('conversejs_param_show_call_button'),
  );

  $form['conversejs_basic']['conversejs_param_show_only_online_users'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show only online users'),
    '#description' => t('If checked, only online users will be shown in the contacts roster. Users with any other status (e.g. away, busy etc.) will not be shown.'),
    '#default_value' => conversejs_get_params('conversejs_param_show_only_online_users'),
  );

  $form['conversejs_basic']['conversejs_param_use_vcards'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use vCards'),
    '#description' => t('Determines whether the XMPP server will be queried for roster contacts’ VCards or not. VCards contain extra personal information such as your fullname and avatar image.'),
    '#default_value' => conversejs_get_params('conversejs_param_use_vcards'),
  );

  $form['conversejs_advanced']['conversejs_param_xhr_custom_status'] = array(
    '#type' => 'checkbox',
    '#title' => t('XHR custom status'),
    '#description' => t('This option will let converse.js make an AJAX POST with your changed custom chat status to a remote server.'),
    '#default_value' => conversejs_get_params('conversejs_param_xhr_custom_status'),
  );

  $form['conversejs_advanced']['conversejs_param_xhr_custom_status_url'] = array(
    '#type' => 'textfield',
    '#title' => t('XHR custom status URL'),
    '#description' => t('This is the URL to which the AJAX POST request to set the user’s custom status message will be made.<br />The message itself is sent in the request under the key msg.'),
    '#default_value' => conversejs_get_params('conversejs_param_xhr_custom_status_url'),
    '#states' => array(
      'visible' => array(
        ':input[name="conversejs_param_xhr_custom_status"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="conversejs_param_xhr_custom_status"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['conversejs_advanced']['conversejs_param_xhr_user_search'] = array(
    '#type' => 'checkbox',
    '#title' => t('XHR user search'),
    '#description' => t('<p>There are two ways to add users.</p><ul><li>The user inputs a valid JID (Jabber ID), and the user is added as a pending contact.</li><li>The user inputs some text (for example part of a firstname or lastname), an XHR (Ajax Request) will be made to a remote server, and a list of matches are returned. The user can then choose one of the matches to add as a contact.</li></ul><p>This setting enables the second mechanism, otherwise by default the first will be used.</p><p><em>What is expected from the remote server?</em></p><p>A default JSON encoded list of objects must be returned. Each object corresponds to a matched user and needs the keys id and fullname.</p>'),
    '#default_value' => conversejs_get_params('conversejs_param_xhr_user_search'),
  );

  $form['conversejs_advanced']['conversejs_param_xhr_user_search_url'] = array(
    '#type' => 'textfield',
    '#title' => t('XHR user search URL'),
    '#description' => t('This is the URL to which an AJAX GET request will be made to fetch user data from your remote server. The query string will be included in the request with q as its key.'),
    '#default_value' => conversejs_get_params('conversejs_param_xhr_user_search_url'),
    '#states' => array(
      'visible' => array(
        ':input[name="conversejs_param_xhr_user_search"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="conversejs_param_xhr_user_search"]' => array('checked' => TRUE),
      ),
    ),
  );

  return system_settings_form($form);
}

/**
 * Form validation callback.
 */
function conversejs_admin_page_advanced_validate($form, &$form_state) {
  if ($form_state['values']['conversejs_param_xhr_custom_status']) {
    if (empty($form_state['values']['conversejs_param_xhr_custom_status_url'])) {
      form_set_error('conversejs_param_xhr_custom_status_url', t('Please enter XHR custom status URL or disable XHR custom status checkbox.'));
    }
  }

  if ($form_state['values']['conversejs_param_xhr_user_search']) {
    if (empty($form_state['values']['conversejs_param_xhr_user_search_url'])) {
      form_set_error('conversejs_param_xhr_user_search_url', t('Please enter XHR user search URL or disable XHR user search checkbox.'));
    }
  }
}

/**
 * Form builder for admin/config/services/conversejs/libraries.
 */
function conversejs_admin_page_libraries($form, &$form_state) {
  $form['conversejs_help'] = array(
    '#markup' => '<p style="color: red; font-weight: bold;">' . t('These settings are for developers only.') . '</p>' . t('<p>In this page you can reorder or change converse.js libraries.</p><p>Normally, you should not need to change anything here, but if you are using some of these libraries in another place on your site, you may experience some problems and conflicts.<br /> In this case you can remove or change converse.js library paths.</p>'),
  );

  $description = t('You can comment a path by placing a # at front of it.<p><strong>DEVELOPER NOTE:</strong> These paths are passed to drupal_add_js() function.</p>');

  $form['conversejs_libraries_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Converse.js libraries mode'),
    '#description' => t('Please select a mode for javascript libraries.'),
    '#options' => array(
      CONVERSEJS_UNPACKED => t('Unpacked version'),
      CONVERSEJS_PACKED => t('Packed version'),
    ),
    '#default_value' => conversejs_get_params('conversejs_libraries_mode'),
    '#rows' => 10,
  );

  $form['conversejs_libraries_unpacked'] = array(
    '#type' => 'textarea',
    '#title' => t('Converse.js libraries and dependencies (Unpacked)'),
    '#description' => $description,
    '#default_value' => conversejs_get_params('conversejs_libraries_unpacked'),
    '#rows' => 10,
  );

  $form['conversejs_libraries_packed'] = array(
    '#type' => 'textarea',
    '#title' => t('Converse.js libraries and dependencies (Packed)'),
    '#description' => $description,
    '#default_value' => conversejs_get_params('conversejs_libraries_packed'),
    '#rows' => 10,
  );

  return system_settings_form($form);
}

/**
 * Form validation callback.
 */
function conversejs_admin_page_libraries_validate($form, &$form_state) {
}

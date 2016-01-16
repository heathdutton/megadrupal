<?php
/**
 * @file
 * Includes for phpfreechat.module
 */

/**
 * Define data folders (these are relative to Drupal root)
 */
define('PHPFREECHAT_DATA_DIR', variable_get('file_' . file_default_scheme() . '_path', conf_path() . '/files') . '/phpfreechat');
define('PHPFREECHAT_PUBLIC_DIR', variable_get('file_' . file_default_scheme() . '_path', conf_path() . '/files') . '/phpfreechat/public');
define('PHPFREECHAT_PRIVATE_DIR', variable_get('file_' . file_default_scheme() . '_path', conf_path() . '/files') . '/phpfreechat/private');

/**
 * Make sure data directories are created.
 */
function _phpfreechat_check_files() {
  // Create data folders:
  // First parameter is passed as a variable because file_check_directory
  // references the first parameter, and so requires a variable.
  $file_directory_path = variable_get('file_' . file_default_scheme() . '_path', conf_path() . '/files');
    
  return (file_prepare_directory($file_directory_path, FILE_CREATE_DIRECTORY) && 
          file_prepare_directory($f = PHPFREECHAT_DATA_DIR, FILE_CREATE_DIRECTORY) && 
          file_prepare_directory($f = PHPFREECHAT_PRIVATE_DIR, FILE_CREATE_DIRECTORY) && 
          file_prepare_directory($f = PHPFREECHAT_PUBLIC_DIR, FILE_CREATE_DIRECTORY));
}

/**
 * Make sure that third party files exist.
 */
function _phpfreechat_check_install() {
  $install_path = drupal_get_path('module', 'phpfreechat') . '/phpfreechat';
  if (file_exists($install_path . '/src/phpfreechat.class.php') && file_exists($install_path . '/src/pfcinfo.class.php')) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Private hook_help function, to reduce module overhead
 */
function _phpfreechat_help($path, $arg) {
  switch ($path) {
    case 'admin/help#phpfreechat':
      return t('<p>This module allows you to create phpFreeChat chatboxes on any page of the site. You first must enable phpFreeChat for any content types you would like a chatbox on by going to: ' . l('/admin/content/types', 'admin/structure/types') . '.</p>' . '<p>phpFreeChat chatboxes use channels or "rooms". Chatboxes can contain multiple channels, and can connect to one or more channels on startup. By default, one new channel with the name of the page title is created for each chatbox. If you use the \'Default Channels\' setting below, you can have each chatbox join the same channel, creating a site-wide chatroom. You also can override the channel at the content-type or node level, which allows you to create a content-type channel that multiple nodes can share.</p>');

    case 'admin/config/content/phpfreechat':
      return t('<p>This module allows you to create phpFreeChat chatboxes on any page of the site. You first must enable phpFreeChat for any content types you would like a chatbox on by going to ' . l('/admin/content/types', 'admin/structure/types') . '.</p>' . '<p>Please read the ' . l('phpFreeChat help page', 'admin/help/phpfreechat') . ' to learn how to configure chatrooms (aka channels).</p>' . '<p>Don\'t change a setting unless you know what you\'re doing - especially the Advanced Settings. All the phpFreeChat 1.x settings are listed. Some of the settings that appear to be blank (set to the default) have been set programmatically within Drupal, and changing them could crash the chatbox.</p>' . '<p>Note that some setting changes may not take effect until you clear the phpFreeChat cache with the /rehash command (typed into a chatroom).</p>');
  }
}

/**
 * Delete logs of a chatroom.
 * @params $rid (Room ID)
 */
function _phpfreechat_nuke($rid) {
  global $base_url;
  $errors = 0;

  $params = phpfreechat_load_params();
  if (file_unmanaged_delete($params['data_private_path'] . '/chat/' . $rid . '/messages.data') == FALSE) {
    $errors++;
  }
  if (file_unmanaged_delete($params['data_private_path'] . '/chat/' . $rid . '/messages.index') == FALSE) {
    $errors++;
  }
  if ($errors == 0) {
    drupal_set_message('Chatroom contents cleared.');
  }
  else {
    drupal_set_message('Error: Chatroom contents not cleared.');
  }
  drupal_goto('');
}

/**
 * Clears cached data from cookies
 */
function _phpfreechat_clear_cookies() {
  setcookie('phpfreechat', '', REQUEST_TIME - 3600);
  setcookie('pfc_nickmarker', '', REQUEST_TIME - 3600);
  setcookie('pfc_clock', '', REQUEST_TIME - 3600);
  setcookie('pfc_minmax_status', '', REQUEST_TIME - 3600);
  setcookie('pfc_current_text_color', '', REQUEST_TIME - 3600);
  setcookie('pfc_showsmileys', '', REQUEST_TIME - 3600);
  setcookie('pfc_showwhosonline', '', REQUEST_TIME - 3600);
}

/**
 * Clears cached data from the configuration cache
 */
function _phpfreechat_clear_cache() {
  $params = phpfreechat_load_params();
  $params = phpfreechat_prepare_params($params, $node, $user);
  require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'phpfreechat') . '/phpfreechat/src/pfcinfo.class.php';
  $info = new pfcInfo($params['serverid'], $params['data_private_path']);
  $result = $info->rehash();
  drupal_set_message('phpFreeChat settings cache cleared.');
}

/**
 * Private hook_settings function, to reduce module overhead
 */
function _phpfreechat_settings($form, $form_state) {

  if ($form_state['submitted']) {
     _phpfreechat_clear_cache();
    _phpfreechat_clear_cookies();
  }

  $params = phpfreechat_load_params();

  $form['module'] = array(
    '#type' => 'fieldset',
    '#title' => t('phpFreeChat Module Settings'),
  );

  $form['module']['phpfreechat_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Default title'),
    '#default_value' => variable_get('phpfreechat_title', ''),
    '#size' => 70,
    '#maxlength' => 128,
    '#description' => t('This is the default title shown at the top of the chatbox. The default, My Chat, can be overridden at the content-type or node level. If all of these are empty, the node title will be used.'),
  );

  $form['module']['phpfreechat_channels'] = array(
    '#type' => 'textfield',
    '#title' => t('Default chatroom (channel) names'),
    '#default_value' => variable_get('phpfreechat_channels', ''),
    '#size' => 70,
    '#maxlength' => 128,
    '#description' => t('These are the default channels (auto-joined at startup). They can be overridden at the content type or node level. You can enter multiple rooms separated by commas. Leaving this blank will auto-generate a string based on the title parameter.'),
  );

  $form['chat'] = array(
    '#type' => 'fieldset',
    '#title' => t('Chatroom settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['chat']['phpfreechat_theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#options' => _phpfreechat_ls($params['theme_path']),
    '#default_value' => variable_get('phpfreechat_theme', 'default'),
    '#description' => t('This parameter specifies which theme this chatbox will use.'),
  );

  $form['chat']['phpfreechat_language'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#options' => _phpfreechat_ls(drupal_get_path('module', 'phpfreechat') . '/phpfreechat/i18n'),
    '#default_value' => variable_get('phpfreechat_language', 'en_US'),
    '#description' => t('Used to specify the chat labels language. Accepted values are i18n/* sub directories names. (Default value: the local server language)'),
  );

  $form['chat']['phpfreechat_auto_nick'] = array(
    '#type' => 'select',
    '#title' => t('Auto Nick'),
    '#default_value' => variable_get('phpfreechat_auto_nick', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Use the Drupal username as the nick, rather than prompting the user to enter their own nick. (Default value: true)'),
  );

  $form['chat']['phpfreechat_frozen_nick'] = array(
    '#type' => 'select',
    '#title' => t('Frozen Nick'),
    '#default_value' => variable_get('phpfreechat_frozen_nick', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Setting this to true will prevent user from changing their nicknames. (Default value: true)'),
  );

  $form['chat']['phpfreechat_max_nick_len'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum Nick Length'),
    '#default_value' => variable_get('phpfreechat_max_nick_len', ''),
    '#description' => t('This is the maximum nickname length, a longer nickname is forbidden. (Default: 15 characters)'),
  );

  $form['chat']['phpfreechat_nickmeta'] = array(
    '#type' => 'textfield',
    '#title' => t('Nickmeta'),
    '#default_value' => variable_get('phpfreechat_nickmeta', ''),
    '#description' => t('Contains some extra data (metadata) about the user that can be used to customize the display. For example: the user\'s gender, age, real name, etc. can be setup in order to display it in the user\'s info box. A example for gender is : $params["nickmeta"] = array(\'gender\'=>\'f\'); Separate values with commas. (Default value: set internally)'),
  );

  $form['chat']['phpfreechat_nickmeta_private'] = array(
    '#type' => 'textfield',
    '#title' => t('Nickmeta Private'),
    '#default_value' => variable_get('phpfreechat_nickmeta_private', ''),
    '#description' => t('Can be used to set user metadata that is only visible to admins. Separate values with commas. (Default value: array(\'ip\') - means that the user\'s IP address is shown to admins only)'),
  );

  $form['chat']['phpfreechat_nickmeta_key_to_hide'] = array(
    '#type' => 'textfield',
    '#title' => t('Nickmeta Key To Hide'),
    '#default_value' => variable_get('phpfreechat_nickmeta_key_to_hide', ''),
    '#description' => t('Can be used to hide keys in the final displayed whoisbox. Separate values with commas. (Default value: array() - means that nothing is hidden)'),
  );

  $form['chat']['phpfreechat_max_text_len'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum Message Length'),
    '#default_value' => variable_get('phpfreechat_max_text_len', ''),
    '#description' => t('This is the maximum message length, a longer message is forbidden. (Default value: 400)'),
  );

  $form['chat']['phpfreechat_max_msg'] = array(
    '#type' => 'textfield',
    '#title' => t('Message History Length'),
    '#default_value' => variable_get('phpfreechat_max_msg', ''),
    '#description' => t('This is the number of messages kept in the history. When users connect, they can see the history. The number of messages they can see is defined by this. (Default value: 20)'),
  );

  $form['chat']['phpfreechat_startwithsound'] = array(
    '#type' => 'select',
    '#title' => t('Startwithsound'),
    '#default_value' => variable_get('phpfreechat_startwithsound', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t(' Setting it to false will start the chat without sound notifications. (Default value: true)'),
  );

  $form['chat']['phpfreechat_start_minimized'] = array(
    '#type' => 'select',
    '#title' => t('Start Minimized'),
    '#default_value' => variable_get('phpfreechat_start_minimized', 'false'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Setting it to true will start the chat minimized. (Default value: false)'),
  );

  $form['chat']['phpfreechat_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Chat Height'),
    '#default_value' => variable_get('phpfreechat_height', ''),
    '#description' => t('Height of the chat area. (Default value: "440px")'),
  );

  $form['chat']['phpfreechat_shownotice'] = array(
    '#type' => 'textfield',
    '#title' => t('Show Notice'),
    '#default_value' => variable_get('phpfreechat_shownotice', ''),
    '#description' => t('Setting this to 0 will disable all notices. Setting it to 1 will show nicknames changes. Setting it to 2 will show connect/disconnect notifications. Setting it to 3 (1+2) will show nicknames and connect/disconnect notifications. (Default value: 3)'),
  );

  $form['chat']['phpfreechat_nickmarker'] = array(
    '#type' => 'select',
    '#title' => t('Nick Marker'),
    '#default_value' => variable_get('phpfreechat_nickmarker', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Setting this to false will disable nickname colorization. (Default value: true)'),
  );

  $form['chat']['phpfreechat_clock'] = array(
    '#type' => 'select',
    '#title' => t('Clock'),
    '#default_value' => variable_get('phpfreechat_clock', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Setting this to false will hide the date/hour column. (Default value: true)'),
  );

  $form['chat']['phpfreechat_openlinknewwindow'] = array(
    '#type' => 'select',
    '#title' => t('Open Links in New Window'),
    '#default_value' => variable_get('phpfreechat_openlinknewwindow', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Setting this to true will add target="_blank" into parsed links. This attribute is used to open the followed link in a new window. (Default value: true)'),
  );

  $form['chat']['phpfreechat_showwhosonline'] = array(
    '#type' => 'select',
    '#title' => t('Show Who\'s Online'),
    '#default_value' => variable_get('phpfreechat_showwhosonline', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Used to show/hide online users at startup. (Default value: true)'),
  );

  $form['chat']['phpfreechat_showsmileys'] = array(
    '#type' => 'select',
    '#title' => t('Show Smileys'),
    '#default_value' => variable_get('phpfreechat_showsmileys', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Used to show/hide the smiley list at startup. (Default value: true)'),
  );

  $form['chat']['phpfreechat_btn_sh_whosonline'] = array(
    '#type' => 'select',
    '#title' => t('Show Who\'s Online Button'),
    '#default_value' => variable_get('phpfreechat_btn_sh_whosonline', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Used to show/hide the showwhosonline button. (Default value: true)'),
  );

  $form['chat']['phpfreechat_btn_sh_smileys'] = array(
    '#type' => 'select',
    '#title' => t('Show Smileys Button'),
    '#default_value' => variable_get('phpfreechat_btn_sh_smileys', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Used to show/hide the showsmileys button. (Default value: true)'),
  );

  $form['chat']['phpfreechat_display_ping'] = array(
    '#type' => 'select',
    '#title' => t('Display Ping'),
    '#default_value' => variable_get('phpfreechat_display_ping', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Used to show/hide the ping information near the phpfreechat linkback logo. The ping is the time between a client request and a server response. More the ping is low, faster the chat is responding. (Default value: true)'),
  );

  $form['chat']['phpfreechat_display_pfc_logo'] = array(
    '#type' => 'select',
    '#title' => t('Display Pfc Logo'),
    '#default_value' => variable_get('phpfreechat_display_pfc_logo', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Used to hide the phpfreechat linkback logo. Be sure that you are conform to the license page before setting this to false! (Default value: true)'),
  );

  $form['chat']['phpfreechat_displaytabimage'] = array(
    '#type' => 'select',
    '#title' => t('Displaytabimage'),
    '#default_value' => variable_get('phpfreechat_displaytabimage', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Used to show/hide the images in the channels and pv tabs. (Default value: true)'),
  );

  $form['chat']['phpfreechat_displaytabclosebutton'] = array(
    '#type' => 'select',
    '#title' => t('Displaytabclosebutton'),
    '#default_value' => variable_get('phpfreechat_displaytabclosebutton', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Used to show/hide the close button in the channels tabs. (Default value: true)'),
  );

  $form['chat']['phpfreechat_bbcode_colorlist'] = array(
    '#type' => 'textfield',
    '#title' => t('Bbcode Colorlist'),
    '#default_value' => variable_get('phpfreechat_bbcode_colorlist', ''),
    '#description' => t('This is the list of colors that will appears into the bbcode palette. Separate values with commas. (Default value: contains an array of basic colors: \'#FFFFFF\', \'#000000\', ...)'),
  );

  $form['chat']['phpfreechat_nickname_colorlist'] = array(
    '#type' => 'textfield',
    '#title' => t('Nickname Colorlist'),
    '#default_value' => variable_get('phpfreechat_nickname_colorlist', ''),
    '#description' => t('This is the list of colors that will be used to automaticaly and randomly colorize the nicknames in the chat. Separate values with commas. (Default value: contains an array of basic colors: \'#CCCCCC\',\'#000000\')'),
  );

  $form['chat']['phpfreechat_max_displayed_lines'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Displayed Lines'),
    '#default_value' => variable_get('phpfreechat_max_displayed_lines', ''),
    '#description' => t('The maximum number of lines displayed in the window. Old lines will be deleted to save browser\'s memory on clients. Default value: 150)'),
  );

  $form['chat']['phpfreechat_date_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Date Format'),
    '#default_value' => variable_get('phpfreechat_date_format', ''),
    '#description' => t('How to display the dates in the chat. (Default value: \'d/m/Y\')'),
  );

  $form['chat']['phpfreechat_time_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Time Format'),
    '#default_value' => variable_get('phpfreechat_time_format', ''),
    '#description' => t('How to display the time in the chat (Default value: \'H:i:s\')'),
  );

  $form['chat']['phpfreechat_time_offset'] = array(
    '#type' => 'textfield',
    '#title' => t('Time Offset'),
    '#default_value' => variable_get('phpfreechat_time_offset', ''),
    '#description' => t('Can be used to setup the chat time zone. It is the difference in seconds between chat clock and server clock. (Default value: 0)'),
  );

  $form['chat']['phpfreechat_short_url'] = array(
    '#type' => 'select',
    '#title' => t('Short Url'),
    '#default_value' => variable_get('phpfreechat_short_url', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Setting it to true will shorten long URLs entered by users in the chat area. (Default value: true)'),
  );

  $form['chat']['phpfreechat_short_url_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Short Url Width'),
    '#default_value' => variable_get('phpfreechat_short_url_width', ''),
    '#description' => t('Final width of the shortened URL in characters. (This includes the elipsis on shortened URLs.) This parameter is taken into account only when short_url is true. (Default value: 40)'),
  );

  $form['chat']['phpfreechat_notify_window'] = array(
    '#type' => 'select',
    '#title' => t('Notify Window'),
    '#default_value' => variable_get('phpfreechat_notify_window', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Setting it to false will disable the window title notification. When a message is received and this parameter is true, the window title is modified with [n] (n is the number of new posted messages). (Default value: true)'),
  );

  $form['system'] = array(
    '#type' => 'fieldset',
    '#title' => t('Performance & Output Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['system']['phpfreechat_container_type'] = array(
    '#type' => 'textfield',
    '#title' => t('Container Type'),
    '#default_value' => variable_get('phpfreechat_container_type', ''),
    '#description' => t('Used to specify the chat container (chat database). Accepted containers are : File and Mysql (maybe others in the future). (Default value: \'File\')'),
  );

  $form['system']['phpfreechat_refresh_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Refresh Delay'),
    '#default_value' => variable_get('phpfreechat_refresh_delay', ''),
    '#description' => t('This is the time to wait (in milliseconds) between two refreshes. A refresh is a HTTP request which asks the server if there are new messages to display. If there are no new messages, then a empty HTTP response is returned. This parameter will be dynamically changed depending on the chat activity, see refresh_delay_steps parameter for more information. (Default value: 2000 it means 2000 ms or 2 seconds)'),
  );

  $form['system']['phpfreechat_refresh_delay_steps'] = array(
    '#type' => 'textfield',
    '#title' => t('Refresh Delay Steps'),
    '#default_value' => variable_get('phpfreechat_refresh_delay_steps', ''),
    '#description' => t('This parameter is used to control the refresh_delay value dynamically. The more active the chat, the lower the refresh_delay, making the chat more responsive. The first parameter is a refresh delay value, the second is a time inactivity boundary etc. Separate values with commas. (Default value: array (2000,20000,3000,60000 ... that means: start with 2s delay after 20s of inactivity, 3s delay after 60s of inactivity ...)'),
  );

  $form['system']['phpfreechat_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout'),
    '#default_value' => variable_get('phpfreechat_timeout', ''),
    '#description' => t('This is the time of inactivity to wait before considering a user is disconnected (in milliseconds).
         A user is inactive only if s/he closed his/her chat window. A user with an open chat window is not
         inactive because s/he sends each refresh_delay an HTTP request. (Default value: 35000 it means
         35000 ms or 35 seconds)'),
  );

  $form['system']['phpfreechat_connect_at_startup'] = array(
    '#type' => 'select',
    '#title' => t('Connect At Startup'),
    '#default_value' => variable_get('phpfreechat_connect_at_startup', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Setting this to false will oblige user to click on the connect button if s/he wants to chat. (Default value: true - a connection to the chat is automaticaly performed)'),
  );

  $form['system']['phpfreechat_output_encoding'] = array(
    '#type' => 'textfield',
    '#title' => t('Output Encoding'),
    '#default_value' => variable_get('phpfreechat_output_encoding', ''),
    '#description' => t('Set a sepcific encoding for chat labels. This is really useful when the Web page embedding the chat is not UTF-8 encoded. This parameter should be the same as the chat web page. Could be ISO-8859-1 or anything else but it must be supported by iconv php module. (Default value: UTF-8)'),
  );

  $form['system']['phpfreechat_frozen_channels'] = array(
    '#type' => 'textfield',
    '#title' => t('Frozen Channels'),
    '#default_value' => variable_get('phpfreechat_frozen_channels', ''),
    '#description' => t('This parameter can be used to restrict channels to users. If the array is empty, it allows users to create their own channels. Separate values with commas. (Default value: empty array)'),
  );

  $form['system']['phpfreechat_max_channels'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Channels'),
    '#default_value' => variable_get('phpfreechat_max_channels', ''),
    '#description' => t('The maximum number of allowed channels for each user. (Default value: 10)'),
  );

  $form['system']['phpfreechat_privmsg'] = array(
    '#type' => 'textfield',
    '#title' => t('Privmsg'),
    '#default_value' => variable_get('phpfreechat_privmsg', ''),
    '#description' => t('This array contains the nicknames list you want to initiate a private message at chat loading. Of course, the listed nicknames should be online or it will just be ignored. Separate values with commas. (Default value: empty array)'),
  );

  $form['system']['phpfreechat_max_privmsg'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Privmsg'),
    '#default_value' => variable_get('phpfreechat_max_privmsg', ''),
    '#description' => t('This is the maximum number of private message allowed at the same time for one user. (Default value: 5)'),
  );

  $form['system']['phpfreechat_islocked'] = array(
    '#type' => 'select',
    '#title' => t('Islocked'),
    '#default_value' => variable_get('phpfreechat_islocked', 'false'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('When this parameter is true, all the chatters will be redirected to the url indicated by the lockurl parameter. (Default value: false)'),
  );

  $form['system']['phpfreechat_lockurl'] = array(
    '#type' => 'textfield',
    '#title' => t('Lockurl'),
    '#default_value' => variable_get('phpfreechat_lockurl', ''),
    '#description' => t('This url is used when islocked parameter is true. The users will be redirected (http redirect) to this url. (Default value: http://www.phpfreechat.net)'),
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // For debugging.
  $form['advanced']['phpfreechat_params'] = array(
    '#type' => 'select',
    '#title' => t('Debug Parameters'),
    // Set to "true" to turn on debugging.
    '#default_value' => variable_get('phpfreechat_params', 'false'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('For developers. Setting this to true will list the chat parameters, session and cookies. (Default value: false)'),
  );

  // For debugging.
  $form['advanced']['phpfreechat_debug'] = array(
    '#type' => 'select',
    '#title' => t('Debug Console'),
    '#default_value' => variable_get('phpfreechat_debug', 'false'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('For developers. When debug is true, some traces will be shown on the chat clients (Default value: false)'),
  );

  $form['advanced']['phpfreechat_serverid'] = array(
    '#type' => 'textfield',
    '#title' => t('Serverid'),
    '#default_value' => variable_get('phpfreechat_serverid', ''),
    '#description' => t('This is the only mandatory parameter used to identify the chat server. You can compare it to the server ip/host like on an IRC server. If you don\'t know what to write, just try : $params["serverid"] = md5(__FILE__); (Default value: set internally)'),
  );

  $form['advanced']['phpfreechat_server_script_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Server Script Path'),
    '#default_value' => variable_get('phpfreechat_server_script_path', ''),
    '#description' => t('Used to specify the script that will handle asynchronous requests. Very useful when the chat (client) script is resource consuming (ex: forum or portal chat integration). (Default value: \'\' - means this parameter is automatically calculated)'),
  );

  $form['advanced']['phpfreechat_server_script_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Server Script URL'),
    '#default_value' => variable_get('phpfreechat_server_script_url', ''),
    '#description' => t('This url indicates the server_script_path. It will be used to do AJAX requests from the browser. Therefore, this URL should be a browsable public url. This parameter is useful when using URL rewriting because basic auto-calculation will fail. (Default value: \'\' - means this parameter is automatically calculated)'),
  );

  $form['advanced']['phpfreechat_client_script_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Client Script Path'),
    '#default_value' => variable_get('phpfreechat_client_script_path', ''),
    '#description' => t('Used to specify the script path which first displays the chat. This path will be used to calculate relatives paths for resources: javascript lib and images. Useful when the PHP configuration is uncommon. This option can be used to force the automatic detection process. (Default value: \'\' - means this parameter is automatically calculated)'),
  );

  // No "client_script_url" in the code, oddly.

  $form['advanced']['phpfreechat_cmd_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Commands Directory'),
    '#default_value' => variable_get('phpfreechat_cmd_path', ''),
    '#description' => t('This parameter indicates your own commands directory location. The chat uses commands to communicate between client and server. As an example, when a message is sent, the /send your message command is used, when a nickname is changed, the /nick newnickname command is used. To create a new command, you have to write it and indicate in this parameter where it is located. (Default value: empty string - means no custom command path is used)'),
  );

  $form['advanced']['phpfreechat_cmd_path_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Cmd Path Default'),
    '#default_value' => variable_get('phpfreechat_cmd_path_default', ''),
    '#description' => t('Contains the default command path used by the system. Do not change this parameter if you don\'t know what you are doing. If you try to add your own command, check the cmd_path parameter. (Default value: dirname(__FILE__).\'/commands\')'),
  );

  $form['advanced']['phpfreechat_theme_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Theme Path'),
    '#default_value' => variable_get('phpfreechat_theme_path', ''),
    '#description' => t('Indicates where the themes are located. Use this parameter if you want to store your own theme in a special location. (Default value: the same as theme_default_path)'),
  );

  $form['advanced']['phpfreechat_theme_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Theme Url'),
    '#default_value' => variable_get('phpfreechat_theme_url', ''),
    '#description' => t('This url indicates the theme_path location. It will be used by the browser to load theme resources : images, css, js. If this parameter is not indicated, the themes will be copied to data_public_path/themes and this parameter value will be set to data_public_url/theme. (Default value: \'\')'),
  );

  $form['advanced']['phpfreechat_theme_default_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Theme Default Path'),
    '#default_value' => variable_get('phpfreechat_theme_default_path', ''),
    '#description' => t('Indicate where the official pfc default theme is located. Do not change this parameter if you don\'t know what you are doing. If you try to add your own theme, check the theme_path parameter. (Default value: \'\' - empty string means dirname(__FILE__).\'/../themes\' is used automatically)'),
  );

  $form['advanced']['phpfreechat_theme_default_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Theme Default Url'),
    '#default_value' => variable_get('phpfreechat_theme_default_url', ''),
    '#description' => t('This url indicates the theme_default_path location. Do not change this parameter if you don\'t know what you are doing. If you try to add your own theme, check the theme_path parameter. (Default value: the theme is copied into data_public_path/themes and this parameter will be set to data_public_url/theme)'),
  );

  $form['advanced']['phpfreechat_data_private_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Data Private Path'),
    '#default_value' => variable_get('phpfreechat_data_private_path', ''),
    '#description' => t('Used by the filesystem container to store chat data. Used by smarty to store compiled templates. Tip: you can optimize your chat performance, by pointing this to a RAM filesystem mount. (Default value: \'\' - means dirname(__FILE__)."/../data/private" is used automatically)'),
  );

  $form['advanced']['phpfreechat_data_public_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Data Public Path'),
    '#default_value' => variable_get('phpfreechat_data_public_path', ''),
    '#description' => t('This path must be reachable by your web server. Javascript and every resources (theme) files will be stored here. (Default value: \'\' - means dirname(__FILE__)."/../data/public" is used automatically)'),
  );

  $form['advanced']['phpfreechat_data_public_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Data Public URL'),
    '#default_value' => variable_get('phpfreechat_data_public_url', ''),
    '#description' => t('This URL should link to the data_private_path directory so that the clients\' browsers will be able to load needed javascript files and theme resources. It can be useful when url rewriting is done on the server. (Default value: \'\' - means this parameter is automatically calculated from data_private_path)'),
  );

  $form['advanced']['phpfreechat_prototypejs_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Prototypejs Url'),
    '#default_value' => variable_get('phpfreechat_prototypejs_url', ''),
    '#description' => t('This is the prototype javascript library URL. Use this parameter to use your external library. (Default value: \'\' - means data/js/prototype.js is used automatically)'),
  );

  $form['advanced']['phpfreechat_skip_proxies'] = array(
    '#type' => 'textfield',
    '#title' => t('Skip Proxies'),
    '#default_value' => variable_get('phpfreechat_skip_proxies', ''),
    '#description' => t('Contains the list of proxies to ingore. For example: append \'censor\' to the list to disable words censoring. The list of system proxies can be found in src/proxies/. Attention: \'checktimeout\' and \'checknickchange\' proxies should not be disabled or the chat will not work anymore. Separate values with commas. (Default value: empty array - no proxies will be skipped)'),
  );

  $form['advanced']['phpfreechat_post_proxies'] = array(
    '#type' => 'textfield',
    '#title' => t('Post Proxies'),
    '#default_value' => variable_get('phpfreechat_post_proxies', ''),
    '#description' => t('This array contains the proxies that will be handled just before to process a command and just after the system proxies. You can use this array to execute your own proxy. Separate values with commas. (Default value: empty array)'),
  );

  $form['advanced']['phpfreechat_pre_proxies'] = array(
    '#type' => 'textfield',
    '#title' => t('Pre Proxies'),
    '#default_value' => variable_get('phpfreechat_pre_proxies', ''),
    '#description' => t('This array ocntains the proxies that will be handled just before system proxies. You can use this array to execute your own proxy. Separate values with commas. (Default value: empty array)'),
  );

  $form['advanced']['phpfreechat_proxies_cfg'] = array(
    '#type' => 'textfield',
    '#title' => t('Proxies Cfg'),
    '#default_value' => variable_get('phpfreechat_proxies_cfg', ''),
    '#description' => t('Contains the proxies configuration. TODO: explain the possible values for each proxies.'),
  );

  $form['advanced']['phpfreechat_proxies_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Proxies Path'),
    '#default_value' => variable_get('phpfreechat_proxies_path', ''),
    '#description' => t('A custom proxies path. Used to easily plugin your own proxy to the chat without modifying the code. (Default value: empty path)'),
  );

  $form['advanced']['phpfreechat_proxies_path_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Proxies Path Default'),
    '#default_value' => variable_get('phpfreechat_proxies_path_default', ''),
    '#description' => t('Contains the default proxies location. Do not change this parameter if you don\'t know what you are doing. If you try to add your own proxy, check the proxies_path parameter. (Default value: dirname(__FILE__).\'/proxies\')'),
  );

  $form['advanced']['phpfreechat_get_ip_from_xforwardedfor'] = array(
    '#type' => 'select',
    '#title' => t('Get Ip From Xforwardedfor'),
    '#default_value' => variable_get('phpfreechat_get_ip_from_xforwardedfor', 'false'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('This parameter is useful when your chat server is behind a reverse proxy that forwards client ip address in HTTP_X_FORWARDED_FOR http header. Some discussions about this parameter are available on the forum. (Default value: false)'),
  );

  $form['advanced']['phpfreechat_dyn_params'] = array(
    '#type' => 'textfield',
    '#title' => t('Dyn Params'),
    '#default_value' => variable_get('phpfreechat_dyn_params', ''),
    '#description' => t('Most of the chat parameters are stored in a internal cache for performances issues. It means that for all the clients the chat will have the same parameters. However sometime you need to customize some parameters for each clients. For example: the \'language\' parameter could depends on the chatter profil so it could interesting to ignore the cache for this parameter. The \'dyn_params\' contains the parameters that need to be dynamic (not stored in the cache). Separate values with commas. (Default value: array())'),
  );

  $form['advanced']['phpfreechat_focus_on_connect'] = array(
    '#type' => 'select',
    '#title' => t('Focus On Connect'),
    '#default_value' => variable_get('phpfreechat_focus_on_connect', 'true'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Setting this to true will give the focus to the input text box when connecting to the chat. It can be useful not to touch the focus when integrating the chat into an existing website because when the focus is changed, the viewport follows the focus location. (Default value: true)'),
  );

  $form['advanced']['phpfreechat_quit_on_closedwindow'] = array(
    '#type' => 'select',
    '#title' => t('Quit On Closedwindow'),
    '#default_value' => variable_get('phpfreechat_quit_on_closedwindow', 'false'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Setting this to true will send a /quit command when the user closes his/her window. (NOTE: Doesn\'t work on Firefox). This parameter isn\'t true by default because on IE and Konqueror/Safari, reloading the window (F5) will generate the same event as closing the window which can be annoying. (Default value: false)'),
  );

  $form['advanced']['phpfreechat_admins'] = array(
    '#type' => 'textfield',
    '#title' => t('Admins'),
    '#default_value' => variable_get('phpfreechat_admins', ''),
    '#description' => t('This parameter contains a list of key/value that identify admin access. The keys are the nicknames and the values are the corresponding passwords. Note: The "isadmin" parameter does not depend on this variable. Separate values with commas. (Default value: No admin/password accounts are available. Don\'t forget to change it.)'),
  );

  $form['advanced']['phpfreechat_isadmin'] = array(
    '#type' => 'select',
    '#title' => t('Isadmin'),
    '#default_value' => variable_get('phpfreechat_isadmin', 'false'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('Set this parameter to true if you want to give admin rights to the connected user. Attention : if you don\'t use any external registration system, all your users will be admins. You have to test current user rights before setting this parameter to true. (Default value: false)'),
  );

  $form['advanced']['phpfreechat_firstisadmin'] = array(
    '#type' => 'select',
    '#title' => t('Firstisadmin'),
    '#default_value' => variable_get('phpfreechat_firstisadmin', 'false'),
    '#options' => array(
      'true' => 'true',
      'false' => 'false',
    ),
    '#description' => t('When this parameter is true, it gives admin rights to the first connected user on the server. (Default value: false)'),
  );

  return $form;
}

/**
 * Return a 'not found' message
 */
function _phpfreechat_not_found() {
  $message = t('phpFreeChat script is not found.<br />Before you can use this module you must download the script from <a href="http://www.phpfreechat.net/">http://www.phpfreechat.net/</a> and extract in to the /%directory directory on your web server.<br />See INSTALL.txt for more details.', array('%directory' => drupal_get_path('module', 'phpfreechat') . '/phpfreechat'));
  watchdog('phpfreechat', $message, 'WATCHDOG_ERROR');
  return $message;
}

/**
 * Returns a list of files/folders in a path.
 *
 */
function _phpfreechat_ls($dir) {
  // scan the path
  $list = file_scan_directory($dir, '/.*/', array('recurse' => FALSE, 'key' => 'filename'));
  // get only filenames
  $list = array_keys($list);
  // order according to file names descendingly
  sort($list);
  // convert a an associative array and return it
  return drupal_map_assoc($list);
}

/**
 * Recursively deletes everything in a folder, but not the folder itself.
 */
function _phpfreechat_rm($dir) {
  if (!$dh = @opendir($dir)) {
    return;
  }
  while (($obj = @readdir($dh))) {
    if ($obj == '.' || $obj == '..') {
      continue;
    }
    if (!@unlink($dir . '/' . $obj)) {
      _phpfreechat_rm($dir . '/' . $obj);
    }
  }
  @closedir($dh);
  @rmdir($dir);
}

/**
 * Programmatically issues a phpFreeChat /quit command.
 *
 */
function _phpfreechat_quit() {
  global $user, $base_url;
    
  // We will need to load the current node. Use HTTP_REFERER to extract the node ID.
  $path = str_replace(array($base_url . '/', '/?q='), '', $_SERVER['HTTP_REFERER']);
  // Get the real path from the current request
  $args = explode('/', drupal_get_normal_path($path));
  //debug 
  /*
  echo "path: $path<br>";
  echo "base_url: $base_url<br>";
  echo "#_SERVER['HTTP_REFERER']: ".$_SERVER['HTTP_REFERER']."<br>";
  echo "<pre>";
  print_r($args);
  echo "</pre>";
  */
  /* OUTPUT:
  path: chatbox
  base_url: http://drup6.procrastinators-anonymous.org
  #_SERVER['HTTP_REFERER']: http://drup6.procrastinators-anonymous.org/chatbox

  Array
  (
      [0] => node
      [1] => 10
  )
  */

  $result = db_query('SELECT * FROM {node} n INNER JOIN {phpfreechat} pfc ON pfc.nid = n.nid WHERE n.nid = :nid', array(':nid' => $args[1]));
  $node = $result->fetchObject();
    
  require_once drupal_get_path('module', 'phpfreechat') . '/phpfreechat.module';
  $params = phpfreechat_load_params(); // Load global params
  $params = phpfreechat_prepare_params($params, $node, $user); // Prepare params for current node
  require_once drupal_get_path('module', 'phpfreechat') . '/phpfreechat/src/phpfreechat.class.php';
  $pfc = new phpFreeChat($params);
    
  // need to leave all public and private rooms before quitting or username switches to Anonymous.
  $p = array(); // command parameter array
  $xml_reponse = new pfcResponse(); // response object

  $clientid    = $p["clientid"];
  $param       = $p["param"];
  $sender      = $p["sender"];
  $recipient   = $p["recipient"];
  $recipientid = $p["recipientid"];
  
  $c  =& pfcGlobalConfig::Instance();
  $u  =& pfcUserConfig::Instance();
  $ct =& pfcContainer::Instance();

  $nick = $ct->getNickname($u->nickid);

  // leave the channels
  foreach( $u->channels as $id => $chandetail ) {
    if ($ct->removeNick($chandetail["recipient"], $u->nickid)) {
      $cmdp = $p;
      $cmdp["param"] = $id;
      $cmdp["recipient"] = $chandetail["recipient"];
      $cmdp["recipientid"] = $id;
      $cmd =& pfcCommand::Factory("leave");
      $cmd->run($xml_reponse, $cmdp);
    }
  }

  // leave the private messages
  foreach( $u->privmsg as $id => $pvdetail ) {
    if ($ct->removeNick($pvdetail["recipient"], $u->nickid)) {
      $cmdp = $p;
      $cmdp["param"] = $id;
      $cmdp["recipient"] = $pvdetail["recipient"];
      $cmdp["recipientid"] = $id;        
      $cmd =& pfcCommand::Factory("leave");
      $cmd->run($xml_reponse, $cmdp);
    }
  }

  // leave the server
  $ct->removeNick(NULL, $u->nickid);

  $xml_reponse->script("pfc.handleResponse('quit', 'ok', '');");
    
}

?>

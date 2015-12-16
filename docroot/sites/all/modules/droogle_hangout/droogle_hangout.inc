<?php

/**
 * @file
 * Google Calendar api connection
 * and google calendar form creation.
 */

/**
 * Callback to create Google Hangout.
 */
function droogle_hangout_simple_button_ajax() {
  global $base_url;
  global $user;
  $ajax_email = $_POST['ajaxemail'];
  $email_array = explode(",", $ajax_email);
  $caldate = isset($_POST['caldate']) ? $_POST['caldate'] : '';

  foreach ($email_array as $email) {
    $json_attendees[] = ', { "email": "' . $email . '" }';
  }

  $json_attendee = implode(",", $json_attendees);
  $json_attendee = str_replace(",,", ",", $json_attendee);

  $access_token = NULL;
  if (empty($access_token)) {
    if ($path = libraries_get_path('google-api-php-client')) {
      require_once $path . '/src/Google_Client.php';
      require_once $path . '/src/contrib/Google_CalendarService.php';
    }

    // Console account email, client id, client secret
    // and refresh token entered on admin screen.
    $droogle_hangout_master_email = check_plain(variable_get('droogle_hangout_master_account_email', ''));
    $droogle_hangout_clientid = check_plain(variable_get('droogle_hangout_clientid', ''));
    $droogle_hangout_client_secret = check_plain(variable_get('droogle_hangout_client_secret', ''));
    $droogle_hangout_refresh_token = check_plain(variable_get('droogle_hangout_refresh_token', ''));
    // Initialize access to Google.
    $client = new Google_Client();
    $client->setClientId($droogle_hangout_clientid);
    $client->setClientSecret($droogle_hangout_client_secret);
    $client->setRedirectUri("$base_url/droogle_hangout_get_token");
    $client->setAccessType('offline');

    // Initialize access to Calendar as service.
    $service = new Google_CalendarService($client);
    // If Access Token Expired (uses Google_OAuth2 class),
    // refresh access token by refresh token.
    if ($client->isAccessTokenExpired()) {
      $client->refreshToken($droogle_hangout_refresh_token);
    }
    // If client got access token successfuly - perform operations.
    $access_tokens = json_decode($client->getAccessToken());
    $new_access_token = $access_tokens->access_token;
  }
  $access_token = $new_access_token;
  if ($caldate == '') {
    $start = date3339(time());
    $end = date3339(strtotime('+1 hour', time()));
  }
  else {
    $start_timestamp = strtotime($caldate);
    $start = date3339($start_timestamp);
    $end = date3339(strtotime('+1 hour', $start_timestamp));
  }
  $calendar_output = shell_exec('curl "https://www.googleapis.com/calendar/v3/calendars/' . $droogle_hangout_master_email . '/events?access_token=' . $access_token . '" -H "Content-Type: application/json" -d \' { "summary": "Google Hangout", "start": { "dateTime": "' . $start . '" }, "end": { "dateTime": "' . $end . '" }, "attendees": [ { "email": "{' . $user->mail . '}" }' . $json_attendee . ' ], "reminders": { "overrides":[ ] } }\' -v');
  $calendar_json = drupal_json_decode($calendar_output);
  $hangout_link = $calendar_json['hangoutLink'];

  // Send email out.
  $to = $user->mail;
  $cc = $_POST['ajaxemail'];
  $subj = 'You are invited to a Google Hangout';
  $scheduled_time = $caldate != '' ? 'The hangout is scheduled for ' . $caldate . '.  ' : '';
  $body = $scheduled_time . 'Please visit ' . $hangout_link . ' to join the hangout.';
  $from = $to;

  $message = drupal_mail('droogle_hangout', 'droogle_hangout_alert', $to, user_preferred_language($user),
    array(
      'body' => array($body),
      'subject' => $subj,
      'headers' => array(
        'Cc' => $cc,
        'Bcc' => ''),
    ),
    $from, TRUE);

  drupal_json_output(array(
    'successful' => 'Hangout created',
    'hangoutlink' => $hangout_link,
    'invitee_email' => $ajax_email,
  ));
}

/**
 * Converts date to RFC3339 format for Google Calendar compatibility.
 */
function date3339($timestamp = 0) {
  if (!$timestamp) {
    $timestamp = time();
  }
  $date = date('Y-m-d\TH:i:s', $timestamp);

  $matches = array();
  if (preg_match('/^([\-+])(\d{2})(\d{2})$/', date('O', $timestamp), $matches)) {
    $date .= $matches[1] . $matches[2] . ':' . $matches[3];
  }
  else {
    $date .= 'Z';
  }

  return $date;
}

/**
 * Callback from droogle_hangout_block_view().
 */
function droogle_hangout_invite_popup($form, &$form_state) {
  $form = array();
  // For buttons and email invitations popup.

  $hangout_invite_popup_wrapper = '<div id="hangout-invite-wrapper">';
  $hangout_invite_popup_wrapper .= '<div class="slant-45"></div>';
  $hangout_invite_popup_wrapper .= '<h2 id="hangout-invite-header">Create A Hangout</h2>';
  $hangout_invite_popup_wrapper .= '<div id="close-hangout-invite-popup">x</div>';
  $hangout_invite_popup_wrapper .= '<div id="hangout-invite-popup">';
  $hangout_invite_popup_wrapper .= '<div id="google-popup-logo"><div id="google-popup-logo-inner"></div>';
  $hangout_invite_popup_wrapper .= '</div>';
  $hangout_invite_popup_wrapper .= '<div id="hangout-email-list-wrapper">';
  $hangout_invite_popup_wrapper .= '<div id="count-message"></div>';

  $form['droogle_hangout_wrapper'] = array(
    '#markup' => $hangout_invite_popup_wrapper,
  );

  if ($path = libraries_get_path('datetimepicker')) {
    $form['droogle_hangout_calendar'] = array(
      '#prefix' => '<div class="schedule-wrapper">',
      '#type' => 'textfield',
      '#title' => t('Schedule Hangout'),
      '#default_value' => date('m/d/Y g:i A'),
      '#size' => 25,
      '#suffix' => '</div>',
    );
    $form['droogle_hangout_js']['#attached']['js'] = array(
      array(
        'data' => $path . '/jquery-ui-timepicker-addon.js',
        'scope' => 'footer',
      ),
      array(
        'data' => $path . '/jquery-ui-sliderAccess.js',
        'scope' => 'footer',
      ),
      array(
        'data' => drupal_get_path('module', 'droogle_hangout') . '/js/droogle_hangout_datetimepicker.js',
        'scope' => 'footer',
      ),
      array(
        'data' => array(
          'calendar_img_path' => '/' . drupal_get_path('module', 'droogle_hangout') . '/images/calendar.png',
        ),
        'type' => 'setting',
      ),
    );
  }
  $hangout_invite_popup_wrapend = '<ul id="hangout-email-list"></ul>';
  $hangout_invite_popup_wrapend .= '</div>';
  $hangout_invite_popup_wrapend .= '<button type="button" id="jabber-hangout-invites-btn">Start Hangout</button>';
  $hangout_invite_popup_wrapend .= '</div>';
  $hangout_invite_popup_wrapend .= '</div>';

  $form['droogle_hangout_wrapend'] = array(
    '#markup' => $hangout_invite_popup_wrapend,
  );

  $public_file_path = file_create_url('public://');

  $form['droogle_hangout_css']['#attached']['css'] = array(
    drupal_get_path('module', 'droogle_hangout') . '/css/droogle_hangout.css' => array(),
    $public_file_path . 'droogle_hangout_custom.css' =>
    array(
      'type' => 'external',
    ),
    '//ajax.googleapis.com/ajax/libs/jqueryui/1.10.0/themes/smoothness/jquery-ui.css' =>
    array(
      'type' => 'external',
    ),
  );

  return $form;
}

/**
 * Implements hook_theme().
 *
 * Implements hook_theme()for hangout chat and e-mail buttons.
 * Phineas, isn't using a theme function for a button, Overkill?
 * Why yes Mr. Drupal Developer, yes it is.
 */
function theme_droogle_hangout_invite_button($variables) {
  $email = $variables['email'];
  $fullname = $variables['fullname'];
  $src = $variables['src'];
  if ($variables['text'] == TRUE) {
    $text = 'Hangout';
  }
  else {
    $text = '';
  }
  if (is_null($fullname) || $fullname == '') {
    $fullname = $email;
  }
  if (is_null($src) || $src == '') {
    $src = ""; /* place default value here sometime later */
  }

  $button = '<button type="button" class="jabber-hangout-btn" name="' . $email . '">';
  $button .= '<span class="jabber-hangout-btn-text hangout-btn-a">' . $text . '</span>';
  $button .= '<span class="button-fullname" style="display:none;">' . $fullname . '</span>';
  $button .= '<span class="button-src" style="display:none;">' . $src . '</span>';
  $button .= '<div class="hangout-btn-bubble-wrapper">';
  $button .= '<p class="hangout-btn-bubble">Start<br/>Hangout</p>';
  $button .= '</div></button>';

  return $button;
}

/**
 * Implements hook_theme().
 */
function theme_droogle_hangout_email_button($variables) {
  $email = $variables['email'];

  if ($variables['text'] == TRUE) {
    $text = 'Send Email';
  }
  else {
    $text = '';
  }

  $button = '<div type="button" class="email-btn send-email" name="' . $email . '">';
  $button .= '<a class="email-btn-a" href="mailto:' . $email . '">' . $text . '</a>';
  $button .= '<div class="email-btn-bubble-wrapper">';
  $button .= '<p class="email-btn-bubble">Send<br/>Email</p>';
  $button .= '</div></div>';

  return $button;
}

/**
 * Implements hook_theme().
 */
function theme_droogle_hangout_chat_button($variables) {
  $email = $variables['email'];
  $fullname = $variables['fullname'];

  if ($variables['text'] == TRUE) {
    $text = 'Start Chat';
  }
  else {
    $text = '';
  }

  $button = '<button type="button" class="chat-btn" name="' . $email . '">';
  $button .= '<span class="chat-btn-a" href="#">' . $text . '</span>';
  $button .= '<div class="button-fullname">' . $fullname  . '</div>';
  $button .= '<div class="button-jid">' . $email . '</div>';
  $button .= '<div class="chat-btn-bubble-wrapper">';
  $button .= '<p class="chat-btn-bubble">Start<br/>Chat</p>';
  $button .= '</div></button>';

  return $button;
}

/**
 * Implements hook_theme().
 */
function theme_droogle_hangout_contact_me_popup($variables) {
  if (!module_exists('jabber')) {
    $variables['chat'] = FALSE;
    $variables['hangout'] = FALSE;
  }

  $uid = $variables['uid'];

  $sql = db_select('users', 'u');
  // Get Fullname from Realname module if installed.
  if (module_exists('realname')) {
    $sql->leftJoin('realname', 'r', 'r.uid = u.uid');
    $sql->fields('realname', array('realname'));
  }
  // If not using Realname module check if first and last name
  // tables exist (used by Babson College).
  if (!module_exists('realname') &&
    db_table_exists('field_data_field_first_name') &&
    db_table_exists('field_data_field_last_name')) {
    $sql->leftJoin('field_data_field_first_name', 'fn', 'fn.entity_id = u.uid');
    $sql->leftJoin('field_data_field_last_name', 'ln', 'ln.entity_id = u.uid');
    $sql->fields('first', array('field_first_name_value'));
    $sql->fields('last', array('field_last_name_value'));
  }
  $sql->fields('u', array('name', 'mail'));
  $and = db_and();
  $and->condition('u.uid', $uid);
  $sql->condition($and);

  $result = $sql->execute();

  foreach ($result as $row) {
    $username = $row->name;
    if (module_exists('realname')) {
      $fullname = $row->realname;
    }
    if (!module_exists('realname') &&
      db_table_exists('field_data_field_first_name') &&
      db_table_exists('field_data_field_last_name')) {
      $firstname = $row->first;
      $lastname = $row->last;
      $fullname = $firstname . ' ' . $lastname;
    }
    else {
      $fullname = '';
    }
    $user_mail = $row->mail;
  }

  $contact_me_popup = '<div class="contact-me">';
  $contact_me_popup .= '<div class="close-contact-me">x</div>';
  $contact_me_popup .= '<div class="contact-me-button-wrapper">';
  if ($variables['chat'] == TRUE) {
    $contact_me_popup .= theme('droogle_hangout_chat_button', array('email' => $user_mail, 'fullname' => $fullname));
  }
  if ($variables['email'] == TRUE) {
    $contact_me_popup .= theme('droogle_hangout_email_button', array('email' => $user_mail, 'fullname' => $fullname));
  }
  if ($variables['hangout'] == TRUE) {

    $contact_me_popup .= theme('droogle_hangout_invite_button', array('email' => $user_mail, 'fullname' => $fullname));
  }
  $contact_me_popup .= '</div><!-- ./presence-context-menu-wrapper -->';
  $contact_me_popup .= '<div class="chat-fin"></div>';
  $contact_me_popup .= '</div>';

  return $contact_me_popup;
}

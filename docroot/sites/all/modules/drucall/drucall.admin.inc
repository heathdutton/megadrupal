<?php

/**
 * @file
 * Admin page callbacks for the DruCall module.
 */

/**
 * Form for configuring DruCall settings.
 */
function drucall_admin($form, &$form_state) {

  $form['default_destination'] = array(
    '#type' => 'textfield',
    '#title' => t('Default destination'),
    '#default_value' => variable_get('default_destination', 'operator'),
    '#cols' => 40,
    '#rows' => 1,
    '#description' => t('The default destination that should be dialed when a visitor makes a DruCall call.  Can be the user part of a URI, or a full <b>sip:</b> URI.'),
  );

  $form['enable_audio'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow audio call'),
    '#default_value' => variable_get('enable_audio', true),
    '#description' => t('Whether or not to show a button allowing an audio call.'),
  );

  $form['enable_video'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow video call'),
    '#default_value' => variable_get('enable_video', true),
    '#description' => t('Whether or not to show a button allowing a video call.'),
  );

  $form['enable_chat'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show chat panel'),
    '#default_value' => variable_get('enable_chat', true),
    '#description' => t('Whether or not to show the chat messaging facility.'),
  );

  $form['enable_dtmf_pad'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show DTMF panel in-call'),
    '#default_value' => variable_get('enable_dtmf_pad', true),
    '#description' => t('Whether or not to show a DTMF dialing pad during calls.'),
  );

  $form['sip_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('SIP domain'),
    '#default_value' => variable_get('sip_domain', ''),
    '#cols' => 40,
    '#rows' => 1,
    '#description' => t('The SIP domain to be used to construct the <em>From:</em> header for calls made by logged-in Drupal users.'),
  );

  $form['ws_cookie_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('WebSocket Cookie Shared Secret'),
    '#default_value' => variable_get('ws_cookie_secret', ''),
    '#cols' => 40,
    '#rows' => 1,
    '#description' => t('The shared secret used to authenticate WebSocket cookies.  Must match the <b>WSCookieAuthSharedSecret</b> in <a href="http://www.resiprocate.org/SIP_Over_WebSocket_Cookies">the repro SIP proxy</a>.'),
  );

  $form['ws_cookie_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('WebSocket Cookie Domain'),
    '#default_value' => variable_get('ws_cookie_domain', ''),
    '#cols' => 40,
    '#rows' => 1,
    '#description' => t('The domain to set in cookies if using <a href="http://www.resiprocate.org/SIP_Over_WebSocket_Cookies">SIP over WebSocket cookie authentication</a>.  This may just be part of the domain and it must match both the Drupal web server domain and the WebSocket URL (below).  E.g. if the web site is <em>www.example.org</em> and WebSocket server is <em>wss://sip-ws.example.org</em> then you must put <em>example.org</em> in this field.'),
  );

  $form['ws_cookie_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('WebSocket Cookie Timeout'),
    '#default_value' => variable_get('ws_cookie_timeout', '900'),
    '#cols' => 40,
    '#rows' => 1,
    '#description' => t('Specify the duration, in seconds, that the authentication code is valid.  This also sets the expiration time of the cookies.'),
  );

  $form['ws_cookies_in_url'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send WebSocket authentication as URL parameters'),
    '#default_value' => variable_get('ws_cookies_in_url', true),
    '#description' => t('If the WebSocket server doesn\'t have the same domain name or domain suffix as the Drupal web site, the browser will not send the cookies.  Select this option to append the cookie values to the WebSocket URL so authentication will work across domains.'),
  );

  $form['display_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Display name for caller'),
    '#default_value' => variable_get('display_name', 'DruCall user'),
    '#cols' => 40,
    '#rows' => 1,
    '#description' => t('The display name to be inserted in the <em>From:</em> header of a call made by a guest user.'),
  );

  $form['from_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('SIP User ID (For From: header)'),
    '#default_value' => variable_get('from_uri', 'sip:user@example.org'),
    '#cols' => 40,
    '#rows' => 1,
    '#description' => t('The SIP URI, including the <b>sip:</b> scheme prefix, for a guest user.'),
  );

  $form['auth_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Authentication username'),
    '#default_value' => variable_get('auth_user', 'username'),
    '#cols' => 40,
    '#rows' => 1,
    '#description' => t('The guest account username for authenticating to the SIP proxy.'),
  );

  $form['auth_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Authentication password'),
    '#default_value' => variable_get('auth_password', ''),
    '#cols' => 40,
    '#rows' => 1,
    '#description' => t('The guest account password for authenticating to the SIP proxy.  <b>WARNING: not intended as a security mechanism, the password will be visible in the client-side JavaScript</b>.'),
  );

  $form['auth_realm'] = array(
    '#type' => 'textfield',
    '#title' => t('Authentication realm'),
    '#default_value' => variable_get('auth_realm', 'realm'),
    '#cols' => 40,
    '#rows' => 1,
    '#description' => t('The realm for authenticating the guest account to the SIP proxy.'),
  );

  $form['websocket_server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('WebSocket Server URL'),
    '#default_value' => variable_get('websocket_server_url', 'ws://sip-proxy.example.org:80'),
    '#cols' => 40,
    '#rows' => 1,
    '#description' => t('The WebSocket URL of the SIP proxy, typically with the <b>ws:</b> or <b>wss:</b> prefix.'),
  );

  $form['sip_outboundproxy_url'] = array(
    '#type' => 'textfield',
    '#title' => t('SIP Outbound Proxy URL'),
    '#default_value' => variable_get('sip_outboundproxy_url', 'tcp://sip-proxy.example.org:5060'),
    '#cols' => 40,
    '#rows' => 1,
    '#description' => t('SIP Outbound Proxy URL.  Only specify this if required.'),
  );

  $form['turn_server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('STUN/TURN server URL'),
    '#default_value' => variable_get('turn_server_url', 'turn:turn-server.example.org'),
    '#cols' => 40,
    '#rows' => 1,
    '#description' => t('The URL of the TURN server, for example, <em>turn:turn-server.example.org</em>'),
  );

  $form['turn_username'] = array(
    '#type' => 'textfield',
    '#title' => t('TURN server username'),
    '#default_value' => variable_get('turn_username', ''),
    '#cols' => 40,
    '#rows' => 1,
    '#description' => t('The username (long-term credential) for authenticating to the TURN server.'),
  );

  $form['turn_password'] = array(
    '#type' => 'textfield',
    '#title' => t('TURN server password'),
    '#default_value' => variable_get('turn_password', ''),
    '#cols' => 40,
    '#rows' => 1,
    '#description' => t('The password (long-term credential) for authenticating to the TURN server.  <b>WARNING: not intended as a security mechanism, the password will be visible in the client-side JavaScript</b>.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
  );

  return $form;

}

/**
 * Submit hook for drucall_admin.
 */
function drucall_admin_submit($form, &$form_state) {

  variable_set('default_destination', $form_state['values']['default_destination']);
  variable_set('enable_audio', $form_state['values']['enable_audio']);
  variable_set('enable_video', $form_state['values']['enable_video']);
  variable_set('enable_chat', $form_state['values']['enable_chat']);
  variable_set('enable_dtmf_pad', $form_state['values']['enable_dtmf_pad']);
  variable_set('sip_domain', $form_state['values']['sip_domain']);
  variable_set('ws_cookie_secret', $form_state['values']['ws_cookie_secret']);
  variable_set('ws_cookie_domain', $form_state['values']['ws_cookie_domain']);
  variable_set('ws_cookie_timeout', $form_state['values']['ws_cookie_timeout']);
  variable_set('ws_cookies_in_url', $form_state['values']['ws_cookies_in_url']);
  variable_set('display_name', $form_state['values']['display_name']);
  variable_set('from_uri', $form_state['values']['from_uri']);
  variable_set('auth_user', $form_state['values']['auth_user']);
  variable_set('auth_password', $form_state['values']['auth_password']);
  variable_set('auth_realm', $form_state['values']['auth_realm']);
  variable_set('websocket_server_url', $form_state['values']['websocket_server_url']);
  variable_set('sip_outboundproxy_url', $form_state['values']['sip_outboundproxy_url']);
  variable_set('turn_server_url', $form_state['values']['turn_server_url']);
  variable_set('turn_username', $form_state['values']['turn_username']);
  variable_set('turn_password', $form_state['values']['turn_password']);

  drupal_set_message(t('Your settings have been saved.'));

}


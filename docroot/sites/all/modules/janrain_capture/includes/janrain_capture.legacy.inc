<?php
/**
 * @file
 * Legacy Capture functions
 */

function janrain_capture_legacy_add_js($settings = array()) {
  // Legacy settings
  $janrain_capture_main = variable_get('janrain_capture_main', array());
  $janrain_capture_optional = variable_get('janrain_capture_optional', array());

  if (!empty($janrain_capture_optional['capture_sso_address'])) {
    $settings['janrainCapture']['sso_address'] = $janrain_capture_optional['capture_sso_address'];
  }

  $capture_sso_address = !empty($janrain_capture_optional['capture_sso_address'])
    ? $janrain_capture_optional['capture_sso_address'] : '';
  
  if (!empty($janrain_capture_optional['backplane_server'])
      && !empty($janrain_capture_optional['backplane_bus_name'])) {
    $settings['janrainCapture']['backplane_server'] = $janrain_capture_optional['backplane_server'];
    $settings['janrainCapture']['backplane_bus_name'] = $janrain_capture_optional['backplane_bus_name'];
  }

  // Add settings array into a JS variable
  drupal_add_js($settings, array('type' => 'setting', 'every_page' => TRUE, 'preprocess' => FALSE));

  // Set up scripts array
  $scripts = array(
    'file' => array(),
    'inline' => array(),
    'external' => array(),
  );

  // Legacy scripts
  if (!empty($janrain_capture_optional['capture_sso_address']) &&
      !empty($janrain_capture_optional['backplane_bus_name'])) {
    $scripts['external'][] = "https://$capture_sso_address/sso.js";

    $capture_sso_address = !empty($janrain_capture_optional['capture_sso_address']) ? $janrain_capture_optional['capture_sso_address'] : '';

    $redirect_uri = url('janrain_capture/oauth', array('absolute' => TRUE));
    $logout_uri = url('janrain_capture/simple_logout', array('absolute' => TRUE));
    $xd_receiver = url(NULL, array('absolute' => TRUE)) . drupal_get_path('module', 'janrain_capture') . '/xdcomm.html';

    $capture_sso_segment = !empty($janrain_capture_optional['capture_sso_segment_name']) ? $janrain_capture_optional['capture_sso_segment_name'] : '';

    if (!empty($janrain_capture_optional['capture_sso_supported_segment_names'])) {
      $capture_sso_supported_segment_names = implode('-', explode(',', $janrain_capture_optional['capture_sso_supported_segment_names']));
    }

    $legacy_scripts = "
    //console.log('Not Federated');
    JANRAIN.SSO.CAPTURE.check_login({
      sso_server: 'https://{$capture_sso_address}',
      client_id: '{$janrain_capture_main['capture_client_id']}',
      redirect_uri: '{$redirect_uri}',
      logout_uri: '{$logout_uri}',
      xd_receiver: '{$xd_receiver}',
";
    
    if (!empty($janrain_capture_optional['capture_sso_segment_name'])
      && !empty($janrain_capture_optional['capture_sso_supported_segment_names'])) {
      $legacy_scripts .= "
      segment: '{$janrain_capture_optional['capture_sso_segment_name']}',
      supported_segments: '{$capture_sso_supported_segment_names}'";
    }

    $legacy_scripts .= "
    });";

    $scripts['inline'][] = $legacy_scripts;
  }

  if (!empty($janrain_capture_optional['backplane_js_path'])) {
    $scripts['external'][] = $janrain_capture_optional['backplane_js_path'];
  }

  if (isset($_SESSION['janrain_capture_password_recover']) && $_SESSION['janrain_capture_password_recover'] == TRUE) {
    $url = url('janrain_capture/profile', array(
      'absolute' => TRUE,
      'query' => array(
        'method' => '_change_password',
        'callback' => 'Drupal.janrainCapture.closeRecoverPassword',
      ),
    ));
    $scripts['inline'][] = 'jQuery(function($) {Drupal.janrainCapture.passwordRecover(' . $url .')});';
    $_SESSION['janrain_capture_password_recover'] = FALSE;
  }

  // Add janrain_capture.js
  $scripts['file'][] = drupal_get_path('module', 'janrain_capture') . '/janrain_capture.js';

  // Add the scripts
  foreach ($scripts['file'] as $s) {
    drupal_add_js($s, array('type' => 'file', 'every_page' => TRUE, 'weight' => 1, 'preprocess' => FALSE));
  }
  foreach ($scripts['external'] as $s) {
    drupal_add_js($s, array('type' => 'external', 'every_page' => TRUE, 'weight' => 2, 'preprocess' => FALSE));
  }
  foreach ($scripts['inline'] as $s) {
    drupal_add_js($s, array('type' => 'inline', 'every_page' => TRUE, 'weight' => 3, 'preprocess' => FALSE));
  }
}

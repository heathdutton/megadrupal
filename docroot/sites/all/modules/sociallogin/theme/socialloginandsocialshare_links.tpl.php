<?php
/**
 * Theme social links.
 **/
$api_key = trim(variable_get('socialloginandsocialshare_apikey'));
if (!empty($api_key)) {
  drupal_add_js('//hub.loginradius.com/include/js/LoginRadius.js', array(
    'type' => 'external',
    'scope' => 'header',
    'weight' => -10
  ));
  drupal_add_js(drupal_get_path('module', 'socialloginandsocialshare') . '/js/LoginRadiusSDK.js', array(
    'type' => 'file',
    'scope' => 'footer'
  ));
  drupal_add_js(drupal_get_path('module', 'socialloginandsocialshare') . '/js/sociallogin_interface.js', array(
    'type' => 'file',
    'scope' => 'footer'
  ));
}
?>
<div class="interfacecontainerdiv"></div>

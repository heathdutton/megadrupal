<?php

/**
 * @file
 * Theme social links.
 **/

$api_key = trim(variable_get('lr_social_login_apikey'));
if (!empty($api_key)) :
  drupal_add_js('//hub.loginradius.com/include/js/LoginRadius.js', array(
    'type' => 'external',
    'scope' => 'header',
    'weight' => -10,
  ));
  drupal_add_js(drupal_get_path('module', 'lr_social_login') . '/js/sociallogin_interface.js', array(
    'type' => 'file',
      'scope' => 'header',
      'weight' => -10,
  ));
endif;
?>
<div class="interfacecontainerdiv"></div>

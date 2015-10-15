<?php

/**
 * Implementation of hook_settings().
 */
function sslproxy_settings() {
 $form['sslproxy_var'] = array(
   '#type' => 'textfield',
   '#title' => t('SSL Proxy Variable'),
   '#description' => t('The variable being set by the SSL proxy server.'),
   '#default_value' => variable_get('sslproxy_var', 'HTTP_USING_HTTPS'),
 );

 $form['sslproxy_var_value'] = array(
   '#type' => 'textfield',
   '#title' => t('SSL Proxy Variable Value'),
   '#description' => t('The value of the variable being set by the SSL proxy server.'),
   '#default_value' => variable_get('sslproxy_var_value', 1),
 );
  
 return system_settings_form($form);
}

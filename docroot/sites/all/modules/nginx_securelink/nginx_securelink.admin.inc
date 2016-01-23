<?php

/**
 * @author "Ahmad Hesamzadeh" <mngafa@gmail.com>
 * @file Administration pages and forms
 */

/**
 * form builder
 */
function nginx_securelink_options_form($form, &$form_state) {
  $form['nginx_securelink_deadline'] = array(
    '#type' => 'textfield',
    '#title' => t('Deadline'),
    '#description' => t('How many seconds the generated link will be valid?'),
    '#default_value' => variable_get('nginx_securelink_deadline', NGINX_SECURELINK_DEADLINE_DEFAULT),
    '#size' => 7,
    '#field_suffix' => t('seconds'),
    '#required' => TRUE,
  );

  $form['nginx_securelink_hash_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Hash format'),
    '#description' => t('Please enter the hash format used by nginx.!br You can use these tokens:!br @exp for expiration timestamp.!br @url for file url.!br @ip for user IP.!br @passwd for the ngix_securelink secret code.', array(
      '!br' => '<br />',
      '@exp' => '<expire>',
      '@url' => '<url>',
      '@ip' => '<ip>',
      '@passwd' => '<passwd>',
    )),
    '#default_value' => variable_get('nginx_securelink_hash_format', NGINX_SECURELINK_HASH_FORMAT_DEFAULT),
    '#required' => TRUE,
  );

  $form['nginx_securelink_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret code'),
    '#description' => t('Please enter the ngix_securelink\'s secret code.'),
    '#default_value' => variable_get('nginx_securelink_secret', NGINX_SECURELINK_SECRET_DEFAULT),
    '#required' => TRUE,
  );

  $form['nginx_securelink_md5_arg_name'] = array(
    '#type' => 'textfield',
    '#title' => t('MD5 Argument name'),
    '#description' => t('The argument name for "md5" in URLs.'),
    '#default_value' => variable_get('nginx_securelink_md5_arg_name', NGINX_SECURELINK_MD5_ARG_NAME_DEFAULT),
    '#size' => 7,
    '#required' => TRUE,
  );

  $form['nginx_securelink_expires_arg_name'] = array(
    '#type' => 'textfield',
    '#title' => t('expires Argument name'),
    '#description' => t('The argument name for "expires" in URLs.'),
    '#default_value' => variable_get('nginx_securelink_expires_arg_name', NGINX_SECURELINK_EXPIRES_ARG_NAME_DEFAULT),
    '#size' => 7,
    '#required' => TRUE,
  );

  $form['nginx_securelink_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL'),
    '#description' => t('The base URL for download links. Please enter a trailing slash.'),
    '#default_value' => variable_get('nginx_securelink_base_url', NGINX_SECURELINK_BASE_URL_DEFAULT),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Form builder for 'admin/config/media/nginx-securelink/generator'
 */
function nginx_securelink_config_generation_settings_form($form, &$form_state) {
  $form['nginx_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Nginx settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['nginx_settings']['nginx_securelink_nginx_ip'] = array(
    '#type' => 'textfield',
    '#title' => t('IP Address'),
    '#description' => t('The IP Address of nginx server. Nginx will listen on this IP. You can use * to listen to all network interfaces.'),
    '#default_value' => variable_get('nginx_securelink_nginx_ip', NGINX_SECURELINK_NGINX_IP_DEFAULT),
    '#required' => TRUE,
  );

  $form['nginx_settings']['nginx_securelink_nginx_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#description' => t('The port of nginx server. Nginx will listen on this port. Default is 80'),
    '#default_value' => variable_get('nginx_securelink_nginx_port', NGINX_SECURELINK_NGINX_PORT_DEFAULT),
    '#required' => TRUE,
  );

  $form['nginx_settings']['nginx_securelink_nginx_servername'] = array(
    '#type' => 'textfield',
    '#title' => t('ServerName'),
    '#description' => t('The server name of nginx. This is the fully qualified domain name (FQDN). You can separate multiple domain names with spaces. Example: "private.domain.com private.domain.net"'),
    '#default_value' => variable_get('nginx_securelink_nginx_servername', NGINX_SECURELINK_NGINX_SERVERNAME_DEFAULT),
    '#required' => TRUE,
  );

  $form['nginx_settings']['nginx_securelink_nginx_root_dir'] = array(
    '#type' => 'textfield',
    '#title' => t('Root directory'),
    '#description' => t('The root directory where your private files are located. This should be an absolute path. Example: /var/privatefiles'),
    '#default_value' => variable_get('nginx_securelink_nginx_root_dir', NGINX_SECURELINK_NGINX_ROOT_DIR_DEFAULT),
    '#required' => TRUE,
  );

  $form['error_handling'] = array(
    '#type' => 'fieldset',
    '#title' => t('Error handling'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['error_handling']['nginx_securelink_nginx_access_log'] = array(
    '#type' => 'textfield',
    '#title' => t('Access log file'),
    '#description' => t('Absolue path to access log file.'),
    '#default_value' => variable_get('nginx_securelink_nginx_access_log', NGINX_SECURELINK_NGINX_ACCESS_LOG_DEFAULT),
    '#required' => TRUE,
  );

  $form['error_handling']['nginx_securelink_nginx_access_log_bytes'] = array(
    '#type' => 'textfield',
    '#title' => t('Access log file (bytes)'),
    '#description' => t('Absolue path to access log file. for "bytes" output.'),
    '#default_value' => variable_get('nginx_securelink_nginx_access_log_bytes', NGINX_SECURELINK_NGINX_ACCESS_LOG_BYTES_DEFAULT),
    '#required' => TRUE,
  );

  $form['error_handling']['nginx_securelink_nginx_error_log'] = array(
    '#type' => 'textfield',
    '#title' => t('Error log file'),
    '#description' => t('Absolue path to error log file.'),
    '#default_value' => variable_get('nginx_securelink_nginx_error_log', NGINX_SECURELINK_NGINX_ERROR_LOG_DEFAULT),
    '#required' => TRUE,
  );


  return system_settings_form($form);
}

/**
 * Page callback for 'admin/config/media/nginx-securelink/generator/generate'
 */
function nginx_securelink_config_generator_page() {
  // First get all configuration options and check if all are set.

  // Check nginx generator options
  $nginx_ip               = variable_get('nginx_securelink_nginx_ip', NGINX_SECURELINK_NGINX_IP_DEFAULT);
  $nginx_port             = variable_get('nginx_securelink_nginx_port', NGINX_SECURELINK_NGINX_PORT_DEFAULT);
  $nginx_servername       = variable_get('nginx_securelink_nginx_servername', NGINX_SECURELINK_NGINX_SERVERNAME_DEFAULT);
  $nginx_root_dir         = variable_get('nginx_securelink_nginx_root_dir', NGINX_SECURELINK_NGINX_ROOT_DIR_DEFAULT);
  $nginx_access_log       = variable_get('nginx_securelink_nginx_access_log', NGINX_SECURELINK_NGINX_ACCESS_LOG_DEFAULT);
  $nginx_access_log_bytes = variable_get('nginx_securelink_nginx_access_log_bytes', NGINX_SECURELINK_NGINX_ACCESS_LOG_BYTES_DEFAULT);
  $nginx_error_log        = variable_get('nginx_securelink_nginx_error_log', NGINX_SECURELINK_NGINX_ERROR_LOG_DEFAULT);

  if (empty($nginx_ip) ||
      empty($nginx_port) ||
      empty($nginx_servername) ||
      empty($nginx_root_dir) ||
      empty($nginx_access_log) ||
      empty($nginx_access_log_bytes) ||
      empty($nginx_error_log)) {

    drupal_set_message(t('Incomplete config.'), 'error');

    return t('Please configre nginx genertor options first.') . '<br />' .
      l(t('Generator options'), 'admin/config/media/nginx-securelink/generator');
  }

  // Check securelink options
  $hash_format      = variable_get('nginx_securelink_hash_format', NGINX_SECURELINK_HASH_FORMAT_DEFAULT);
  $secret           = variable_get('nginx_securelink_secret', NGINX_SECURELINK_SECRET_DEFAULT);
  $md5_arg_name     = variable_get('nginx_securelink_md5_arg_name', NGINX_SECURELINK_MD5_ARG_NAME_DEFAULT);
  $expires_arg_name = variable_get('nginx_securelink_expires_arg_name', NGINX_SECURELINK_EXPIRES_ARG_NAME_DEFAULT);

  if (empty($hash_format) ||
      empty($secret) ||
      empty($md5_arg_name) ||
      empty($expires_arg_name)) {

    drupal_set_message(t('Incomplete config.'), 'error');

    return t('Please configre nginx securelink options first.') . '<br />' .
      l(t('Securelink options'), 'admin/config/media/nginx-securelink');
  }

  // Now generate nginx config
  // The tamplate:
  $nginx_template = <<<FINISH
server {
  listen <ip>:<port>;
  server_name <server_name>;
  access_log <access_log>;
  access_log <access_log_bytes> bytes;
  error_log <error_log>;
  root <root>;
  index index.php index.html index.htm;

  location / {
    secure_link \$arg_<md5_arg>,\$arg_<expires_arg>;
    secure_link_md5 "<hash_format>";

    if (\$secure_link = "") {
        return 403;
    }

    if (\$secure_link = "0") {
        return 410;
    }
  }

  location = /403.html {
    allow all;
  }
  location = /410.html {
    allow all;
  }
  location = /404.html {
    allow all;
  }
  location = /robots.txt {
    allow all;
  }

  error_page   403  =  /403.html;
  error_page   410  =  /410.html;
  error_page   404  =  /404.html;
}
FINISH;

  // Prepare hash format for nginx config
  $hash_format_for_nginx = strtr($hash_format, array(
    '<expire>' => '$secure_link_expires',
    '<url>'    => '$uri',
    '<ip>'     => '$remote_addr',
    '<passwd>' => $secret,
  ));

  // Generate config
  $config_generated = strtr($nginx_template, array(
    '<ip>'               => $nginx_ip,
    '<port>'             => $nginx_port,
    '<server_name>'      => $nginx_servername,
    '<access_log>'       => $nginx_access_log,
    '<access_log_bytes>' => $nginx_access_log_bytes,
    '<error_log>'        => $nginx_error_log,
    '<root>'             => $nginx_root_dir,
    '<md5_arg>'          => $md5_arg_name,
    '<expires_arg>'      => $expires_arg_name,
    '<hash_format>'      => $hash_format_for_nginx,
  ));

  // Create a page array
  $page = array();

  $description = 'You should copy the text shown below. And adjust it. Then copy it to your nginx.conf file.';
  $description .= ' You should also create robots.txt and 4xx.html files, if you want to use this generated config.';

  $page['desc'] = array(
    '#markup' => t($description),
  );

  $page['nginx_config_generated'] = array(
    '#type' => 'textarea',
    '#title' => t('Generated nginx config'),
    '#description' => t('Please adjust and put the above config in nginx.conf.'),
    '#value' => $config_generated,
    '#rows' => 30,
  );

  return $page;
}

/**
 * Page callback for 'admin/config/media/nginx-securelink/handlers'
 */
function nginx_securelink_access_handlers_page() {
  return t('Every access handler has a seconday tab on this page. If you see no tabs, then no access handler is installed.');
}

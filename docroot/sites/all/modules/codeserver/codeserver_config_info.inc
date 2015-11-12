<?php
/*
 * @file
 * Code Server module for Drupal by netgenius.co.uk
 */
function codeserver_config_info() {

  $tvars = array('@match' => t('Must match on client and server.'));

  return array(
    'service_id' => t('The id of this service (server), or the service to request (client).'),
    'service_name' => t('A descriptive name for the service.'),
    'service_enabled' => t('If enabled, this service is available to use.'),
    'server' => t('The URL of the server (client).'),
    'allowed_ips' => t('IP addresses which may access this service (server).'),
    'allowed_functions' => t('Names of PHP functions which the client may call (server).'),
    'set_uid' => t('User id to set when executing client requests. Can be overriden by login, if allowed (server).'),
    'allow_login' => t('If enabled, the client is allowed to log in (server).'),
    'require_permission' => t('If enabled, a matching permission is required to access this service (server).'),
    'request_timeout' => t('The timeout (seconds) for requests (client).'),
    'options' => t('Options such as user id and password (client).'),
    'encrypt_method' => t('The method (cipher) used for encryption. @match', $tvars),
    'encrypt_key' => t('The key used for encryption. @match', $tvars),
    'encrypt_iv' => t('The initialization vector used for encryption. @match', $tvars),
    'show_messages' => t('If enabled, return messages to the client (server), or display server messages (client).'),
    'safe_serialize' => t("If enabled, use Code Server's own safer alternatives to PHP's serialize() and unserialize() functions. @match", $tvars),
  );
}

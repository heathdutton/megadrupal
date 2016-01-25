<?php
/**
 * @file
 * Rules API integration for gate_rules.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 */
function clientcert_rules_event_info() {
  $events = array();

  $events['clientcert_verify'] = array(
    'label' => t('Clientcert verify url is accessed.'),
    'group' => t('Clientcert'),
    'variables' => array(
      'clientcert_path_argument' => array(
        'type' => 'text',
        'label' => t('The url path arg(2) to operate with.'),
      ),
    ),
  );

  return $events;
}

/**
 * Implements hook_rules_action_info().
 */
function clientcert_rules_action_info() {
  $actions = array();

  $actions['clientcert_item_set'] = array(
    'label' => t('Create/update clientcert item, setting main values.'),
    'named parameter' => TRUE,
    'parameter' => array(
      'specialtoken' => array(
        'type' => 'boolean',
        'label' => t('Use special token and "REDIRECT_"-search'),
        'description' => t('Example: (SERVER:SSL_CLIENT_M_SERIAL). If key "SSL_CLIENT_M_SERIAL" is not successful, the script tries to retrieve "REDIRECT_SSL_CLIENT_M_SERIAL" in $_SERVER.'),
        'optional' => TRUE,
        'default mode' => 'input',
      ),
      'uid' => array(
        'type' => 'integer',
        'label' => t('The user ID of the clientcert item.'),
        'optional' => FALSE,
        'default mode' => 'input',
      ),
      'title' => array(
        'type' => 'text',
        'label' => t('Preset the title of clientcert item (can be changed by user).'),
        'description' => t("'special token' can be used e.g (SERVER:SSL_CLIENT_S_DN_Email)."),
        'optional' => TRUE,
        'default mode' => 'input',
      ),
      'cert' => array(
        'type' => 'text',
        'label' => t('The certificate (signed public key) in PEM format.'),
        'description' => t("'special token' can be used e.g. '(SERVER:SSL_CLIENT_CERT)'."),
        'optional' => TRUE,
        'default mode' => 'input',
      ),
      'serial' => array(
        'type' => 'text',
        'label' => t("The serial of the client certificate."),
        'description' => t("'special token' can be used e.g. '(SERVER:SSL_CLIENT_M_SERIAL)'."),
        'optional' => TRUE,
        'default mode' => 'input',
      ),
      'dn_subject' => array(
        'type' => 'text',
        'label' => t("Subject DN in client's certificate."),
        'description' => t("'special token' can be used e.g. '(SERVER:SSL_CLIENT_S_DN)'"),
        'optional' => TRUE,
        'default mode' => 'input',
      ),
      'dn_issuer' => array(
        'type' => 'text',
        'label' => t("Issuer DN in client's certificate."),
        'description' => t("'special token' can be used e.g. '(SERVER:SSL_CLIENT_I_DN)'"),
        'optional' => TRUE,
        'default mode' => 'input',
      ),
      'verify' => array(
        'type' => 'text',
        'label' => t('The servers verify report.'),
        'description' => t("'special token' can be used. e.g. '(SERVER:SSL_CLIENT_VERIFY)' .On Apache webserver this is 'NONE, SUCCESS, GENEROUS or FAILED:reason'."),
        'optional' => TRUE,
        'default mode' => 'input',
      ),
      'status' => array(
        'type' => 'boolean',
        'label' => t('Active'),
        'optional' => TRUE,
        'description' => t("You can force to 'active' here but maybe you want to use 'Verify value to to set active'  field."),
        'default mode' => 'input',
      ),
      'verify_active' => array(
        'type' => 'text',
        'label' => t('Verify value to to set active.'),
        'description' => t("If set e.g. to 'SUCCESS' (used by Apache) you can activate a clientcert item automatically when the webserver validates successful. You can use other rules for a more complex strategy."),
        'optional' => TRUE,
        'default mode' => 'input',
      ),
      'status_login' => array(
        'type' => 'boolean',
        'label' => t('Can be used for login (can be changed by user).'),
        'optional' => TRUE,
        'default mode' => 'input',
      ),
      'status_encrypt' => array(
        'type' => 'boolean',
        'label' => t('Can be used for encryption (can be changed by user).'),
        'optional' => TRUE,
        'default mode' => 'input',
      ),
      'status_public' => array(
        'type' => 'boolean',
        'label' => t('Certificate can be viewed by others (can be changed by user).'),
        'optional' => TRUE,
        'default mode' => 'input',
      ),
       'validity_start' => array(
        'type' => 'text',
        'label' => t("Validity of server's certificate (start time)"),
        'optional' => TRUE,
        'description' => t("'special token' can be used e.g. '(SERVER:SSL_CLIENT_V_START)'. Input of time string (text). This will be changed to unix timestamp by the PHP strtotime() function."),
        'default mode' => 'input',
      ),
      'validity_end' => array(
        'type' => 'text',
        'label' => t("Validity of server's certificate (end time)"),
        'optional' => TRUE,
        'description' => t("'special token' can be used e.g. '(SERVER:SSL_CLIENT_V_END)'. Input of time string (text). This will be changed to unix timestamp by the PHP strtotime() function."),
        'default mode' => 'input',
      ),
    ),
    'group' => t('Clientcert'),
    'provides' => array(
      'clientcert_item' => array(
        'label' => t('Created clientcert item entity.'),
        'type' => 'entity',
      ),
    ),
    'callbacks' => array(
      'execute' => '_clientcert_item_set',
    ),
  );

  $actions['clientcert_items_pem'] = array(
    'label' => t('Retrieve clientcert items by PEM.'),
    'named parameter' => TRUE,
    'parameter' => array(
      'specialtoken' => array(
        'type' => 'boolean',
        'label' => t('Use special token and "REDIRECT_"-search'),
        'description' => t('Example: (SERVER:SSL_CLIENT_CERT). If key is not successful, the script tries to retrieve "REDIRECT_SSL_CLIENT_CERT" in $_SERVER.'),
        'optional' => TRUE,
        'default mode' => 'input',
      ),
      'cert' => array(
        'type' => 'text',
        'label' => t('The certificate (signed public key) in PEM format.'),
        'description' => t("'special token' can be used e.g. '(SERVER:SSL_CLIENT_CERT)'."),
        'optional' => FALSE,
        'default mode' => 'input',
      ),
      'uid_token' => array(
        'type' => 'token',
        'label' => t('url uid identifier'),
        'description' => t('If set the uid is read from url like ?uid=12 and only certificate (usually one) with this uid will be retrieved.'),
        'optional' => TRUE,
        'default mode' => 'input',
      ),

    ),
    'group' => t('Clientcert'),
    'provides' => array(
      'clientcert_items' => array(
        'label' => t('List of retrieved clientcert items.'),
        'type' => 'list<clientcert>',
      ),
      'clientcert_items_count' => array(
        'label' => t('The number of retrieved clientcert items.'),
        'type' => 'integer',
      ),
    ),
    'callbacks' => array(
      'execute' => '_clientcert_items_pem',
    ),
  );

  return $actions;
}

/**
 * @}
 */

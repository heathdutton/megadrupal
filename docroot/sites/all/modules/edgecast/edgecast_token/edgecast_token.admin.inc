<?php

/**
 * @file
 * Settings administration UI.
 */

/**
 * System settings for to control the token authentication
 * @return form
 */
function edgecast_token_admin_token() {
  $form = array();

  $form['edgecast_cdn_token_intro_01'] = array(
    '#markup' => t('<p>Token Authentication allows for tokenized URLs to be created that protect your content from download by unauthorized users.</p><p>This portion of the module requires a PHP module called ectoken.so to be installed on your server to function.</p>'),
  );

  $form['edgecast_cdn_token_intro_02'] = array(
    '#markup' => t('<p>To read how to setup this up you must have a CDN account in order to access the documenents necessary to undersand all of these options. We will not explain these in detail here as it would take too many pages.  Access the documents through your ') . l('control panel', 'https://my.edgecast.com/support/docs/') . t('.</p>'),
  );

  $form['edgecast_cdn_token_method'] = array(
    '#decription' => 'Choose if you want to tokenize vie webservice or installed ECToken PHP Library.',
    '#type' => 'select',
    '#title' => t('Tokenization method'),
    '#options' => array(
      '1' => 'PHPLibrary',
      '2' => 'WebService'
    ),
    '#default_value' => variable_get('edgecast_cdn_token_method'),
  );

  $form['edgecast_cdn_token_pkey'] = array(
    '#type' => 'textfield',
    '#size' => 174,
    '#title' => t('Edgecast Private Key'),
    '#default_value' => variable_get('edgecast_cdn_token_pkey'),
    '#description' => t('Place your Primary private key for token authentication here.  This is available via your control panel.
      In each service on your EC panel there is a "Token Auth" menu item.  This is an optional service that must be activated
      for your account.  You must contact your sale agent to get this turned on. <b>This module only works with a single Private key.</b>
      if you are using multiple services for files you will need to make sure all the services have the same Private Key.'),
  );

  $form['edgecast_cdn_token_ecexpire'] = array(
    '#type' => 'textfield',
    '#size' => 15,
    '#title' => t('Edgecast Token Expire Time'),
    '#default_value' => variable_get('edgecast_cdn_token_ecexpire'),
    '#description' => t('Time in seconds that the link will expire. Leave blank if you do not want to use this setting. This
      is referred to as ec_expire in the EC documentation.'),
  );
  $form['edgecast_cdn_token_eccountryallow'] = array(
    '#type' => 'textfield',
    '#size' => 100,
    '#title' => t('Edgecast Country Allow'),
    '#default_value' => variable_get('edgecast_cdn_token_eccountryallow'),
    '#description' => t('Countries allowed to get content.  This is a comma seperated list of ISO 3166 country codes.'),
  );
  $form['edgecast_cdn_token_eccountrydeny'] = array(
    '#type' => 'textfield',
    '#size' => 100,
    '#title' => t('Edgecast Country Deny'),
    '#default_value' => variable_get('edgecast_cdn_token_eccountrydeny'),
    '#description' => t('Countries disallowed to get content.  This is a comma seperated list of ISO 3166 country codes.'),
  );
  $form['edgecast_cdn_token_ecrefallow'] = array(
    '#type' => 'textfield',
    '#size' => 100,
    '#title' => t('Edgecast HTTP References Allowed'),
    '#default_value' => variable_get('edgecast_cdn_token_ecrefallow'),
    '#description' => t('HTTP Referals allowed to retrieve content.  It is possible to spoof this as a client is responsible for
      reporting it\'s own referal.  Also going from SSL to a non-SSL site it is disallowed in browsers for the referral to be sent.'),
  );
  $form['edgecast_cdn_token_ecrefdeny'] = array(
    '#type' => 'textfield',
    '#size' => 100,
    '#title' => t('Edgecast HTTP References Denied'),
    '#default_value' => variable_get('edgecast_cdn_token_ecrefdeny'),
    '#description' => t('HTTP Referals denied to retrieve content.'),
  );
  $form['edgecast_cdn_token_ecprotoallow'] = array(
    '#type' => 'select',
    '#title' => t('Edgecast Allowed Protocols'),
    '#default_value' => variable_get('edgecast_cdn_token_ecprotoallow'),
    '#description' => t('Only two options: http or https. Defaults to both.'),
    '#options' => array(
      '0' => 'HTTP',
      '1' => 'HTTPS',
      '2' => 'BOTH'
    ),
  );
  $form['edgecast_cdn_token_ecprotodeny'] = array(
    '#type' => 'select',
    '#title' => t('Edgecast Deny Protocols'),
    '#default_value' => variable_get('edgecast_cdn_token_ecprotodeny'),
    '#description' => t('Only two options: http or https. Defaults to none. Denying both protocols is possible 
      but not for this module - doesn\'t make sense.'),
    '#options' => array(
      '0' => 'HTTP',
      '1' => 'HTTPS',
      '2' => 'NONE'
    ),
  );
  $form['edgecast_cdn_token_ecclientip'] = array(
    '#type' => 'textfield',
    '#size' => 100,
    '#title' => t('Edgecast IP Addresses Allowed'),
    '#default_value' => variable_get('edgecast_cdn_token_ecclientip'),
    '#description' => t('IP Address to allow to connect to content.  Add IP\'s in IPv4 notation
      (e.g., 100.10.1.123), comma separate multiple values'),
  );
  $form['edgecast_cdn_token_ecurlallow'] = array(
    '#type' => 'textfield',
    '#size' => 100,
    '#title' => t('Edgecast URL Allowed'),
    '#default_value' => variable_get('edgecast_cdn_token_ecurlallow'),
    '#description' => t('This is a not as used paramater where you have fine grained control of the URL\'s that can be accessed.
      This is actually the path after the domain name to the content.  I would suggest reading the EC Token Auth documentation for a
      very long explanation of thise settings.  It\'s too much to explain here. Comma seperate the values.'),
  );

  return system_settings_form($form);
}

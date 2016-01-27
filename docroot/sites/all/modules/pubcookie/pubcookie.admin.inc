<?php

/**
 * @file
 * Admin page callbacks for the pubcookie module.
 */

/*
 * The pubcookie settings page.
 */
function pubcookie_settings() {
  global $base_url; // http://www.example.edu/drupal
  $array = explode('/', $base_url);
  $base_domain = $array[2]; // www.example.edu
  $parts = explode('.', $base_domain);

  if (count($parts) == 1) {
    drupal_set_message(t('Pubcookie only works on real domains, e.g., example.edu.'));
    $domain = array_pop($parts);
  }
  else { // Parse out example.edu.
    //        'example'                   '.'   'edu'
    $domain = $parts[count($parts) - 2] . '.' . $parts[count($parts) -1];
  }

  $form['pubcookie_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain'),
    '#default_value' => variable_get('pubcookie_domain', $domain),
    '#description' => t('What is the domain for which your pubcookie server is serving out cookies?'),
    '#size' => '40',
    '#maxlength' => '255'
    );
  $form['pubcookie_login_dir'] = array(
    '#type' => 'textfield',
    '#title' => t('Login directory'),
    '#default_value' => variable_get('pubcookie_login_dir', 'pubcookie'),
    '#description' => t('What is the subdirectory in this Drupal installation that contains the .htaccess file with the PubCookieAppID directive? (Do not use a trailing slash.)'),
    '#size' => '40',
    '#maxlength' => '255'
    );
  $form['pubcookie_success_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Successful login URL'),
    '#default_value' => variable_get('pubcookie_success_url', ''),
    '#description' => t('Where do you want the user directed after a successful pubcookie login? Leave blank if you want the user to return to the page that they were on when they logged in.'),
    '#size' => '40',
    '#maxlength' => '255'
    );
  $form['pubcookie_id_is_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('ID/E-mail equivalency'),
    '#default_value' => variable_get('pubcookie_id_is_email', 1),
    '#description' => t("Check this box if the login ID (joe@example.edu) is the same as the user's email address. If so, the mail column of the user table will be populated when a user registers.")
    );
  $form['pubcookie_ldap_server'] = array(
    '#type' => 'textfield',
    '#title' => t('LDAP server'),
    '#default_value' => variable_get('pubcookie_ldap_server', ''),
    '#description' => t('If you wish to populate profile data when new users register, enter the LDAP server to query here.'),
    '#size' => '40',
    '#maxlength' => '255'
    );
  $form['pubcookie_ldap_basedn'] = array(
    '#type' => 'textfield',
    '#title' => t('LDAP base DN'),
    '#default_value' => variable_get('pubcookie_ldap_basedn', ''),
    '#description' => t('Base DN for your particular LDAP directory (e.g. %dn). Check with your LDAP administrator.', array('%dn' => 'dc=example,dc=com')),
    '#size' => '40',
    '#maxlength' => '255'
    );
  $form['pubcookie_ldap_searchfield'] = array(
    '#type' => 'textfield',
    '#title' => t('LDAP search string'),
    '#default_value' => variable_get('pubcookie_ldap_searchfield', '(|(uid=%username))'),
    '#description' => t("The query that will be used to search on LDAP for the username of the user logging in to your Drupal site. (%username will be replaced by the username of the user logging in.)" ),
    '#size' => '40',
    '#maxlength' => '255'
    );
  $form['pubcookie_ldap_usernamefield'] = array(
    '#type' => 'textfield',
    '#title' => t('LDAP field to use as username'),
    '#default_value' => variable_get('pubcookie_ldap_usernamefield', ''),
    '#description' => t("The name of the LDAP field that will be used as a user's Drupal username. Leave blank to use the pubcookie ID (the joe in joe@example.edu) as username." ),
    '#size' => '40',
    '#maxlength' => '255'
    );

  return system_settings_form($form);
}

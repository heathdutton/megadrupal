<?php

/**
 * @file
 * Rules integration for the ip2country module.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_condition_info().
 */
function ip2country_rules_condition_info() {
  $conditions = array();

  $conditions['ip2country_user_country'] = array(
    'label' => t('User is in country (based on IP address)'),
    'group' => t('User'),
    'parameter' => array(
      'countries' => array(
        'label' => t('Countries'),
        'type' => 'list<text>',
        'options list' => 'country_get_list',
        'restriction' => 'input',
      ),
    ),
    'callbacks' => array(
      'execute' => 'ip2country_user_country',
      'help' => 'ip2country_user_country_help',
    ),
   );

  return $conditions;
}

/**
 * Help callback for ip2country_user_country condition.
 */
function ip2country_user_country_help() {
  return t('Uses the ip2country module to determine if the user is located in one of the selected countries.');
}

/**
 * Determines if the user's country is one of the selected countries.
 *
 * @param $countries
 *   An array of ISO 3166-2 country codes to search.
 *
 * @return
 *   TRUE if the user's country is found in $countries, FALSE otherwise.
 */
function ip2country_user_country(array $countries = array()) {
  global $user;

  if (isset($user->data['country_iso_code_2'])) {
    // Use the country stored in the $user object.
    $country_code = $user->data['country_iso_code_2'];
  }
  else {
    // Determine the user's country based on IP address of the page request.
    $ip = ip_address();
    $country_code = ip2country_get_country($ip);
  }

  return in_array($country_code, $countries);
}

/**
 * Implements hook_rules_action_info().
 */
function ip2country_rules_action_info() {
  $actions = array();

  $actions['ip2country_set_country'] = array(
    'label' => t("Sets the user's country"),
    'group' => t('User'),
    'base' => 'ip2country_action_set_country',
    'parameter' => array(
      'user' => array(
        'type' => 'user',
        'label' => t('User'),
      ),
      'country' => array(
        'type' => 'text',
        'label' => t('Country'),
        'options list' => 'country_get_list',
        'restriction' => 'input',
      ),
    ),
  );

  return $actions;
}

/**
 * Sets the country_iso_code_2 property of the global $user object.
 */
function ip2country_action_set_country($account, $country_code) {
  // Store the ISO country code in the $user object.
  user_save($account, array('country_iso_code_2' => $country_code));
}

/**
 * @}
 */

<?php

/**
 * Implements hook_rules_condition_info().
 */
function quova_rules_condition_info() {
  $conditions = array();

  $conditions['user_in_country'] = array(
    'label' => t('User is in country'),
    'parameter' => array(
      'country' => array(
        'type' => 'text',
        'label' => t('Country'),
        'description' => t('The lowercase name of the country'),
      ),
    ),
    'group' => t('User'),
    'callbacks' => array(
      'execute' => 'quova_condition_user_in_country',
    ),
  );

  return $conditions;
}


/**
 * User has role condition help callback.
 */
function quova_condition_user_in_country_help() {
  return t('Whether the user is in the selected country.');
}


/**
 * Check whether a user is in a country.
 */
function quova_condition_user_in_country($country) {
  $ipinfo = quova_ipinfo_request();
  if (!isset($ipinfo->ipinfo->Location->CountryData->country)) {
    return FALSE;
  }
  
  $user_country = $ipinfo->ipinfo->Location->CountryData->country;
  if ($user_country === $country) {
    return TRUE;
  }
  
  return FALSE;
}

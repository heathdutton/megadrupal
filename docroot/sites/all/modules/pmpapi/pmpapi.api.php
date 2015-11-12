<?php

/**
 * @file
 * Hooks provided by the PMPAPI module.
 */

/**
 * Add to PMP profile information
 *
 * @see pmpapi_get_profile_info()
 * @see pmpapi_get_profile_list()
 */
function hook_pmpapi_profile_info() {
  $profiles['segment'] = array(
    'title' => array(
      'type' => 'text',
      'accepted_types' => array('text'),
      'description' => t('The title of the doc.'),
      'required' => TRUE,
    ),
    'byline' => array(
      'type' => 'text',
      'accepted_types' => array('text'),
      'description' => t('Rendered byline as suggested by document distributor.'),
    ),
    'teaser' => array(
      'type' => 'text',
      'accepted_types' => array('text_textfield', 'text_long'),
      'description' => t('A short description, appropriate for showing in small spaces.'),
    ),
  );
  return $profiles;
}

/**
 * Alter PMP profile information provided by modules
 *
 * @param array $info
 *   Profile info being altered.
 *
 * @see pmpapi_get_profile_info()
 * @see pmpapi_get_profile_list()
 */
function hook_pmpapi_profile_info_alter(&$info) {
  unset($info['segment']['teaser']);
}

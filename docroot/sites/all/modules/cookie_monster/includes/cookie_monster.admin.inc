<?php
/**
 * Created by PhpStorm.
 * User: tbenice
 * Date: 6/7/14
 * Time: 5:35 PM
 */

/**
 * Admin form for cookie monster.
 * @param array $form
 * @param array $form_state
 */
function cookie_monster_admin_form($form, $form_state) {
  $form['cookie_monster_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Encryption Secret'),
    '#description' => t('Enter a string of 15-20 alpha-numeric characters that will be used in encryption'),
    '#default_value' => variable_get('cookie_monster_secret', NULL),
    '#element_validate' => array('cookie_monster_secret_validate'),
    '#required' => TRUE,
  );
  $form['cookie_monster_expiration'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie Expiration'),
    '#description' => t('Default expiration time for a cookie in hours.'),
    '#default_value' => cookie_monster_expire_default_value(variable_get('cookie_monster_expiration', 86400)),
    '#element_validate' => array('cookie_monster_expire_validate'),
    '#required' => TRUE,
    '#value_callback' => 'cookie_monster_expire_value',
  );
  return system_settings_form($form);
}

/**
 * Element validation for expire time in admin form.
 * @param array $element
 * @param array $form_state
 * @param array $form
 */
function cookie_monster_expire_validate($element, &$form_state, $form) {
  if (!is_numeric($element['#value'])) {
    form_set_error($element, t('Must be a numeric value'));
  }
}

/**
 * Converts timestamp into hours for admin form expire time.
 * @param int $default_value
 * @return Int
 *   Returns the default value / 3600 to return
 *   days.
 */
function cookie_monster_expire_default_value($default_value) {
  return $default_value / 3600;
}

/**
 * Element value callback for expire time.
 * @param array $element
 * @param array $input
 * @param array $form_state
 * @return int
 *   Returns the element default value * 3600
 *   to return timestamp.
 */
function cookie_monster_expire_value($element, $input = FALSE, $form_state = array()) {
  if ($input) {
    return $element['#default_value'] * 3600;
  }
}

/**
 * Element validation for secret. 15 - 20 characters.
 * @param array $element
 * @param array $form_state
 * @param array $form
 */
function cookie_monster_secret_validate($element, &$form_state, $form) {
  if(strlen($element['#value']) < 15 || strlen($element['#value']) > 20) {
    form_error($element, t('Secret must be between 15 and 20 characters in length'));
  }
}

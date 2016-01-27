<?php
/**
 * @file
 * Module that implements two factor authentication using Authy.
 */

/**
 * Menu callback: user/%user/authy.
 *
 * Administer Authy settings for user.
 */
function authy_page_manage($user) {
  module_load_include('inc', 'authy', 'authy.forms');
  $authy_id = _authy_get_authy_id($user->uid, TRUE);
  $authy_enabled = _authy_get_authy_id($user->uid);

  $output = array();

  $output['about'] = array(
    '#markup' => filter_xss(variable_get('authy_about'), array(
      'a',
      'em',
      'strong',
      'cite',
      'blockquote',
      'code',
      'ul',
      'ol',
      'li',
      'dl',
      'dt',
      'dd',
      'br',
      'p',
    )),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );

  if ($authy_enabled === FALSE) {
    $output['status'] = array(
      '#markup' => t('Authy is not enabled for this Account.'),
      '#prefix' => '<div class="messages warning"><ul><li>',
      '#suffix' => '</ul></div>',
    );

    if ($authy_id !== FALSE) {
      $output['enable_authy'] = drupal_get_form('authy_form_enable', $user);
    }
  }
  else {
    $output['status'] = array(
      '#markup' => t('Authy is enabled on this Account.'),
      '#prefix' => '<div class="messages status"><ul><li>',
      '#suffix' => '</ul></div>',
    );
    $output['disable_authy'] = drupal_get_form('authy_form_disable', $user);
  }

  if ($authy_id === FALSE) {
    $output['register_authy'] = drupal_get_form('authy_form_register', $user);
  }

  return $output;
}
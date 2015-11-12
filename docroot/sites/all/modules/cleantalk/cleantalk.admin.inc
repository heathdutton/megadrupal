<?php

/**
 * @file
 * CleanTalk module admin functions.
 */

/**
 * Cleantalk settings form.
 */
function cleantalk_settings_form($form, &$form_state) {
  $ct_comments_default = 3;

  $ct_authkey = variable_get('cleantalk_authkey', '');
  $ct_comments = variable_get('cleantalk_comments', $ct_comments_default);
  $ct_automod = variable_get('cleantalk_automod', 1);

  $form['cleantalk_authkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Access key'),
    '#size' => 20,
    '#maxlength' => 20,
    '#default_value' => $ct_authkey ? $ct_authkey : '',
    '#description' => t(
      'Click <a target="_blank" href="!ct_link">here</a> to get access key.',
      array(
        '!ct_link' => url('http://cleantalk.org/register?platform=drupal'),
      )
    ),
  );

  $form['cleantalk_comments'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum approved comments per registered user'),
    '#size' => 5,
    '#maxlength' => 5,
    '#default_value' => $ct_comments,
    '#element_validate' => array('element_validate_integer_positive'),
    '#description' => t('Moderate messages of guests and registered users who have approved messages less than this value (must be more than 0).'),
  );

  $form['cleantalk_automod'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable automoderation'),
    '#default_value' => $ct_automod,
    '#description' => t('Automatically publish good messages and put bad ones to admin approvement.') .
    '<br /><span class="admin-enabled">' .
    t('Note: It overrides "Skip comment approval" permissions') .
    '</span>',
  );

  return system_settings_form($form);
}

<?php
/**
 * @file
 * Admin forms for lyris member administration.
 */
/**
 * Set unsubscribe messages for the unsubscribe landing page.
 */
function lyris_member_unsubscribe_messages_form($form, &$form_state) {
  $defaults[LYRIS_MEMBER_UNSUB_ERROR]    = variable_get('lyris_member_unsub_error_message');
  $defaults[LYRIS_MEMBER_UNSUB_NOLIST]   = variable_get('lyris_member_unsub_nolist_message');
  $defaults[LYRIS_MEMBER_UNSUB_NOMEMBER] = variable_get('lyris_member_unsub_nomember_message');
  $defaults[LYRIS_MEMBER_UNSUB_SUCCESS]  = variable_get('lyris_member_unsub_success_message');

  // Setup the vertical tabs
  $form['messages'] = array(
    '#type' => 'vertical_tabs',
  );

  // Define a fieldset for tokens that we can reuse.
  $form['tokens'] = array(
    '#type' => 'lyris_token_fieldset',
    'list' => array(
      '#theme' => 'token_tree',
      '#token_types' => array(),
    ),
  );

  // Success Message
  $form[LYRIS_MEMBER_UNSUB_SUCCESS] = array(
    '#type' => 'fieldset',
    '#title' => t('Success Message'),
    '#group' => 'messages',
  );
  $form[LYRIS_MEMBER_UNSUB_SUCCESS]['lyris_member_unsub_success_message'] = array(
    '#type' => 'text_format',
    '#description' => t('Display this message when a member is successfully unsubscribed from a list.'),
    '#default_value' => ($defaults[LYRIS_MEMBER_UNSUB_SUCCESS]['value'] ? $defaults[LYRIS_MEMBER_UNSUB_SUCCESS]['value'] : LYRIS_MEMBER_UNSUB_SUCCESS_MSG),
    '#format' => $defaults[LYRIS_MEMBER_UNSUB_SUCCESS]['format'],
    '#required' => TRUE,
  );
  $form[LYRIS_MEMBER_UNSUB_SUCCESS]['token'] = $form['tokens'];
  $form[LYRIS_MEMBER_UNSUB_SUCCESS]['token']['list']['#token_types'] += array('lyris_list', 'lyris_member');

  // No List Message
  $form[LYRIS_MEMBER_UNSUB_NOLIST] = array(
    '#type' => 'fieldset',
    '#title' => t('List-Not-Found Message'),
    '#group' => 'messages',
  );
  $form[LYRIS_MEMBER_UNSUB_NOLIST]['lyris_member_unsub_nolist_message'] = array(
    '#type' => 'text_format',
    '#description' => t('Display this message when a list can\'t be found by the specified hash and the member is not unsubscribed.'),
    '#default_value' => ($defaults[LYRIS_MEMBER_UNSUB_NOLIST]['value'] ? $defaults[LYRIS_MEMBER_UNSUB_NOLIST]['value'] : LYRIS_MEMBER_UNSUB_NOLIST_MSG),
    '#format' => $defaults[LYRIS_MEMBER_UNSUB_NOLIST]['format'],
    '#required' => TRUE,
  );
  $form[LYRIS_MEMBER_UNSUB_NOLIST]['token'] = $form['tokens'];

  // No Member Message
  $form[LYRIS_MEMBER_UNSUB_NOMEMBER] = array(
    '#type' => 'fieldset',
    '#title' => t('Member-Not-Found Message'),
    '#group' => 'messages',
  );
  $form[LYRIS_MEMBER_UNSUB_NOMEMBER]['lyris_member_unsub_nomember_message'] = array(
    '#type' => 'text_format',
    '#description' => t('Display this message when a Lyris member cannot be retrieved for the given list.'),
    '#default_value' => ($defaults[LYRIS_MEMBER_UNSUB_NOMEMBER]['value'] ? $defaults[LYRIS_MEMBER_UNSUB_NOMEMBER]['value'] : LYRIS_MEMBER_UNSUB_NOMEMBER_MSG),
    '#format' => $defaults[LYRIS_MEMBER_UNSUB_NOMEMBER]['format'],
    '#required' => TRUE,
  );
  $form[LYRIS_MEMBER_UNSUB_NOMEMBER]['token'] = $form['tokens'];
  $form[LYRIS_MEMBER_UNSUB_NOMEMBER]['token']['list']['#token_types'][] = 'lyris_list';

  // Error Message
  $form[LYRIS_MEMBER_UNSUB_ERROR] = array(
    '#type' => 'fieldset',
    '#title' => t('Error Message'),
    '#group' => 'messages',
  );
  $form[LYRIS_MEMBER_UNSUB_ERROR]['lyris_member_unsub_error_message'] = array(
    '#type' => 'text_format',
    '#description' => t('Display this message when there is an error processing the request and the member is not unsubscribed.'),
    '#default_value' => ($defaults[LYRIS_MEMBER_UNSUB_ERROR]['value'] ? $defaults[LYRIS_MEMBER_UNSUB_ERROR]['value'] : LYRIS_MEMBER_UNSUB_ERROR_MSG),
    '#format' => $defaults[LYRIS_MEMBER_UNSUB_ERROR]['format'],
    '#required' => TRUE,
  );
  $form[LYRIS_MEMBER_UNSUB_ERROR]['token'] = $form['tokens'];
  $form[LYRIS_MEMBER_UNSUB_ERROR]['token']['list']['#token_types'] += array('lyris_list', 'lyris_member');

  // Remove the tokens template item
  unset($form['tokens']);

  return system_settings_form($form);
}

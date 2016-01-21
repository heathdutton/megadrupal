<?php

function nodegift_delete_confirm_form($form, &$form_state, $gift_id = NULL, $account_id = NULL)
{
  if (!$gift_id) return;

  $nid = db_query("SELECT nid from {node_gift} where gift_id = :gift_id", array(':gift_id' => (int) $gift_id))->fetchField();
  $node = node_load($nid);
  if (!$node) {
    drupal_goto('user/' .$account_id. '/gifts');
    return;
  }

  $form['gift_id'] = array('#type' => 'value', '#value' => (int) $gift_id);
  $form['user_id'] = array('#type' => 'value', '#value' => (int) $account_id);


  $message = t('Are you sure you want to remove gift <em>@gift</em> from your gifts?', array('@gift' => check_plain($node->title)));
  $caption = '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, $message, 'user/' . $account_id . '/gifts', $caption, t('Delete'));
}

function nodegift_delete_confirm_form_submit($form, &$form_state)
{

  $gift_id = $form_state['values']['gift_id'];

  db_delete('node_gift')
    ->condition('gift_id', $gift_id)
    ->execute();

  drupal_set_message(t('Gift was deleted from your page'));

  $form_state['redirect'] = 'user/' . $form_state['values']['user_id'] . '/gifts';

}


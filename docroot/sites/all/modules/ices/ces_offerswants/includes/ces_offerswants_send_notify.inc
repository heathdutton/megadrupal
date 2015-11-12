<?php
/**
 * @file
 * Implements the drupal hooks for this module.
 */

/**
 * @defgroup ces_offerswants_send_notify Ces OffersWants Send Notify
 * @ingroup ces_offerswants
 * @{
 * Implements the drupal hooks for send order.
 */

/**
 * Form for send order.
 */
function ces_offerwant_send_notify_form($form, &$form_state, $extra = FALSE, $offer = FALSE) {
  $form['offer'] = array(
    '#type' => 'value',
    '#value' => $offer->id,
  );
  $form['body'] = array(
    '#type' => 'textarea',
    '#description' => t('Enter here a description.'),
    '#title' => t('Description'),
    '#default_value' => '',
    '#required' => TRUE,
    '#weight' => 2,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
    '#weight' => 100,
  );
  return $form;
}
/**
 * Validate form for send order.
 */
function ces_offerwant_send_notify_form_validate($form, &$form_state) {
  if ($form_state['values']['body'] == '') {
    form_set_error('body', t('The body of message is empty.'));
  }
}
/**
 * Submit form for send order.
 */
function ces_offerwant_send_notify_form_submit($form, &$form_state) {
  global $user;
  $offer = $form_state['build_info']['args'][1];
  $notify = array('body' => $form_state['values']['body'], 'user' => $user->uid);
  ces_message_send_notify('send_offerwant_notify', array('offer' => $offer, 'notify' => $notify), array($offer->user), $user->uid);
}
/** @} */

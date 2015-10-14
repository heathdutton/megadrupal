<?php

/**
 * @file
 * Page callbacks for Ubercart Payment gateway for Element Payment Services.
 */

/**
 * User stored cards page. Display a list of the user's cards.
 *
 * @param int $uid
 *   User ID of the user whose stored cards are being viewed.
 *
 * @return array
 *   render array for a table containing user's stored cards
 */
function uc_element_user_card_list($uid) {
  $user = user_load($uid);
  drupal_set_title(t('@user Stored Credit Cards', array('@user' => $user->name)));
  // Gets the info about the stored card.
  $card = _uc_element_card_get($uid);

  $table_rows = array();
  if (!empty($card->last_4)) {
    $table_rows[] = array(
      '************' . $card->last_4,
      array(
        'data' => str_replace('.', '/', $card->added),
        'class' => array('extra'),
      ),
      str_pad($card->exp_month, 2, '0', STR_PAD_LEFT) . '/' . $card->exp_year,
      l(t('delete'), 'user/' . $uid . '/cards/delete'),
    );
  }
  return array(
    '#theme' => 'table',
    '#header' => array(
      t('Stored Card'),
      array(
        'data' => t('Date added'),
        'class' => array('extra'),
      ),
      t('Expiration'),
      t('Actions'),
    ),
    '#rows' => $table_rows,
    '#attributes' => array('class' => array('sticky-table')),
    '#caption' => '',
    '#colgroups' => array(),
    '#sticky' => TRUE,
    '#empty' => t('No Stored Credit Cards found'),
  );
}

/**
 * Form for deleting stored credit cards by user.
 *
 * Deleting a stored card through the user interface
 * Superficial delete - does not delete through Element
 * TODO: option to delete through Element API as well.
 */
function uc_element_delete_form($node, &$form_state, $uid) {
  // Get the info about the stored card.
  $card = _uc_element_card_get($uid);

  // Set the title now that we know what the card information is.
  drupal_set_title(t('Delete Stored Card: ****@lastfour', array('@lastfour' => $card->last_4)));

  // Render the form.
  $form = array();
  $form['uid'] = array(
    '#type' => 'hidden',
    '#value' => $uid,
  );
  $form['areyousure'] = array('#markup' => '<div>' . t('Are you sure you want to delete this card?') . '</div>');
  $form['submit_uc_element_delete_form'] = array('#type' => 'submit', '#value' => t('Delete'));
  return $form;
}

/**
 * Implements hook_form_submit().
 */
function uc_element_delete_form_submit($form, &$form_state) {
  // Get the info about the stored card.
  $card = _uc_element_card_get($form_state['values']['uid']);

  // Delete the card superficially - this is a database-only delete.
  _uc_element_token_delete($form_state['values']['uid']);

  drupal_set_message(t('Card @lastfour successfully deleted', array('@lastfour' => '****' . $card->last_4)), 'status');
  $form_state['redirect'] = 'user/' . $form_state['values']['uid'] . '/cards';
}

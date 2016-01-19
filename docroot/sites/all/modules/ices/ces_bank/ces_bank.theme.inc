<?php
/**
 * @file
 * All theme default functions for bank module.
 */

/**
 * @defgroup ces_bank_theme Theme functions from Ces Bank
 * @ingroup ces_bank
 * @{
 * All theme default functions for bank module.
 */

/**
 * Theme function for page ces_bank_exchange_view().
 */
function theme_ces_bank_exchange_view($variables) {
  $elem = $variables['element']['exchange'];
  $out = '<h2>' . $elem['name'] . '</h2>';
  $out .= '<dl>';
  $out .= '<dt>' . t('Code') . '</dt>';
  $out .= '<dd>' . $elem['code'] . '</dd>';
  $out .= '<dt>' . t('Website') . '</dt>';
  $out .= '<dd>' . $elem['website'] . '</dd>';
  $out .= '<dt>' . t('Country') . '</dt>';
  $out .= '<dd>' . $elem['country'] . '</dd>';
  $out .= '<dt>' . t('Town') . '</dt>';
  $out .= '<dd>' . $elem['town'] . ' (' . $elem['region'] . ')</dd>';
  $out .= '<dt>' . t('Currency') . '</dt>';
  $out .= '<dd>' . $elem['currencyname'] . ', ' . $elem['currencysymbol'] . '</dd>';
  $out .= '<dt>' . t('Currency value') . '</dt>';
  $out .= '<dd>' . $elem['currencyvalue'] . '</dd>';
  $out .= '<dt>' . t('Since') . '</dt>';
  $out .= '<dd>' . format_date($elem['created']) . '</dd>';
  $out .= '</dl>';
  return $out;
}
/**
 * Theme function for page ces_bank_account_view().
 */
function theme_ces_bank_account_view($variables) {
  $elem = $variables['element']['account'];
  $out = '<h2>' . t('Account details') . '</h2>';
  $out .= '<dl>';
  $out .= '<dt>' . t('Exchange') . '</dt>';
  $out .= '<dd>' . $elem['exchange'] . '</dd>';
  $out .= '<dt>' . t('Account number') . '</dt>';
  $out .= '<dd>' . $elem['name'] . '</dd>';
  $out .= '<dt>' . t('Username') . '</dt>';
  $out .= '<dd>' . $elem['username'] . '</dd>';
  $out .= '<dt>' . t('Type') . '</dt>';
  $out .= '<dd>' . $elem['kind'] . '</dd>';
  $out .= '<dt>' . t('Limit') . '</dt>';
  $out .= '<dd>' . $elem['limitchain'] . '</dd>';
  $out .= '<dt>' . t('State') . '</dt>';
  $out .= '<dd>' . $elem['state'] . '</dd>';
  $out .= '</dl>';
  return $out;
}
/**
 * Theme function for page ces_bank_transaction_view().
 */
function theme_ces_bank_transaction_view($variables) {
  $tran = $variables['element']['transaction'];
  $from = $variables['element']['fromaccount'];
  $to = $variables['element']['toaccount'];
  $fex = $variables['element']['fromexchange'];
  $tex = $variables['element']['toexchange'];

  $out = '<dl class="ces-transaction-view">';

  if ($fex['id'] == $tex['id']) {
    $out .= '<dt>' . t('Exchange') . '</dt>';
    $out .= '<dd>' . $fex['name'] . '</dd>';
  }
  if ($fex['id'] != $tex['id']) {
    $out .= '<dt>' . t("Buyer's exchange") . '</dt>';
    $out .= '<dd>' . $fex['name'] . '</dd>';
  }
  $out .= '<dt>' . t('Buyer') . '</dt>';
  $out .= '<dd>' . $from['name'];
  $i = 0;
  foreach ($from['users'] as $accuser) {
    $out .= ($i > 0) ? ', ' : ' - ';
    $out .= l(ces_user_get_name(user_load($accuser['user'])), 'user/' . $accuser['user']);
    $i++;
  }
  $out .= '</dd>';
  if ($fex['id'] != $tex['id']) {
    $out .= '<dt>' . t("Seller's exchange") . '</dt>';
    $out .= '<dd>' . $tex['name'] . '</dd>';
  }
  $out .= '<dt>' . t('Seller') . '</dt>';
  $out .= '<dd>' . $to['name'];
  $i = 0;
  foreach ($to['users'] as $accuser) {
    $out .= ($i > 0) ? ', ' : ' - ';
    $out .= l(ces_user_get_name(user_load($accuser['user'])), 'user/' . $accuser['user']);
    $i++;
  }
  $out .= '</dd>';
  $out .= '<dt>' . t('Description') . '</dt>';
  $out .= '<dd>' . check_plain($tran['concept']) . '</dd>';

  $out .= '<dt>' . t('Amount') . '</dt>';
  $out .= '<dd><strong>' . $tran['amount'] . '</strong></dd>';
  $out .= '<dt>' . t('State') . '</dt>';
  $out .= '<dd>' . $tran['state'] . '</dd>';
  $out .= '<dt>' . t('Date') . '</dt>';
  $out .= '<dd>' . format_date($tran['created']) . '</dd>';
  if ($tran['created'] != $tran['modified']) {
    $out .= '<dt>' . t('Modified') . '</dt>';
    $out .= '<dd>' . format_date($tran['modified']) . '</dd>';
  }
  $out .= '<dt>' . t('Entered by') . '</dt>';
  $out .= '<dd>' . l(ces_user_get_name(user_load($tran['user'])), 'user/' . $tran['user']) . '</dd>';
  $out .= '</dl>';
  return $out;
}
/**
 * Theme function for page ces_bank_limitchain_view().
 *
 * @todo move this to page function! -My God, this is logic!!
 */
function theme_ces_bank_limitchain_view($variables) {
  drupal_add_tabledrag('ces-bank-limitchain-table', 'order', 'sibling', 'ces-bank-limit-weight');
  $limitchain = $variables['element']['limitchain'];
  $classes = $variables['element']['limitclasses'];
  $out = '';
  $out .= '<h2>' . $limitchain['name'] . '</h2>';
  // @todo ¿description?
  // $out .= '<div>';
  // $out .= filter_xss($limitchain['description']);
  // $out .= '</div>';
  $out .= '<div>';
  $out .= '<h3>' . t('Limits') . '</h3>';
  if (empty($limitchain['limits'])) {
    $out .= t('No limits.');
  }
  else {
    $table = array(
      '#theme' => 'table',
      '#attributes' => array(
        'class' => array('ces-table'),
      ),
      '#header' => array(t('Type'), t('Value'), t('Action'), t('Weight')),
      '#rows' => array(),
      '#attributes' => array(
        'id' => array('ces-bank-limitchain-table'),
      ),
    );
    $count = 1;
    foreach ($limitchain['limits'] as $limit) {
      // @todo debug !! this line must be removed!
      $limit['weight'] = $count;
      $type = array(
        '#type' => 'textfield',
        '#value' => $limit['classname'],
        '#size' => 15,
      );
      $value = array(
        '#type' => 'textfield',
        '#value' => $limit['value'],
        '#size' => 15,
      );
      $weight = array(
        '#type' => 'textfield',
        '#default_value' => $limit['weight'],
        '#size' => 3,
        '#attributes' => array(
          'class' => array('ces-bank-limit-weight'),
        ),
      );
      $table['#rows'][] = array(
        'data' => array(
          $type['#value'],
          $value['#value'],
          $limit['block'] ? t('Block') : t('Warn'),
          drupal_render($weight),
        ),
        'class' => array('draggable'),
      );
      $count++;
    }
    $out .= drupal_render($table);
  }
  $out .= '</div>';
  return $out;
}
/**
 * @}
 */

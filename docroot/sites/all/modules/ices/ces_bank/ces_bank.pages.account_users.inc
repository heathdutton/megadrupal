<?php
/**
 * @file
 * Account users pages callback functions for bank module.
 */

/**
 * @defgroup ces_bank_pages_account_users Pages from Ces Bank Account Users
 * @ingroup ces_bank
 * @{
 * All page callback functions for users account in bank module.
 */

/**
 * Admin users of account.
 */
function ces_bank_account_users_admin($administrator = NULL, $account = NULL) {
  if ($account == NULL) {
    $account = ces_bank_get_current_account();
  }
  $t_list_users_account = t('List users account');
  drupal_set_title($t_list_users_account . ' ' . $account['name']);
  $s = new IcesSerializer('CesBankLocalAccount');
  $account = $s->loadFromUniqueKey('name', $account['name']);
  // @todo Comprobar permisos con cuenta.
  $page = array();
  $page['table'] = array(
    '#theme' => 'table',
    '#attributes' => array(
      'class' => array('ces-table'),
    ),
    '#header' => array(
      array(
        'data' => t('Name'),
        'field' => 'name',
      ),
      array(
        'data' => t('Mail'),
        'field' => 'kind',
      ),
      array(
        'data' => t('Privilege'),
        'field' => 'privilege',
      ),
      array(
        'data' => t('Actions'),
      ),
    ),
    '#rows' => array(),
  );

  $account_users = $account->getUsers();
  $rows = array();
  foreach ($account_users as $account_user) {
    $user_drupal = user_load($account_user->user);
    if ($administrator) {
      $link_delete = 'ces/admin/account/users/' . $account->id . '/users_account/' . $account_user->user . '/delete';
      $link_edit   = 'ces/admin/account/users/' . $account->id . '/users_account/' . $account_user->user . '/edit';
      $link_delete_not_user   = 'ces/admin/account/users/' . $account->id . '/users_account/' . $account_user->user . '/delete_not_user';
    }
    else {
      $link_delete = 'ces/bank/account/' . $account_user->user . '/users_account/delete';
      $link_edit   = 'ces/bank/account/' . $account_user->user . '/users_account/edit';
    }
    if ($user_drupal) {
      $row = array(
        $user_drupal->name,
        $user_drupal->mail,
        ($account_user->privilege == 0) ? t('yes') : t('No'),
        l(t('Edit'), $link_edit) . ' ' . l(t('Delete'), $link_delete),
      );
    }
    else {
      $row = array(
        t('Nonexistent user'),
        '',
        '',
        (isset($link_delete_not_user)) ? l(t('Delete'), $link_delete_not_user) : '',
      );
    }
    $rows[] = $row;
  }
  $page['table']['#rows'] = $rows;
  return $page;

}
/**
 * @}
 */

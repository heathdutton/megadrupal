<?php


/**
 * Implements hook_dqx_adminmenu_items()
 */
function masquerade_dqx_adminmenu_items() {
  global $user;
  $items = array();

  if (isset($_SESSION['masquerading'])) {
    if ($user->uid > 0) {
      $title = t('You are masquerading as %masq_as.', array('%masq_as' => $user->name));
    }
    else {
      $title = t('You are masquerading as %anonymous.', array('%anonymous' => variable_get('anonymous', t('Anonymous'))));
    }
    $path = 'masquerade/unswitch';
    $item = array(
      'title' => t('Switch back'),
      'link_path' => $path,
      'weight' => -1,
      'attributes' => array('title' => $title),
    );
    $item['localized_options']['query']['token'] = drupal_get_token($path);
    $item['localized_options']['attributes']['title'] = $title;
    $item['item_attributes']['class'] = 'dqx-adminmenu-masquerade dqx-adminmenu-masquerade-unswitch';
    $items['dqx-adminmenu-user/masquerade'] = $item;
  }
  else {

    $item = array(
      'title' => t('Administer masquerade'),
      'link_path' => 'admin/config/people/masquerade',
      'weight' => -1,
    );
    $item['localized_options']['attributes']['title'] = t('Administer masquerade');
    $item['item_attributes']['class'] = 'dqx-adminmenu-masquerade';
    $items['dqx-adminmenu-user/masquerade'] = $item;

    $quick_switches = variable_get('masquerade_quick_switches', array());

    // Add in user-specific switches, and prevent duplicates.
    $user_switches = db_query(
      "SELECT uid_to FROM {masquerade_users} WHERE uid_from = :uid_from",
      array(':uid_from' => $user->uid)
    )->fetchCol();
    $masquerade_switches = array_unique(array_merge($quick_switches, $user_switches));

    foreach ($masquerade_switches as $switch_user) {
      if (!isset($_SESSION['user']->uid) || $switch_user != $_SESSION['user']->uid) {
        $account = user_load($switch_user);
        if (isset($account->uid)) {
          $item = array();
          $path = 'masquerade/switch/' . $account->uid;
          $item['link_path'] = $path;
          $item['localized_options']['query']['token'] = drupal_get_token($path);
          if ($account->uid) {
            $item['title'] = $account->name;
          }
          if ($switch_user == 0) {
            $item['title'] = variable_get('anonymous', t('Anonymous'));
          }
          $item['localized_options']['attributes']['title'] = t('Masquerade as !user', array('!user' => $item['title']));
          $items['dqx-adminmenu-user/masquerade/' . $account->uid] = $item;
        }
      }
    }
  }

  return $items;
}


/**
 * Implements hook_dqx_adminmenu_structure_alter()
 */
function masquerade_dqx_adminmenu_structure_alter($api) {
  // $api->submenuAddChild('admin
}

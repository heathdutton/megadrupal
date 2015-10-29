<?php
// @file
// Wishlist management module for Drupal
// Written by Scott McLewin and Melanie Paul-McLewin
// drupal AT mclewin DOT com

/**
 * Administration settings for the wishlist module
 *
 */
function wishlist_admin_settings($form, &$form_state) {

  // Drupal 4.7 forms api overview   http://drupal.org/node/33338
  // Permission Check
  if (!user_access('admin wishlist')) {
    return drupal_access_denied();
  }

  $form["wishlist_help"] = array(
    '#type' => 'textarea',
    '#title' => t("Explanation or submission guidelines"),
    '#default_value' => variable_get("wishlist_help", ""),
    '#cols' => 55,
    '#rows' => 4,
    '#description' => t("This text will be displayed at the top of the wishlist item submission form.  Useful for helping or instructing your users."),
  );

  $form["wishlist_block_max_names"] = array(
    '#type' => 'textfield',
    '#title' => t("Maximum number of names to show in Wishlists block"),
    '#default_value' => variable_get("wishlist_block_max_names", "5"),
    '#size' => 2,
    '#maxlength' => 2,
    '#description' => t("The maximum number of wishlists to display in the block."),
  );


  $form["wishlist_item_list_max_description"] = array(
    '#type' => 'textfield',
    '#title' => t("Item list description limit"),
    '#default_value' => variable_get("wishlist_item_list_max_description", "200"),
    '#size' => 2,
    '#maxlength' => 3,
    '#description' => t("Limit the length of the description that is shown on the view of all wishlist items."),
  );

  $form['wishlist_currency'] = array(
    '#type' => 'fieldset',
    '#title' => t('Currency'),
    '#collapsible' => FALSE,
  );
  $form['wishlist_currency']["wishlist_show_currency"] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the currency field'),
    '#return_value' => TRUE,
    '#default_value' => variable_get("wishlist_show_currency", TRUE),
    '#description' => t('By default the currency field for the price can be set by the user on each wishlist item.  If your site only needs to deal in a single currency, you can hide the field.  All entries will be saved with the default currency set below.'),
  );
  $form['wishlist_currency']["wishlist_default_currency"] = array(
    '#type' => 'textfield',
    '#title' => t('Default currency'),
    '#default_value' => variable_get("wishlist_default_currency", "USD"),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t("Enter the default three letter ISO currency code for new wishlist items."),
  );


  $form['wishlist_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Misc. options'),
    '#collapsible' => FALSE,
  );

  $form['wishlist_options']["wishlist_url_in_new_window"] = array(
    '#type' => 'checkbox',
    '#title' => t("Open URL links in new window"),
    '#return_value' => TRUE,
    '#default_value' => variable_get("wishlist_url_in_new_window", TRUE),
    '#description' => t("When checked clicking on either the primary or secondary URL fields will open a new browser window."),
  );

  $form['wishlist_hideopts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Item purchased status protection options'),
    '#collapsible' => FALSE,
  );
  $form['wishlist_hideopts']["wishlist_hide_purchase_info_anonymous"] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide the purchase information from anonymous users'),
    '#return_value' => TRUE,
    '#default_value' => variable_get("wishlist_hide_purchase_info_anonymous", FALSE),
    '#description' => t('Check this box to hide the purchase information about an item for anonymous users.  When this is checked, only authenticated users will be able to see whether an item remains available to purchase.  This will also remove all links from purchase URLs to prevent an unwitting anonymous user from purchasing an item without indicating it was purchased.  This is a way to prevent your users from peeking at the wishlist by visiting the site anonymously.'),
  );

  $form['wishlist_hideopts']['wishlist_hide_purchase_info_own'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide the purchase information from the user on their own wishlist by default'),
    '#return_value' => TRUE,
    '#default_value' => variable_get('wishlist_hide_purchase_info_own', FALSE),
    '#description' => t('Select this box to hide the purchase information about an item for the owner of the wishlist.  When this is checked users will not see the purchased information on their wishlist entries by default.  They will need to explicitly choose to reveal purchased items.'),
  );
  $form['wishlist_hideopts']['wishlist_hide_admins_own_items'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide your (the admin) wishlist items when viewing the wishlist administration screen'),
    '#return_value' => TRUE,
    '#default_value' => variable_get('wishlist_hide_admins_own_items', FALSE),
    '#description' => t('This only applies to site administrators.  If checked, the site administrator will not see information about their own items in the wishlist administration screen'),
  );


  $form["wishlist_item_expire_days"] = array(
    '#type' => 'textfield',
    '#title' => t('Days before a fully purchased item is hidden'),
    '#default_value' => variable_get('wishlist_item_expire_days', 30),
    '#size' => 2,
    '#maxlength' => 2,
    '#description' => t("The number of days to leave an item on a user's public wishlist after it has been fully purchased.  After this many days, the item will be moved to the user's private wishlist.  It will not be deleted.  Set to 0 to disable automatic purchased item hiding."),
  );

  $form["wishlist_display_count"] = array(
    '#type' => 'textfield',
    '#title' => t('Number of items to display in the user\'s wishlist summary'),
    '#default_value' => variable_get('wishlist_display_count', 10),
    '#size' => 2,
    '#maxlength' => 2,
    '#description' => t('This is the number of items to display on the wishlist summary page.  This value will be applied to both the public wishlist and the user\'s private wishlist.'),
  );

  $form['wishlist_table'] = array(
    '#type' => 'fieldset',
    '#title' => t('Control which columns are displayed'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $columns = array(
    'wishlist_show_action' => t('Action'),
    'wishlist_show_title' => t('Title'),
    'wishlist_show_description' => t('Description'),
    'wishlist_show_priority' => t('Priority'),
    'wishlist_show_cost' => t('Cost'),
    'wishlist_show_quantity' => t('Quantity'),
    'wishlist_show_urls' => t('URLs'),
    'wishlist_show_updated' => t('Last updated'),
  );


  $form['wishlist_table']['wishlist_showcolumn'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show the following columns in the wishlist table view'),
    '#default_value' => variable_get('wishlist_showcolumn', array('wishlist_show_action', 'wishlist_show_title', 'wishlist_show_description', 'wishlist_show_priority', 'wishlist_show_cost', 'wishlist_show_quantity', 'wishlist_show_urls', 'wishlist_show_updated')),
    '#options' => $columns,
    '#description' => t('This controls which column show up in the wishlist summary view.'),
  );

  $form['wishlist_user_name_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Tokens for user name'),
    '#default_value' => variable_get('wishlist_user_name_token', ''),
    '#size' => 60,
    '#maxlength' => 255,
    '#description' => t('Replace giftee user name with tokens. Token module must be enabled. Leave blank to use standard user name.'),
  );
  
  if (module_exists('token')) {
    $form['wishlist_user_name_token_help'] = array(
      '#title' => t('Replacement patterns'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      );

    $form['wishlist_user_name_token_help']['help'] = array(
      '#theme' => 'token_tree',
      '#token_types' => array('user'), 
      '#global_types' => FALSE,
      '#click_insert' => TRUE,
    );
  }

  return system_settings_form($form);
}
/**
 * Function to display the wishlist module's admin page, where an administrator
 * can view all purchase records and delete them if necessary.
 */
function wishlist_admin_page() {
  global $user;

/*
  $sql = "SELECT p.*, buyer.name as buyer_name, receiver.name as receiver_name, n.title, n.nid FROM {wishlist_purchased} p
             INNER JOIN {node} n ON n.nid = p.wishlist_purch_nid
            INNER JOIN {users} buyer ON buyer.uid = p.wishlist_purch_buyer_uid
            INNER JOIN {users} receiver ON receiver.uid = n.uid ";
*/
  $select = db_select('wishlist_purchased', 'p');
  $select->join('node', 'n', 'n.nid = p.wishlist_purch_nid');
  $select->join('users', 'buyer', 'buyer.uid = p.wishlist_purch_buyer_uid');
  $select->join('users', 'receiver', 'receiver.uid = n.uid');
  $select->fields('p', array('wishlist_purch_wid', 'wishlist_purch_nid', 'wishlist_purch_buyer_uid', 'wishlist_purch_quantity', 'purch_date'));
  $select->addField('buyer', 'name', 'buyer_name');
  $select->addField('receiver', 'name', 'receiver_name');
  $select->fields('n', array('title', 'nid'));
  
  // If the option to suppress having the admin see their own items is on, then remove em from the query.
  if (variable_get('wishlist_hide_admins_own_items', FALSE)) {
//    $sql .= "WHERE receiver.uid != %d ";
    $select->condition('receiver.uid', $user->uid, '<>');
  }
//  $sql .= "ORDER BY p.purch_date";
  $select->orderBy('p.purch_date');
//  $result = db_query($sql, $user->uid);
  $result = $select->execute();

  $header = array(
    array('data' => t('Purchaser')),
    array('data' => t('On')),
    array('data' => t('Quantity')),
    array('data' => t('Item')),
    array('data' => t('For')),
    array('data' => t('Action')),
  );
  $rows = array();

  while ($row = $result->fetchObject()) {
    $rows[] = array(
      array('data' => check_plain($row->buyer_name)),
      array('data' => ($row->purch_date != 0) ? format_date($row->purch_date, 'custom', 'j M y') : t('unknown')),
      array('data' => $row->wishlist_purch_quantity),
      array('data' => l($row->title, 'node/' . $row->nid)),
      array('data' => check_plain($row->receiver_name)),
      array('data' => l(t('Delete'), 'admin/config/content/wishlist/delete/' . $row->nid . '/' . $row->wishlist_purch_wid)),
    );
  }

  $header_text = '<div>' . t('This is a list of all purchase records for your site.  The Delete action will delete the record of the purchase, not the item itself.') . '</div>';

  if (variable_get('wishlist_hide_admins_own_items', FALSE)) {
    $header_text .= '<div>' . t('The site is configure to not display your items on this list') . '</div>';
  }

  $output = '<div class="wishlist-admin">';
  $output .= $header_text . theme('table', array('header' => $header, 'rows' => $rows));
  $output .= '</div>';

  return $output;
}

/**
 * Form constructor for callback to delete wishlist purchase item
 *
 * @param $delete_nid
 *   The node ID for the item where some of the purchased quantity will be returned
 * @param $delete_wp
 *   The ID of the wishlist_purchased record to delete.
 *
 * @see _wishlist_admin_delete_item_submit()
 *
 * @ingroup forms
 */
function _wishlist_admin_delete_item($form, &$form_state, $delete_nid, $delete_wp) {
  if (!is_numeric($delete_nid) || !is_numeric($delete_wp)) {
    drupal_set_message(t('Wishlist purchase item was not deleted. The parameters must be numeric.'), 'error');
    drupal_goto('admin/reports/wishlist');
  }

  $form['delete_nid'] = array(
    '#type' => 'value',
    '#value' => $delete_nid,
  );
  $form['delete_wp'] = array(
    '#type' => 'value',
    '#value' => $delete_wp,
  );

  $question = t('Are you sure you want to delete this wishlist purchase item?');
  return confirm_form($form, $question, 'admin/reports/wishlist', '', t('Delete'), t('Cancel'));
}

/**
 * Form submission handler for _wishlist_admin_delete_item().
 */
function _wishlist_admin_delete_item_submit($form, &$form_state) {
  global $user;
  $form_state['redirect'] = 'admin/reports/wishlist';

  $delete_nid = $form_state['values']['delete_nid'];
  $delete_wp = $form_state['values']['delete_wp'];

  if (!is_numeric($delete_nid) || !is_numeric($delete_wp)) {
    drupal_set_message(t('Wishlist purchase item was not deleted. The parameters must be numeric.'), 'error');
    return;
  }

  $select = db_select('wishlist_purchased', 'p');
  $select->fields('p', array('wishlist_purch_nid', 'wishlist_purch_wid', 'wishlist_purch_buyer_uid', 'wishlist_purch_quantity'));
  $select->condition('wishlist_purch_wid', $delete_wp);
  $result = $select->execute();
  if ($wishlist_purch = $result->fetchObject()) {
    if ($delete_nid != $wishlist_purch->wishlist_purch_nid) {
      drupal_set_message(t('Inputs do not agree.  Node @delete_nid is not related to wpid @delete_wp', array(
        '@delete_nid' => $delete_nid,
        '@delete_wp' => $delete_wp,
      )), 'error');
      return;
    }
  }
  else {
    drupal_set_message(t('Invalid record - item return failed for wpid @delete_wp', array(
      '@delete_wp' => $delete_wp,
    )), 'error');
    return;
  }

  // Load all this up just so that we can print a nice information message
  $node = node_load($delete_nid);
  $purchaser_account = user_load($user->uid);
  $giftee_account = user_load($node->uid);
  watchdog('content', 'The administrator forced a return of @purchase_quantity of "@node_title" from @purchaser_name for @giftee_name', array(
    '@purchaser_name' => $purchaser_account->name,
    '@purchase_quantity' => $wishlist_purch->wishlist_purch_quantity,
    '@node_title' => $node->title,
    '@giftee_name' => $giftee_account->name,
  ));

  db_delete('wishlist_purchased')
      ->condition('wishlist_purch_wid', $delete_wp)
      ->execute();

  // Set a successful return and then go back to the node display for this item.
  drupal_set_message(t('You have deleted the record for a purchase of @purchase_quantity unit for <i>@node_title</i>', array('@purchase_quantity' => $wishlist_purch->wishlist_purch_quantity, '@node_title' => $node->title)));
}

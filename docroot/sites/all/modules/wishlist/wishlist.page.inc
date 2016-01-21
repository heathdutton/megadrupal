<?php
// @file
// Wishlist management module for Drupal
// Written by Scott McLewin and Melanie Paul-McLewin
// drupal AT mclewin DOT com

/**
 * Menu callback function to handle the "wishlist" URL
 *
 * @return
 *   theme("page",...) ready to display the wishlist page result
 */
function wishlist_page() {
  $arg1 = arg(1);

  if (!isset($arg1)) {
    return _wishlist_show_all_lists();
  }
  elseif (is_numeric($arg1)) {
    return _wishlist_list_items($arg1);
  }
  else {
    switch ($arg1) {
      case "item":
        return _wishlist_item_action_handler(arg(2), arg(3), arg(4));
        break;
      default:
        return t('Invalid argument to wishlist_page [@arg1]', array('@arg1' => $arg1));
        break;
    }
  }
}


/**
 * Function to display every wishlist in the system
 *
 * @return
 *   A theme("page", ...) with display content showing all available wishlists.
 */
function _wishlist_show_all_lists() {
  $output = "";

  $query = db_select('node', 'n');
  $query->innerJoin('users', 'u', 'n.uid = u.uid');
  $query->fields('n', array('nid', 'title', 'uid'))
        ->fields('u', array('name'))
        ->condition('n.type', 'wishlist')
        ->groupBy('u.uid')
        ->orderBy('u.name', 'DESC');
  $result = $query->execute();
  
  
  $output .= "<div class='wishlist'><div class='all_lists'>";
  $output .= '<p>' . t("Wishlists allow their owners to maintain a running list of items they may want to purchase or have purchased for them on a birthday or special occasion.  The items on these lists are not sold through this web site, but point to other stores and vendors who do sell them.  This list is the on-line version of the birthday list you may have given to your mother growing up.  It was her job to make sure that no two people purchased the same gift for you.") . '</p>';
  $has_rows = FALSE;
  foreach ($result as $account) {
    $items[] = format_wishlists($account, t("'s wishlist"));
    $has_rows = TRUE;
  }
  if (FALSE == $has_rows) {
    return t("No wishlists exist on this site");
  }
  else {

    $output .= theme('item_list', array('items' => $items));
    $output .= "</div></div>";

    return $output;
  }
}

/**
 * Lists out the items for the user $uid
 *
 * @param $uid
 *   User for whom we should list out wishlist items.
 * @return
 *   $output for display
 */
function _wishlist_list_items($uid) {
  global $user;

  if (!is_numeric($uid)) {
    return "Invalid argument to wishlist_list_items [" . $uid . "]";
  }

  $showcolumns = variable_get('wishlist_showcolumn', _wishlist_get_default_showcolumn_settings_array());

  $header = array();
  if ($showcolumns['wishlist_show_action'] == TRUE) {
    $header[] = array('data' => t("Action"));
  }
  if ($showcolumns['wishlist_show_title'] == TRUE) {
    $header[] = array(
      'data' => t("Item Name"),
      "field" => "n.title",
    );
  }
  if ($showcolumns['wishlist_show_description'] == TRUE) {
    $header[] = array('data' => t("Description"));
  }
  if ($showcolumns['wishlist_show_priority'] == TRUE) {
    $header[] = array(
      'data' => t("Priority"),
      "field" => "w.item_priority",
      "sort" => "asc",
    );
  }
  if ($showcolumns['wishlist_show_cost'] == TRUE) {
    $header[] = array(
      'data' => t("Cost"),
      "field" => "w.item_est_cost",
    );
  }
  if ($showcolumns['wishlist_show_quantity'] == TRUE) {
    $header[] = array(
      'data' => t("Quantity"),
      "field" => "w.item_quantity_requested",
    );
  }
  if ($showcolumns['wishlist_show_urls'] == TRUE) {
    $header[] = array('data' => t("URLs"));
  }
  if ($showcolumns['wishlist_show_updated'] == TRUE) {
    $header[] = array(
      'data' => t("Last Updated"),
      "field" => "n.changed",
    );
  }

  if (empty($header)) {
    drupal_set_message('The site administrator has removed all columns from this display.  No wishlist information will appear.', 'error');
  }

  // Load up the user account information for the giftee so that we can include it in the display and in the breadcrumb
  $giftee_account = user_load($uid);

  // Build user name from tokens if possible, otherwise $giftee_account->name
  $giftee_name = _wishlist_user_name_from_tokens($giftee_account);
  
  $breadcrumb[] = l(t('Home'), NULL);
  $breadcrumb[] = l(t('All wishlists'),   'wishlist');
  $breadcrumb[] = l(t("@name's wishlist", array('@name' => check_plain($giftee_name))), 'wishlist/' . $giftee_account->uid);
  drupal_set_breadcrumb($breadcrumb);

  $itemsq = db_select('node', 'n')->extend('PagerDefault')->extend('TableSort');
  $itemsq->limit(variable_get('wishlist_display_count', 10));
  $itemsq->orderByHeader($header);
  $itemsq->leftJoin('wishlist', 'w', 'n.nid = w.nid');
  $itemsq->leftJoin('field_data_body', 'b', 'n.nid = b.entity_id');
  $itemsq->fields('n', array('nid', 'title', 'uid', 'changed', 'created'));
  $itemsq->fields('w', array('item_priority', 'item_est_cost', 'item_currency', 'item_quantity_requested', 'item_url1', 'item_url2', 'item_is_public'));
  $itemsq->fields('b', array('body_value', 'body_format'));
  $itemsq->condition('n.type', 'wishlist');
  $itemsq->condition('n.uid', $uid);
  $itemsq->condition('w.item_is_public', 1);
 
//TODO: Add the sorting bits of the query in....  
   
  $result = $itemsq->execute();

/*  
  $result = pager_query(db_rewrite_sql("SELECT n.nid, n.title, n.uid, w.item_priority, w.item_est_cost, w.item_currency, 
    r.body, n.changed, n.created, w.item_quantity_requested, w.item_url1, w.item_url2, w.item_is_public 
  FROM {node} n 
  INNER JOIN {node_revisions} r ON n.vid = r.vid 
  LEFT JOIN {wishlist} w ON n.nid = w.nid 
  WHERE n.type='wishlist' AND n.uid=" . $uid . " AND w.item_is_public " . tablesort_sql($header)), variable_get('wishlist_display_count', 10), 0);
*/
  
  $rows = _wishlist_fill_list_table_array($result);


  if (!$rows) {
    $rows[] = array(array(
        "data" => t("No wishlist items found."),
        "colspan" => "5",
      ));
  }

  // Add a pager control to the bottom of the table (if necessary)
/*
  $pager = theme("pager", array('tags' => NULL, 'element' => 0));
  if (!empty($pager)) {
    $rows[] = array(array(
        "data" => $pager,
        "colspan" => "8",
      ));
  }
*/
  // Build up the output buffer.
  $output = '<div class="wishlist">';
  $output .= t("<p>The list below shows all of the items on @giftee_name's wishlist.</p><p>Once you have decided on an item to purchase for @giftee_name, click on 'get this gift' and follow the instructions on the screen that comes up.  This will indicate that you have purchased the gift and will prevent somebody else from getting a duplicate.  Once you have clicked on 'get this gift' you still need to actually purchase the gift.  You don't do that here at this site.  You need to visit the store or website described by @giftee_name and complete your transaction there.  Please always 'get this gift' here on this website before you purchase it at the store.  Somebody else might decide to buy one too while you are out shopping.</p>", array('@giftee_name' => check_plain($giftee_name)));
  $output .= t("<p>If you change your mind about purchasing a particular gift you can put the item back on @giftee_name's wishlist.  View the item (by clicking on the 'Item Name' field) and you will be able to return the item to the list.  You need to be logged in to this web site to see the 'return' option.</p>", array('@giftee_name' => check_plain($giftee_name)));
  $output .= _wishlist_get_reveal_form($uid);
  $output .= theme('table', array('header' => $header, 'rows' => $rows)) . '</div>';

  // If the wishlist being shown belongs to the user who is currently logged in, then also
  // display a table of the private items below the public ones
  if ($uid == $user->uid) {
    $output .= "<div class='wishlist_private'>" . t("Items which are private and not shown to others who view your wishlist") . "</div>";

    
    
    //$result = pager_query(db_rewrite_sql("SELECT n.nid, n.title, n.uid, w.item_priority, w.item_est_cost, w.item_currency, r.body, n.changed, n.created, w.item_quantity_requested, w.item_url1, w.item_url2, w.item_is_public FROM {node} n INNER JOIN {node_revisions} r ON n.vid = r.vid LEFT JOIN {wishlist} w ON n.nid = w.nid WHERE n.type='wishlist' AND n.uid=" . $uid . " AND NOT w.item_is_public " . tablesort_sql($header)), variable_get('wishlist_display_count', 10), 1);

    $query = db_select('node', 'n')->extend('PagerDefault')->extend('TableSort');
    $query->limit(variable_get('wishlist_display_count', 10));
    $query->orderByHeader($header);
    $query->innerJoin('node_revision', 'r', 'n.vid = r.vid');
    $query->leftJoin('wishlist', 'w', 'n.nid = w.nid');
    $query->leftJoin('field_data_body', 'b', 'n.nid = b.entity_id');
    $query->fields('n', array('nid', 'title', 'uid', 'changed', 'created'));
    $query->fields('w', array('item_priority', 'item_est_cost', 'item_currency', 'item_quantity_requested', 'item_url1', 'item_url2', 'item_is_public'));
    $query->fields('b', array('body_value', 'body_format'));
    $query->condition('n.type', 'wishlist');
    $query->condition('n.uid', $uid);
    $query->condition('w.item_is_public', 0);
    $result = $query->execute();

    //TODO: See http://blog.worldempire.ch/story/howto-convert-module-dbtng for what happens here for conversion.
    $rows = _wishlist_fill_list_table_array($result);
    if (!$rows) {
      $rows[] = array(array(
          "data" => t("No private wishlist items found."),
          "colspan" => "5",
        ));
    }
/*
    // Add a pager control to the bottom of the table (if necessary)
    $pager = theme("pager", array('tags' => NULL, 'element' => 1));
    if (!empty($pager)) {
      $rows[] = array(array(
          "data" => $pager,
          "colspan" => "8",
        ));
    }
*/
    $output .= '<div class="wishlist">' . theme('table', array('header' => $header, 'rows' => $rows)) . '</div>';
  }

  return $output;
}





/**
 * Fills an associative array for use with a theme('table' call as the $rows argument.
 *
 * @param result
 *   Result of a db_query() for a series of node records of type wishlist
 *
 * @return
 *   An associative array ready for use with theme('table', ...) call as the $rows argument.
 */
function _wishlist_fill_list_table_array($result) {
  global $user;
  $rows = array();
  $priority_str = _wishlist_get_item_priority_array();

  $showcolumns = variable_get('wishlist_showcolumn', _wishlist_get_default_showcolumn_settings_array());


//  while ($node = db_fetch_object($result)) {
  while ($node = $result->fetchObject()) {
    $node->item_quantity_purchased = _wishlist_get_node_quantity_purchased($node->nid);


    if (drupal_strlen($node->body_value) > variable_get("wishlist_item_list_max_description", "200")) {
      $body = truncate_utf8($node->body_value, variable_get("wishlist_item_list_max_description", "200"));
      $body = check_markup($body, $node->body_format);
	  $body .= l(t('...(more)'), "node/$node->nid");
    }
    else {
      $body = $node->body_value;
      $body = check_markup($body, $node->body_format);
    }

    // If the module is configured to toss links open in new windows, then generate the appropriate attributes for the link
    if (variable_get("wishlist_url_in_new_window", FALSE)) {
      $url_link_target = "_wishlist_url_window";
    } else 
	  $url_link_target = '';


    // In links column display links to the purchase point if there are still items to purchase.
    // If all have been purchased, don't put the links in the list.  This is to cut down
    // on the folks who will visit, ignore the column about how many are left, click through
    // to the shop and then end up purchasing a duplicate.  We also hide the links when
    // the module is configured to hide purchase information from anonymous users.
    if ((($node->item_quantity_requested != 0) && ($node->item_quantity_purchased >= $node->item_quantity_requested))
               || _wishlist_hide_purchase_info($node)) {
      $links = _wishlist_url($node->item_url1, 20) . "<BR>" . _wishlist_url($node->item_url2, 20);
    }
    else {
      $links = l(_wishlist_url($node->item_url1, 20), $node->item_url1, array('attributes' => array('target' => $url_link_target))) . "<BR>" . l(_wishlist_url($node->item_url2, 20), $node->item_url2, array('attributes' => array('target' => $url_link_target)));
    }

    $rowdata = array();

    if ($showcolumns['wishlist_show_action'] == TRUE) {
      if (_wishlist_hide_purchase_info($node)) {
        if ($user->uid == $node->uid) {
          $rowdata[] = array('data' => t('Hidden from you'));
        }
        else {
          $rowdata[] = array('data' => l(t('Login for information on this gift'), "user/login"));
        }
      }
      else {
        $rowdata[] = array(
          "data" => (user_access('access wishlists') && $node->item_quantity_requested > $node->item_quantity_purchased) ?
               l(t('Get this gift'), "wishlist/item/$node->nid/purchase")
              : (($node->item_quantity_purchased >= $node->item_quantity_requested) ? t("(none left to purchase)")
               : l(t("Login to get this gift"), "user/login")),
        );
      }
    }

    $newtext = '';
    if ($user->uid) {
      $last_viewed = node_last_viewed($node->nid);
      if (($last_viewed == 0) && ($node->created > NODE_NEW_LIMIT)) {
        $newtext = '<span class="newitem">' . t('new') . '</span>';
      }
    }

    if ($showcolumns['wishlist_show_title'] == TRUE) {
      $rowdata[] = array("data" => l($node->title, "node/$node->nid") . $newtext);
    }
    if ($showcolumns['wishlist_show_description'] == TRUE) {
      $rowdata[] = array("data" => $body);
    }
    if ($showcolumns['wishlist_show_priority'] == TRUE) {
      $rowdata[] = array("data" => $priority_str[$node->item_priority]);
    }
    if ($showcolumns['wishlist_show_cost'] == TRUE) {
      $rowdata[] = array("data" => _wishlist_currency_str($node->item_currency) . $node->item_est_cost);
    }
    if ($showcolumns['wishlist_show_quantity'] == TRUE) {
      $rowdata[] = array("data" => $node->item_quantity_requested);
    }
    if ($showcolumns['wishlist_show_urls'] == TRUE) {
      $rowdata[] = array("data" => $links);
    }
    if ($showcolumns['wishlist_show_updated'] == TRUE) {
      $rowdata[] = array("data" => format_date($node->changed, 'short'));
    }

    $rows[] = $rowdata;
  }

  return $rows;
}

/**
 * Retrieves the default values array for the show-column settings array
 */
function _wishlist_get_default_showcolumn_settings_array() {
  $default_values['wishlist_show_action'] = TRUE;
  $default_values['wishlist_show_title'] = TRUE;
  $default_values['wishlist_show_description'] = TRUE;
  $default_values['wishlist_show_priority'] = TRUE;
  $default_values['wishlist_show_cost'] = TRUE;
  $default_values['wishlist_show_quantity'] = TRUE;
  $default_values['wishlist_show_urls'] = TRUE;
  $default_values['wishlist_show_updated'] = TRUE;
  return $default_values;
}


/**
 *
 * Handler function for actions take on an item - purchase and return
 * @param $nid
 *   ID of the node on which we are going to act
 * @param $action
 *   Action to be taken.  "purchase" or "return".
 * @param $wishlist_purch_id
 *   For the return action only, the ID of the wishlist_purchase record that is being returned.
 * @return
 *   output for display (via theme('page', ...))
 */
function _wishlist_item_action_handler($nid, $action, $wishlist_purch_id) {

  if (!is_numeric($nid)) {
    watchdog('wishlist', 'Invalid node argument to wishlist_item_action_handler[@nid]', array(
      '@nid' => $nid,
    ), WATCHDOG_ERROR);
    drupal_set_message(t('An error occurred while trying to carry out this action on a wishlist item.'), 'warning');
    drupal_goto();
  }

  switch ($action) {
    case "purchase":
      return _wishlist_item_action_purchase($nid);

    default:
      watchdog('wishlist', 'Invalid action argument to wishlist_item_action_handler[@action]', array(
        '@action' => $action,
      ), WATCHDOG_ERROR);
      drupal_set_message(t('An error occurred while trying to carry out this action on a wishlist item.'), 'warning');
      drupal_goto();
  }
}

/**
 * Builds the page to display when a user wants to purchase an item from a wishlist
 *
 * @param $nid
 *   ID of the node on which to act.  We assume the user that is currently logged in will be making
 *   the purchase.  Purchases by user 0 - anonymous - are not allowed.  Purchases off your own list are allowed.
 * @return
 *   $output for display.
 */
function _wishlist_item_action_purchase($nid) {
  global $user;

  if ($user->uid == 0) {
    watchdog('wishlist', 'Anonymous user attempted to purchase item off wishlist: nid=@nid.  You can prevent this error by removing wishlist permissions from anonymous accounts on your site', array(
      '@nid' => $nid,
    ), WATCHDOG_ERROR);
    return t('Anonymous wishlist purchases are not allowed');
  }

  $node = node_load($nid);
  if (empty($node->type) || $node->type != 'wishlist') {
    watchdog('wishlist', 'User @uid attempted to purchase a node which is not a wishlist item: nid=@nid.', array(
      '@uid' => $user->uid,
      '@nid' => $nid,
    ), WATCHDOG_ERROR);
    return t('Node @nid cannot be purchased', array('@nid' => $nid));
  }

  drupal_set_title(t('Purchase wishlist item - @title', array('@title' => $node->title)), PASS_THROUGH);

  $build = node_view($node, 'teaser');
  // Hide all links on this rendering.
  $build['links']['#access'] = FALSE;

  $output = drupal_render(drupal_get_form("wishlist_action_purchase_form", $node));
  $output .= '<div id="wishlist-purchase-node-view">'.drupal_render($build).'</div>';
  

  return $output;
}

/**
 * Form constructor for callback to return a wishlist purchase item.
 *
 * @param $nid
 *   The node ID for the item where some of the purchased quantity will be returned
 * @param $wpid
 *   The ID of the wishlist_purchased record to delete.
 *
 * @see _wishlist_item_action_return_submit()
 *
 * @ingroup forms
 */
function _wishlist_item_action_return($form, &$form_state, $nid, $wpid) {
  global $user;

  if ($user->uid == 0) {
    watchdog('wishlist', 'Anonymous user attempted to return an item from a wishlist: nid=@nid.  You can prevent this error by removing wishlist permissions from anonymous accounts on your site', array(
      '@nid' => $nid,
    ), WATCHDOG_ERROR);
    drupal_set_message(t('Anonymous wishlist returns are not allowed'), 'warning');
    drupal_goto();
  }

  if (!is_numeric($nid) || !is_numeric($wpid)) {
    watchdog('wishlist', 'Invalid attempt to return wishlist purchase item by user @uid - node @nid, wpid @wpid', array(
      '@uid' => $user->uid,
      '@nid' => $nid,
      '@wpid' => $wpid,
        ), WATCHDOG_ERROR);
    drupal_set_message(t('Invalid attempt to return wishlist purchase item.'), 'warning');
    drupal_goto();
  }

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $nid,
  );
  $form['wpid'] = array(
    '#type' => 'value',
    '#value' => $wpid,
  );

  $question = t('Are you sure you want to return this wishlist purchase item?');
  return confirm_form($form, $question, 'node/' . $nid, '', t('Return item'), t('Cancel'));

}

/**
 * Form submission handler for _wishlist_item_action_return().
 */
function _wishlist_item_action_return_submit($form, &$form_state) {
  global $user;

  $nid = $form_state['values']['nid'];
  $wpid = $form_state['values']['wpid'];

  $result = db_query('SELECT p.wishlist_purch_nid, p.wishlist_purch_wid, p.wishlist_purch_buyer_uid, p.wishlist_purch_quantity FROM {wishlist_purchased} p WHERE p.wishlist_purch_wid=:wpid', array(':wpid' => $wpid));

  if ($wishlist_purch = $result->fetchObject()) {
    if ($nid != $wishlist_purch->wishlist_purch_nid) {
      drupal_set_message(t('Inputs do not agree. Node @nid is not related to wpid=@wpid', array(
        '@nid' => $nid,
        '@wpid' => $wpid,
      )), 'warning');
      drupal_goto();
    }
    if ($user->uid != $wishlist_purch->wishlist_purch_buyer_uid) {
      drupal_set_message(t('Inputs do not agree. Node @nid wpid @wpid was not purchased by user @uid', array(
        '@nid' => $nid,
        '@wpid' => $wpid,
        '@uid' => $user->uid,
      )), 'warning');
      drupal_goto();
    }
  }
  else {
    drupal_set_message(t('Return of wishlist item failed'), 'warning');
    drupal_goto();
  }

  // Load all this up just so that we can print a nice information message
  $node = node_load($nid);
  if (empty($node->type) || $node->type != 'wishlist') {
    watchdog('wishlist', 'User @uid attempted to purchase a node which is not a wishlist item: nid=@nid.', array(
      '@uid' => $user->uid,
      '@nid' => $nid,
    ), WATCHDOG_ERROR);
  }
  $purchaser_account = user_load($user->uid);
  $giftee_account = user_load($node->uid);
  watchdog('content', '@purchaser_name returned @purchase_quantity of "@node_title" for @giftee_name', array('@purchaser_name' => $purchaser_account->name, '@purchase_quantity' => $wishlist_purch->wishlist_purch_quantity, '@node_title' => $node->title, '@giftee_name' => $giftee_account->name));

  db_delete('wishlist_purchased')
  ->condition('wishlist_purch_wid', $wpid)
  ->execute();

  // Set a successful return and then go back to the node display for this item.
  drupal_set_message(t('You have placed @purchase_quantity of <i>@node_title</i> back on the wishlist.', array('@purchase_quantity' => $wishlist_purch->wishlist_purch_quantity, '@node_title' => $node->title)));
  drupal_goto('node/' . $nid);
}

/**
 * Validates a request to purchase an item from a wishlist
 * @see http://drupal.org/node/1354
 */
function wishlist_action_purchase_form_validate($form, &$form_state) {
  $item_action_purchase_quantity = check_plain($form_state['values']['item_action_purchase_quantity']);

  if (!is_numeric($item_action_purchase_quantity)) {
    form_set_error('item_action_purchase_quantity', t('The purchase quantity must be numeric.'));
  }

  if ($item_action_purchase_quantity < 0) {
    form_set_error('item_action_purchase_quantity', t('Your purchase quantity must be a positive number, and not zero.'));
  }

  if ($item_action_purchase_quantity == 0) {
    form_set_error('item_action_purchase_quantity', t('Be serious.  Zero of them?'));
  }

  $node = node_load($form_state['values']['nid']);
  if (empty($node->type) || $node->type != 'wishlist') {
    form_set_error('nid', 'Node @nid is not a wishlist item: nid=@nid.', array(
      '@nid' => $form_state['values']['nid'],
    ));
  }
  $items_remaining = $node->item_quantity_requested -_wishlist_get_node_quantity_purchased($form_state['values']['nid']);
  if ($item_action_purchase_quantity > $items_remaining) {
    form_set_error('item_action_purchase_quantity', t('You cannot purchase more than @items_remaining of this item', array('@items_remaining' => $items_remaining)));
  }

}

/**
 * Submit handler for for the wishlist purchase form
 *
 */
function wishlist_action_purchase_form_submit($form, &$form_state) {
  global $user;

  $node = node_load($form_state['values']['nid']);
  $purchaser_account = user_load($user->uid);
  $giftee_account = user_load($node->uid);
  $item_action_purchase_quantity = check_plain($form_state['values']['item_action_purchase_quantity']);
  
  // Create the node that tracks what was purchased.
  $id = db_insert('wishlist_purchased')
  ->fields(array(
    'wishlist_purch_nid' => $node->nid,
    'wishlist_purch_buyer_uid' => $purchaser_account->uid,
    'wishlist_purch_quantity' => $item_action_purchase_quantity,
    'purch_date' => REQUEST_TIME,
  ))
  ->execute();

  // Set a successful return and then go back to the node display for this item.
  watchdog('content', '@purchaser_name purchased @purchase_quantity of "@node_title" for @giftee_name', array('@purchaser_name' => $purchaser_account->name, '@purchase_quantity' => $item_action_purchase_quantity, '@node_title' => $node->title, '@giftee_name' => $giftee_account->name));
  drupal_set_message(t('Your intent to buy @item_purchase_quantity of this item has been recorded.  Next you should buy the described item at the store/website that is selling it.', array('@item_purchase_quantity' => $item_action_purchase_quantity)));
  $form_state['redirect'] = 'node/' . $node->nid;
}


/*
 * Returns the form array for the form we display when the user
 * wants to purchase an item.
 *
 * @param $node  The node being acted on
 */
/**
 * Form to complete the purchase of an item off the wishlist
 * @see http://drupal.org/node/1354
 */
function wishlist_action_purchase_form($form, &$form_state, $node) {
  $giftee_account = user_load($node->uid);

  $form["item_action_purchase_quantity"] = array(
    '#type' => 'textfield',
    '#title' => t("Quantity that you will purchase"),
    '#default_value' => $node->item_quantity_requested -$node->item_quantity_purchased,
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t("Enter the quantity that you intend to purchase for @giftee_name.  You should 'purchase' items off the wishlist here before you actually buy the items from the store/website.  If you do not, you create the chance for somebody else to get these items instead.  Remember, you can always put an item you 'purchase' back on the wishlist.", array('@giftee_name' => $giftee_account->name)),
  );

  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );

  $form[] = array(
    '#type' => 'submit',
    '#value' => t('Purchase'),
  );

  return $form;
}


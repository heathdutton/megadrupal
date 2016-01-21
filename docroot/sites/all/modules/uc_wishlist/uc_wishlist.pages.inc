<?php

/**
 * @file
 * Page callback and functions for wish lists.
 */

/**
 * Display a wish list for viewing or altering.
 *
 * @param numeric $wid
 *   The ID of the wish list to display.
 *
 * @return html
 *   HTML output for the wish list display and form.
 */
function uc_wishlist_display($wid = NULL, $mode = 'wid') {
  global $user;
  $output = '';
  $own = FALSE;

  if (empty($wid)) {
    // Default to the current user's wish list if no wishlist ID is specified.
    $wid = uc_wishlist_get_wid();
    $own = TRUE;
  }
  elseif (is_object($wid)) {
    // $wid is actually a user account.
    $wid = uc_wishlist_get_wid($wid->uid);
  }

  if (!$own && $wid == uc_wishlist_get_wid()) {
    $own = TRUE;
  }

  // Attempt to load the wish list.
  $wishlist = uc_wishlist_load($wid);

  // Handle a non-existent wish list.
  if (!$wishlist) {
    // If the requested list was for the current user...
    if ($own) {
      // Display a message letting them know their list is empty.
      drupal_set_title(t('Wish list'));
      drupal_set_message(t("You have not added any products to your wish list. You can add any product from the store to your wish list by clicking the 'Add to wish list' button on the product's page."));

      return t('There are no products on this wish list.');
    }
    else {
      // Otherwise send them to the search form.
      drupal_set_message(t('The wish list you requested could not be found.  Perhaps you can try looking for it through the wish list search form below.'));
      drupal_goto('wishlist/search');
    }
  }
  // Display only if it's my own or it's not private.
  if (!$wishlist->private || $own) {
    drupal_set_title($wishlist->title);

    // Add the settings form if the user is viewing his own wish list.
    if ($own) {
      if (!$user->uid) {
        drupal_set_message(filter_xss(t('You must <a href="!login_url">login</a> or <a href="!register_url">register</a> to save your wish list.', array('!login_url' => url('user/login'), '!register_url' => url('user/register')))));
      }

      $collapsed = TRUE;
    }

    // Add expiration information to the display.
    if ($wishlist->expiration < REQUEST_TIME) {
      $output .= '<p>' . t('This wish list may no longer be valid. It was for an event on @date.', array('@date' => format_date($wishlist->expiration))) . '</p>';
    }
    elseif ($wishlist->expiration > 0) {
      $output .= '<p>' . t('This wish list is valid until @date.', array('@date' => format_date($wishlist->expiration))) . '</p>';
    }

    $items = uc_wishlist_get_contents($wid);

    if (empty($items)) {
      return '<p>' . t('There are no products on this wish list.') . '</p>';
    }

    $form = drupal_get_form('uc_wishlist_view_form', $items, $wid, $own);
    $output .= drupal_render($form);
  }
  else {
    drupal_goto('wishlist/search');
  }
  return $output;
}

/**
 * Display a wishlist in user account page.
 *
 * We will display wish list address settings and wishlist expiry settings.
 */
function uc_wishlist_user_display($wid = NULL, $mode = 'wid') {
  global $user;
  $output = '';

  // $own defines whether the wish list is owned by the current user or not.
  // This affects how many parts of the page are rendered.
  $own = FALSE;

  if ($mode == 'user') {
    // $wid is actually a user account.
    $wid = uc_wishlist_get_wid($wid->uid);
  }
  elseif (empty($wid)) {
    // Default to the current user's wish list if no wishlist ID is specified.
    $wid = uc_wishlist_get_wid();
    $own = TRUE;
  }

  if (!$own && $wid == uc_wishlist_get_wid()) {
    $own = TRUE;
  }

  // Attempt to load the wish list.
  $wishlist = uc_wishlist_load($wid);

  // Handle a non-existent wish list.
  if (!$wishlist) {
    // If the requested list was for the current user...
    if ($own) {
      // Display a message letting them know their list is empty.
      drupal_set_title(t('Wish list'));
      drupal_set_message(t("You have not added any products to your wish list. You can add any product from the store to your wish list by clicking the 'Add to wish list' button on the product's page."));

      return t('There are no products on this wish list.');
    }
    else {
      // Otherwise send them to the search form.
      drupal_set_message(t('The wish list you requested could not be found.  Perhaps you can try looking for it through the wish list search form below.'));
      drupal_goto('wishlist/search');
    }
  }

  drupal_set_title($wishlist->title);

  // Add the settings form if the user is viewing his own wish list.
  if ($own) {
    if (!$user->uid) {
      drupal_set_message(filter_xss(t('You must <a href="!login_url">login</a> or <a href="!register_url">register</a> to save your wish list.', array('!login_url' => url('user/login'), '!register_url' => url('user/register')))));
    }

    $collapsed = TRUE;

    if (variable_get('uc_wishlist_save_address', TRUE) && (empty($wishlist->address->firstname) || empty($wishlist->address->lastname) || empty($wishlist->address->addr1) || empty($wishlist->address->city) || empty($wishlist->address->postcode))) {
      $collapsed = FALSE;
    }

    $settings_form = drupal_get_form('uc_wishlist_settings_form', $wishlist, $collapsed);
  }

  // Display only if it's my own or it's not private.
  if (!$wishlist->private || $own) {
    $expired = $wishlist->expiration < REQUEST_TIME;
    $expiration_date = format_date($wishlist->expiration, 'uc_store');
    // Add expiration information to the display.
    if ($expired) {
      $message = t('This wish list may no longer be valid. It was for an event on @date.', array('@date' => $expiration_date));
    }
    elseif ($wishlist->expiration > 0) {
      $message = t('This wish list is valid until @date.', array('@date' => $expiration_date));
    }

    $items = uc_wishlist_get_contents($wid);

    if (empty($items)) {
      return '<p>' . t('There are no products on this wish list.') . '</p>';
    }

    $form = drupal_get_form('uc_wishlist_view_form', $items, $wid, $own);
    $output = theme('uc_wishlist_user_display', array(
      'settings_form' => $settings_form,
      'expired' => $expired,
      'expiration_date' => $expiration_date,
      'message' => $message,
      'wishlist' => $form,
      'email_link' => url('user/' . $user->uid . '/wishlist-email'),
      )
    );
  }
  return $output;
}

/**
 * Builds the user wish list settings form.
 */
function uc_wishlist_settings_form($form, $form_state, $wishlist, $collapsed = FALSE) {
  global $user;

  drupal_add_js(drupal_get_path('module', 'uc_wishlist') . '/uc_wishlist.js');

  $expiration = getdate($wishlist->expiration);
  $expiration['day'] = $expiration['mday'];
  $expiration['month'] = $expiration['mon'];

  $form = array();

  $form['wishlist'] = array(
    '#type' => 'fieldset',
    '#title' => t('Wish list settings'),
    '#collapsible' => TRUE,
    '#collapsed' => $collapsed,
  );
  $form['wishlist']['wid'] = array(
    '#type' => 'hidden',
    '#value' => $wishlist->wid,
  );
  $form['wishlist']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $wishlist->title,
    '#required' => TRUE,
  );
  $form['wishlist']['expiration'] = array(
    '#type' => 'date',
    '#title' => t('Event or expiration date'),
    '#default_value' => $expiration,
    '#description' => t('If this wish list is associated with an event or will no longer be relevant on a specific date, enter it here.'),
  );

  if (!variable_get('uc_wishlist_default_private', TRUE) && variable_get('uc_wishlist_allow_private', TRUE)) {
    $form['wishlist']['private'] = array(
      '#type' => 'checkbox',
      '#title' => t('Private'),
      '#default_value' => $wishlist->private,
      '#description' => t('Check this to make your wish list private and exclude it from wish list search results.'),
    );
  }

  if (variable_get('uc_wishlist_save_address', TRUE)) {
    $form['wishlist']['address'] = array(
      '#type' => 'fieldset',
      '#title' => t('Mailing address'),
      '#description' => t('The address you enter here will be available as a shipping address to anyone who purchases an item from your wish list.'),
    );

    if ($user->uid) {
      $addresses = uc_select_address($user->uid, 'delivery', 'apply_address(\'delivery\', this.value);', t('Saved addresses'), TRUE);
      if (!empty($addresses)) {
        $form['wishlist']['address']['delivery_address_select'] = $addresses;
        unset($form['wishlist']['address']['delivery_address_select']['#suffix']);
      }
    }

    if (uc_address_field_enabled('first_name')) {
      $form['wishlist']['address']['delivery_first_name'] = uc_textfield(uc_get_field_name('first_name'), empty($wishlist->address->firstname) ? NULL : $wishlist->address->firstname, uc_address_field_required('first_name'));
    }
    if (uc_address_field_enabled('last_name')) {
      $form['wishlist']['address']['delivery_last_name'] = uc_textfield(uc_get_field_name('last_name'), empty($wishlist->address->lastname) ? NULL : $wishlist->address->lastname, uc_address_field_required('last_name'));
    }
    if (uc_address_field_enabled('company')) {
      $form['wishlist']['address']['delivery_company'] = uc_textfield(uc_get_field_name('company'), empty($wishlist->address->company) ? NULL : $wishlist->address->company, uc_address_field_required('company'), NULL, 64);
    }
    if (uc_address_field_enabled('street1')) {
      $form['wishlist']['address']['delivery_street1'] = uc_textfield(uc_get_field_name('street1'), empty($wishlist->address->addr1) ? NULL : $wishlist->address->addr1, uc_address_field_required('street1'), NULL, 64);
    }
    if (uc_address_field_enabled('street2')) {
      $form['wishlist']['address']['delivery_street2'] = uc_textfield(uc_get_field_name('street2'), empty($wishlist->address->addr2) ? NULL : $wishlist->address->addr2, uc_address_field_required('street2'), NULL, 64);
    }
    if (uc_address_field_enabled('city')) {
      $form['wishlist']['address']['delivery_city'] = uc_textfield(uc_get_field_name('city'), empty($wishlist->address->city) ? NULL : $wishlist->address->city, uc_address_field_required('city'));
    }
    if (uc_address_field_enabled('country')) {
      $form['wishlist']['address']['delivery_country'] = array(
        '#type' => 'select',
        '#title' => uc_get_field_name('country'),
        '#description' => NULL,
        '#required' => uc_address_field_required('country'),
        '#options' => uc_country_option_list(),
        '#default_value' => isset($wishlist->address->country) ? $wishlist->address->country : uc_store_default_country(),
      );
    }
    if (uc_address_field_enabled('zone')) {
      if (isset($_POST['delivery_country'])) {
        $country_id = intval(check_plain($_POST['delivery_country']));
      }
      else {
        $country_id = isset($wishlist->address->country) ? $wishlist->address->country : uc_store_default_country();
      }
      $form['wishlist']['address']['delivery_zone'] = uc_zone_select(uc_get_field_name('zone'), empty($wishlist->address->zone) ? NULL : $wishlist->address->zone, $country_id, array('required' => uc_address_field_required('zone')));
      if (isset($_POST['panes']) && count($form['wishlist']['address']['delivery_zone']['#options']) == 1) {
        $form['wishlist']['address']['delivery_zone']['#required'] = FALSE;
      }
    }
    if (uc_address_field_enabled('postal_code')) {
      $form['wishlist']['address']['delivery_postal_code'] = uc_textfield(uc_get_field_name('postal_code'), empty($wishlist->address->postcode) ? NULL : $wishlist->address->postcode, uc_address_field_required('postal_code'), NULL, 10, 10);
    }
    if (uc_address_field_enabled('phone')) {
      $form['wishlist']['address']['delivery_phone'] = uc_textfield(uc_get_field_name('phone'), empty($wishlist->address->phone) ? NULL : $wishlist->address->phone, uc_address_field_required('phone'), NULL, 32, 16);
    }
  }

  $form['wishlist']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

/**
 * Validation handler for wish list settings form.
 */
function uc_wishlist_settings_form_validate($form, &$form_state) {
  global $user;

  $wid = $form_state['values']['wid'];

  $wishlist = uc_wishlist_load($wid);
  if (!$wishlist) {
    drupal_set_message(t('Could not find the specified wish list.'), 'error');
    return FALSE;
  }
  if ($wishlist->uid != $user->uid && !user_perm('administer store')) {
    drupal_set_message(t('You do not have permission to edit this wish list.'), 'error');
    return FALSE;
  }
}

/**
 * Submission handler for wish list settings form.
 */
function uc_wishlist_settings_form_submit($form, &$form_state) {
  $expiration = mktime(0, 0, 0, $form_state['values']['expiration']['month'], $form_state['values']['expiration']['day'], $form_state['values']['expiration']['year']);

  if (variable_get('uc_wishlist_save_address', TRUE)) {
    $address = array(
      'firstname' => $form_state['values']['delivery_first_name'],
      'lastname' => $form_state['values']['delivery_last_name'],
      'company' => empty($form_state['values']['delivery_company']) ? '' : $form_state['values']['delivery_company'],
      'addr1' => $form_state['values']['delivery_street1'],
      'addr2' => empty($form_state['values']['delivery_street2']) ? '' : $form_state['values']['delivery_street2'],
      'city' => $form_state['values']['delivery_city'],
      'country' => $form_state['values']['delivery_country'],
      'zone' => $form_state['values']['delivery_zone'],
      'postcode' => $form_state['values']['delivery_postal_code'],
      'phone' => empty($form_state['values']['delivery_phone']) ? '' : $form_state['values']['delivery_phone'],
    );
  }
  else {
    $address = NULL;
  }
  $private = variable_get('uc_wishlist_default_private', FALSE) ? variable_get('uc_wishlist_default_private', FALSE) : 0;
  $private = variable_get('uc_wishlist_allow_private', FALSE) ? $form_state['values']['private'] : $private;
  uc_wishlist_update_wishlist($form_state['values']['wid'], $form_state['values']['title'], $expiration, (object) $address, $private);
  drupal_set_message(t('Your wish list has been updated.'));
}

/**
 * Display a page allowing the customer to view his/her wish list.
 */
function uc_wishlist_view_form($form, $form_state, $items, $wid, $own) {
  $form = array();

  $form['items'] = array(
    '#tree' => TRUE,
  );

  // Load each wish list product and add it to the form array.
  foreach ($items as $item) {

    $node = node_load($item->nid);
    $element = array();

    $element['nid'] = array(
      '#type' => 'value',
      '#value' => $node->nid,
    );
    $element['wpid'] = array(
      '#type' => 'value',
      '#value' => $item->wpid,
    );
    $element['module'] = array(
      '#type' => 'value',
      '#value' => 'uc_product',
    );

    if ($own) {
      $element['remove'] = array('#type' => 'checkbox');
    }

    $item->haveqty = 0;
    if (is_array($item->purchase)) {
      $item->haveqty = count($item->purchase);
    }

    $element['title'] = array(
      '#type' => 'item',
      '#markup' => l(filter_xss($node->title, array()), 'node/' . $node->nid),
    );

    $description = uc_product_get_description($item);
    if ($description) {
      $element['description'] = array(
        '#type' => 'item',
        '#markup' => $description,
      );
    }

    $element['#total'] = $item->price * $item->qty;

    $element['data'] = array(
      '#type' => 'hidden',
      '#value' => serialize($item->data),
    );

    $element['wantqty'] = array(
      '#type' => 'textfield',
      '#default_value' => $item->qty,
      '#size' => 5,
      '#maxlength' => 6,
      '#disabled' => $own ? FALSE : TRUE,
    );
    if (!$own) {
      // Disabled elements do not pass their default value.
      $element['wantqty']['#value'] = $item->qty;
    }
    $element['haveqty'] = array(
      '#type' => 'textfield',
      '#default_value' => $item->haveqty,
      '#size' => 5,
      '#maxlength' => 6,
      '#disabled' => TRUE,
    );

    $element['qty'] = array(
      '#type' => 'textfield',
      '#default_value' => $item->qty - $item->haveqty > 0 ? $item->qty - $item->haveqty : 1,
      '#size' => 5,
      '#maxlength' => 6,
    );

    // Checking if uc_stock module is install in the site and
    // prevent user to add product into cart if the stock value of the product
    // is equal to 0.

    // Checking if uc_stock module install in the site.
    if (module_exists('uc_stock')) {

      // If product kit module is installed in the site and wishlist node type
      // is product kit.
      if (module_exists('uc_product_kit') && $node->type == 'product_kit') {

        // Getting the number of products attached with the Product Kit.
        // As there is no stock configuration, so we will check the stock value
        // of the each product of Product Kit.

        // If all products of the Product Kit has stock active, then we allow
        // user to purchase product kit.
        // @var unknown_type .

        $products = $node->products;

        $stock = TRUE;

        // Looping through each product.
        foreach ($products as $product) {

          // Checking stock level of the product.
          if (!uc_stock_level($product->model)) {
            $stock = FALSE;
          }
        }
      }
      else {
        // Getting stock value of the particular product SKU.
        // It will return FALSE, if stock level is not active to product SKU.
        $stock = uc_stock_level($node->model);
      }

      if ($stock) {

        $element['addcart'] = array(
          '#type' => 'submit',
          '#name' => 'addcart-' . $item->wpid,
          '#value' => t('Add to cart'),
        );

      }
      else {
        $element['addcart'] = array(
          '#type' => 'item',
          '#name' => 'addcart-' . $item->wpid,
          '#markup' => t('Out Of Stock'),
        );
      }
    }
    else {
      $element['addcart'] = array(
        '#type' => 'submit',
        '#name' => 'addcart-' . $item->wpid,
        '#value' => t('Add to cart'),
      );
    }

    $form['items'][] = $element;
  }

  $form['wid'] = array(
    '#type' => 'hidden',
    '#value' => $wid,
  );

  if ($own) {
    $form['own'] = array(
      '#type' => 'value',
      '#value' => TRUE,
    );
    $form['update'] = array(
      '#type' => 'submit',
      '#value' => t('Update wish list'),
    );
  }

  return $form;
}

/**
 * Theme function for wish list display.
 */
function theme_uc_wishlist_view_form($variables) {
  $form = $variables['form'];
  $rows = array();

  $own = isset($form['own']) ? TRUE : FALSE;

  $header = array(t('Products'), '', t('Wanted'), t('Have'), t('Qty.'), t('Purchase'),
    array('data' => t('Total'), 'align' => 'right'));

  if ($own) {
    array_unshift($header, t('Remove'));
  }

  foreach (element_children($form['items']) as $i) {
    $nid = $form['items'][$i]['nid']['#value'];
    $desc = drupal_render($form['items'][$i]['title']) . '<br />';
    $desc .= drupal_render($form['items'][$i]['description']);
    $img = uc_product_get_picture($nid, 'uc_cart');

    $row = array(
      empty($img) ? '' : drupal_render($img),
      $desc,
      $form['items'][$i]['wantqty'] ? drupal_render($form['items'][$i]['wantqty']) : '',
      $form['items'][$i]['haveqty'] ? drupal_render($form['items'][$i]['haveqty']) : '',
      $form['items'][$i]['qty'] ? drupal_render($form['items'][$i]['qty']) : '',
      $form['items'][$i]['addcart'] ? drupal_render($form['items'][$i]['addcart']) : '',
      array(
        'data' => uc_currency_format($form['items'][$i]['#total']),
        'nowrap' => 'nowrap',
      ),
    );

    if (isset($form['items'][$i]['remove'])) {
      array_unshift($row, drupal_render($form['items'][$i]['remove']));
    }

    $rows[] = array(
      'data' => $row,
      'valign' => 'top',
    );
  }

  $output = '<div id="wishlist-form-products">' . theme('table', array('header' => $header, 'rows' => $rows)) . '</div>';

  return $output . drupal_render_children($form);
}

/**
 * Validation handler for wish list view form.
 */
function uc_wishlist_view_form_validate($form, &$form_state) {
  // Find wish list item associated with 'add to cart' submitted.
  foreach ($form_state['values'] as $key => $val) {
    if (strpos($key, 'addcart-') === 0) {
      $wpid = intval(substr($key, 8));
    }
  }

  // If adding item to cart, check quantities.
  if (!empty($wpid)) {
    foreach ($form_state['values']['items'] as $key => $item) {
      if ($item['wpid'] == $wpid && $item['qty'] == 0) {
        $item['qty'] = 1;
      }
      if ($item['qty'] && ($item['qty'] + $item['haveqty']) > $item['wantqty']) {
        form_set_error($key, t('You have selected to purchase more items than requested.'));
      }
    }
  }
}

/**
 * Submission handler for wish list view form.
 */
function uc_wishlist_view_form_submit($form, &$form_state) {

  global $user;

  if (isset($form_state['values']['op']) && !empty($form_state['values']['op'])) {
    if ($form_state['values']['op'] == $form_state['values']['update']) {

      uc_wishlist_update_item_object((object) $form_state['values']);

      drupal_set_message(t('Your wish list has been updated.'));

      $form_state['redirect'] = 'user/' . $user->uid . '/wishlist';
    }
  }
  else {
    // Find wish list item associated with 'add to cart' submitted.
    foreach ($form_state['values'] as $key => $val) {
      if (strpos($key, 'addcart-') === 0) {
        $wpid = intval(substr($key, 8));
      }
    }

    if ($wpid) {
      foreach ($form_state['values']['items'] as $key => $item) {
        if ($item['wpid'] == $wpid) {
          if ($item['qty'] == 0) {
            $form_state['values']['items'][$key]['qty'] = 1;
          }
        }
        else {
          $form_state['values']['items'][$key]['qty'] = 0;
        }
      }

      uc_wishlist_add_items_to_cart((object) $form_state['values']);
      drupal_set_message(t('The item(s) have been added to your shopping cart.'));

      $form_state['redirect'] = 'cart';
    }
  }
}

/**
 * Display the wish list search form.
 */
function uc_wishlist_search_form($form, $form_state, $keywords = '') {
  global $user;
  $form = array();

  // Generate link to 'create or manage your wish list'.
  $path = 'wishlist';
  $query = NULL;

  if (!$user->uid && !user_access('create wish lists')) {
    $path = 'user';
    $query = array('destination' => 'wishlist');
  }

  $form['wishlist_link'] = array(
    '#type' => 'item',
    '#markup' => '<div>' . l(t('Create or manage your wish list.'), $path, array('query' => $query)) . '</div>',
  );

  $form['search'] = array(
    '#type' => 'fieldset',
  );
  $form['search']['keywords'] = array(
    '#type' => 'textfield',
    '#title' => t('Search keywords'),
    '#description' => t('Enter the keywords to use to search wish list titles and addresses.'),
    '#default_value' => $keywords,
  );
  $form['search']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  if (!empty($keywords)) {
    // Check for user, wish list title, or address matches.
    $query = db_select('uc_wishlists', 'w');
    $query->join('users', 'u', 'w.uid = u.uid');
    $query->fields('w', array(
      'wid',
      'title',
    ));
    $query->distinct();
    $query->condition(db_or()
    ->condition('u.name', '%' . $keywords . '%', 'LIKE')
    ->condition('w.title', '%' . $keywords . '%', 'LIKE')
    ->condition('w.address', '%' . $keywords . '%', 'LIKE'));
  }
  else {
    $query = db_select('uc_wishlists', 'w');
    $query->fields('w', array(
      'wid',
      'title',
    ));
  }
  $query->condition('w.private', 0, '=');
  $result = $query->orderBy('w.title')->extend('PagerDefault')->limit(25)->execute();

  $links = array();

  foreach ($result as $wishlist) {
    $links[] = array(
      'title' => filter_xss($wishlist->title, array()),
      'href' => 'wishlist/' . $wishlist->wid,
    );
  }

  if (!empty($links)) {
    $output = theme_links(array(
      'links' => $links,
      'attributes' => array(
        'class' => array('wishlist'),
      ),
      'heading' => NULL,
    ));
  }
  else {
    $output = ' ' . t('No wish lists found.');
  }

  $form['output'] = array(
    '#type' => 'item',
    '#markup' => filter_xss('<div><h2>' . t('Wish lists:') . '</h2>' . $output . '</div>'),
  );

  return $form;
}

/**
 * Submission handler for wish list search form.
 */
function uc_wishlist_search_form_submit($form, &$form_state) {
  if (empty($form_state['values']['keywords'])) {
    $form_state['redirect'] = 'wishlist/search';
  }
  else {
    $form_state['redirect'] = 'wishlist/search/' . drupal_encode_path($form_state['values']['keywords']);
  }
}

/**
 * Email form for wishlist.
 */
function uc_wishlist_email_form($node, &$form_state, $account) {

  global $user;
  // TODO: Handle multiple wishlists?
  $wid = uc_wishlist_get_wid($account->uid);

  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#required' => TRUE,
    '#description' => t('Enter Email Subject.'),
  );
  $form['recipients'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipients'),
    '#required' => TRUE,
    '#description' => t('Email address of the reciepient. Seperate recipients using a comma.'),
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#rows' => 6,
    '#required' => TRUE,
    '#description' => t('Enter Email Message.'),
  );
  $form['wid'] = array(
    '#type' => 'value',
    '#value' => $wid,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
  );

  return $form;
}

/**
 * Validate function for the wishlist Email form use to send email.
 *
 * @param array $form
 *   Form array.
 *
 * @param array $form_state
 *   Formstate array contains the user submitted values.
 *
 * @return none
 *   Returns nothing.
 */
function uc_wishlist_email_form_validate($form, &$form_state) {

  // Exploding the emails id of the recpients.
  $emails = explode(',', $form_state['values']['recipients']);
  $emails = array_filter($emails);
  foreach ($emails as $email) {
    $email = trim($email);
    if ($email != '' && !valid_email_address($email)) {
      // Generating error.
      form_set_error('emails', t('%email is not a valid email address', array('%email' => $email)));
    }
  }
}

/**
 * Submit callback for the wishlist Email form use to send email.
 *
 * @param array $form
 *   Form array.
 *
 * @param array $form_state
 *   Formstate array contains the user submitted values.
 *
 * @return none
 *   Returns nothing.
 */
function uc_wishlist_email_form_submit($form, &$form_state) {

  // Exploding the emails id of the recpients.
  $emails = explode(',', $form_state['values']['recipients']);

  $emails = array_filter($emails);
  // Getting subject of the mail and its sanitization.
  $subject = check_plain($form_state['values']['subject']);
  // Getting message of the mail and text sanitization.
  $message = check_markup($form_state['values']['message']);
  $wid = $form_state['values']['wid'];
  $message = $message . "\n" . l(t('Wishlist'), 'wishlist/' . $wid);

  foreach ($emails as $email) {
    uc_wishlist_send_mail($email, $subject, $message);
  }
}

/**
 * Wishlist: Sending mail function.
 *
 * @param mail $to
 *   Reciever email address.
 *
 * @param string $subject
 *   Subject of the mail.
 *
 * @param string $message
 *   Body of the mail.
 *
 * @return none
 *   Returns nothing.
 */
function uc_wishlist_send_mail($to, $subject, $message) {

  $from = variable_get('uc_wishlist_default_from', '');
  $params = array(
    'subject' => $subject,
    'body' => $message,
    'from' => $from,
  );
  drupal_mail('uc_wishlist', 'wishlist', $to, language_default(), $params, $from, TRUE);
}

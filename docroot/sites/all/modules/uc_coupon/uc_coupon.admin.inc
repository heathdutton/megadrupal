<?php
/**
 * @file
 * Discount Coupons administration pages.
 */

/**
 * Store-wide coupon settings form.
 */
function uc_coupon_settings_form($form, &$form_state) {
  global $base_url;

  $form['uc_coupon_line_item_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Default line item title format'),
    '#description' => t('Controls the way coupons appear in the cart and order summary. This setting be overridden by individual coupons.'),
    '#default_value' => variable_get('uc_coupon_line_item_format', t('Coupon !code', array('!code' => '[uc_coupon:code]'))),
  );

  if (module_exists('token')) {
    $form['uc_coupon_line_item_format']['#description'] .= ' ' . t('You may use any of the following replacement patterns.');
    $form['uc_coupon_line_item_format']['#suffix'] = theme('token_tree', array('token_types' => array('uc_coupon')));
  }

  $form['uc_coupon_show_in_cart'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show coupons as an item in the cart.'),
    '#default_value' => variable_get('uc_coupon_show_in_cart', TRUE),
  );

  $options = array();
  for ($i = -10; $i <= 10; $i++) {
    $options[$i] = $i;
  }
  $form['uc_li_coupon_weight'] = array(
    '#type' => 'select',
    '#title' => t('Line item weight'),
    '#description' => t('Controls the placement of the coupon line item in the order summary. Heavier items appear lower in the list.'),
    '#options' => $options,
    '#default_value' => variable_get('uc_li_coupon_weight', 0),
  );

  $form['uc_coupon_form_components'] = array(
    '#title' => t('Coupon submission form components'),
    '#description' => t('Select what to display on the coupon submission form (this appears in the coupon block, cart and checkout panes).
    Note that you must enable the coupon code entry field in order for customers to submit coupons.  Also, if coupons are not displayed
    in the cart (see above),  you must enable either the active coupons table or the active coupons list in order for custmers to be able
    to remove coupons.'),
    '#type' => 'checkboxes',
    '#options' => array(
      'entry' => t('Coupon code entry field'),
      'table' => t('Active coupons (table)'),
      'list' => t('Active coupons (list)'),
    ),
    '#default_value' => variable_get('uc_coupon_form_components',
      drupal_map_assoc(variable_get('uc_coupon_allow_multiple', FALSE) ? array('entry') : array('entry', 'list'))),
  );

  $form['uc_coupon_allow_multiple'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow multiple coupons to apply to a single order.'),
    '#default_value' => variable_get('uc_coupon_allow_multiple', FALSE),
    '#description' => t('Note, you must enable this feature if you have configured multiple automatic coupons,
    		even if the restrictions for those coupons would prevent them ever applying to the same order.'),
  );

  $form['uc_coupon_default_usage'] = array(
    '#type' => 'radios',
    '#title' => t('Default redemption setting for new coupons'),
    '#options' => array(
      'single' => t('Single use per code.'),
      'multi' => t('Multiple uses per code.'),
    ),
    '#default_value' => variable_get('uc_coupon_default_usage', 'single'),
  );

  $form['uc_coupon_used_order_status'] = array(
    '#type' => 'select',
    '#title' => t('Order status to mark a coupon used'),
    '#options' => uc_order_status_options_list(),
    '#default_value' => variable_get('uc_coupon_used_order_status', 'processing'),
    '#description' => t('A coupon will be considered used when it has been applied to an order that reaches this status 
       (or a later one).  If the order status subsequenty reverts to an earlier status, the coupon use will also be reverted.'),
  );

  $form['uc_coupon_querystring'] = array(
    '#type' => 'textfield',
    '#title' => t('Query string parameter'),
    '#description' => t('If set, coupon codes can be applied by including them in any URL. Leave blank to disable this feature.<br />Example: if set to "coupon", visiting !base_url?coupon=CODE will apply the CODE coupon.', array('!base_url' => $base_url)),
    '#default_value' => variable_get('uc_coupon_querystring', ''),
    '#size' => 15,
  );

  $form['uc_coupon_expiry'] = array(
    '#title' => t('Coupon activation/expiration time'),
    '#type' => 'item',
    '#description' => t('Date-restricted coupons will activate and expire at this hour on the dates specified for the
    	individual coupon. Note that changing this setting will not effect coupons which have already been created.  To
    	update the activation/expiration times of existing coupons, edit and save them.'),
  );

  $form['uc_coupon_expiry']['uc_coupon_expiry_hour'] = array(
    '#type' => 'select',
    '#options' => drupal_map_assoc(range(0, 23, 1), '_uc_coupon_format_hour'),
    '#default_value' => variable_get('uc_coupon_expiry_hour', 0),
  );

  $form['uc_coupon_expiry']['uc_coupon_expiry_timezone'] = array(
    '#type' => 'radios',
    '#options' => array( 0 => 'UTC', 1 => 'Local'),
    '#default_value' => variable_get('uc_coupon_expiry_timezone', 0),
  );


  return system_settings_form($form);
}

/**
 * Display a brief overview of system coupons
 *
 * @param $view_type
 *   'active' or 'inactive'
 */
function uc_coupon_display($view_type = 'active') {
  _uc_coupon_paypal_check();

  $header = array(
    array('data' => t('Actions')),
    array(
      'data' => t('Name'),
      'field' => 'name',
    ),
    array(
      'data' => t('Code'),
      'field' => 'code',
      'sort' => 'asc',
    ),
    array(
      'data' => t('Value'),
      'field' => 'value',
    ),
    array(
      'data' => t('Created'),
      'field' => 'created',
    ),
    array(
      'data' => t('Valid from'),
      'field' => 'valid_from',
    ),
    array(
      'data' => t('Valid until'),
      'field' => 'valid_until',
    ),
  );

  $query = db_select('uc_coupons', 'c')
    ->extend('TableSort')
    ->extend('PagerDefault')
    ->orderByHeader($header);
  $query->condition('c.status', $view_type == 'inactive' ? 0 : 1)->fields('c');

  $rows = array();
  $result = $query->execute();
  foreach ($result as $coupon) {
    $coupon->data = $coupon->data ? unserialize($coupon->data) : array();
    $rows[] = array(
      theme('uc_coupon_actions', array('coupon' => $coupon)),
      check_plain($coupon->name),
      check_plain($coupon->code) . ($coupon->bulk ? '* ' . t('(bulk)') : ''),
      theme('uc_coupon_discount', array('coupon' => $coupon)),
      _uc_coupon_format_date($coupon->created, variable_get('date_format_uc_store', 'm/d/Y')),
      $coupon->valid_from ? _uc_coupon_format_date($coupon->valid_from, variable_get('date_format_uc_store', 'm/d/Y H:iT')) : '-',
      $coupon->valid_until ? _uc_coupon_format_date($coupon->valid_until, variable_get('date_format_uc_store', 'm/d/Y H:iT')) : '-',
    );
  }

  if (count($rows)) {
    $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('width' => '100%')));
    $output .= theme('pager', array('tags' => NULL));
  }
  else {
    switch ($view_type) {
      case 'active':
        $output = '<p>' . t('There are currently no active coupons in the system.') . '</p>';
        break;
      case 'inactive':
        $output = '<p>' . t('There are currently no inactive coupons in the system.') . '</p>';
        break;
    }
  }

  $output .= '<p>' . l(t('Add a new coupon.'), 'admin/store/coupons/add') . '</p>';

  return $output;
}

/**
 * View coupon details.
 *
 * @param $coupon object
 *   Coupon to view.
 */
function uc_coupon_view($coupon) {
  $rows = array();

  $rows[] = array(t('Name'), check_plain($coupon->name));

  if (!$coupon->status) {
    $status = t('Inactive');
  }
  elseif (!$coupon->valid_until) {
    $status = t('Active');
  }
  elseif (REQUEST_TIME < $coupon->valid_from) {
    $status = t('Not valid until @date', array('@date' => _uc_coupon_format_date($coupon->valid_from, variable_get('date_format_uc_store', 'm/d/Y H:iT'))));
  }
  elseif (REQUEST_TIME > $coupon->valid_until) {
    $status = t('Expired on @date', array('@date' => _uc_coupon_format_date($coupon->valid_until, variable_get('date_format_uc_store', 'm/d/Y H:iT'))));
  }
  else {
    $status = t('Active until @date', array('@date' => _uc_coupon_format_date($coupon->valid_until, variable_get('date_format_uc_store', 'm/d/Y H:iT'))));
  }
  $rows[] = array(t('Status'), $status);

  if (!$coupon->bulk) {
    $rows[] = array(t('Code'), check_plain($coupon->code) . ' (' . l(t('Print'), 'admin/store/coupons/' . $coupon->cid . '/print') . ')');
    if ($coupon->max_uses) {
      $rows[] = array(t('Maximum uses'), $coupon->max_uses);
    }
  }
  else {
    $codes = '<strong>' . check_plain($coupon->code) . ' &times; ' . check_plain($coupon->data['bulk_number']) . '</strong>';
    $codes .= ' (' . l(t('Download codes'), 'admin/store/coupons/' . $coupon->cid . '/codes') . ')';
    $codes .= ' (' . l(t('Print all'), 'admin/store/coupons/' . $coupon->cid . '/print') . ')';
    for ($id = 0; $id < $coupon->data['bulk_number']; $id++) {
      $code = uc_coupon_get_bulk_code($coupon, $id);
      $codes .= '<br />' . check_plain($code) . ' (' . l(t('Print'), 'admin/store/coupons/' . $coupon->cid . '/print/' . $code) . ')';
    }

    $rows[] = array(t('Codes'), $codes);
    //$rows[] = array(t('Bulk seed'), check_plain($coupon->bulk_seed));
    if ($coupon->max_uses) {
      $rows[] = array(t('Maximum uses per code'), $coupon->max_uses);
    }
  }

  $rows[] = array(t('Discount value'), theme('uc_coupon_discount', array('coupon' => $coupon)));

  switch ($coupon->data['apply_to']) {
    case 'subtotal':
      $applies = t('Order subtotal');
      break;
    case 'products_total':
      $applies = t('Total of matching products');
      break;
    case 'products':
      $applies = t('Matching products');
      break;
    case 'cheapest':
      $applies = format_plural($coupon->data['apply_count'], 'Cheapest product', '@count cheapest products');
      break;
    case 'expensive':
      $applies = format_plural($coupon->data['apply_count'], 'Most expensive product', '@count most expensive products');
      break;
  }
  $rows[] = array(t('Applied to'), $applies);

  if ($coupon->data['apply_to'] != 'subtotal') {
    $restrict = array();
    if (isset($coupon->data['product_types'])) {
      $key = format_plural(count($coupon->data['product_types']), 'All products in class', 'All products in classes');
      $restrict[$key] = $coupon->data['product_types'];
    }
    if (isset($coupon->data['products'])) {
      $products = array();
      foreach ($coupon->data['products'] as $nid) {
        $products[] = check_plain(db_query("SELECT title FROM {node} WHERE nid = :nid", array(':nid' => $nid))->fetchField());
      }
      if (isset($coupon->data['negate_products'])) {
        $restrict[t('All products except')] = $products;
      }
      else {
        $restrict[format_plural(count($products), 'Product', 'Products')] = $products;
      }
    }
    if (isset($coupon->data['skus'])) {
      $restrict[format_plural(count($coupon->data['skus']), 'SKU', 'SKUs')] = $coupon->data['skus'];
    }
    if (isset($coupon->data['terms'])) {
      $terms = array();
      foreach ($coupon->data['terms'] as $tid) {
        $terms[] = check_plain(db_query("SELECT name FROM {taxonomy_term_data} WHERE tid = :tid", array(':tid' => $tid))->fetchField());
      }
      if (isset($coupon->data['negate_terms'])) {
        $restrict[t('All taxonomy terms except')] = $terms;
      }
      else {
        $restrict[format_plural(count($terms), 'Taxonomy term', 'Taxonomy terms')] = $terms;
      }
    }
    if ($restrict) {
      $or = FALSE;
      foreach ($restrict as $title => &$restriction) {
        if ($or) {
          $title = t('or') . ' ' . $title;
        }
        $restriction = $title . ': <em>' . implode('</em>, <em>', $restriction) . '</em>';
        $or = TRUE;
      }
      $rows[] = array(t('Product restrictions'), implode($restrict, '<br />'));
    }
  }

  $restrict = array();
  if (isset($coupon->data['users'])) {
    $users = array();
    foreach ($coupon->data['users'] as $uid) {
      $users[] = check_plain(db_query("SELECT name FROM {users} WHERE uid = :uid", array(':uid' => $uid))->fetchField());
    }
    if (isset($coupon->data['negate_users'])) {
      $restrict[t('All users except')] = $users;
    }
    else {
      $restrict[format_plural(count($users), 'User', 'Users')] = $users;
    }
  }
  if (isset($coupon->data['max_uses_per_user'])) {
    $restrict['Maximum uses per user'] = array($coupon->data['max_uses_per_user']);
  }
  if (isset($coupon->data['roles'])) {
    if (isset($coupon->data['negate_roles'])) {
      $restrict[t('All roles except')] = $coupon->data['roles'];
    }
    else {
      $restrict[format_plural(count($users), 'Role', 'Roles')] = $coupon->data['roles'];
    }
  }
  if ($restrict) {
    foreach ($restrict as $title => &$restriction) {
      $restriction = $title . ': <em>' . implode('</em>, <em>', $restriction) . '</em>';
    }
    $rows[] = array(t('User restrictions'), implode($restrict, '<br />'));
  }

  if ($coupon->minimum_order > 0) {
    $rows[] = array(t('Order restrictions'), t('Minimum subtotal') . ': <em>' . uc_currency_format($coupon->minimum_order) . '</em>');
  }

  $combo_list = array();
  if (!empty($coupon->data['combinations'])) {
    $combos = db_query('SELECT cid, name FROM {uc_coupons} where cid IN (:cids)', array(':cids' => $coupon->data['combinations']));
    foreach ($combos as $combo) {
      $combo_list[] = check_plain("$combo->name [cid:$combo->cid]");
    }
  }
  if (isset($coupon->data['negate_combinations'])) {
    $ctext = empty($combo_list) ? t('Any.') : t('Any but:') . ' ' . implode(', ', $combo_list);
  }
  else {
    $ctext = empty($combo_list) ? t('None.') : t('Only:') . ' ' . implode(', ', $combo_list);
  }
  $rows[] = array(t('Allowed Combinations'), $ctext);

  foreach ($rows as &$row) {
    $row[0] = array(
      'header' => TRUE,
      'data' => $row[0],
    );
  }

  $output = theme('table', array('header' => array(), 'rows' => $rows));
  return $output;
}

/**
 * Print a coupon, or set of bulk coupons.
 */
function uc_coupon_print($coupon, $code = NULL, $op = 'view') {
  if ($code) {
    $codes = array($code);
  }
  elseif (!$coupon->bulk) {
    $codes = array($coupon->code);
  }
  else {
    $codes = array();
    for ($id = 0; $id < $coupon->data['bulk_number']; $id++) {
      $codes[] = uc_coupon_get_bulk_code($coupon, $id);
    }
  }

  $output = '';
  foreach ($codes as $code) {
    $output .= theme('uc_coupon_certificate', array('coupon' => $coupon, 'code' => $code));
  }

  drupal_add_css(drupal_get_path('module', 'uc_coupon') . '/uc_coupon.css');

  if ($op == 'print') {
    echo theme('uc_coupon_page', array('content' => $output));
    exit;
  }

  return $output;
}

/**
 * Coupon add/edit form.
 *
 * @param $coupon object
 *   Coupon object, or NULL to add a new coupon.
 */
function uc_coupon_add_form($form, &$form_state, $coupon = NULL) {
  _uc_coupon_paypal_check();

  drupal_add_css(drupal_get_path('module', 'uc_coupon') . '/uc_coupon.css');
  drupal_add_js(drupal_get_path('module', 'uc_coupon') . '/uc_coupon.admin.js');

  if ($coupon) {
    $value = $coupon;
    $usage = uc_coupon_count_usage($coupon->cid);
    $used = array_sum($usage['codes']);
    $form['#uc_coupon_cid'] = $value->cid;
    $form['#uc_coupon'] = $value;
    $form['#uc_coupon_used'] = $used;
    $use_validity = $value->valid_until;
  }
  else {
    $use_validity = FALSE;
    $value = new stdClass;
    $value->minimum_order = 0;
    $value->data['apply_to'] = 'subtotal';
    $value->max_uses = variable_get('uc_coupon_default_usage', 'single') == 'single' ? 1 : 0;
    $used = 0;
  }

  if (!$use_validity) {
    $value->valid_from = REQUEST_TIME;
    $value->valid_until = REQUEST_TIME;
  }

  // Handle AJAX functionality.
  $ajax_fields = array(
    'products' => 'admin/store/coupons/autocomplete/node',
    'skus' => '',
    'terms' => 'admin/store/coupons/autocomplete/term',
    'users' => 'admin/store/coupons/autocomplete/user',
    'roles' => 'admin/store/coupons/autocomplete/role',
  );

  if (empty($form_state['ajax_submission'])) {
    // Set the initial field count.
    foreach (array_keys($ajax_fields) as $field) {
      $form_state['storage']['field_count'][$field] = isset($value->data[$field]) ? (count($value->data[$field]) + 3) : 3;
    }
  }
  else {
    // Use data from the AJAX submission.
    $value->data = $form_state['coupon_data'];

    // Increase the count for the selected field.
    $form_state['storage']['field_count'][$form_state['ajax_submission']] += 3;

    // Reset form action, otherwise validation errors go wrong.
    $form['#action'] = $form_state['action'];
  }

  foreach (array_keys($ajax_fields) as $field) {
    if (!isset($value->data[$field])) {
      $value->data[$field] = array();
    }
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Coupon name'),
    '#default_value' => isset($value->name) ? $value->name : '',
    '#required' => TRUE,
    '#weight' => -15,
  );

  $form['code'] = array(
    '#type' => 'textfield',
    '#title' => t('Coupon code'),
    '#description' => t('Coupon codes cannot be changed once they have been used in an order.'),
    '#default_value' => isset($value->code) ? $value->code : '',
    '#size' => 25,
    '#required' => !$used,
    '#maxlength' => 50,
    '#disabled' => $used,
    '#weight' => -10,
  );

  $form['bulk'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bulk coupon codes'),
    '#description' => t('The coupon code entered above will be used to prefix each generated code.'),
    '#collapsible' => TRUE,
    '#collapsed' => isset($value->bulk) ? !$value->bulk : TRUE,
    '#weight' => -5,
  );

  $form['bulk']['bulk_generate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable bulk generation of coupon codes.'),
    '#default_value' => isset($value->bulk) ? $value->bulk : FALSE,
    '#disabled' => $used,
  );

  $form['bulk']['bulk_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of codes to generate'),
    '#default_value' => isset($value->data['bulk_number']) ? $value->data['bulk_number'] : NULL,
    '#size' => 10,
    '#maxlength' => 10,
    '#description' => $used ? t('You cannot decrease this number, because this coupon has already been used.') : '',
  );

  $form['bulk']['bulk_length'] = array(
    '#type' => 'select',
    '#title' => t('Code length'),
    '#description' => t('The number of characters selected here will be appended to the code entered above when generating bulk coupons.'),
    '#default_value' => isset($value->data['bulk_length']) ? $value->data['bulk_length'] : NULL,
    '#options' => drupal_map_assoc(range(8, 30)),
    '#disabled' => $used,
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Coupon is active.'),
    '#description' => t('Only active coupons can be used at checkout.'),
    '#default_value' => isset($value->status) ? $value->status : FALSE,
  );

  $form['discount'] = array(
    '#type' => 'textfield',
    '#title' => t('Discount'),
    '#default_value' => isset($coupon->value) ? _uc_coupon_format_discount($value, FALSE) : '',
    '#size' => 6,
    '#description' => t('Three discount types are possible:') . theme('item_list', array('items' => array(
        t('Fixed value discount; enter "15" for @p off.', array('@p' => uc_currency_format(15))),
        t('Percentage discount; enter "15%" for 15% off.'),
        t('Set product price; enter "=15" to make all matching products @p after discount.', array('@p' => uc_currency_format(15))),
      ))),
    '#field_prefix' => variable_get('uc_sign_after_amount', FALSE) ? '' : variable_get('uc_currency_sign', '$'),
    '#field_suffix' => variable_get('uc_sign_after_amount', FALSE) ? variable_get('uc_currency_sign', '$') : '',
    '#required' => TRUE,
  );

  $form['apply_to'] = array(
    '#type' => 'radios',
    '#title' => t('Apply discount to'),
    '#options' => array(
      'subtotal' => t('Order subtotal'),
      'products_total' => t('Total of matching product(s)'),
      'products' => t('All matching product(s)'),
      'cheapest' => t('The cheapest matching product(s)'),
      'expensive' => t('The most expensive matching product(s)'),
    ),
    '#description' => t('Coupons with no restrictions will match all products.'),
    '#default_value' => $value->data['apply_to'],
  );
  $form['apply_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of products to discount'),
    '#default_value' => isset($value->data['apply_count']) ? $value->data['apply_count'] : '',
    '#size' => 5,
  );
  $form['require_match_all'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require all products to match'),
    '#default_value' => isset($value->data['require_match_all']) ? TRUE : FALSE,
    '#description' => t('If checked, the coupon will be rejected if an order contains any non-applicable products as configured below.'),
  );

  $form['date_restrictions'] = array(
    '#type' => 'item',
    '#title' => t('Date restrictions'),
  );
  $form['date_restrictions']['use_validity'] = array(
    '#type' => 'checkbox',
    '#title' => t('Restrict coupon by date.'),
    '#default_value' => $use_validity != 0,
  );
  $zone = variable_get('uc_coupon_expiry_timezone', 0) ? '(local time)' : '(UTC)';
  $hour = _uc_coupon_format_hour(variable_get('uc_coupon_expiry_hour', 0));
  $link = l('store-wide coupon settings page', 'admin/store/settings/coupon', array('query' => array('destination' => $_GET['q'])));
  $form['date_restrictions']['valid_from'] = array(
    '#type' => 'date',
    '#title' => t('Start date'),
    '#default_value' => array(
      'year' => _uc_coupon_format_date($value->valid_from, 'Y'),
      'month' => _uc_coupon_format_date($value->valid_from, 'n'),
      'day' => _uc_coupon_format_date($value->valid_from, 'j'),
    ),
    '#description' => t('Coupon will activate at @hour @zone on the specified date. The time/zone may be changed on the !link',
        array('@hour' => $hour, '@zone' => $zone, '!link' => $link)),
    '#after_build' => array('_uc_coupon_date_range'),
  );
  $form['date_restrictions']['valid_until'] = array(
    '#type' => 'date',
    '#title' => t('Expiry date'),
    '#default_value' => array(
      'year' => _uc_coupon_format_date($value->valid_until, 'Y'),
      'month' => _uc_coupon_format_date($value->valid_until, 'n'),
      'day' => _uc_coupon_format_date($value->valid_until, 'j'),
    ),
    '#description' => t('Coupon will expire at @hour @zone on the specified date. The time/zone may be changed on the !link',
        array('@hour' => $hour, '@zone' => $zone, '!link' => $link)),
    '#after_build' => array('_uc_coupon_date_range'),
  );

  $form['max_uses'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of redemptions (per code)'),
    '#default_value' => $value->max_uses ? $value->max_uses : '',
    '#description' => t('If set, the coupon can only be used this number of times. For bulk coupons, this applies to each available code.'),
    '#size' => 5,
  );
  $form['max_uses_per_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of redemptions (per user)'),
    '#default_value' => isset($value->data['max_uses_per_user']) ? $value->data['max_uses_per_user'] : '',
    '#description' => t('If set, each user can only use the coupon this number of times.'),
    '#size' => 5,
  );

  $form['order'] = array(
    '#type' => 'fieldset',
    '#title' => t('Minimum order limits'),
    '#collapsible' => TRUE,
    '#collapsed' => $value->minimum_order < 0.01 && !isset($value->data['minimum_qty']),
  );
  $form['order']['minimum_order'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum order total'),
    '#default_value' => $value->minimum_order < 0.01 ? '' : $value->minimum_order,
    '#size' => 6,
    '#description' => t('If set, the subtotal of products must be at least this amount for the coupon to be accepted.'),
    '#field_prefix' => variable_get('uc_sign_after_amount', FALSE) ? '' : variable_get('uc_currency_sign', '$'),
    '#field_suffix' => variable_get('uc_sign_after_amount', FALSE) ? variable_get('uc_currency_sign', '$') : '',
  );
  $form['order']['minimum_qty'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum order quantity'),
    '#default_value' => isset($value->data['minimum_qty']) ? $value->data['minimum_qty'] : '',
    '#size' => 5,
    '#description' => t('If set, at least this many products must be in the cart for the coupon to be accepted.'),
  );
  $form['order']['minimum_qty_restrict'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only count applicable products'),
    '#default_value' => isset($value->data['minimum_qty_restrict']) ? TRUE : FALSE,
    '#description' => t('If checked, only applicable products as configured below will count towards the minimum quantity or total.'),
  );


  $form['product_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Applicable product classes'),
    '#description' => t('Selecting a class will apply this coupon to <strong>all</strong> products of the class, overriding other restrictions except for products or terms explicitly excluded below.'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($value->data['product_types']),
  );
  $form['product_types']['product_types'] = array(
    '#type' => 'checkboxes',
    '#options' => array_map('check_plain', array_intersect_key(node_type_get_names(), drupal_map_assoc(uc_product_types()))),
    '#default_value' => isset($value->data['product_types']) ? $value->data['product_types'] : array(),
  );

  $form['products'] = array(
    '#type' => 'fieldset',
    '#title' => t('Applicable products'),
    '#description' => t('Enter one or more products below to restrict this coupon to a set of products, regardless of any product attributes. Discounts will apply to each matching product.'),
  );
  $form['products']['negate_products'] = array(
    '#type' => 'radios',
    '#default_value' => isset($value->data['negate_products']) ? 1 : 0,
    '#options' => array(
      0 => t('Apply coupon to products listed below.'),
      1 => t('Apply coupon to all products except those listed below.'),
    ),
    '#tree' => FALSE,
  );
  foreach ($value->data['products'] as $nid) {
    $title = db_query('SELECT title FROM {node} WHERE nid = :nid', array(':nid' => $nid))->fetchField();
    $form['products'][] = array(
      '#type' => 'textfield',
      '#default_value' => $title . ' [nid:' . $nid . ']',
      '#autocomplete_path' => 'admin/store/coupons/autocomplete/node',
      '#maxlength' => 300,
    );
  }

  $form['skus'] = array(
    '#type' => 'fieldset',
    '#title' => t('Applicable SKUs'),
    '#description' => t('Enter one or more SKUs below to restrict this coupon to a set of SKUs, allowing coupons to apply to specific products or attribute options. Discounts will apply to matching SKUs. Wildcards are supported, e.g. "E*" will match all products with SKUs beginning with E.'),
  );
  foreach ($value->data['skus'] as $sku) {
    $form['skus'][] = array(
      '#type' => 'textfield',
      '#default_value' => $sku,
    );
  }

  $form['terms'] = array(
    '#type' => 'fieldset',
    '#title' => t('Applicable taxonomy terms'),
    '#description' => t('Enter one or more taxonomy terms (categories) below to restrict this coupon to a set of products. Discounts will apply to all matching products with these terms.'),
    '#maxlength' => 300,
  );
  $form['terms']['negate_terms'] = array(
    '#type' => 'radios',
    '#default_value' => isset($value->data['negate_terms']) ? 1 : 0,
    '#options' => array(
      0 => t('Apply coupon to products with terms listed below.'),
      1 => t('Apply coupon to all products except those with terms listed below.'),
    ),
    '#tree' => FALSE,
  );
  foreach ($value->data['terms'] as $tid) {
    $name = db_query('SELECT name FROM {taxonomy_term_data} WHERE tid = :tid', array(':tid' => $tid))->fetchField();
    $form['terms'][] = array(
      '#type' => 'textfield',
      '#default_value' => $name . ' [tid:' . $tid . ']',
      '#autocomplete_path' => 'admin/store/coupons/autocomplete/term',
    );
  }

  $form['users'] = array(
    '#type' => 'fieldset',
    '#title' => t('User restrictions'),
    '#description' => t('Enter one or more user names and/or "anonymous users" below to restrict this coupon by user.'),
  );
  $form['users']['negate_users'] = array(
    '#type' => 'radios',
    '#default_value' => isset($value->data['negate_users']) ? 1 : 0,
    '#options' => array(
      0 => t('Allow this coupon only for users listed below.'),
      1 => t('Allow this coupon for all users except those listed below.'),
    ),
    '#tree' => FALSE,
  );

  foreach ($value->data['users'] as $uid) {
    $username = $uid ? db_query('SELECT name FROM {users} WHERE uid = :uid', array(':uid' => $uid))->fetchField() : t('anonymous users');
    $form['users'][] = array(
      '#type' => 'textfield',
      '#default_value' => $username . ' [uid:' . $uid . ']',
      '#autocomplete_path' => 'admin/store/coupons/autocomplete/user',
    );
  }

  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role restrictions'),
    '#description' => t('Enter one or more role names below to restrict this coupon based on a user\'s roles.'),
  );
  $form['roles']['negate_roles'] = array(
    '#type' => 'radios',
    '#default_value' => isset($value->data['negate_roles']) ? 1 : 0,
    '#options' => array(
      0 => t('Allow this coupon only for users with the roles listed below.'),
      1 => t('Allow this coupon for users with all roles except those listed below.'),
    ),
    '#tree' => FALSE,
  );

  foreach ($value->data['roles'] as $role) {
    $form['roles'][] = array(
      '#type' => 'textfield',
      '#default_value' => $role,
      '#autocomplete_path' => 'admin/store/coupons/autocomplete/role',
    );
  }

  $form['combinations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Combinations'),
    '#description' => t('Restrict the way this coupon may be combined with other coupons.'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($value->data['combinations']) && !isset($value->data['negate_combinations']),
  );
  $form['combinations']['negate_combinations'] = array(
    '#type' => 'radios',
    '#default_value' => isset($value->data['negate_combinations']) ? 1 : 0,
    '#options' => array(
      0 => t('Allow this coupon to be combined only with those selected below. If you do not select any coupons, this coupon may not be combined.'),
      1 => t('Allow this coupon to be combined with all coupons except those selected below.'),
    ),
    '#tree' => FALSE,
  );
  $form['combinations']['combinations'] = array(
    '#type' => 'select',
    '#multiple' => 'true',
    '#options' => db_query('SELECT cid, name FROM {uc_coupons}')->fetchAllKeyed(),
    '#default_value' => isset($value->data['combinations']) ? $value->data['combinations'] : array(),
  );

  // Add common settings and ajax functionality to restriction fieldsets.
  foreach ($ajax_fields as $field => $path) {
    $form[$field] = array_merge($form[$field], array(
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => empty($value->data[$field]) && empty($form_state['ajax_submission']),
      '#prefix' => '<div id="ajax-' . $field . '">',
      '#suffix' => '</div>',
      'more_' . $field => array(
        '#type' => 'submit',
        '#value' => t('Add more items'),
        '#weight' => 1,
        '#name' =>  'op_' . $field,
        '#limit_validation_errors' => array(),
        '#submit' => array('uc_coupon_add_form_submit'),
        '#ajax' => array(
          'callback' => 'uc_coupon_ajax_callback',
          'wrapper' => 'ajax-' . $field,
          'effect' => 'fade',
        ),
        '#tree' => FALSE,
      ),
    ));

    // Add blank fields if needed.
    for ($i = count($value->data[$field]); $i < $form_state['storage']['field_count'][$field]; $i++) {
      $form[$field][] = array(
        '#type' => 'textfield',
        '#autocomplete_path' => $path ? $path : NULL,
        '#maxlength' => ($field == 'products' || $field == 'terms') ? 300 : 128,
      );
    }
  }

  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display options'),
    '#description' => t('Options governing the way this coupon appears to users.'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($value->data['line_item_format']) || empty($value->data['apply_message']),
  );

  $form['display']['line_item_format'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Line item title format.'),
    '#description' => t('The title for this coupon when displayed in the cart and as a line-item.
    		Leave blank to use the default format as specified on the main coupon settings page.'),
    '#default_value' => empty($value->data['line_item_format']) ? '' : $value->data['line_item_format'],
  );

  $form['display']['apply_message'] = array(
    '#type' => 'textarea',
    '#rows' => 3,
    '#title' => t('Message'),
    '#description' => t('The message to be displayed when this coupon is applied. Leave blank for a default message.'),
    '#default_value' => empty($value->data['apply_message']) ? '' : $value->data['apply_message'],
  );

  if (module_exists('token')) {
    $form['display']['replacement_patterns'] = array(
      '#type' => 'item',
      '#title' => t('Replacement Patterns'),
      '#markup' => theme('token_tree', array('token_types' => array('uc_coupon'))),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save coupon'),
    '#weight' => 10,
  );

  return $form;
}

function _uc_coupon_format_hour($hour) {
  return sprintf("%02d:00", $hour);
}

function _uc_coupon_date_range($form_element) {
  $form_element['year']['#options'] = drupal_map_assoc(range(2008, 2020));
  return $form_element;
}

/**
 * Ajax form element selector callback for "Add more items" buttons in restrictions fieldsets.
 */
function uc_coupon_ajax_callback($form, $form_state) {
  return $form[$form_state['ajax_submission']];
}

/**
 * Autocomplete callback for product restrictions.
 */
function uc_coupon_autocomplete_node($string) {
  $matches = array();
  $product_types = uc_product_types();

  $result = db_query("SELECT nid, title FROM {node} WHERE type IN (:types) AND title LIKE :title", array( ':types' => $product_types, ':title' => "%$string%"));
  foreach ($result as $row) {
    $title = check_plain($row->title);
    $matches[$title . ' [nid:' . $row->nid . ']'] = $title;
  }
  print drupal_json_encode($matches);
  exit;
}

/**
 * Autocomplete callback for term restrictions.
 */
function uc_coupon_autocomplete_term($string) {

  $matches = array();
  $result = db_query("SELECT tid, name FROM {taxonomy_term_data} WHERE name LIKE :name", array(':name' => "%$string%"));
  foreach ($result as $row) {
    $matches[$row->name . ' [tid:' . $row->tid . ']'] = $row->name;
  }
  print drupal_json_encode($matches);
  exit;
}

/**
 * Autocomplete callback for user restrictions.
 */
function uc_coupon_autocomplete_user($string) {
  $matches = array();
  $anonymous = t('anonymous users');
  if (strpos($anonymous, $string) !== FALSE) {
    $matches[$anonymous . ' [uid:0]'] = $anonymous;
  }

  $result = db_query("SELECT uid, name FROM {users} WHERE name LIKE :name", array( ':name' => "%$string%"));
  foreach ($result as $row) {
    $matches[$row->name . ' [uid:' . $row->uid . ']'] = $row->name;
  }
  print drupal_json_encode($matches);
  exit;
}

/**
 * Autocomplete callback for role restrictions.
 */
function uc_coupon_autocomplete_role($string) {
  $matches = array();
  $result = db_query("SELECT name FROM {role} WHERE name LIKE :name", array(':name' => "%$string%"));
  foreach ($result as $row) {
    $matches[$row->name] = $row->name;
  }
  print drupal_json_encode($matches);
  exit;
}

/**
 * Coupon form validate handler.
 */
function uc_coupon_add_form_validate($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'uc_coupon') . '/uc_coupon.css');
  drupal_add_js(drupal_get_path('module', 'uc_coupon') . '/uc_coupon.admin.js');

  $name = db_query("SELECT name FROM {uc_coupons} WHERE code = :code AND cid <> :cid", array(':code' => strtoupper($form_state['values']['code']), ':cid' => isset($form['#uc_coupon_cid']) ? $form['#uc_coupon_cid'] : 0))->fetchField();
  if ($name) {
    form_set_error('code', t('Coupon code already used by %name.', array('%name' => $name)));
  }

  if (isset($form['#uc_coupon_used']) && $form['#uc_coupon_used']
      && isset($form_state['values']['bulk_number']) && isset($form['#uc_coupon']->data['bulk_number'])
      && $form_state['values']['bulk_number'] < $form['#uc_coupon']->data['bulk_number']) {
    form_set_error('bulk_number', t('You cannot decrease the number of bulk codes for a coupon that has been used.'));
  }

  if (!preg_match('/^(=(?!.*%))?\d+(\.\d+)?%?$/', $form_state['values']['discount'])) {
    form_set_error('discount', t('Invalid discount.'));
  }

  if (substr($form_state['values']['discount'], 0, 1) == '=' &&
    ($form_state['values']['apply_to'] == 'subtotal' || $form_state['values']['apply_to'] == 'products_total')) {
    form_set_error('apply_to', t('Set prices cannot be applied to the order subtotal or matching products total.'));
  }

  if ($form_state['values']['apply_to'] == 'cheapest' || $form_state['values']['apply_to'] == 'expensive') {
    if (!preg_match('/^[1-9]\d*$/', $form_state['values']['apply_count'])) {
      form_set_error('apply_count', t('You must specify the maximum number of products to discount.'));
    }
  }

  foreach ($form_state['values']['products'] as $key => $product) {
    if ($product && !preg_match('/\[nid:(\d+)\]$/', $product)) {
      form_set_error('products][' . $key, t('Products must include the node ID.'));
    }
  }

  foreach ($form_state['values']['users'] as $key => $user) {
    if ($user && !preg_match('/\[uid:(\d+)\]$/', $user)) {
      form_set_error('users][' . $key, t('User names must include the user ID.'));
    }
  }

  if ( (!isset($form['#uc_coupon_used']) || $form['#uc_coupon_used'] == 0)
      && $form_state['values']['bulk_generate']
      && intval($form_state['values']['bulk_number']) <= 0) {
    form_set_error('bulk_number', t('You must specify the number of codes to generate.'));
  }

  if ($form_state['values']['use_validity']) {
    $valid_from = _uc_coupon_mktime($form_state['values']['valid_from']['month'], $form_state['values']['valid_from']['day'], $form_state['values']['valid_from']['year']);
    $valid_until = _uc_coupon_mktime($form_state['values']['valid_until']['month'], $form_state['values']['valid_until']['day'], $form_state['values']['valid_until']['year']);
    if ($valid_from > $valid_until) {
      form_set_error('valid_from', t('The coupon start date must be before the expiration date.'));
    }
  }
}

/**
 * Coupon form submit handler.
 */
function uc_coupon_add_form_submit($form, &$form_state) {
  // If the coupon was previously used, reset disabled textfields to their original values.
  if (isset($form['#uc_coupon_used']) && $form['#uc_coupon_used'] != 0) {
    $form_state['values']['code'] = $form['#uc_coupon']->code;
  }

  // Ensure all fields of original coupon object propagate to presave hook.
  $coupon = (isset($form['#uc_coupon'])) ? $form['#uc_coupon'] : new stdClass;
  if (isset($form['#uc_coupon_cid'])) {
    $coupon->cid = $form['#uc_coupon_cid'];
  }

  // Set basic coupon information.
  $coupon->name = $form_state['values']['name'];
  $coupon->code = strtoupper($form_state['values']['code']);
  $coupon->bulk = $form_state['values']['bulk_generate'];
  $coupon->data['bulk_number'] = $form_state['values']['bulk_generate'] ? $form_state['values']['bulk_number'] : 0;
  $coupon->data['bulk_length'] = $form_state['values']['bulk_length'];
  if ($form_state['values']['use_validity']) {
    $coupon->valid_from = _uc_coupon_mktime($form_state['values']['valid_from']['month'], $form_state['values']['valid_from']['day'], $form_state['values']['valid_from']['year']);
    $coupon->valid_until = _uc_coupon_mktime($form_state['values']['valid_until']['month'], $form_state['values']['valid_until']['day'], $form_state['values']['valid_until']['year']);
  }
  else {
    $coupon->valid_from = $coupon->valid_until = 0;
  }
  $coupon->status = $form_state['values']['status'];
  $coupon->type = 'price';
  if (substr($form_state['values']['discount'], -1) == '%') {
    $coupon->type = 'percentage';
  }
  if (substr($form_state['values']['discount'], 0, 1) == '=') {
    $coupon->type = 'set_price';
  }
  $coupon->value = str_replace(array('%', '='), '', $form_state['values']['discount']);
  $coupon->minimum_order = $form_state['values']['minimum_order'] ? $form_state['values']['minimum_order'] : 0;
  $coupon->data['minimum_qty'] = $form_state['values']['minimum_qty'];
  $coupon->data['minimum_qty_restrict'] = $form_state['values']['minimum_qty_restrict'];
  $coupon->data['require_match_all'] = $form_state['values']['require_match_all'];
  $coupon->max_uses = $form_state['values']['max_uses'] ? $form_state['values']['max_uses'] : 0;
  $coupon->data['max_uses_per_user'] = $form_state['values']['max_uses_per_user'];
  $coupon->data['apply_to'] = $form_state['values']['apply_to'];
  if ($form_state['values']['apply_to'] == 'cheapest' || $form_state['values']['apply_to'] == 'expensive') {
    $coupon->data['apply_count'] = $form_state['values']['apply_count'];
  }

  // Map restrictions back to their IDs.
  array_walk($form_state['values']['products'], '_uc_coupon_map_restriction', 'nid');
  array_walk($form_state['values']['terms'], '_uc_coupon_map_restriction', 'tid');
  array_walk($form_state['values']['users'], '_uc_coupon_map_restriction', 'uid');

  // Set coupon restrictions.
  $coupon->data['product_types'] = drupal_map_assoc(array_filter($form_state['values']['product_types']));
  $coupon->data['negate_products'] = $form_state['values']['negate_products'];
  $coupon->data['products'] = drupal_map_assoc(array_filter($form_state['values']['products']));
  $coupon->data['skus'] = drupal_map_assoc(array_filter($form_state['values']['skus']));
  $coupon->data['negate_terms'] = $form_state['values']['negate_terms'];
  $coupon->data['terms'] = drupal_map_assoc(array_filter($form_state['values']['terms']));
  $coupon->data['negate_users'] = $form_state['values']['negate_users'];
  $coupon->data['users'] = drupal_map_assoc(array_filter($form_state['values']['users'], 'is_numeric'));
  $coupon->data['negate_roles'] = $form_state['values']['negate_roles'];
  $coupon->data['roles'] = drupal_map_assoc(array_filter($form_state['values']['roles']));
  $coupon->data['negate_combinations'] = $form_state['values']['negate_combinations'];
  $coupon->data['combinations'] = drupal_map_assoc(array_filter($form_state['values']['combinations']));

  // Set coupon display info
  $coupon->data['apply_message'] = trim($form_state['values']['apply_message']);
  $coupon->data['line_item_format'] = trim($form_state['values']['line_item_format']);

  // Remove zero values and empty arrays.
  $coupon->data = array_filter($coupon->data);

  // Handle AJAX submissions.
  if (!empty($form_state['triggering_element']) && substr($elem = $form_state['triggering_element']['#name'], 0, 3) === 'op_') {
    $form_state['ajax_submission'] = substr($elem, 3);
    $form_state['coupon_data'] = $coupon->data;
    $form_state['rebuild'] = TRUE;
  }
  else {
    uc_coupon_save($coupon, $form_state['values']);
    drupal_set_message(t('Coupon %name has been saved.', array('%name' => $coupon->name)));
    $form_state['redirect'] = 'admin/store/coupons' . ($coupon->status ? '' : '/inactive');
  }
}

function _uc_coupon_map_restriction(&$value, $key, $prefix) {
  if ($value && preg_match('/\[' . $prefix . ':(\d+)\]$/', $value, $matches)) {
    $value = $matches[1];
  }
  else {
    $value = FALSE;
  }
}

/**
 * Delete coupon confirm form
 *
 * @param $cid int
 * Coupon ID.
 *
 * @return $confirm
 *  Return a drupal confirm form.
 */
function uc_coupon_delete_confirm($form, &$form_state, $coupon) {
  $form['#uc_coupon_cid'] = $coupon->cid;
  return confirm_form($form, t('Are you sure you want to delete coupon %name with code %code?', array('%name' => $coupon->name, '%code' => $coupon->code)), 'admin/store/coupons', t('This action cannot be undone. Deleting this coupon will remove all records of past uses as well.'), t('Delete'));
}

/**
 * Delete coupon confirm form submit handler.
 */
function uc_coupon_delete_confirm_submit($form, &$form_state) {
  $coupon = uc_coupon_delete($form['#uc_coupon_cid']);
  drupal_set_message(t('Coupon %name has been deleted.', array('%name' => $coupon->name)));
  $form_state['redirect'] = 'admin/store/coupons' . ($coupon->status ? '' : '/inactive');
}

/**
 * Generate a list of bulk coupon codes.
 */
function uc_coupon_codes_csv($coupon) {
  if (!$coupon->bulk) {
    drupal_not_found();
    return;
  }

  header('Content-Type: application/octet-stream');
  header('Content-Disposition: attachment; filename="' . $coupon->code . '.csv";');

  for ($id = 0; $id < $coupon->data['bulk_number']; $id++) {
    echo uc_coupon_get_bulk_code($coupon, $id) . "\n";
  }
  exit;
}

/**
 * Show the actions a user may perform on a coupon.
 */
function theme_uc_coupon_actions($variables) {
  $coupon = $variables['coupon'];
  $actions = array();
  foreach (module_invoke_all('uc_coupon_actions', $coupon) as $action) {
    $icon = theme('image', array('path' => $action['icon'], 'alt' => $action['title']));
    $actions[] = l($icon, $action['url'], array('attributes' => array('title' => $action['title']), 'html' => TRUE));
  }
  return implode(' ', $actions);
}

/**
 * Show coupon code in Views.
 *
 * @param $coupon
 *   The coupon object for which this code is to be formatted.
 *   $coupon->usage can optionally be set.
 *   For bulk coupons, $coupon->code should be set to the specific code to display,.
 */
function theme_uc_coupon_code($variables) {
  $coupon = $variables['coupon'];
  $code = $coupon->code;

  $class = "uc-coupon-code";
  if (isset($coupon->usage)) {
    $uses = $coupon->usage['codes'][$coupon->code];
    if ($coupon->max_uses == 0 || $coupon->max_uses > $uses) {
      $class .= ' uc-coupon-code-available';
      if (arg(0) == 'user') {
        $code = l($code, "user/" . arg(1) . '/coupons/' . $coupon->cid . "/view/$code");
      }
    }
    else {
      $class .= ' uc-coupon-code-used';
    }
  }

  return '<span class="' . $class . '">' . $code . '</span>';
}

function _uc_coupon_format_date($date, $format) {
  $func = variable_get('uc_coupon_expiry_timezone', 0) ? 'date' : 'gmdate';
	return $func($format, $date);
}

function _uc_coupon_mktime($month, $day, $year) {
  $func = variable_get('uc_coupon_expiry_timezone', 0) ? 'mktime' : 'gmmktime';
  $hour = variable_get('uc_coupon_expiry_hour', 0);
  $time = $func($hour, 0, 0, $month, $day, $year);
  //dpm($time . ' ' . date('m/d/y h:i:s T', $time), 'using ' . $func . '(' . $hour . ')');
  return $func($hour, 0, 0, $month, $day, $year);

}
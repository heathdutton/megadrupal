<?php

/**
 * @file
 * Admin page callbacks and forms for wish lists.
 */

/**
 * Build the admin settings form.
 */
function uc_wishlist_admin_settings_form($form, &$form_state) {
  $form = array();

  $form['uc_wishlist_default_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Default wish list title'),
    '#default_value' => variable_get('uc_wishlist_default_title', "%user's wish list"),
    '#description' => t("The default name of a new wish list. The token %user will be replaced by the user's name."),
  );
  $form['uc_wishlist_save_address'] = array(
    '#type' => 'checkbox',
    '#title' => t('Permit a saved shipping address in a wishlist.'),
    '#default_value' => variable_get('uc_wishlist_save_address', TRUE),
    '#description' => t('Check this box to permit users to specify a default delivery address when creating a wish list. If not checked, purchasers of wish list items must enter a delivery address at checkout.'),
  );
  $form['uc_wishlist_default_private'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make users wishlist private by default.'),
    '#default_value' => variable_get('uc_wishlist_default_private', FALSE),
    '#description' => t('This makes the users wish list private by default and exclude it from wish list search results.This will not give the option to users to set the privacy of their wishlists.'),
  );
  $form['uc_wishlist_allow_private'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to make their wishlist private.'),
    '#default_value' => variable_get('uc_wishlist_allow_private', FALSE),
    '#description' => t('Check this box to allow users to make their wish list private and exclude it from wish list search results.This option will disable when <em>Make users wishlist private by default.</em> is enabled.'),
  );

  // Allow admin to configure out of stock.
  $form['uc_wishlist_out_of_stock'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to add out of stock item into wishlist.'),
    '#default_value' => variable_get('uc_wishlist_out_of_stock', FALSE),
    '#description' => t('Check this box to allow user to add product to their wishlist if product is out of stock.'),
  );

  $form['uc_wishlist_default_from'] = array(
    '#type' => 'textfield',
    '#title' => t('Specify a default <em>From</em> address'),
    '#default_value' => variable_get('uc_wishlist_default_from', ''),
    '#description' => t("If this field is set then all outgoing emails will have the From address set to the given value (normally something like no-reply@example.com). The <em>Reply To</em> address will be set to the users specified email address. This is recommended if you find your outgoing emails are being flagged as spam due to the sender address domain differing from the domain of the outgoing SMTP server."),
  );
  // TODO: add uc_wishlist_default_length

  return system_settings_form($form);
}

/**
 * View and manage the wish lists on the site.
 */
function uc_wishlist_admin() {
  $rows = array();

  $header = array(
    array(
      'data' => t('User'),
      'field' => 'u.name',
      'sort' => 'asc',
    ),
    array(
      'data' => t('Title'),
      'field' => 'w.title',
    ),
    array(
      'data' => t('Expiration date'),
      'field' => 'w.expiration',
    ),
    array('data' => t('Status')),
  );

  // Get a paged list of wish lists from the database.
  $query = db_select('uc_wishlists', 'w');
  $query->leftJoin('users', 'u', 'w.uid = u.uid');
  $query->fields('w', array(
    'wid',
    'uid',
    'title',
    'expiration',
  ));
  $query->addField('u', 'name');
  $result = $query->extend('PagerDefault')->limit(25)->execute();

  foreach ($result as $wishlist) {
    // Build the operations array for the wish list.
    $op = array(
      $wishlist->expiration < REQUEST_TIME ? t('Expired') : t('Active'),
      l(t('Delete'), 'admin/store/customers/wishlist/' . $wishlist->wid . '/delete'),
    );

    $rows[] = array(
      $wishlist->name ? l($wishlist->name, 'user/' . $wishlist->uid) : t('Anonymous'),
      l(filter_xss($wishlist->title), 'wishlist/' . $wishlist->wid),
      format_date($wishlist->expiration),
      implode(' | ', $op),
    );
  }

  if (empty($rows)) {
    $rows[] = array(array(
        'data' => t('No wish lists found.'),
        'colspan' => 4,
      ));
  }

  return theme('table', array('header' => $header, 'rows' => $rows)) . theme('pager');
}

/**
 * Confirm the deletion of a wish list.
 */
function uc_wishlist_admin_delete_form($form, $form_state, $wishlist) {
  $form = array();

  $form['wishlist'] = array(
    '#type' => 'value',
    '#value' => $wishlist,
  );

  return confirm_form($form, t('Are you sure you want to delete wish list %title?', array('%title' => $wishlist->title)), 'admin/store/customers/wishlist', NULL, t('Delete'));
}

/**
 * Submission handler for wish list deletion form.
 */
function uc_wishlist_admin_delete_form_submit($form, &$form_state) {
  // Delete the wish list.
  uc_wishlist_delete($form_state['values']['wishlist']->wid);

  // Display a message and redirect back to the admin table.
  drupal_set_message(t('Wish list %title deleted.', array('%title' => $form_state['values']['wishlist']->title)));

  $form_state['redirect'] = 'admin/store/customers/wishlist';
}

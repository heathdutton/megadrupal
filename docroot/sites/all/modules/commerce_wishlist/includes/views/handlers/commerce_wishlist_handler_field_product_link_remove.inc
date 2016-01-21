<?php

/**
 * Field handler to present a link to remove a product for wishlist.
 */
class commerce_wishlist_handler_field_product_link_remove extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = array(
      'table' => 'commerce_wishlist',
      'field' => 'uid',
    );
    $this->additional_fields['item_id'] = 'item_id';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    // Ensure the user has access to remove this product from wishlist.
    global $user;
    $account = user_load($user->uid);
    $wishlist_id    = $this->get_value($values, 'item_id');
    $wishlist_owner = $this->get_value($values, 'uid');
    if (commerce_wishlist_manage_access($account, $wishlist_id)) {
      $text = !empty($this->options['text']) ? $this->options['text'] : t('Remove');

      $destination = drupal_get_destination();

      return l($text, 'user/' . $wishlist_owner . '/wishlist/remove/' . $wishlist_id, (array(
        'query' => $destination,
      )));
    }

    return '';
  }
}

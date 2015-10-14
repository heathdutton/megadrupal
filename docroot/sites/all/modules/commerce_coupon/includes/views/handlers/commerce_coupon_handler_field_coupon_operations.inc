<?php

/**
 * @file
 * Coupon..
 * Field handler to present an order's operations .
 */
class commerce_coupon_handler_field_coupon_operations extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['coupon_id'] = 'coupon_id';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['add_destination'] = TRUE;

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['add_destination'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add a destination parameter to operations links so users return to this View on form submission.'),
      '#default_value' => $this->options['add_destination'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $coupon_id = $this->get_value($values, 'coupon_id');
    $links = menu_contextual_links('commerce-coupon', 'admin/commerce/coupons', array($coupon_id));

    if (!empty($links)) {
      // Add the destination to the links if specified.
      if ($this->options['add_destination']) {
        foreach ($links as $id => &$link) {
          $link['query'] = drupal_get_destination();
        }
      }
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }

  }
}

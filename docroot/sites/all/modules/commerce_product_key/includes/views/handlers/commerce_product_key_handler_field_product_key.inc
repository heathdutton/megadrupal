<?php

/**
 * @file
 * Contains the basic product field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a product.
 */
class commerce_product_key_handler_field_product_key extends views_handler_field {
  function init(&$view, &$options) {
    parent::init($view, $options);

    if (!empty($this->options['link_to_product'])) {
      $this->additional_fields['product_key_id'] = 'product_key_id';
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_product'] = array('default' => FALSE);

    return $options;
  }

  /**
   * Provide the link to product option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_product_key'] = array(
      '#title' => t("Link this field to the product key's administrative view page"),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_product_key']),
    );
  }

  /**
   * Render whatever the data is as a link to the product.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    /*
    if (!empty($this->options['link_to_product']) && $data !== NULL && $data !== '') {
      $product_key_id = $this->get_value($values, 'product_key_id');
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'admin/commerce/product-keys/' . $product_key_id;
    }
    */

    if (!empty($this->options['link_to_product']) && in_array($this->options['link_to_product'], array('customer', 'admin')) && $data !== NULL && $data !== '') {
      $uid = $this->get_value($values, 'uid');
      $product_key_id = $this->get_value($values, 'product_key_id');

      if ($this->options['link_to_order'] == 'customer' && $uid) {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = 'user/' . $uid . '/orders/' . $product_key_id;
      }
      elseif ($this->options['link_to_order'] == 'admin') {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = 'admin/commerce/orders/' . $product_key_id;
      }
    }

    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}

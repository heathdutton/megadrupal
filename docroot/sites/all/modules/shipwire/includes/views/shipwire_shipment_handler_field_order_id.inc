<?php

/**
 * @file
 * Definition of shipwire_shipment_handler_field_order_id.
 */

/**
 * Field handler to present a link to an order linked to a shipment.
 *
 * @ingroup views_field_handlers
 */
class shipwire_shipment_handler_field_order_id extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);

    if (!empty($this->options['link_to_order'])) {
      $this->additional_fields['order_id'] = 'order_id';
      $this->additional_fields['type'] = 'type';
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_order'] = array('default' => 'none');

    return $options;
  }

  /**
   * Provide the link to order option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_order'] = array(
      '#type' => 'radios',
      '#title' => t('Link this field to'),
      '#options' => array(
        'none' => t('Nothing, unless specified in <em>Rewrite results</em> below'),
        'admin' => t('The administrator view page'),
      ),
      '#default_value' => $this->options['link_to_order'],
    );
  }

  /**
   * Render whatever the data is as a link to the order.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_order']) && $data !== NULL && $data !== '') {
      $type = $this->get_value($values, 'type');
      $order_id = $this->get_value($values, 'order_id');

      if ($this->options['link_to_order'] == 'admin' && in_array($type, array('commerce_order', 'uc_order'))) {
        $this->options['alter']['make_link'] = TRUE;
        switch ($type) {
          case 'commerce_order':
            $this->options['alter']['path'] = 'admin/commerce/orders/' . $order_id;
            break;

          case 'uc_order':
            $this->options['alter']['path'] = 'admin/store/orders/' . $order_id;
            break;
        }
      }
    }

    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }

}

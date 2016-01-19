<?php

/**
 * @file
 * Definition of shipwire_shipment_handler_field_shipment_id.
 */

/**
 * Field handler to render a shipment id which can be optionally linked to the
 * shipment.
 *
 * @ingroup views_field_handlers
 */
class shipwire_shipment_handler_field_shipment_id extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['shipwire_shipment_id'] = 'shipwire_shipment_id';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_shipment'] = array('default' => 'none');

    return $options;
  }

  /**
   * Provide the link to order option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_to_shipment'] = array(
      '#type' => 'radios',
      '#title' => t('Link this field to'),
      '#options' => array(
        'none' => t('Nothing, unless specified in <em>Rewrite results</em> below'),
        'admin' => t('The administrator view page'),
      ),
      '#default_value' => $this->options['link_to_shipment'],
    );
  }

  /**
   * Render whatever the data is as a link to the order.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_shipment']) && $data !== NULL && $data !== '') {
      $shipment_id = $this->get_value($values, 'shipwire_shipment_id');

      if ($this->options['link_to_shipment'] == 'admin') {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = 'admin/shipwire/shipment/' . $shipment_id;
      }
    }

    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }

}

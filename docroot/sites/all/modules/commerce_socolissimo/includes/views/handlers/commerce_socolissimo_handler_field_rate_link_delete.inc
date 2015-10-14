<?php

/**
 * @file
 * Field handler to present a link to delete a socolissimo flexibility rate.
 */

class commerce_socolissimo_handler_field_rate_link_delete extends commerce_socolissimo_handler_field_rate_link {
  /**
   * Class constructor.
   */
  function construct() {
    parent::construct();
  }

  /**
   * Renders a link to delete a rate.
   *
   * @param array $values
   *   View's values.
   *
   * @return bool|string
   *   Returns a link to delete a rate.
   */
  function render($values) {
    // Ensure the user has access to delete this order.
    $name = $this->get_value($values, 'name');
    $rate_id = $this->get_value($values, 'rate_id');
    $socolissimo_flexibility_rate = commerce_socolissimo_rate_new($name);
    $socolissimo_flexibility_rate->rate_id = $rate_id;

    if (!commerce_entity_access('delete', $socolissimo_flexibility_rate, NULL, 'socolissimo_flexibility_rate')) {
      return FALSE;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');

    $service_name_url = 'socolissimo-flexibility' . '-' . strtr($name, '_', '-');

    return l($text, 'admin/commerce/config/shipping/services/' . $service_name_url . '/rate/' . $rate_id . '/delete', array('query' => drupal_get_destination()));
  }
}

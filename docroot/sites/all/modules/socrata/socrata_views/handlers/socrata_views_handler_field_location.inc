<?php

/**
 * @file
 * Definition of socrata_views_handler_field_location.
 */

/**
 * Field handler to provide renderer capable of displaying Socrata location items
 *
 * @ingroup views_field_handlers
 */
class socrata_views_handler_field_location extends views_handler_field {
  /**
   * Collect options for field display.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['display'] = array('default' => 'text');
    $options['link_text'] = array('default' => 'Map', 'translatable' => TRUE);

    return $options;
  }

  /**
   * Option form.
   */
  function options_form(&$form, &$form_state) {
    $form['display'] = array(
      '#type' => 'select',
      '#title' => t('Display mode'),
      '#options' => array(
        'text' => t('Text, no link'),
        'link' => t('Dispay as link'),
        'wkt' => t('Render as WKT'),
      ),
      '#default_value' => $this->options['display'],
    );

    $form['link_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Link text'),
      '#description' => t('Text to use for link'),
      '#default_value' => $this->options['link_text'],
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Generate output of location.
   */
  function render($values) {
    $text = '';
    $value = $this->get_value($values);
    if (!empty($value)) {
      // Decode the nested address data if provided.
      if (!empty($value['human_address'])) {
        $value['human_address'] = json_decode($value['human_address'], TRUE);
        $human_address = $this->sanitize_value(implode(', ', $value['human_address']), 'xss');
      }

      // Suss out default display text - TODO make themeable.
      if (!empty($this->options['link_text'])) {
        $text = $this->sanitize_value($this->options['link_text'], 'xss');
      }
      elseif (!empty($value['human_address'])) {
        $text = $human_address;
      }
      elseif (isset($value['latitude']) && isset($value['longitude'])) {
        $longitude = $this->sanitize_value($value['longitude'], 'xss');
        $latitude = $this->sanitize_value($value['latitude'], 'xss');
        // There is a method to the madness; This is the correct ordering for
        // embedding into a WKT POINT definition.
        $text = "{$longitude}, {$latitude}";
      }
      else {
        $text = t('Map');
      }

      if ('link' == $this->options['display']) {
        $this->options['alter']['make_link'] = TRUE;

        if (!empty($value['human_address'])) {
          $this->options['alter']['path'] = "http://maps.google.com/maps?q={$human_address}";
        }
        elseif (isset($value['latitude']) && isset($value['longitude'])) {
          $this->options['alter']['path'] = "http://maps.google.com/maps?q={$value['latitude']},{$value['longitude']}";
        }
      }
      elseif ('wkt' == $this->options['display']) {
        $longitude = $this->sanitize_value($value['longitude'], 'xss');
        $latitude = $this->sanitize_value($value['latitude'], 'xss');
        $text = "POINT ({$longitude} {$latitude})";
      }
    }

    return $text;
  }
}

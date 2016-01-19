<?php

/**
 * @file
 * Definition of socrata_views_handler_field_point.
 */

/**
 * Field handler to provide renderer capable of displaying Socrata point items.
 *
 * @ingroup views_field_handlers
 */
class socrata_views_handler_field_point extends views_handler_field {
  /**
   * Collect options for field display
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
   * Generate output of point.
   */
  function render($values) {
    $text = '';
    $value = $this->get_value($values);
    if (!empty($value)) {
      // Get the lat/long in the order as specified
      // at http://dev.socrata.com/docs/datatypes/point.html.
      $longitude = $this->sanitize_value($value['coordinates'][0], 'xss');
      $latitude = $this->sanitize_value($value['coordinates'][1], 'xss');
      // Suss out default display text - TODO make themeable
      if (!empty($this->options['link_text'])) {
        $text = $this->sanitize_value($this->options['link_text'], 'xss');
      }
      elseif ($longitude && $latitude) {
        // There is a method to the madness;this is the correct ordering
        // for embedding into a WKT POINT definition.
        $text = "{$longitude}, {$latitude}";
      }
      else {
        $text = t('Map');
      }

      if ('link' == $this->options['display']) {
        $this->options['alter']['make_link'] = TRUE;

        if ($longitude && $latitude) {
          $this->options['alter']['path'] = "http://maps.google.com/maps?q={$latitude},{$longitude}";
        }
      }
      elseif ('wkt' == $this->options['display']) {
        $text = "POINT ({$longitude} {$latitude})";
      }
    }

    return $text;
  }
}

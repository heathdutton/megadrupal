<?php

/**
 * @file
 * Contains the list style plugin.
 */

/**
 * Style plugin to render each item in a Carousel.
 *
 * @ingroup views_style_plugins
 */
class JqueryCarouselPluginStyleCarousel extends views_plugin_style {

  /**
   * Set default options.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['selector'] = array('default' => 'rs-carousel');
    return $options;
  }

  /**
   * Build the settings form for the view.
   */
  public function options_form(&$form, &$form_state) {
    $carousel_config_form = jquery_carousel_config_form();
    $form = array_merge($form, $carousel_config_form);

    foreach (array_keys($form) as $key) {
      if (isset($form[$key]) && is_array($form[$key])) {
        $form[$key]['#default_value'] = $this->options[$key];
      }
    }
  }

  /**
   * Validates the settings form values.
   */
  public function options_validate(&$form, &$form_state) {
    $selector = $form_state['input']['style_options']['selector'];
    _jquery_carousel_config_validate('options][selector', $selector, 'view');
  }
}

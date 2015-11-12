<?php

define('GEOCLUSTER_VIEWS_SECTION', 'style_options');

/**
 * Display extender class that integrates geocluster config with views.
 */
class GeoclusterConfigViewsDisplayExtender extends views_plugin_display_extender implements GeoclusterConfigBackendInterface {

  /**
   * @var GeoclusterConfig
   */
  var $config;

  function init(&$view, &$display) {
    parent::init($view, $display);
    $this->config = geocluster_init_config($this);
  }

  function options_definition_alter(&$options) {
    // options_definition() doesn't work for display_extender plugins.
    // see http://drupal.org/node/681468#comment-4384814
    // and http://drupal.org/node/1616540
    $options = array_merge($options, $this->config->options_definition());
  }

  function options_form(&$form, &$form_state) {
    if ($form_state['section'] == GEOCLUSTER_VIEWS_SECTION && isset($form[GEOCLUSTER_VIEWS_SECTION])) {
      $this->config->options_form($form, $form_state);
    }
  }

  function options_submit(&$form, &$form_state) {
    if ($form_state['section'] == GEOCLUSTER_VIEWS_SECTION) {
      $this->config->options_submit($form, $form_state);
    }
  }

  function options_validate(&$form, &$form_state) {
    if ($form_state['section'] == GEOCLUSTER_VIEWS_SECTION && isset($form[GEOCLUSTER_VIEWS_SECTION])) {
      $this->config->options_validate($form, $form_state);
    }
  }

  /** HELPERS */

  /**
   * Returns a display option value.
   */
  public function get_option($option) {
    return $this->get_option_handler()->get_option($option);
  }

  /**
   * Sets an option value.
   */
  public function set_option($option, $value) {
    $this->get_option_handler()->set_option($option, $value);
  }

  /**
   * Get the appropriate option display handler (default or overridden).
   *
   * @return views_display
   */
  protected function &get_option_handler() {
    if ($this->display->is_defaulted(GEOCLUSTER_VIEWS_SECTION) && isset($this->view->display['default'])) {
      return $this->view->display['default']->handler;
    }
    // Else.
    return $this->display;
  }

  /**
   * Returns the view that the configuration is attached to.
   * @return View
   */
  public function get_view() {
    return $this->view;
  }

  /**
   * Returns the display of the configuration.
   * @return views_plugin_display
   */
  public function get_display() {
    return $this->display;
  }
}



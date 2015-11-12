<?php

/**
 * Provides access to geocluster configuration.
 */
interface GeoclusterConfigBackendInterface {

  /**
   * Returns a configuration option value.
   * @return mixed
   */
  public function get_option($option);

  /**
   * Sets an option value.
   */
  public function set_option($option, $value);

  /**
   * Returns the view that the configuration is attached to.
   * @return View
   */
  public function get_view();

  /**
   * Returns the display of the configuration.
   * @return views_plugin_display
   */
  public function get_display();

}


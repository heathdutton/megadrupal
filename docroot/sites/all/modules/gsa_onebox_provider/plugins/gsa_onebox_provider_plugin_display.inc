<?php

/**
 * @file
 * Contains the GSA Onebox display plugin.
 */

/**
 * We are based on a feed display for compatibility.
 *
 * @ingroup views_display_plugins
 */
class GSAOneboxProviderPluginDisplay extends views_plugin_display_feed {

  /**
   * Return the type of styles we require.
   */
  public function get_style_type() {
    return 'gsa_onebox_provider';
  }

  /**
   * Return the sections that can be defaultable.
   */
  public function defaultable_sections($section = NULL) {
    $sections = array('items_per_page', 'offset', 'use_pager', 'pager_element');
    if (in_array($section, $sections)) {
      return FALSE;
    }

    return parent::defaultable_sections($section);
  }

  /**
   * Define the option for this view.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['items_per_page'] = array('default' => '0');
    $options['style_plugin']['default'] = 'gsa_onebox_provider_xml';

    if (isset($options['defaults']['default']['items_per_page'])) {
      $options['defaults']['default']['items_per_page'] = FALSE;
    }

    return $options;
  }

  /**
   * Provide the summary for page options in the views UI.
   *
   * This output is returned as an array.
   */
  public function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    $categories['page']['title'] = t('GSA Onebox settings');
  }

  /**
   * Add HTTP headers for the feed.
   */
  public function add_http_headers() {
    // Ask the style plugin to add any HTTP headers if it wants.
    if (method_exists($this->view->style_plugin, 'add_http_headers')) {
      $this->view->style_plugin->add_http_headers();
    }
  }

  /**
   * Render the display.
   *
   * We basically just work out if we should be rendering the header, body or
   * footer and call the appropriate functions on the style plugins.
   */
  public function render() {
    $result = parent::render();
    if (empty($this->view->live_preview)) {
      $this->add_http_headers();
    }
    return $result;
  }

  /**
   * Preview the display.
   */
  public function preview() {
    if (!empty($this->view->live_preview)) {
      // Change the items per page.
      $this->view->set_items_per_page(20);
      // Force a pager to be used.
      $this->set_option('pager', array('type' => 'some'));
      return '<p>' . t('A maximum of 20 items will be shown in this live preview.') . '</p><pre>' . check_plain($this->view->render()) . '</pre>';
    }
    return $this->view->render();
  }
}

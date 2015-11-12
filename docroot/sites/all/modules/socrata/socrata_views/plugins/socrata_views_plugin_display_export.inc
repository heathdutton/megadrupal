<?php
/**
 * @file
 * Contains the bulk export display plugin.
 *
 * This allows views to be rendered in parts by batch API.
 */

/**
 * The plugin that batches its rendering.
 *
 * We are based on a feed display for compatibility.
 *
 * @ingroup views_display_plugins
 */
class socrata_views_plugin_display_export extends views_plugin_display_feed {

  /**
   * Return the type of styles we require.
   */
  function get_style_type() { return 'socrata_export'; }

  /**
   * The export display has no path because it comes from the endpoint.
   */
  function has_path() { return FALSE; }

  /**
   * Return the sections that can be defaultable.
   */
  function defaultable_sections($section = NULL) {
    if (in_array($section, array(
      'items_per_page',
      'offset',
      'use_pager',
      'pager_element',
    ))) {
      return FALSE;
    }

    return parent::defaultable_sections($section);
  }

  /**
   * Define the option for this view.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['return_path'] = array('default' => '');
    $options['style_plugin']['default'] = 'socrata_views_csv';

    if (isset($options['defaults']['default']['items_per_page'])) {
      $options['defaults']['default']['items_per_page'] = FALSE;
    }

    // Default to no paging
    $options['pager']['contains']['type']['default'] = 'none';

    return $options;
  }

  /**
   * Provide the summary for page options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    // Disable unused sections
    unset($options['path']);
    unset($options['pager']);
    unset($options['cache']);

    $categories['page']['title'] = t('Socrata data export settings');
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_form($form, $form_state);

    switch ($form_state['section']) {
      case 'cache':
        // We're basically going to disable using cache plugins, by disabling
        // the UI.
        if (isset($form['cache']['type']['#options'])) {
          foreach ($form['cache']['type']['#options'] as $id => $v) {
            if ($id != 'none') {
              unset($form['cache']['type']['#options'][$id]);
            }
            $form['cache']['type']['#description'] = t("Views data export isn't currently compatible with caching plugins.");
          }
        }
        break;

    }
  }

  function get_option($option) {
    return parent::get_option($option);
  }

  /**
   * Save the options from the options form.
   */
  function options_submit(&$form, &$form_state) {
    // It is very important to call the parent function here.
    parent::options_submit($form, $form_state);
  }

  /**
   * Show the link without the path.
   */
  function preview() {
    // Force the view to build.
    $build = $this->view->build();
    $text = check_plain($this->view->display['socrata_data_export_1']->display_options['style_options']['attach_text']);
    return l($text, '');
  }
}

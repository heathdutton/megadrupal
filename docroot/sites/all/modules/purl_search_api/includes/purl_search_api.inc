<?php
/**
 * @file
 * The ctools plugin that manages the Purl processing.
 */

/**
 * SearchAPI processor.
 */
class purl_search_api implements purl_processor {
  /**
   * Sets a method that Purl uses to find this class.
   *
   * @return string
   *   Name of the method.
   */
  public function method() {
    return 'search_api';
  }

  /**
   * Returns the Admin form that manages this plugin.
   *
   * Will appear under the standard Purl Processor settings.
   *
   * @param array $form
   *   The form
   * @param string $id
   *   The id of the Form
   */
  public function admin_form(&$form, $id) {
    // Note that while this form element's key includes the method ("pair"),
    // it will eventually save to the variable purl_method_[id]_key. See
    // element validator for how this occurs.
    $options = $this->condition_values();
    $form['purl_searchapi'] = array(
      '#type' => 'fieldset',
    );
    $form['purl_searchapi']["purl_method_search_api_key"] = array(
      '#title' => t('SearchAPI Setting'),
      '#type' => 'checkboxes',
      '#options' => $options,
      '#size' => 12,
      '#default_value' => variable_get("purl_method_search_api_key", array()),
    );

  }

  /**
   * Detect a default value for in the GET request.
   *
   * Ignores elements drupal uses already.
   */
  public function detect($q) {
    $f = drupal_get_query_parameters($_GET, array(
      'q', 'sort', 'order', 'page', 'pass',
      )
    );
    if (isset($f['f'])) {
      $new = array(
        'f' => $f['f'],
      );
      $query = drupal_http_build_query($new);
      return $query;
    }
    return "";
  }

  /**
   * Returns the description of the processor.
   *
   * @return string
   *   Description
   */
  public function description() {
    return t("Alway always choose the single value 'f'.");
  }

  /**
   * Tear apart the path and iterate thought it looking for valid values.
   *
   * This function will only receive the elements that are called f
   * if the Purl settings link the SearchAPI modifier type to the key f
   */
  public function parse($valid_values, $qs) {
    $elements = array();
    parse_str($qs, $elements);
    $parsed = array();
    if (isset($elements['f'])) {
      $temp = array_flip(array_flip($elements['f']));
      unset($elements['f']);
    }

    foreach ($elements as $k => $v) {
      if (isset($valid_values[$k])) {
        $parsed[$k] = $valid_values[$k];
        $parsed[$k]['id'] = $v;
      }
    }

    if (isset($temp)) {
      $parsed['f'] = array(
        'provider' => 'devtrac_purl_provider',
        'id' => $temp,
      );
      return purl_path_elements($this, $parsed);
    }
  }

  /**
   * No need to do nothing at all.
   */
  public function adjust(&$value, $item, &$q) {
    watchdog ("reinier", "reinier");
  }

  /**
   * Removes specific modifier from a query string.
   *
   * @param array $value
   *   The current querystring.
   * @param array $element
   *   a purl_path_element object
   *
   * @return string
   *   querystring with the modifier removed.
   */
  public function remove($value, $element) {
    $qs = array();
    parse_str($value, $qs);
    unset($qs[$element->id]);
    return drupal_query_string_encode($qs);
  }

  /**
   * Adds all f[] parameters to the selected views.
   *
   * @see purl_search_api_menu_link_alter()
   */
  public function rewrite(&$path, &$options, $element) {
    if (isset($options['attributes']['class'])) {
      if (in_array('purl_search_api_noreset', $options['attributes']['class'])) {
        if (!_purl_skip($element, $options)) {
          $options['query'][$element->value] = $element->id;
        }
      }
    }
  }


/**
   * Generate a list of database and module provided views.
   * Ripped from context_condition_views.inc
   */
  function condition_values() {
    $enabled_views = array();

    $views = views_get_all_views();
    ksort($views);

    foreach ($views as $view) {
      if (!isset($views[$view->name]->disabled) || !$views[$view->name]->disabled) {
        $enabled_views[$view->name] = check_plain($view->name);

        // Provide more granular options for each page display
        $displays = array();
        foreach ($view->display as $id => $display) {
          if ($display->display_plugin != 'block') {
            $displays[$view->name . ":" . $id] = check_plain("-- {$display->display_title}");
          }
        }
        $enabled_views += $displays;
      }
    }
    return $enabled_views;
  }
}

<?php

/**
 * @file
 * Contains the event sub-page display plugin.
 */

/**
 * The plugin that handles an event sub-page.
 *
 * @ingroup views_display_plugins
 */
class commerce_booking_views_plugin_display_event_subpage extends views_plugin_display_page {

  /**
   * Overrides views_plugin_display_page::option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();

    unset($options['menu']['contains']['name']);

    return $options;
  }

  /**
   * Overrides views_plugin_display_page::execute_hook_menu().
   */
  function execute_hook_menu($callbacks) {
    $items = array();
    $path = $this->get_option('path');

    // Add a menu callback for each event entity.
    foreach (array_keys(commerce_booking_get_event_entities()) as $entity_type) {
      // We can only process entities that have a path.
      $entity_info = entity_get_info($entity_type);
      if (isset($entity_info['default path'])) {
        // Replace the autoloader in the path and set the full path.
        $event_path = preg_replace('/%[a-zA-Z0-9]+/', '%', $entity_info['default path']);
        $this->set_option('path', $event_path . '/' . $path);

        // Pass onto the normal page hook menu.
        $event_items = parent::execute_hook_menu($callbacks);

        // Loop over making tweaks to the items.
        foreach ($event_items as $path => $item) {
          // Set up the additional arguments for our callback.
          $path_parts = explode('/', $path);
          $access_arguments = array(
            $this->view->name,
            $this->display->id,
            $entity_type,
            array_search('%', $path_parts),
            $item['access callback'],
          );
          $item['access arguments'] = array_merge($access_arguments, $item['access arguments']);

          // Use out own access callback.
          $item['access callback'] = 'commerce_booking_views_access';

          // Adjust the page arguments.
          array_splice($item['page arguments'], 2, 0, $entity_type);

          // Add our item to the menu.
          $items[$path] = $item;
        }
      }
    }

    // Set our path back.
    $this->set_option('path', $path);

    return $items;
  }

  /**
   * Overrides views_plugin_display_page::options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    switch ($form_state['section']) {
      case 'path':
        $form['path']['#description'] = t('This view will be displayed at this sub-path under the event entity.');
        $form['path']['#field_prefix'] .= '[event entity url]';
        break;
      case 'menu':
        unset($form['menu']['type']['#options']['normal']);
        unset($form['menu']['name']);
        break;
      case 'tab_options':
        unset($form['tab_options']['type']['#options']['normal']);
        unset($form['tab_options']['name']);
        break;
    }
  }

  /**
   * Overrides views_plugin_display_page::get_argument_text().
   */
  function get_argument_text() {
    $text = parent::get_argument_text();
    $text['description'] .= ' ' . t('The first two arguments will be entity type and entity id.');
    return $text;
  }

  /**
   * Overrides views_plugin_display_page::execute().
   */
  function execute() {
    $render = parent::execute();

    // Update the title.
    $entity = entity_load_single($this->view->args[0], $this->view->args[1]);
    $label = entity_label($this->view->args[0], $entity);
    drupal_set_title($label);

    return $render;
  }

  /**
   * Overrides views_plugin_display_plugin::has_path().
   */
  function has_path() {
    return (bool) $this->get_event_path();
  }

  /**
   * Overrides views_plugin_display::get_path().
   */
  function get_path() {
    if ($path = $this->get_event_path()) {
      $path .= '/' . parent::get_path();
      return $path;
    }
  }

  /**
   * Overrides views_plugin_display::get_url().
   */
  function get_url() {
    $args = array_slice($this->view->args, 2);
    return $this->view->get_url($args);
  }

  /**
   * Get the URL of the entity from the given arguemnts.
   *
   * @return string|NULL
   *   Either a path string or NULL if there isn't one.
   */
  protected function get_event_path() {
    // Check we have our arguments.
    if (count($this->view->args) < 2) {
      return NULL;
    }

    // Get the default path for this entity.
    list($entity_type, $entity_id) = $this->view->args;
    $entity = entity_load_single($entity_type, $entity_id);
    $uri = entity_uri($entity_type, $entity);
    return $uri ? $uri['path'] : NULL;
  }
}

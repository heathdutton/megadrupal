<?php

class services_client_ui extends ctools_export_ui {

  /**
   * Page callback; Basic configuration page.
   */
  function configure_page($js, $input, $item) {
    drupal_set_title($this->get_page_title('configure', $item));
    $event = $this->get_event_handler($item, TRUE);
    return drupal_get_form('services_client_event_config', $event);
  }

  /**
   * Page callback; Add new plugin.
   */
  function add_plugin_page($js, $input, $item, $type) {
    drupal_set_title($this->get_page_title('add_plugin', $item));
    $event = $this->get_event_handler($item, TRUE);
    if ($type == 'mapping') {
      $uuid = $event->addPlugin('mapping', 'ServicesClientMappingPlugin');
      $event->setObjectCache();
      drupal_goto($event->getUrl('plugin/mapping/' . $uuid . '/edit'));
    }
    return drupal_get_form('services_client_plugin_add', $event, $type);
  }

  /**
   * Page callback; Configure plugin.
   */
  function configure_plugin_page($js, $input, $item, $type, $uuid) {
    $event = $this->get_event_handler($item, TRUE);
    $plugin = $event->getPlugin($type, $uuid);
    return drupal_get_form('services_client_plugin_edit', $event, $type, $uuid, $plugin);
  }

  /**
   * Page callback; Remove existing plugin.
   */
  function remove_plugin_page($js, $input, $item, $type, $uuid) {
    $event = $this->get_event_handler($item, TRUE);

    // Check if valid token is provided as there is no confirmation form here.
    if (!isset($_GET['token']) || !drupal_valid_token($_GET['token'], $uuid)) {
      drupal_set_message(t('Invalid token'), 'error');
      return MENU_ACCESS_DENIED;
    }
    else {
      $event->removePlugin($type, $uuid);
      $event->setObjectCache();
      drupal_set_message(t('Plugin was removed.'));
    }

    // Redirect user back to configuration page.
    drupal_goto($event->getUrl('configure'));
  }

  /**
   * Page callback; Break edit lock.
   */
  function break_lock_page($js, $input, $item) {
    drupal_set_title($this->get_page_title('configure', $item));
    $event = $this->get_event_handler($item, TRUE);
    return drupal_get_form('services_client_event_break_lock', $event);
  }

  /**
   * Retrieve event handler by event configuration.
   *
   * @param stdClass $item
   *   Export event object.
   *
   * @param boolean $object_cache
   *   TRUE if should be loaded from ctools object cache (for configuratin pages).
   *
   * @return EventHandler
   *   Instance of EventHandler.
   */
  function get_event_handler($item, $object_cache = FALSE) {
    $event = $item->getHandler();
    if ($object_cache) {
      return $event->objectCached();
    }
    return $event;
  }

  /**
   * Provide a list of sort options.
   *
   * Override this if you wish to provide more or change how these work.
   * The actual handling of the sorting will happen in build_row().
   */
  function list_sort_options() {
    if (!empty($this->plugin['export']['title'])) {
      $options = array(
        'disabled' => t('Enabled, title'),
        $this->plugin['export']['title'] => t('Title'),
      );
    }
    else {
      $options = array(
        'disabled' => t('Enabled, name'),
      );
    }

    $options += array(
      'name' => t('Name'),
      'storage' => t('Storage'),
      'connection' => t('Connection'),
    );

    return $options;
  }

  /**
   * Build a row based on the item.
   *
   * By default all of the rows are placed into a table by the render
   * method, so this is building up a row suitable for theme('table').
   * This doesn't have to be true if you override both.
   */
  function list_build_row($item, &$form_state, $operations) {
    // Set up sorting
    $name = $item->{$this->plugin['export']['key']};
    $schema = ctools_export_get_schema($this->plugin['schema']);

    // Note: $item->{$schema['export']['export type string']} should have already been set up by export.inc so
    // we can use it safely.
    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->sorts[$name] = empty($item->disabled) . $name;
        break;
      case 'title':
        $this->sorts[$name] = $item->{$this->plugin['export']['title']};
        break;
      case 'name':
        $this->sorts[$name] = $name;
        break;
      case 'connection':
        $this->sorts[$name] = $item->connection;
        break;
      case 'storage':
        $this->sorts[$name] = $item->{$schema['export']['export type string']} . $name;
        break;
    }

    $this->rows[$name]['data'] = array();
    $this->rows[$name]['class'] = !empty($item->disabled) ? array('ctools-export-ui-disabled') : array('ctools-export-ui-enabled');

    // If we have an admin title, make it the first row.
    $connection = services_client_connection_load($item->connection);
    $this->rows[$name]['data'][] = array('data' => check_plain($item->title), 'class' => array('ctools-export-ui-title'));
    $this->rows[$name]['data'][] = array('data' => check_plain($name), 'class' => array('ctools-export-ui-name'));
    $this->rows[$name]['data'][] = array('data' => l($connection->admin_title, 'admin/structure/services_client/connection/list/' . $item->connection . '/edit', array('query' => drupal_get_destination())), 'class' => array('ctools-export-ui-connection'));
    $this->rows[$name]['data'][] = array('data' => check_plain($item->entity_type), 'class' => array('ctools-export-ui-entity-type'));
    $this->rows[$name]['data'][] = array('data' => check_plain($item->event), 'class' => array('ctools-export-ui-event'));
    $this->rows[$name]['data'][] = array('data' => check_plain($item->plugin), 'class' => array('ctools-export-ui-handler'));
    $this->rows[$name]['data'][] = array('data' => check_plain($item->{$schema['export']['export type string']}), 'class' => array('ctools-export-ui-storage'));

    $ops = theme('links', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));

    $this->rows[$name]['data'][] = array('data' => $ops, 'class' => array('ctools-export-ui-operations'));

    // Add an automatic mouseover of the description if one exists.
    if (!empty($this->plugin['export']['admin_description'])) {
      $this->rows[$name]['title'] = $item->{$this->plugin['export']['admin_description']};
    }
  }

  /**
   * Provide the table header.
   *
   * If you've added columns via list_build_row() but are still using a
   * table, override this method to set up the table header.
   */
  function list_table_header() {
    $header = array();

    $header[] = array('data' => t('Title'), 'class' => array('ctools-export-ui-title'));
    $header[] = array('data' => t('Name'), 'class' => array('ctools-export-ui-name'));
    $header[] = array('data' => t('Connection'), 'class' => array('ctools-export-ui-connection'));
    $header[] = array('data' => t('Entity Type'), 'class' => array('ctools-export-ui-entity-type'));
    $header[] = array('data' => t('Event'), 'class' => array('ctools-export-ui-event'));
    $header[] = array('data' => t('Handler'), 'class' => array('ctools-export-ui-handler'));
    $header[] = array('data' => t('Storage'), 'class' => array('ctools-export-ui-storage'));
    $header[] = array('data' => t('Operations'), 'class' => array('ctools-export-ui-operations'));

    return $header;
  }

}

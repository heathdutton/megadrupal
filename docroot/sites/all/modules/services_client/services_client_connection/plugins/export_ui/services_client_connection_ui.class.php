<?php

class services_client_connection_ui extends ctools_export_ui {
  /**
   * Page callback for the auth page.
   */
  function auth_page($js, $input, $item) {
    drupal_set_title($this->get_page_title('edit', $item));
    return drupal_get_form('services_client_connection_plugin_config', 'auth', $item);
  }

  /**
   * Page callback for the server page.
   */
  function server_page($js, $input, $item) {
    drupal_set_title($this->get_page_title('edit', $item));
    return drupal_get_form('services_client_connection_plugin_config', 'server', $item);
  }


  /**
   * Page callback for the authentication page.
   */
  function request_page($js, $input, $item) {
    drupal_set_title($this->get_page_title('edit', $item));
    return drupal_get_form('services_client_connection_plugin_config', 'request', $item);
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
        $this->sorts[$name] = $item->{$this->plugin['export']['admin_title']};
        break;
      case 'name':
        $this->sorts[$name] = $name;
        break;
      case 'storage':
        $this->sorts[$name] = $item->{$schema['export']['export type string']} . $name;
        break;
    }

    $this->rows[$name]['data'] = array();
    $this->rows[$name]['class'] = !empty($item->disabled) ? array('ctools-export-ui-disabled') : array('ctools-export-ui-enabled');

    // If we have an admin title, make it the first row.
    if (!empty($this->plugin['export']['admin_title'])) {
      $this->rows[$name]['data'][] = array('data' => check_plain($item->{$this->plugin['export']['admin_title']}), 'class' => array('ctools-export-ui-title'));
    }
    $this->rows[$name]['data'][] = array('data' => check_plain($name), 'class' => array('ctools-export-ui-name'));
    $this->rows[$name]['data'][] = array('data' => check_plain($item->{$schema['export']['export type string']}), 'class' => array('ctools-export-ui-storage'));

    $ops = theme('links', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));

    $this->rows[$name]['data'][] = array('data' => $ops, 'class' => array('ctools-export-ui-operations'));

    // Add an automatic mouseover of the description if one exists.
    if (!empty($this->plugin['export']['admin_description'])) {
      $this->rows[$name]['title'] = $item->{$this->plugin['export']['admin_description']};
    }
  }
}

/**
 * Plugin configuration form
 */
function services_client_connection_plugin_config($form, &$form_state, $type, $item) {
  // Get plugin name and configuration
  $name = $item->config[$type]['plugin'];
  $config = $item->config[$type]['config'];

  // Get new plugin
  $plugin = services_client_connection_get_plugin($type, $name, $item, $config);

  // Run config form function
  $plugin->configForm($form, $form_state);

  $form_state += array(
    'type' => $type,
    'item' => $item,
    'plugin' => $plugin,
    'config' => $config
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );

  return $form;
}

/**
 * Validate plugin configuration
 */
function services_client_connection_plugin_config_validate($form, &$form_state) {
  $plugin = $form_state['plugin'];
  $plugin->configFormValidate($form, $form_state);
}

/**
 * Save plugin configuration
 */
function services_client_connection_plugin_config_submit($form, &$form_state) {
  $plugin = $form_state['plugin'];

  $plugin->configFormSubmit($form, $form_state);

  $item = $form_state['item'];
  $item->config[$form_state['type']]['config'] = $form_state['config'];

  $result = ctools_export_crud_save('services_client_connection', $item);

  if ($result) {
    drupal_set_message(t('Configuration has been saved.'));
  }
}

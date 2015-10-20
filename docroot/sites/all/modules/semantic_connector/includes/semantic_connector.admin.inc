<?php

/**
 * @file
 * Administration functionality for the PoolParty Semantic Connector.
 */

/**
 * Show an overview of all the available semantic Drupal modules.
 *
 * @return string
 *   The rendered HTML of the module overview.
 */
function semantic_connector_overview() {
  $installed_modules = module_list();
  $semantic_modules = array(
    'powertagging' => array(
      'title' => 'PowerTagging',
      'configuration_path' => 'admin/config/services/powertagging',
    ),
    /*'skos_importer' => array(
      'title' => 'SKOS Importer',
      'configuration_path' => 'admin/config/services/skos_importer',
    ),*/
    'smart_glossary' => array(
      'title' => 'Smart Glossary',
      'configuration_path' => 'admin/config/services/smart_glossary',
    ),
    'sonr_webmining' => array(
      'title' => 'sOnr webMining Content Aggregator',
      'configuration_path' => 'admin/config/services/sonr_webmining',
    ),
  );

  $headers = array(t('Module'), t('Installed'), t('Configuration'));

  $module_rows = array();
  $installed_semantic_modules = array();
  foreach ($semantic_modules as $module_key => $module_info) {
    $installed = in_array($module_key, $installed_modules);
    $module_rows[] = array(
      $module_info['title'],
      ($installed ? 'installed' : l(t('Download'), 'http://www.drupal.org/project/' . $module_key)),
      ($installed ? l(t('Configure'), $module_info['configuration_path']) : ''),
    );

    // Create a list of connections used by module and connection-id.
    if ($installed) {
      $installed_semantic_modules[] = $module_key;
    }
  }

  $output = theme('table', array(
    'header' => $headers,
    'rows' => $module_rows,
    'attributes' => array(),
    'caption' => NULL,
    'colgroups' => array(),
    'sticky' => FALSE,
    'empty' => '',
    'attributes' => array('style' => 'margin-top:20px;'),
  ));

  // Find out what connections are used by the installed semantic modules.
  $connections_used = semantic_connector_check_connection_usage($installed_semantic_modules);

  $pp_server_connections = SemanticConnector::getConnectionsByType('pp_server');
  $sparql_endpoint_connections = SemanticConnector::getConnectionsByType('sparql_endpoint');
  $sparql_endpoint_connections_assoc = array();
  foreach ($sparql_endpoint_connections as $sparql_endpoint_connection) {
    $sparql_endpoint_connections_assoc[$sparql_endpoint_connection->getURL()] = $sparql_endpoint_connection;
  }
  $sparql_endpoint_connections = $sparql_endpoint_connections_assoc;
  unset($sparql_endpoint_connections_assoc);

  foreach ($pp_server_connections as $pp_server_connection) {
    $connection_content = '';
    $server_config = $pp_server_connection->getConfig();
    $connection_content .= '<div class="pp-server-connection-header-content">';
    $connection_content .= '<h2 class="pp-server-connection-title">' . t('PoolParty Server "%pptitle"', array('%pptitle' => $pp_server_connection->getTitle())) . '</h2>';
    $connection_content .= '<div class="pp-server-connection-url">' . t('Url') . ': <i>' . l($pp_server_connection->getURL(), $pp_server_connection->getURL(), array('attributes' => array('target' => array('_blank')))) . '</i></div>';
    $connection_content .= '</div>';
    $connection_content .= '<div class="pp-server-connection-main-content">';

    if (isset($server_config['projects']) && !empty($server_config['projects'])) {
      foreach ($server_config['projects'] as $project) {
        if (isset($project->sparql_endpoint_url) && isset($sparql_endpoint_connections[$project->sparql_endpoint_url])) {
          $project_content = '';
          $project_content .= '<div class="pp-server-connection-project">';
          $project_content .= '<div class="pp-server-connection-project-header-content">';
          $project_content .= '<h3 class="pp-server-connection-project-title">' . $project->title . '</h3>';
          $project_content .= '</div>';
          $project_content .= '<div class="pp-server-connection-project-main-content">';
          $project_content .= '<div class="pp-server-connection-project-id">ID: <i>' . $project->id . '</i></div>';
          $uses = _semantic_connector_theme_connection_uses($sparql_endpoint_connections[$project->sparql_endpoint_url]->getID(), $connections_used, $semantic_modules);
          $project_content .= '<div class="pp-sparql-endpoint-connection">' . t('SPARQL-Endpoint') . ': <i>' . l($sparql_endpoint_connections[$project->sparql_endpoint_url]->getTitle(), $project->sparql_endpoint_url, array('attributes' => array('target' => array('_blank')))) . '</i></div>';
          $project_content .= '</div>';
          $project_content .= '<div class="pp-server-connection-project-footer-content">';
          $project_content .= _semantic_connector_theme_connection_buttons($sparql_endpoint_connections[$project->sparql_endpoint_url], empty($uses));
          $project_content .= '</div>';
          $project_content .= '<div class="pp-server-connection-project-footer-uses-content">';
          $project_content .= $uses;
          $project_content .= '</div>';
          $project_content .= '</div>';

          $connection_content .= $project_content;
          unset($sparql_endpoint_connections[$project->sparql_endpoint_url]);
        }
      }
    }

    $connection_content .= '<div class="clearBoth"></div>';
    $connection_content .= '</div>';
    $connection_content .= '<div class="pp-server-connection-footer-content">';
    $uses = _semantic_connector_theme_connection_uses($pp_server_connection->getID(), $connections_used, $semantic_modules);
    $connection_content .= _semantic_connector_theme_connection_buttons($pp_server_connection, empty($uses));
    $connection_content .= '</div>';
    $connection_content .= '<div class="pp-server-connection-footer-uses-content">';
    $connection_content .= $uses;
    $connection_content .= '</div>';
    $output .= '<div class="pp-server-connection">' . $connection_content . '</div>';
  }

  if (!empty($sparql_endpoint_connections)) {
    $custom_connections_content = '';
    $custom_connections_content .= '<div class="pp-server-connection-header-content">';
    $custom_connections_content .= '<h2 class="pp-server-connection-title">' . t('Custom Connections') . '</h2>';
    $custom_connections_content .= '<div class="pp-server-connection-category-title">' . t('SPARQL Endpoints') . '</div>';
    $custom_connections_content .= '</div>';
    $custom_connections_content .= '<div class="pp-server-connection-main-content">';

    // Add all the custom SPARQL endpoints.
    foreach ($sparql_endpoint_connections as $sparql_endpoint_connection) {
      $uses = _semantic_connector_theme_connection_uses($sparql_endpoint_connection->getID(), $connections_used, $semantic_modules);
      $custom_connections_content .= '<div class="pp-server-connection-custom">';
      $custom_connections_content .= '<div class="pp-server-connection-custom-header-content">';
      $custom_connections_content .= l($sparql_endpoint_connection->getTitle(), $sparql_endpoint_connection->getURL(), array('attributes' => array('target' => array('_blank'))));
      $custom_connections_content .= '</div>';
      $custom_connections_content .= '<div class="pp-server-connection-custom-main-content">';
      $custom_connections_content .= $uses;
      $custom_connections_content .= '</div>';
      $custom_connections_content .= '<div class="pp-server-connection-custom-footer-content">';
      $custom_connections_content .= _semantic_connector_theme_connection_buttons($sparql_endpoint_connection, empty($uses));
      $custom_connections_content .= '</div>';
      $custom_connections_content .= '</div>';
    }
    $custom_connections_content .= '<div class="clearBoth"></div>';
    $custom_connections_content .= '</div>';

    // Do the same for sOnr here.
    $output .= '<div class="pp-server-connection">' . $custom_connections_content . '</div>';
  }

  return $output;
}

/**
 * Theme the module configurations, that use a given connection.
 *
 * @param int $connection_id
 *   The ID of the Semantic Connector connection to theme the uses for.
 * @param array $used_connections
 *   An associative array of connection uses, categorized by the connection ID.
 * @param array $module_list
 *   An array of information for every semantic module.
 *
 * @return string
 *   The rendered HTML.
 */
function _semantic_connector_theme_connection_uses($connection_id, array $used_connections, array $module_list) {
  $output = '';

  if (isset($used_connections[$connection_id])) {
    $output = '<div class="semantic-connector-configuration-list"><div class="header">Used by:</div><ul>';
    foreach ($used_connections[$connection_id] as $type => $configurations) {
      foreach ($configurations as $configuration) {
        $output .= '<li>' . $module_list[$type]['title'] . ' configuration ' . l($configuration['title'], $module_list[$type]['configuration_path'] . '/' . $configuration['id']) . '</li>';
      }
    }
    $output .= '</ul></div>';
  }

  return $output;
}

/**
 * Theme buttons to edit or delete a Semantic Connector connection.
 *
 * @param object $connection
 *   The Semantic Connector connection to theme the buttons for.
 * @param bool $can_be_deleted
 *   Whether a delete-button should be added or not.
 *
 * @return string
 *   The rendered HTML.
 */
function _semantic_connector_theme_connection_buttons($connection, $can_be_deleted = FALSE) {
  $output = '<div class="semantic-connector-connection-buttons">';

  // Edit-button.
  $output .= l(t('Edit'), 'admin/config/services/semantic_connector/connections/' . $connection->getType() . '/' . $connection->getID(), array('attributes' => array('class' => array('semantic-connector-connection-buttons-edit'))));

  // Delete button.
  if ($can_be_deleted) {
    $output .= '|' . l(t('Delete'), 'admin/config/services/semantic_connector/connections/' . $connection->getType() . '/' . $connection->getID() . '/delete', array('attributes' => array('class' => array('semantic-connector-connection-buttons-delete'))));
  }

  // Refresh projects button
  if ($connection->getType() == 'pp_server') {
    $output .= '|' . l(t('Refresh projects'), 'admin/config/services/semantic_connector/connections/' . $connection->getType() . '/' . $connection->getID() . '/refresh_projects', array('attributes' => array('class' => array('semantic-connector-connection-buttons-refresh-projects'))));
  }

  $output .= '</div>';
  return $output;
}

/**
 * The form to edit an existing Semantic Connector connection.
 */
function semantic_connector_connection_edit_form($form, &$form_state, $type, $connection_id) {
  $connection = SemanticConnector::getConnection($type, $connection_id);

  if (!is_null($connection)) {
    $form['server_settings']['connection_id'] = array(
      '#type' => 'hidden',
      '#value' => $connection_id,
    );

    $form['server_settings']['type'] = array(
      '#type' => 'hidden',
      '#value' => $type,
    );

    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Server title'),
      '#description' => t('A short title for the connection.'),
      '#size' => 35,
      '#maxlength' => 60,
      '#default_value' => $connection->getTitle(),
      '#required' => TRUE,
    );

    $form['url'] = array(
      '#type' => 'textfield',
      '#title' => t('URL'),
      '#description' => t('URL of the connection.'),
      '#size' => 35,
      '#maxlength' => 255,
      '#default_value' => $connection->getURL(),
      '#required' => TRUE,
    );

    $credentials = $connection->getCredentials();
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Username'),
      '#description' => t('Name of a user for the credentials.'),
      '#size' => 35,
      '#maxlength' => 60,
      '#default_value' => $credentials['username'],
    );

    $form['pass'] = array(
      '#type' => 'textfield',
      '#title' => t('Password'),
      '#description' => t('Password of a user for the credentials.'),
      '#size' => 35,
      '#maxlength' => 128,
      '#default_value' => $credentials['password'],
    );

    $form['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }
  else {
    // todo: throw error.
  }

  return $form;
}

/**
 * Submit handler for powertagging_form.
 */
function semantic_connector_connection_edit_form_submit($form, &$form_state) {
  SemanticConnector::createConnection($form_state['values']['type'], $form_state['values']['url'], $form_state['values']['title'], array(
    'username' => $form_state['values']['name'],
    'password' => $form_state['values']['pass'],
  ), array(), $form_state['values']['connection_id']);

  drupal_set_message(t('Successfully updated connection "%contitle".', array('%contitle' => $form_state['values']['title'])));
  drupal_goto('admin/config/services/semantic_connector');
}

/**
 * Delete a Semantic Connector connection.
 *
 * @param int $connection_id
 *   The ID of the Semantic Connector connection to delete.
 * @param bool $is_user_interaction
 *   Handle this as a user interaction? If TRUE, a Drupal messages regarding the
 *   success of this operation and a goto will be included.
 *
 * @return bool
 *   TRUE if the operation succeeded, FALSE if not.
 */
function semantic_connector_connection_delete($connection_id, $is_user_interaction = FALSE) {
  $success = FALSE;

  // Delete the connection if it is not required by any configuration.
  $connection_usage = semantic_connector_check_connection_usage();
  if (!isset($connection_usage[$connection_id]) || empty($connection_usage[$connection_id])) {
    SemanticConnector::deleteConnections(array($connection_id));
    $success = TRUE;
  }

  if ($is_user_interaction) {
    if ($success) {
      drupal_set_message(t('The connection was deleted successfully.'));
    }
    else {
      drupal_set_message(t('The connection is used by a module and therefore could not be deleted.'), 'error');
    }
    drupal_goto('admin/config/services/semantic_connector');
  }

  return $success;
}

/**
 * Refresh the projects of a Semantic Connector Poolparty server connection.
 *
 * All depending connections (SPARQL-endpoints) get recreated or updated by
 * resaving the connection.
 *
 * @param $connection_id
 *   The ID of the Semantic Connector PoolParty server connection to refresh the
 *   projects for.
 */
function semantic_connector_connection_refresh_projects($connection_id) {
  $connection = SemanticConnector::getConnection('pp_server', $connection_id);
  if (!is_null($connection)) {
    $connection->save();
    drupal_set_message(t('Successfully refreshed the projects of PoolParty server connection "%contitle".', array('%contitle' => $connection->getTitle())));
  }
  else {
    drupal_set_message(t('Refreshing failed due to an invalid connection ID.'));
  }

  drupal_goto('admin/config/services/semantic_connector');
}

/**
 * Check what Semantic Connector connections are used by which module.
 *
 * @param array $modules_to_check
 *   An array of module keys to check for connections.
 *
 * @return array
 *   Associative array of connections usages, categorized by connection_id and
 *   then by module_key.
 */
function semantic_connector_check_connection_usage(array $modules_to_check = array(
  'powertagging',
  'smart_glossary',
  'sonr_webmining',
)) {
  $connections_used = array();

  foreach ($modules_to_check as $module_key) {
    if (module_exists($module_key)) {
      switch ($module_key) {
        case 'powertagging':
          foreach (powertagging_config_load() as $config) {
            if (!isset($connections_used[$config->connection_id])) {
              $connections_used[$config->connection_id] = array();;
            }
            if (!isset($connections_used[$config->connection_id][$module_key])) {
              $connections_used[$config->connection_id][$module_key] = array();
            }
            $connections_used[$config->connection_id][$module_key][] = array(
              'id' => $config->powertagging_id,
              'title' => $config->title,
            );
          }
          break;

        case 'smart_glossary':
          foreach (smart_glossary_config_load() as $config) {
            if (!isset($connections_used[$config->connection_id])) {
              $connections_used[$config->connection_id] = array();;
            }
            if (!isset($connections_used[$config->connection_id][$module_key])) {
              $connections_used[$config->connection_id][$module_key] = array();
            }
            $connections_used[$config->connection_id][$module_key][] = array(
              'id' => $config->sg_id,
              'title' => $config->title,
            );
          }
          break;

        case 'sonr_webmining':
          break;
      }
    }
  }

  return $connections_used;
}

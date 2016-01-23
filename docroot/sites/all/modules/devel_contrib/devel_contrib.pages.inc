<?php
/**
 * @file devel_contrib.pages.inc
 * Contains page callbacks.
 */

/**
 * Generic page callback for info hooks.
 *
 * @param $info_hooks
 *  An array whose keys are the names of info functions to call, and whose
 *  values are labels to show with the data from each function.
 *  For example: entity_get_info => 'Entity info'.
 */
function devel_contrib_info_page($info_hooks) {
  $output = '';

  foreach ($info_hooks as $function => $label) {
    $info = $function();
    $output .= kprint_r($info, TRUE, $label);
  }

  return $output;
}

/**
 * Page callback for the bundle info page.
 */
function devel_contrib_field_info_page($entity_type, $bundle) {
  $output = kprint_r($bundle, TRUE, t('Bundle'));

  return $output;
}

/**
 * Page callback for permissions info.
 */
function devel_contrib_permissions_info_page() {
  $output = '';

  $roles = user_roles();
  $output .= kprint_r($roles, TRUE, t('Roles'));

  $modules = array();
  foreach (module_implements('permission') as $module) {
    $modules[] = $module;
  }
  asort($modules);

  foreach ($modules as $module) {
    if ($permissions = module_invoke($module, 'permission')) {
      $info[$module] = $permissions;
    }
  }

  $output .= kprint_r($info, TRUE, t('All permissions'));

  $info = user_role_permissions($roles);
  $output .= kprint_r($info, TRUE, t('Permissions by role'));

  return $output;
}

/**
 * Page callback for the hook info page.
 */
function devel_contrib_hook_info_page() {
  $implementations = cache_get('module_implements', 'cache_bootstrap');

  $output = t('Registered hook implementations. This may not contain data on all hooks, only those which have been cached so far.');

  if (isset($implementations->data)) {
    $output .= kprint_r($implementations->data, TRUE, t('Registered hook implementations'));
  }

  return $output;
}

/**
 * Page callback for devel menu item: Token.
 */
function devel_contrib_token_info_page() {
  $output = theme('token_tree', array(
    'token_types' => 'all',
  ));

  return $output;
}

/**
 * Page callback for devel menu item: CTools.
 */
function devel_contrib_ctools_info_page() {
  ctools_include('plugins');
  $plugin_types_info = ctools_plugin_get_plugin_type_info();
  $output = kprint_r($plugin_types_info, TRUE, t('CTools plugin types'));

  $plugins = array();
  foreach ($plugin_types_info as $module => $module_info) {
    foreach ($module_info as $type => $plugin_info) {
      $plugins[$module][$type] = ctools_get_plugins($module, $type);
    }
  }

  $output .= kprint_r($plugins, TRUE, t('CTools plugins'));

  return $output;
}

/**
 * Page callback for devel menu item.
 */
function devel_contrib_views_info_page() {
  $info = views_fetch_data();
  $output = kprint_r($info, TRUE, t('Views data'));

  $info = views_fetch_plugin_data();
  $output .= kprint_r($info, TRUE, t('Views plugins'));

  return $output;
}

/**
 * Devel tab on a single View.
 *
 * @param $view
 *  The View object.
 */
function devel_contrib_views_devel_page($view) {
  $output = kprint_r($view, TRUE);
  return $output;
}

/**
 * Page callback for devel menu item: Entity API.
 */
function devel_contrib_entity_info_page() {
  $info = entity_get_property_info();
  $output = kprint_r($info, TRUE, t('Entity API property info'));

  return $output;
}

/**
 * Page callback for devel menu item: VBO.
 */
function devel_contrib_vbo_info_page() {
  $info = views_bulk_operations_get_operation_types();
  $output = kprint_r($info, TRUE, t('VBO operation types'));

  $info = views_bulk_operations_get_operation_info();
  $output .= kprint_r($info, TRUE, t('VBO operations info'));

  return $output;
}

/**
 * Page callback for devel menu item: Features.
 */
function devel_contrib_features_page() {
  $info = features_get_components();
  $output = kprint_r($info, TRUE, t('Features components'));

  $components = array();
  foreach (features_get_feature_components() as $source => $info) {
    if ($options = features_invoke($source, 'features_export_options')) {
      foreach ($options as $name => $title) {
        $components[$source][$name] = $title;
      }
    }
  }
  $output .= kprint_r($components, TRUE, t('Features exportables'));

  return $output;
}

/**
 * Page callback for devel menu item: OG.
 */
function devel_contrib_og_page() {
  $info = og_fields_info();
  $output = kprint_r($info, TRUE, t('OG Field Info'));

  $info = og_get_all_group_bundle();
  $output .= kprint_r($info, TRUE, t('OG Group types'));

  $info = og_get_all_group_content_bundle();
  $output .= kprint_r($info, TRUE, t('OG Group content types'));

  $info = og_context_negotiation_info();
  $output .= kprint_r($info, TRUE, t('OG Group context providers'));

  // There is no API function to get all OG roles!
  $query = db_select('og_role', 'ogr')
    ->fields('ogr')
    ->condition('gid', 0, '=')
    ->orderBy('rid', 'ASC');

  $roles = $query
    ->execute()
    ->fetchAllAssoc('rid');
  $output .= kprint_r($roles, TRUE, t('OG Roles data (raw query, no API)'));

  $info = og_get_permissions();
  $output .= kprint_r($info, TRUE, t('OG permissions'));

  return $output;
}

/**
 * Page callback for devel menu item: Commerce order statuses.
 */
function devel_contrib_commerce_order_states_info_page() {
  $states = commerce_order_states();
  foreach ($states as $name => $state) {
    $states[$name]['statues'] = commerce_order_statuses(array('state' => $name));
  }

  $output = kprint_r($states, TRUE, t('Commerce order states and statuses'));

  return $output;
}

/**
 * Page callback for devel menu item: Commerce checkout panes.
 */
function devel_contrib_commerce_checkout_panes_info_page() {
  $panes = commerce_checkout_panes();

  $output = kprint_r($panes, TRUE, t('Commerce checkout panes'));

  return $output;
}

/**
 * Page callback for devel menu item.
 */
function devel_contrib_flag_info_page() {
  $info = flag_fetch_definition();
  $output = kprint_r($info, TRUE, t('Flag types'));

  $info = flag_get_link_types();
  $output .= kprint_r($info, TRUE, t('Flag link types'));

  $info = flag_get_flags();
  $output .= kprint_r($info, TRUE, t('Defined flags'));

  return $output;
}

/**
 * Page callback for devel menu item: Services
 */
function devel_contrib_services_info_page() {
  $resources = services_get_resources();

  $output = kprint_r($resources, TRUE, t('Services resources'));

  return $output;
}

/**
 * Clients info.
 */
function devel_contrib_clients_info_page() {
  $info = clients_get_connection_types();
  $output = kprint_r($info, TRUE, t('Client connection types'));

  $info = clients_get_resource_types();
  $output .= kprint_r($info, TRUE, t('Client resource types'));

  return $output;
}

/**
 *
 */
function devel_contrib_migrate_info_page() {
  $info = migrate_migrations();
  $output = kprint_r($info, TRUE, t('Migrations'));

  return $output;
}

/**
 * Page callback for devel menu item: FacetAPI info.
 */
function devel_contrib_facetapi_info_page() {
  $searchers = facetapi_get_searcher_info();
  $output = kprint_r($searchers, TRUE, t('Searchers'));

  $info = facetapi_get_realm_info();
  $output .= kprint_r($info, TRUE, t('Realms'));

  foreach ($searchers as $searcher) {
    $info = facetapi_get_facet_info($searcher['name']);
    $output .= kprint_r($info, TRUE, t('Facets for @searcher', array('@searcher' => $searcher['label'])));
  }

  return $output;
}

/**
 * Page callback for devel menu item: Remote Entity table info.
 */
function devel_contrib_remote_entity_info_page() {
  $output = '';

  $resources = clients_resource_load_all('remote_entity');
  $output .= kprint_r($resources, TRUE, t('Remote entity resources'));

  // Build a list of relevant connections.
  $connections = array();
  foreach ($resources as $resource) {
    $connections[$resource->connection] = $resource->connection;
  }

  // Show the table info for each connection.
  foreach ($connections as $connection) {
    $table_info = remote_entity_get_query_table_info($connection);
    $output .= kprint_r($table_info, TRUE, t("Remote table info for connection '@connection'", array(
      '@connection' => $connection,
    )));
  }

  // Show the property shadowing for each remote entity.
  $info = array();
  $property_info = entity_get_property_info();
  foreach ($resources as $resource) {
    $entity_type = $resource->component;
    foreach ($property_info[$entity_type]['properties'] as $property_name => $property) {
      if (isset($property['remote property shadowing'])) {
        $info[$entity_type][$property_name] = $property;
      }
    }
  }
  $output .= kprint_r($info, TRUE, t('Remote shadowed entity properties'));

  return $output;
}

/**
 * Page callback for devel menu item: Entity Operations info.
 */
function devel_contrib_entity_operations_info_page() {
  $op_info = entity_operations_get_operation_info();
  $output = kprint_r($op_info, TRUE, t('Entity Operation info'));
  return $output;
}

<?php

/**
* @file
* Provide views data and handlers for statistics.module
*/
/**
* @defgroup views_statistics_module statistics.module handlers
*
* Includes the ability to create views of just the statistics table.
* @{
*/

/**
 * Implements hook_views_data().
 */
function osf_views_views_data() {
  // Define the table group for this module
  // This is what will appear in the list of "Show" in the first window of Views
  $data['osf_views_data']['table']['group'] = t('OSF');

  // Define the query plugin to use for this data source
  $data['osf_views_data']['table']['base'] = array(
    'field' => 'id',
    'title' => t('OSF Data'),
    'help' => t('All kind of data coming from a OSF instance'),
    'query class' => 'osf_views_query',
  );  

  // Define the query plugin to use for this data source
  
  // Here we defined the built-in fields, filters and sorts.
  $data['osf_views_data']['type'] = array(
    'title' => t('Type'),
    'help' => t('Type of the records to view'),
    'field' => array(
      'handler' => 'osf_views_field_osf_type',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'osf_views_filter_osf_type',
    ),
    'argument' => array(
      'field' => 'type',
      'handler' => 'osf_views_handler_argument_type',
    ),
  );

  // Define a keyword filter that we can use to search PrefLabel and Description fields.
  $data['osf_views_data']['keyword'] = array(
    'title' => t('Keyword (Description)'),
    'help' => t('Search the text of a description with a keyword'),
    'field' => array(
      'handler' => 'osf_views_field_keyword',
    ),
    'argument' => array(
      'handler' => 'osf_views_handler_argument_keyword',
    ),
  );

  $data['osf_views_data']['datatypeproperty'] = array(
    'title' => t('Data property'),
    'help' => t('Data property in the ontologies loaded on the OSF instance'),
    'field' => array(
      'handler' => 'osf_views_field_osf_datatypeproperty',
    ),
    'filter' => array(
      'handler' => 'osf_views_filter_osf_datatypeproperty',
    )  
  );  

  $data['osf_views_data']['objectproperty'] = array(
    'title' => t('Object property'),
    'help' => t('Object property in the ontologies loaded on the OSF instance'),
    'field' => array(
      'handler' => 'osf_views_field_osf_objectproperty',
    ),
    'filter' => array(
      'handler' => 'osf_views_filter_osf_objectproperty',
    )
  );  

  $data['osf_views_data']['annotationproperty'] = array(
    'title' => t('Annotation property'),
    'help' => t('Annotation property in the ontologies loaded on the OSF instance'),
    'field' => array(
      'handler' => 'osf_views_field_osf_annotationproperty',
    ),
    'filter' => array(
      'handler' => 'osf_views_filter_osf_annotationproperty',
    )  
  );  

  $data['osf_views_data']['undefinedproperty'] = array(
    'title' => t('Undefined property'),
    'help' => t('Undefined property in the ontologies loaded on the OSF instance'),
    'field' => array(
      'handler' => 'osf_views_field_osf_undefinedproperty',
    ),
    'filter' => array(
      'handler' => 'osf_views_filter_osf_undefinedproperty',
    )  
  );  

  return $data;
}



/**
* Implements hook_views_plugins_alter()
* 
* Alter existing plugins data, defined by modules.
* 
* @param mixed $plugins The array of plugins descriptions currently setuped for the view.
*/
function osf_views_views_plugins_alter(&$plugins) { 
  // Modify the default handler of the Page display to use the one specialized
  // for this module
  $plugins['display']['page']['handler'] = "osf_views_views_plugin_display_page";
}

/**
* Implements hook_views_plugins().
*/ 
function osf_views_views_plugins() {
  // Define the query plugin used to send queries to the OSF endpoint(s)
  $plugins = array(
    'module' => 'osf_views',
    'query' => array(
      'osf_views_query' => array(
        'title' => t('OSF API Query Plugin'),
        'help' => t('Views query plugin that sends queries to a OSF instance.'),
        'handler' => 'osf_views_views_plugin_query',
      ),
    ),
    'argument validator' => array(
      'node' => array(
        'title' => t('Content'),
        'handler' => 'views_plugin_argument_validate_node',
      ),
    ),
    'argument default' => array(
      'node' => array(
        'title' => t('Content ID from URL'),
        'handler' => 'views_plugin_argument_default_node'
      ),
    ),
  );

  // Define the entitiy views of the Views rows
  $data = views_cache_get('entity_base_tables', TRUE);
  if (!empty($data->data)) {
    $base_tables = $data->data;
  }
  else {
    foreach (views_fetch_data() as $table => $data) {
      if (!empty($data['table']['entity type']) && !empty($data['table']['base'])) {
        $base_tables[] = $table;
      }
    }
    views_cache_set('entity_base_tables', $base_tables, TRUE);
  }

  // If we have entities, define a Rendered Entity row type (reuse the one from Entity module).
  if (!empty($base_tables)) {
    $base_tables[] = 'osf_views';
    $base_tables[] = 'OSF';
    $plugins['row'] = array(
      'osf_views_entity' => array(
        'title' => t('Rendered entity (OSF)'),
        'help' => t('Renders a single entity in a specific view mode (e.g. teaser).'),
        'handler' => 'osf_views_views_plugin_row_entity_view',
        'uses fields' => FALSE,
        'uses options' => TRUE,
        'type' => 'normal',
        //'base' => 'osf_views_data',
      ),
    );
  }

  return $plugins;
}

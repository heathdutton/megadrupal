<?php

/**
 * Implements hook_views_data_alter().
 */
function geocluster_solr_views_data_alter(&$data) {
  $fields = field_info_fields();
  $servers = search_api_server_load_multiple(FALSE);
  foreach (search_api_index_load_multiple(FALSE) as $index) {
    if (!empty($index->server) && !empty($servers[$index->server])) {
      $server = $servers[$index->server];
      if ($server->class == 'geocluster_solr_service') {
        $item = &$data['search_api_index_' . $index->machine_name];
        // Set GeoclusterSearchApiViewsQuery class for all indices that
        // use the GeoclusterSearchApiSolrService.
        $item['table']['base']['query class'] = 'geocluster_search_api_views_query';
        // Add solr geocluster fields.

        // We could add the field name as prefix but we don't for handler simplicity.
        $geocluster_field = 'geocluster';
        $item[$geocluster_field . '_ids'] = array(
          'group' => 'Content',
          'title' => 'Geocluster ids (Solr)',
          'help' => 'Geocluster ids',
          'field' => array(
            'handler' => 'geocluster_handler_field_default',
          ),
        );
        $item[$geocluster_field . '_count'] = array(
          'group' => 'Content',
          'title' => 'Geocluster result count',
          'help' => 'Geocluster result count',
          'field' => array(
            'handler' => 'geocluster_handler_field_numeric',
          ),
        );
        $item[$geocluster_field . '_lat'] = array(
          'group' => 'Content',
          'title' => 'Geocluster lat',
          'help' => 'Geocluster lat',
          'field' => array(
            'handler' => 'geocluster_handler_field_numeric',
          ),
        );
        $item[$geocluster_field . '_lon'] = array(
          'group' => 'Content',
          'title' => 'Geocluster lon',
          'help' => 'Geocluster lon',
          'field' => array(
            'handler' => 'geocluster_handler_field_numeric',
          ),
        );

      }
    }
  }
}

/**
 * Implements hook_views_plugins().
 */
function geocluster_solr_views_plugins() {
  $plugins = array(
    'query' => array(
      'geocluster_search_api_views_query' => array(
        'title' => t('Geocluster Search API Query'),
        'help' => t('Geocluster Solr Query will be generated and run using the Search API.'),
        'handler' => 'GeoclusterSearchApiViewsQuery'
      ),
    ),
  );

  return $plugins;
}


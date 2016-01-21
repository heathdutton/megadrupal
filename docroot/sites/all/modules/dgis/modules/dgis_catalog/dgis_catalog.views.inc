<?php
/**
 * @file
 * Views for 2gis catalog.
 */

/**
 * Implements hook_views_data().
 */
function dgis_catalog_views_data() {
  $data['dgis_catalog']['table']['group'] = t('2gis catalog query');
  $data['dgis_catalog']['table']['base'] = array(
    'field' => 'id',
    'title' => t('2gis catalog'),
    'help' => t('Data from 2gis Server.'),
    'query class' => 'dgis_catalog_views',
  );
  $dgis_fields = array(
    'id' => t('ID'),
    'name' => t('Name'),
    'lon' => t('Longitude'),
    'lat' => t('Latitude'),
    'hash' => t('Hash'),
    'city_name' => t('City name'),
    'project_id' => t('Project ID'),
    'address' => t('Address'),
    'micro_comment' => t('Description'),
    'rubrics' => t('Rubrics'),
  );
  foreach ($dgis_fields as $code => $title) {
    $data['dgis_catalog'][$code] = array(
      'title' => $title,
      'help' => t('2gis organisation: ' . $title),
      'argument' => array(
        'handler' => 'views_handler_argument',
      ),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    );
  }
  $data['dgis_catalog']['dgis_catalog_what'] = array(
    'title' => t('What'),
    'help' => t('Search query'),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['dgis_catalog']['dgis_catalog_where'] = array(
    'title' => t('Where'),
    'help' => t('City (address is optional)'),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function dgis_catalog_views_plugins() {
  return array(
    'query' => array(
      'dgis_catalog_views' => array(
        'title' => t('2gis catalog'),
        'help' => t('Data is retreived from the 2gis api server.'),
        'handler' => 'dgis_catalog_views_plugin_query_dgis',
      ),
    ),
  );
}

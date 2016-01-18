<?php

/**
 * @file
 * Sampler API Views module support.
 */

/**
 * Implements hook_views_data().
 */
function sampler_views_data() {
  module_load_include('inc', 'sampler', 'sampler.api');

  $data = array();
  $metrics = sampler_load_metrics();
  $metrics_schemas = sampler_build_drupal_metric_schema();

  foreach ($metrics_schemas as $module => $metric_data) {
    foreach ($metric_data as $metric => $schema_data) {
      $info = $metrics[$module][$metric];
      $table_name = $schema_data['table_name'];
      $schema = $schema_data['schema'];

      // Base table info
      $data[$table_name] = array(
        'table' => array(
          'group' => t('@metric', array('@metric' => $info['title'])),
          'base' => array(
            'field' => 'object_id',
            'title' => t('Sampler metric: @metric', array('@metric' => $info['title'])),
            'help' => $info['description'],
          ),
        ),
      );

      $join = FALSE;
      if (isset($info['object_base_table']) && $info['object_base_table'] !== FALSE) {
        $join = TRUE;
        $base_table = $info['object_base_table'] ? $info['object_base_table'] : 'node';
        $primary_key = $info['object_primary_key'] ? $info['object_primary_key'] : 'nid';
        $data[$table_name]['table']['join'] = array(
          $base_table => array(
            'handler' => 'views_join',
            'left_field' => $primary_key,
            'field' => 'object_id',
          ),
        );
      }

      foreach ($schema['fields'] as $field_name => $field_info) {
        // Special case certain fields so that we can specify proper plugins
        // and names for primary key and timestamp.
        switch ($field_name) {
          case 'object_id':
            $data[$table_name][$field_name] = array(
              'title' => t('Object ID'),
              'help' =>  t('@schema-description', array('@schema-description' => $field_info['description'])),
              'field' => array(
                'handler' => 'views_handler_field_numeric',
                'click sortable' => TRUE,
              ),
              'argument' => array(
                'handler' => 'views_handler_argument_numeric',
                'numeric' => TRUE,
              ),
              'filter' => array(
                'handler' => 'views_handler_filter_numeric',
              ),
              'sort' => array(
                'handler' => 'views_handler_sort',
              ),
            );
            if ($join) {
              $data[$table_name][$field_name]['relationship'] = array(
                'handler' => 'views_handler_relationship',
                'base' => $base_table,
                'base field' => $primary_key,
                'label' => t('Metrics data'),
              );
            }
            break;

          case 'timestamp':
            $data[$table_name][$field_name] = array(
              'title' => t('Timestamp'),
              'help' =>  t('@schema-description', array('@schema-description' => $field_info['description'])),
              'field' => array(
                'handler' => 'views_handler_field_date',
                'click sortable' => TRUE,
              ),
              'sort' => array(
                'handler' => 'views_handler_sort_date',
              ),
              'filter' => array(
                'handler' => 'views_handler_filter_date',
              ),
            );
            break;

          case 'module':
          case 'metric':
            // @TODO add handling for these fields once we have an example that
            // uses them
            break;

          default:
            $data[$table_name][$field_name] = array(
              'title' => t('@data-type', array('@data-type' => $field_name)),
              'help' =>  t('@schema-description', array('@schema-description' => $field_info['description'])),
              'field' => array(
                'handler' => 'views_handler_field_numeric',
                'click sortable' => TRUE,
              ),
              'argument' => array(
                'handler' => 'views_handler_argument_numeric',
                'numeric' => TRUE,
              ),
              'filter' => array(
                'handler' => 'views_handler_filter_numeric',
              ),
              'sort' => array(
                'handler' => 'views_handler_sort',
              ),
            );
            break;

        }
      }
    }
  }

  return $data;
}


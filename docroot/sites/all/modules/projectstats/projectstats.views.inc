<?php

/**
 * Implements hook_views_data()
 */
function projectstats_views_data() {
  $data = array();

  $data['projectstats']['table']['group']  = t('Projectstats');

  $data['projectstats']['table']['base'] = array(
    'field' => 'pid',
    'title' => t('projectstats'),
    'help' => t('projectstats'),
  );

  $data['projectstats']['pid'] = array(
    'title' => t('pid'),
    'help' => t('Project ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['projectstats']['full_project_name'] = array(
    'field' => array(
      'title' => t('full_project_name'),
      'help' =>  t('full_project_name'),
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  $data['projectstats']['short_project_name'] = array(
    'field' => array(
      'title' => t('short_project_name'),
      'help' =>  t('short_project_name'),
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );

  $data['projectstats']['installs'] = array(
    'title' => t('installs'),
    'help' => t('installs'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['projectstats']['downloads'] = array(
    'title' => t('downloads'),
    'help' => t('downloads'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['projectstats']['updated'] = array(
    'title' => t('Project data updated'),
    'help' => t('updated'),
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

  return $data;
}

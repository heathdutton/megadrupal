<?php 

/**
 * Implementation of hook_views_data_alter().
 *
 * Add relationship pointing from the `node` table to the `webform_ab`
 * and `webform_ab_tests` tables. This allows site builders access to
 * Webform A/B metadata.
 */
function webform_ab_views_views_data_alter(&$data) {
  $data['node']['nid_to_webform_ab'] = array(
    'relationship' => array(
      'base' => 'webform_ab',
      'base field' => 'nid',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Webform A/B Tests'),
      'title' => t('Webform A/B Tests'),
      'help' => t('Relates nodes to Webform A/B test data'),
    ),
  );
  $data['node']['nid_to_webform_ab_forms'] = array(
    'relationship' => array(
      'base' => 'webform_ab_forms',
      'base field' => 'test_nid',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Webform A/B Forms'),
      'title' => t('Webform A/B Forms'),
      'help' => t('Relates nodes to individual Webform A/B forms'),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 *
 * Describe `webform_ab` and `webform_ab_tests` tables to Views. These tables
 * include Webform A/B metadata, as well as a relationship from `webform_ab_forms`
 * to the `node` table, in order to pull in additional node data about individual
 * webforms being used in A/B tests, if needed.
 */
function webform_ab_views_views_data(){
  // Webform_AB table.
  $data['webform_ab']['table']['group'] = t('Webform A/B Tests');
  $data['webform_ab']['nid'] = array(
    'title' => t('NID'), 
    'help' => t('The NID of the Webform A/B test.'), 
    'field' => array(
      'handler' => 'views_handler_field_node', 
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
    ),
  );
  $data['webform_ab']['webform_types'] = array(
    'title' => t('Type'),
    'help' => t('The types of Webform-enabled nodes allowed on this A/B test.'),
    'field' => array(
      'handler' => 'views_handler_field', 
      'click sortable' => TRUE,
    ), 
    'sort' => array(
      'handler' => 'views_handler_sort',
    ), 
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ), 
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['webform_ab']['public_teaser'] = array(
    'title' => t('Public Teaser'),
    'help' => t('The public-facing teaser on this A/B test.'),
    'field' => array(
      'handler' => 'views_handler_field', 
      'click sortable' => TRUE,
    ), 
    'sort' => array(
      'handler' => 'views_handler_sort',
    ), 
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ), 
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['webform_ab']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display the operations links for the Webform A/B test.'),
      'handler' => 'webform_ab_views_handler_field_operations',
    ),
  );
  
  // Webform_AB_Tests table.
  $data['webform_ab_forms']['table']['group'] = t('Webform A/B Forms');
  $data['webform_ab_forms']['test_nid'] = array(
    'title' => t('NID'), 
    'help' => t('The NID of the Webform A/B test.'), 
    'field' => array(
      'handler' => 'views_handler_field_node', 
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
    ),
  );
  $data['webform_ab_forms']['webform_nid'] = array(
    'title' => t('Referenced Webform NID'), 
    'help' => t('The NID of the individual Webform.'), 
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
    'relationship' => array(
      'base' => 'node',
      'base field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Webform Node'),
      'title' => t('Webform Node'),
      'help' => t('Relate Webform A/B forms to node data.'),
    ),
  );
  $data['webform_ab_forms']['status'] = array(
    'title' => t('Status'), 
    'help' => t('The status of the relationship between the A/B test and the Webform.'), 
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
  
  return $data;
}

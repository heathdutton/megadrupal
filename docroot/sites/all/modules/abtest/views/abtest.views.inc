<?php

/**
 * @file
 * A/B test integration with views.
 */

/**
 * Implements hook_views_data().
 */
function abtest_views_data() {
  $data = array();

  $data['abtest'] = array(
    'table' => array(
      'group' => t('A/B test'),
      'base' => array(
        'field' => 'test_id',
        'title' => t('A/B tests'),
      ),
    ),

    'test_id' => array(
      'title' => t('Test ID'),
      'help' => t('The ID of the test.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'argument' => array(
        'handler' => 'abtest_views_handler_argument_test_id',
        'name field' => 'test_name',
        'numeric' => TRUE,
        'validate type' => 'numeric',
      ),
    ),

    'test_name' => array(
      'title' => t('Name'),
      'help' => t('The unique string that identifies this test.'),
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
      )
    ),

    'conversion_name' => array(
      'title' => t('Conversion name'),
      'help' => t('The conversion event for this test.'),
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
      )
    ),

    'status' => array(
      'title' => t('Status'),
      'help' => t('Whether or not the test is running.'),
      'field' => array(
        'handler' => 'views_handler_field_boolean',
        'click sortable' => TRUE,
        'output formats' => array(
          'running-notrunning' => array(t('Running'), t('Not running')),
        ),
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_boolean_operator',
        'label' => t('Running'),
        'type' => 'yes-no',
        'use equal' => TRUE, // Use status = 1 instead of status <> 0 in WHERE statment
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),

    'created' => array(
      'title' => t('Created date'),
      'help' => t('The date the test was created.'),
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
    ),

    'alternatives' => array(
      'relationship' => array(
        'base' => 'abtest_alternatives',
        'base field' => 'test_id',
        'field' => 'test_id',
        'handler' => 'views_handler_relationship',
        'label' => t('alternative'),
        'title' => t('A/B test alternative'),
        'help' => t('The alternatives for this test.'),
      ),
    ),

    'total_participants' => array(
      'title' => t('Total participants'),
      'help' => t('The total number of participants in this test.'),
      'field' => array(
        'handler' => 'abtest_views_handler_field_statistics',
        'statistics function' => 'getTotalParticipants',
      ),
    ),

    'total_conversions' => array(
      'title' => t('Total conversions'),
      'help' => t('The total number of conversions in this test.'),
      'field' => array(
        'handler' => 'abtest_views_handler_field_statistics',
        'statistics function' => 'getTotalConversions',
      ),
    ),

    // TODO: add a statistical significance field
  );

  $data['abtest_alternatives'] = array(
    'table' => array(
      'group' => t('A/B test alternative'),
    ),

    'alternative_id' => array(
      'title' => t('Alternative ID'),
      'help' => t('The ID of the alternative.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      // TODO: the title it shows should be 'value'
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
        'name field' => 'value',
        'numeric' => TRUE,
        'validate type' => 'numeric',
      ),
    ),

    'value' => array(
      'title' => t('Value'),
      'help' => t('The value of the alternative.'),
      'field' => array(
        'handler' => 'views_handler_field_serialized',
      ),
    ),

    'participants' => array(
      'title' => t('Participants'),
      'help' => t('The number of participants who saw this alternative.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),

    'conversions' => array(
      'title' => t('Conversions'),
      'help' => t('The number of participants who successfully converted.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),

    'total_conversions_percent' => array(
      'title' => t('Conversions / Total Participants'),
      'help' => t('The percentage of total participants who converted with this alternative.'),
      'field' => array(
        'handler' => 'abtest_views_handler_field_statistics_alternative',
        'alternative property' => 'total_conversions_percent',
        'percentage' => TRUE,
      ),
    ),

    'total_participants_percent' => array(
      'title' => t('Participants / Total Participants'),
      'help' => t('The percentage of total participants who got this alternative.'),
      'field' => array(
        'handler' => 'abtest_views_handler_field_statistics_alternative',
        'alternative property' => 'total_participants_percent',
        'percentage' => TRUE,
      ),
    ),

    'conversions_percent' => array(
      'title' => t('Conversions / Participants'),
      'help' => t('The percentage of participants who converted with this alternative.'),
      'field' => array(
        'handler' => 'abtest_views_handler_field_statistics_alternative',
        'alternative property' => 'conversions_percent',
        'percentage' => TRUE,
      ),
    ),
  );

  return $data;
}


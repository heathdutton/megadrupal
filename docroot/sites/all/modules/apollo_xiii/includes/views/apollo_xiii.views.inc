<?php
/**
 * @file
 * This file will include all the Views hook implementations.
 */

/**
 * Implements hook_views_plugin().
 */
function apollo_xiii_views_plugins() {
  return array(
    'module' => 'apollo_xiii',
    'query' => array(
      'apollo_xiii_query_plugin' => array(
        'title' => t('Query from Drupal projects issues'),
        'handler' => 'apollo_xiii_query_plugin',
        'parent' => 'views_query',
      ),
    ),
  );
}

/**
 * Implements hook_views_data().
 *
 * Making the option appear as a view type
 * requires implementing hook_views_data().
 *
 * With the field, we are defining how this item must be handled, but we have to
 * declare first the type of handler we are talking about. We may have different
 * handlers for each field, and we may use views default handlers or our custom
 * ones.
 */
function apollo_xiii_views_data() {
  $data['apollo_xiii']['table']['group']  = t('Entries from Drupal projects issues');
  $data['apollo_xiii']['table']['base'] = array(
    'title' => t('Entries from Drupal project issues'),
    'query class' => 'apollo_xiii_query_plugin',
  );

  // Define the id field.
  $data['apollo_xiii']['id'] = array(
    'title' => t('Issue ID'),
    'help' => t('Current issue ID.'),
    'field' => array(
      'handler' => 'apollo_xiii_handler_field_numeric',
      'click sortable' => TRUE,
      'rss_part' => 'item',
      'rss_field' => 'guid',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'apollo_xiii_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Define the title field.
  $data['apollo_xiii']['title'] = array(
    'title' => t('Title'),
    'help' => t('Current issue Title.'),
    'field' => array(
      'handler' => 'apollo_xiii_handler_field',
      'click sortable' => TRUE,
      'rss_part' => 'item',
      'rss_field' => 'title',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'apollo_xiii_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Define the description field.
  $data['apollo_xiii']['description'] = array(
    'title' => t('Description'),
    'help' => t('Current issue description.'),
    'field' => array(
      'handler' => 'apollo_xiii_handler_field_markup',
      'click sortable' => TRUE,
      'rss_part' => 'item',
      'rss_field' => 'description',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'apollo_xiii_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Define the date field.
  $data['apollo_xiii']['date'] = array(
    'title' => t('Created date'),
    'help' => t('The date the original issue item was posted.'),
    'field' => array(
      'handler' => 'apollo_xiii_handler_field_date',
      'click sortable' => TRUE,
      'rss_part' => 'item',
      'rss_field' => 'pubDate',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'apollo_xiii_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
  );

  // Define the creator field.
  $data['apollo_xiii']['creator'] = array(
    'title' => t('Creator'),
    'help' => t('The name of the creator of the current issue.'),
    'field' => array(
      'handler' => 'apollo_xiii_handler_field',
      'click sortable' => TRUE,
      'rss_part' => 'item',
      'rss_field' => 'creator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'apollo_xiii_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Define the project title field.
  $data['apollo_xiii']['project_title'] = array(
    'title' => t('Project Title'),
    'help' => t('Current issue main project title.'),
    'field' => array(
      'handler' => 'apollo_xiii_handler_field',
      'click sortable' => TRUE,
      'rss_part' => 'channel',
      'rss_field' => 'project_title',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'apollo_xiii_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Define the project name field.
  $data['apollo_xiii']['project_name'] = array(
    'title' => t('Project Name'),
    'help' => t('Current issue main project machine name.'),
    'field' => array(
      'handler' => 'apollo_xiii_handler_field_machine_name',
      'click sortable' => TRUE,
      'rss_part' => 'channel',
      'rss_field' => 'project_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'apollo_xiii_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Define the project homepage field.
  $data['apollo_xiii']['project_homepage'] = array(
    'title' => t('Project Homepage'),
    'help' => t('Current issue main project homepage.'),
    'field' => array(
      'handler' => 'apollo_xiii_handler_field_project_url',
      'click sortable' => TRUE,
      'rss_part' => 'channel',
      'rss_field' => 'project_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'apollo_xiii_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Define the project homepage field.
  $data['apollo_xiii']['issue_url'] = array(
    'title' => t('Issue URL'),
    'help' => t('Current issue url.'),
    'field' => array(
      'handler' => 'apollo_xiii_handler_field_issue_url',
      'click sortable' => TRUE,
      'rss_part' => 'item',
      'rss_field' => 'guid',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'apollo_xiii_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  return $data;
}

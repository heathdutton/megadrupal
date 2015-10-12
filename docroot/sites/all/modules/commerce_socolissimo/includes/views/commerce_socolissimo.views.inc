<?php

/**
 * @file
 * Export SoColissimo flexibility rate to views.
 */

/**
 * Implements hook_views_data().
 */
function commerce_socolissimo_views_data() {
  $data = array();

  $data['commerce_socolissimo_rate']['table']['group'] = t('Commerce So Colissimo Flexibility rate');

  $data['commerce_socolissimo_rate']['table']['base'] = array(
    'field' => 'rate_id',
    'title' => t('Commerce So Colissimo Flexibility rate'),
    'help' => t('A So Colissimo Flexibility rate relative to a shipping service.'),
    'access query tag' => 'commerce_entity_access',
  );
  $data['commerce_socolissimo_rate']['table']['entity type'] = 'commerce_socolissimo_rate';

  // Expose the rate_id column.
  $data['commerce_socolissimo_rate']['rate_id'] = array(
    'title' => t('Rate ID', array(), array('context' => 'a SoColissimo flexibility rate relative to a shipping service.')),
    'help' => t('Primary identifier for a rate.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Expose the name column.
  $data['commerce_socolissimo_rate']['name'] = array(
    'title' => t('So Colissimo shipping service name', array(), array('context' => 'a SoColissimo flexibility rate relative to a shipping service.')),
    'help' => t('The shipping service name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the type column.
  $data['commerce_socolissimo_rate']['type'] = array(
    'title' => t('Type'),
    'help' => t('The type of a So Colissimo Flexibility rate.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Expose the created and changed timestamps.
  $data['commerce_socolissimo_rate']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date the rate was created.'),
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

  $data['commerce_socolissimo_rate']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the rate was last updated.'),
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

  $data['commerce_socolissimo_rate']['edit_rate'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit a So Colissimo Flexibility rate.'),
      'handler' => 'commerce_socolissimo_handler_field_rate_link_edit',
    ),
  );

  $data['commerce_socolissimo_rate']['delete_rate'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete a So Colissimo Flexibility rate.'),
      'handler' => 'commerce_socolissimo_handler_field_rate_link_delete',
    ),
  );

  $data['commerce_socolissimo_rate']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all the available operations links for a So Colissimo Flexibility rate.'),
      'handler' => 'commerce_socolissimo_handler_field_rate_operations',
    ),
  );

  return $data;
}

/**
 * Alter existing plugins data, defined by modules.
 *
 * Adds the expeditor inet export plugin to views.
 *
 * @see hook_views_plugins()
 */
function commerce_socolissimo_views_plugins_alter(&$plugins) {
  if (isset($plugins['display']['views_data_export'])) {
    $path = drupal_get_path('module', 'commerce_socolissimo');

    $plugins['style']['views_data_export_expeditor_inet'] = array(
      'title' => t('Expeditor INET file'),
      'name' => 'views_data_export_expeditor_inet',
      'help' => t('Display the view as a formatted file used by So Colissimo services.'),
      'handler' => 'commerce_socolissimo_export_expeditor_inet',
      'export headers' => array('Content-type' => 'text/plain; charset=utf-8'),
      'export feed type' => 'fmt',
      'export feed text' => 'Export views results as an Expeditor INET file.',
      'export feed file' => '%view.fmt',
      'export feed icon' => $path . '/theme/images/exp_inet.png',
      'additional themes base' => 'views_data_export_expeditor_inet',
      'path' => $path . '/views/plugins',
      'parent' => 'views_data_export',
      'theme' => 'views_data_export',
      'theme path' => drupal_get_path('module', 'views_data_export') . '/theme',
      'theme file' => 'views_data_export.theme.inc',
      'uses row plugin' => FALSE,
      'uses fields' => TRUE,
      'uses options' => TRUE,
      'type' => 'data_export',
    );
  }
}

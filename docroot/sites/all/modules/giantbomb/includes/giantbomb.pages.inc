<?php
/**
 * @file
 * Full pages for informational purposes.
 */

/**
 * Search for Giant Bomb objects, and return a result table.
 *
 * @param string $query
 *   Un-sanitized query string.
 *
 * @return array
 *   Render array
 */
function giantbomb_search_result($query, $endpoint) {
  $types = giantbomb_types_all($endpoint, 'object');
  $rows = array();
  $header = array(
    t('#'),
    t(''),
    t('Title'),
    t('Type'),
    t('Local Entity'),
    array('colspan' => 2, 'data' => t('Scheduler Actions')),
  );
  $no_results = t('No results found.');

  // Query
  $options['limit'] = 100;
  $result = giantbomb_api_query_search($endpoint, ($query), array(), $options);

  if ($result->status_code != GIANTBOMB_API_CODE_OK) {
    $no_results = giantbomb_api_status($result->status_code);
  }

  $i = 0;
  foreach ($result->results as $item) {
    $type = isset($types[$item->resource_type]) ? $types[$item->resource_type] : NULL;
    $i++;

    $row = array();
    $row[] = $i;

    // Image
    if (isset($item->image->tiny_url) && valid_url($item->image->tiny_url)) {
      $row[] = '<img src="' . $item->image->tiny_url . '" />';
    }
    else {
      $row[] = '';
    }

    // Name
    if (isset($item->name)) {
      if (isset($item->site_detail_url) && valid_url($item->site_detail_url)) {
        $name = l($item->name, $item->site_detail_url, array(
          'attributes' => array(
            'title' => t('@type object #@id', array(
              '@type' => $item->resource_type,
              '@id' => $item->id,
            )),
          ),
        ));

        if (isset($item->deck)) {
          $name .= '<p>' . check_plain($item->deck) . '</p>';
        }

        $row[] = $name;
      }
      else {
        $row[] = $item->name;
      }
    }

    // Type
    $row[] = $type ? $type->title : $item->resource_type;

    // Local Entity
    list($entity_type, $entity) = giantbomb_local_entity_load($endpoint, $item->resource_type, $item->id);
    if ($entity) {
      // Local entity exists
      $link = entity_uri($entity_type, $entity);
      $row[] = l(t('View'), $link['path']);
    }
    else {
      $row[] = '';
    }

    // Scheduler
    if ($type) {
      $row[] = array('colspan' => 2, 'data' => l(t('Create job'), 'admin/config/services/giantbomb/scheduler/add', array(
        'query' => array(
          'type' => $endpoint . '|' . $type->id,
          'id' => $item->id,
        ),
      )));
    }

    $rows[] = $row;
  }

  return array(
    array('#markup' => theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'caption' => t('Search Results'),
      'empty' => $no_results,
    ))),
  );
}

function giantbomb_job_list_page() {
  giantbomb_process_job_object('giantbomb', 'person', 93335, array());;//@todo remove
  $rows = array();
  $header = array(
    t('Website'),
    t('Type'),
    t('Detail'),
    t('Last Update'),
    t('Next Update'),
    t('Active'),
    array('colspan' => 2, 'data' => t('Operations')),
  );

  $endpoints = giantbomb_api_endpoints();
  foreach ($endpoints as $endpoint => $endpoint_info) {
    $jobs = giantbomb_jobs_all($endpoint);
    $objects = giantbomb_types_all($endpoint, 'id');

    foreach ($jobs as $job) {
      $row = array();

      $row[] = $endpoint_info['title'];

      if (isset($objects[$job->type])) {
        $object_info = $objects[$job->type];
        $object = $object_info->object;
        $row[] = $object_info->title;

        if ($job->object_id) {
          list($entity_type, $entity) = giantbomb_local_entity_load($endpoint, $object, $job->object_id);
          if ($entity) {
            $label = entity_label($entity_type, $entity);
            $label = !empty($label) ? $label : 'Untitled'; 
            $link = entity_uri($entity_type, $entity);
            $row[] = !empty($link['path']) ? l($label, $link['path']) : $label;
          }
          else {
            $row[] = t(
              'Entity has not been not created. (#@object_id)', 
              array('@object_id' => $job->object_id)
            );
          }
        }
      }
      else {
        $row[] = t('Unknown object type.');
      }

      if ($job->resource != 'object') {
        $row[] = ucwords($job->resource);
      }

      $row[] = $job->updated ? format_date($job->updated) : t('Never updated');
      $row[] = $job->next_update ? format_date($job->next_update) : t('Never');
      $row[] = $job->active ? t('Active') : t('Inactive');
      $row[] = l(t('edit'), 'admin/config/services/giantbomb/scheduler/job/' . $job->id . '/edit');
      $row[] = l(t('delete'), 'admin/config/services/giantbomb/scheduler/job/' . $job->id . '/delete');

      $rows[] = $row;
    }
  }

  return array(
    array('#markup' => theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'empty' => t('No jobs found.'),
    ))),
  );
}

function giantbomb_types_list_page() {
  $endpoints = giantbomb_api_endpoints();
  ksort($endpoints);

  $rows = array();
  $header = array(
    t('Website'),
    t('Type'),
    array('colspan' => 1, 'data' => t('Entity')),
    array('colspan' => 4, 'data' => t('Operations')),
  );
  
  foreach ($endpoints as $name => $endpoint) {
    $types = giantbomb_types_all($name, 'object');

    foreach ($types as $type) {
      $row = array();
      $row[] = $endpoint['title'];
      $row[] = l($type->title, 'admin/config/services/giantbomb/types/manage/' . $name . '/' . $type->object);

      $entity_mapping = l(t('entity mapping'), 'admin/config/services/giantbomb/types/manage/' . $name . '/' . $type->object . '/entity');

      if ($type->entity_type && $type->bundle) {
        $entity_info = entity_get_info($type->entity_type);

        if (isset($entity_info['bundles'][$type->bundle]['admin']['real path'])) {
          $row[] = $entity_info['label'] . ': ' . l($entity_info['bundles'][$type->bundle]['label'], $entity_info['bundles'][$type->bundle]['admin']['real path']);
        }
        else {
          $row[] = $type->entity_type . ': ' . $type->bundle;
        }

        $row[] = $entity_mapping;
        $row[] = l(t('field mapping'), 'admin/config/services/giantbomb/types/manage/' . $name . '/' . $type->object . '/fields');
        $row[] = l(t('relationships'), 'admin/config/services/giantbomb/types/manage/' . $name . '/' . $type->object . '/relationships');
        $row[] = l(t('delete'), 'admin/config/services/giantbomb/types/manage/' . $name . '/' . $type->object . '/delete');
      }
      else {
        $row[] = '';
        $row[] = $entity_mapping;
        $row[] = array('colspan' => 3, 'data' => '');
      }

      $rows[] = $row;
    }
  }

  return array(
    array('#markup' => theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'caption' => t('Giant Bomb Object Types'),
      'empty' => t('No object types found. Try refreshing types.'),
    ))),
  );
}

function giantbomb_type_page($endpoint_info, $type_info) {
  $object = $type_info->object;
  $endpoint = $endpoint_info['name'];
  $properties = giantbomb_object_properties('giantbomb', $object);

  $elements = array();

  // Property Table
  $rows = array();
  $header = array(
    t('Title'),
    t('Description'),
    t('Data type'),
  );

  foreach ($properties as $property) {
    $title = $property['title'];
    $description = isset($property['description']) ? $property['description'] : '';

    $row = array();
    if (!empty($property['required'])) {
      $row[] = "<strong>$title</strong>";
      $row[] = t(
        '@description This field is required.',
        array(
          '@description' => $description,
        )
      );
    }
    else {
      $row[] = $title;
      $row[] = $description;
    }

    $row[] = $property['data type'];
    $rows[] = $row;
  }

  $elements['properties']['#markup'] = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'caption' => t('Object Properties'),
    'empty' => t('No results found.'),
  ));

  module_load_include('inc', 'giantbomb', 'includes/giantbomb.forms');

  $elements['settings']['#prefix'] = '<h3>Settings</h3>';
  $elements['settings']['form'] = drupal_get_form('giantbomb_type_edit_settings_form', $endpoint_info, $type_info);

  return $elements;
}

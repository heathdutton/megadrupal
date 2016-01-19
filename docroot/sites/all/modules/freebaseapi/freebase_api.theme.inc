<?php
/**
 * @file
 * Theme functions for Freebase API. Admin UI and diagnostic screens.
 */

/**
 * Render the Freebase topic data packet somewhat.
 *
 * THIS IS NOT INTENDED TO BE USED DIRECTLY.
 * It's just a way to 'dump' the data returned by a generic 'topic' lookup.
 * You should deal with the expected Data in your own way, this is diagnostic
 * mainly.
 *
 * @param array $topic
 *   Prototype topic definition.
 * @param bool $nested
 *   Whether to return results structured or flat.
 *
 * @return string
 *   Rendered HTML, displaying the result.
 */
function theme_freebase_api_topic_summary($topic, $nested = TRUE) {
  // A 'topic' is an array with probably an id and a whole bunch of properties.
  // The properties are arrays, and the values inside those arrays usually
  // present a readable 'text' attribute.
  // However, we make no other assumptions about what we are rendering.
  $elements = array();
  if (isset($topic['property']['/type/object/name'])) {
    $elements['name'] = array(
      '#markup' => '<h2>Topic: ' . $topic['property']['/type/object/name']['values'][0]['text'] . '</h2>',
    );
  }

  if (empty($topic['property'])) {
    $elements['error'] = array(
      '#markup' => '<h2>No properties</h2>There has probably been a lookup error. Check the logs.',
    );
    return drupal_render($elements);
  }

  // When iterating, nest properties based on domain key.
  $nested_elements = array();
  $flat_elements = array();
  foreach ($topic['property'] as $property_id => $property) {
    if ($property_id == '/type/object/key') {
      // Skip this. There may be hundreds and they are useless in a summary.
      continue;
    }
    $text_values = array();
    foreach ($property['values'] as $value_delta => $value) {
      if (is_string($value['text'])) {
        $text_values[$value_delta] = freebase_api_link($value['id'], $value['text']);
      }
    }
    if (!empty($text_values)) {
      $flat_elements[$property_id] = array(
        'title' => array(
          '#type' => 'html_tag',
          '#tag' => 'h3',
          '#value' => $property_id,
        ),
        'body' => array(
          '#markup' => theme('item_list', array('items' => $text_values)),
        ),
      );
    }
  }

  if (!$nested) {
    $elements += $flat_elements;
    return drupal_render($elements);
  };

  // Optional layout
  // Re-sort flat elements into nested elements.
  // Don't try to guess what's happening here, it's just wacky array-munging.
  foreach ($flat_elements as $property_id => $value) {
    $pointer = &$nested_elements;
    $last_child = NULL;
    $lineage = explode('/', $property_id);
    // Pop the top first, it's always null.
    array_shift($lineage);
    while ($child = array_shift($lineage)) {
      if (!isset($pointer[$child])) {
        $pointer[$child] = array(
          '#type' => 'fieldset',
          '#title' => $child,
        );
      }
      $pointer =& $pointer[$child];
      $last_child = $child;
    }
    $value['#title'] = $last_child;
    $value['title']['#value'] = $last_child;
    $pointer = array(
      'value' => $value,
    );
  }

  $elements += $nested_elements;
  return drupal_render($elements);
}

/**
 * Diagnostic dump.
 *
 * @param array $response
 *   Response data from search.
 *
 * @return string
 *   Rendered HTML.
 */
function theme_freebase_api_search_results($response) {
  $elements = array();
  $info_vars = array('status', 'cursor', 'cost', 'hits');
  $elements['summary'] = array(
    // html_tag in d7 doesn't do children :-(
    '#prefix' => '<dl>',
    '#suffix' => '<dl>',
  );
  foreach ($info_vars as $var) {
    $elements['summary'][$var]['#markup'] = t('<dt>%term</dt><dd>%definition</dd>',
      array(
        '%term' => $var,
        '%definition' => $response[$var],
      )
    );
  }

  if (empty($response['result'])) {
    $elements['result'] = array(
      '#markup' => 'No result',
    );
  }
  else {
    $elements['result'] = array(
      '#prefix' => '<ul>',
      '#suffix' => '</ul>',
    );
    foreach ($response['result'] as $i => $result) {
      $elements['result'][$i] = array(
        '#type' => 'html_tag',
        '#tag' => 'li',
        '#value' => freebase_api_link($result['mid'], $result['name']) . ' (' . @$result['notable']['name'] . ')',
      );
    }
  }

  return drupal_render($elements);
}


/**
 * Render the Freebase schema somewhat.
 *
 * THIS IS NOT INTENDED TO BE USED DIRECTLY.
 * It's just a way to 'dump' the data returned by a generic 'topic' lookup.
 * You should deal with the expected Data in your own way, this is diagnostic
 * mainly.
 *
 * @param array $response
 *   Result of an mql read that returns things of type /type/type.
 * @param Bool $nested
 *   Result should be structured or flat.
 *
 * @return string
 *   Rendered HTML.
 */
function theme_freebase_api_schema_summary($response, $nested = TRUE) {
  $schema = $response['result'];
  $elements = array();
  if (isset($schema['/type/object/name'])) {
    $elements['name'] = array(
      '#markup' => '<h2>Schema: ' . freebase_api_link($schema['/type/object/id'], $schema['/type/object/name']) . '</h2>',
    );
  }

  // Build a table describing the object schema.
  $properties = array();
  foreach ($schema['/type/type/properties'] as $property) {
    $properties[] = array(
      t('<strong>@name</strong> (@id)', array(
        '@name' => $property['/type/object/name'],
        '@id' => $property['/type/object/id'],
      )),
      t('@type', array(
        '@type' => $property['/type/property/expected_type'],
      )),
    );
  };

  foreach ($schema['/freebase/type_hints/included_types'] as $included_type) {
    // $type_id = $included_type['/type/object/id'];
    $properties[] = array(
      'data' => array(
        'data' => check_plain($included_type['/type/object/name']),
        'attributes' => array('colspan' => 2),
      ),
      'header' => TRUE,
    );
    foreach ($included_type['/type/type/properties'] as $property) {
      $properties[] = array(
        t('<strong>@name</strong> (@id)', array(
          '@name' => $property['/type/object/name'],
          '@id' => $property['/type/object/id'],
        )),
        t('@type', array(
          '@type' => $property['/type/property/expected_type'],
        )),
      );
    };
  }

  $elements['properties'] = array(
    '#theme' => 'table',
    '#header' => array(t('Field name'), t('Type')),
    '#rows' => $properties,
  );
  return drupal_render($elements);
}

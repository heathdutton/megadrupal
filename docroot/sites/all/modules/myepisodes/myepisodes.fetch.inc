<?php

/**
 * @file
 * Fetch related functions.
 */

/**
 * Parse given XML content and return an array of my episodes feed items.
 * 
 * @return array
 *   Array of items or FALSE in case of error.
 */
function myepisodes_parse_xml($buffer) {
  libxml_use_internal_errors();

  $xml = new SimpleXMLElement($buffer);
  $items = array();
  $xml_items = $xml->xpath('/rss/channel/item/title');

  if (empty($xml_items)) {
    return FALSE;
  }

  foreach ($xml_items as $xml_item) {
    if (preg_match('/\[([^\]]+)\]\s*\[([^\]]+)\]\s*\[([^\]]+)\]\s*\[([^\]]+)\]/', (string)$xml_item, $matches)) {
      $item = array(
        'title'          => $matches[1],
        'episode_number' => $matches[2],
        'episode_title'  => $matches[3],
        'air_date'       => $matches[4],
      );
      if (preg_match('/(\d+)x(\d+)/i', $item['episode_number'], $matches)) {
        $item += array(
          'season'  => $matches[1],
          'episode' => $matches[2],
        );
      }
      $items[] = $item;
    }
  }

  if (empty($items)) {
    return FALSE;
  } else {
    return $items;
  }
}

/**
 * Refresh given field data item.
 * 
 * @param array &$item
 * 
 * @return bool
 *   TRUE if updated, FALSE else.
 * 
 * @throws RuntimeException
 */
function myepisodes_refresh_item(array &$item) {
  if (!isset($item['uid']) || !isset($item['password_hash'])) {
    throw new RuntimeException("Given item is not a my episode field item or is empty");
  }

  $query = array(
    'feed'   => 'mylist',
    'uid'    => $item['uid'],
    'pwdmd5' => $item['password_hash'],
  );
  $url = url(MYEPISODES_BASE_URL . MYEPISODES_FEED_URL, array('query' => $query));

  $response = drupal_http_request($url);

  if (!isset($response->data) || empty($response->data)) {
    watchdog('myepisodes', "Response from My Episodes site feed at URL '@url' is not OK", array('@url' => $url), WATCHDOG_ERROR);
    return FALSE;
  }

  $items = myepisodes_parse_xml($response->data);

  // Parse feed and fetch content.
  if (FALSE === $items) {
    watchdog('myepisodes', "Response from My Episodes site feed at URL '@url' is not formatted as expected", array('@url' => $url), WATCHDOG_ERROR);
    return FALSE;
  }

  $item['last_refresh'] = time();
  $item['cache_mylist'] = serialize($items);

  return TRUE;
}

/**
 * Refresh current field. If field is multiple, it will refresh all values.
 * 
 * Note that this function won't save the result.
 * 
 * @param object $entity
 * @param string|array $field
 *   Field instance or name.
 * 
 * @return bool
 *   TRUE if one or more field entry have been updated, FALSE else.
 * 
 * @throws RuntimeException
 */
function myepisodes_refresh_entity_field($entity, $field) {
  if (is_array($field)) {
    $field_name = $field['field_name'];
  } else {
    $field_name = $field;
  }

  if (!property_exists($entity, $field_name)) {
    throw new RuntimeException("Field does not exists in given entity.");
  }

  $updated = FALSE;

  foreach ($entity->{$field_name}['und'] as $delta => &$item) {
    $updated = myepisodes_refresh_item($item) || $updated;
  }

  return $updated;
}

/**
 * This implementation is not scalable and may fail if you have a lot of
 * entities with refresh enabled.
 */
function myepisodes_refresh_all($limit = 20) {
  $delay = time() - variable_get(MYEPISODES_REFRESH_DELAY_VAR, MYEPISODES_REFRESH_DELAY_DEFAULT);
  $info = _field_info_collate_fields();

  $current_limit = $limit;

  // Iterate over field in order to find the one matching our type.
  foreach ($info['fields'] as $field) {
    if ('myepisodes' === $field['type']) {
      // Use the infamous EFQ to fetch entities that are refresh enabled.
      $query = new EntityFieldQuery;
      $query->fieldCondition($field, 'refresh_enabled', 1);
      $query->fieldCondition($field, 'last_refresh', $delay, '<');
      $query->range(0, $current_limit);
      $results = $query->execute();

      foreach ($results as $entity_type => $result) {
        $entity_info = entity_get_info($entity_type);
        $primary_key = $entity_info['entity keys']['id'];

        // Aggregate entities identifiers for multiple loading at once. The EFQ
        // really does return an ugly result set.
        $ids = array();
        foreach ($result as $data) {
          $ids[] = $data->{$primary_key};
          --$current_limit;
        }

        // Force refresh for each enabled one.
        foreach (entity_get_controller($entity_type)->load($ids) as $entity) {
          try {
            myepisodes_refresh_entity_field($entity, $field);
          } catch (Exception $e) {
            watchdog_exception('myepisodes', $e);
          }

          // Save field content only, this is the only way to avoid being
          // dependent type for saving our data.
          field_attach_presave($entity_type, $entity);
          field_attach_update($entity_type, $entity);
        }
      }
    }
  }
}

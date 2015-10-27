<?php
/**
 * @file
 * API demonstrating various hooks for customizing Thether Stats.
 */

/**
 * Have Tether Stats map a url to an entity or derivative.
 *
 * This hook allows a module to map a url to a specific entity_type and
 * entity_id when creating entries in the tether_stats_elements table.
 * This results in stats for matching pages to be recorded with the
 * entity_id turning a weaker url stat into a more manageable entity stat
 * for data mining purposes.
 *
 * You may also map a page url to an entity derivative.
 *
 * @param string $url
 *   The url to be mapped.
 * @param string $query
 *   The query string set for the page or NULL if no query string was
 *   added. Query strings normally only spawn new elements when the variable
 *   setting "tether_stats_allow_query_string_to_define_new_elements" is TRUE.
 *   Setting the query parameter on the identity set here, however, will
 *   enforce a different element gets used that's specific to the query
 *   string.
 *
 * @return array|false
 *   An associative array with the following options:
 *   name: {element unique name}
 *   url: {url string}
 *   query: {query string}
 *   entity_type: {entity type}
 *   entity_id: {entity id}
 *   derivative: {derivative name}
 *
 *   or FALSE if there is no url mapping. The associative array must complete
 *   a valid identity set.
 */
function hook_tether_stats_url($url, $query) {

  $ret_value = FALSE;
  $base_uri = strtok($url, '?');
  $src = drupal_get_normal_path($base_uri);

  if ($src) {

    $parts = explode('/', $src);

    if (count($parts) == 2 && is_numeric($parts[1])) {

      switch ($parts[0]) {

        case 'user':
        case 'node':
          $ret_value = array(
            'entity_type' => $parts[0],
            'entity_id' => (int) $parts[1],
            'url' => $base_uri,
          );

          if (variable_get('tether_stats_allow_query_string_to_define_new_elements', FALSE)) {

            $ret_value['query'] = $query;
          }
          break;

        case 'term':
          $ret_value = array(
            'entity_type' => 'taxonomy_term',
            'entity_id' => (int) $parts[1],
            'url' => $base_uri,
          );

          if (variable_get('tether_stats_allow_query_string_to_define_new_elements', FALSE)) {

            $ret_value['query'] = $query;
          }
          break;

      }
    }
  }
  return $ret_value;
}

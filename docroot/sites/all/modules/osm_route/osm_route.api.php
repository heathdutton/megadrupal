<?php

/**
 * @file
 * Hooks provided by OSM Route.
 */

/**
 * Filter available nodes before building the route.
 *
 * @param node|poi $poi_nodes_id
 *   The array of available nodes can be filtered before making it available
 *   for route building. Its avail stems mainly from two facts:
 *   (1) Referenced nodes in osm_route_node_presave() can be published or not.
 *       This would allow users to have tentative changes to routes,
 *       and unpublished nodes are normally inaccessible to anonymous users.
 *   (2) An itinerary may contain POIs that aren't accessible to all users
 *       (for example, if node access records are in use), raising to
 *       information exposition about the node the current user doesn't have
 *       access to.
 */
function hook_osm_route_poi_nodes_alter(&$poi_nodes_id) {
  $filtered_poi_nodes_id = array();

  // By default, I remove nodes which are not published or accessible by
  // anonymous users.
  foreach ($poi_nodes_id as $value) {
    $node_point = node_load($value['target_id']);
    // Let's build a rule about the accessibility to the current node.
    $can_view = node_access('view', $node_point, drupal_anonymous_user());
    if ($can_view) {
      // If the node is accessible I add it to the final array.
      $filtered_poi_nodes_id[] = $value;
    }
  }
  $poi_nodes_id = $filtered_poi_nodes_id;
}

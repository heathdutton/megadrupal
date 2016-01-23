<?php

/**
 * Returns a site and the server it's on (or NULL if none found)
 */
function hosting_network_api_get_site_by_url($site_url, $hostname = NULL) {
  // TODO: $hostname search

  $network_nodes = hosting_get_servers('network_node');

  foreach($network_nodes as $server_nid => $network_hostname) {
    $sites = (array) hosting_network_rest_call($server_nid, 'hosting_site');

    // TODO: Replace with the retrieve (by url) REST call once it works
    foreach ($sites as $site) {
      if ($site->title == $site_url) {
        $output = array(
          'server' => $server_nid,
          'site' => hosting_network_rest_call($server_nid, 'hosting_site/get_by_nid', array('nid' => $site->nid), 'POST'),
        );

        return $output;
      }
    }
  }
}

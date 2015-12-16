<?php
/**
 * @file
 * gitlab.features.inc
 */

/**
 * Implements hook_default_wsconfig_type().
 */
function gitlab_default_wsconfig_type() {
  $items = array();
  $items['gitlab'] = entity_import('wsconfig_type', '{
    "type" : "gitlab",
    "label" : "GitLab (v3)",
    "weight" : "0",
    "data" : {
      "endpoint" : "https:\\/\\/gitlab.com\\/api\\/v3",
      "connector" : "restclient_wsconnector"
    }
  }');
  return $items;
}

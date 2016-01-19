<?php
/**
 * @file
 * gitlab_projects.features.inc
 */

/**
 * Implements hook_default_wsconfig().
 */
function gitlab_groups_default_wsconfig() {
  $items = array();
  $items['gitlab_groups'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_groups",
    "title" : "GitLab Groups",
    "created" : "1366894847",
    "changed" : "1366894847",
    "data" : {
      "create_data_method" : "groups",
      "read_data_method" : "groups\\/%id",
      "update_data_method" : "",
      "delete_data_method" : "groups\\/%id",
      "index_data_method"  : "groups",
      "cache_default_time" : 60,
      "cache_default_override" : 0
    }
  }');
  $items['gitlab_groups_members'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_groups_members",
    "title" : "GitLab Group Members",
    "created" : "1366894847",
    "changed" : "1366894847",
    "data" : {
      "create_data_method" : "groups\\/%id\\/members",
      "read_data_method" : "groups\\/%id",
      "update_data_method" : "",
      "delete_data_method" : "groups\\/%id\\/members\\/%user",
      "index_data_method"  : "groups\\/%id\\/members",
      "cache_default_time" : 60,
      "cache_default_override" : 0
    }
  }');
  return $items;
}

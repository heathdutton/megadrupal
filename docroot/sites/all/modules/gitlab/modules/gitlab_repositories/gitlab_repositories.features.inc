<?php
/**
 * @file
 * gitlab_repositories.features.inc
 */

/**
 * Implements hook_default_wsconfig().
 */
function gitlab_repositories_default_wsconfig() {
  $items = array();
  $items['gitlab_repositories_files'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_repositories",
    "title" : "GitLab Repositories Files",
    "created" : "1395703624",
    "changed" : "1395703736",
    "data" : {
      "cache_default_time" : 0,
      "cache_default_override" : 0,
      "read_data_method" : "projects\\/%id\\/repository\\/files?file_path=%file_path&ref=%ref",
      "update_data_method" : "projects\\/%id\\/repository\\/files",
      "create_data_method" : "projects\\/%id\\/repository\\/files",
      "delete_data_method" : "projects\\/%id\\/repository\\/files"
    }
  }');
  $items['gitlab_repository_tree'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_repository_tree",
    "title" : "GitLab Repository Tree",
    "created" : "1395703624",
    "changed" : "1439412237",
    "data" : {
      "cache_default_time" : 0,
      "cache_default_override" : 0,
      "read_data_method" : "projects\\/%id\\/repository\\/tree?path=%path&ref_name=%ref_name"
    }
  }');
  return $items;
}

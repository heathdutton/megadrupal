<?php
/**
 * @file
 * gitlab_users.features.inc
 */

/**
 * Implements hook_default_wsconfig().
 */
function gitlab_users_default_wsconfig() {
  $items = array();
  $items['gitlab_users_current'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_users_current",
    "title" : "GitLab Users Current",
    "created" : "1370191077",
    "changed" : "1370191077",
    "data" : {
      "create_data_method" : "",
      "read_data_method" : "user",
      "update_data_method" : "",
      "delete_data_method" : ""
    }
  }');
  $items['gitlab_users_current_sshkey'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_users_current_sshkey",
    "title" : "GitLab Users Current SSH Key",
    "created" : "1370191077",
    "changed" : "1370191077",
    "data" : {
      "create_data_method" : "user\\/keys",
      "read_data_method" : "user\\/keys\\/%id",
      "update_data_method" : "",
      "delete_data_method" : "user\\/keys\\/%id",
      "index_data_method" : "user\\/keys"
    }
  }');
  $items['gitlab_users'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_users",
    "title" : "GitLab Users",
    "created" : "1366757858",
    "changed" : "1366757858",
    "data" : {
      "create_data_method" : "users",
      "read_data_method" : "users\\/%id",
      "update_data_method" : "users\\/%id",
      "delete_data_method" : "users\\/%id"
    }
  }');
  $items['gitlab_users_sshkey'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_users_sshkey",
    "title" : "GitLab Users SSH Key",
    "created" : "1366757858",
    "changed" : "1366757858",
    "data" : {
      "create_data_method" : "users\\/%id\\/keys",
      "read_data_method" : "",
      "update_data_method" : "",
      "delete_data_method" : ""
    }
  }');
  return $items;
}

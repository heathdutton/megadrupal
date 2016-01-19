<?php
/**
 * @file
 * gitlab_projects.features.inc
 */

/**
 * Implements hook_default_wsconfig().
 */
function gitlab_projects_default_wsconfig() {
  $items = array();
  $items['gitlab_project_branches'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_project_branches",
    "title" : "GitLab Project Branches",
    "created" : "1366894847",
    "changed" : "1366894847",
    "data" : {
      "create_data_method" : "projects\\/%id\\/repository\\/branches",
      "read_data_method" : "projects\\/%id\\/repository\\/branches\\/%branch",
      "update_data_method" : "",
      "delete_data_method" : "",
      "index_data_method" : "projects\\/%id\\/repository\\/branches",
      "cache_default_time" : 60,
      "cache_default_override" : 0
    }
  }');
  $items['gitlab_project_commits'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_project_commits",
    "title" : "GitLab Project Commits",
    "created" : "1366894924",
    "changed" : "1366894924",
    "data" : {
      "create_data_method" : "",
      "read_data_method" : "projects\\/%id\\/repository\\/commits",
      "update_data_method" : "",
      "delete_data_method" : "",
      "cache_default_time" : 60,
      "cache_default_override" : 0
    }
  }');
  $items['gitlab_project_deploy_keys'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_project_deploy_keys",
    "title" : "GitLab Project Deploy Keys",
    "created" : "1366895015",
    "changed" : "1366895015",
    "data" : {
      "index_data_method" : "projects\\/%id\\/keys",
      "create_data_method" : "projects\\/%id\\/keys",
      "read_data_method" : "projects\\/%id\\/keys\\/%key_id",
      "update_data_method" : "",
      "delete_data_method" : "projects\\/%id\\/keys\\/%key_id",
      "cache_default_time" : 60,
      "cache_default_override" : 0
    }
  }');
  $items['gitlab_project_events'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_project_events",
    "title" : "GitLab Project Events",
    "created" : "1366891123",
    "changed" : "1366891123",
    "data" : {
      "read_data_method" : "projects\\/%id\\/events",
      "cache_default_time" : 60,
      "cache_default_override" : 0
    }
  }');
  $items['gitlab_project_hooks'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_project_hooks",
    "title" : "GitLab Project Hooks",
    "created" : "1389642930",
    "changed" : "1389643026",
    "data" : {
      "read_data_method" : "projects\\/%id\\/hooks\\/%hook_id",
      "update_data_method" : "projects\\/%id\\/hooks\\/%hook_id",
      "index_data_method" : "projects\\/%id\\/hooks",
      "create_data_method" : "projects\\/%id\\/hooks",
      "delete_data_method" : "projects\\/%id\\/hooks\\/%hook_id",
      "cache_default_time" : 60,
      "cache_default_override" : 0
    }
  }');
  $items['gitlab_project_tags'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_project_tags",
    "title" : "GitLab Project Tags",
    "created" : "1366894898",
    "changed" : "1366894898",
    "data" : {
      "create_data_method" : "",
      "read_data_method" : "projects\\/%id\\/repository\\/tags",
      "update_data_method" : "",
      "delete_data_method" : "",
      "cache_default_time" : 60,
      "cache_default_override" : 0
    }
  }');
  $items['gitlab_project_team_members'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_project_team_members",
    "title" : "GitLab Project Team Members",
    "created" : "1366892994",
    "changed" : "1366892994",
    "data" : {
      "create_data_method" : "projects\\/%id\\/members",
      "read_data_method" : "projects\\/%id\\/members\\/%uid",
      "update_data_method" : "projects\\/%id\\/members\\/%uid",
      "delete_data_method" : "projects\\/%id\\/members\\/%uid",
      "index_data_method" : "projects\\/%id\\/members",
      "cache_default_time" : 60,
      "cache_default_override" : 0
    }
  }');
  $items['gitlab_projects'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_projects",
    "title" : "GitLab Projects",
    "created" : "1366891123",
    "changed" : "1366891123",
    "data" : {
      "create_data_method" : "projects",
      "read_data_method" : "projects\\/%id",
      "update_data_method" : "projects\\/%id",
      "delete_data_method" : "projects\\/%id",
      "index_data_method" : "projects",
      "cache_default_time" : 60,
      "cache_default_override" : 0
    }
  }');
  $items['gitlab_user_projects'] = entity_import('wsconfig', '{
    "connector" : {},
    "type" : "gitlab",
    "language" : "",
    "name" : "gitlab_user_projects",
    "title" : "GitLab Projects",
    "created" : "1366891123",
    "changed" : "1366891123",
    "data" : {
      "create_data_method" : "projects\\/user\\/%user_id",
      "cache_default_time" : 60,
      "cache_default_override" : 0
    }
  }');
  return $items;
}

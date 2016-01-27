<?php
/**
 * @file
 * recruiter_job_search_notifications.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function recruiter_job_search_notifications_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}

/**
 * Implements hook_views_api().
 */
function recruiter_job_search_notifications_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_default_search_api_saved_searches_settings().
 */
function recruiter_job_search_notifications_default_search_api_saved_searches_settings() {
  $items = array();
  $items['job_search'] = entity_import('search_api_saved_searches_settings', '{
    "delta" : "job_search",
    "index_id" : "job_search",
    "enabled" : "1",
    "options" : {
      "date_field" : "changed",
      "description" : "Get notified for similar jobs!",
      "registered_choose_mail" : 0,
      "choose_name" : 1,
      "registered_user_delete_key" : 1,
      "show_empty_list" : 0,
      "default_true" : "1",
      "ids_list" : [],
      "user_select_interval" : 1,
      "interval_options" : { "86400" : "Daily", "604800" : "Weekly", "2592000" : "Monthly" },
      "set_interval" : "86400",
      "mail" : {
        "activate" : {
          "send" : 1,
          "title" : "Activate your saved search at [site:name]",
          "body" : "A saved search on [site:name] with this e-mail address was created.\\r\\nTo activate this saved search, click the following link:\\r\\n\\r\\n[search-api-saved-search:activate-url]\\r\\n\\r\\nIf you didn\\u0027t create this saved search, just ignore this mail and it will be deleted.\\r\\n\\r\\n--  [site:name] team"
        },
        "notify" : {
          "title" : "New results for your saved search at [site:name]",
          "body" : "[user:name],\\r\\n\\r\\nThere are new results for your saved search on [site:name]:\\r\\n\\r\\n[search-api-saved-searches:results]\\r\\n\\r\\n--  [site:name] team",
          "results" : "New results for search \\u0022[search-api-saved-search:name]\\u0022:\\r\\n[search-api-saved-search:items]\\r\\n\\r\\n[search-api-saved-search:results-capped]\\r\\n\\r\\nIf you want to edit this search, click here:\\r\\n[search-api-saved-search:edit-url]\\r\\n\\r\\nIf you want to delete this saved search, click here:\\r\\n[search-api-saved-search:delete-url]",
          "result" : "[search-api-saved-search-result:label] ([search-api-saved-search-result:url])",
          "max_results" : "25",
          "results_capped" : "\\u2026\\r\\nView all results: [search-api-saved-search:view-url]"
        }
      },
      "manual" : {
        "allow" : 0,
        "fulltext" : 0,
        "fields" : [],
        "page" : { "path" : "", "fulltext" : "", "direct_filter_params" : "0" }
      }
    },
    "rdf_mapping" : []
  }');
  return $items;
}

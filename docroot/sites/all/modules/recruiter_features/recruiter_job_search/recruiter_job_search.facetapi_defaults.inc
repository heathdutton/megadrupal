<?php
/**
 * @file
 * recruiter_job_search.facetapi_defaults.inc
 */

/**
 * Implements hook_facetapi_default_facet_settings().
 */
function recruiter_job_search_facetapi_default_facet_settings() {
  $export = array();

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search::field_job_employment_type_term:parents_all';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = '';
  $facet->facet = 'field_job_employment_type_term:parents_all';
  $facet->enabled = FALSE;
  $facet->settings = array(
    'operator' => 'and',
    'hard_limit' => '20',
    'dependencies' => array(
      'bundle' => 'none',
      'bundle_selected' => array(),
      'roles' => array(),
    ),
    'facet_mincount' => '1',
    'facet_missing' => '0',
    'flatten' => '0',
    'query_type' => 'term',
    'default_true' => TRUE,
    'facet_search_ids' => array(),
    'pretty_paths_alias' => 'type-of-employment',
    'pretty_paths_taxonomy_pathauto' => TRUE,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'employment_type',
  );
  $export['search_api@job_search::field_job_employment_type_term:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search::field_job_languages:parents_all';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = '';
  $facet->facet = 'field_job_languages:parents_all';
  $facet->enabled = FALSE;
  $facet->settings = array(
    'operator' => 'and',
    'hard_limit' => '20',
    'dependencies' => array(
      'bundle' => 'none',
      'bundle_selected' => array(),
      'roles' => array(),
    ),
    'facet_mincount' => '1',
    'facet_missing' => '0',
    'flatten' => '0',
    'query_type' => 'term',
    'default_true' => TRUE,
    'facet_search_ids' => array(),
    'pretty_paths_alias' => 'language',
    'pretty_paths_taxonomy_pathauto' => TRUE,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'languages',
  );
  $export['search_api@job_search::field_job_languages:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search::field_job_location';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = '';
  $facet->facet = 'field_job_location';
  $facet->enabled = FALSE;
  $facet->settings = array(
    'operator' => 'and',
    'hard_limit' => '20',
    'dependencies' => array(
      'bundle' => 'none',
      'bundle_selected' => array(),
      'roles' => array(),
    ),
    'facet_mincount' => '1',
    'facet_missing' => '0',
    'flatten' => 0,
    'query_type' => 'term',
    'default_true' => TRUE,
    'facet_search_ids' => array(),
    'pretty_paths_alias' => 'location',
  );
  $export['search_api@job_search::field_job_location'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search::field_job_occupational_field:parents_all';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = '';
  $facet->facet = 'field_job_occupational_field:parents_all';
  $facet->enabled = FALSE;
  $facet->settings = array(
    'operator' => 'and',
    'hard_limit' => '20',
    'dependencies' => array(
      'bundle' => 'none',
      'bundle_selected' => array(),
      'roles' => array(),
    ),
    'facet_mincount' => '1',
    'facet_missing' => '0',
    'flatten' => '0',
    'query_type' => 'term',
    'default_true' => TRUE,
    'facet_search_ids' => array(),
    'pretty_paths_alias' => 'occupational-field',
    'pretty_paths_taxonomy_pathauto' => TRUE,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'occupational_fields',
  );
  $export['search_api@job_search::field_job_occupational_field:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search::field_job_of_study:parents_all';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = '';
  $facet->facet = 'field_job_of_study:parents_all';
  $facet->enabled = FALSE;
  $facet->settings = array(
    'operator' => 'and',
    'hard_limit' => '20',
    'dependencies' => array(
      'bundle' => 'none',
      'bundle_selected' => array(),
      'roles' => array(),
    ),
    'facet_mincount' => '1',
    'facet_missing' => '0',
    'flatten' => '0',
    'query_type' => 'term',
    'default_true' => '1',
    'facet_search_ids' => array(),
    'pretty_paths_alias' => 'field-of-study',
    'pretty_paths_taxonomy_pathauto' => 1,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'fields_of_study',
  );
  $export['search_api@job_search::field_job_of_study:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search::field_job_organization';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = '';
  $facet->facet = 'field_job_organization';
  $facet->enabled = FALSE;
  $facet->settings = array(
    'operator' => 'and',
    'hard_limit' => '20',
    'dependencies' => array(
      'bundle' => 'none',
      'bundle_selected' => array(),
      'roles' => array(),
    ),
    'facet_mincount' => '1',
    'facet_missing' => '0',
    'flatten' => 0,
    'query_type' => 'term',
    'default_true' => TRUE,
    'facet_search_ids' => array(),
  );
  $export['search_api@job_search::field_job_organization'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search::field_job_region:parents_all';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = '';
  $facet->facet = 'field_job_region:parents_all';
  $facet->enabled = FALSE;
  $facet->settings = array(
    'operator' => 'and',
    'hard_limit' => '100',
    'dependencies' => array(
      'bundle' => 'none',
      'bundle_selected' => array(),
      'roles' => array(),
    ),
    'facet_mincount' => '1',
    'facet_missing' => '0',
    'flatten' => '0',
    'query_type' => 'term',
    'default_true' => TRUE,
    'facet_search_ids' => array(),
    'pretty_paths_alias' => 'region',
    'pretty_paths_taxonomy_pathauto' => TRUE,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'geography',
  );
  $export['search_api@job_search::field_job_region:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search::field_job_skills_general:parents_all';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = '';
  $facet->facet = 'field_job_skills_general:parents_all';
  $facet->enabled = FALSE;
  $facet->settings = array(
    'operator' => 'and',
    'hard_limit' => '20',
    'dependencies' => array(
      'bundle' => 'none',
      'bundle_selected' => array(),
      'roles' => array(),
    ),
    'facet_mincount' => '1',
    'facet_missing' => '0',
    'flatten' => '0',
    'query_type' => 'term',
    'default_true' => TRUE,
    'facet_search_ids' => array(),
    'pretty_paths_alias' => 'general-skill',
    'pretty_paths_taxonomy_pathauto' => TRUE,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'general_skills',
  );
  $export['search_api@job_search::field_job_skills_general:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search::field_job_skills_it:parents_all';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = '';
  $facet->facet = 'field_job_skills_it:parents_all';
  $facet->enabled = FALSE;
  $facet->settings = array(
    'operator' => 'and',
    'hard_limit' => '20',
    'dependencies' => array(
      'bundle' => 'none',
      'bundle_selected' => array(),
      'roles' => array(),
    ),
    'facet_mincount' => '1',
    'facet_missing' => '0',
    'flatten' => '0',
    'query_type' => 'term',
    'default_true' => TRUE,
    'facet_search_ids' => array(),
    'pretty_paths_alias' => 'it-skill',
    'pretty_paths_taxonomy_pathauto' => TRUE,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'it_skills',
  );
  $export['search_api@job_search::field_job_skills_it:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search:block:field_job_employment_type_term:parents_all';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = 'block';
  $facet->facet = 'field_job_employment_type_term:parents_all';
  $facet->enabled = TRUE;
  $facet->settings = array(
    'weight' => 0,
    'widget' => 'facetapi_checkbox_links',
    'filters' => array(),
    'active_sorts' => array(
      'active' => 'active',
      'count' => 'count',
      'display' => 'display',
      'indexed' => 0,
    ),
    'sort_weight' => array(
      'active' => '-50',
      'count' => '-49',
      'display' => '-48',
      'indexed' => '0',
    ),
    'sort_order' => array(
      'active' => '3',
      'count' => '3',
      'display' => '4',
      'indexed' => '4',
    ),
    'empty_behavior' => 'none',
    'soft_limit' => '5',
    'nofollow' => 0,
    'show_expanded' => 0,
    'empty_text' => array(
      'value' => '',
      'format' => 'filtered_html',
    ),
    'submit_realm' => 'Save and go back to realm settings',
    'pretty_paths_alias' => 'type-of-employment',
    'pretty_paths_taxonomy_pathauto' => TRUE,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'employment_type',
  );
  $export['search_api@job_search:block:field_job_employment_type_term:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search:block:field_job_languages:parents_all';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = 'block';
  $facet->facet = 'field_job_languages:parents_all';
  $facet->enabled = TRUE;
  $facet->settings = array(
    'weight' => 0,
    'widget' => 'facetapi_checkbox_links',
    'filters' => array(),
    'active_sorts' => array(
      'active' => 'active',
      'count' => 'count',
      'display' => 'display',
      'indexed' => 0,
    ),
    'sort_weight' => array(
      'active' => '-50',
      'count' => '-49',
      'display' => '-48',
      'indexed' => '0',
    ),
    'sort_order' => array(
      'active' => '3',
      'count' => '3',
      'display' => '4',
      'indexed' => '4',
    ),
    'empty_behavior' => 'none',
    'soft_limit' => '5',
    'nofollow' => 0,
    'show_expanded' => 0,
    'empty_text' => array(
      'value' => '',
      'format' => 'filtered_html',
    ),
    'submit_realm' => 'Save and go back to realm settings',
    'pretty_paths_alias' => 'language',
    'pretty_paths_taxonomy_pathauto' => TRUE,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'languages',
  );
  $export['search_api@job_search:block:field_job_languages:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search:block:field_job_location';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = 'block';
  $facet->facet = 'field_job_location';
  $facet->enabled = TRUE;
  $facet->settings = array(
    'weight' => 0,
    'widget' => 'facetapi_checkbox_links',
    'filters' => array(),
    'active_sorts' => array(
      'active' => 'active',
      'count' => 'count',
      'display' => 'display',
      'indexed' => 0,
    ),
    'sort_weight' => array(
      'active' => '-50',
      'count' => '-49',
      'display' => '-48',
      'indexed' => '0',
    ),
    'sort_order' => array(
      'active' => '3',
      'count' => '3',
      'display' => '4',
      'indexed' => '4',
    ),
    'empty_behavior' => 'none',
    'soft_limit' => '5',
    'nofollow' => 0,
    'show_expanded' => 0,
    'empty_text' => array(
      'value' => '',
      'format' => 'filtered_html',
    ),
    'submit_realm' => 'Save and go back to realm settings',
    'pretty_paths_alias' => 'location',
  );
  $export['search_api@job_search:block:field_job_location'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search:block:field_job_occupational_field:parents_all';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = 'block';
  $facet->facet = 'field_job_occupational_field:parents_all';
  $facet->enabled = TRUE;
  $facet->settings = array(
    'weight' => 0,
    'widget' => 'facetapi_checkbox_links',
    'filters' => array(),
    'active_sorts' => array(
      'active' => 'active',
      'count' => 'count',
      'display' => 'display',
      'indexed' => 0,
    ),
    'sort_weight' => array(
      'active' => '-50',
      'count' => '-49',
      'display' => '-48',
      'indexed' => '0',
    ),
    'sort_order' => array(
      'active' => '3',
      'count' => '3',
      'display' => '4',
      'indexed' => '4',
    ),
    'empty_behavior' => 'none',
    'soft_limit' => '5',
    'nofollow' => 0,
    'show_expanded' => 0,
    'empty_text' => array(
      'value' => '',
      'format' => 'filtered_html',
    ),
    'submit_realm' => 'Save and go back to realm settings',
    'pretty_paths_alias' => 'occupational-field',
    'pretty_paths_taxonomy_pathauto' => TRUE,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'occupational_fields',
  );
  $export['search_api@job_search:block:field_job_occupational_field:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search:block:field_job_of_study:parents_all';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = 'block';
  $facet->facet = 'field_job_of_study:parents_all';
  $facet->enabled = TRUE;
  $facet->settings = array(
    'weight' => 0,
    'widget' => 'facetapi_checkbox_links',
    'filters' => array(),
    'active_sorts' => array(
      'active' => 'active',
      'count' => 'count',
      'display' => 'display',
      'indexed' => 0,
    ),
    'sort_weight' => array(
      'active' => '-50',
      'count' => '-49',
      'display' => '-48',
      'indexed' => '0',
    ),
    'sort_order' => array(
      'active' => '3',
      'count' => '3',
      'display' => '4',
      'indexed' => '4',
    ),
    'empty_behavior' => 'none',
    'soft_limit' => '5',
    'nofollow' => 0,
    'show_expanded' => 0,
    'empty_text' => array(
      'value' => '',
      'format' => 'filtered_html',
    ),
    'submit_realm' => 'Save and go back to realm settings',
    'pretty_paths_alias' => 'field-of-study',
    'pretty_paths_taxonomy_pathauto' => TRUE,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'fields_of_study',
  );
  $export['search_api@job_search:block:field_job_of_study:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search:block:field_job_organization';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = 'block';
  $facet->facet = 'field_job_organization';
  $facet->enabled = TRUE;
  $facet->settings = array(
    'weight' => 0,
    'widget' => 'facetapi_checkbox_links',
    'filters' => array(),
    'active_sorts' => array(
      'active' => 'active',
      'count' => 'count',
      'display' => 'display',
      'indexed' => 0,
    ),
    'sort_weight' => array(
      'active' => '-50',
      'count' => '-49',
      'display' => '-48',
      'indexed' => '0',
    ),
    'sort_order' => array(
      'active' => '3',
      'count' => '3',
      'display' => '4',
      'indexed' => '4',
    ),
    'empty_behavior' => 'none',
    'soft_limit' => '5',
    'nofollow' => 0,
    'show_expanded' => 0,
    'empty_text' => array(
      'value' => '',
      'format' => 'filtered_html',
    ),
    'submit_realm' => 'Save and go back to realm settings',
  );
  $export['search_api@job_search:block:field_job_organization'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search:block:field_job_region:parents_all';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = 'block';
  $facet->facet = 'field_job_region:parents_all';
  $facet->enabled = TRUE;
  $facet->settings = array(
    'weight' => 0,
    'widget' => 'facetapi_checkbox_links',
    'filters' => array(),
    'active_sorts' => array(
      'active' => 'active',
      'count' => 'count',
      'display' => 'display',
      'indexed' => 0,
    ),
    'sort_weight' => array(
      'active' => '-50',
      'count' => '-49',
      'display' => '-48',
      'indexed' => '0',
    ),
    'sort_order' => array(
      'active' => '3',
      'count' => '3',
      'display' => '4',
      'indexed' => '4',
    ),
    'empty_behavior' => 'none',
    'soft_limit' => '5',
    'nofollow' => 0,
    'show_expanded' => 0,
    'empty_text' => array(
      'value' => '',
      'format' => 'filtered_html',
    ),
    'submit_realm' => 'Save and go back to realm settings',
    'pretty_paths_alias' => 'region',
    'pretty_paths_taxonomy_pathauto' => TRUE,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'geography',
  );
  $export['search_api@job_search:block:field_job_region:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search:block:field_job_skills_general:parents_all';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = 'block';
  $facet->facet = 'field_job_skills_general:parents_all';
  $facet->enabled = TRUE;
  $facet->settings = array(
    'weight' => 0,
    'widget' => 'facetapi_checkbox_links',
    'filters' => array(),
    'active_sorts' => array(
      'active' => 'active',
      'count' => 'count',
      'display' => 'display',
      'indexed' => 0,
    ),
    'sort_weight' => array(
      'active' => '-50',
      'count' => '-49',
      'display' => '-48',
      'indexed' => '0',
    ),
    'sort_order' => array(
      'active' => '3',
      'count' => '3',
      'display' => '4',
      'indexed' => '4',
    ),
    'empty_behavior' => 'none',
    'soft_limit' => '5',
    'nofollow' => 0,
    'show_expanded' => 0,
    'empty_text' => array(
      'value' => '',
      'format' => 'filtered_html',
    ),
    'submit_realm' => 'Save and go back to realm settings',
    'pretty_paths_alias' => 'general-skill',
    'pretty_paths_taxonomy_pathauto' => TRUE,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'general_skills',
  );
  $export['search_api@job_search:block:field_job_skills_general:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@job_search:block:field_job_skills_it:parents_all';
  $facet->searcher = 'search_api@job_search';
  $facet->realm = 'block';
  $facet->facet = 'field_job_skills_it:parents_all';
  $facet->enabled = TRUE;
  $facet->settings = array(
    'weight' => 0,
    'widget' => 'facetapi_checkbox_links',
    'filters' => array(),
    'active_sorts' => array(
      'active' => 'active',
      'count' => 'count',
      'display' => 'display',
      'indexed' => 0,
    ),
    'sort_weight' => array(
      'active' => '-50',
      'count' => '-49',
      'display' => '-48',
      'indexed' => '0',
    ),
    'sort_order' => array(
      'active' => '3',
      'count' => '3',
      'display' => '4',
      'indexed' => '4',
    ),
    'empty_behavior' => 'none',
    'soft_limit' => '5',
    'nofollow' => 0,
    'show_expanded' => 0,
    'empty_text' => array(
      'value' => '',
      'format' => 'filtered_html',
    ),
    'submit_realm' => 'Save and go back to realm settings',
    'pretty_paths_alias' => 'it-skill',
    'pretty_paths_taxonomy_pathauto' => TRUE,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'it_skills',
  );
  $export['search_api@job_search:block:field_job_skills_it:parents_all'] = $facet;

  return $export;
}

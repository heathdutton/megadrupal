<?php
/**
 * @file
 * recruiter_resume_search.facetapi_defaults.inc
 */

/**
 * Implements hook_facetapi_default_facet_settings().
 */
function recruiter_resume_search_facetapi_default_facet_settings() {
  $export = array();

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search::field_resume_education:field_resume_of_study:parents_all';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = '';
  $facet->facet = 'field_resume_education:field_resume_of_study:parents_all';
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
    'default_true' => '1',
    'facet_search_ids' => array(),
    'pretty_paths_alias' => 'field-of-study',
    'pretty_paths_taxonomy_pathauto' => 1,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'fields_of_study',
  );
  $export['search_api@resume_search::field_resume_education:field_resume_of_study:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search::field_resume_job_preferences:field_resume_career_status';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = '';
  $facet->facet = 'field_resume_job_preferences:field_resume_career_status';
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
    'pretty_paths_alias' => 'career-status',
    'pretty_paths_taxonomy_pathauto' => 1,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'career_status',
  );
  $export['search_api@resume_search::field_resume_job_preferences:field_resume_career_status'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search::field_resume_job_preferences:field_resume_jobpref_field';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = '';
  $facet->facet = 'field_resume_job_preferences:field_resume_jobpref_field';
  $facet->enabled = FALSE;
  $facet->settings = array(
    'operator' => 'and',
    'hard_limit' => '20',
    'dependencies' => array(
      'roles' => array(),
      'bundle' => 'none',
      'bundle_selected' => array(),
    ),
    'facet_mincount' => '1',
    'facet_missing' => '0',
    'flatten' => '0',
    'default_true' => '1',
    'facet_search_ids' => array(),
    'pretty_paths_alias' => 'job-preferences',
    'pretty_paths_taxonomy_pathauto' => 1,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'occupational_fields',
    'query_type' => 'term',
  );
  $export['search_api@resume_search::field_resume_job_preferences:field_resume_jobpref_field'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search::field_resume_job_preferences:field_resume_region:parents_all';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = '';
  $facet->facet = 'field_resume_job_preferences:field_resume_region:parents_all';
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
    'default_true' => '1',
    'facet_search_ids' => array(),
    'pretty_paths_alias' => 'region',
    'pretty_paths_taxonomy_pathauto' => 1,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'geography',
  );
  $export['search_api@resume_search::field_resume_job_preferences:field_resume_region:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search::field_resume_languages:field_resume_languages_level:term';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = '';
  $facet->facet = 'field_resume_languages:field_resume_languages_level:term';
  $facet->enabled = FALSE;
  $facet->settings = array(
    'operator' => 'and',
    'hard_limit' => '20',
    'dependencies' => array(
      'roles' => array(),
      'bundle' => 'none',
      'bundle_selected' => array(),
    ),
    'facet_mincount' => '1',
    'facet_missing' => '0',
    'flatten' => '0',
    'default_true' => '1',
    'facet_search_ids' => array(),
    'pretty_paths_alias' => 'language',
    'query_type' => 'term',
    'pretty_paths_taxonomy_pathauto' => 1,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'languages',
  );
  $export['search_api@resume_search::field_resume_languages:field_resume_languages_level:term'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search::field_resume_skills_general:field_resume_skills_general_lev:term:parents_all';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = '';
  $facet->facet = 'field_resume_skills_general:field_resume_skills_general_lev:term:parents_all';
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
    'default_true' => '1',
    'facet_search_ids' => array(),
    'pretty_paths_alias' => 'general-skill',
    'pretty_paths_taxonomy_pathauto' => 1,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'general_skills',
  );
  $export['search_api@resume_search::field_resume_skills_general:field_resume_skills_general_lev:term:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search::field_resume_skills_it:field_resume_skills_it_level:term:parents_all';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = '';
  $facet->facet = 'field_resume_skills_it:field_resume_skills_it_level:term:parents_all';
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
    'default_true' => '1',
    'facet_search_ids' => array(),
    'pretty_paths_alias' => 'it-skill',
    'pretty_paths_taxonomy_pathauto' => 1,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'it_skills',
  );
  $export['search_api@resume_search::field_resume_skills_it:field_resume_skills_it_level:term:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search::field_resume_xp:field_resume_xp_occupation_field';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = '';
  $facet->facet = 'field_resume_xp:field_resume_xp_occupation_field';
  $facet->enabled = FALSE;
  $facet->settings = array(
    'operator' => 'and',
    'hard_limit' => '20',
    'dependencies' => array(
      'roles' => array(),
      'bundle' => 'none',
      'bundle_selected' => array(),
    ),
    'facet_mincount' => '1',
    'facet_missing' => '0',
    'flatten' => '0',
    'default_true' => '1',
    'facet_search_ids' => array(),
    'pretty_paths_alias' => 'occupation-field',
    'pretty_paths_taxonomy_pathauto' => 1,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'occupational_fields',
    'query_type' => 'term',
  );
  $export['search_api@resume_search::field_resume_xp:field_resume_xp_occupation_field'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search::field_resume_xp:field_resume_xp_organization';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = '';
  $facet->facet = 'field_resume_xp:field_resume_xp_organization';
  $facet->enabled = FALSE;
  $facet->settings = array(
    'operator' => 'and',
    'hard_limit' => '20',
    'dependencies' => array(
      'roles' => array(),
      'bundle' => 'none',
      'bundle_selected' => array(),
    ),
    'facet_mincount' => '1',
    'facet_missing' => '0',
    'flatten' => 0,
    'default_true' => '1',
    'facet_search_ids' => array(),
    'pretty_paths_alias' => 'organization',
    'query_type' => 'term',
  );
  $export['search_api@resume_search::field_resume_xp:field_resume_xp_organization'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search:block:field_resume_education:field_resume_of_study:parents_all';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = 'block';
  $facet->facet = 'field_resume_education:field_resume_of_study:parents_all';
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
  $export['search_api@resume_search:block:field_resume_education:field_resume_of_study:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search:block:field_resume_job_preferences:field_resume_career_status';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = 'block';
  $facet->facet = 'field_resume_job_preferences:field_resume_career_status';
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
    'pretty_paths_alias' => 'career-status',
    'pretty_paths_taxonomy_pathauto' => TRUE,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'career_status',
  );
  $export['search_api@resume_search:block:field_resume_job_preferences:field_resume_career_status'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search:block:field_resume_job_preferences:field_resume_jobpref_field';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = 'block';
  $facet->facet = 'field_resume_job_preferences:field_resume_jobpref_field';
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
    'show_expanded' => 0,
    'empty_text' => array(
      'value' => '',
      'format' => 'filtered_html',
    ),
    'submit_realm' => 'Save and go back to realm settings',
    'nofollow' => 0,
    'pretty_paths_alias' => 'job-preferences',
    'pretty_paths_taxonomy_pathauto' => TRUE,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'occupational_fields',
  );
  $export['search_api@resume_search:block:field_resume_job_preferences:field_resume_jobpref_field'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search:block:field_resume_job_preferences:field_resume_region:parents_all';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = 'block';
  $facet->facet = 'field_resume_job_preferences:field_resume_region:parents_all';
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
  $export['search_api@resume_search:block:field_resume_job_preferences:field_resume_region:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search:block:field_resume_languages:field_resume_languages_level:term';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = 'block';
  $facet->facet = 'field_resume_languages:field_resume_languages_level:term';
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
    'show_expanded' => 0,
    'empty_text' => array(
      'value' => '',
      'format' => 'filtered_html',
    ),
    'submit_realm' => 'Save and go back to realm settings',
    'nofollow' => 0,
    'pretty_paths_alias' => 'language',
  );
  $export['search_api@resume_search:block:field_resume_languages:field_resume_languages_level:term'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search:block:field_resume_skills_general:field_resume_skills_general_lev:term:parents_all';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = 'block';
  $facet->facet = 'field_resume_skills_general:field_resume_skills_general_lev:term:parents_all';
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
  );
  $export['search_api@resume_search:block:field_resume_skills_general:field_resume_skills_general_lev:term:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search:block:field_resume_skills_it:field_resume_skills_it_level:term:parents_all';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = 'block';
  $facet->facet = 'field_resume_skills_it:field_resume_skills_it_level:term:parents_all';
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
  );
  $export['search_api@resume_search:block:field_resume_skills_it:field_resume_skills_it_level:term:parents_all'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search:block:field_resume_xp:field_resume_xp_occupation_field';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = 'block';
  $facet->facet = 'field_resume_xp:field_resume_xp_occupation_field';
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
    'show_expanded' => 0,
    'empty_text' => array(
      'value' => '',
      'format' => 'filtered_html',
    ),
    'submit_realm' => 'Save and go back to realm settings',
    'nofollow' => 0,
    'pretty_paths_alias' => 'occupation-field',
    'pretty_paths_taxonomy_pathauto' => TRUE,
    'pretty_paths_taxonomy_pathauto_vocabulary' => 'occupational_fields',
  );
  $export['search_api@resume_search:block:field_resume_xp:field_resume_xp_occupation_field'] = $facet;

  $facet = new stdClass();
  $facet->disabled = FALSE; /* Edit this to true to make a default facet disabled initially */
  $facet->api_version = 1;
  $facet->name = 'search_api@resume_search:block:field_resume_xp:field_resume_xp_organization';
  $facet->searcher = 'search_api@resume_search';
  $facet->realm = 'block';
  $facet->facet = 'field_resume_xp:field_resume_xp_organization';
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
    'show_expanded' => 0,
    'empty_text' => array(
      'value' => '',
      'format' => 'filtered_html',
    ),
    'submit_realm' => 'Save and go back to realm settings',
    'nofollow' => 0,
    'pretty_paths_alias' => 'organization',
  );
  $export['search_api@resume_search:block:field_resume_xp:field_resume_xp_organization'] = $facet;

  return $export;
}

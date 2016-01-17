<?php
/**
 * @file
 * recruiter_search.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function recruiter_search_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "current_search" && $api == "current_search") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_default_search_api_server().
 */
function recruiter_search_default_search_api_server() {
  $items = array();
  $items['db_server'] = entity_import('search_api_server', '{
    "name" : "DB server",
    "machine_name" : "db_server",
    "description" : "",
    "class" : "search_api_db_service",
    "options" : {
      "database" : "default:default",
      "min_chars" : "3",
      "indexes" : {
        "job_search" : {
          "type" : {
            "table" : "search_api_db_job_search_type",
            "type" : "string",
            "boost" : "1.0"
          },
          "status" : {
            "table" : "search_api_db_job_search_status",
            "type" : "boolean",
            "boost" : "1.0"
          },
          "promote" : {
            "table" : "search_api_db_job_search_promote",
            "type" : "boolean",
            "boost" : "1.0"
          },
          "sticky" : {
            "table" : "search_api_db_job_search_sticky",
            "type" : "boolean",
            "boost" : "1.0"
          },
          "created" : {
            "table" : "search_api_db_job_search_created",
            "type" : "date",
            "boost" : "1.0"
          },
          "changed" : {
            "table" : "search_api_db_job_search_changed",
            "type" : "date",
            "boost" : "1.0"
          },
          "author" : {
            "table" : "search_api_db_job_search_author",
            "type" : "integer",
            "boost" : "1.0"
          },
          "field_job_employment_type_term" : {
            "table" : "search_api_db_job_search_field_job_employment_type_term",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_job_experience_term" : {
            "table" : "search_api_db_job_search_field_job_experience_term",
            "type" : "integer",
            "boost" : "1.0"
          },
          "field_job_languages" : {
            "table" : "search_api_db_job_search_field_job_languages",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_job_location" : {
            "table" : "search_api_db_job_search_field_job_location",
            "type" : "string",
            "boost" : "1.0"
          },
          "field_job_occupational_field" : {
            "table" : "search_api_db_job_search_field_job_occupational_field",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_job_of_study" : {
            "table" : "search_api_db_job_search_field_job_of_study",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_job_organization" : {
            "table" : "search_api_db_job_search_field_job_organization",
            "type" : "string",
            "boost" : "1.0"
          },
          "field_job_region" : {
            "table" : "search_api_db_job_search_field_job_region",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_job_skills_general" : {
            "table" : "search_api_db_job_search_field_job_skills_general",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_job_skills_it" : {
            "table" : "search_api_db_job_search_field_job_skills_it",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_job_workflow_state" : {
            "table" : "search_api_db_job_search_field_job_workflow_state",
            "type" : "string",
            "boost" : "1.0"
          },
          "search_api_language" : {
            "table" : "search_api_db_job_search_search_api_language",
            "type" : "string",
            "boost" : "1.0"
          },
          "search_api_viewed" : {
            "table" : "search_api_db_job_search_search_api_viewed",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_job_employment_type_term:parents_all" : {
            "table" : "search_api_db_job_search_field_job_employment_type_term_parents",
            "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
            "boost" : "1.0"
          },
          "field_job_languages:parents_all" : {
            "table" : "search_api_db_job_search_field_job_languages_parents_all",
            "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
            "boost" : "1.0"
          },
          "field_job_occupational_field:parents_all" : {
            "table" : "search_api_db_job_search_field_job_occupational_field_parents_a",
            "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
            "boost" : "1.0"
          },
          "field_job_of_study:parents_all" : {
            "table" : "search_api_db_job_search_field_job_of_study_parents_all",
            "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
            "boost" : "1.0"
          },
          "field_job_region:parents_all" : {
            "table" : "search_api_db_job_search_field_job_region_parents_all",
            "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
            "boost" : "1.0"
          },
          "field_job_skills_general:parents_all" : {
            "table" : "search_api_db_job_search_field_job_skills_general_parents_all",
            "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
            "boost" : "1.0"
          },
          "field_job_skills_it:parents_all" : {
            "table" : "search_api_db_job_search_field_job_skills_it_parents_all",
            "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
            "boost" : "1.0"
          }
        },
        "resume_search" : {
          "changed" : {
            "table" : "search_api_db_resume_search_changed",
            "type" : "date",
            "boost" : "1.0"
          },
          "field_resume_state" : {
            "table" : "search_api_db_resume_search_field_resume_state",
            "type" : "string",
            "boost" : "1.0"
          },
          "search_api_language" : {
            "table" : "search_api_db_resume_search_search_api_language",
            "type" : "string",
            "boost" : "1.0"
          },
          "search_api_viewed" : {
            "table" : "search_api_db_resume_search_search_api_viewed",
            "type" : "text",
            "boost" : "1.0"
          },
          "field_resume_job_preferences:field_resume_career_status" : {
            "table" : "search_api_db_resume_search_field_resume_job_preferences_field_",
            "type" : "integer",
            "boost" : "1.0"
          },
          "field_resume_job_preferences:field_resume_jobpref_field" : {
            "table" : "search_api_db_resume_search_field_resume_job_preferences_fiel_1",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_resume_xp:field_resume_xp_occupation_field" : {
            "table" : "search_api_db_resume_search_field_resume_xp_field_resume_xp_occ",
            "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
            "boost" : "1.0"
          },
          "field_resume_xp:field_resume_xp_organization" : {
            "table" : "search_api_db_resume_search_field_resume_xp_field_resume_xp_org",
            "type" : "list\\u003Cstring\\u003E",
            "boost" : "1.0"
          },
          "field_resume_education:field_resume_of_study:parents_all" : {
            "table" : "search_api_db_resume_search_field_resume_education_field_resume",
            "type" : "list\\u003Clist\\u003Clist\\u003Cinteger\\u003E\\u003E\\u003E",
            "boost" : "1.0"
          },
          "field_resume_job_preferences:field_resume_region:parents_all" : {
            "table" : "search_api_db_resume_search_field_resume_job_preferences_fiel_2",
            "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
            "boost" : "1.0"
          },
          "field_resume_languages:field_resume_languages_level:term" : {
            "table" : "search_api_db_resume_search_field_resume_languages_field_resume",
            "type" : "list\\u003Cinteger\\u003E",
            "boost" : "1.0"
          },
          "field_resume_skills_general:field_resume_skills_general_lev:term:parents_all" : {
            "table" : "search_api_db_resume_search_field_resume_skills_general_field_r",
            "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
            "boost" : "1.0"
          },
          "field_resume_skills_it:field_resume_skills_it_level:term:parents_all" : {
            "table" : "search_api_db_resume_search_field_resume_skills_it_field_resume",
            "type" : "list\\u003Clist\\u003Cinteger\\u003E\\u003E",
            "boost" : "1.0"
          }
        }
      }
    },
    "enabled" : "1",
    "rdf_mapping" : []
  }');
  return $items;
}

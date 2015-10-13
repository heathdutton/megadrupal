<?php
/**
 * @file
 * recruiter_resume.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function recruiter_resume_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}

/**
 * Implements hook_views_api().
 */
function recruiter_resume_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_flag_default_flags().
 */
function recruiter_resume_flag_default_flags() {
  $flags = array();
  // Exported flag: "Bookmark resume".
  $flags['resume_bookmarks'] = array(
    'entity_type' => 'profile2',
    'title' => 'Bookmark resume',
    'global' => '0',
    'types' => array(
      0 => 'resume',
    ),
    'flag_short' => 'Bookmark resume',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Unmark resume',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'account' => 'account',
      'diff_standard' => 'diff_standard',
      'page' => 'page',
      'teaser' => 'teaser',
      'token' => 'token',
    ),
    'show_as_field' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => 0,
    'module' => 'recruiter_resume',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  return $flags;

}

/**
 * Implements hook_default_profile2_type().
 */
function recruiter_resume_default_profile2_type() {
  $items = array();
  $items['resume'] = entity_import('profile2_type', '{
    "userCategory" : false,
    "userView" : false,
    "type" : "resume",
    "label" : "Resume",
    "weight" : "0",
    "data" : { "use_page" : 1, "page_path" : "resume" },
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_default_rules_link().
 */
function recruiter_resume_default_rules_link() {
  $items = array();
  $items['resume_init_visible'] = entity_import('rules_link', '{
    "settings" : {
      "text" : "Show resume",
      "link_type" : "confirm",
      "bundles" : { "resume" : "resume" },
      "entity_link" : 0,
      "confirm_question" : "Resume activation",
      "confirm_description" : "You have completed all necessary information and can now activate your resume. Active resumes are visible to recruiters."
    },
    "name" : "resume_init_visible",
    "label" : "Resume init visible",
    "path" : "resume_init_visible",
    "entity_type" : "profile2",
    "rdf_mapping" : []
  }');
  $items['resume_set_hidden'] = entity_import('rules_link', '{
    "settings" : {
      "text" : "Hide resume",
      "link_type" : "confirm",
      "bundles" : { "resume" : "resume" },
      "entity_link" : 0,
      "confirm_question" : "Are you sure you want to hide your resume?",
      "confirm_description" : "Hiding your resume will exclude you from searches and being contacted by companies. You can always re-activate your resume. "
    },
    "name" : "resume_set_hidden",
    "label" : "Resume set hidden",
    "path" : "resume_set_hidden",
    "entity_type" : "profile2",
    "rdf_mapping" : []
  }');
  $items['resume_set_visible'] = entity_import('rules_link', '{
    "settings" : {
      "text" : "Show resume",
      "link_type" : "token",
      "bundles" : { "resume" : "resume" },
      "entity_link" : 0
    },
    "name" : "resume_set_visible",
    "label" : "Resume set visible",
    "path" : "resume_set_visible",
    "entity_type" : "profile2",
    "rdf_mapping" : []
  }');
  return $items;
}

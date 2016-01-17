<?php
/**
 * @file
 * recruiter_job_application.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function recruiter_job_application_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function recruiter_job_application_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_default_message_type().
 */
function recruiter_job_application_default_message_type() {
  $items = array();
  $items['job_application_recruiter'] = entity_import('message_type', '{
    "name" : "job_application_recruiter",
    "description" : "Job application",
    "argument_keys" : [],
    "argument" : [],
    "category" : "message_type",
    "data" : {
      "token options" : { "clear" : 1 },
      "purge" : { "override" : 0, "enabled" : 0, "quota" : "", "days" : "" }
    },
    "language" : "en",
    "arguments" : null,
    "message_text" : {
      "de" : [
        {
          "value" : "\\u003Ca href=\\u0022[message:user:profile-resume:url]\\u0022\\u003E[message:user:profile-resume:field-resume-personal-info:field_resume_firstname] [message:user:profile-resume:field-resume-personal-info:field_resume_lastname]\\u003C\\/a\\u003E hat eine neue \\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003EBewerbung\\u003C\\/a\\u003E f\\u00fcr Ihren Job \\u003Ca href=\\u0022[message:field-job-application:field-job-application-job-ref:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E gesendet.",
          "format" : "full_html",
          "safe_value" : "\\u003Cp\\u003E\\u003Ca href=\\u0022[message:user:profile-resume:url]\\u0022\\u003E[message:user:profile-resume:field-resume-personal-info:field_resume_firstname] [message:user:profile-resume:field-resume-personal-info:field_resume_lastname]\\u003C\\/a\\u003E hat eine neue \\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003EBewerbung\\u003C\\/a\\u003E f\\u00fcr Ihren Job \\u003Ca href=\\u0022[message:field-job-application:field-job-application-job-ref:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E gesendet.\\u003C\\/p\\u003E\\n"
        }
      ],
      "en" : [
        {
          "value" : "\\u003Ca href=\\u0022[message:user:profile-resume:url]\\u0022\\u003E[message:user:profile-resume:field-resume-personal-info:field_resume_firstname] [message:user:profile-resume:field-resume-personal-info:field_resume_lastname]\\u003C\\/a\\u003E has sent you a new \\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003Eapplication\\u003C\\/a\\u003E for your job \\u003Ca href=\\u0022[message:field-job-application:field-job-application-job-ref:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E.",
          "format" : "full_html",
          "safe_value" : "\\u003Cp\\u003E\\u003Ca href=\\u0022[message:user:profile-resume:url]\\u0022\\u003E[message:user:profile-resume:field-resume-personal-info:field_resume_firstname] [message:user:profile-resume:field-resume-personal-info:field_resume_lastname]\\u003C\\/a\\u003E has sent you a new \\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003Eapplication\\u003C\\/a\\u003E for your job \\u003Ca href=\\u0022[message:field-job-application:field-job-application-job-ref:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E.\\u003C\\/p\\u003E\\n"
        }
      ],
      "und" : [
        {
          "value" : "\\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003E[message:user:profile-resume:field-resume-personal-info:field_resume_firstname] [message:user:profile-resume:field-resume-personal-info:field_resume_lastname]\\u003C\\/a\\u003E has sent you a new application for your job \\u003Ca href=\\u0022[message:field-job-application:field-job-application-job-ref:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E.",
          "format" : "full_html",
          "safe_value" : "\\u003Cp\\u003E\\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003E[message:user:profile-resume:field-resume-personal-info:field_resume_firstname] [message:user:profile-resume:field-resume-personal-info:field_resume_lastname]\\u003C\\/a\\u003E has sent you a new application for your job \\u003Ca href=\\u0022[message:field-job-application:field-job-application-job-ref:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E.\\u003C\\/p\\u003E\\n"
        }
      ]
    },
    "rdf_mapping" : []
  }');
  $items['job_application_rejected'] = entity_import('message_type', '{
    "name" : "job_application_rejected",
    "description" : "Job application rejected",
    "argument_keys" : [ "@reason", "@reasonmsg" ],
    "argument" : [],
    "category" : "message_type",
    "data" : {
      "token options" : { "clear" : 1 },
      "purge" : { "override" : 0, "enabled" : 0, "quota" : "", "days" : "" }
    },
    "language" : "en",
    "arguments" : null,
    "message_text" : {
      "de" : [
        {
          "value" : "Deine Bewerbung f\\u00fcr \\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E wurde abgelehnt (Begr\\u00fcndung: @reason).",
          "format" : "full_html",
          "safe_value" : "\\u003Cp\\u003EDeine Bewerbung f\\u00fcr \\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E wurde abgelehnt (Begr\\u00fcndung: @reason).\\u003C\\/p\\u003E\\n"
        }
      ],
      "en" : [
        {
          "value" : "Your application for \\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E has been rejected (Reason: @reason).",
          "format" : "full_html",
          "safe_value" : "\\u003Cp\\u003EYour application for \\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E has been rejected (Reason: @reason).\\u003C\\/p\\u003E\\n"
        }
      ],
      "und" : [
        {
          "value" : "Your application for \\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E has been rejected (Reason: @reason).",
          "format" : "full_html",
          "safe_value" : "\\u003Cp\\u003EYour application for \\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E has been rejected (Reason: @reason).\\u003C\\/p\\u003E\\n"
        }
      ]
    },
    "rdf_mapping" : []
  }');
  $items['job_application_shortlisted'] = entity_import('message_type', '{
    "name" : "job_application_shortlisted",
    "description" : "Job application shortlisted",
    "argument_keys" : [],
    "argument" : [],
    "category" : "message_type",
    "data" : {
      "token options" : { "clear" : 1 },
      "purge" : { "override" : 0, "enabled" : 0, "quota" : "", "days" : "" }
    },
    "language" : "en",
    "arguments" : null,
    "message_text" : {
      "de" : [
        {
          "value" : "Deine Bewerbung f\\u00fcr \\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E wurde vorgemerkt.",
          "format" : "full_html",
          "safe_value" : "\\u003Cp\\u003EDeine Bewerbung f\\u00fcr \\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E wurde vorgemerkt.\\u003C\\/p\\u003E\\n"
        }
      ],
      "en" : [
        {
          "value" : "Your application for \\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E has been shortlisted.",
          "format" : "full_html",
          "safe_value" : "\\u003Cp\\u003EYour application for \\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E has been shortlisted.\\u003C\\/p\\u003E\\n"
        }
      ],
      "und" : [
        {
          "value" : "Your application for \\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E has been shortlisted.",
          "format" : "full_html",
          "safe_value" : "\\u003Cp\\u003EYour application for \\u003Ca href=\\u0022[message:field-job-application:url]\\u0022\\u003E[message:field-job-application:field-job-application-job-ref:title]\\u003C\\/a\\u003E has been shortlisted.\\u003C\\/p\\u003E\\n"
        }
      ]
    },
    "rdf_mapping" : []
  }');
  return $items;
}

/**
 * Implements hook_node_info().
 */
function recruiter_job_application_node_info() {
  $items = array(
    'job_application' => array(
      'name' => t('Job application'),
      'base' => 'node_content',
      'description' => t('Create a new job application for job offering.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implements hook_default_rules_link().
 */
function recruiter_job_application_default_rules_link() {
  $items = array();
  $items['job_application_reject'] = entity_import('rules_link', '{
    "settings" : {
      "text" : "Reject application",
      "link_type" : "confirm",
      "bundles" : { "job_application" : "job_application" },
      "entity_link" : 0,
      "confirm_question" : "Reject application",
      "confirm_description" : ""
    },
    "name" : "job_application_reject",
    "label" : "Job application reject",
    "path" : "job_application_reject",
    "entity_type" : "node",
    "rdf_mapping" : []
  }');
  $items['job_application_shortlist'] = entity_import('rules_link', '{
    "settings" : {
      "text" : "Shortlist application",
      "link_type" : "token",
      "bundles" : { "job_application" : "job_application" },
      "entity_link" : 0
    },
    "name" : "job_application_shortlist",
    "label" : "Job application shortlist",
    "path" : "job_application_shortlist",
    "entity_type" : "node",
    "rdf_mapping" : []
  }');
  return $items;
}

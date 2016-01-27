<?php
/**
 * @file
 * recruiter_job_application.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function recruiter_job_application_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_job_application';
  $strongarm->value = 0;
  $export['comment_anonymous_job_application'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_job_application';
  $strongarm->value = 1;
  $export['comment_default_mode_job_application'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_job_application';
  $strongarm->value = '50';
  $export['comment_default_per_page_job_application'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_job_application';
  $strongarm->value = 1;
  $export['comment_form_location_job_application'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_job_application';
  $strongarm->value = '2';
  $export['comment_job_application'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_job_application';
  $strongarm->value = '1';
  $export['comment_preview_job_application'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_job_application';
  $strongarm->value = 1;
  $export['comment_subject_field_job_application'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_access_job_application';
  $strongarm->value = array(
    'view' => array(
      0 => 30037204,
    ),
    'view_own' => array(
      0 => 115362423,
      1 => 30037204,
    ),
    'per_node' => 1,
    'priority' => '0',
  );
  $export['content_access_job_application'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_message__job_application_rejected';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'metatags' => array(
          'weight' => '2',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_message__job_application_rejected'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_message__job_application_shortlisted';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'metatags' => array(
          'weight' => '2',
        ),
      ),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_message__job_application_shortlisted'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__job_application';
  $strongarm->value = array(
    'view_modes' => array(
      'teaser' => array(
        'custom_settings' => TRUE,
      ),
      'full' => array(
        'custom_settings' => FALSE,
      ),
      'rss' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
      'diff_standard' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '0',
        ),
        'redirect' => array(
          'weight' => '1',
        ),
        'metatags' => array(
          'weight' => '9',
        ),
        'path' => array(
          'weight' => '8',
        ),
        'xmlsitemap' => array(
          'weight' => '8',
        ),
      ),
      'display' => array(
        'rules_links_job_application_shortlist' => array(
          'default' => array(
            'weight' => '-10',
            'visible' => TRUE,
          ),
        ),
        'rules_links_job_application_reject' => array(
          'default' => array(
            'weight' => '-9',
            'visible' => TRUE,
          ),
        ),
      ),
    ),
  );
  $export['field_bundle_settings_node__job_application'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_job_application';
  $strongarm->value = '0';
  $export['language_content_type_job_application'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_job_application';
  $strongarm->value = array();
  $export['menu_options_job_application'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_job_application';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_job_application'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_job_application';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_job_application'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_job_application';
  $strongarm->value = '0';
  $export['node_preview_job_application'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_job_application';
  $strongarm->value = 1;
  $export['node_submitted_job_application'] = $strongarm;

  return $export;
}

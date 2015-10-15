<?php
/**
 * @file
 * openacadept_main.ds.inc
 */

/**
 * Implements hook_ds_field_settings_info().
 */
function openacadept_main_ds_field_settings_info() {
  $export = array();

  $ds_fieldsetting = new stdClass;
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|event|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'event';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'comments' => array(
      'weight' => '8',
      'label' => 'hidden',
      'format' => 'default',
    ),
  );
  $export['node|event|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass;
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|event|teaser';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'event';
  $ds_fieldsetting->view_mode = 'teaser';
  $ds_fieldsetting->settings = array(
    'title' => array(
      'weight' => '1',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'link' => '1',
        'wrapper' => 'h3',
        'class' => '',
      ),
    ),
    'links' => array(
      'weight' => '6',
      'label' => 'hidden',
      'format' => 'default',
    ),
  );
  $export['node|event|teaser'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass;
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|project|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'project';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'comments' => array(
      'weight' => '9',
      'label' => 'hidden',
      'format' => 'default',
    ),
  );
  $export['node|project|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass;
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|project|teaser';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'project';
  $ds_fieldsetting->view_mode = 'teaser';
  $ds_fieldsetting->settings = array(
    'title' => array(
      'weight' => '1',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'link' => '1',
        'wrapper' => 'h3',
        'class' => '',
      ),
    ),
    'links' => array(
      'weight' => '7',
      'label' => 'hidden',
      'format' => 'default',
    ),
  );
  $export['node|project|teaser'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass;
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|publication|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'publication';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'comments' => array(
      'weight' => '7',
      'label' => 'hidden',
      'format' => 'default',
    ),
  );
  $export['node|publication|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass;
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|publication|teaser';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'publication';
  $ds_fieldsetting->view_mode = 'teaser';
  $ds_fieldsetting->settings = array(
    'title' => array(
      'weight' => '0',
      'label' => 'hidden',
      'format' => 'default',
      'formatter_settings' => array(
        'link' => '1',
        'wrapper' => 'h3',
        'class' => '',
      ),
    ),
    'links' => array(
      'weight' => '5',
      'label' => 'hidden',
      'format' => 'default',
    ),
  );
  $export['node|publication|teaser'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass;
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'node|resource|default';
  $ds_fieldsetting->entity_type = 'node';
  $ds_fieldsetting->bundle = 'resource';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'comments' => array(
      'weight' => '3',
      'label' => 'hidden',
      'format' => 'default',
    ),
  );
  $export['node|resource|default'] = $ds_fieldsetting;

  $ds_fieldsetting = new stdClass;
  $ds_fieldsetting->api_version = 1;
  $ds_fieldsetting->id = 'user|user|default';
  $ds_fieldsetting->entity_type = 'user';
  $ds_fieldsetting->bundle = 'user';
  $ds_fieldsetting->view_mode = 'default';
  $ds_fieldsetting->settings = array(
    'biography_header' => array(
      'weight' => '13',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'contact_information_header' => array(
      'weight' => '6',
      'label' => 'hidden',
      'format' => 'default',
    ),
    'ds_user_picture' => array(
      'weight' => '12',
      'label' => 'hidden',
      'format' => 'ds_picture_medium',
    ),
  );
  $export['user|user|default'] = $ds_fieldsetting;

  return $export;
}

/**
 * Implements hook_ds_custom_fields_info().
 */
function openacadept_main_ds_custom_fields_info() {
  $export = array();

  $ds_field = new stdClass;
  $ds_field->api_version = 1;
  $ds_field->field = 'biography_header';
  $ds_field->label = 'Biography Header';
  $ds_field->field_type = 5;
  $ds_field->entities = array(
    'user' => 'user',
  );
  $ds_field->properties = array(
    'code' => array(
      'value' => '<h3>Biography</h3>',
      'format' => 'ds_code',
    ),
    'use_token' => 0,
  );
  $export['biography_header'] = $ds_field;

  $ds_field = new stdClass;
  $ds_field->api_version = 1;
  $ds_field->field = 'contact_information_header';
  $ds_field->label = 'Contact Information Header';
  $ds_field->field_type = 5;
  $ds_field->entities = array(
    'user' => 'user',
  );
  $ds_field->properties = array(
    'code' => array(
      'value' => '<h3>Contact Information</h3>',
      'format' => 'ds_code',
    ),
    'use_token' => 0,
  );
  $export['contact_information_header'] = $ds_field;

  return $export;
}

/**
 * Implements hook_ds_layout_settings_info().
 */
function openacadept_main_ds_layout_settings_info() {
  $export = array();

  $ds_layout = new stdClass;
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|event|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'event';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_2col_stacked_fluid';
  $ds_layout->settings = array(
    'hide_empty_regions' => 1,
    'hide_sidebars' => 0,
    'regions' => array(
      'left' => array(
        0 => 'body',
      ),
      'right' => array(
        0 => 'field_event_image',
        1 => 'field_event_place',
        2 => 'field_event_date',
        3 => 'field_event_duration',
        4 => 'field_event_audience',
        5 => 'field_event_admission',
        6 => 'field_tags',
      ),
      'footer' => array(
        0 => 'comments',
      ),
    ),
    'fields' => array(
      'body' => 'left',
      'field_event_image' => 'right',
      'field_event_place' => 'right',
      'field_event_date' => 'right',
      'field_event_duration' => 'right',
      'field_event_audience' => 'right',
      'field_event_admission' => 'right',
      'field_tags' => 'right',
      'comments' => 'footer',
    ),
    'classes' => array(),
  );
  $export['node|event|default'] = $ds_layout;

  $ds_layout = new stdClass;
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|event|teaser';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'event';
  $ds_layout->view_mode = 'teaser';
  $ds_layout->layout = 'ds_2col';
  $ds_layout->settings = array(
    'hide_empty_regions' => 1,
    'hide_sidebars' => 0,
    'regions' => array(
      'left' => array(
        0 => 'field_event_image',
      ),
      'right' => array(
        0 => 'title',
        1 => 'field_event_place',
        2 => 'field_event_date',
        3 => 'field_event_audience',
        4 => 'body',
        5 => 'links',
      ),
    ),
    'fields' => array(
      'field_event_image' => 'left',
      'title' => 'right',
      'field_event_place' => 'right',
      'field_event_date' => 'right',
      'field_event_audience' => 'right',
      'body' => 'right',
      'links' => 'right',
    ),
    'classes' => array(),
  );
  $export['node|event|teaser'] = $ds_layout;

  $ds_layout = new stdClass;
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|project|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'project';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_2col_stacked_fluid';
  $ds_layout->settings = array(
    'hide_empty_regions' => 1,
    'hide_sidebars' => 0,
    'regions' => array(
      'left' => array(
        0 => 'body',
      ),
      'right' => array(
        0 => 'field_project_image',
        1 => 'field_project_head',
        2 => 'field_project_participants',
        3 => 'field_project_customer',
        4 => 'field_project_grant',
        5 => 'field_project_url',
        6 => 'field_project_research_topics',
        7 => 'field_project_files',
      ),
      'footer' => array(
        0 => 'comments',
      ),
    ),
    'fields' => array(
      'body' => 'left',
      'field_project_image' => 'right',
      'field_project_head' => 'right',
      'field_project_participants' => 'right',
      'field_project_customer' => 'right',
      'field_project_grant' => 'right',
      'field_project_url' => 'right',
      'field_project_research_topics' => 'right',
      'field_project_files' => 'right',
      'comments' => 'footer',
    ),
    'classes' => array(),
  );
  $export['node|project|default'] = $ds_layout;

  $ds_layout = new stdClass;
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|project|teaser';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'project';
  $ds_layout->view_mode = 'teaser';
  $ds_layout->layout = 'ds_2col';
  $ds_layout->settings = array(
    'hide_empty_regions' => 1,
    'hide_sidebars' => 0,
    'regions' => array(
      'left' => array(
        0 => 'field_project_image',
      ),
      'right' => array(
        0 => 'title',
        1 => 'field_project_head',
        2 => 'field_project_participants',
        3 => 'field_project_customer',
        4 => 'field_project_research_topics',
        5 => 'body',
        6 => 'links',
      ),
    ),
    'fields' => array(
      'field_project_image' => 'left',
      'title' => 'right',
      'field_project_head' => 'right',
      'field_project_participants' => 'right',
      'field_project_customer' => 'right',
      'field_project_research_topics' => 'right',
      'body' => 'right',
      'links' => 'right',
    ),
    'classes' => array(),
  );
  $export['node|project|teaser'] = $ds_layout;

  $ds_layout = new stdClass;
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|publication|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'publication';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_2col_stacked_fluid';
  $ds_layout->settings = array(
    'hide_empty_regions' => 1,
    'hide_sidebars' => 0,
    'regions' => array(
      'left' => array(
        0 => 'body',
      ),
      'right' => array(
        0 => 'field_publication_authors',
        1 => 'field_publication_journal',
        2 => 'field_publication_date',
        3 => 'field_publication_topics',
        4 => 'field_publication_url',
        5 => 'field_publication_files',
      ),
      'footer' => array(
        0 => 'comments',
      ),
    ),
    'fields' => array(
      'body' => 'left',
      'field_publication_authors' => 'right',
      'field_publication_journal' => 'right',
      'field_publication_date' => 'right',
      'field_publication_topics' => 'right',
      'field_publication_url' => 'right',
      'field_publication_files' => 'right',
      'comments' => 'footer',
    ),
    'classes' => array(),
  );
  $export['node|publication|default'] = $ds_layout;

  $ds_layout = new stdClass;
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|publication|teaser';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'publication';
  $ds_layout->view_mode = 'teaser';
  $ds_layout->layout = 'ds_1col';
  $ds_layout->settings = array(
    'hide_empty_regions' => 1,
    'hide_sidebars' => 0,
    'regions' => array(
      'ds_content' => array(
        0 => 'title',
        1 => 'field_publication_authors',
        2 => 'field_publication_journal',
        3 => 'field_publication_date',
        4 => 'field_publication_topics',
        5 => 'links',
      ),
    ),
    'fields' => array(
      'title' => 'ds_content',
      'field_publication_authors' => 'ds_content',
      'field_publication_journal' => 'ds_content',
      'field_publication_date' => 'ds_content',
      'field_publication_topics' => 'ds_content',
      'links' => 'ds_content',
    ),
    'classes' => array(),
  );
  $export['node|publication|teaser'] = $ds_layout;

  $ds_layout = new stdClass;
  $ds_layout->api_version = 1;
  $ds_layout->id = 'node|resource|default';
  $ds_layout->entity_type = 'node';
  $ds_layout->bundle = 'resource';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_2col_stacked_fluid';
  $ds_layout->settings = array(
    'hide_empty_regions' => 1,
    'hide_sidebars' => 0,
    'regions' => array(
      'left' => array(
        0 => 'body',
      ),
      'right' => array(
        0 => 'field_resource_category',
        1 => 'field_resource_file',
      ),
      'footer' => array(
        0 => 'comments',
      ),
    ),
    'fields' => array(
      'body' => 'left',
      'field_resource_category' => 'right',
      'field_resource_file' => 'right',
      'comments' => 'footer',
    ),
    'classes' => array(),
  );
  $export['node|resource|default'] = $ds_layout;

  $ds_layout = new stdClass;
  $ds_layout->api_version = 1;
  $ds_layout->id = 'user|user|default';
  $ds_layout->entity_type = 'user';
  $ds_layout->bundle = 'user';
  $ds_layout->view_mode = 'default';
  $ds_layout->layout = 'ds_2col_stacked_fluid';
  $ds_layout->settings = array(
    'hide_empty_regions' => 1,
    'hide_sidebars' => 0,
    'regions' => array(
      'left' => array(
        0 => 'field_profile_academic_degree',
        1 => 'field_staff_position',
        2 => 'field_admission_year',
        3 => 'field_student_group',
        4 => 'field_staff_research_topics',
        5 => 'field_staff_courses',
        6 => 'contact_information_header',
        7 => 'field_staff_building',
        8 => 'field_staff_room',
        9 => 'field_profile_phone',
        10 => 'field_profile_public_email',
        11 => 'field_profile_website',
      ),
      'right' => array(
        0 => 'ds_user_picture',
      ),
      'footer' => array(
        0 => 'biography_header',
        1 => 'field_profile_biography',
      ),
    ),
    'fields' => array(
      'field_profile_academic_degree' => 'left',
      'field_staff_position' => 'left',
      'field_admission_year' => 'left',
      'field_student_group' => 'left',
      'field_staff_research_topics' => 'left',
      'field_staff_courses' => 'left',
      'contact_information_header' => 'left',
      'field_staff_building' => 'left',
      'field_staff_room' => 'left',
      'field_profile_phone' => 'left',
      'field_profile_public_email' => 'left',
      'field_profile_website' => 'left',
      'ds_user_picture' => 'right',
      'biography_header' => 'footer',
      'field_profile_biography' => 'footer',
    ),
    'classes' => array(),
  );
  $export['user|user|default'] = $ds_layout;

  return $export;
}

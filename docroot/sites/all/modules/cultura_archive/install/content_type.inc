<?php

/**
 * @file
 * Provides content type related helper functions for cultura_archive_install().
 */

/**
 * Add the host school to discussions.
 */
function cultura_archive_add_host_school_to_discussion_content_type() {
  $field = array(
    'field_name' => CULTURA_HOST_SCHOOL,
    'type' => 'taxonomy_term_reference',
    'cardinality' => 1,
    'settings' => array(
      'allowed_values' => array(
        array(
          'vocabulary' => CULTURA_SCHOOLS,
          'parent' => 0,
        ),
      ),
    ),
  );
  field_create_field($field);

  $help = st('Provide the host school or university for this exchange.');
  $instance = array(
    'field_name' => CULTURA_HOST_SCHOOL,
    'entity_type' => 'node',
    'label' => 'Host school',
    'bundle' => CULTURA_DISCUSSION_NODE_TYPE,
    'description' => $help,
    'required' => TRUE,
    'widget' => array(
      'type' => 'taxonomy_autocomplete',
      'module' => 'taxonomy',
      'active' => 0,
      'settings' => array(
        'size' => 60,
        'autocomplete_path' => 'taxonomy/autocomplete',
      ),
    ),
    'display' => array(
      'default' => array(
        'type' => 'hidden',
      ),
      'teaser' => array(
        'type' => 'hidden',
      ),
    ),
  );
  field_create_instance($instance);
}

/**
 * Add the guest school to discussions.
 */
function cultura_archive_add_guest_school_to_discussion_content_type() {
  $field = array(
    'field_name' => CULTURA_GUEST_SCHOOL,
    'type' => 'taxonomy_term_reference',
    'cardinality' => 1,
    'settings' => array(
      'allowed_values' => array(
        array(
          'vocabulary' => CULTURA_SCHOOLS,
          'parent' => 0,
        ),
      ),
    ),
  );
  field_create_field($field);

  $help = st('Provide the guest school or university for this exchange.');
  $instance = array(
    'field_name' => CULTURA_GUEST_SCHOOL,
    'entity_type' => 'node',
    'label' => 'Guest school',
    'bundle' => CULTURA_DISCUSSION_NODE_TYPE,
    'description' => $help,
    'required' => TRUE,
    'widget' => array(
      'type' => 'taxonomy_autocomplete',
      'module' => 'taxonomy',
      'active' => 0,
      'settings' => array(
        'size' => 60,
        'autocomplete_path' => 'taxonomy/autocomplete',
      ),
    ),
    'display' => array(
      'default' => array(
        'type' => 'hidden',
      ),
      'teaser' => array(
        'type' => 'hidden',
      ),
    ),
  );
  field_create_instance($instance);
}

/**
 * Add the host language to discussions.
 */
function cultura_archive_add_host_language_to_discussion_content_type() {
  $field = array(
    'field_name' => CULTURA_HOST_LANGUAGE,
    'type' => 'taxonomy_term_reference',
    'cardinality' => 1,
    'settings' => array(
      'allowed_values' => array(
        array(
          'vocabulary' => CULTURA_LANGUAGES,
          'parent' => 0,
        ),
      ),
    ),
  );
  field_create_field($field);

  $help = st('Provide the host school or university for this exchange.');
  $instance = array(
    'field_name' => CULTURA_HOST_LANGUAGE,
    'entity_type' => 'node',
    'label' => 'Host language',
    'bundle' => CULTURA_DISCUSSION_NODE_TYPE,
    'description' => $help,
    'required' => TRUE,
    'widget' => array(
      'type' => 'taxonomy_autocomplete',
      'module' => 'taxonomy',
      'active' => 0,
      'settings' => array(
        'size' => 60,
        'autocomplete_path' => 'taxonomy/autocomplete',
      ),
    ),
    'display' => array(
      'default' => array(
        'type' => 'hidden',
      ),
      'teaser' => array(
        'type' => 'hidden',
      ),
    ),
  );
  field_create_instance($instance);
}

/**
 * Add the host language to discussions.
 */
function cultura_archive_add_guest_language_to_discussion_content_type() {
  $field = array(
    'field_name' => CULTURA_GUEST_LANGUAGE,
    'type' => 'taxonomy_term_reference',
    'cardinality' => 1,
    'settings' => array(
      'allowed_values' => array(
        array(
          'vocabulary' => CULTURA_LANGUAGES,
          'parent' => 0,
        ),
      ),
    ),
  );
  field_create_field($field);

  $help = st('Provide the guest language or university for this exchange.');
  $instance = array(
    'field_name' => CULTURA_GUEST_LANGUAGE,
    'entity_type' => 'node',
    'label' => 'Guest language',
    'bundle' => CULTURA_DISCUSSION_NODE_TYPE,
    'description' => $help,
    'required' => TRUE,
    'widget' => array(
      'type' => 'taxonomy_autocomplete',
      'module' => 'taxonomy',
      'active' => 0,
      'settings' => array(
        'size' => 60,
        'autocomplete_path' => 'taxonomy/autocomplete',
      ),
    ),
    'display' => array(
      'default' => array(
        'type' => 'hidden',
      ),
      'teaser' => array(
        'type' => 'hidden',
      ),
    ),
  );
  field_create_instance($instance);
}

/**
 * Add the year to discussions.
 */
function cultura_archive_add_year_to_discussion_content_type() {
  $field = array(
    'field_name' => CULTURA_YEAR,
    'type' => 'taxonomy_term_reference',
    'cardinality' => 1,
    'settings' => array(
      'allowed_values' => array(
        array(
          'vocabulary' => CULTURA_YEAR,
          'parent' => 0,
        ),
      ),
    ),
  );
  field_create_field($field);

  $help = st('Provide the academic year of this exchange.');
  $instance = array(
    'field_name' => CULTURA_YEAR,
    'entity_type' => 'node',
    'label' => 'Academic year',
    'bundle' => CULTURA_DISCUSSION_NODE_TYPE,
    'description' => $help,
    'required' => TRUE,
    'widget' => array(
      'type' => 'taxonomy_autocomplete',
      'module' => 'taxonomy',
      'active' => 0,
      'settings' => array(
        'size' => 60,
        'autocomplete_path' => 'taxonomy/autocomplete',
      ),
    ),
    'display' => array(
      'default' => array(
        'type' => 'hidden',
      ),
      'teaser' => array(
        'type' => 'hidden',
      ),
    ),
  );
  field_create_instance($instance);
}

/**
 * Add the semester to discussions.
 */
function cultura_archive_add_semester_to_discussion_content_type() {
  $field = array(
    'field_name' => CULTURA_SEMESTER,
    'type' => 'taxonomy_term_reference',
    'cardinality' => 1,
    'settings' => array(
      'allowed_values' => array(
        array(
          'vocabulary' => CULTURA_SEMESTER,
          'parent' => 0,
        ),
      ),
    ),
  );
  field_create_field($field);

  $help = st('Provide the academic year of this exchange.');
  $instance = array(
    'field_name' => CULTURA_SEMESTER,
    'entity_type' => 'node',
    'label' => 'Semester',
    'bundle' => CULTURA_DISCUSSION_NODE_TYPE,
    'description' => $help,
    'required' => TRUE,
    'widget' => array(
      'type' => 'taxonomy_autocomplete',
      'module' => 'taxonomy',
      'active' => 0,
      'settings' => array(
        'size' => 60,
        'autocomplete_path' => 'taxonomy/autocomplete',
      ),
    ),
    'display' => array(
      'default' => array(
        'type' => 'hidden',
      ),
      'teaser' => array(
        'type' => 'hidden',
      ),
    ),
  );
  field_create_instance($instance);
}

<?php

/**
 * @file
 * Provides helper functions for cultura_questionnaire_install().
 */

/**
 * Configure user entity, user bundle to have language field.
 */
function cultura_questionnaire_configure_user() {

  // Language field was created in cultura_questionnaire_configure_webform().
  // Add it to users.
  $instance = array(
    'field_name' => CULTURA_QUESTIONNAIRE_FIELD_LANGUAGE,
    'entity_type' => 'user',
    'bundle' => 'user',
    'label' => 'Language',
    'widget' => array(
      'weight' => -10,
      'active' => 1,
    ),
    'display' => array(
      'default' => array(
        'type' => 'hidden',
      ),
      'teaser' => array(
        'type' => 'hidden',
      ),
    ),
    'settings' => array(
      'user_register_form' => 1,
    ),
    'default_value' => NULL,
  );
  field_create_instance($instance);
}

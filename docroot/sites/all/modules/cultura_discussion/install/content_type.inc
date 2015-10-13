<?php

/**
 * @file
 * Provides helper functions for cultura_discussion_install().
 */

/**
 * Create the Questionnaire configuration content type.
 */
function cultura_discussion_create_discussion_content_type($vocabulary) {
  $type = array(
    'type' => CULTURA_DISCUSSION_NODE_TYPE,
    'name' => st('Discussion'),
    'base' => 'node_content',
    'description' => st("Use <em>discussions</em> for introducing topics which students may discuss."),
    'custom' => 1,
    'modified' => 1,
    'locked' => 0,
  );
  $type = node_type_set_defaults($type);
  node_type_save($type);
  node_add_body_field($type);

  $rdf_mapping = array(
    'type' => 'node',
    'bundle' => CULTURA_DISCUSSION_NODE_TYPE,
    'mapping' => array(
      'rdftype' => array('foaf:Document'),
    ),
  );
  rdf_mapping_save($rdf_mapping);
  // Don't display date and author information for "Exchange" nodes by default.
  variable_set('node_submitted_' . CULTURA_DISCUSSION_NODE_TYPE, FALSE);

  // Comment settings.
  variable_set('comment_default_mode_' . CULTURA_DISCUSSION_NODE_TYPE, COMMENT_MODE_FLAT);
  variable_set('comment_default_per_page_' . CULTURA_DISCUSSION_NODE_TYPE, 50);
  variable_set('comment_' . CULTURA_DISCUSSION_NODE_TYPE, COMMENT_NODE_OPEN);
  variable_set('comment_form_location_' . CULTURA_DISCUSSION_NODE_TYPE, COMMENT_FORM_BELOW);
  variable_set('comment_preview_' . CULTURA_DISCUSSION_NODE_TYPE, FALSE);
  variable_set('comment_subject_field_' . CULTURA_DISCUSSION_NODE_TYPE, FALSE);

  $field = array(
    'field_name' => CULTURA_QUESTIONNAIRE_FIELD_QUESTION_TYPE,
    'type' => 'taxonomy_term_reference',
    'cardinality' => 1,
    'settings' => array(
      'allowed_values' => array(
        array(
          'vocabulary' => $vocabulary->machine_name,
          'parent' => 0,
        ),
      ),
    ),
  );
  field_create_field($field);

  $help = st('Select the category for this exchange.');
  $instance = array(
    'field_name' => CULTURA_QUESTIONNAIRE_FIELD_QUESTION_TYPE,
    'entity_type' => 'node',
    'label' => 'Question type',
    'bundle' => CULTURA_DISCUSSION_NODE_TYPE,
    'description' => $help,
    'required' => TRUE,
    'widget' => array(
      'type' => 'options_select',
      'weight' => -10,
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

  // Create a Double Textarea field for prompts.
  $field = array(
    'field_name' => CULTURA_QUESTIONNAIRE_FIELD_PROMPTS,
    'type' => 'double_field',
    'cardinality' => FIELD_CARDINALITY_UNLIMITED,
  );
  field_create_field($field);

  $instance = array(
    'field_name' => CULTURA_QUESTIONNAIRE_FIELD_PROMPTS,
    'entity_type' => 'node',
    'bundle' => CULTURA_DISCUSSION_NODE_TYPE,
    'label' => 'Prompts',
    'widget' => array(
      'weight' => '3',
      'type' => 'textarea_&_textarea',
      'module' => 'double_field',
      'active' => 1,
      'settings' => array(
        'inline' => 0,
        'first' => array(
          'textarea' => array(
            'cols' => '72',
            'rows' => '2',
            'resizable' => 1,
            'placeholder' => '',
          ),
          'general' => array(
            'required' => 1,
            'prefix' => '',
            'suffix' => '',
          ),
        ),
        'second' => array(
          'textarea' => array(
            'cols' => '72',
            'rows' => '2',
            'resizable' => 1,
            'placeholder' => '',
          ),
          'general' => array(
            'required' => 1,
            'prefix' => '',
            'suffix' => '',
          ),
        ),
      ),
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'weight' => '2',
        'settings' => array(
          'style' => 'block',
          'first' => array(
            'format' => 'filtered_html',
            'prefix' => '<h3>',
            'suffix' => '</h3>',
            'hidden' => FALSE,
          ),
          'second' => array(
            'format' => 'filtered_html',
            'prefix' => '<h3>',
            'suffix' => '</h3>',
            'hidden' => FALSE,
          ),
        ),
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
  );
  field_create_instance($instance);

  // Create Double Textarea field to hold answer sets (one for each language).
  $field = array(
    'field_name' => CULTURA_DISCUSSION_FIELD_ANSWERS,
    'type' => 'double_field',
    'settings' => array(
      'first' => array(
        'type' => 'text',
      ),
      'second' => array(
        'type' => 'text',
      ),
    ),
    'columns' => array(
      'first' => array(
        'type' => 'text',
        'size' => 'big',
      ),
      'second' => array(
        'type' => 'text',
        'size' => 'big',
      ),
    ),
  );
  field_create_field($field);

  $instance = array(
    'field_name' => CULTURA_DISCUSSION_FIELD_ANSWERS,
    'entity_type' => 'node',
    'bundle' => CULTURA_DISCUSSION_NODE_TYPE,
    'label' => 'Answers',
    'widget' => array(
      'weight' => '4',
      'type' => 'textarea_&_textarea',
      'module' => 'double_field',
      'settings' => array(
        'inline' => FALSE,
        'first' => array(
          'general' => array(
            'required' => TRUE,
            'prefix' => '',
            'suffix' => '',
          ),
          'textarea' => array(
            'cols' => 10,
            'rows' => 5,
            'resizable' => TRUE,
            'placeholder' => '',
          ),
        ),
        'second' => array(
          'general' => array(
            'required' => TRUE,
            'prefix' => '',
            'suffix' => '',
          ),
          'textarea' => array(
            'cols' => 10,
            'rows' => 5,
            'resizable' => TRUE,
            'placeholder' => '',
          ),
        ),
      ),
    ),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'weight' => '3',
        'settings' => array(
          'style' => 'block',
          'first' => array(
            'format' => 'full_html',
            'prefix' => '',
            'suffix' => '',
            'hidden' => FALSE,
          ),
          'second' => array(
            'format' => 'full_html',
            'prefix' => '',
            'suffix' => '',
            'hidden' => FALSE,
          ),
        ),
      ),
      'teaser' => array(
        'type' => 'hidden',
        'settings' => array(),
        'weight' => 0,
        'label' => 'above',
      ),
    ),
    'description' => 'Introduce the questionnaire (in each language).',
    'default_value' => NULL,
  );
  field_create_instance($instance);

  // Create and attach Tags field for free-tagging taxonomy.
  $field = array(
    'field_name' => CULTURA_TAGS,
    'type' => 'taxonomy_term_reference',
    'settings' => array(
      'allowed_values' => array(
        array(
          'vocabulary' => CULTURA_TAGS,
        ),
      ),
    ),
  );
  field_create_field($field);

  $help = st('List words or phrases you associate with this content, separated by commas.');
  $instance = array(
    'field_name' => CULTURA_TAGS,
    'entity_type' => 'node',
    'bundle' => CULTURA_DISCUSSION_NODE_TYPE,
    'label' => 'Tags',
    'description' => $help,
    'widget' => array(
      'weight' => '2',
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
        'label' => 'inline',
        'weight' => '10',
      ),
      'teaser' => array(
        'label' => 'inline',
        'type' => 'hidden',
        'weight' => '1',
        'settings' => array(),
      ),
    ),
    'default_value' => NULL,
  );
  field_create_instance($instance);

}

/**
 * Update shortcut sets.
 */
function cultura_discussion_configure_shortcuts() {
  $set_1 = shortcut_set_load('shortcut-set-1');
  $link = array();
  $link['link_title'] = t('Create discussion topic');
  $link['link_path'] = $link['router_path'] = 'node/add/' . drupal_html_class(CULTURA_DISCUSSION_NODE_TYPE);
  $link['weight'] = -20;
  $set_1->links[] = $link;
  shortcut_set_save($set_1);
}

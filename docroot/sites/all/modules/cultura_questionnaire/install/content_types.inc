<?php

/**
 * @file
 * Provides helper functions for cultura_questionnaire_install().
 */

/**
 * Create the Questionnaire builder content type.
 */
function cultura_questionnaire_create_questionnaire_builder_content_type() {
  $type = array(
    'type' => CULTURA_QUESTIONNAIRE_BUILDER_NODE_TYPE,
    'name' => st('Questionnaire builder'),
    'base' => 'node_content',
    'description' => st("Use <em>questionnaires</em> to define the prompts (in both languages) to which students should give short responses."),
    'custom' => 1,
    'modified' => 1,
    'locked' => 0,
  );
  $type = node_type_set_defaults($type);
  node_type_save($type);

  $rdf_mapping = array(
    'type' => 'node',
    'bundle' => CULTURA_QUESTIONNAIRE_BUILDER_NODE_TYPE,
    'mapping' => array(
      'rdftype' => array('foaf:Document'),
    ),
  );
  rdf_mapping_save($rdf_mapping);

  // Node options are 'status' (published), 'promote', 'sticky', and 'revision'.
  variable_set('node_options_' . CULTURA_QUESTIONNAIRE_BUILDER_NODE_TYPE, array('revision'));

  // Do not offer a preview for the Questionnaire builder type.
  variable_set('node_preview_' . CULTURA_QUESTIONNAIRE_BUILDER_NODE_TYPE, FALSE);

  // Don't display date and author information for Questionnaire nodes.
  variable_set('node_submitted_' . CULTURA_QUESTIONNAIRE_BUILDER_NODE_TYPE, FALSE);

  // Comment settings.
  variable_set('comment_' . CULTURA_QUESTIONNAIRE_BUILDER_NODE_TYPE, COMMENT_NODE_HIDDEN);

  // Publish button setting (for publish_button module).
  variable_set('publish_button_content_type_' . CULTURA_QUESTIONNAIRE_BUILDER_NODE_TYPE, TRUE);

  // Field already created in cultura_discussion.
  $help = st('Select the category for this exchange.');
  $instance = array(
    'field_name' => CULTURA_QUESTIONNAIRE_FIELD_QUESTION_TYPE,
    'entity_type' => 'node',
    'label' => 'Question type',
    'bundle' => CULTURA_QUESTIONNAIRE_BUILDER_NODE_TYPE,
    'description' => $help,
    'required' => TRUE,
    'widget' => array(
      'type' => 'options_select',
      'weight' => -10,
    ),
    'display' => array(
      'default' => array(
        'type' => 'taxonomy_term_reference_link',
        'weight' => 10,
      ),
      'teaser' => array(
        'type' => 'taxonomy_term_reference_link',
        'weight' => 10,
      ),
    ),
  );
  field_create_instance($instance);

  // Create a Double Textarea field for webform intros.
  $field = array(
    'field_name' => CULTURA_QUESTIONNAIRE_FIELD_INTROS,
    'type' => 'double_field',
  );
  field_create_field($field);

  $instance = array(
    'label' => 'Introductions',
    'widget' => array(
      'weight' => '8',
      'type' => 'textarea_&_textarea',
      'module' => 'double_field',
    ),
    'display' => array(
      'default' => array(
        'settings' => array(
          'first' => array(
            'hidden' => 0,
            'format' => '_none',
            'prefix' => '',
            'suffix' => ': ',
          ),
          'second' => array(
            'hidden' => 0,
            'format' => '_none',
            'prefix' => '',
            'suffix' => '',
          ),
        ),
        'weight' => 12,
      ),
      'teaser' => array(
        'type' => 'hidden',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => 'Introduce the questionnaire (in each language).',
    'field_name' => CULTURA_QUESTIONNAIRE_FIELD_INTROS,
    'entity_type' => 'node',
    'bundle' => CULTURA_QUESTIONNAIRE_BUILDER_NODE_TYPE,
  );
  field_create_instance($instance);

  // Prompts already created in cultura_discussion.
  $instance = array(
    'label' => 'Prompts',
    'widget' => array(
      'weight' => '32',
      'type' => 'textarea_&_textarea',
      'module' => 'double_field',
    ),
    'display' => array(
      'default' => array(
        'settings' => array(
          'first' => array(
            'hidden' => 0,
            'format' => '_none',
            'prefix' => '',
            'suffix' => ': ',
          ),
          'second' => array(
            'hidden' => 0,
            'format' => '_none',
            'prefix' => '',
            'suffix' => '',
          ),
        ),
        'weight' => 12,
      ),
      'teaser' => array(
        'type' => 'hidden',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 1,
    'description' => 'Give us some prompts.',
    'field_name' => CULTURA_QUESTIONNAIRE_FIELD_PROMPTS,
    'entity_type' => 'node',
    'bundle' => CULTURA_QUESTIONNAIRE_BUILDER_NODE_TYPE,
  );
  field_create_instance($instance);
}

/**
 * Add a reference to the Questionnaire Builder on Discussion content types.
 */
function cultura_questionnaire_add_builder_reference_to_discussion() {
  // Add nodereference to Questionnaire Builder.
  // The field was already created in Cultura Questionnaire (webform).
  $instance = array(
    'field_name' => CULTURA_QUESTIONNAIRE_FIELD_QUESTIONNAIRE,
    'entity_type' => 'node',
    'bundle' => CULTURA_DISCUSSION_NODE_TYPE,
    'label' => 'Questionnaire builder',
    'widget' => array(
      'weight' => '31',
      'type' => 'node_reference_autocomplete',
      'module' => 'node_reference',
      'active' => 1,
      'settings' => array(
        'autocomplete_match' => 'starts_with',
        'size' => '60',
        'autocomplete_path' => 'node_reference/autocomplete',
      ),
    ),
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'type' => 'hidden',
        'weight' => '1',
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 1,
    'default_value' => NULL,
  );
  field_create_instance($instance);
}

/**
 * Configure Auto Node Title (auto_nodetitle) for Questionnaire nodes.
 */
function cultura_questionnaire_configure_auto_nodetitle() {
  variable_set('ant_' . CULTURA_QUESTIONNAIRE_BUILDER_NODE_TYPE, 1);
  variable_set('ant_pattern_' . CULTURA_QUESTIONNAIRE_BUILDER_NODE_TYPE,
    '[node:cultura_question_type]-[node:created:short]');
}

/**
 * Configure webform content type (used for questionnaires).
 */
function cultura_questionnaire_configure_webform() {

  $type = node_type_load('webform');
  // Rename needs old_type undocumented & inexplicably different from orig_type.
  $type->old_type = 'webform';
  // Rename to Questionnaire.
  $type->name = t("Questionnaire");
  $type->type = CULTURA_QUESTIONNAIRE_NODE_TYPE;
  // Add body/description field.
  node_add_body_field($type);
  node_type_save($type);

  // Change the variable enabling webform capabilities to the new node type.
  variable_del('webform_node_webform');
  variable_set('webform_node_' . CULTURA_QUESTIONNAIRE_NODE_TYPE, TRUE);

  // Don't display date and author information for Webform nodes.
  variable_set('node_submitted_' . CULTURA_QUESTIONNAIRE_NODE_TYPE, FALSE);
  // Configure default Webform content type: Not promoted, no author info.
  variable_set('node_options_' . CULTURA_QUESTIONNAIRE_NODE_TYPE, array('status'));

  // Hide comments on webforms.
  variable_set('comment_' . CULTURA_QUESTIONNAIRE_NODE_TYPE, COMMENT_NODE_HIDDEN);

  // Add nodereference field to source Questionnaire builder.
  $field = array(
    'field_name' => CULTURA_QUESTIONNAIRE_FIELD_QUESTIONNAIRE,
    'type' => 'node_reference',
    'settings' => array(
      'referenceable_types' => array(
        'cultura_questionnaire' => CULTURA_QUESTIONNAIRE_BUILDER_NODE_TYPE,
      ),
    ),
  );
  field_create_field($field);

  $instance = array(
    'field_name' => CULTURA_QUESTIONNAIRE_FIELD_QUESTIONNAIRE,
    'entity_type' => 'node',
    'bundle' => CULTURA_QUESTIONNAIRE_NODE_TYPE,
    'label' => 'Questionnaire builder',
    'widget' => array(
      'weight' => '31',
      'type' => 'node_reference_autocomplete',
      'module' => 'node_reference',
      'active' => 1,
      'settings' => array(
        'autocomplete_match' => 'starts_with',
        'size' => '60',
        'autocomplete_path' => 'node_reference/autocomplete',
      ),
    ),
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'type' => 'hidden',
        'weight' => '1',
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'required' => 1,
    'default_value' => NULL,
  );
  field_create_instance($instance);

  // Create a field to hold the language of one half of a Questionnaire pair.
  $field = array(
    'field_name' => CULTURA_QUESTIONNAIRE_FIELD_LANGUAGE,
    'type' => 'taxonomy_term_reference',
    'cardinality' => 1,
    'settings' => array(
      'allowed_values' => array(
        array(
          'vocabulary' => CULTURA_QUESTIONNAIRE_LANGUAGE_VOCABULARY,
          'parent' => 0,
        ),
      ),
    ),
  );
  field_create_field($field);
  $instance = array(
    'field_name' => CULTURA_QUESTIONNAIRE_FIELD_LANGUAGE,
    'entity_type' => 'node',
    'label' => 'Language',
    'bundle' => CULTURA_QUESTIONNAIRE_NODE_TYPE,
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

}

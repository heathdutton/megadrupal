<?php
/**
 * @file
 * minds_eye_vampire.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function minds_eye_vampire_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_abilities|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_abilities';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Abilities',
    'weight' => '3',
    'children' => array(
      0 => 'group_knowledges',
      1 => 'group_skills',
      2 => 'group_talents',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_abilities|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_abilities|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_abilities';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Abilities',
    'weight' => '3',
    'children' => array(
      0 => 'group_knowledges',
      1 => 'group_skills',
      2 => 'group_talents',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_abilities|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_advantages|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_advantages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Advantages',
    'weight' => '4',
    'children' => array(
      0 => 'group_column_1',
      1 => 'group_column_2',
      2 => 'group_column_3',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_advantages|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_advantages|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_advantages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Advantages',
    'weight' => '4',
    'children' => array(
      0 => 'group_column_1',
      1 => 'group_column_2',
      2 => 'group_column_3',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_advantages|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_attributes|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_attributes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Attributes',
    'weight' => '2',
    'children' => array(
      0 => 'group_mental',
      1 => 'group_physical',
      2 => 'group_social',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_attributes|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_attributes|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_attributes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Attributes',
    'weight' => '2',
    'children' => array(
      0 => 'group_mental',
      1 => 'group_physical',
      2 => 'group_social',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_attributes|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_backgrounds|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_backgrounds';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Backgrounds',
    'weight' => '9',
    'children' => array(
      0 => 'field_me_v_background_set',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Backgrounds',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_backgrounds|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_backgrounds|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_backgrounds';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Backgrounds',
    'weight' => '9',
    'children' => array(
      0 => 'field_me_v_background_set',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Backgrounds',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_backgrounds|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character_details|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_character_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Character Details',
    'weight' => '1',
    'children' => array(
      0 => 'field_char_image',
      1 => 'field_desc',
      2 => 'field_history',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_character_details|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character_details|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_character_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Character Details',
    'weight' => '1',
    'children' => array(
      0 => 'field_char_image',
      1 => 'field_desc',
      2 => 'field_history',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_character_details|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_character';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Character',
    'weight' => '0',
    'children' => array(
      0 => 'group_column_a',
      1 => 'group_column_b',
      2 => 'group_column_c',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_character|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_character';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Character',
    'weight' => '0',
    'children' => array(
      0 => 'group_column_a',
      1 => 'group_column_b',
      2 => 'group_column_c',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_character|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_1|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_column_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '12',
    'children' => array(
      0 => 'group_backgrounds',
      1 => 'group_merits_flaws',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Column',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_column_1|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_1|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_column_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '12',
    'children' => array(
      0 => 'group_backgrounds',
      1 => 'group_merits_flaws',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Column',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_column_1|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_2|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_column_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '13',
    'children' => array(
      0 => 'field_hum_path',
      1 => 'field_willpower',
      2 => 'group_disciplines',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Column',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_column_2|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_2|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_column_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '13',
    'children' => array(
      0 => 'field_hum_path',
      1 => 'field_willpower',
      2 => 'group_disciplines',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Column',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_column_2|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_3|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_column_3';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '14',
    'children' => array(
      0 => 'group_virtues',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Column',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_column_3|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_3|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_column_3';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '14',
    'children' => array(
      0 => 'group_virtues',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Column',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_column_3|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_a|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_column_a';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '13',
    'children' => array(
      0 => 'field_chronicle',
      1 => 'field_player',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Column',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_column_a|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_a|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_column_a';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '13',
    'children' => array(
      0 => 'field_chronicle',
      1 => 'field_player',
      2 => 'title',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Column',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_column_a|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_b|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_column_b';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '14',
    'children' => array(
      0 => 'field_clan',
      1 => 'field_demeanor',
      2 => 'field_nature',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Column',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_column_b|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_b|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_column_b';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '14',
    'children' => array(
      0 => 'field_clan',
      1 => 'field_demeanor',
      2 => 'field_nature',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Column',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_column_b|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_c|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_column_c';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '15',
    'children' => array(
      0 => 'field_concept',
      1 => 'field_generation',
      2 => 'field_haven',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Column',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_column_c|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_c|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_column_c';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '15',
    'children' => array(
      0 => 'field_concept',
      1 => 'field_generation',
      2 => 'field_haven',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Column',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_column_c|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_disciplines|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_disciplines';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_2';
  $field_group->data = array(
    'label' => 'Disciplines',
    'weight' => '24',
    'children' => array(
      0 => 'field_me_v_discipline_set',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_disciplines|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_disciplines|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_disciplines';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_2';
  $field_group->data = array(
    'label' => 'Disciplines',
    'weight' => '24',
    'children' => array(
      0 => 'field_me_v_discipline_set',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_disciplines|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_knowledges|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_knowledges';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Knowledges',
    'weight' => '16',
    'children' => array(
      0 => 'field_me_v_know_set',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_knowledges|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_knowledges|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_knowledges';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Knowledges',
    'weight' => '16',
    'children' => array(
      0 => 'field_me_v_know_set',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_knowledges|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_mental|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_mental';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Mental',
    'weight' => '5',
    'children' => array(
      0 => 'field_intel',
      1 => 'field_percep',
      2 => 'field_wits',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_mental|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_mental|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_mental';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Mental',
    'weight' => '5',
    'children' => array(
      0 => 'field_intel',
      1 => 'field_percep',
      2 => 'field_wits',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_mental|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_merits_flaws|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_merits_flaws';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Merits / Flaws',
    'weight' => '10',
    'children' => array(
      0 => 'field_merit_flaw',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_merits_flaws|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_merits_flaws|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_merits_flaws';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Merits / Flaws',
    'weight' => '10',
    'children' => array(
      0 => 'field_merit_flaw',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_merits_flaws|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notes',
    'weight' => '52',
    'children' => array(
      0 => 'field_player_notes',
      1 => 'field_st_notes',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_notes|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notes',
    'weight' => '52',
    'children' => array(
      0 => 'field_player_notes',
      1 => 'field_st_notes',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_notes|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_physical|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_physical';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Physical',
    'weight' => '3',
    'children' => array(
      0 => 'field_dexterity',
      1 => 'field_stamina',
      2 => 'field_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_physical|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_physical|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_physical';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Physical',
    'weight' => '3',
    'children' => array(
      0 => 'field_dexterity',
      1 => 'field_stamina',
      2 => 'field_strength',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_physical|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_property|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_property';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Property',
    'weight' => '49',
    'children' => array(
      0 => 'field_gear',
      1 => 'field_additional',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_property|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_property|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_property';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Property',
    'weight' => '49',
    'children' => array(
      0 => 'field_gear',
      1 => 'field_additional',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_property|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '15',
    'children' => array(
      0 => 'field_me_v_skill_set',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_skills|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '15',
    'children' => array(
      0 => 'field_me_v_skill_set',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_skills|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_social|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_social';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Social',
    'weight' => '4',
    'children' => array(
      0 => 'field_appear',
      1 => 'field_charisma',
      2 => 'field_manip',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_social|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_social|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_social';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Social',
    'weight' => '4',
    'children' => array(
      0 => 'field_appear',
      1 => 'field_charisma',
      2 => 'field_manip',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_social|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_talents|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_talents';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Talents',
    'weight' => '14',
    'children' => array(
      0 => 'field_me_v_talent_set',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_talents|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_talents|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_talents';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Talents',
    'weight' => '14',
    'children' => array(
      0 => 'field_me_v_talent_set',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_talents|node|me_v_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_virtues|node|me_v_character_sheet|default';
  $field_group->group_name = 'group_virtues';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_3';
  $field_group->data = array(
    'label' => 'Virtues',
    'weight' => '8',
    'children' => array(
      0 => 'field_con_con',
      1 => 'field_courage',
      2 => 'field_self_instinct',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_virtues|node|me_v_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_virtues|node|me_v_character_sheet|form';
  $field_group->group_name = 'group_virtues';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'me_v_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_3';
  $field_group->data = array(
    'label' => 'Virtues',
    'weight' => '8',
    'children' => array(
      0 => 'field_con_con',
      1 => 'field_courage',
      2 => 'field_self_instinct',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_virtues|node|me_v_character_sheet|form'] = $field_group;

  return $export;
}

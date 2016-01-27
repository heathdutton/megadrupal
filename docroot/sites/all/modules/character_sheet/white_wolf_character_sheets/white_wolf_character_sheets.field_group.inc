<?php
/**
 * @file
 * white_wolf_character_sheets.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function white_wolf_character_sheets_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_abilities|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_abilities';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
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
  $export['group_abilities|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_abilities|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_abilities';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
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
  $export['group_abilities|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_abilities|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_abilities';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
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
  $export['group_abilities|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_abilities|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_abilities';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
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
  $export['group_abilities|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_abilities|node|mage_character_sheet|default';
  $field_group->group_name = 'group_abilities';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
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
  $export['group_abilities|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_abilities|node|mage_character_sheet|form';
  $field_group->group_name = 'group_abilities';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
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
  $export['group_abilities|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_abilities|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_abilities';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_abilities|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_abilities|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_abilities';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_abilities|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_abilities|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_abilities';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
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
  $export['group_abilities|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_abilities|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_abilities';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
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
  $export['group_abilities|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_advantages|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_advantages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
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
  $export['group_advantages|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_advantages|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_advantages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
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
  $export['group_advantages|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_advantages|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_advantages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Advantages',
    'weight' => '4',
    'children' => array(
      0 => 'group_column_1',
      1 => 'group_column_2',
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
  $export['group_advantages|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_advantages|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_advantages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Advantages',
    'weight' => '4',
    'children' => array(
      0 => 'group_column_1',
      1 => 'group_column_2',
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
  $export['group_advantages|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_advantages|node|mage_character_sheet|default';
  $field_group->group_name = 'group_advantages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Advantages',
    'weight' => '5',
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
  $export['group_advantages|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_advantages|node|mage_character_sheet|form';
  $field_group->group_name = 'group_advantages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Advantages',
    'weight' => '5',
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
  $export['group_advantages|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_advantages|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_advantages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_advantages|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_advantages|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_advantages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_advantages|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_advantages|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_advantages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
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
  $export['group_advantages|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_advantages|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_advantages';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
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
  $export['group_advantages|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_arts|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_arts';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_2';
  $field_group->data = array(
    'label' => 'Arts',
    'weight' => '30',
    'children' => array(
      0 => 'field_art',
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
  $export['group_arts|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_arts|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_arts';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_2';
  $field_group->data = array(
    'label' => 'Arts',
    'weight' => '30',
    'children' => array(
      0 => 'field_art',
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
  $export['group_arts|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_attributes|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_attributes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
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
  $export['group_attributes|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_attributes|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_attributes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
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
  $export['group_attributes|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_attributes|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_attributes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
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
  $export['group_attributes|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_attributes|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_attributes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
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
  $export['group_attributes|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_attributes|node|mage_character_sheet|default';
  $field_group->group_name = 'group_attributes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
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
  $export['group_attributes|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_attributes|node|mage_character_sheet|form';
  $field_group->group_name = 'group_attributes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
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
  $export['group_attributes|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_attributes|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_attributes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_attributes|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_attributes|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_attributes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_attributes|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_attributes|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_attributes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
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
  $export['group_attributes|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_attributes|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_attributes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
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
  $export['group_attributes|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_backgrounds|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_backgrounds';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Background',
    'weight' => '27',
    'children' => array(
      0 => 'field_background_set',
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
  $export['group_backgrounds|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_backgrounds|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_backgrounds';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Background',
    'weight' => '27',
    'children' => array(
      0 => 'field_background_set',
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
  $export['group_backgrounds|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_backgrounds|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_backgrounds';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Background',
    'weight' => '4',
    'children' => array(
      0 => 'field_background_set',
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
  $export['group_backgrounds|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_backgrounds|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_backgrounds';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Background',
    'weight' => '4',
    'children' => array(
      0 => 'field_background_set',
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
  $export['group_backgrounds|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_backgrounds|node|mage_character_sheet|default';
  $field_group->group_name = 'group_backgrounds';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Background',
    'weight' => '6',
    'children' => array(
      0 => 'field_background_set',
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
  $export['group_backgrounds|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_backgrounds|node|mage_character_sheet|form';
  $field_group->group_name = 'group_backgrounds';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Background',
    'weight' => '6',
    'children' => array(
      0 => 'field_background_set',
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
  $export['group_backgrounds|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_backgrounds|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_backgrounds';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Backgrounds',
    'weight' => '9',
    'children' => array(
      0 => 'field_background_set',
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
  $export['group_backgrounds|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_backgrounds|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_backgrounds';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Backgrounds',
    'weight' => '9',
    'children' => array(
      0 => 'field_background_set',
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
  $export['group_backgrounds|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_backgrounds|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_backgrounds';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Background',
    'weight' => '22',
    'children' => array(
      0 => 'field_background_set',
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
  $export['group_backgrounds|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_backgrounds|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_backgrounds';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Background',
    'weight' => '22',
    'children' => array(
      0 => 'field_background_set',
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
  $export['group_backgrounds|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character_details|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_character_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
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
  $export['group_character_details|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character_details|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_character_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
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
  $export['group_character_details|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character_details|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_character_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
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
  $export['group_character_details|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character_details|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_character_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
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
  $export['group_character_details|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character_details|node|mage_character_sheet|default';
  $field_group->group_name = 'group_character_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
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
  $export['group_character_details|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character_details|node|mage_character_sheet|form';
  $field_group->group_name = 'group_character_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
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
  $export['group_character_details|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character_details|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_character_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_character_details|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character_details|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_character_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_character_details|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character_details|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_character_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
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
  $export['group_character_details|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character_details|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_character_details';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
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
  $export['group_character_details|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_character';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
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
  $export['group_character|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_character';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
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
  $export['group_character|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_character';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
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
  $export['group_character|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_character';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
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
  $export['group_character|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character|node|mage_character_sheet|default';
  $field_group->group_name = 'group_character';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
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
  $export['group_character|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character|node|mage_character_sheet|form';
  $field_group->group_name = 'group_character';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
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
  $export['group_character|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_character';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_character|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_character';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_character|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_character';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
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
  $export['group_character|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_character|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_character';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
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
  $export['group_character|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_1|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_column_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '15',
    'children' => array(
      0 => 'group_backgrounds',
      1 => 'group_merits_flaws',
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
  $export['group_column_1|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_1|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_column_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '15',
    'children' => array(
      0 => 'group_backgrounds',
      1 => 'group_merits_flaws',
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
  $export['group_column_1|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_1|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_column_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '14',
    'children' => array(
      0 => 'group_backgrounds',
      1 => 'group_derangements',
      2 => 'group_merits_flaws',
      3 => 'group_virtues',
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
  $export['group_column_1|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_1|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_column_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '14',
    'children' => array(
      0 => 'group_backgrounds',
      1 => 'group_derangements',
      2 => 'group_merits_flaws',
      3 => 'group_virtues',
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
  $export['group_column_1|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_1|node|mage_character_sheet|default';
  $field_group->group_name = 'group_column_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '7',
    'children' => array(
      0 => 'group_backgrounds',
      1 => 'group_merits_flaws',
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
  $export['group_column_1|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_1|node|mage_character_sheet|form';
  $field_group->group_name = 'group_column_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '7',
    'children' => array(
      0 => 'group_backgrounds',
      1 => 'group_merits_flaws',
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
  $export['group_column_1|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_1|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_column_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_column_1|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_1|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_column_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_column_1|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_1|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_column_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '47',
    'children' => array(
      0 => 'group_backgrounds',
      1 => 'group_merits_flaws',
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
  $export['group_column_1|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_1|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_column_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '47',
    'children' => array(
      0 => 'group_backgrounds',
      1 => 'group_merits_flaws',
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
  $export['group_column_1|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_2|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_column_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '16',
    'children' => array(
      0 => 'field_banality',
      1 => 'field_glamour',
      2 => 'field_willpower',
      3 => 'group_arts',
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
  $export['group_column_2|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_2|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_column_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '16',
    'children' => array(
      0 => 'field_banality',
      1 => 'field_glamour',
      2 => 'field_willpower',
      3 => 'group_arts',
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
  $export['group_column_2|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_2|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_column_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '15',
    'children' => array(
      0 => 'field_convict',
      1 => 'field_willpower',
      2 => 'group_edges',
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
  $export['group_column_2|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_2|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_column_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '15',
    'children' => array(
      0 => 'field_convict',
      1 => 'field_willpower',
      2 => 'group_edges',
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
  $export['group_column_2|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_2|node|mage_character_sheet|default';
  $field_group->group_name = 'group_column_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '8',
    'children' => array(
      0 => 'field_arete',
      1 => 'field_paradox',
      2 => 'field_quint',
      3 => 'field_willpower',
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
  $export['group_column_2|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_2|node|mage_character_sheet|form';
  $field_group->group_name = 'group_column_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '8',
    'children' => array(
      0 => 'field_arete',
      1 => 'field_paradox',
      2 => 'field_quint',
      3 => 'field_willpower',
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
  $export['group_column_2|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_2|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_column_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_column_2|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_2|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_column_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_column_2|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_2|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_column_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '48',
    'children' => array(
      0 => 'field_gnosis',
      1 => 'field_rage',
      2 => 'field_willpower',
      3 => 'group_gifts',
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
  $export['group_column_2|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_2|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_column_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '48',
    'children' => array(
      0 => 'field_gnosis',
      1 => 'field_rage',
      2 => 'field_willpower',
      3 => 'group_gifts',
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
  $export['group_column_2|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_3|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_column_3';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '17',
    'children' => array(
      0 => 'group_other_traits',
      1 => 'group_realms',
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
  $export['group_column_3|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_3|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_column_3';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '17',
    'children' => array(
      0 => 'group_other_traits',
      1 => 'group_realms',
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
  $export['group_column_3|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_3|node|mage_character_sheet|default';
  $field_group->group_name = 'group_column_3';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '9',
    'children' => array(
      0 => 'group_reson',
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
  $export['group_column_3|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_3|node|mage_character_sheet|form';
  $field_group->group_name = 'group_column_3';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '9',
    'children' => array(
      0 => 'group_reson',
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
  $export['group_column_3|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_3|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_column_3';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_column_3|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_3|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_column_3';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_column_3|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_3|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_column_3';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '49',
    'children' => array(
      0 => 'field_rank',
      1 => 'group_renown',
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
  $export['group_column_3|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_3|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_column_3';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_advantages';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '49',
    'children' => array(
      0 => 'field_rank',
      1 => 'group_renown',
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
  $export['group_column_3|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_a|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_column_a';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '17',
    'children' => array(
      0 => 'field_chronicle',
      1 => 'field_player',
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
  $export['group_column_a|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_a|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_column_a';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '17',
    'children' => array(
      0 => 'field_chronicle',
      1 => 'field_player',
      2 => 'title',
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
  $export['group_column_a|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_a|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_column_a';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '3',
    'children' => array(
      0 => 'field_chronicle',
      1 => 'field_player',
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
  $export['group_column_a|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_a|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_column_a';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '3',
    'children' => array(
      0 => 'field_chronicle',
      1 => 'field_player',
      2 => 'title',
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
  $export['group_column_a|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_a|node|mage_character_sheet|default';
  $field_group->group_name = 'group_column_a';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '17',
    'children' => array(
      0 => 'field_chronicle',
      1 => 'field_player',
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
  $export['group_column_a|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_a|node|mage_character_sheet|form';
  $field_group->group_name = 'group_column_a';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '17',
    'children' => array(
      0 => 'field_chronicle',
      1 => 'field_player',
      2 => 'title',
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
  $export['group_column_a|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_a|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_column_a';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_column_a|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_a|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_column_a';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_column_a|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_a|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_column_a';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '17',
    'children' => array(
      0 => 'field_chronicle',
      1 => 'field_player',
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
  $export['group_column_a|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_a|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_column_a';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '17',
    'children' => array(
      0 => 'field_chronicle',
      1 => 'field_player',
      2 => 'title',
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
  $export['group_column_a|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_b|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_column_b';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '18',
    'children' => array(
      0 => 'field_court',
      1 => 'field_demeanor',
      2 => 'field_house',
      3 => 'field_legacies',
      4 => 'field_nature',
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
  $export['group_column_b|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_b|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_column_b';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '18',
    'children' => array(
      0 => 'field_court',
      1 => 'field_demeanor',
      2 => 'field_house',
      3 => 'field_legacies',
      4 => 'field_nature',
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
  $export['group_column_b|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_b|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_column_b';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '4',
    'children' => array(
      0 => 'field_demeanor',
      1 => 'field_nature',
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
  $export['group_column_b|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_b|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_column_b';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '4',
    'children' => array(
      0 => 'field_demeanor',
      1 => 'field_nature',
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
  $export['group_column_b|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_b|node|mage_character_sheet|default';
  $field_group->group_name = 'group_column_b';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '18',
    'children' => array(
      0 => 'field_demeanor',
      1 => 'field_essence',
      2 => 'field_nature',
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
  $export['group_column_b|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_b|node|mage_character_sheet|form';
  $field_group->group_name = 'group_column_b';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '18',
    'children' => array(
      0 => 'field_demeanor',
      1 => 'field_essence',
      2 => 'field_nature',
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
  $export['group_column_b|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_b|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_column_b';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_column_b|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_b|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_column_b';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_column_b|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_b|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_column_b';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '18',
    'children' => array(
      0 => 'field_auspice',
      1 => 'field_breed',
      2 => 'field_demeanor',
      3 => 'field_nature',
      4 => 'field_tribe',
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
  $export['group_column_b|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_b|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_column_b';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '18',
    'children' => array(
      0 => 'field_auspice',
      1 => 'field_breed',
      2 => 'field_demeanor',
      3 => 'field_nature',
      4 => 'field_tribe',
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
  $export['group_column_b|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_c|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_column_c';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '19',
    'children' => array(
      0 => 'field_concept',
      1 => 'field_kith',
      2 => 'field_motley',
      3 => 'field_seeming',
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
  $export['group_column_c|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_c|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_column_c';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '19',
    'children' => array(
      0 => 'field_concept',
      1 => 'field_kith',
      2 => 'field_motley',
      3 => 'field_seeming',
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
  $export['group_column_c|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_c|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_column_c';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '5',
    'children' => array(
      0 => 'field_concept',
      1 => 'field_conviction',
      2 => 'field_creed',
      3 => 'field_virtue',
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
  $export['group_column_c|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_c|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_column_c';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '5',
    'children' => array(
      0 => 'field_concept',
      1 => 'field_conviction',
      2 => 'field_creed',
      3 => 'field_virtue',
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
  $export['group_column_c|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_c|node|mage_character_sheet|default';
  $field_group->group_name = 'group_column_c';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '19',
    'children' => array(
      0 => 'field_cabal',
      1 => 'field_concept',
      2 => 'field_tradition',
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
  $export['group_column_c|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_c|node|mage_character_sheet|form';
  $field_group->group_name = 'group_column_c';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '19',
    'children' => array(
      0 => 'field_cabal',
      1 => 'field_concept',
      2 => 'field_tradition',
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
  $export['group_column_c|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_c|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_column_c';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_column_c|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_c|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_column_c';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_column_c|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_c|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_column_c';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '19',
    'children' => array(
      0 => 'field_concept',
      1 => 'field_pack_name',
      2 => 'field_pack_totem',
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
  $export['group_column_c|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_column_c|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_column_c';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_character';
  $field_group->data = array(
    'label' => 'Column',
    'weight' => '19',
    'children' => array(
      0 => 'field_concept',
      1 => 'field_pack_name',
      2 => 'field_pack_totem',
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
  $export['group_column_c|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_derangements|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_derangements';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Derangements',
    'weight' => '7',
    'children' => array(
      0 => 'field_derangement',
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
  $export['group_derangements|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_derangements|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_derangements';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Derangements',
    'weight' => '7',
    'children' => array(
      0 => 'field_derangement',
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
  $export['group_derangements|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_disciplines|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_disciplines';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_2';
  $field_group->data = array(
    'label' => 'Disciplines',
    'weight' => '24',
    'children' => array(
      0 => 'field_discipline_set',
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
  $export['group_disciplines|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_disciplines|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_disciplines';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_2';
  $field_group->data = array(
    'label' => 'Disciplines',
    'weight' => '24',
    'children' => array(
      0 => 'field_discipline_set',
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
  $export['group_disciplines|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_edges|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_edges';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_2';
  $field_group->data = array(
    'label' => 'Edges',
    'weight' => '10',
    'children' => array(
      0 => 'field_edge',
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
  $export['group_edges|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_edges|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_edges';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_2';
  $field_group->data = array(
    'label' => 'Edges',
    'weight' => '9',
    'children' => array(
      0 => 'field_edge',
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
  $export['group_edges|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_gifts|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_gifts';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_2';
  $field_group->data = array(
    'label' => 'Gifts',
    'weight' => '24',
    'children' => array(
      0 => 'field_gift',
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
  $export['group_gifts|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_gifts|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_gifts';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_2';
  $field_group->data = array(
    'label' => 'Gifts',
    'weight' => '26',
    'children' => array(
      0 => 'field_gift',
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
  $export['group_gifts|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_knowledges|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_knowledges';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Knowledges',
    'weight' => '104',
    'children' => array(
      0 => 'field_know_set',
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
  $export['group_knowledges|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_knowledges|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_knowledges';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Knowledges',
    'weight' => '104',
    'children' => array(
      0 => 'field_know_set',
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
  $export['group_knowledges|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_knowledges|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_knowledges';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Knowledges',
    'weight' => '20',
    'children' => array(
      0 => 'field_know_set',
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
  $export['group_knowledges|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_knowledges|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_knowledges';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Knowledges',
    'weight' => '20',
    'children' => array(
      0 => 'field_know_set',
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
  $export['group_knowledges|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_knowledges|node|mage_character_sheet|default';
  $field_group->group_name = 'group_knowledges';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Knowledges',
    'weight' => '20',
    'children' => array(
      0 => 'field_know_set',
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
  $export['group_knowledges|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_knowledges|node|mage_character_sheet|form';
  $field_group->group_name = 'group_knowledges';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Knowledges',
    'weight' => '20',
    'children' => array(
      0 => 'field_know_set',
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
  $export['group_knowledges|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_knowledges|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_knowledges';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Knowledges',
    'weight' => '16',
    'children' => array(
      0 => 'field_know_set',
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
  $export['group_knowledges|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_knowledges|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_knowledges';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Knowledges',
    'weight' => '16',
    'children' => array(
      0 => 'field_know_set',
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
  $export['group_knowledges|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_knowledges|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_knowledges';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Knowledges',
    'weight' => '21',
    'children' => array(
      0 => 'field_know_set',
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
  $export['group_knowledges|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_knowledges|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_knowledges';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Knowledges',
    'weight' => '21',
    'children' => array(
      0 => 'field_know_set',
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
  $export['group_knowledges|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_mental|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_mental';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Mental',
    'weight' => '104',
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
  $export['group_mental|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_mental|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_mental';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Mental',
    'weight' => '104',
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
  $export['group_mental|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_mental|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_mental';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Mental',
    'weight' => '21',
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
  $export['group_mental|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_mental|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_mental';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Mental',
    'weight' => '21',
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
  $export['group_mental|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_mental|node|mage_character_sheet|default';
  $field_group->group_name = 'group_mental';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Mental',
    'weight' => '6',
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
  $export['group_mental|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_mental|node|mage_character_sheet|form';
  $field_group->group_name = 'group_mental';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Mental',
    'weight' => '6',
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
  $export['group_mental|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_mental|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_mental';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_mental|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_mental|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_mental';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_mental|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_mental|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_mental';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Mental',
    'weight' => '104',
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
  $export['group_mental|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_mental|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_mental';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Mental',
    'weight' => '104',
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
  $export['group_mental|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_merits_flaws|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_merits_flaws';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Merits / Flaws',
    'weight' => '28',
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
  $export['group_merits_flaws|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_merits_flaws|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_merits_flaws';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Merits / Flaws',
    'weight' => '28',
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
  $export['group_merits_flaws|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_merits_flaws|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_merits_flaws';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Merits / Flaws',
    'weight' => '5',
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
  $export['group_merits_flaws|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_merits_flaws|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_merits_flaws';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Merits / Flaws',
    'weight' => '5',
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
  $export['group_merits_flaws|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_merits_flaws|node|mage_character_sheet|default';
  $field_group->group_name = 'group_merits_flaws';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Merits / Flaws',
    'weight' => '7',
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
  $export['group_merits_flaws|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_merits_flaws|node|mage_character_sheet|form';
  $field_group->group_name = 'group_merits_flaws';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Merits / Flaws',
    'weight' => '7',
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
  $export['group_merits_flaws|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_merits_flaws|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_merits_flaws';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_merits_flaws|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_merits_flaws|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_merits_flaws';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_merits_flaws|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_merits_flaws|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_merits_flaws';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Merits / Flaws',
    'weight' => '23',
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
  $export['group_merits_flaws|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_merits_flaws|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_merits_flaws';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Merits / Flaws',
    'weight' => '23',
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
  $export['group_merits_flaws|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notes',
    'weight' => '49',
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
  $export['group_notes|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notes',
    'weight' => '49',
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
  $export['group_notes|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notes',
    'weight' => '50',
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
  $export['group_notes|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notes',
    'weight' => '50',
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
  $export['group_notes|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|mage_character_sheet|default';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notes',
    'weight' => '49',
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
  $export['group_notes|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|mage_character_sheet|form';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Notes',
    'weight' => '49',
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
  $export['group_notes|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_notes|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_notes|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_notes|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_notes';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
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
  $export['group_notes|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_other_traits|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_other_traits';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_3';
  $field_group->data = array(
    'label' => 'Other Traits',
    'weight' => '31',
    'children' => array(
      0 => 'field_other_trait',
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
  $export['group_other_traits|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_other_traits|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_other_traits';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_3';
  $field_group->data = array(
    'label' => 'Other Traits',
    'weight' => '31',
    'children' => array(
      0 => 'field_other_trait',
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
  $export['group_other_traits|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_physical|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_physical';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Physical',
    'weight' => '102',
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
  $export['group_physical|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_physical|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_physical';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Physical',
    'weight' => '102',
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
  $export['group_physical|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_physical|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_physical';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Physical',
    'weight' => '19',
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
  $export['group_physical|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_physical|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_physical';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Physical',
    'weight' => '19',
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
  $export['group_physical|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_physical|node|mage_character_sheet|default';
  $field_group->group_name = 'group_physical';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Physical',
    'weight' => '4',
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
  $export['group_physical|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_physical|node|mage_character_sheet|form';
  $field_group->group_name = 'group_physical';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Physical',
    'weight' => '4',
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
  $export['group_physical|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_physical|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_physical';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_physical|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_physical|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_physical';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_physical|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_physical|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_physical';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Physical',
    'weight' => '102',
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
  $export['group_physical|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_physical|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_physical';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Physical',
    'weight' => '102',
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
  $export['group_physical|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_property|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_property';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Property',
    'weight' => '47',
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
  $export['group_property|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_property|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_property';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Property',
    'weight' => '47',
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
  $export['group_property|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_property|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_property';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Property',
    'weight' => '47',
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
  $export['group_property|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_property|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_property';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Property',
    'weight' => '47',
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
  $export['group_property|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_property|node|mage_character_sheet|default';
  $field_group->group_name = 'group_property';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Property',
    'weight' => '6',
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
  $export['group_property|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_property|node|mage_character_sheet|form';
  $field_group->group_name = 'group_property';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Property',
    'weight' => '6',
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
  $export['group_property|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_property|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_property';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_property|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_property|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_property';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_property|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_property|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_property';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
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
  $export['group_property|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_realms|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_realms';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_3';
  $field_group->data = array(
    'label' => 'Realms',
    'weight' => '29',
    'children' => array(
      0 => 'field_realm',
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
  $export['group_realms|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_realms|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_realms';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_3';
  $field_group->data = array(
    'label' => 'Realms',
    'weight' => '29',
    'children' => array(
      0 => 'field_realm',
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
  $export['group_realms|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_renown|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_renown';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_3';
  $field_group->data = array(
    'label' => 'Renown',
    'weight' => '24',
    'children' => array(
      0 => 'field_glory',
      1 => 'field_honor',
      2 => 'field_wisdom',
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
  $export['group_renown|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_renown|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_renown';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_3';
  $field_group->data = array(
    'label' => 'Renown',
    'weight' => '24',
    'children' => array(
      0 => 'field_glory',
      1 => 'field_honor',
      2 => 'field_wisdom',
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
  $export['group_renown|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_reson|node|mage_character_sheet|default';
  $field_group->group_name = 'group_reson';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_3';
  $field_group->data = array(
    'label' => 'Resonance',
    'weight' => '8',
    'children' => array(
      0 => 'field_res_dyn',
      1 => 'field_res_ent',
      2 => 'field_res_stat',
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
  $export['group_reson|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_reson|node|mage_character_sheet|form';
  $field_group->group_name = 'group_reson';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_3';
  $field_group->data = array(
    'label' => 'Resonance',
    'weight' => '8',
    'children' => array(
      0 => 'field_res_dyn',
      1 => 'field_res_ent',
      2 => 'field_res_stat',
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
  $export['group_reson|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '103',
    'children' => array(
      0 => 'field_skill_set',
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
  $export['group_skills|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '103',
    'children' => array(
      0 => 'field_skill_set',
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
  $export['group_skills|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '19',
    'children' => array(
      0 => 'field_skill_set',
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
  $export['group_skills|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '19',
    'children' => array(
      0 => 'field_skill_set',
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
  $export['group_skills|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|mage_character_sheet|default';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '19',
    'children' => array(
      0 => 'field_skill_set',
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
  $export['group_skills|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|mage_character_sheet|form';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '19',
    'children' => array(
      0 => 'field_skill_set',
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
  $export['group_skills|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '15',
    'children' => array(
      0 => 'field_skill_set',
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
  $export['group_skills|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '15',
    'children' => array(
      0 => 'field_skill_set',
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
  $export['group_skills|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '20',
    'children' => array(
      0 => 'field_skill_set',
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
  $export['group_skills|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_skills|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_skills';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Skills',
    'weight' => '20',
    'children' => array(
      0 => 'field_skill_set',
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
  $export['group_skills|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_social|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_social';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Social',
    'weight' => '103',
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
  $export['group_social|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_social|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_social';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Social',
    'weight' => '103',
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
  $export['group_social|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_social|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_social';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Social',
    'weight' => '20',
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
  $export['group_social|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_social|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_social';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Social',
    'weight' => '20',
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
  $export['group_social|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_social|node|mage_character_sheet|default';
  $field_group->group_name = 'group_social';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Social',
    'weight' => '5',
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
  $export['group_social|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_social|node|mage_character_sheet|form';
  $field_group->group_name = 'group_social';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Social',
    'weight' => '5',
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
  $export['group_social|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_social|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_social';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_social|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_social|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_social';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_social|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_social|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_social';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Social',
    'weight' => '103',
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
  $export['group_social|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_social|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_social';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_attributes';
  $field_group->data = array(
    'label' => 'Social',
    'weight' => '103',
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
  $export['group_social|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spheres_1|node|mage_character_sheet|default';
  $field_group->group_name = 'group_spheres_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_spheres';
  $field_group->data = array(
    'label' => 'Spheres 1',
    'weight' => '8',
    'children' => array(
      0 => 'field_sphere_corr',
      1 => 'field_sphere_ent',
      2 => 'field_sphere_for',
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
  $export['group_spheres_1|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spheres_1|node|mage_character_sheet|form';
  $field_group->group_name = 'group_spheres_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_spheres';
  $field_group->data = array(
    'label' => 'Spheres 1',
    'weight' => '8',
    'children' => array(
      0 => 'field_sphere_corr',
      1 => 'field_sphere_ent',
      2 => 'field_sphere_for',
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
  $export['group_spheres_1|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spheres_2|node|mage_character_sheet|default';
  $field_group->group_name = 'group_spheres_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_spheres';
  $field_group->data = array(
    'label' => 'Spheres 2',
    'weight' => '9',
    'children' => array(
      0 => 'field_sphere_lif',
      1 => 'field_sphere_mat',
      2 => 'field_sphere_min',
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
  $export['group_spheres_2|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spheres_2|node|mage_character_sheet|form';
  $field_group->group_name = 'group_spheres_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_spheres';
  $field_group->data = array(
    'label' => 'Spheres 2',
    'weight' => '9',
    'children' => array(
      0 => 'field_sphere_lif',
      1 => 'field_sphere_mat',
      2 => 'field_sphere_min',
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
  $export['group_spheres_2|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spheres_3|node|mage_character_sheet|default';
  $field_group->group_name = 'group_spheres_3';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_spheres';
  $field_group->data = array(
    'label' => 'Spheres 3',
    'weight' => '11',
    'children' => array(
      0 => 'field_sphere_prim',
      1 => 'field_sphere_spi',
      2 => 'field_sphere_time',
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
  $export['group_spheres_3|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spheres_3|node|mage_character_sheet|form';
  $field_group->group_name = 'group_spheres_3';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_spheres';
  $field_group->data = array(
    'label' => 'Spheres 3',
    'weight' => '11',
    'children' => array(
      0 => 'field_sphere_prim',
      1 => 'field_sphere_spi',
      2 => 'field_sphere_time',
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
  $export['group_spheres_3|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spheres|node|mage_character_sheet|default';
  $field_group->group_name = 'group_spheres';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Spheres',
    'weight' => '4',
    'children' => array(
      0 => 'group_spheres_1',
      1 => 'group_spheres_2',
      2 => 'group_spheres_3',
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
  $export['group_spheres|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_spheres|node|mage_character_sheet|form';
  $field_group->group_name = 'group_spheres';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Spheres',
    'weight' => '4',
    'children' => array(
      0 => 'group_spheres_1',
      1 => 'group_spheres_2',
      2 => 'group_spheres_3',
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
  $export['group_spheres|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_talents|node|changeling_character_sheet|default';
  $field_group->group_name = 'group_talents';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Talents',
    'weight' => '102',
    'children' => array(
      0 => 'field_talent_set',
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
  $export['group_talents|node|changeling_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_talents|node|changeling_character_sheet|form';
  $field_group->group_name = 'group_talents';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'changeling_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Talents',
    'weight' => '102',
    'children' => array(
      0 => 'field_talent_set',
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
  $export['group_talents|node|changeling_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_talents|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_talents';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Talents',
    'weight' => '18',
    'children' => array(
      0 => 'field_talent_set',
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
  $export['group_talents|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_talents|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_talents';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Talents',
    'weight' => '18',
    'children' => array(
      0 => 'field_talent_set',
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
  $export['group_talents|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_talents|node|mage_character_sheet|default';
  $field_group->group_name = 'group_talents';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Talents',
    'weight' => '18',
    'children' => array(
      0 => 'field_talent_set',
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
  $export['group_talents|node|mage_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_talents|node|mage_character_sheet|form';
  $field_group->group_name = 'group_talents';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'mage_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Talents',
    'weight' => '18',
    'children' => array(
      0 => 'field_talent_set',
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
  $export['group_talents|node|mage_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_talents|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_talents';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Talents',
    'weight' => '14',
    'children' => array(
      0 => 'field_talent_set',
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
  $export['group_talents|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_talents|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_talents';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Talents',
    'weight' => '14',
    'children' => array(
      0 => 'field_talent_set',
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
  $export['group_talents|node|vampire_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_talents|node|werewolf_character_sheet|default';
  $field_group->group_name = 'group_talents';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Talents',
    'weight' => '19',
    'children' => array(
      0 => 'field_talent_set',
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
  $export['group_talents|node|werewolf_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_talents|node|werewolf_character_sheet|form';
  $field_group->group_name = 'group_talents';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'werewolf_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_abilities';
  $field_group->data = array(
    'label' => 'Talents',
    'weight' => '19',
    'children' => array(
      0 => 'field_talent_set',
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
  $export['group_talents|node|werewolf_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_virtues|node|hunter_character_sheet|default';
  $field_group->group_name = 'group_virtues';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'default';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Virtues',
    'weight' => '6',
    'children' => array(
      0 => 'field_mercy',
      1 => 'field_vision',
      2 => 'field_zeal',
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
  $export['group_virtues|node|hunter_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_virtues|node|hunter_character_sheet|form';
  $field_group->group_name = 'group_virtues';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'hunter_character_sheet';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_column_1';
  $field_group->data = array(
    'label' => 'Virtues',
    'weight' => '6',
    'children' => array(
      0 => 'field_mercy',
      1 => 'field_vision',
      2 => 'field_zeal',
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
  $export['group_virtues|node|hunter_character_sheet|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_virtues|node|vampire_character_sheet|default';
  $field_group->group_name = 'group_virtues';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_virtues|node|vampire_character_sheet|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_virtues|node|vampire_character_sheet|form';
  $field_group->group_name = 'group_virtues';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'vampire_character_sheet';
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
  $export['group_virtues|node|vampire_character_sheet|form'] = $field_group;

  return $export;
}

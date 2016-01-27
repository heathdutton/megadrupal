<?php
/**
 * @file
 * dnd_character.features.field.inc
 */

/**
 * Implementation of hook_field_default_fields().
 */
function dnd_character_field_default_fields() {
  $fields = array();

  // Exported field: 'node-dnd_character-body'
  $fields['node-dnd_character-body'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(
        0 => 'node',
      ),
      'field_name' => 'body',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'text_with_summary',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Background information about the player character is one of the most vital pieces of information about her. Information about history and goals add an extra dimension to role playing, so flesh this out as much as possible.',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 0,
        ),
        'teaser' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(
            'trim_length' => 600,
          ),
          'type' => 'text_summary_or_trimmed',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'body',
      'label' => 'Background',
      'required' => 0,
      'settings' => array(
        'display_summary' => 1,
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '3',
          'summary_rows' => 5,
        ),
        'type' => 'text_textarea_with_summary',
        'weight' => '66',
      ),
      'widget_type' => 'text_textarea_with_summary',
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_abilities'
  $fields['node-dnd_character-field_dnd_character_abilities'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '6',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_abilities',
      'foreign keys' => array(),
      'indexes' => array(
        'ability' => array(
          0 => 'ability',
        ),
      ),
      'module' => 'dnd_fields',
      'settings' => array(
        'abilities' => 'Strength
Dexterity
Constitution
Intelligence
Wisdom
Charisma',
        'allowed_values' => array(),
        'allowed_values_function' => '',
      ),
      'translatable' => '1',
      'type' => 'dnd_fields_ability',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => array(
        0 => array(
          'ability' => 'Strength',
          'mod' => '',
          'score' => '',
          'tempmod' => '',
          'tempscore' => '',
        ),
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'dnd_fields',
          'settings' => array(),
          'type' => 'dnd_fields_ability',
          'weight' => 39,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_abilities',
      'label' => 'Ability scores',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'dnd_fields',
        'settings' => array(),
        'type' => 'dnd_fields_ability',
        'weight' => '2',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_ac_check_pen'
  $fields['node-dnd_character-field_dnd_character_ac_check_pen'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_ac_check_pen',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Any armor heavier than leather hurts a character\'s ability to use some skills. An armor check penalty number is the penalty that applies to Balance, Climb, Escape Artist, Hide, Jump, Move Silently, Sleight of Hand, and Tumble checks by a character wearing a certain kind of armor. Double the normal armor check penalty is applied to Swim checks. A character\'s encumbrance (the amount of gear carried, including armor) may also apply an armor check penalty. ',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 35,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_ac_check_pen',
      'label' => 'Armor Check Penalty',
      'required' => 0,
      'settings' => array(
        'max' => '20',
        'min' => '-10',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '38',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_ac_flat_foot'
  $fields['node-dnd_character-field_dnd_character_ac_flat_foot'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_ac_flat_foot',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A character who has not yet acted during a combat is flat-footed, not yet reacting normally to the situation. A flat-footed character loses his Dexterity bonus to AC (if any) and cannot make attacks of opportunity. ',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 34,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_ac_flat_foot',
      'label' => 'Flat Foot',
      'required' => 0,
      'settings' => array(
        'max' => '50',
        'min' => '-10',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '36',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_ac_misc_mods'
  $fields['node-dnd_character-field_dnd_character_ac_misc_mods'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_ac_misc_mods',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Miscellaneous modifiers to Armor Class can come from blessings, curses, and magic items.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 32,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_ac_misc_mods',
      'label' => 'Miscellaneous Modifers',
      'required' => 0,
      'settings' => array(
        'max' => '20',
        'min' => '-20',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '40',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_ac_modifiers'
  $fields['node-dnd_character-field_dnd_character_ac_modifiers'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '3',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_ac_modifiers',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '1',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A list of modifiers that affect the overall armor class.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 37,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_ac_modifiers',
      'label' => 'Armor Class Modifiers',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '34',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_ac_size_mod'
  $fields['node-dnd_character-field_dnd_character_ac_size_mod'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_ac_size_mod',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => array(
        0 => array(
          'value' => '0',
        ),
      ),
      'deleted' => '0',
      'description' => 'This applies to the adjustment to armor class based on size. The size modifiers are: Fine: +8, Diminutive: +4 , Tiny: +2, Small: +1 , Medium: 0, Large: −1, Huge: −2, Gargantuan: −4, Colossal: −8.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 31,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_ac_size_mod',
      'label' => 'Size Modifier',
      'required' => 0,
      'settings' => array(
        'max' => '8',
        'min' => '-8',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '34',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_ac_vs_touch'
  $fields['node-dnd_character-field_dnd_character_ac_vs_touch'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_ac_vs_touch',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Some attacks disregard armor, including shields and natural armor. In these cases, the attacker makes a touch attack roll (either ranged or melee). When you are the target of a touch attack, your AC doesn\'t include any armor bonus, shield bonus, or natural armor bonus. All other modifiers, such as your size modifier, Dexterity modifier, and deflection bonus (if any) apply normally. ',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 36,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_ac_vs_touch',
      'label' => 'Vs. Touch',
      'required' => 0,
      'settings' => array(
        'max' => '20',
        'min' => '-10',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '37',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_age'
  $fields['node-dnd_character-field_dnd_character_age'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_age',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The age in years of the player character.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 20,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_age',
      'label' => 'Age',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '10',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '57',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_alignment'
  $fields['node-dnd_character-field_dnd_character_alignment'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_alignment',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'ce' => t('Chaotic Evil'),
          'cg' => t('Chaotic Good'),
          'cn' => t('Chaotic Neutral'),
          'le' => t('Lawful Evil'),
          'lg' => t('Lawful Good'),
          'ln' => t('Lawful Neutral'),
          'ne' => t('Neutral Evil'),
          'ng' => t('Neutral Good'),
          'tn' => t('True Neutral'),
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '1',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'A creature\'s general moral and personal attitudes are represented by its alignment: lawful good, neutral good, chaotic good, lawful neutral, neutral, chaotic neutral, lawful evil, neutral evil, or chaotic evil.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 18,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_alignment',
      'label' => 'Alignment',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '29',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_appearance'
  $fields['node-dnd_character-field_dnd_character_appearance'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_appearance',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Describe the appearance of the player character to provide additional background information.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 28,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_appearance',
      'label' => 'Appearance',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '2',
        ),
        'type' => 'text_textarea',
        'weight' => '70',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_arcane_fail'
  $fields['node-dnd_character-field_dnd_character_arcane_fail'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_arcane_fail',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Armor interferes with the gestures that you need to make to cast an arcane spell. This percentage indicates the level of possible failure.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 33,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_arcane_fail',
      'label' => 'Arcane Spell Failure',
      'required' => 0,
      'settings' => array(
        'max' => '100',
        'min' => '0',
        'prefix' => '',
        'suffix' => '%',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '39',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_armor'
  $fields['node-dnd_character-field_dnd_character_armor'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_armor',
      'foreign keys' => array(),
      'indexes' => array(
        'armor' => array(
          0 => 'armor',
        ),
      ),
      'module' => 'dnd_fields',
      'settings' => array(
        'allowed_values' => array(),
        'allowed_values_function' => '',
      ),
      'translatable' => '1',
      'type' => 'dnd_fields_armor',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'dnd_fields',
          'settings' => array(),
          'type' => 'dnd_fields_armor',
          'weight' => 38,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_armor',
      'label' => 'Armor',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'dnd_fields',
        'settings' => array(),
        'type' => 'dnd_fields_armor',
        'weight' => '35',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_armor_bonus'
  $fields['node-dnd_character-field_dnd_character_armor_bonus'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_armor_bonus',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The bonus (or penalty) of the armor used. This only applies to armor currently equipped.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 29,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_armor_bonus',
      'label' => 'Armor Bonus',
      'required' => 0,
      'settings' => array(
        'max' => '100',
        'min' => '-10',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '32',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_attack_mods'
  $fields['node-dnd_character-field_dnd_character_attack_mods'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '4',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_attack_mods',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '1',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 49,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_attack_mods',
      'label' => 'Attack bonus modifiers',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '40',
        ),
        'type' => 'text_textfield',
        'weight' => '19',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_base_attack'
  $fields['node-dnd_character-field_dnd_character_base_attack'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_base_attack',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 46,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_base_attack',
      'label' => 'Base attack',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '16',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_class_level'
  $fields['node-dnd_character-field_dnd_character_class_level'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_class_level',
      'foreign keys' => array(),
      'indexes' => array(
        'class' => array(
          0 => 'class',
        ),
      ),
      'module' => 'dnd_fields',
      'settings' => array(
        'allowed_values' => array(),
        'allowed_values_function' => '',
        'class' => 'adept|' . t('Adept') . '
agent_retriever|' . t('Agent Retriever') . '
arcane_archer|' . t('Arcane Archer') . '
arcane_trickster|' . t('Arcane Trickster') . '
archmage|' . t('Archmage') . '
aristocrat|' . t('Aristocrat') . '
assassin|' . t('Assassin') . '
barbarian|' . t('Barbarian') . '
bard|' . t('Bard') . '
blackguard|' . t('Blackguard') . '
cerebremancer|' . t('Cerebremancer') . '
cleric|' . t('Cleric') . '
commoner|' . t('Commoner') . '
cosmic_descryer|' . t('Cosmic Descryer') . '
divine_emissary|' . t('Divine Emissary') . '
dragon_disciple|' . t('Dragon Disciple') . '
druid|' . t('Druid') . '
duelist|' . t('Duelist') . '
diskblade|' . t('Duskblade') . '
dwarven_defender|' . t('Dwarven Defender') . '
eldritch_knight|' . t('Eldritch Knight') . '
elocater|' . t('Elocater') . '
epic_infiltrator|' . t('Epic Infiltrator') . '
expert|' . t('Expert') . '
fighter|' . t('Fighter') . '
guardian_paramount|' . t('Guardian Paramount') . '
hierophant|' . t('Hierophant') . '
high_proselytizer|' . t('High Proselytizer') . '
horizon_walker|' . t('Horizon Walker') . '
legendary_dreadnought|' . t('Legendary Dreadnought') . '
loremaster|' . t('Loremaster') . '
metamind|' . t('Metamind') . '
monk|' . t('Monk') . '
mystic_theurge|' . t('Mystic Theurge') . '
paladin|' . t('Paladin') . '
perfect_wight|' . t('Perfect Wight') . '
psion|' . t('Psion') . '
psion_uncarnate|' . t('Psion Uncarnate') . '
psionic_fist|' . t('Psionic Fist') . '
psychic_warrior|' . t('Psychic Warrior') . '
pyrokineticist|' . t('Pyrokineticist') . '
ranger|' . t('Ranger') . '
rogue|' . t('Rogue') . '
shadowdancer|' . t('Shadowdancer') . '
slayer|' . t('Slayer') . '
sorcerer|' . t('Sorcerer') . '
soulknife|' . t('Soulknife') . '
thaumaturgist|' . t('Thaumaturgist') . '
thrallherd|' . t('Thrallherd') . '
union_sentinel|' . t('Union Sentinel') . '
war_mind|' . t('War Mind') . '
warrior|' . t('Warrior') . '
wilder|' . t('Wilder') . '
wizard|' . t('Wizard'),
      ),
      'translatable' => '1',
      'type' => 'dnd_fields_class',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => array(
        0 => array(
          'class' => 'sorcerer',
          'level' => '',
        ),
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'dnd_fields',
          'settings' => array(),
          'type' => 'dnd_fields_class',
          'weight' => 60,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_class_level',
      'label' => 'Class and level',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'dnd_fields',
        'settings' => array(),
        'type' => 'dnd_fields_class',
        'weight' => '24',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_combat_notes'
  $fields['node-dnd_character-field_dnd_character_combat_notes'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '3',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_combat_notes',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '1',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Include anything helpful to remember during combat, such as special abilities, magical effects, or tactical information.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 52,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_combat_notes',
      'label' => 'Combat notes',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '40',
        ),
        'type' => 'text_textfield',
        'weight' => '23',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_containers'
  $fields['node-dnd_character-field_dnd_character_containers'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '2',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_containers',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '64',
      ),
      'translatable' => '1',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'List the two primary containers for your gear and equipment that the PC carries.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 55,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_containers',
      'label' => 'Containers',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '30',
        ),
        'type' => 'text_textfield',
        'weight' => '11',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_deity'
  $fields['node-dnd_character-field_dnd_character_deity'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_deity',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '128',
      ),
      'translatable' => '1',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 42,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_deity',
      'label' => 'Deity',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '30',
        ),
        'type' => 'text_textfield',
        'weight' => '65',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_equipment'
  $fields['node-dnd_character-field_dnd_character_equipment'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '2',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_equipment',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'List the equipment this PC carries. Each text field corresponds to one of the containers above.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 56,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_equipment',
      'label' => 'Equipment',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5',
        ),
        'type' => 'text_textarea',
        'weight' => '12',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_eyes'
  $fields['node-dnd_character-field_dnd_character_eyes'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_eyes',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '32',
      ),
      'translatable' => '1',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Eye color helps create a physical description of the player character.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 26,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_eyes',
      'label' => 'Eye Color',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '30',
        ),
        'type' => 'text_textfield',
        'weight' => '63',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_feats'
  $fields['node-dnd_character-field_dnd_character_feats'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_feats',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'ability_focus' => 'Ability Focus',
          'acrobatic' => 'Acrobatic',
          'additional_magic_item_space' => 'Additional Magic Item Space',
          'agile' => 'Agile',
          'alertness' => 'Alertness',
          'aligned_attack' => 'Aligned Attack',
          'animal_affinity' => 'Animal Affinity',
          'antipsionic_magic' => 'Antipsionic Magic',
          'armor_proficiency_heavy' => 'Armor Proficiency (Heavy)',
          'armor_proficiency_light' => 'Armor Proficiency (Light)',
          'armor_proficiency_medium' => 'Armor Proficiency (Medium)',
          'armor_skin' => 'Armor Skin',
          'athletic' => 'Athletic',
          'augment_summoning' => 'Augment Summoning',
          'augmented_alchemy' => 'Augmented Alchemy',
          'automatic_quicken_spell' => 'Automatic Quicken Spell',
          'automatic_silent_spell' => 'Automatic Silent Spell',
          'automatic_still_spell' => 'Automatic Still Spell',
          'autonomous' => 'Autonomous',
          'awesome_blow' => 'Awesome Blow',
          'bane_of_enemies' => 'Bane of Enemies',
          'blind_fight' => 'Blind-Fight',
          'blinding_speed' => 'Blinding Speed',
          'blindsight_5_ft_radius' => 'Blindsight, 5-Ft. Radius',
          'body_fuel' => 'Body Fuel',
          'bonus_domain' => 'Bonus Domain',
          'boost_construct' => 'Boost Construct',
          'brew_potion' => 'Brew Potion',
          'bulwark_of_defense' => 'Bulwark of Defense',
          'burrowing_power' => 'Burrowing Power',
          'chain_power' => 'Chain Power',
          'chaotic_mind' => 'Chaotic Mind',
          'chaotic_rage' => 'Chaotic Rage',
          'cleave' => 'Cleave',
          'cloak_dance' => 'Cloak Dance',
          'closed_mind' => 'Closed Mind',
          'colossal_wild_shape' => 'Colossal Wild Shape',
          'combat_archery' => 'Combat Archery',
          'combat_casting' => 'Combat Casting',
          'combat_expertise' => 'Combat Expertise',
          'combat_manifestation' => 'Combat Manifestation',
          'combat_reflexes' => 'Combat Reflexes',
          'craft_cognizance_crystal' => 'Craft Cognizance Crystal',
          'craft_construct' => 'Craft Construct',
          'craft_dorje' => 'Craft Dorje',
          'craft_epic_magic_arms_and_armor' => 'Craft Epic Magic Arms and Armor',
          'craft_epic_rod' => 'Craft Epic Rod',
          'craft_epic_staff' => 'Craft Epic Staff',
          'craft_epic_wondrous_item' => 'Craft Epic Wondrous Item',
          'craft_magic_arms_and_armor' => 'Craft Magic Arms and Armor',
          'craft_psicrown' => 'Craft Psicrown',
          'craft_psionic_arms_and_armor' => 'Craft Psionic Arms and Armor',
          'craft_psionic_construct' => 'Craft Psionic Construct',
          'craft_rod' => 'Craft Rod',
          'craft_staff' => 'Craft Staff',
          'craft_universal_item' => 'Craft Universal Item',
          'craft_wand' => 'Craft Wand',
          'craft_wondrous_item' => 'Craft Wondrous Item',
          'damage_reduction' => 'Damage Reduction',
          'deadly_precision' => 'Deadly Precision',
          'deafening_song' => 'Deafening Song',
          'death_of_enemies' => 'Death of Enemies',
          'deceitful' => 'Deceitful',
          'deep_impact' => 'Deep Impact',
          'deflect_arrows' => 'Deflect Arrows',
          'deft_hands' => 'Deft Hands',
          'delay_power' => 'Delay Power',
          'devastating_critical' => 'Devastating Critical',
          'dexterous_fortitude' => 'Dexterous Fortitude',
          'dexterous_will' => 'Dexterous Will',
          'diehard' => 'Diehard',
          'diligent' => 'Diligent',
          'diminutive_wild_shape' => 'Diminutive Wild Shape',
          'dire_charge' => 'Dire Charge',
          'disguise_spell' => 'Disguise Spell',
          'distant_shot' => 'Distant Shot',
          'divine_might' => 'Divine Might',
          'divine_vengeance' => 'Divine Vengeance',
          'dodge' => 'Dodge',
          'dragon_wild_shape' => 'Dragon Wild Shape',
          'efficient_item_creation' => 'Efficient Item Creation',
          'empower_power' => 'Empower Power',
          'empower_spell' => 'Empower Spell',
          'empower_spell_like_ability' => 'Empower Spell-Like Ability',
          'endurance' => 'Endurance',
          'energy_resistance' => 'Energy Resistance',
          'energy_substitution' => 'Energy Substitution',
          'enhance_spell' => 'Enhance Spell',
          'enlarge_power' => 'Enlarge Power',
          'enlarge_spell' => 'Enlarge Spell',
          'epic_dodge' => 'Epic Dodge',
          'epic_endurance' => 'Epic Endurance',
          'epic_expanded_knowledge' => 'Epic Expanded Knowledge',
          'epic_fortitude' => 'Epic Fortitude',
          'epic_inspiration' => 'Epic Inspiration',
          'epic_leadership' => 'Epic Leadership',
          'epic_prowess' => 'Epic Prowess',
          'epic_psionic_focus' => 'Epic Psionic Focus',
          'epic_reflexes' => 'Epic Reflexes',
          'epic_reputation' => 'Epic Reputation',
          'epic_skill_focus' => 'Epic Skill Focus',
          'epic_speed' => 'Epic Speed',
          'epic_spell_focus' => 'Epic Spell Focus',
          'epic_spell_penetration' => 'Epic Spell Penetration',
          'epic_spellcasting' => 'Epic Spellcasting',
          'epic_toughness' => 'Epic Toughness',
          'epic_weapon_focus' => 'Epic Weapon Focus',
          'epic_weapon_specialization' => 'Epic Weapon Specialization',
          'epic_will' => 'Epic Will',
          'eschew_materials' => 'Eschew Materials',
          'exceptional_deflection' => 'Exceptional Deflection',
          'exotic_weapon_proficiency' => 'Exotic Weapon Proficiency',
          'expanded_knowledge' => 'Expanded Knowledge',
          'extend_power' => 'Extend Power',
          'extend_spell' => 'Extend Spell',
          'extended_life_span' => 'Extended Life Span',
          'extra_music' => 'Extra Music',
          'extra_turning' => 'Extra Turning',
          'eyes_in_the_back_of_your_head' => 'Eyes in the Back of Your Head',
          'familiar_spell' => 'Familiar Spell',
          'far_shot' => 'Far Shot',
          'fast_healing' => 'Fast Healing',
          'feat_name' => 'Feat Name',
          'fell_shot' => 'Fell Shot',
          'fine_wild_shape' => 'Fine Wild Shape',
          'fleet_of_foot' => 'Fleet of Foot',
          'flyby_attack' => 'Flyby Attack',
          'focused_sunder' => 'Focused Sunder',
          'force_of_will' => 'Force of Will',
          'forge_epic_ring' => 'Forge Epic Ring',
          'forge_ring' => 'Forge Ring',
          'gargantuan_wild_shape' => 'Gargantuan Wild Shape',
          'ghost_attack' => 'Ghost Attack',
          'great_charisma' => 'Great Charisma',
          'great_cleave' => 'Great Cleave',
          'great_constitution' => 'Great Constitution',
          'great_dexterity' => 'Great Dexterity',
          'great_fortitude' => 'Great Fortitude',
          'great_intelligence' => 'Great Intelligence',
          'great_smiting' => 'Great Smiting',
          'great_strength' => 'Great Strength',
          'great_wisdom' => 'Great Wisdom',
          'greater_manyshot' => 'Greater Manyshot',
          'greater_multiweapon_fighting' => 'Greater Multiweapon Fighting',
          'greater_power_penetration' => 'Greater Power Penetration',
          'greater_power_specialization' => 'Greater Power Specialization',
          'greater_psionic_endowment' => 'Greater Psionic Endowment',
          'greater_psionic_fist' => 'Greater Psionic Fist',
          'greater_psionic_shot' => 'Greater Psionic Shot',
          'greater_psionic_weapon' => 'Greater Psionic Weapon',
          'greater_spell_focus' => 'Greater Spell Focus',
          'greater_spell_penetration' => 'Greater Spell Penetration',
          'greater_two_weapon_fighting' => 'Greater Two-Weapon Fighting',
          'greater_weapon_focus' => 'Greater Weapon Focus',
          'greater_weapon_specialization' => 'Greater Weapon Specialization',
          'group_inspiration' => 'Group Inspiration',
          'heighten_spell' => 'Heighten Spell',
          'hindering_song' => 'Hindering Song',
          'hold_the_line' => 'Hold the Line',
          'holy_strike' => 'Holy Strike',
          'hostile_mind' => 'Hostile Mind',
          'hover' => 'Hover',
          'ignore_material_components' => 'Ignore Material Components',
          'imprint_stone' => 'Imprint Stone',
          'improved_alignment_based_casting' => 'Improved Alignment-Based Casting',
          'improved_arrow_of_death' => 'Improved Arrow of Death',
          'improved_aura_of_courage' => 'Improved Aura of Courage',
          'improved_aura_of_despair' => 'Improved Aura of Despair',
          'improved_bull_rush' => 'Improved Bull Rush',
          'improved_combat_casting' => 'Improved Combat Casting',
          'improved_combat_reflexes' => 'Improved Combat Reflexes',
          'improved_counterspell' => 'Improved Counterspell',
          'improved_critical' => 'Improved Critical',
          'improved_darkvision' => 'Improved Darkvision',
          'improved_death_attack' => 'Improved Death Attack',
          'improved_disarm' => 'Improved Disarm',
          'improved_elemental_wild_shape' => 'Improved Elemental Wild Shape',
          'improved_familiar' => 'Improved Familiar',
          'improved_favored_enemy' => 'Improved Favored Enemy',
          'improved_feint' => 'Improved Feint',
          'improved_flyby_attack' => 'Improved Flyby Attack',
          'improved_grapple' => 'Improved Grapple',
          'improved_heighten_spell' => 'Improved Heighten Spell',
          'improved_initiative' => 'Improved Initiative',
          'improved_ki_strike' => 'Improved Ki Strike',
          'improved_low_light_vision' => 'Improved Low-Light Vision',
          'improved_manifestation' => 'Improved Manifestation',
          'improved_manyshot' => 'Improved Manyshot',
          'improved_metamagic' => 'Improved Metamagic',
          'improved_metapsionics' => 'Improved Metapsionics',
          'improved_multiattack' => 'Improved Multiattack',
          'improved_multiweapon_fighting' => 'Improved Multiweapon Fighting',
          'improved_natural_armor' => 'Improved Natural Armor',
          'improved_natural_attack' => 'Improved Natural Attack',
          'improved_overrun' => 'Improved Overrun',
          'improved_precise_shot' => 'Improved Precise Shot',
          'improved_psicrystal' => 'Improved Psicrystal',
          'improved_shield_bash' => 'Improved Shield Bash',
          'improved_sneak_attack' => 'Improved Sneak Attack',
          'improved_spell_capacity' => 'Improved Spell Capacity',
          'improved_spell_resistance' => 'Improved Spell Resistance',
          'improved_stunning_fist' => 'Improved Stunning Fist',
          'improved_sunder' => 'Improved Sunder',
          'improved_trip' => 'Improved Trip',
          'improved_turning' => 'Improved Turning',
          'improved_two_weapon_fighting' => 'Improved Two-Weapon Fighting',
          'improved_unarmed_strike' => 'Improved Unarmed Strike',
          'improved_whirlwind_attack' => 'Improved Whirlwind Attack',
          'incite_rage' => 'Incite Rage',
          'infinite_deflection' => 'Infinite Deflection',
          'inquisitor' => 'Inquisitor',
          'inspire_excellence' => 'Inspire Excellence',
          'instant_reload' => 'Instant Reload',
          'intensify_spell' => 'Intensify Spell',
          'investigator' => 'Investigator',
          'iron_will' => 'Iron Will',
          'jack_of_all_trades' => 'Jack of all Trades',
          'keen_strike' => 'Keen Strike',
          'knock_down' => 'Knock-Down',
          'lasting_inspiration' => 'Lasting Inspiration',
          'leadership' => 'Leadership',
          'legendary_climber' => 'Legendary Climber',
          'legendary_commander' => 'Legendary Commander',
          'legendary_leaper' => 'Legendary Leaper',
          'legendary_rider' => 'Legendary Rider',
          'legendary_tracker' => 'Legendary Tracker',
          'legendary_wrestler' => 'Legendary Wrestler',
          'lightning_reflexes' => 'Lightning Reflexes',
          'lingering_damage' => 'Lingering Damage',
          'magical_aptitude' => 'Magical Aptitude',
          'magical_beast_companion' => 'Magical Beast Companion',
          'magical_beast_wild_shape' => 'Magical Beast Wild Shape',
          'manyshot' => 'Manyshot',
          'martial_weapon_proficiency' => 'Martial Weapon Proficiency',
          'master_staff' => 'Master Staff',
          'master_wand' => 'Master Wand',
          'maximize_power' => 'Maximize Power',
          'maximize_spell' => 'Maximize Spell',
          'mental_leap' => 'Mental Leap',
          'mental_resistance' => 'Mental Resistance',
          'metamorphic_transfer' => 'Metamorphic Transfer',
          'mighty_rage' => 'Mighty Rage',
          'mind_over_body' => 'Mind Over Body',
          'mobile_defense' => 'Mobile Defense',
          'mobility' => 'Mobility',
          'mounted_archery' => 'Mounted Archery',
          'mounted_combat' => 'Mounted Combat',
          'multiattack' => 'Multiattack',
          'multispell' => 'Multispell',
          'multiweapon_fighting' => 'Multiweapon Fighting',
          'multiweapon_rend' => 'Multiweapon Rend',
          'music_of_the_gods' => 'Music of the Gods',
          'narrow_mind' => 'Narrow Mind',
          'natural_spell' => 'Natural Spell',
          'negative_energy_burst' => 'Negative Energy Burst',
          'negotiator' => 'Negotiator',
          'nimble_fingers' => 'Nimble Fingers',
          'open_minded' => 'Open Minded',
          'opportunity_power' => 'Opportunity Power',
          'overchannel' => 'Overchannel',
          'overwhelming_critical' => 'Overwhelming Critical',
          'penetrate_damage_reduction' => 'Penetrate Damage Reduction',
          'perfect_health' => 'Perfect Health',
          'perfect_multiweapon_fighting' => 'Perfect Multiweapon Fighting',
          'perfect_two_weapon_fighting' => 'Perfect Two-Weapon Fighting',
          'permanent_emanation' => 'Permanent Emanation',
          'persistent_spell' => 'Persistent Spell',
          'persuasive' => 'Persuasive',
          'planar_turning' => 'Planar Turning',
          'plant_control' => 'Plant Control',
          'plant_defiance' => 'Plant Defiance',
          'plant_wild_shape' => 'Plant Wild Shape',
          'point_blank_shot' => 'Point Blank Shot',
          'polyglot' => 'Polyglot',
          'positive_energy_aura' => 'Positive Energy Aura',
          'power_attack' => 'Power Attack',
          'power_critical' => 'Power Critical',
          'power_knowledge' => 'Power Knowledge',
          'power_penetration' => 'Power Penetration',
          'power_specialization' => 'Power Specialization',
          'precise_shot' => 'Precise Shot',
          'psicrystal_affinity' => 'Psicrystal Affinity',
          'psicrystal_containment' => 'Psicrystal Containment',
          'psicrystal_power' => 'Psicrystal Power',
          'psionic_affinity' => 'Psionic Affinity',
          'psionic_body' => 'Psionic Body',
          'psionic_charge' => 'Psionic Charge',
          'psionic_dodge' => 'Psionic Dodge',
          'psionic_endowment' => 'Psionic Endowment',
          'psionic_fist' => 'Psionic Fist',
          'psionic_hole' => 'Psionic Hole',
          'psionic_meditation' => 'Psionic Meditation',
          'psionic_shot' => 'Psionic Shot',
          'psionic_talent' => 'Psionic Talent',
          'psionic_weapon' => 'Psionic Weapon',
          'quick_draw' => 'Quick Draw',
          'quicken_power' => 'Quicken Power',
          'quicken_spell' => 'Quicken Spell',
          'quicken_spell_like_ability' => 'Quicken Spell-Like Ability',
          'ranged_inspiration' => 'Ranged Inspiration',
          'rapid_inspiration' => 'Rapid Inspiration',
          'rapid_metabolism' => 'Rapid Metabolism',
          'rapid_reload' => 'Rapid Reload',
          'rapid_shot' => 'Rapid Shot',
          'reach_spell' => 'Reach Spell',
          'reactive_countersong' => 'Reactive Countersong',
          'reckless_offense' => 'Reckless Offense',
          'reflect_arrows' => 'Reflect Arrows',
          'repeat_spell' => 'Repeat Spell',
          'return_shot' => 'Return Shot',
          'ride_by_attack' => 'Ride-by Attack',
          'righteous_strike' => 'Righteous Strike',
          'ruinous_rage' => 'Ruinous Rage',
          'run' => 'Run',
          'sacred_spell' => 'Sacred Spell',
          'scribe_epic_scroll' => 'Scribe Epic Scroll',
          'scribe_scroll' => 'Scribe Scroll',
          'scribe_tattoo' => 'Scribe Tattoo',
          'self_concealment' => 'Self-Concealment',
          'self_sufficient' => 'Self-Sufficient',
          'sharp_shooting' => 'Sharp-Shooting',
          'shattering_strike' => 'Shattering Strike',
          'shield_proficiency' => 'Shield Proficiency',
          'shot_on_the_run' => 'Shot on the Run',
          'sidestep_charge' => 'Sidestep Charge',
          'silent_spell' => 'Silent Spell',
          'simple_weapon_proficiency' => 'Simple Weapon Proficiency',
          'skill_focus' => 'Skill Focus',
          'snatch' => 'Snatch',
          'snatch_arrows' => 'Snatch Arrows',
          'sneak_attack_of_opportunity' => 'Sneak Attack of Opportunity',
          'spectral_strike' => 'Spectral Strike',
          'speed_of_thought' => 'Speed of Thought',
          'spell_focus' => 'Spell Focus',
          'spell_knowledge' => 'Spell Knowledge',
          'spell_mastery' => 'Spell Mastery',
          'spell_opportunity' => 'Spell Opportunity',
          'spell_penetration' => 'Spell Penetration',
          'spell_stowaway' => 'Spell Stowaway',
          'spellcasting_harrier' => 'Spellcasting Harrier',
          'spirited_charge' => 'Spirited Charge',
          'split_psionic_ray' => 'Split Psionic Ray',
          'spontaneous_domain_access' => 'Spontaneous Domain Access',
          'spontaneous_spell' => 'Spontaneous Spell',
          'spring_attack' => 'Spring Attack',
          'stand_still' => 'Stand Still',
          'stealthy' => 'Stealthy',
          'still_spell' => 'Still Spell',
          'storm_of_throws' => 'Storm of Throws',
          'stunning_fist' => 'Stunning Fist',
          'subdual_substitution' => 'Subdual Substitution',
          'superior_expertise' => 'Superior Expertise',
          'superior_initiative' => 'Superior Initiative',
          'swarm_of_arrows' => 'Swarm of Arrows',
          'talented' => 'Talented',
          'tenacious_magic' => 'Tenacious Magic',
          'terrifying_rage' => 'Terrifying Rage',
          'thundering_rage' => 'Thundering Rage',
          'toughness' => 'Toughness',
          'tower_shield_proficiency' => 'Tower Shield Proficiency',
          'track' => 'Track',
          'trample' => 'Trample',
          'trap_sense' => 'Trap Sense',
          'twin_power' => 'Twin Power',
          'two_weapon_defense' => 'Two-Weapon Defense',
          'two_weapon_fighting' => 'Two-Weapon Fighting',
          'two_weapon_rend' => 'Two-Weapon Rend',
          'unavoidable_strike' => 'Unavoidable Strike',
          'uncanny_accuracy' => 'Uncanny Accuracy',
          'unconditional_power' => 'Unconditional Power',
          'undead_mastery' => 'Undead Mastery',
          'unholy_strike' => 'Unholy Strike',
          'up_the_walls' => 'Up the Walls',
          'vermin_wild_shape' => 'Vermin Wild Shape',
          'vorpal_strike' => 'Vorpal Strike',
          'weapon_finesse' => 'Weapon Finesse',
          'weapon_focus' => 'Weapon Focus',
          'weapon_specialization' => 'Weapon Specialization',
          'whirlwind_attack' => 'Whirlwind Attack',
          'widen_aura_of_courage' => 'Widen Aura of Courage',
          'widen_aura_of_despair' => 'Widen Aura of Despair',
          'widen_power' => 'Widen Power',
          'widen_spell' => 'Widen Spell',
          'wild_talent' => 'Wild Talent',
          'wingover' => 'Wingover',
          'wounding_attack' => 'Wounding Attack',
          'zone_of_animation' => 'Zone of Animation',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '1',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Some feats have prerequisites. Your character must have the indicated ability score, class feature, feat, skill, base attack bonus, or other quality designated in order to select or use that feat. A character can gain a feat at the same level at which he or she gains the prerequisite. A character can\'t use a feat if he or she has lost a prerequisite.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 9,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_feats',
      'label' => 'Feats',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '32',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_gender'
  $fields['node-dnd_character-field_dnd_character_gender'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_gender',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'female' => 'Female',
          'male' => 'Male',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '1',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 17,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_gender',
      'label' => 'Gender',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '56',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_goals'
  $fields['node-dnd_character-field_dnd_character_goals'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_goals',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Describe the goals of the player character to provide additional background information.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 28,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_goals',
      'label' => 'Goals',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '2',
        ),
        'type' => 'text_textarea',
        'weight' => '68',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_hair'
  $fields['node-dnd_character-field_dnd_character_hair'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_hair',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '32',
      ),
      'translatable' => '1',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Hair color helps create a player character\'s physical description.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 25,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_hair',
      'label' => 'Hair Color',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '30',
        ),
        'type' => 'text_textfield',
        'weight' => '61',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_height'
  $fields['node-dnd_character-field_dnd_character_height'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_height',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The height of the player character, in inches. For reference, 4 feet is 48 inches, 6 feet is 72 inches, and 8 feet is 96 inches.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 22,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_height',
      'label' => 'Height',
      'required' => 0,
      'settings' => array(
        'max' => '255',
        'min' => '0',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '59',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_history'
  $fields['node-dnd_character-field_dnd_character_history'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_history',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Describe the history of the player character to provide additional background information.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 28,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_history',
      'label' => 'History',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '2',
        ),
        'type' => 'text_textarea',
        'weight' => '67',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_hp'
  $fields['node-dnd_character-field_dnd_character_hp'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_hp',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Each character has hit points (hp), representing how difficult he or she is to kill. At 1st level, wizards and sorcerers get 4 hp. Rogues and bards get 6 hp. Clerics, druids, and monks get 8 hp. Fighters, paladins, and rangers get 10 hp. Barbarians get 12 hp. To this number, add your character\'s Constitution modifier.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 12,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_hp',
      'label' => 'Hit Points',
      'required' => 1,
      'settings' => array(
        'max' => '2000',
        'min' => '1',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '15',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_init'
  $fields['node-dnd_character-field_dnd_character_init'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_init',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 43,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_init',
      'label' => 'Initiative',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '20',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_init_mod'
  $fields['node-dnd_character-field_dnd_character_init_mod'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_init_mod',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 44,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_init_mod',
      'label' => 'Initiative modifier',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '21',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_init_mods'
  $fields['node-dnd_character-field_dnd_character_init_mods'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '3',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_init_mods',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '1',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 45,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_init_mods',
      'label' => 'Initiative modifiers',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '40',
        ),
        'type' => 'text_textfield',
        'weight' => '22',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_languages'
  $fields['node-dnd_character-field_dnd_character_languages'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_languages',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'List the languages spoken by this character.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 58,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_languages',
      'label' => 'Languages',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '3',
        ),
        'type' => 'text_textarea',
        'weight' => '33',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_laterality'
  $fields['node-dnd_character-field_dnd_character_laterality'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_laterality',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '16',
      ),
      'translatable' => '1',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Laterality is the "handedness" of the player character; right-handed or left-handed. Ambidextrous is possible, but requires a Feat.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 27,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_laterality',
      'label' => 'Laterality',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '10',
        ),
        'type' => 'text_textfield',
        'weight' => '64',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_melee'
  $fields['node-dnd_character-field_dnd_character_melee'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_melee',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 47,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_melee',
      'label' => 'Melee attack',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '17',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_notes'
  $fields['node-dnd_character-field_dnd_character_notes'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_notes',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Provide additional notes about the player character.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 28,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_notes',
      'label' => 'Notes',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '2',
        ),
        'type' => 'text_textarea',
        'weight' => '71',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_personality'
  $fields['node-dnd_character-field_dnd_character_personality'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_personality',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Describe the personality of the player character to provide additional background information.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 28,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_personality',
      'label' => 'Personality/Behavior',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '2',
        ),
        'type' => 'text_textarea',
        'weight' => '69',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_race'
  $fields['node-dnd_character-field_dnd_character_race'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_race',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'dwarf' => 'Dwarf',
          'elf' => 'Elf',
          'gnome' => 'Gnome',
          'half_elf' => 'Half-Elf',
          'half_orc' => 'Half-Orc',
          'halfling' => 'Halfling',
          'human' => 'Human',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '1',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 8,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_race',
      'label' => 'Race',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '27',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_raceclass'
  $fields['node-dnd_character-field_dnd_character_raceclass'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_raceclass',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'List any notes relevant to this character that come from race or class. Ex: Elves are immune to sleep and charm effects.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 59,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_raceclass',
      'label' => 'Racial traits and class features',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '3',
        ),
        'type' => 'text_textarea',
        'weight' => '28',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_ranged'
  $fields['node-dnd_character-field_dnd_character_ranged'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_ranged',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 48,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_ranged',
      'label' => 'Ranged attack',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '18',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_save_mods'
  $fields['node-dnd_character-field_dnd_character_save_mods'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '2',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_save_mods',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '1',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'List the different items that affect saving throws, especially the miscellaneous modifiers.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 16,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_save_mods',
      'label' => 'Saving Throw Modifiers',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '60',
        ),
        'type' => 'text_textfield',
        'weight' => '17',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_saves'
  $fields['node-dnd_character-field_dnd_character_saves'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '3',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_saves',
      'foreign keys' => array(),
      'indexes' => array(
        'save' => array(
          0 => 'save',
        ),
      ),
      'module' => 'dnd_fields',
      'settings' => array(
        'allowed_values' => array(),
        'allowed_values_function' => '',
        'saves' => 'Fortitude
Will
Reflex',
      ),
      'translatable' => '1',
      'type' => 'dnd_fields_save',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => array(
        0 => array(
          'magicmod' => '',
          'miscmod' => '',
          'save' => 'Fortitude',
        ),
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'dnd_fields',
          'settings' => array(),
          'type' => 'dnd_fields_save',
          'weight' => 40,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_saves',
      'label' => 'Saving throws',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'dnd_fields',
        'settings' => array(),
        'type' => 'dnd_fields_save',
        'weight' => '16',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_shield_bon'
  $fields['node-dnd_character-field_dnd_character_shield_bon'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_shield_bon',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The bonus (or penalty) of the shield used. This only applies to a single shield currently equipped.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 30,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_shield_bon',
      'label' => 'Sheild Bonus',
      'required' => 0,
      'settings' => array(
        'max' => '20',
        'min' => '-10',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '33',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_size'
  $fields['node-dnd_character-field_dnd_character_size'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_size',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'colossal' => 'Colossal',
          'diminutive' => 'Diminutive',
          'fine' => 'Fine',
          'gargantuan' => 'Gargantuan',
          'huge' => 'Huge',
          'large' => 'Large',
          'medium' => 'Medium',
          'small' => 'Small',
          'tiny' => 'Tiny',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '1',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => array(
        0 => array(
          'value' => 'medium',
        ),
      ),
      'deleted' => '0',
      'description' => 'Creatures smaller than Small or larger than Medium have special rules relating to position.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 21,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_size',
      'label' => 'Size',
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '58',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_skill_mods'
  $fields['node-dnd_character-field_dnd_character_skill_mods'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_skill_mods',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 57,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_skill_mods',
      'label' => 'Skill modifiers',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '3',
        ),
        'type' => 'text_textarea',
        'weight' => '31',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_skills'
  $fields['node-dnd_character-field_dnd_character_skills'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_skills',
      'foreign keys' => array(),
      'indexes' => array(
        'skill' => array(
          0 => 'skill',
        ),
      ),
      'module' => 'dnd_fields',
      'settings' => array(
        'allowed_values' => array(),
        'allowed_values_function' => '',
        'skill' => 'appraise|' . t('Appraise') . '
autohypnosis|' . t('Autohypnosis') . '
balance|' . t('Balance') . '
bluff|' . t('Bluff') . '
climb|' . t('Climb') . '
concentration|' . t('Concentration') . '
concentration|' . t('Concentration') . '
craft_general|' . t('Craft: General') . '
craft_alchemy|' . t('Craft: Alchemy') . '
craft_armorsmithing|' . t('Craft: Armorsmithing') . '
craft_blacksmithing|' . t('Craft: Blacksmithing') . '
craft_dreamweaving|' . t('Craft: Dreamweaving') . '
craft_gemcutting|' . t('Craft: Gemcutting') . '
craft_leatherworking|' . t('Craft: Leatherworking') . '
craft_metalworking|' . t('Craft: Metalworking') . '
craft_painting|' . t('Craft: Painting') . '
craft_sculpting|' . t('Craft: Sculpting') . '
craft_shipmaking|' . t('Craft: Shipmaking') . '
craft_stonemasonry|' . t('Craft: Stonemasonry') . '
craft_stoneworking|' . t('Craft: Stoneworking') . '
craft_trapmaking|' . t('Craft: Trapmaking') . '
craft_weaponsmithing|' . t('Craft: Weaponsmithing') . '
decipher_script|' . t('Decipher Script') . '
diplomacy|' . t('Diplomacy') . '
disable_device|' . t('Disable Device') . '
disguise|' . t('Disguise') . '
escape_artist|' . t('Escape Artist') . '
forgery|' . t('Forgery') . '
gather_information|' . t('Gather Information') . '
handle_animal|' . t('Handle Animal') . '
heal|' . t('Heal') . '
hide|' . t('Hide') . '
intimidate|' . t('Intimidate') . '
jump|' . t('Jump') . '
knowledge_general|' . t('Knowledge: General') . '
knowledge_arcana|' . t('Knowledge: Arcana') . '
knowledge_architecture_and_engineering|' . t('Knowledge: Architecture and Engineering') . '
knowledge_dungeoneering|' . t('Knowledge: Dungeoneering') . '
knowledge_geography|' . t('Knowledge: Geography') . '
knowledge_history|' . t('Knowledge: History') . '
knowledge_local|' . t('Knowledge: Local') . '
knowledge_nature|' . t('Knowledge: Nature') . '
knowledge_nobility_and_royalty|' . t('Knowledge: Nobility and Royalty') . '
knowledge_psionics|' . t('Knowledge: Psionics') . '
knowledge_religion|' . t('Knowledge: Religion') . '
knowledge_the_planes|' . t('Knowledge: The Planes') . '
listen|' . t('Listen') . '
move_silently|' . t('Move Silently') . '
open_lock|' . t('Open Lock') . '
perform_general|' . t('Perform: General') . '
perform_act|' . t('Perform: Act') . '
perform_comedy|' . t('Perform: Comedy') . '
perform_dance|' . t('Perform: Dance') . '
perform_keyboard_instruments|' . t('Perform: Keyboard Instruments') . '
perform_oratory|' . t('Perform: Oratory') . '
perform_percussion_instruments|' . t('Perform: Percussion Instruments') . '
perform_sing|' . t('Perform: Sing') . '
perform_string_instruments|' . t('Perform: String Instruments') . '
perform_wind_instruments|' . t('Perform: Wind Instruments') . '
profession_general|' . t('Profession: General') . '
profession_bookkeeper|' . t('Profession: Bookkeeper') . '
profession_hunter|' . t('Profession: Hunter') . '
profession_miner|' . t('Profession: Miner') . '
profession_siege_engineer|' . t('Profession: Siege Engineer') . '
psicraft|' . t('Psicraft') . '
ride|' . t('Ride') . '
search|' . t('Search') . '
sense_motive|' . t('Sense Motive') . '
sleight_of_hand|' . t('Sleight of Hand') . '
speak_language_general|' . t('Speak Language: General') . '
speak_language_abyssal|' . t('Speak Language: Abyssal') . '
speak_language_aquan|' . t('Speak Language: Aquan') . '
speak_language_auran|' . t('Speak Language: Auran') . '
speak_language_celestial|' . t('Speak Language: Celestial') . '
speak_language_common|' . t('Speak Language: Common') . '
speak_language_draconic|' . t('Speak Language: Draconic') . '
speak_language_drow_sign_language|' . t('Speak Language: Drow Sign Language') . '
speak_language_druidic|' . t('Speak Language: Druidic') . '
speak_language_dwarven|' . t('Speak Language: Dwarven') . '
speak_language_elven|' . t('Speak Language: Elven') . '
speak_language_formian|' . t('Speak Language: Formian') . '
speak_language_giant|' . t('Speak Language: Giant') . '
speak_language_gnoll|' . t('Speak Language: Gnoll') . '
speak_language_gnome|' . t('Speak Language: Gnome') . '
speak_language_goblin|' . t('Speak Language: Goblin') . '
speak_language_grimlock|' . t('Speak Language: Grimlock') . '
speak_language_halfling|' . t('Speak Language: Halfling') . '
speak_language_ignan|' . t('Speak Language: Ignan') . '
speak_language_infernal|' . t('Speak Language: Infernal') . '
speak_language_maenad|' . t('Speak Language: Maenad') . '
speak_language_orc|' . t('Speak Language: Orc') . '
speak_language_sphinx|' . t('Speak Language: Sphinx') . '
speak_language_sylvan|' . t('Speak Language: Sylvan') . '
speak_language_terran|' . t('Speak Language: Terran') . '
speak_language_undercommon|' . t('Speak Language: Undercommon') . '
speak_language_worg|' . t('Speak Language: Worg') . '
speak_language_xeph|' . t('Speak Language: Xeph') . '
spellcraft|' . t('Spellcraft') . '
spot|' . t('Spot') . '
survival|' . t('Survival') . '
swim|' . t('Swim') . '
tumble|' . t('Tumble') . '
use_magic_device|' . t('Use Magic Device') . '
use_psionic_device|' . t('Use Psionic Device') . '
use_rope|' . t('Use Rope') . ',
      '),
      'translatable' => '1',
      'type' => 'dnd_fields_skill',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'dnd_fields',
          'settings' => array(),
          'type' => 'dnd_fields_skill',
          'weight' => 41,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_skills',
      'label' => 'Skills',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'dnd_fields',
        'settings' => array(),
        'type' => 'dnd_fields_skill',
        'weight' => '30',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_skin'
  $fields['node-dnd_character-field_dnd_character_skin'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_skin',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '32',
      ),
      'translatable' => '1',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The color of the skin helps with physical character descriptions.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 24,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_skin',
      'label' => 'Skin Color',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '20',
        ),
        'type' => 'text_textfield',
        'weight' => '62',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_wealth'
  $fields['node-dnd_character-field_dnd_character_wealth'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_wealth',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 54,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_wealth',
      'label' => 'Wealth and valuables',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5',
        ),
        'type' => 'text_textarea',
        'weight' => '10',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_weapon_prof'
  $fields['node-dnd_character-field_dnd_character_weapon_prof'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '3',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_weapon_prof',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
      'translatable' => '1',
      'type' => 'text',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'List the weapon proficiencies this character has. This does not subtract from the proficiencies above (all proficiencies should be listed together above); this is only for quick notes only.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 51,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_weapon_prof',
      'label' => 'Weapon proficiencies',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '40',
        ),
        'type' => 'text_textfield',
        'weight' => '18',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_weapons'
  $fields['node-dnd_character-field_dnd_character_weapons'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_weapons',
      'foreign keys' => array(),
      'indexes' => array(
        'weapon' => array(
          0 => 'weapon',
        ),
      ),
      'module' => 'dnd_fields',
      'settings' => array(
        'allowed_values' => array(),
        'allowed_values_function' => '',
      ),
      'translatable' => '1',
      'type' => 'dnd_fields_weapon',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'dnd_fields',
          'settings' => array(),
          'type' => 'dnd_fields_weapon',
          'weight' => 50,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_weapons',
      'label' => 'Weapons',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'dnd_fields',
        'settings' => array(),
        'type' => 'dnd_fields_weapon',
        'weight' => '17',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_weight'
  $fields['node-dnd_character-field_dnd_character_weight'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_weight',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The weight of the player character in pounds.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 23,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_weight',
      'label' => 'Weight',
      'required' => 0,
      'settings' => array(
        'max' => '2047',
        'min' => '2',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '60',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_worn_equip'
  $fields['node-dnd_character-field_dnd_character_worn_equip'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_worn_equip',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 53,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_worn_equip',
      'label' => 'Worn equipment',
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '10',
        ),
        'type' => 'text_textarea',
        'weight' => '9',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_xp'
  $fields['node-dnd_character-field_dnd_character_xp'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_xp',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Experience points (XP) measure how much your character has learned and how much he or she has grown in personal power. Your character earns XP by defeating monsters and other opponents.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 13,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_xp',
      'label' => 'Experience Points',
      'required' => 1,
      'settings' => array(
        'max' => '',
        'min' => '0',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '25',
      ),
    ),
  );

  // Exported field: 'node-dnd_character-field_dnd_character_xp_penalty'
  $fields['node-dnd_character-field_dnd_character_xp_penalty'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_dnd_character_xp_penalty',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '1',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'dnd_character',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Some special races or classes incur an experience penalty, such as playing a Drow.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => ' ',
          ),
          'type' => 'number_integer',
          'weight' => 19,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_dnd_character_xp_penalty',
      'label' => 'Experience Penalty',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '0',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '26',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('A character who has not yet acted during a combat is flat-footed, not yet reacting normally to the situation. A flat-footed character loses his Dexterity bonus to AC (if any) and cannot make attacks of opportunity.');
  t('A creature\'s general moral and personal attitudes are represented by its alignment: lawful good, neutral good, chaotic good, lawful neutral, neutral, chaotic neutral, lawful evil, neutral evil, or chaotic evil.');
  t('A list of modifiers that affect the overall armor class.');
  t('Ability scores');
  t('Age');
  t('Alignment');
  t('Any armor heavier than leather hurts a character\'s ability to use some skills. An armor check penalty number is the penalty that applies to Balance, Climb, Escape Artist, Hide, Jump, Move Silently, Sleight of Hand, and Tumble checks by a character wearing a certain kind of armor. Double the normal armor check penalty is applied to Swim checks. A character\'s encumbrance (the amount of gear carried, including armor) may also apply an armor check penalty.');
  t('Appearance');
  t('Arcane Spell Failure');
  t('Armor');
  t('Armor Bonus');
  t('Armor Check Penalty');
  t('Armor Class Modifiers');
  t('Armor interferes with the gestures that you need to make to cast an arcane spell. This percentage indicates the level of possible failure.');
  t('Attack bonus modifiers');
  t('Background');
  t('Background information about the player character is one of the most vital pieces of information about her. Information about history and goals add an extra dimension to role playing, so flesh this out as much as possible.');
  t('Base attack');
  t('Class and level');
  t('Combat notes');
  t('Containers');
  t('Creatures smaller than Small or larger than Medium have special rules relating to position.');
  t('Deity');
  t('Describe the appearance of the player character to provide additional background information.');
  t('Describe the goals of the player character to provide additional background information.');
  t('Describe the history of the player character to provide additional background information.');
  t('Describe the personality of the player character to provide additional background information.');
  t('Each character has hit points (hp), representing how difficult he or she is to kill. At 1st level, wizards and sorcerers get 4 hp. Rogues and bards get 6 hp. Clerics, druids, and monks get 8 hp. Fighters, paladins, and rangers get 10 hp. Barbarians get 12 hp. To this number, add your character\'s Constitution modifier.');
  t('Equipment');
  t('Experience Penalty');
  t('Experience Points');
  t('Experience points (XP) measure how much your character has learned and how much he or she has grown in personal power. Your character earns XP by defeating monsters and other opponents.');
  t('Eye Color');
  t('Eye color helps create a physical description of the player character.');
  t('Feats');
  t('Flat Foot');
  t('Gender');
  t('Goals');
  t('Hair Color');
  t('Hair color helps create a player character\'s physical description.');
  t('Height');
  t('History');
  t('Hit Points');
  t('Include anything helpful to remember during combat, such as special abilities, magical effects, or tactical information.');
  t('Initiative');
  t('Initiative modifier');
  t('Initiative modifiers');
  t('Languages');
  t('Laterality');
  t('Laterality is the "handedness" of the player character; right-handed or left-handed. Ambidextrous is possible, but requires a Feat.');
  t('List any notes relevant to this character that come from race or class. Ex: Elves are immune to sleep and charm effects.');
  t('List the different items that affect saving throws, especially the miscellaneous modifiers.');
  t('List the equipment this PC carries. Each text field corresponds to one of the containers above.');
  t('List the languages spoken by this character.');
  t('List the two primary containers for your gear and equipment that the PC carries.');
  t('List the weapon proficiencies this character has. This does not subtract from the proficiencies above (all proficiencies should be listed together above); this is only for quick notes only.');
  t('Melee attack');
  t('Miscellaneous Modifers');
  t('Miscellaneous modifiers to Armor Class can come from blessings, curses, and magic items.');
  t('Notes');
  t('Personality/Behavior');
  t('Provide additional notes about the player character.');
  t('Race');
  t('Racial traits and class features');
  t('Ranged attack');
  t('Saving Throw Modifiers');
  t('Saving throws');
  t('Sheild Bonus');
  t('Size');
  t('Size Modifier');
  t('Skill modifiers');
  t('Skills');
  t('Skin Color');
  t('Some attacks disregard armor, including shields and natural armor. In these cases, the attacker makes a touch attack roll (either ranged or melee). When you are the target of a touch attack, your AC doesn\'t include any armor bonus, shield bonus, or natural armor bonus. All other modifiers, such as your size modifier, Dexterity modifier, and deflection bonus (if any) apply normally.');
  t('Some feats have prerequisites. Your character must have the indicated ability score, class feature, feat, skill, base attack bonus, or other quality designated in order to select or use that feat. A character can gain a feat at the same level at which he or she gains the prerequisite. A character can\'t use a feat if he or she has lost a prerequisite.');
  t('Some special races or classes incur an experience penalty, such as playing a Drow.');
  t('The age in years of the player character.');
  t('The bonus (or penalty) of the armor used. This only applies to armor currently equipped.');
  t('The bonus (or penalty) of the shield used. This only applies to a single shield currently equipped.');
  t('The color of the skin helps with physical character descriptions.');
  t('The height of the player character, in inches. For reference, 4 feet is 48 inches, 6 feet is 72 inches, and 8 feet is 96 inches.');
  t('The weight of the player character in pounds.');
  t('This applies to the adjustment to armor class based on size. The size modifiers are: Fine: +8, Diminutive: +4 , Tiny: +2, Small: +1 , Medium: 0, Large: −1, Huge: −2, Gargantuan: −4, Colossal: −8.');
  t('Vs. Touch');
  t('Wealth and valuables');
  t('Weapon proficiencies');
  t('Weapons');
  t('Weight');
  t('Worn equipment');

  return $fields;
}

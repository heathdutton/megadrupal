<?php
/**
 * @file
 * drush_recipes_entity.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function drush_recipes_entity_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'drush_recipes-drecipe-field_drecipe_file'
  $field_instances['drush_recipes-drecipe-field_drecipe_file'] = array(
    'bundle' => 'drecipe',
    'deleted' => 0,
    'description' => 'Upload a drecipe file you have created or built through the UI',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'drush_recipes',
    'fences_wrapper' => 'no_wrapper',
    'field_name' => 'field_drecipe_file',
    'label' => 'drecipe file',
    'required' => 1,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => 'recipes/drecipe_uploads',
      'file_extensions' => 'drecipe',
      'max_filesize' => '50KB',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'drush_recipes-recipe-field_description'
  $field_instances['drush_recipes-recipe-field_description'] = array(
    'bundle' => 'recipe',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'drush_recipes_xml_output' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'drush_recipes',
    'field_name' => 'field_description',
    'label' => 'Description',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 3,
      ),
      'type' => 'text_textarea',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'drush_recipes-recipe-field_drush_recipes_api'
  $field_instances['drush_recipes-recipe-field_drush_recipes_api'] = array(
    'bundle' => 'recipe',
    'default_value' => array(
      0 => array(
        'value' => '1.0',
      ),
    ),
    'deleted' => 0,
    'description' => 'Which version of the API is this recipe written against',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 2,
          'thousand_separator' => ' ',
        ),
        'type' => 'number_decimal',
        'weight' => 0,
      ),
      'drush_recipes_xml_output' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 2,
          'thousand_separator' => ' ',
        ),
        'type' => 'number_decimal',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
    ),
    'entity_type' => 'drush_recipes',
    'field_name' => 'field_drush_recipes_api',
    'label' => 'Drush recipes API',
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
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'drush_recipes-recipe-field_ingredients'
  $field_instances['drush_recipes-recipe-field_ingredients'] = array(
    'bundle' => 'recipe',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'field_collection',
        'settings' => array(
          'add' => 'Add',
          'delete' => 'Delete',
          'description' => TRUE,
          'edit' => 'Edit',
          'view_mode' => 'full',
        ),
        'type' => 'field_collection_view',
        'weight' => 2,
      ),
      'drush_recipes_xml_output' => array(
        'label' => 'hidden',
        'module' => 'field_collection',
        'settings' => array(
          'view_mode' => 'drush_recipes_xml_output',
        ),
        'type' => 'field_collection_fields',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'field_collection',
        'settings' => array(
          'view_mode' => 'full',
        ),
        'type' => 'field_collection_fields',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'drush_recipes',
    'field_name' => 'field_ingredients',
    'label' => 'Ingredients',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'field_collection_table',
      'settings' => array(),
      'type' => 'field_collection_table',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'drush_recipes-recipe-field_machine_name'
  $field_instances['drush_recipes-recipe-field_machine_name'] = array(
    'bundle' => 'recipe',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'The machine name for this recipe',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'machine_name',
        'settings' => array(),
        'type' => 'machine_name_default',
        'weight' => 5,
      ),
      'drush_recipes_xml_output' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'drush_recipes',
    'fences_wrapper' => 'no_wrapper',
    'field_name' => 'field_machine_name',
    'label' => 'machine name',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'machine_name',
      'settings' => array(
        'size' => 128,
      ),
      'type' => 'machine_name_default',
      'weight' => 0,
    ),
  );

  // Exported field_instance: 'drush_recipes-recipe-field_type'
  $field_instances['drush_recipes-recipe-field_type'] = array(
    'bundle' => 'recipe',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Describe this recipe',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 2,
      ),
      'drush_recipes_xml_output' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'drush_recipes',
    'fences_wrapper' => 'div',
    'field_name' => 'field_type',
    'label' => 'Type',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'drush_recipes-recipe-field_version'
  $field_instances['drush_recipes-recipe-field_version'] = array(
    'bundle' => 'recipe',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 4,
      ),
      'drush_recipes_xml_output' => array(
        'label' => 'inline',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'drush_recipes',
    'field_name' => 'field_version',
    'label' => 'Version',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 10,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'drush_recipes-recipe-field_weight'
  $field_instances['drush_recipes-recipe-field_weight'] = array(
    'bundle' => 'recipe',
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'deleted' => 0,
    'description' => 'The weight of this recipe relative to others if they are all told to execute at once. Default is 0.',
    'display' => array(
      'default' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => ' ',
        ),
        'type' => 'number_integer',
        'weight' => 1,
      ),
      'drush_recipes_xml_output' => array(
        'label' => 'inline',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => ' ',
        ),
        'type' => 'number_integer',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'drush_recipes',
    'field_name' => 'field_weight',
    'label' => 'Weight',
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
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'field_collection_item-field_ingredients-field_item'
  $field_instances['field_collection_item-field_ingredients-field_item'] = array(
    'bundle' => 'field_ingredients',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_plain',
        'weight' => 0,
      ),
      'drush_recipes_xml_output' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_plain',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'field_collection_item',
    'fences_wrapper' => 'no_wrapper',
    'field_name' => 'field_item',
    'label' => 'Item',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Describe this recipe');
  t('Description');
  t('Drush recipes API');
  t('Ingredients');
  t('Item');
  t('The machine name for this recipe');
  t('The weight of this recipe relative to others if they are all told to execute at once. Default is 0.');
  t('Type');
  t('Upload a drecipe file you have created or built through the UI');
  t('Version');
  t('Weight');
  t('Which version of the API is this recipe written against');
  t('drecipe file');
  t('machine name');

  return $field_instances;
}

<?php

/*
 * Returns an associative array where key is a Parser class and value
 * is an array of processors that will handle the parsed pieces of content.
 *
 * @return array
 */
function hook_smartling_data_processor_info() {
  return array(
    'Drupal\\smartling\\Alters\\SmartlingContentImageUrlParser' => array('Drupal\\smartling_demo_content\\Alters\\SmartlingContentImageUrlProcessor'),
    'Drupal\\smartling_demo_content\\Alters\\SmartlingContentMediaParser' => array('Drupal\\smartling_demo_content\\Alters\\SmartlingContentMediaProcessor'),
    'Drupal\\smartling_demo_content\\Alters\\SmartlingContentMediaEncodedParser' => array('Drupal\\smartling_demo_content\\Alters\\SmartlingContentMediaEncodedProcessor'),
  );
}

/*
 * Allows a module to change the Smartling Parser => Processor settings
 *
 * @param $map - a map of parser -> processor pairs.
 *    see hook_smartling_data_processor_info() for more info.
 */
function hook_smartling_data_processor_info_alter(&$map) {
  unset($map['Drupal\\smartling\\Alters\\SmartlingContentImageUrlParser']);
}

/*
 * Allows to add/change class-handlers for Field Processors.
 *
 * See smartling.services.yml file for default parameters that will be passed into the function.
 *
 * @param $map - an associated key-value array of Field Processors for different field types.
 */
function hook_smartling_field_processor_mapping_info_alter(&$field_processor_map) {
  //a default value that is stored in smartling.services.yml
  $field_processor_map = array(
    'real' => array(
      'text' => 'Drupal\smartling\FieldProcessors\TextFieldProcessor',
      'text_long' => 'Drupal\smartling\FieldProcessors\TextFieldProcessor',
      'text_with_summary' => 'Drupal\smartling\FieldProcessors\TextSummaryFieldProcessor',
      'image' => 'Drupal\smartling\FieldProcessors\ImageFieldProcessor',
      'field_collection' => 'Drupal\smartling\FieldProcessors\FieldCollectionFieldProcessor'
    ),
    'fake' => array(
      'title_property' => 'Drupal\smartling\FieldProcessors\TitlePropertyFieldProcessor',
      'title_property_field' => 'Drupal\smartling\FieldProcessors\TitlePropertyFieldProcessor',
      'description_property_field' => 'Drupal\smartling\FieldProcessors\DescriptionPropertyFieldProcessor',
      'name_property_field' => 'Drupal\smartling\FieldProcessors\NamePropertyFieldProcessor'
    ),
  );
}

/*
 * Allows to add/change class-handlers for Entity Processors.
 *
 * See smartling.services.yml file for default parameters that will be passed into the function.
 *
 * @param $map - an associated key-value array of Field Processors for different field types.
 */
function hook_smartling_entity_processor_mapping_info_alter(&$entity_processor_map) {
  //a default value that is stored in smartling.services.yml
  $entity_processor_map = array(
    'node' => 'Drupal\smartling\Processors\NodeProcessor',
    'taxonomy_term' => 'Drupal\smartling\Processors\TaxonomyTermProcessor',
    'generic' => 'Drupal\smartling\Processors\GenericEntityProcessor'
  );
}

/**
 * Allows other modules to inject their forms into smartling settings page
 * on a separate tab. The "key" is a machine name of a form and "value" is a human readable title.
 *
 * @return array
 */
function hook_smartling_settings_form_info() {
  $forms = array(
    'smartling_admin_account_info_settings_form' => array(
      'weight' => 0,
      'title' => t('Account info')
    ),
    'smartling_admin_node_translation_settings_form' => array(
      'weight' => 0,
      'title' => t('Nodes settings')
    ),
    'smartling_admin_entities_translation_settings_form' => array(
      'weight' => 0,
      'title' => t('Entities settings')
    ),
  );

  if (module_exists('taxonomy')) {
    $forms['smartling_admin_taxonomy_translation_settings_form'] = array(
      'weight' => 0,
      'title' => t('Taxonomy vocabularies and fields')
    );
  }

  return $forms;
}
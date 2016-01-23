<?php
/**
 * @file
 * white_wolf_character_sheets.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function white_wolf_character_sheets_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function white_wolf_character_sheets_node_info() {
  $items = array(
    'changeling_character_sheet' => array(
      'name' => t('Changeling character sheet'),
      'base' => 'node_content',
      'description' => t('A changeling character sheet'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'hunter_character_sheet' => array(
      'name' => t('Hunter character sheet'),
      'base' => 'node_content',
      'description' => t('A hunter character sheet'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'mage_character_sheet' => array(
      'name' => t('Mage character sheet'),
      'base' => 'node_content',
      'description' => t('A mage character sheet'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'vampire_character_sheet' => array(
      'name' => t('Vampire character sheet'),
      'base' => 'node_content',
      'description' => t('A vampire character sheet.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'werewolf_character_sheet' => array(
      'name' => t('Werewolf character sheet'),
      'base' => 'node_content',
      'description' => t('A werewolf character sheet'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}

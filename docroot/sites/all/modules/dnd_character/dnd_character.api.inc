<?php

/*
  D&D Character
  - abilities (field, cardinality: 6)
  - saves (field, variable cardinality)
  - race (machine name)
  - classes/levels (array of classes and level values, keyed by class machine name)
*/


/**
 * Helper functions
 */


/**
 * Helper function to return race data.
 * @todo: Get race data from database rather than default values.
 */
function dnd_character_get_race_data($machine_name = NULL) {
  module_load_include('inc', 'dnd_character', 'dnd_character.races');
  $races = dnd_character_race_defaults();

  if (!empty($machine_name)) {
    return $races[$machine_name];
  }

  return $races;
}


function dnd_character_node_skills($node) {
  module_load_include('inc', 'dnd_character', 'dnd_character.skills');
  $skills = dnd_character_skill_data();

  $character_classes = array();
  foreach ($node->field_dnd_character_class_level[$node->language] as $character_class) {
    $character_classes[$character_class['class']] = array('machine_name' => $character_class['class']);
  }

  $character_skills = array();
  foreach ($node->field_dnd_character_skills[$node->language] as $character_skill) {
    $character_skills[$character_skill['skill']] = array(
      'machine_name' => $character_skill['skill'],
      'ranks' => $character_skill['ranks'],
    );
  }

  foreach ($skills as $skill_machine_name => $skill) {
    if (!empty($character_skills[$skill_machine_name]) || !($skill['trained'])) {
      // Establish that we have a skill machine name and purchased ranks
      if (empty($character_skills[$skill_machine_name]['machine_name'])) $character_skills[$skill_machine_name] = array('machine_name' => $skill_machine_name);
      if (empty($character_skills[$skill_machine_name]['ranks'])) $character_skills[$skill_machine_name]['ranks'] = 0;

      // Add the default skill data from the database to our collection for "this" node
      $character_skills[$skill_machine_name] += $skill;

      // Determine the base points based on class skill and purchased ranks
      $class_intersections = array_intersect($character_skills[$skill_machine_name]['classes'], array_keys($character_classes));
      if (empty($class_intersections)) { // cross-class skill, so divide ranks by 2
        $character_skills[$skill_machine_name]['base_points'] = (int) floor($character_skills[$skill_machine_name]['ranks'] / 2);
      }
      else {
        $character_skills[$skill_machine_name]['base_points'] = $character_skills[$skill_machine_name]['ranks'];
      }

      // Ability modifier and points
      $character_skills[$skill_machine_name]['ability_modifier'] = $node->field_dnd_character_abilities[$node->language][$character_skills[$skill_machine_name]['ability_key']]['mod'];
      $character_skills[$skill_machine_name]['points'] = $character_skills[$skill_machine_name]['base_points'] + $character_skills[$skill_machine_name]['ability_modifier'];
    }
  }

  return $character_skills;
}

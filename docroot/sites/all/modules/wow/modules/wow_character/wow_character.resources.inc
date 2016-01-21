<?php

/**
 * @file
 * Service related strings that needs to be translated at runtime.
 */

t('head');
t('neck');
t('shoulder');
t('back');
t('chest');
t('wrist');
t('hands');
t('waist');
t('legs');
t('feet');
t('finger1');
t('finger2');
t('trinket1');
t('trinket2');
t('mainHand');
t('offHand');
t('ranged');

/**
 * Implements hook_wow_data_resources().
 */
function wow_character_wow_data_resources() {
  return array(
    'wow_character' => array(
      'races' => array(
        'name' => t('Character Races'),
        'label' => t("Character race's name."),
        'description' => t('The character races data API provides a list of character races.'),
        'remote path' => 'character/races',
        'entity type' => 'wow_character_race',
      ),
      'classes' => array(
        'name' => t('Character Classes'),
        'label' => t("Character class's name."),
        'description' => t('The character classes data API provides a list of character classes.'),
        'remote path' => 'character/classes',
        'entity type' => 'wow_character_class',
      ),
    ),
  );
}

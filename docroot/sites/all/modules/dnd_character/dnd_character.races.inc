<?php

function dnd_character_race_defaults () {
  $races = array(
    'human' = array(
      'machine_name' => 'human',
      'name' => t('Human'),
      'plural' => t('Humans'),
      'ability_mods' => array(),
      'speed' => 30,
      'size' => 'm',
      'racial_traits' => array(
        t('Medium size.'),
        t('Human base speed is 30 feet.'),
      ),
    ),
    'elf' = array(
      'machine_name' => 'elf',
      'name' => t('Elf'),
      'plural' => t('Elves'),
      'ability_mods' => array('dex' => +2, 'con' => -2),
      'speed' => 30,
      'size' => 'm',
      'racial_traits' => array(
        t('+2 Dexterity, -2 Constitution.'),
        t('Medium size.'),
        t('Elven base speed is 30 feet.'),
        t('Immunity to magic sleep spells and effects.'),
        t('+2 racial saving throw bonus against Enchantment spells or effects.'),
        t('Low-light Vision: Elves can see twice as far as a human in starlight, moonlight, torchlight, and similar conditions of poor illumination. They retain the ability to distinguish color and detail under these conditions.'),
        t('Proficient with either longsword or rapier; proficient with shortbow, longbow, composite longbow, and composite shortbow.'),
        t('+2 racial bonus on Listen, Search, and Spot checks. An elf who merely passes within 5 feet of a secret or concealed door is entitled to a Search check to notice it as if she were actively looking for the door.'),
        t('Automatic Languages: Common and Elven.'),
        t('Bonus Languages: Draconic, Gnoll, Gnome, Goblin, Orc, and Sylvan.'),
        t('Favored Class: Wizard.'),
      ),
    ),
    'dwarf' = array(
      'machine_name' => 'dwarf',
      'name' => t('Dwarf'),
      'plural' => t('Dwarves'),
      'ability_mods' => array('con' => +2, 'chr' => -2),
      'speed' => 20,
      'size' => 'm',
      'racial_traits' => array(
        t('Medium size.'),
        t('Dwarven base speed is 20 feet.'),
      ),
    ),
  );

  return $races;
}

<?php
/**
 * @file
 * 
 * This file documents hooks invoked by the Petfinder module
 */

/**
 * hook_petfinder_pet_alter
 * 
 * Alter hook invoked after a pet record has been loaded from Petfinder via 
 * petfinder_pet_load(), but before it has been returned to the calling 
 * function.  This gives the ability to inject or modify the information 
 * returned.
 * 
 * @param &$pet
 *   Pet data loaded, record is located under the array element 'pet' to 
 *   minimize name collisions with any theme callbacks
 */
function hook_petfinder_pet_alter(&$pet) {
  $pet['pet']['more_info'] = t('This is some more info added to the pet record');
  $pet['#more_theme_info'] = t('This is some more info added that will get passed through to the theme callbacks');
}

/**
 * hook_petfinder_pet_load
 * 
 * Hook invoked after a pet record has been loaded from Petfinder via 
 * petfinder_pet_load(), after hook_petfinder_pet_alter() has been invoked.  
 * Allows modules to trigger off a load event without altering the pet record.
 * 
 * @param $pet
 *   Pet data loaded, record is located under the array element 'pet' to 
 *   minimize name collisions with any theme callbacks
 */
function hook_petfinder_pet_load($pet) {
  watchdog('petfinder', 'Pet record loaded: @id', array('@id' => $pet['pet']['id']));
}

/**
 * hook_petfinder_pets_alter
 * 
 * Alter hook invoked after a pet search list has been loaded from Petfinder via 
 * petfinder_pet_search(), but before it has been returned to the calling 
 * function.  This gives the ability to inject or modify the information 
 * returned.
 * 
 * @param &$pets
 *   Pet data loaded, list is located under the array element 'pets' to 
 *   minimize name collisions with any theme callbacks
 */
function hook_petfinder_pets_alter(&$pets) {
  // Invoke our record callback on each record
  foreach ($pets['pets'] as &$pet) {
    hook_petfinder_pet_alter($shelter);
  }
}

/**
 * hook_petfinder_pet_search
 * 
 * Hook invoked after a pet search list has been loaded from Petfinder via 
 * petfinder_pet_search(), after hook_petfinder_pets_alter() has been invoked.  
 * Allows modules to trigger off a load event without altering the pet list.
 * 
 * @param $pets
 *   Pet data loaded, list is located under the array element 'pets' to 
 *   minimize name collisions with any theme callbacks
 */
function hook_petfinder_pet_search($pets) {
  // Invoke our record callback on each record
  foreach ($pets['pets'] as $pet) {
    hook_petfinder_pet_load($pet);
  }
}



/**
 * hook_petfinder_shelter_alter
 * 
 * Alter hook invoked after a shelter record has been loaded from Petfinder via 
 * petfinder_shelter_load(), but before it has been returned to the calling 
 * function.  This gives the ability to inject or modify the information 
 * returned.
 * 
 * @param &$shelter
 *   Shelter data loaded, record is located under the array element 'shelter' to 
 *   minimize name collisions with any theme callbacks
 */
function hook_petfinder_shelter_alter(&$shelter) {
  $shelter['shelter']['more_info'] = t('This is some more info added to the shelter record');
  $shelter['#more_theme_info'] = t('This is some more info added that will get passed through to the theme callbacks');
}

/**
 * hook_petfinder_shelter_load
 * 
 * Hook invoked after a shelter record has been loaded from Petfinder via 
 * petfinder_shelter_load(), after hook_petfinder_shelter_alter() has been invoked.  
 * Allows modules to trigger off a load event without altering the shelter record.
 * 
 * @param $shelter
 *   Shelter data loaded, record is located under the array element 'shelter' to 
 *   minimize name collisions with any theme callbacks
 */
function hook_petfinder_shelter_load($shelter) {
  watchdog('petfinder', 'Shelter record loaded: @id', array('@id' => $shelter['shelter']['id']));
}

/**
 * hook_petfinder_shelters_alter
 * 
 * Alter hook invoked after a shelter search list has been loaded from Petfinder via 
 * petfinder_shelter_search(), but before it has been returned to the calling 
 * function.  This gives the ability to inject or modify the information 
 * returned.
 * 
 * @param &$shelters
 *   Shelter data loaded, list is located under the array element 'shelters' to 
 *   minimize name collisions with any theme callbacks
 */
function hook_petfinder_shelters_alter(&$shelters) {
  // Invoke our record callback on each record
  foreach ($shelters['shelters'] as &$shelter) {
    hook_petfinder_shelter_alter($shelter);
  }
}

/**
 * hook_petfinder_shelter_search
 * 
 * Hook invoked after a shelter search list has been loaded from Petfinder via 
 * petfinder_shelter_search(), after hook_petfinder_shelters_alter() has been invoked.  
 * Allows modules to trigger off a load event without altering the shelter list.
 * 
 * @param $shelters
 *   Shelter data loaded, list is located under the array element 'shelters' to 
 *   minimize name collisions with any theme callbacks
 */
function hook_petfinder_shelter_search($shelters) {
  // Invoke our record callback on each record
  foreach ($shelters['shelters'] as $shelter) {
    hook_petfinder_shelter_load($shelter);
  }
}

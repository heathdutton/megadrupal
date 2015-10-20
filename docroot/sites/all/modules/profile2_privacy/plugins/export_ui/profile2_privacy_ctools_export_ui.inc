<?php
/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'profile2_privacy_levels',  // As defined in hook_schema().
  'access' => 'administer profile2 privacy',  // Define a permission users must have to access these pages.
  // Define the menu item.
  'menu' => array(
    'menu item' => 'profile2_privacy',
    'menu title' => 'Profile2 Privacy',
    'menu description' => 'Administer Profile2 Privacy levels.',
  ),
  // Define user interface texts.
  'title singular' => t('privacy level'),
  'title plural' => t('privacy levels'),
  'title singular proper' => t('Profile2 privacy level'),
  'title plural proper' => t('Profile2 privacy levels'),
  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'profile2_privacy_level_form',
    'submit' => 'profile2_privacy_level_form_submit',
    // 'submit' and 'validate' are also valid callbacks.
  ),
);

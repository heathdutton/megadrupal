<?php
/**
 * @file
 * Module settings.
 */

/**
 * Menu callback. Displays the administration settings.
 */
function github_admin_settings() {
  $form = array();

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 50,
  );

  $form['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Your Details'),
    '#group' => 'settings',
  );

  $form['details']['github_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Your Github username'),
    '#description' => t('Your GitHub username - required to determine the link to your profile'),
    '#default_value' => variable_get('github_username', ''),
    '#required' => TRUE,
  );

  $form['appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance'),
    '#group' => 'settings',
  );

  $form['appearance']['github_placement'] = array(
    '#type' => 'select',
    '#options' => array('left' => t('Left'), 'right' => t('Right')),
    '#title' => t('Placement'),
    '#description' => t('Which side should the ribbon be placed on'),
    '#default_value' => variable_get('github_placement', 'right'),
    '#required' => TRUE,
  );

  $form['appearance']['github_colour'] = array(
    '#type' => 'select',
    '#options' => array(
      'red' => t('Red'),
      'green' => t('Green'),
      'darkblue' => t('Dark Blue'),
      'orange' => t('Orange'),
      'grey' => t('Grey'),
      'white' => t('White'),
    ),
    '#title' => t('Colour'),
    '#description' => t('Which color should the ribbon have'),
    '#default_value' => variable_get('github_colour', 'red'),
    '#required' => TRUE,
  );

  $theme_regions = system_region_list('lndesign_theme', 'REGIONS_ALL');
  $system_regions = array(
    'page_top' => 'Page top',
    'header' => 'Header',
    'sidebar_first' => 'Sidebar First',
    'sidebar_second' => 'Sidebar Second',
    'page_bottom' => 'Page bottom',
  );

  // Merge arrays and remove dublicates
  $region_array = array_unique(array_merge($system_regions, $theme_regions));
  
  // Find content position in array
  $content_pos = array_search('Content', $region_array);
  // Remove content from array
  if ($content_pos) {
    unset($region_array[$content_pos]);
  }

  $form['appearance']['github_page_block'] = array(
    '#type' => 'select',
    '#options' => $region_array,
    '#title' => t('Block placement'),
    '#description' => t('Which region should the ribbon be placed in'),
    '#default_value' => variable_get('github_page_block', 'page_top'),
  );

  return system_settings_form($form);
}

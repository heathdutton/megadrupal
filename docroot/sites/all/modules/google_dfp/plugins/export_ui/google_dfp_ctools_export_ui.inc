<?php

/**
 * @file
 * Define Google DFP Ad Export UI plugin.
 */

$plugin = array(
  'schema' => 'google_dfp',
  'access' => 'administer google_dfp',
  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/structure/google-dfp',
    'menu item' => 'google_dfp',
    'menu title' => 'Google DFP Ads',
    'menu description' => 'Administer Google DFP ad presets.',
  ),
  // Define user interface texts.
  'title singular' => t('ad'),
  'title plural' => t('ads'),
  'title singular proper' => t('Ad preset'),
  'title plural proper' => t('Ad presets'),
  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'google_dfp_ctools_export_ui_form',
    'submit' => 'google_dfp_ctools_export_ui_form_submit',
    'validate' => 'google_dfp_ctools_export_ui_form_validate',
  ),
);

/**
 * Form builder for ad preset.
 */
function google_dfp_ctools_export_ui_form(&$form, $form_state) {
  $ad = $form_state['item'];
  $form['#theme'] = 'google_dfp_form';
  $form['#attached']['js'][] = drupal_get_path('module', 'google_dfp') . '/js/google_dfp.admin.js';
  $form['#attached']['library'][] = array('system', 'ui.tabs');
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t('The human readable name of this ad.'),
    '#default_value' => $ad->name,
    '#required' => TRUE,
  );
  $form['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#description' => t('The width of this ad.'),
    '#default_value' => $ad->width,
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
  );
  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#description' => t('The height of this ad.'),
    '#default_value' => $ad->height,
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
  );
  $form['premium'] = array(
    '#type' => 'checkbox',
    '#title' => t('Premium'),
    '#description' => t('Leave unchecked if you use DFP Small Business.<br /> For premium accounts tiers correlate to ad unit hierarchies (parent/child). <br />For Small Business each tier combination is a single unit.'),
    '#default_value' => $ad->premium,
  );
  $form['screen'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Screen targets'),
    '#description' => t('Select the breakpoints to target'),
    '#default_value' => $ad->screen,
    '#options' => array('_all' => 'all'),
    '#access' => module_exists('breakpoints'),
  );
  if (module_exists('breakpoints')) {
    // Load the breakpoints list.
    $breakpoints = array('_all' => 'all');
    $enabled_breakpoints = drupal_map_assoc(array_keys(breakpoints_breakpoint_load_all_active(variable_get('theme_default', ''))));
    $breakpoint_options = $breakpoints + $enabled_breakpoints;
    $form['screen']['#options'] = $breakpoint_options;
  }
  $form['position'] = array(
    '#type' => 'select',
    '#title' => t('Position'),
    '#description' => t('Select a position'),
    '#default_value' => $ad->position,
    '#options' => drupal_map_assoc(range(0, 10)),
  );

  $form['enabled_tiers'] = array(
    '#type' => 'item',
    '#title' => t('Enabled tiers'),
    '#prefix' => '<div id="tiers-status-wrapper">',
    '#suffix' => '</div>',
  );
  $form['enabled_tiers']['order'] = array(
    '#type' => 'item',
    '#title' => t('Tier processing order'),
    '#theme' => 'google_dfp_plugin_order',
    '#weight' => 100,
  );
  $form['tiers_title'] = array(
    '#type' => 'item',
    '#title' => t('Tier settings'),
  );

  $form['tiers'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
    '#tree' => TRUE,
    '#parents' => array('tiers'),
    '#title' => t('Tiers'),
  );

  foreach ($ad->getTierInstances() as $tier) {
    // Settings.
    $form['tiers'][$tier->getId()] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($tier->getTitle()),
      '#collapsible' => TRUE,
      '#group' => 'additional_settings',
      '#attributes' => array(
        'id' => 'edit-tiers-' . drupal_clean_css_identifier($tier->getId()) . '-settings',
      ),
    );
    $form['tiers'][$tier->getId()] += $tier->settingsForm($form, $form_state);
    // Enabled and weight.
    $form['enabled_tiers'][$tier->getId()] = array(
      '#type' => 'checkbox',
      '#title' => $tier->getTitle(),
      '#parents' => array('tiers', $tier->getId(), 'enabled'),
      '#default_value' => isset($ad->tiers[$tier->getId()]),
    );
    $form['enabled_tiers']['order'][$tier->getId()]['title'] = array(
      '#markup' => $tier->getTitle(),
    );
    $form['enabled_tiers']['order'][$tier->getId()]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $tier->getTitle())),
      '#title_display' => 'invisible',
      '#delta' => 50,
      '#default_value' => $tier->getConfiguration('weight'),
      '#parents' => array('tiers', $tier->getId(), 'weight'),
    );
    $form['enabled_tiers']['order'][$tier->getId()]['#weight'] = $tier->getConfiguration('weight');
  }

  $form['enabled_keywords'] = array(
    '#type' => 'item',
    '#title' => t('Enabled keywords'),
    '#prefix' => '<div id="keywords-status-wrapper">',
    '#suffix' => '</div>',
  );
  $form['enabled_keywords']['order'] = array(
    '#type' => 'item',
    '#title' => t('Keyword processing order'),
    '#theme' => 'google_dfp_plugin_order',
    '#weight' => 100,
  );

  $form['keywords_title'] = array(
    '#type' => 'item',
    '#title' => t('Keyword settings'),
  );
  $form['keywords'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 99,
    '#tree' => TRUE,
    '#title' => t('Keywords'),
  );

  foreach ($ad->getKeywordInstances() as $keyword) {
    // Settings.
    $form['keywords'][$keyword->getId()] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($keyword->getTitle()),
      '#collapsible' => TRUE,
      '#group' => 'additional_settings',
      '#attributes' => array(
        'id' => 'edit-tiers-' . drupal_clean_css_identifier($tier->getId()) . '-settings',
      ),
    );
    $form['keywords'][$keyword->getId()] += $keyword->settingsForm($form, $form_state);
    // Enabled and weight.
    $form['enabled_keywords'][$keyword->getId()] = array(
      '#type' => 'checkbox',
      '#title' => $keyword->getTitle(),
      '#parents' => array('keywords', $keyword->getId(), 'enabled'),
      '#default_value' => isset($ad->keywords[$keyword->getId()]),
    );
    $form['enabled_keywords']['order'][$keyword->getId()]['title'] = array(
      '#markup' => $keyword->getTitle(),
    );
    $form['enabled_keywords']['order'][$keyword->getId()]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $keyword->getTitle())),
      '#title_display' => 'invisible',
      '#delta' => 50,
      '#default_value' => $keyword->getConfiguration('weight'),
      '#parents' => array('keywords', $keyword->getId(), 'weight'),
    );
    $form['enabled_keywords']['order'][$keyword->getId()]['#weight'] = $keyword->getConfiguration('weight');
  }
  return $form;
}

/**
 * Validates the configuration form.
 */
function google_dfp_ctools_export_ui_form_validate($form, &$form_state) {

}

/**
 * Form submission handler for configuraiton form.
 */
function google_dfp_ctools_export_ui_form_submit($form, &$form_state) {
  $ad = $form_state['item'];
  $basic_values = array(
    'placement',
    'name',
    'height',
    'width',
    'position',
    'screen',
  );
  foreach ($basic_values as $key) {
    $ad->{$key} = $form_state['values'][$key];
  }
  $tier_plugins = ctools_get_plugins('google_dfp', 'GoogleDfp\Tier');
  $tiers = array();
  foreach ($tier_plugins as $plugin_id => $plugin) {
    $class = $plugin['class'];
    if (!empty($form_state['values']['tiers'][$plugin_id]['enabled'])) {
      $config = $form_state['values']['tiers'][$plugin_id];
      unset($config['enabled']);
      $instance = $class::createInstance($plugin_id, $config);
      $instance->settingsFormSubmit($form, $form_state);
      $tiers[$plugin_id] = $instance->getConfiguration();
    }
  }
  $ad->tiers = $tiers;
  $keyword_plugins = ctools_get_plugins('google_dfp', 'GoogleDfp\Keyword');
  $keywords = array();
  foreach ($keyword_plugins as $plugin_id => $plugin) {
    $class = $plugin['class'];
    if (!empty($form_state['values']['keywords'][$plugin_id]['enabled'])) {
      $config = $form_state['values']['keywords'][$plugin_id];
      unset($config['enabled']);
      $instance = $class::createInstance($plugin_id, $config);
      $keywords[$plugin_id] = $instance->getConfiguration();
    }
  }
  $ad->$keywords = $keywords;
  // Mark the tiers and keywords as processed.
  unset($form_state['values']['tiers'], $form_state['values']['keywords']);
  $form_state['item'] = $ad;
}

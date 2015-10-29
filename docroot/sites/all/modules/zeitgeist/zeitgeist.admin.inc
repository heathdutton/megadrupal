<?php

/**
 * @file
 * Admin pages for the Zeitgeist module.
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

use Drupal\zeitgeist\Block;
use Drupal\zeitgeist\Cache;
use Drupal\zeitgeist\ListBlock;
use Drupal\zeitgeist\Report;
use Drupal\zeitgeist\Sanitization;
use Drupal\zeitgeist\Statistics;

/**
 * Implement a settings form.
 *
 * @return array
 *   An array containing form items to place on the module settings page.
 */
function _zeitgeist_admin_settings($form) {
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'zeitgeist') . '/zeitgeist.js' => array('type' => 'file'),
  );

  // Statistics settings sub-form.
  $form[Statistics::VARHISTORY] = Statistics::settings();

  $form['zeitgeist-display'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Display settings'),
    '#collapsible'   => TRUE,
  );

  // Block settings sub-forms.
  foreach (array_keys(zeitgeist_block_info()) as $delta) {
    $form['zeitgeist-display']["zeitgeist-$delta"] = Block::instance($delta)->settings();
  }

  // Report settings sub-form.
  $form['zeitgeist-display']['zeitgeist-page'] = Report::instance()->settings();

  // Sanitization/filtering settings sub-form.
  $form['zeigeist-filtering'] = Sanitization::settings();

  // Advanced settings sub-form.
  $form['advanced'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Advanced settings'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
  );

  $form['advanced'][ListBlock::VARNOFOLLOW] = ListBlock::settingsAdvanced();

  $modules = system_rebuild_module_data();
  $name = 'zeitgeist';
  $info = $modules[$name];
  $form['advanced']['version'] = array(
    '#markup' => '<p>' . t('This site is running %module version [%version], schema version: [%schema].', array(
      '%module' => $name,
      '%version' => isset($info->info['version']) ? $info->info['version'] : t('Undefined'),
      '%schema' => $info->schema_version,
    )) . '</p>',
  );

  // Handlers cannot be 'Foo::bar' or array('Foo', 'bar'), but can be closures.
  // @see form_execute_handlers()
  $form['#submit'][] = function () {
    Cache::clearAll();
  };

  $form = system_settings_form($form);
  return $form;
}

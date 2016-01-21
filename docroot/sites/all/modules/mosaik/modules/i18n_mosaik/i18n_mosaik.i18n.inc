<?php
/**
 * @file
 * Internationalization (i18n) hooks
 */

/**
 * Implements hook_i18n_object_info().
 */
function i18n_mosaik_i18n_object_info() {
  $info['mosaik'] = array(
    'title' => t('Mosaik'),
    'class' => 'i18n_mosaik',
    'key' => 'name',
    'placeholders' => array(
      '%mosaik' => 'name',
    ),
    'edit path' => MOSAIK_ADMIN_MENU . '/%mosaik/edit',
    'string translation' => array(
      'textgroup' => 'mosaik',
      'type' => 'type',
      'properties' => array(
        'name' => t('Name'),
        'title' => t('Title'),
      ),
    ),
  );

  return $info;
}

/**
 * Implements hook_i18n_translation_set_info().
 */
function i18n_mosaik_i18n_translation_set_info() {
  $info['mosaik'] = array(
    'title' => t('Mosaik'),
    // The class that handles this translation sets
    'class' => 'i18n_mosaik_translation_set',
    // Table and field into that table that keeps the translation set id for each item.
    'table' => 'mosaik',
    'field' => 'i18n_tsid',
    // Placeholders and path information for generating translation set pages for administration.
    'placeholder' => '%i18n_mosaik_translation_set',
    'list path' => MOSAIK_ADMIN_MENU . '/sets',
    'edit path' => MOSAIK_ADMIN_MENU . '/sets/edit/%i18n_mosaik_translation_set',
    'delete path' => MOSAIK_ADMIN_MENU . '/sets/delete/%i18n_mosaik_translation_set',
    'page callback' => 'i18n_mosaik_translation_page',
  );
  return $info;
}

/**
 * Callback for mosaik translation tab.
 *
 * @param $type
 *   Should be always 'mosaik'.
 * @pram $mosaik
 *   Mosaik array.
 */
function i18n_mosaik_translation_page($type, $mosaik) {
  module_load_include('inc', 'i18n_mosaik', 'includes/i18n_mosaik.admin');
  $translation_set = !empty($mosaik['i18n_tsid']) ? i18n_translation_set_load($mosaik['i18n_tsid']) : NULL;
  $translation_overview = i18n_mosaik_translation_mosaik_overview($mosaik);
  $translation_mosaik_form = drupal_get_form('i18n_mosaik_translation_form', $translation_set, $mosaik);

  return $translation_overview + $translation_mosaik_form;
}

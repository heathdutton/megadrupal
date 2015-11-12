<?php
/**
 * @file
 * Features file for the bazaarvoice module, custom attributes.
 */

/**
 * Implements COMPONENT_features_export_options().
 */
function bazaarvoice_locale_features_export_options() {
  $options = array();

  if ($locales = bazaarvoice_get_locales()) {
    $languages = language_list('language');
    foreach ($locales as $lang_code => $locale) {
      $options[$lang_code] = $languages[$lang_code]->name;
    }
  }

  return $options;
}

/**
 * Implements COMPONENT_features_export().
 */
function bazaarvoice_locale_features_export($data, &$export, $module_name) {
  $export['dependencies']['bazaarvoice'] = 'bazaarvoice';
  $pipe = array();
  foreach ($data as $language) {
    $export['features']['bazaarvoice_locale'][$language] = $language;
    $pipe['language'][] = $language;
  }

  return $pipe;
}

/**
 * Implements COMPONENT_features_export_render().
 */
function bazaarvoice_locale_features_export_render($module_name, $data) {
  $items = array();
  if ($locales = bazaarvoice_get_locales()) {
    foreach ($data as $language) {
      if (isset($locales[$language])) {
        $items[$language] = $locales[$language];
      }
    }
  }

  $code = "  \$items = " . features_var_export($items, '  ') . ";\n";
  $code .= '  return $items;';

  return array('bazaarvoice_locale' => $code);
}

/**
 * Implements COMPONENT_features_revert().
 */
function bazaarvoice_locale_features_revert($module) {
  bazaarvoice_locale_features_rebuild($module);
}

/**
 * Implements COMPONENT_features_enable_feature().
 */
function bazaarvoice_locale_features_enable_feature($module) {
  bazaarvoice_locale_features_rebuild($module);
}

/**
 * Implements COMPONENT_features_rebuild().
 */
function bazaarvoice_locale_features_rebuild($module) {
  if ($locales = features_get_default('bazaarvoice_locale', $module)) {
    bazaarvoice_set_locales($locales);
  }
  drupal_static_reset('bazaarvoice_get_locales');
}

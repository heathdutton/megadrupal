<?php

/**
 * @return array
 *   A keyed array of items, suitable for use with a FormAPI select or
 *   checkboxes element.
 */
function features_site_frontpage_features_export_options() {
  $front_pages = array();
  $site_frontpage_setting = features_site_frontpage_deployable_value();

  $front_pages[$site_frontpage_setting] = t('Site Front Page');
  return $front_pages;
}

/**
 * @param array $data
 *   this is the machine name for the component in question
 * @param array &$export
 *   array of all components to be exported
 * @param string $module_name
 *   The name of the feature module to be generated.
 * @return array
 *   The pipe array of further processors that should be called
 */
function features_site_frontpage_features_export($data, &$export, $module_name = '') {

  $site_frontpage_setting = features_site_frontpage_deployable_value();
  $export['dependencies']['features'] = 'features';
  $export['dependencies']['uuid'] = 'uuid';
  $export['features']['features_site_frontpage'][$site_frontpage_setting] = $site_frontpage_setting;

  return $export;
}

/**
 * Render one or more component objects to code.
 *
 * @param string $module_name
 *   The name of the feature module to be exported.
 * @param array $data
 *   An array of machine name identifiers for the objects to be rendered.
 * @param array $export
 *   The full export array of the current feature being exported. This is only
 *   passed when hook_features_export_render() is invoked for an actual feature
 *   update or recreate, not during state checks or other operations.
 * @return array
 *   An associative array of rendered PHP code where the key is the name of the
 *   hook that should wrap the PHP code. The hook should not include the name
 *   of the module, e.g. the key for `hook_example` should simply be `example`.
 */
function features_site_frontpage_features_export_render($module, $data) {

  $code = array();

  foreach ($data as $name) {
    $code['enabled'] = $name;
  }

  $code = "  return " . features_var_export($code, '  ') . ";";
  return array('features_site_frontpage_features_default_frontpage' => $code);
}

/**
 * Implements hook_features_revert().
 */
function features_site_frontpage_features_revert($module) {
  features_site_frontpage_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 *
 * Rebuilds @font-your-face fonts from code defaults.
 */
function features_site_frontpage_features_rebuild($module) {
  $site_frontpages = module_invoke($module, 'features_site_frontpage_features_default_frontpage');

  foreach ($site_frontpages as $site_frontpage) {
    $site_frontpage_setting = $site_frontpage;

    if (substr($site_frontpage, 0, 5) == 'UUID:') {
      $uuids[] = substr($site_frontpage, 5);
      $nids = entity_get_id_by_uuid('node', $uuids);
      $nid = array_shift($nids);
      $site_frontpage_setting = 'node/' . $nid;
    }

    variable_set('site_frontpage', $site_frontpage_setting);
  }
}

/**
 *  Provides a deployable version of the site frontpage setting,
 *  node ID is converted to UUID.
 **/
function features_site_frontpage_deployable_value() {

  $site_frontpage = variable_get('site_frontpage', NULL);
  $site_frontpage_setting = $site_frontpage;

  if (!empty($site_frontpage) && substr($site_frontpage, 0, 5) == 'node/') {
    $nid = substr($site_frontpage, 5);

    if (is_numeric($nid)) {
      $uuid = entity_get_uuid_by_id('node', array($nid));

      if (is_array($uuid)) {
        $site_frontpage_setting = 'UUID:' . array_shift($uuid);
      }
    }
  }

  return $site_frontpage_setting;
}

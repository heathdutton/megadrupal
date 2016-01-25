<?php

/**
 * Page callback for JavaScript web labels.
 */
function librejs_jslicense() {
  $licenses = librejs_licenses();
  $variables['attributes']['id'] = 'jslicense-labels1';
  $variables['header'] = array(t('Script'), t('License'), t('Source'));
  foreach (variable_get('drupal_js_cache_files', array()) as $cache => $file) {
    $url = file_create_url($file);
    $variables['rows'][] = array(
      l(basename($file), $url),
      l('GNU-GPL-3.0-or-later', 'http://www.gnu.org/licenses/gpl-3.0.html'),
      l(t('See below'), $url),
    );
  }
  $javascript = db_query('SELECT data, license, source, version, type FROM {librejs} ORDER BY data');
  foreach ($javascript as $js) {
    if ($js->type == 'external') {
      $url = $js->data;
    }
    else {
      $url = file_create_url($js->data);
      if ($js->version) {
        $url .= '?' . variable_get('drupal_js_version_query_string', 'v=') . urlencode($js->version);
      }
      else {
        $url .= '?' . variable_get('css_js_query_string', '0');
      }
    }
    if (!$js->source) {
      $js->source = $url;
    }
    $variables['rows'][] = array(
      l(basename($js->data), $url),
      $js->license ? l($licenses[$js->license], $js->license) : t('Other'),
      l(basename(parse_url($js->source, PHP_URL_PATH)), $js->source),
    );
  }
  return theme('table', $variables);
}

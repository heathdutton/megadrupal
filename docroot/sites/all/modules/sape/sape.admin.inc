<?php
/**
 * @file
 * The Sape module admin interface.
 */

/**
 * Menu callback. Generate the page with some statistic information.
 *
 * @see sape_menu()
 */
function sape_info_page() {
  $o = '';
  $path = variable_get('sape_file_path', SAPE_DEFAULT_FILE_PATH);
  $path = _sape_get_path($path);
  $path = dirname($path);
  $host = variable_get('sape_host', $_SERVER['SERVER_NAME']);
  $links = $host . '.links.db';
  $words = $host . '.words.db';

  $path_links = $path . '/' . $links;
  if (!is_file($path_links) || !$info = file_get_contents($path_links)) {
    return t('File %path_links not found, or it is empty.', array('%path_links' => $path_links));
  }

  $info = unserialize($info);
  $o .= '<h3>' . t('Info from %file (%fdt):', array('%file' => $links, '%fdt' => format_date(filemtime($path . '/' . $links), 'short'))) . '</h3>';

  $header = array(
    array('data' => t('IP')),
    array('data' => t('Banned')),
    array('data' => t('Hits')),
    array('data' => t('Generation time')),
    array('data' => t('Last visit')),
    array('data' => t('Actions')),
  );

  $rows = array();
  $dest = drupal_get_destination();
  $stat_enabled = module_exists('statistics');

  foreach ($info['__sape_ips__'] as $ip) {
    // read ban flag
    if (drupal_is_denied('host', $ip)) {
      $banned = TRUE;
    }
    else {
      $banned = FALSE;
    }
    // read hits for ips
    if ($stat_enabled) {
      $res = db_query('SELECT COUNT(a.uid) AS hits, SUM(a.timer) AS total, MAX(a.timestamp) AS last
      FROM {accesslog} AS a
      WHERE LOWER(a.hostname) LIKE :host', array(':host' => '%' . $ip));
    }
    $actions = array(
      $banned ? l(t('unban'), 'admin/user/rules/', array('query' => $dest)) : l(t('ban'), "admin/user/rules/add/$ip/host", array('query' => $dest)),
    );
    $rows[] = array(
      $ip,
      $banned ? t('Yes') : t('No'),
      $stat_enabled ? $stat['hits'] : t('n/a'),
      $stat_enabled ? format_interval(round($stat['total'] / 1000)) : t('n/a'),
      $stat_enabled ? ($stat['last'] ? format_date($stat['last'], 'short') . ' ' . t('(@time ago)', array('@time' => format_interval(REQUEST_TIME - $stat['last']))) : t('No visits')) : t('n/a'),
      implode(' | ', $actions),
    );
  }

  $caption = t('Sape IPs');

  $o .= theme_table($header, $rows, array(), $caption);

  return $o;
}

/**
 * Form builder for the main Sape configuration page.
 *
 * @see sape_menu()
 * @ingroup forms
 */
function sape_admin_form($form, &$form_state) {
  $form = array();

  $form['sape_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable module'),
    '#default_value' => variable_get('sape_enabled', 0),
    '#description' => t('You can disable showing links without complete disable the module.'),
    '#required' => FALSE,
    '#weight' => -10,
  );

  $form['sape_usercode'] = array(
    '#type' => 'textfield',
    '#title' => t('User code (hash) from <strong>SAPE.RU</strong>'),
    '#default_value' => variable_get('sape_usercode', SAPE_DUMMY_USERCODE),
    '#description' => t('Enter a user code from SAPE.RU, gives to you after <a href="http://www.sape.ru/r.b3b74f2ba7.php" target="_blank">registration</a>.'),
    '#required' => TRUE,
    '#weight' => -7,
  );

  $form['sape_blocks_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Advertisement blocks count'),
    '#default_value' => variable_get('sape_blocks_count', 3),
    '#description' => t('Blocks where SAPE.RU will show the links'),
    '#required' => TRUE,
    '#weight' => -5,
  );

  $form['sape_context_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable context links for body'),
    '#default_value' => variable_get('sape_context_enabled', 1),
    '#description' => t('Enable context links'),
    '#required' => FALSE,
    '#weight' => -3,
  );

  $form['sape_file_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to sape.php'),
    '#default_value' => variable_get('sape_file_path', SAPE_DEFAULT_FILE_PATH),
    '#description' => t('Enter a path to the file sape.php (you can download it from  SAPE.RU)'),
    '#required' => TRUE,
    '#weight' => -2,
    '#after_build' => array('sape_check_file'),
  );

  return system_settings_form($form);
}

/**
 * Check the file exists. After build form element function.
 *
 * @see sape_admin_form()
 */
function sape_check_file($form_element) {
  $uri = _sape_get_path($form_element['#value']);
  if (!is_file($uri)) {
    form_set_error($form_element['#parents'][0], t('The file %uri does not exist.', array('%uri' => $uri)));
    return $form_element;
  }

  require_once($uri);
  if (!class_exists('SAPE_base') || !class_exists('SAPE_client')) {
    form_set_error($form_element['#parents'][0], t('Main Sape classes does not exist.'));
  }
  if (!class_exists('SAPE_context')) {
    form_set_error($form_element['#parents'][0], t('Possible old sape.php file - no context links support.'));
  }

  $dir = dirname($uri);
  if (is_dir($dir) && !is_writable($dir) && !drupal_chmod($dir)) {
    form_set_error($form_element['#parents'][0], t('The directory %dir exists but is not writable and could not be made writable.', array('%dir' => $dir)));
  }

  return $form_element;
}

/**
 * Form builder for the main Sape configuration page.
 *
 * @see sape_menu()
 * @ingroup forms
 */
function sape_code_form($form, &$form_state) {
  $form = array();

  $form['sape_charset'] = array(
    '#type' => 'textfield',
    '#title' => t('Site charset'),
    '#default_value' => variable_get('sape_charset', 'UTF-8'),
    '#description' => t('Enter a site charset. Usually, it is "UTF-8" for Drupal sites . You can reference following <a href="http://www.php.net/manual/en/function.iconv.php">this link</a>.'),
    '#required' => TRUE,
    '#weight' => -10,
  );

  $form['sape_multi_site'] = array(
    '#type' => 'checkbox',
    '#title' => t('Multi-site mode'),
    '#default_value' => variable_get('sape_multi_site', TRUE),
    '#description' => t('Setup the multi-site flag. It is neccessary, if you store your links.db and words.db in one folder for multiple sites.'),
    '#required' => TRUE,
    '#weight' => -9,
  );

  $form['sape_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Site host'),
    '#default_value' => variable_get('sape_host', $_SERVER['SERVER_NAME']),
    '#description' => t("Use this, when SAPE code can't correct find the site host."),
    '#required' => TRUE,
    '#weight' => -8,
  );

  $form['sape_verbose'] = array(
    '#type' => 'checkbox',
    '#title' => t('Error reporting'),
    '#default_value' => variable_get('sape_verbose', FALSE),
    '#description' => t('Turn on if you want the module show you errors (verbose mode).'),
    '#required' => TRUE,
    '#weight' => -7,
  );

  $form['sape_force_show_code'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force show code'),
    '#default_value' => variable_get('sape_force_show_code', FALSE),
    '#description' => t('Turn on when you want the code always show.'),
    '#required' => TRUE,
    '#weight' => -6,
  );

  return system_settings_form($form);
}

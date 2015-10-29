<?php
/**
 * @file
 * VS Conditions - built-in condition types.
 */

/**
 * Advertises the condition types implemented in this file.
 *
 * @return array
 *   An associative array of:
 *   - key: machine readable name of the condition,
 *   - value: an associative array of:
 *     - name: the localized name of the condition,
 *     - description: the localized description of the condition.
 */
function virtual_site_condition_requirement_info() {
  return array(
    'virtual_site_condition_php' => array(
      'name' => t('PHP code'),
      'description' => t('Validates if the following PHP code returns <code>TRUE</code> (experts only!).'),
    ),
    'virtual_site_condition_pages' => array(
      'name' => t('Pages'),
      'description' => t('Validates (not) on a selection of pages.'),
    ),
    'virtual_site_condition_sites' => array(
      'name' => t('Sites'),
      'description' => t('Validates (not) on a selection of site configurations.'),
    ),
    'virtual_site_condition_hostname' => array(
      'name' => t('Hostname'),
      'description' => t('Validates (not) on a selection of site hostnames.'),
    ),
    'virtual_site_condition_browser' => array(
      'name' => t('Browser'),
      'description' => t('Validates (not) on a selection of browser User Agents.'),
    ),
  );
}


/**
 * Form constructor for the "PHP code" condition type.
 *
 * @see virtual_site_condition_requirement_info()
 * @see virtual_site_condition_php_form_submit()
 *
 * @ingroup forms
 */
function virtual_site_condition_php_form($context) {
  $form = array();

  $form['php_code'] = array(
    '#type' => 'textarea',
    '#title' => t('PHP Code'),
    '#description' => t('Enter PHP code between <em>&lt;?php ?&gt;</em>. Note that executing incorrect PHP code can break your Drupal site.'),
    '#default_value' => isset($context['code']) ? $context['code'] : '',
  );
  $form['php_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete requirement'),
    '#description' => t('Check the box to remove this individual requirement.'),
    '#disabled' => !isset($context['code']),
  );

  return $form;
}

/**
 * Form submission handler for virtual_site_condition_php_form().
 *
 * @return array
 *   Associative array describing the condition.
 */
function virtual_site_condition_php_form_submit($form, &$form_state) {
  if ($form_state['values']['php_delete'] == 1) {
    return array();
  }
  elseif ($form_state['values']['php_code']) {
    return array('code' => $form_state['values']['php_code']);
  }
}

/**
 * Evaluates the "PHP code" condition type.
 *
 * @param array $condition
 *   The condition to be evaluated.
 * @param array $context
 *   (Optional) The context of the condition.
 *
 * @return bool
 *   TRUE if the condition is met,
 *   FALSE if not.
 */
function virtual_site_condition_php(&$condition, $context = array()) {
  return ($context['operator'] == CONDITION_SKIP || !$context['code'] || drupal_eval($context['code']));
}

/**
 * Form constructor for the "Pages" condition type.
 *
 * @see virtual_site_condition_requirement_info()
 * @see virtual_site_condition_pages_form_submit()
 *
 * @ingroup forms
 */
function virtual_site_condition_pages_form($context) {
  $form = array();

  $form['pages_operator'] = array(
    '#type' => 'radios',
    '#title' => t('Apply when'),
    '#description' => t("The <em>Always</em> setting allows you to temporarily override a requirement without actually deleting it. If you no longer need it, it's better to remove it completely."),
    '#options' => array(
      CONDITION_TRUE => t('Only on the listed pages.'),
      CONDITION_FALSE => t('Except on the listed pages.'),
      CONDITION_SKIP => t('Always (skip requirement).'),
    ),
    '#default_value' => isset($context['operator']) ? $context['operator'] : CONDITION_TRUE,
  );
  $form['pages_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#description' => t('Enter one page per line as Drupal paths. The * character is a wildcard. Example paths are <em>blog</em> for the blog page and <em>blog/*</em> for every personal blog. <em>&lt;front&gt;</em> is the front page.'),
    '#default_value' => isset($context['pages']) ? $context['pages'] : '',
  );
  $form['pages_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete requirement'),
    '#description' => t('Check the box to remove this individual requirement.'),
    '#disabled' => !isset($context['pages']),
  );

  return $form;
}

/**
 * Form submission handler for virtual_site_condition_pages_form().
 *
 * @return array
 *   Associative array describing the condition.
 */
function virtual_site_condition_pages_form_submit($form, &$form_state) {
  if ($form_state['values']['pages_delete'] == 1) {
    return array();
  }
  elseif ($form_state['values']['pages_operator'] && $form_state['values']['pages_pages']) {
    return array(
      'operator' => $form_state['values']['pages_operator'],
      'pages' => $form_state['values']['pages_pages'],
    );
  }
}

/**
 * Evaluates the "Pages" condition type.
 *
 * @param array $condition
 *   The condition to be evaluated.
 * @param array $context
 *   (Optional) The context of the condition.
 *
 * @return bool
 *   TRUE if the condition is met,
 *   FALSE if not.
 */
function virtual_site_condition_pages(&$condition, $context = array()) {
  if ($context['operator'] == CONDITION_SKIP) {
    return TRUE;
  }
  else {
    $path = $_GET['q'];
    $alias = drupal_get_path_alias($path);
    $condition_met = (drupal_match_path($alias, $context['pages']) || ($path != $alias && drupal_match_path($path, $context['pages'])));
    return ($context['operator'] == CONDITION_FALSE xor $condition_met);
  }
}

/**
 * Form constructor for the "Sites" condition type.
 *
 * @see virtual_site_condition_requirement_info()
 * @see virtual_site_condition_sites_form_submit()
 *
 * @ingroup forms
 */
function virtual_site_condition_sites_form($context) {
  $form = array();

  $options = array();
  $handle = opendir('./sites');
  while ($dir = readdir($handle)) {
    if (is_dir('./sites/'. $dir) && file_exists('./sites/'. $dir .'/settings.php')) {
      $options[$dir] = (conf_path() == 'sites/'. $dir) ? '<strong>'. $dir .'</strong>' : $dir;
    }
  }

  $form['sites_operator'] = array(
    '#type' => 'radios',
    '#title' => t('Apply when'),
    '#description' => t("The <em>Always</em> setting allows you to temporarily override a requirement without actually deleting it. If you no longer need it, it's better to remove it completely."),
    '#options' => array(
      CONDITION_TRUE => t('Only on the listed sites.'),
      CONDITION_FALSE => t('Except on the listed sites.'),
      CONDITION_SKIP => t('Always (skip requirement).'),
    ),
    '#default_value' => isset($context['operator']) ? $context['operator'] : CONDITION_TRUE,
  );
  $form['sites_sites'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Sites'),
    '#description' => t('This is the list of all Drupal multisite directories. The currently active one is written in <strong>bold</strong>.'),
    '#options' => $options,
    '#default_value' => isset($context['sites']) ? $context['sites'] : array(),
  );
  $form['sites_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete requirement'),
    '#description' => t('Check the box to remove this individual requirement.'),
    '#disabled' => !isset($context['sites']),
  );

  return $form;
}

/**
 * Form submission handler for virtual_site_condition_sites_form().
 *
 * @return array
 *   Associative array describing the condition.
 */
function virtual_site_condition_sites_form_submit($form, &$form_state) {
  if ($form_state['values']['sites_delete'] == 1) {
    return array();
  }
  else {
    $sites_sites = array_filter(array_unique(array_values($form_state['values']['sites_sites'])));
    if ($form_state['values']['sites_operator'] && $form_state['values']['sites_sites'] && count($sites_sites)) {
      return array(
        'operator' => $form_state['values']['sites_operator'],
        'sites' => $sites_sites,
      );
    }
  }
}

/**
 * Evaluates the "Sites" condition type.
 *
 * @param array $condition
 *   The condition to be evaluated.
 * @param array $context
 *   (Optional) The context of the condition.
 *
 * @return bool
 *   TRUE if the condition is met,
 *   FALSE if not.
 */
function virtual_site_condition_sites(&$condition, $context = array()) {
  if ($context['operator'] == CONDITION_SKIP) {
    return TRUE;
  }
  else {
    $condition_met = in_array(drupal_substr(conf_path(), drupal_strlen('sites/')), $context['sites']);
    return ($context['operator'] == CONDITION_FALSE xor $condition_met);
  }
}

/**
 * Form constructor for the "Hostname" condition type.
 *
 * @see virtual_site_condition_requirement_info()
 * @see virtual_site_condition_hostname_form_submit()
 *
 * @ingroup forms
 */
function virtual_site_condition_hostname_form($context) {
  $form = array();

  $form['hostname_operator'] = array(
    '#type' => 'radios',
    '#title' => t('Apply when'),
    '#description' => t("The <em>Always</em> setting allows you to temporarily override a requirement without actually deleting it. If you no longer need it, it's better to remove it completely."),
    '#options' => array(
      CONDITION_TRUE => t('Only on the listed hostnames.'),
      CONDITION_FALSE => t('Except on the listed hostnames.'),
      CONDITION_SKIP => t('Always (skip requirement).'),
    ),
    '#default_value' => isset($context['operator']) ? $context['operator'] : CONDITION_TRUE,
  );
  $form['hostname_hostnames'] = array(
    '#type' => 'textarea',
    '#title' => t('Hostnames'),
    '#description' => t('Enter one or more (partial) hostnames, eg. <code>example.com.</code>  One item per line.'),
    '#default_value' => isset($context['hostnames']) ? $context['hostnames'] : '',
  );
  $form['hostname_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete requirement'),
    '#description' => t('Check the box to remove this individual requirement.'),
    '#disabled' => !isset($context['hostnames']),
  );

  return $form;
}

/**
 * Form submission handler for virtual_site_condition_hostname_form().
 *
 * @return array
 *   Associative array describing the condition.
 */
function virtual_site_condition_hostname_form_submit($form, &$form_state) {
  if ($form_state['values']['hostname_delete'] == 1) {
    return array();
  }
  elseif ($form_state['values']['hostname_operator'] && $form_state['values']['hostname_hostnames']) {
    return array(
      'operator' => $form_state['values']['hostname_operator'],
      'hostnames' => $form_state['values']['hostname_hostnames'],
    );
  }
}

/**
 * Evaluates the "Hostname" condition type.
 *
 * @param array $condition
 *   The condition to be evaluated.
 * @param array $context
 *   (Optional) The context of the condition.
 *
 * @return bool
 *   TRUE if the condition is met,
 *   FALSE if not.
 */
function virtual_site_condition_hostname(&$condition, $context = array()) {
  if ($context['operator'] == CONDITION_SKIP) {
    return TRUE;
  }
  else {
    $hostnames = preg_split('/[\r\n]+/', $context['hostnames']);
    $found = FALSE;
    foreach ($hostnames as $hostname) {
      if (stristr($_SERVER['HTTP_HOST'], $hostname)) {
        return ($context['operator'] == CONDITION_TRUE);
      }
    }
    return ($context['operator'] == CONDITION_FALSE);
  }
}

/**
 * Form constructor for the "Browser" condition type.
 *
 * @see virtual_site_condition_requirement_info()
 * @see virtual_site_condition_browser_form_submit()
 *
 * @ingroup forms
 */
function virtual_site_condition_browser_form($context) {
  $form = array();

  $form['browser_operator'] = array(
    '#type' => 'radios',
    '#title' => t('Apply when'),
    '#description' => t("The <em>Always</em> setting allows you to temporarily override a requirement without actually deleting it. If you no longer need it, it's better to remove it completely."),
    '#options' => array(
      CONDITION_TRUE => t('Only on the listed browsers.'),
      CONDITION_FALSE => t('Except on the listed browsers.'),
      CONDITION_SKIP => t('Always (skip requirement).'),
    ),
    '#default_value' => isset($context['operator']) ? $context['operator'] : CONDITION_TRUE,
  );
  $form['browser_browsers'] = array(
    '#type' => 'textarea',
    '#title' => t('User Agent fragments'),
    '#description' => t('Enter one or more browser-specific UA fragments, separated by commas. Case is not important. To target mobile device (smartphone, tablet, etc) browsers, use the following list:<br /><em>phone,mobi,mobile,midp,cldc,wap,android,blackberry,symbian,symbos,maemo,palm,webos,windows ce</em>'),
    '#default_value' => isset($context['browsers']) ? $context['browsers'] : '',
  );
  $form['browser_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete requirement'),
    '#description' => t('Check the box to remove this individual requirement.'),
    '#disabled' => !isset($context['browsers']),
  );

  return $form;
}

/**
 * Form submission handler for virtual_site_condition_browser_form().
 *
 * @return array
 *   Associative array describing the condition.
 */
function virtual_site_condition_browser_form_submit($form, &$form_state) {
  if ($form_state['values']['browser_delete'] == 1) {
    return array();
  }
  elseif ($form_state['values']['browser_operator'] && $form_state['values']['browser_browsers']) {
    return array(
      'operator' => $form_state['values']['browser_operator'],
      'browsers' => $form_state['values']['browser_browsers'],
    );
  }
}

/**
 * Evaluates the "Browser" condition type.
 *
 * @param array $condition
 *   The condition to be evaluated.
 * @param array $context
 *   (Optional) The context of the condition.
 *
 * @return bool
 *   TRUE if the condition is met,
 *   FALSE if not.
 */
function virtual_site_condition_browser(&$condition, $context = array()) {
  if ($context['operator'] == CONDITION_SKIP) {
    return TRUE;
  }
  else {
    $browsers = str_replace(',', '|', $context['browsers']);
    $user_agent = strtolower($_SERVER['HTTP_USER_AGENT']);
    $is_browser = preg_match('/('. $browsers. ')/i', $user_agent);
    return ($context['operator'] == CONDITION_FALSE xor $is_browser);
  }
}

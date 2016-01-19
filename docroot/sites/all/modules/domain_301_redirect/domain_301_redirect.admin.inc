<?php

/**
 * @file
 * Administrative pages for the Domain 301 Redirect module.
 */

/**
 * Admin form for configuring Domain 301 Redirect.
 */
function domain_301_redirect_admin_form($form_state) {
  $disabled_by_check = variable_get('domain_301_redirect_disabled_by_check', false);
  $enabled = variable_get('domain_301_redirect_enabled', 0);

  // Warn the user if the redirect was disabled by cron.
  if (!$enabled && $disabled_by_check) {
    $domain = variable_get('domain_301_redirect_domain', '');
    $last_checked = variable_get('domain_301_redirect_last_checked', 0);
    drupal_set_message(t('Redirects have been disabled by cron because the domain was not available at: %date.', array('%date' => format_date($last_checked))), 'warning');
  }

  $form['domain_301_redirect_enabled'] = array(
    '#type' => 'radios',
    '#title' => t('Status'),
    '#description' => t('Enable this setting to start 301 redirects to the domain below for all other domains.'),
    '#options' => array(
      1 => t('Enabled'),
      0 => t('Disabled'),
    ),
    '#default_value' => variable_get('domain_301_redirect_enabled', 0),
  );
  $form['domain_301_redirect_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain'),
    '#description' => t('This is the domain that all other domains that point to this site will be 301 redirected to. This value should also include the scheme (http or https). E.g. http://www.testsite.com'),
    '#default_value' => variable_get('domain_301_redirect_domain', ''),
  );
  $form['domain_301_redirect_check_period'] = array(
    '#type' => 'select',
    '#title' => t('Domain Check'),
    '#description' => t('This option selects the period between domain validation checks. If the Domain no longer points to this site, Domain 301 Redirection will be disabled.'),
    '#options' => array(
      0 => t('Disabled'),
      3600 => t('1 hour'),
      7200 => t('2 hours'),
      10800 => t('3 hours'),
      21600 => t('6 hours'),
      43200 => t('12 hours'),
      86400 => t('1 day'),
    ),
    '#default_value' => variable_get('domain_301_redirect_check_period', 60 * 60 * 3), // Defaults to 3 hours.
  );
  $form['domain_301_redirect_domain_check_retries'] = array(
      '#type' => 'select',
      '#title' => t('Domain retries'),
      '#description' => t('Number of times to check domain availability before disabling redirects.'),
      '#options' => array(1 => 1, 2 => 2, 3 => 3),
      '#default_value' => variable_get('domain_301_redirect_domain_check_retries', 3),
  );
  $form['domain_301_redirect_domain_check_reenable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Re-enable domain redirection'),
      '#description' => t('Turn domain redirection on when the domain becomes available.'),
      '#default_value' => variable_get('domain_301_redirect_domain_check_reenable', true),
  );

  // Per-path configuration settings to apply the redirect to specific paths.
  $form['applicability']['path'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 0,
  );

  $options = array(
    BLOCK_VISIBILITY_NOTLISTED => t('All pages except those listed'),
    BLOCK_VISIBILITY_LISTED => t('Only the listed pages'),
  );
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

  $title = t('Pages');

  $form['applicability']['path']['domain_301_redirect_applicability'] = array(
      '#type' => 'radios',
      '#title' => t('Redirect on specific pages'),
      '#options' => $options,
      '#default_value' => variable_get('domain_301_redirect_applicability', BLOCK_VISIBILITY_NOTLISTED),
  );
  $form['applicability']['path']['domain_301_redirect_pages'] = array(
      '#type' => 'textarea',
      '#title' => '<span class="element-invisible">' . $title . '</span>',
      '#default_value' => variable_get('domain_301_redirect_pages'),
      '#description' => $description,
  );

  return system_settings_form($form);
}

/**
 * Validation hook for Domain 301 Redirect Admin page.
 */
function domain_301_redirect_admin_form_validate(&$form, &$form_state) {
  if (!empty($form_state['values']['domain_301_redirect_enabled'])) {
    $domain = trim($form_state['values']['domain_301_redirect_domain']);
    if (!preg_match('|^https?://|', $domain)) {
      $domain = 'http://' . $domain;
    }
    if (!valid_url($domain, TRUE)) {
      form_set_error('domain_301_redirect_enabled', t('Domain 301 redirection can not be enabled if no valid domain is set.'));
    }
    else {
      $domain_parts = parse_url($domain);
      $domain = $domain_parts['scheme'] . '://' . $domain_parts['host'] . (!empty($domain_parts['port']) ? ':' . $domain_parts['port'] : '');
      form_set_value($form['domain_301_redirect_domain'], $domain, $form_state);

      if (!domain_301_redirect_check_domain($domain)) {
        form_set_error('domain_301_redirect_enabled', t('Domain 301 redirection can not be enabled as the domain you set does not currently point to this site.'));
      }
      else {
        // Clean up if someone is manually disabling. We don't want the system to
        // re-enable if the disabling was via the admin form.
        variable_set('domain_301_redirect_disabled_by_check', false);
      }

      if (domain_301_redirect_check_loop($domain)) {
        form_set_error('domain_301_redirect_domain', t('The domain cannot be set, as it causes a redirect loop (within @num redirects).', array('@num' => variable_get('domain_301_redirect_loop_max_redirects', 3))));
      }
    }
  }
}

/**
 * Checks for multi level redirect loops with a specified domain.
 *
 * @param string $domain
 *   Domain name to test.
 *
 * @return bool
 *   TRUE if a redirect loop was detected, otherwise FALSE.
 */
function domain_301_redirect_check_loop($domain) {
  // Get host from configured domain.
  $redirect_host = drupal_strtolower(parse_url($domain, PHP_URL_HOST));
  $host = drupal_strtolower(parse_url($GLOBALS['base_url'], PHP_URL_HOST));

  // Redirecting back to this site actually is actively ignored in hook_init, so
  // it makes no sense to allow users to set this as a value. On the other hand
  // when the admin is on the redirected domain he should still be able to alter
  // other settings without first disabling redirection. So let's just accept
  // the current host.
  if ($redirect_host == $host) {
    return FALSE;
  }

  $redirect_loop = FALSE;
  $redirects_to_check = variable_get('domain_301_redirect_loop_max_redirects', 3);
  $checked = 0;

  // Make a request to the domain that is being configured, following a
  // configured number of redirects. This has to be done individually, because
  // if checking all 3 levels at once, we might happen to get the wrong one back
  // (if the redirect loop has multiple levels).
  do {
    $response = drupal_http_request($domain, array('max_redirects' => 0, 'method' => 'HEAD'));
    if (!empty($response->redirect_url)) {
      // Request target for the next request loop.
      $domain = $response->redirect_url;
      // Check if any host names match the redirect host name.
      $location_host = drupal_strtolower(parse_url($response->redirect_url, PHP_URL_HOST));
      if ($redirect_host == $location_host || $host == $location_host) {
        $redirect_loop = TRUE;
      }
    }
    $checked++;
    // Don't check the redirect code, as it's possible there may be another
    // redirect service in operation that does not use 301.
  } while (!$redirect_loop && !empty($response->redirect_url) && $checked < $redirects_to_check);

  return $redirect_loop;
}

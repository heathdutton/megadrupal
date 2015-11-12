<?php
/**
 * @file
 * The admin settings for the PageRank Widget module.
 */

/**
 * Implements hook_settings().
 */
function pr_widget_admin_settings() {
  $last_refresh = variable_get('pr_widget_next_execution', 0) - variable_get('pr_widget_interval', 24 * 60 * 60);
  // A preview area.
  $form['pr_widget_preview'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preview'),
    '#description' => t("The actual font color of the optional copyright notice can differ depending on the background color of the region the widget is in.") . '</br></br>',
    '#collapsible' => TRUE,
    '#collapsed' => variable_get('pr_widget_preview_collapsed', TRUE),
  );
  // Form submit resulted in an uncollapsed preview. Set it back.
  variable_set('pr_widget_preview_collapsed', TRUE);
  drupal_add_css(drupal_get_path('module', 'pr_widget') . '/pr_widget.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE));
  $results = array(
    'module_path' => drupal_get_path('module', 'pr_widget'),
  );
  $form['pr_widget_preview']['pr_widget_preview_markup'] = array(
    '#markup' => theme('pr_widget', array('results' => $results)),
  );

  $form['pr_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('PageRank'),
  );

  $form['pr_settings']['pr_widget_pagerank_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => variable_get('pr_widget_pagerank_enabled', 1),
    '#description' => t('If you only want to use the copyright notice, disable the pagerank display.'),
  );

  // CSS variables.
  $style_settings_module = l(t('Style (CSS) Settings module'), 'https://drupal.org/project/style_settings', array(
      'attributes' => array(
        'title' => t('Style (CSS) Settings | Drupal.org'),
        'target' => '_blank',
      ),
  ));
  // Put CSS variables together in a fieldset. Remove if only one is given.
  $form['pr_settings']['css_variables'] = array(
    '#type' => 'fieldset',
    '#title' => t('CSS variables'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  if (module_exists('style_settings')) {
    $form['pr_settings']['css_variables']['#collapsed'] = TRUE;
    $form['pr_settings']['css_variables']['pr_widget_reset'] = array(
      '#type' => 'checkbox',
      '#title' => t('Reset all values back to default'),
      '#default_value' => 0,
    );
    // A SELECTABLE MEASUREMENT UNIT (px, em). It goes together with a submit
    // handler inside the 'function pr_widget_admin_settings_submit()'.
    $form['pr_settings']['css_variables']['pr_widget_fontsize'] = array(
      '#type' => 'fieldset',
      '#title' => t('Font size'),
      // Make containing fields align horizontally.
      '#attributes' => array('class' => array('container-inline')),
      // Number field input help (min, max, step) will be appended.
    );
    // Number field without a '#field_suffix'.
    $form['pr_settings']['css_variables']['pr_widget_fontsize']['pr_widget_fontsize_value'] = array(
      '#type' => 'style_settings_number',
      '#default_value' => variable_get('pr_widget_fontsize_value', '1'),
    );
    // A measurement unit select field.
    $form['pr_settings']['css_variables']['pr_widget_fontsize']['pr_widget_fontsize_unit'] = array(
      '#type' => 'select',
      '#options' => array(
        'px' => t('px'),
        'em' => t('em'),
        '%' => t('%'),
      ),
      '#default_value' => variable_get('pr_widget_fontsize_unit', 'em'),
      '#required' => TRUE,
    );
    // A SELECTABLE MEASUREMENT UNIT (px, em). It goes together with a submit
    // handler inside the 'function pr_widget_admin_settings_submit()'.
    $form['pr_settings']['css_variables']['pr_widget_radius'] = array(
      '#type' => 'fieldset',
      '#title' => t('Rounded corner radius'),
      // Make containing fields align horizontally.
      '#attributes' => array('class' => array('container-inline')),
      // Number field input help (min, max, step) will be appended.
    );
    // Number field without a '#field_suffix'.
    $form['pr_settings']['css_variables']['pr_widget_radius']['pr_widget_radius_value'] = array(
      '#type' => 'style_settings_number',
      '#default_value' => variable_get('pr_widget_radius_value', '5'),
    );
    // A measurement unit select field.
    $form['pr_settings']['css_variables']['pr_widget_radius']['pr_widget_radius_unit'] = array(
      '#type' => 'select',
      '#options' => array(
        'px' => t('px'),
        'em' => t('em'),
      ),
      '#default_value' => variable_get('pr_widget_radius_unit', 'px'),
      '#required' => TRUE,
    );
    // NUMBER in this case with an appended measurement unit (px).
    // E.g. user input: '2', field_suffix: 'px' => stored variable: '2px'.
    $form['pr_settings']['css_variables']['pr_widget_borderwidth'] = array(
      '#type' => 'style_settings_number',
      '#title' => t('Border width'),
      '#default_value' => floatval(variable_get('pr_widget_borderwidth', '3px')),
      // The suffix gets added to the input on submit if valid measurement unit.
      '#field_suffix' => 'px',
    );

    // COLOR PICKERS.
    $form['pr_settings']['css_variables']['pr_widget_color'] = array(
      '#type' => 'style_settings_colorpicker',
      '#title' => t('Widget color'),
      // Besides hex color value also color names are accepted.
      '#default_value' => variable_get('pr_widget_color', 'YellowGreen'),
    );
    $form['pr_settings']['css_variables']['pr_widget_bgcolor'] = array(
      '#type' => 'style_settings_colorpicker',
      '#title' => t('Background color'),
      '#default_value' => variable_get('pr_widget_bgcolor', 'WhiteSmoke'),
    );
    $form['pr_settings']['css_variables']['pr_widget_shadowcolor'] = array(
      '#type' => 'style_settings_colorpicker',
      '#title' => t('Shadow color'),
      '#default_value' => variable_get('pr_widget_shadowcolor', 'Black'),
    );
    $form['pr_settings']['css_variables']['pr_widget_fontcolor'] = array(
      '#type' => 'style_settings_colorpicker',
      '#title' => t('Font color'),
      '#default_value' => variable_get('pr_widget_fontcolor', 'White'),
    );
    $form['pr_settings']['css_variables']['pr_widget_rankcolor'] = array(
      '#type' => 'style_settings_colorpicker',
      '#title' => t('Rank font color'),
      '#default_value' => variable_get('pr_widget_rankcolor', 'DimGray'),
    );
  }

  // If the Style Settings module is not enabled, provide some instructions.
  else {
    $form['pr_settings']['css_variables']['pr_widget_note'] = array(
      '#markup' => t("Enable the !style_settings_module to get style options exposed here. They consist of:<ul>
          <li>Font size.</li>
          <li>Rounded corner radius.</li>
          <li>Border width.</li>
          <li>All of the widget's colors (with color picker).</li>
        </ul>", array('!style_settings_module' => $style_settings_module)),
    );
  }

  $form['pr_settings']['pr_widget_string'] = array(
    '#type' => 'textfield',
    '#title' => t('Text'),
    '#default_value' => variable_get('pr_widget_string', t('PageRank')),
    '#description' => t('String to use. Suggestions "PR", "Google PageRank", "PageRank" (default)'),
  );

  $form['pr_settings']['pr_widget_suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Suffix'),
    '#default_value' => variable_get('pr_widget_suffix', ''),
    '#description' => t('String to append. Suggestions " / 10", " out of 10" or nothing (default). <strong>Do not forget leading spaces.</strong>'),
  );

  $form['pr_settings']['pr_widget_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link to a third party PR check'),
    '#default_value' => variable_get('pr_widget_link', 'http://domaintyper.com/PageRankCheck/'),
    '#description' => t('Leave empty to disable, but the reliability of the widget is perceived better it leads to a third party PR check (default: "http://domaintyper.com/PageRankCheck/").<br /><strong>Use only services that accept a trailing domain in the URL to query. Do not forget the trailing slash or "?q=http://"</strong>, depending on the used service.'),
  );

  // Grabbing the PageRank once a day is enough, but leave it up to the
  // site owner to decide.
  $form['pr_settings']['pr_widget_interval'] = array(
    '#type' => 'radios',
    '#title' => t('Update interval'),
    '#options' => array(
      24 * 60 * 60 => t('24 hours (recommended)'),
      0 => format_interval(DRUPAL_CRON_DEFAULT_THRESHOLD, 2) . ' ' . t('(every cron run)'),
    ),
    '#default_value' => variable_get('pr_widget_interval', 24 * 60 * 60),
    '#description' => t('Saving this form updates the PageRank instantly, independent from this setting. Last refresh was @interval ago.', array('@interval' => format_interval(REQUEST_TIME - $last_refresh))),
  );

  $form['pr_settings']['pr_widget_pagerank_fallback'] = array(
    '#type' => 'checkbox',
    '#title' => t('Subdomain fallback'),
    '#default_value' => variable_get('pr_widget_pagerank_fallback', 1),
    '#description' => t("If a subdomain returns a PageRank of '0' or 'NA' use the main domain."),
  );

  $form['pr_widget_notice'] = array(
    '#type' => 'fieldset',
    '#title' => t('Copyright notice'),
  );

  $form['pr_widget_notice']['pr_widget_notice_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => variable_get('pr_widget_notice_enabled', 0),
  );

  // For the examples we use real data.
  // Current domain name without the leading protocol.
  global $base_url;
  // For our example use real data from user, but fictional when unset.
  $year = variable_get('pr_widget_year', '');
  $notice = variable_get('pr_widget_prepend', 'All rights reserved') . ' © ' . (($year != date('Y') && !empty($year)) ? $year . '-' . date('Y') : date('Y'));
  $form['pr_widget_notice']['pr_widget_url_name'] = array(
    // Create different types of notices to choose from.
    '#type' => 'radios',
    '#title' => t('Choose a notice'),
    '#options' => array(
      $base_url => '<strong>' . $notice . ' ' . $base_url . '</strong>',
      variable_get('site_name', '') => '<strong>' . $notice . ' ' . variable_get('site_name', '') . '</strong> ' . t("(preferable if the site name is a person's full name or a company name)"),
      ' ' => '<strong>' . $notice . '</strong> ' . t('(leaving out the designation of owner is not recommended)'),
    ),
    '#default_value' => variable_get('pr_widget_url_name', ' '),
    '#description' => t("'Year of first publication' is not used until entered below, for example © 2009-") . date('Y') . '. ' . t('Save this form to refresh above examples.'),
  );

  $form['pr_widget_notice']['pr_widget_year'] = array(
    '#type' => 'textfield',
    '#title' => t('What year was the domain first online?'),
    '#default_value' => variable_get('pr_widget_year', ''),
    '#description' => t("Leave empty to display only the current year (default). Also if the 'starting year' equals the 'current year' only one will be displayed until next year.<br />To play safe legally, it's best to enter a 'Year of first publication', although copyright is in force even without any notice."),
    '#size' => 4,
    '#maxlength' => 4,
  );

  $form['pr_widget_notice']['pr_widget_prepend'] = array(
    '#type' => 'textfield',
    '#title' => t('Prepend text'),
    '#default_value' => trim(variable_get('pr_widget_prepend', 'All rights reserved')),
    '#description' => t("For example 'All images' on a photographer's website."),
  );

  // Call submit_function() on form submission.
  $form['#submit'][] = 'pr_widget_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Validate pr_widget settings submission.
 */
function pr_widget_admin_settings_validate($form, &$form_state) {
  // Before 1991 there was no world wide web and the future can't be a
  // 'year of first publication' but it can be left empty.
  $limit = $form_state['values']['pr_widget_year'];
  if ((!is_numeric($limit) || $limit < 1991 || $limit > date('Y')) && !empty($limit)) {
    form_set_error('pr_widget_year', '<strong>' . t('INVALID YEAR.') . '</strong>');
  }
}

/**
 * Submit form data.
 */
function pr_widget_admin_settings_submit($form, &$form_state) {
  if (module_exists('style_settings')) {
    // Uncollapse the preview. Likely we want to see the changes we just made.
    variable_set('pr_widget_preview_collapsed', FALSE);
    // SELECTABLE MEASUREMENT UNIT: concatenate the value and unit in a new
    // variable (the one that will be used in the CSS).
    variable_set('pr_widget_fontsize', $form_state['values']['pr_widget_fontsize_value'] . $form_state['values']['pr_widget_fontsize_unit']);
    variable_set('pr_widget_radius', $form_state['values']['pr_widget_radius_value'] . $form_state['values']['pr_widget_radius_unit']);
    if ($form_state['values']['pr_widget_reset']) {
      $form_state['values']['pr_widget_fontsize_value'] = '1';
      $form_state['values']['pr_widget_fontsize_unit'] = 'em';
      variable_set('pr_widget_fontsize', '1em');
      $form_state['values']['pr_widget_radius_value'] = '5';
      $form_state['values']['pr_widget_radius_unit'] = 'px';
      variable_set('pr_widget_radius', '5px');
      $form_state['values']['pr_widget_borderwidth'] = '3px';
      $form_state['values']['pr_widget_color'] = 'YellowGreen';
      $form_state['values']['pr_widget_bgcolor'] = 'WhiteSmoke';
      $form_state['values']['pr_widget_shadowcolor'] = 'Black';
      $form_state['values']['pr_widget_fontcolor'] = 'White';
      $form_state['values']['pr_widget_rankcolor'] = 'DimGray';
      drupal_set_message(t("All PageRank widget's CSS values have been set to default."), 'status', FALSE);
    }
    // Make sure changes are visible right after saving the settings.
    _drupal_flush_css_js();
  }
  $url = variable_get('pr_widget_testurl', 0) ? parse_url(variable_get('pr_widget_testurl', 0), PHP_URL_HOST) : parse_url($GLOBALS['base_url'], PHP_URL_HOST);
  variable_set('pr_widget_ratio', pr_widget_pagerank($url));
  // Alternatively get PR from main domain.
  $ratio = variable_get('pr_widget_ratio', pr_widget_pagerank($url));
  if (($ratio == 'NA' || $ratio == '0') && variable_get('pr_widget_pagerank_fallback', 1)) {
    // Get domain from subdomain.
    $url = variable_get('pr_widget_testurl', 0) ? pr_widget_get_domain(variable_get('pr_widget_testurl', 0)) : pr_widget_get_domain($GLOBALS['base_url']);
    $ratio = pr_widget_pagerank($url);
    // If the URL is invalid we won't get a number back anyway.
    if ($ratio >= 1 && $ratio <= 10) {
      variable_set('pr_widget_ratio', $ratio);
    }
  }
  // Set the next time the hook_cron should be invoked.
  $interval = variable_get('pr_widget_interval', 24 * 60 * 60);
  variable_set('pr_widget_next_execution', time() + $interval);
  // Leave a message.
  $message = t('PageRank updated successfully = %pagerank', array('%pagerank' => variable_get('pr_widget_ratio', pr_widget_pagerank($url))));
  drupal_set_message($message, 'status', FALSE);
  watchdog('pr_widget', $message);
}

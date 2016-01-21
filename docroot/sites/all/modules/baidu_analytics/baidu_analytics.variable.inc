<?php

/**
 * @file
 * Definition of variables for Variable API module.
 */

/**
 * Implements hook_variable_info().
 */
function baidu_analytics_variable_info($options) {
  // Baidu Analytics defaults settings.
  $defaults = array(
    'group' => 'baidu_analytics',
    'localize' => TRUE,
    'multidomain' => TRUE,
  );
  // Web Property ID should have exactly 32 lower case hexadecimal characters.
  $variables['baidu_analytics_account'] = array(
    'type' => 'string',
    'title' => t('Web Property ID', array(), $options),
    'default' => '',
    'element' => array(
      '#maxlength' => 32,
      '#size' => 32,
    ),
    'description' => t('This ID is unique to each site you want to track separately, and is in the form of <em>0123456789abcdef0123456789abcdef</em> with exactly 32 lower case hexadecimal characters (only allowed: 0 to 9 and a to f). To get a Web Property ID, <a href="@analytics">register your site with Baidu Analytics</a>, or if you already have registered your site, go to your Baidu Analytics tracker code page to <a href="@screenshot" title="How to find my site\'s Web Property ID">extract the ID inside the Javascript code provided <em>(screenshot)</em></a>. <a href="@webpropertyid">Find more information in the documentation</a>.', array(
      '@analytics' => 'http://tongji.baidu.com/',
      '@webpropertyid' => 'http://tongji.baidu.com/open/api/more?p=guide_overview',
      '@screenshot' => 'https://drupal.org/files/project-images/20130823DO_baidu_analytics_tracking_code_rev1.jpg',
    )),
    'required' => TRUE,
    'validate callback' => 'baidu_analytics_validate_baidu_analytics_account',
  ) + $defaults;

  // Link Tracking specific configurations.
  $variables['baidu_analytics_trackoutbound'] = array(
    'type' => 'boolean',
    'title' => t('Track clicks on outbound links'),
    'default' => TRUE,
  ) + $defaults;
  $variables['baidu_analytics_trackmailto'] = array(
    'type' => 'boolean',
    'title' => t('Track clicks on mailto links'),
    'default' => TRUE,
  ) + $defaults;
  $variables['baidu_analytics_trackfiles'] = array(
    'type' => 'boolean',
    'title' => t('Track downloads (clicks on file links) for the following extensions'),
    'default' => TRUE,
  ) + $defaults;
  $variables['baidu_analytics_trackfiles_extensions'] = array(
    'type' => 'string',
    'title' => t('List of download file extensions'),
    'element' => array(
      '#title_display' => 'invisible',
      '#maxlength' => 255,
    ),
    'default_value' => variable_get('baidu_analytics_trackfiles_extensions', BAIDU_ANALYTICS_TRACKFILES_EXTENSIONS),
    'description' => t('A file extension list separated by the | character that will be tracked as download when clicked. Regular expressions are supported. For example: !extensions', array('!extensions' => BAIDU_ANALYTICS_TRACKFILES_EXTENSIONS)),
  ) + $defaults;

  // Message Tracking specific configurations.
  $variables['baidu_analytics_trackmessages'] = array(
    'type' => 'options',
    'title' => t('Track messages of type'),
    'default' => array(),
    'description' => t('This will track the selected message types shown to users. Tracking of form validation errors may help you identifying usability issues in your site. For each visit (user session), a maximum of approximately 500 combined BATC requests (both events and page views) can be tracked. Every message is tracked as one individual event. Note that - as the number of events in a session approaches the limit - additional events might not be tracked. Messages from excluded pages cannot tracked.'),
    'options' => array(
      'status' => t('Status message'),
      'warning' => t('Warning message'),
      'error' => t('Error message'),
    ),
  ) + $defaults;

  // Privacy specific configurations: donottrack.
  $variables['baidu_analytics_privacy_donottrack'] = array(
    'type' => 'boolean',
    'title' => t('Universal web tracking opt-out'),
    'description' => t('If enabled and your server receives the <a href="@donottrack">Do-Not-Track</a> header from the client browser, the Baidu Analytics module will not embed any tracking code into your site. Compliance with Do Not Track could be purely voluntary, enforced by industry self-regulation, or mandated by state or federal law. Please accept your visitors privacy. If they have opt-out from tracking and advertising, you should accept their personal decision. This feature is currently limited to logged in users and disabled page caching.', array('@donottrack' => 'http://donottrack.us/')),
    'default' => TRUE,
  ) + $defaults;

  // Baidu Analytics Tracking Code (BATC) type.
  $variables['baidu_analytics_code_type'] = array(
    'type' => 'select',
    'title' => t('Select the type of tracking code to insert in the page'),
    'description' => t('Baidu Analytics supports <a href="@screenshot">different types of Javascript code (screenshot)</a> to be added to the page: The <strong>Standard</strong> code (also called <em>Legacy</em> code) and the <strong>Asynchronous</strong> code.<br/>By default, the <em>Asynchronous</em> code is selected since it is recommended for <strong>improved performance</strong> <em>(the page might load faster)</em>.<br/>However, the display of Baidu Analytics small images/logos is only supported with the Standard code type: <strong>small image/logo will not display if the Asynchronous code is selected</strong>.<br/><br/>If <em>Default</em> is selected for the <em>JavaScript scope</em> <em>(field below)</em>, the <em>Asynchronous</em> code would be added to the <em>header</em>, and the <em>Standard</em> code would be added to the <em>footer</em>, as recommended by Baidu Analytics.', array('@screenshot' => 'https://drupal.org/files/project-images/20130823DO_baidu_analytics_tracking_code_rev1.jpg')),
    'options' => array(
      'async' => t('Asynchronous <em>(Recommended)</em>'),
      'standard' => t('Standard'),
    ),
    'default' => 'async',
  ) + $defaults;
  // Enable caching of the Baidu Analytics JavaScript tracker file.
  $variables['baidu_analytics_cache'] = array(
    'type' => 'boolean',
    'title' => t('Locally cache tracking code file'),
    'description' => t("If checked, the tracking code file is retrieved from Baidu Analytics and cached locally. It is updated daily from Baidu's servers to ensure updates to tracking code are reflected in the local copy. Do not activate this until after Baidu Analytics has confirmed that site tracking is working!"),
    'validate callback' => 'baidu_analytics_validate_baidu_analytics_cache',
    'default' => FALSE,
  ) + $defaults;

  // Allow for tracking of the originating node when viewing translation sets.
  if (module_exists('translation')) {
    $variables['baidu_analytics_translation_set'] = array(
      'type' => 'boolean',
      'title' => t('Track translation sets as one unit'),
      'description' => t('When a node is part of a translation set, record statistics for the originating node instead. This allows for a translation set to be treated as a single unit.'),
      'default' => FALSE,
    ) + $defaults;
  }

  // Provide code snippets fields to allow inserting custom JavaScript logic.
  $variables['baidu_analytics_codesnippet_before'] = array(
    'type' => 'text',
    'title' => t('Code snippet (before)'),
    'default' => variable_get('baidu_analytics_codesnippet_before', ''),
    'element' => array(
      '#rows' => 5,
    ),
    'description' => t("Code in this textarea will be added <strong>before</strong> _hmt.push(['_trackPageview'])."),
    'validate callback' => 'baidu_analytics_validate_baidu_analytics_codesnippet',
  ) + $defaults;
  $variables['baidu_analytics_codesnippet_after'] = array(
    '#type' => 'textarea',
    '#title' => t('Code snippet (after)'),
    '#default_value' => variable_get('baidu_analytics_codesnippet_after', ''),
    'element' => array(
      '#rows' => 5,
    ),
    '#description' => t("Code in this textarea will be added <strong>after</strong> _hmt.push(['_trackPageview']). This is useful if you'd like to track a site in two accounts."),
    'validate callback' => 'baidu_analytics_validate_baidu_analytics_codesnippet',
  ) + $defaults;

  // Allow selection of the scope/region in which script should be inserted.
  $js_scope_description_list = theme('item_list', array(
    'items' => array(
      t('<strong>Standard</strong> code in the <strong>footer</strong> of the page right before @body.', array('@body' => "</body>")),
      t('<strong>Asynchronous</strong> code, in the <strong>header</strong> of the page right before @head.', array('@head' => "</head>")),
    ),
  ));
  $variables['baidu_analytics_js_scope'] = array(
    'type' => 'select',
    'title' => t('JavaScript scope'),
    'description' => t('<strong>Default</strong> should be selected to follow Baidu Analytics\' recommended settings:!item_list Feel free to override this setting by selecting a specific scope, such as <em>header</em> or <em>footer</em>, in the dropdown.<br/>For more information, please check <a href="@ba_settings">Baidu Analytics Recommendations</a> or the <a href="@screenshot">different default positions for each code type the code</a>', array(
      '!item_list' => $js_scope_description_list,
      '@ba_settings' => 'http://tongji.baidu.com/open/api/more?p=ref_setAccount',
      '@screenshot' => 'https://drupal.org/files/project-images/20130823DO_baidu_analytics_tracking_code_rev1.jpg',
    )),
    'options' => array(
      'default' => t('Default'),
      'footer' => t('Footer'),
      'header' => t('Header'),
    ),
    'default' => 'default',
  ) + $defaults;

  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function baidu_analytics_variable_group_info() {
  $groups['baidu_analytics'] = array(
    'title' => t('Baidu Analytics'),
    'description' => t('Configure tracking behavior to get insights into your website traffic and marketing effectiveness.'),
    'access' => 'administer baidu analytics',
    'path' => array('admin/config/system/baidu_analytics'),
  );

  return $groups;
}

/**
 * Validate Web Property ID variable.
 */
function baidu_analytics_validate_baidu_analytics_account($variable) {
  // Replace all type of dashes (n-dash, m-dash, minus) with the normal dashes.
  $variable['value'] = str_replace(array('–', '—', '-'), '-', $variable['value']);
  // Ensure the tracker ID contains exactly 32 hexadecimal characters.
  if (!preg_match('/^[a-f0-9]{32}$/', $variable['value'])) {
    return t('A valid Baidu Analytics Web Property ID should have exactly 32 lower case hexadecimal characters (only allowed: 0 to 9 and a to f).');
  }
}

/**
 * Validate callback for the Code Snippet Before and After.
 */
function baidu_analytics_validate_baidu_analytics_codesnippet($variable) {
  // This is for the Newbie's who cannot read a text area description.
  $pattern = '#hm\.baidu\.com/hm?\.js#i';
  // The URLs hm.baidu.com/h.js and hm.baidu.com/hm.js should not be found.
  if (preg_match($pattern, $variable['value'])) {
    form_set_error($variable['name'], t('Do not add the tracker code provided by Baidu into the javascript code snippets! This module already builds the tracker code based on your Baidu Analytics account number and settings.'));
  }
  // Any <script %> markup tags should not be found.
  if (preg_match('/(.*)<\/?script(.*)>(.*)/i', $variable['value'])) {
    form_set_error($variable['name'], t('Do not include the &lt;script&gt; tags in the javascript code snippets.'));
  }
}

/**
 * Validate callback to flush Baidu Analytics JS cache when it is disabled.
 */
function baidu_analytics_validate_baidu_analytics_cache($variable) {
  // Clear obsolete local cache if cache has been disabled.
  if (empty($variable['value']) && variable_get($variable['name'], FALSE)) {
    baidu_analytics_clear_js_cache();
  }
}

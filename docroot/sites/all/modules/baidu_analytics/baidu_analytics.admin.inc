<?php

/**
 * @file
 * Administrative page callbacks for the baidu_analytics module.
 */

/**
 * Menu callback for module's admin settings form page.
 */
function baidu_analytics_admin_settings_form($form_state) {
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );

  // Web Property ID should have exactly 32 lower case hexadecimal characters.
  $form['account']['baidu_analytics_account'] = array(
    '#title' => t('Web Property ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('baidu_analytics_account', ''),
    '#size' => 32,
    '#maxlength' => 32,
    '#required' => TRUE,
    '#description' => t('This ID is unique to each site you want to track separately, and is in the form of <em>0123456789abcdef0123456789abcdef</em> with exactly 32 lower case hexadecimal characters (only allowed: 0 to 9 and a to f). To get a Web Property ID, <a href="@analytics">register your site with Baidu Analytics</a>, or if you already have registered your site, go to your Baidu Analytics tracker code page to <a href="@screenshot" title="How to find my site\'s Web Property ID">extract the ID inside the Javascript code provided <em>(screenshot)</em></a>. <a href="@webpropertyid">Find more information in the documentation</a>.', array(
      '@analytics' => 'http://tongji.baidu.com/',
      '@webpropertyid' => 'http://tongji.baidu.com/open/api/more?p=guide_overview',
      '@screenshot' => 'https://drupal.org/files/project-images/20130823DO_baidu_analytics_tracking_code_rev1.jpg',
    )),
  );

  // Visibility settings.
  $form['tracking_title'] = array(
    '#type' => 'item',
    '#title' => t('Tracking scope'),
  );
  $form['tracking'] = array(
    '#type' => 'vertical_tabs',
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'baidu_analytics') . '/baidu_analytics.admin.js'),
    ),
  );

  // Page specific visibility configurations.
  $php_access = user_access('use PHP for tracking visibility');
  $visibility = variable_get('baidu_analytics_visibility_pages', 0);
  $pages = variable_get('baidu_analytics_pages', BAIDU_ANALYTICS_PAGES);

  $form['tracking']['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  if ($visibility == 2 && !$php_access) {
    $form['tracking']['page_vis_settings'] = array();
    $form['tracking']['page_vis_settings']['visibility'] = array('#type' => 'value', '#value' => 2);
    $form['tracking']['page_vis_settings']['pages'] = array('#type' => 'value', '#value' => $pages);
  }
  else {
    $options = array(
      t('Every page except the listed pages'),
      t('The listed pages only'),
    );
    $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array(
      '%blog' => 'blog',
      '%blog-wildcard' => 'blog/*',
      '%front' => '<front>',
    ));

    if (module_exists('php') && $php_access) {
      $options[] = t('Pages on which this PHP code returns <code>TRUE</code> (experts only)');
      $form['tracking']['page_vis_settings']['#title'] = t('Pages or PHP code');
      $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    // Pages visibility settings exclude/include paths or PHP code.
    $form['tracking']['page_vis_settings']['baidu_analytics_visibility_pages'] = array(
      '#type' => 'radios',
      '#title' => t('Add tracking to specific pages'),
      '#options' => $options,
      '#default_value' => $visibility,
    );
    // Pages visibility textarea could receive either paths or PHP code.
    $form['tracking']['page_vis_settings']['baidu_analytics_pages'] = array(
      '#type' => 'textarea',
      '#title' => $form['tracking']['page_vis_settings']['#title'],
      '#title_display' => 'invisible',
      '#default_value' => $pages,
      '#description' => $description,
      '#rows' => 10,
    );
  }

  // Render the role overview.
  $form['tracking']['role_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
  );

  $form['tracking']['role_vis_settings']['baidu_analytics_visibility_roles'] = array(
    '#type' => 'radios',
    '#title' => t('Add tracking for specific roles'),
    '#options' => array(
      t('Add to the selected roles only'),
      t('Add to every role except the selected ones'),
    ),
    '#default_value' => variable_get('baidu_analytics_visibility_roles', 0),
  );

  $role_options = array_map('check_plain', user_roles());
  $form['tracking']['role_vis_settings']['baidu_analytics_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => variable_get('baidu_analytics_roles', array()),
    '#options' => $role_options,
    '#description' => t('If none of the roles are selected, all users will be tracked. If a user has any of the roles checked, that user will be tracked (or excluded, depending on the setting above).'),
  );

  // Standard tracking configurations.
  $form['tracking']['user_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Users'),
  );
  $t_permission = array('%permission' => t('opt-in or out of tracking'));
  $form['tracking']['user_vis_settings']['baidu_analytics_custom'] = array(
    '#type' => 'radios',
    '#title' => t('Allow users to customize tracking on their account page'),
    '#options' => array(
      t('No customization allowed'),
      t('Tracking on by default, users with %permission permission can opt out', $t_permission),
      t('Tracking off by default, users with %permission permission can opt in', $t_permission),
    ),
    '#default_value' => variable_get('baidu_analytics_custom', 0),
  );

  // Link Tracking specific configurations.
  $form['tracking']['linktracking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links and downloads'),
  );
  $form['tracking']['linktracking']['baidu_analytics_trackoutbound'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track clicks on outbound links'),
    '#default_value' => variable_get('baidu_analytics_trackoutbound', TRUE),
  );
  $form['tracking']['linktracking']['baidu_analytics_trackmailto'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track clicks on mailto links'),
    '#default_value' => variable_get('baidu_analytics_trackmailto', TRUE),
  );
  $form['tracking']['linktracking']['baidu_analytics_trackfiles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track downloads (clicks on file links) for the following extensions'),
    '#default_value' => variable_get('baidu_analytics_trackfiles', TRUE),
  );
  $form['tracking']['linktracking']['baidu_analytics_trackfiles_extensions'] = array(
    '#title' => t('List of download file extensions'),
    '#title_display' => 'invisible',
    '#type' => 'textfield',
    '#default_value' => variable_get('baidu_analytics_trackfiles_extensions', BAIDU_ANALYTICS_TRACKFILES_EXTENSIONS),
    '#description' => t('A file extension list separated by the | character that will be tracked as download when clicked. Regular expressions are supported. For example: !extensions', array('!extensions' => BAIDU_ANALYTICS_TRACKFILES_EXTENSIONS)),
    '#maxlength' => 255,
  );

  // Message Tracking specific configurations.
  $form['tracking']['messagetracking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Messages'),
  );
  $form['tracking']['messagetracking']['baidu_analytics_trackmessages'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Track messages of type'),
    '#default_value' => variable_get('baidu_analytics_trackmessages', array()),
    '#description' => t('This will track the selected message types shown to users. Tracking of form validation errors may help you identifying usability issues in your site. For each visit (user session), a maximum of approximately 500 combined BATC requests (both events and page views) can be tracked. Every message is tracked as one individual event. Note that - as the number of events in a session approaches the limit - additional events might not be tracked. Messages from excluded pages cannot tracked.'),
    '#options' => array(
      'status' => t('Status message'),
      'warning' => t('Warning message'),
      'error' => t('Error message'),
    ),
  );

  // Baidu Analytics already has many translations, otherwise a note is
  // displayed to change the language.
  $form['tracking']['search_and_advertising'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search and Advertising'),
  );

  // Search and Advertising configuration.
  $site_search_dependencies = '<div class="admin-requirements">';
  $site_search_dependencies .= t('Requires: !module-list', array(
    '!module-list' => (module_exists('search') ? t('@module (<span class="admin-enabled">enabled</span>)', array('@module' => 'Search')) : t('@module (<span class="admin-disabled">disabled</span>)', array('@module' => 'Search'))),
  ));
  $site_search_dependencies .= '</div>';

  $form['tracking']['search_and_advertising']['baidu_analytics_site_search'] = array(
    '#type' => 'checkbox',
    '#title' => t('Track internal search'),
    '#description' => t('If checked, internal search keywords are tracked with search results page urls. The query keywords and total items count are added as parameters to the search results page URL being tracked.') . $site_search_dependencies,
    '#default_value' => variable_get('baidu_analytics_site_search', FALSE),
    '#disabled' => (module_exists('search') ? FALSE : TRUE),
  );

  // Privacy specific configurations.
  $form['tracking']['privacy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Privacy'),
  );
  $form['tracking']['privacy']['baidu_analytics_privacy_donottrack'] = array(
    '#type' => 'checkbox',
    '#title' => t('Universal web tracking opt-out'),
    '#description' => t('If enabled and your server receives the <a href="@donottrack">Do-Not-Track</a> header from the client browser, the Baidu Analytics module will not embed any tracking code into your site. Compliance with Do Not Track could be purely voluntary, enforced by industry self-regulation, or mandated by state or federal law. Please accept your visitors privacy. If they have opt-out from tracking and advertising, you should accept their personal decision. This feature is currently limited to logged in users and disabled page caching.', array('@donottrack' => 'http://donottrack.us/')),
    '#default_value' => variable_get('baidu_analytics_privacy_donottrack', TRUE),
  );

  // Custom variables.
  $form['baidu_analytics_custom_var'] = array(
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
    '#description' => t('You can add Baidu Analytics <a href="@custom_var_documentation">Custom Variables</a> here. These will be added to every page that Baidu Analytics tracking code appears on. Baidu Analytics will only accept custom variables if the <em>name</em> and <em>value</em> combined are less than 128 bytes after URL encoding. Keep the names as short as possible and expect long values to get trimmed. You may use tokens in custom variable names and values. Global and user tokens are always available; on node pages, node tokens are also available.', array('@custom_var_documentation' => 'http://tongji.baidu.com/open/api/more?p=guide_setCustomVar')),
    '#theme' => 'baidu_analytics_admin_custom_var_table',
    '#title' => t('Custom variables'),
    '#tree' => TRUE,
    '#type' => 'fieldset',
  );

  $baidu_analytics_custom_vars = variable_get('baidu_analytics_custom_var', array());

  // Baidu Analytics supports up to 5 custom variables.
  for ($i = 1; $i < 6; $i++) {
    $form['baidu_analytics_custom_var']['slots'][$i]['slot'] = array(
      '#default_value' => $i,
      '#description' => t('Slot number'),
      '#disabled' => TRUE,
      '#size' => 1,
      '#title' => t('Custom variable slot #@slot', array('@slot' => $i)),
      '#title_display' => 'invisible',
      '#type' => 'textfield',
    );
    $form['baidu_analytics_custom_var']['slots'][$i]['name'] = array(
      '#default_value' => !empty($baidu_analytics_custom_vars['slots'][$i]['name']) ? $baidu_analytics_custom_vars['slots'][$i]['name'] : '',
      '#description' => t('The custom variable name.'),
      '#maxlength' => 255,
      '#size' => 20,
      '#title' => t('Custom variable name #@slot', array('@slot' => $i)),
      '#title_display' => 'invisible',
      '#type' => 'textfield',
      '#element_validate' => array('baidu_analytics_token_element_validate'),
      '#token_types' => array('node'),
    );
    $form['baidu_analytics_custom_var']['slots'][$i]['value'] = array(
      '#default_value' => !empty($baidu_analytics_custom_vars['slots'][$i]['value']) ? $baidu_analytics_custom_vars['slots'][$i]['value'] : '',
      '#description' => t('The custom variable value.'),
      '#maxlength' => 255,
      '#title' => t('Custom variable value #@slot', array('@slot' => $i)),
      '#title_display' => 'invisible',
      '#type' => 'textfield',
      '#element_validate' => array('baidu_analytics_token_element_validate'),
      '#token_types' => array('node'),
    );
    if (module_exists('token')) {
      $form['baidu_analytics_custom_var']['slots'][$i]['name']['#element_validate'][] = 'token_element_validate';
      $form['baidu_analytics_custom_var']['slots'][$i]['value']['#element_validate'][] = 'token_element_validate';
    }
    $form['baidu_analytics_custom_var']['slots'][$i]['scope'] = array(
      '#default_value' => !empty($baidu_analytics_custom_vars['slots'][$i]['scope']) ? $baidu_analytics_custom_vars['slots'][$i]['scope'] : 3,
      '#description' => t('The scope for the custom variable.'),
      '#title' => t('Custom variable slot #@slot', array('@slot' => $i)),
      '#title_display' => 'invisible',
      '#type' => 'select',
      '#options' => array(
        1 => t('Visitor'),
        2 => t('Session'),
        3 => t('Page'),
      ),
    );
  }

  $form['baidu_analytics_custom_var']['baidu_analytics_custom_var_description'] = array(
    '#type' => 'item',
    '#description' => t('You can supplement Baidu Analytics\' basic IP address tracking of visitors by segmenting users based on custom variables. <a href="@ba_tos">Baidu Analytics terms of service</a> requires that You will not (and will not allow any third party to) use the Service to track, collect or upload any data that personally identifies an individual (such as a name, email address or billing information), or other data which can be reasonably linked to such information by Baidu. You will have and abide by an appropriate Privacy Policy and will comply with all applicable laws and regulations relating to the collection of information from Visitors. You must post a Privacy Policy and that Privacy Policy must provide notice of Your use of cookies that are used to collect traffic data, and You must not circumvent any privacy features (e.g., an opt-out) that are part of the Service.', array('@ba_tos' => 'http://www.baidu.com/duty/')),
  );
  $form['baidu_analytics_custom_var']['baidu_analytics_custom_var_token_tree'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('node'),
    '#dialog' => TRUE,
  );

  // Advanced feature configurations.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Baidu Analytics Tracking Code (BATC) type.
  $form['advanced']['baidu_analytics_code_type'] = array(
    '#type' => 'radios',
    '#title' => t('Select the type of tracking code to insert in the page'),
    '#description' => t('Baidu Analytics supports <a href="@screenshot">different types of Javascript code (screenshot)</a> to be added to the page: The <strong>Standard</strong> code (also called <em>Legacy</em> code) and the <strong>Asynchronous</strong> code.<br/>By default, the <em>Asynchronous</em> code is selected since it is recommended for <strong>improved performance</strong> <em>(the page might load faster)</em>.<br/>However, the display of Baidu Analytics small images/logos is only supported with the Standard code type: <strong>small image/logo will not display if the Asynchronous code is selected</strong>.<br/><br/>If <em>Default</em> is selected for the <em>JavaScript scope</em> <em>(field below)</em>, the <em>Asynchronous</em> code would be added to the <em>header</em>, and the <em>Standard</em> code would be added to the <em>footer</em>, as recommended by Baidu Analytics.', array('@screenshot' => 'https://drupal.org/files/project-images/20130823DO_baidu_analytics_tracking_code_rev1.jpg')),
    '#options' => array(
      'async' => t('Asynchronous <em>(Recommended)</em>'),
      'standard' => t('Standard'),
    ),
    '#default_value' => variable_get('baidu_analytics_code_type', 'async'),
  );

  // Enable caching of the Baidu Analytics JavaScript tracker file.
  $form['advanced']['baidu_analytics_cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Locally cache tracking code file'),
    '#description' => t("If checked, the tracking code file is retrieved from Baidu Analytics and cached locally. It is updated daily from Baidu's servers to ensure updates to tracking code are reflected in the local copy. Do not activate this until after Baidu Analytics has confirmed that site tracking is working!"),
    '#default_value' => variable_get('baidu_analytics_cache', FALSE),
  );

  // Allow for tracking of the originating node when viewing translation sets.
  if (module_exists('translation')) {
    $form['advanced']['baidu_analytics_translation_set'] = array(
      '#type' => 'checkbox',
      '#title' => t('Track translation sets as one unit'),
      '#description' => t('When a node is part of a translation set, record statistics for the originating node instead. This allows for a translation set to be treated as a single unit.'),
      '#default_value' => variable_get('baidu_analytics_translation_set', FALSE),
    );
  }

  // Provide code snippets fields to allow inserting custom JavaScript logic.
  $form['advanced']['codesnippet'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom JavaScript code'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('You can add custom Baidu Analytics <a href="@snippets">code snippets</a> here. These will be added every time tracking is in effect. Before you add your custom code, you should read the <a href="@ba_concepts_overview">Baidu Analytics Tracking Code - Functional Overview</a> and the <a href="@ba_js_api">Baidu Analytics Tracking API</a> documentation. <strong>Do not include the &lt;script&gt; tags</strong>, and always end your code with a semicolon (;).', array(
      '@snippets' => 'https://drupal.org/node/2076741',
      '@ba_concepts_overview' => 'http://tongji.baidu.com/open/api/more?p=ref_trackEvent',
      '@ba_js_api' => 'http://tongji.baidu.com/open/api/',
    )),
  );
  $form['advanced']['codesnippet']['baidu_analytics_codesnippet_before'] = array(
    '#type' => 'textarea',
    '#title' => t('Code snippet (before)'),
    '#default_value' => variable_get('baidu_analytics_codesnippet_before', ''),
    '#rows' => 5,
    '#description' => t("Code in this textarea will be added <strong>before</strong> _hmt.push(['_trackPageview'])."),
  );
  $form['advanced']['codesnippet']['baidu_analytics_codesnippet_after'] = array(
    '#type' => 'textarea',
    '#title' => t('Code snippet (after)'),
    '#default_value' => variable_get('baidu_analytics_codesnippet_after', ''),
    '#rows' => 5,
    '#description' => t("Code in this textarea will be added <strong>after</strong> _hmt.push(['_trackPageview']). This is useful if you'd like to track a site in two accounts."),
  );

  // Allow selection of the scope/region in which script should be inserted.
  $js_scope_description_list = theme('item_list', array(
    'items' => array(
      t('<strong>Standard</strong> code in the <strong>footer</strong> of the page right before @body.', array('@body' => "</body>")),
      t('<strong>Asynchronous</strong> code, in the <strong>header</strong> of the page right before @head.', array('@head' => "</head>")),
    ),
  ));
  $form['advanced']['baidu_analytics_js_scope'] = array(
    '#type' => 'select',
    '#title' => t('JavaScript scope'),
    '#description' => t('<strong>Default</strong> should be selected to follow Baidu Analytics\' recommended settings:!item_list Feel free to override this setting by selecting a specific scope, such as <em>header</em> or <em>footer</em>, in the dropdown.<br/>For more information, please check <a href="@ba_settings">Baidu Analytics Recommendations</a> or the <a href="@screenshot">different recommended positions for each type of code</a>.', array(
      '!item_list' => $js_scope_description_list,
      '@ba_settings' => 'http://tongji.baidu.com/open/api/more?p=ref_setAccount',
      '@screenshot' => 'https://drupal.org/files/project-images/20130823DO_baidu_analytics_tracking_code_rev1.jpg',
    )),
    '#options' => array(
      'default' => t('Default'),
      'footer' => t('Footer'),
      'header' => t('Header'),
    ),
    '#default_value' => variable_get('baidu_analytics_js_scope', 'default'),
  );

  return system_settings_form($form);
}

/**
 * Implements _form_validate().
 */
function baidu_analytics_admin_settings_form_validate($form, &$form_state) {
  // Custom variables validation.
  foreach ($form_state['values']['baidu_analytics_custom_var']['slots'] as $custom_var) {
    $form_state['values']['baidu_analytics_custom_var']['slots'][$custom_var['slot']]['name'] = trim($custom_var['name']);
    $form_state['values']['baidu_analytics_custom_var']['slots'][$custom_var['slot']]['value'] = trim($custom_var['value']);

    // Validate empty names/values.
    if (empty($custom_var['name']) && !empty($custom_var['value'])) {
      form_set_error("baidu_analytics_custom_var][slots][" . $custom_var['slot'] . "][name", t('The custom variable @slot-number requires a <em>Name</em> if a <em>Value</em> has been provided.', array('@slot-number' => $custom_var['slot'])));
    }
    elseif (!empty($custom_var['name']) && empty($custom_var['value'])) {
      form_set_error("baidu_analytics_custom_var][slots][" . $custom_var['slot'] . "][value", t('The custom variable @slot-number requires a <em>Value</em> if a <em>Name</em> has been provided.', array('@slot-number' => $custom_var['slot'])));
    }
  }

  // Trim some text values.
  $form_state['values']['baidu_analytics_account'] = trim($form_state['values']['baidu_analytics_account']);
  $form_state['values']['baidu_analytics_pages'] = trim($form_state['values']['baidu_analytics_pages']);
  $form_state['values']['baidu_analytics_codesnippet_before'] = trim($form_state['values']['baidu_analytics_codesnippet_before']);
  $form_state['values']['baidu_analytics_codesnippet_after'] = trim($form_state['values']['baidu_analytics_codesnippet_after']);

  // Replace all type of dashes (n-dash, m-dash, minus) with the normal dashes.
  $form_state['values']['baidu_analytics_account'] = str_replace(array(
    '–',
    '—',
    '−',
  ), '-', $form_state['values']['baidu_analytics_account']);
  // Ensure the tracker ID contains exactly 32 hexadecimal characters.
  if (!preg_match('/^[a-f0-9]{32}$/', $form_state['values']['baidu_analytics_account'])) {
    form_set_error('baidu_analytics_account', t('A valid Baidu Analytics Web Property ID should have exactly 32 lower case hexadecimal characters (only allowed: 0 to 9 and a to f).'));
  }

  // Clear obsolete local cache if cache has been disabled.
  if (empty($form_state['values']['baidu_analytics_cache']) && $form['advanced']['baidu_analytics_cache']['#default_value']) {
    baidu_analytics_clear_js_cache();
  }

  // This is for the Newbie's who cannot read a text area description.
  $pattern = '#hm\.baidu\.com/hm?\.js#i';
  // The URLs hm.baidu.com/h.js and hm.baidu.com/hm.js should not be found.
  if (preg_match($pattern, $form_state['values']['baidu_analytics_codesnippet_before'])) {
    form_set_error('baidu_analytics_codesnippet_before', t('Do not add the tracker code provided by Baidu into the javascript code snippets! This module already builds the tracker code based on your Baidu Analytics account number and settings.'));
  }
  if (preg_match($pattern, $form_state['values']['baidu_analytics_codesnippet_after'])) {
    form_set_error('baidu_analytics_codesnippet_after', t('Do not add the tracker code provided by Baidu into the javascript code snippets! This module already builds the tracker code based on your Baidu Analytics account number and settings.'));
  }
  // Any <script %> markup tags should not be found.
  if (preg_match('/(.*)<\/?script(.*)>(.*)/i', $form_state['values']['baidu_analytics_codesnippet_before'])) {
    form_set_error('baidu_analytics_codesnippet_before', t('Do not include the &lt;script&gt; tags in the javascript code snippets.'));
  }
  if (preg_match('/(.*)<\/?script(.*)>(.*)/i', $form_state['values']['baidu_analytics_codesnippet_after'])) {
    form_set_error('baidu_analytics_codesnippet_after', t('Do not include the &lt;script&gt; tags in the javascript code snippets.'));
  }
}

/**
 * Layout for the custom variables table in the admin settings form.
 */
function theme_baidu_analytics_admin_custom_var_table($variables) {
  $form = $variables['form'];

  $header = array(
    array('data' => t('Slot')),
    array('data' => t('Name')),
    array('data' => t('Value')),
    array('data' => t('Scope')),
  );

  $rows = array();
  foreach (element_children($form['slots']) as $id) {
    $rows[] = array(
      'data' => array(
        drupal_render($form['slots'][$id]['slot']),
        drupal_render($form['slots'][$id]['name']),
        drupal_render($form['slots'][$id]['value']),
        drupal_render($form['slots'][$id]['scope']),
      ),
    );
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render($form['baidu_analytics_custom_var_description']);
  $output .= drupal_render($form['baidu_analytics_custom_var_token_tree']);

  return $output;
}

/**
 * Validate a form element that should have tokens in it.
 *
 * For example:
 * @code
 * $form['my_node_text_element'] = array(
 *   '#type' => 'textfield',
 *   '#title' => t('Some text to token-ize that has a node context.'),
 *   '#default_value' => 'The title of this node is [node:title].',
 *   '#element_validate' => array('baidu_analytics_token_element_validate'),
 * );
 * @endcode
 */
function baidu_analytics_token_element_validate(&$element, &$form_state) {
  $value = isset($element['#value']) ? $element['#value'] : $element['#default_value'];

  if (!drupal_strlen($value)) {
    // Empty value needs no further validation since the element should depend
    // on using the '#required' FAPI property.
    return $element;
  }

  $tokens = token_scan($value);
  $invalid_tokens = _baidu_analytics_get_forbidden_tokens($tokens);
  if ($invalid_tokens) {
    form_error($element, t('The %element-title is using the following forbidden tokens with personal identifying information: @invalid-tokens.', array(
      '%element-title' => $element['#title'],
      '@invalid-tokens' => implode(', ', $invalid_tokens),
    )));
  }

  return $element;
}

/**
 * Helper function to return a list of tokens that should not be used.
 */
function _baidu_analytics_get_forbidden_tokens($value) {
  $invalid_tokens = array();
  $value_tokens = is_string($value) ? token_scan($value) : $value;

  foreach ($value_tokens as $tokens) {
    if (array_filter($tokens, '_baidu_analytics_contains_forbidden_token')) {
      $invalid_tokens = array_merge($invalid_tokens, array_values($tokens));
    }
  }

  array_unique($invalid_tokens);
  return $invalid_tokens;
}

/**
 * Validate if a string contains forbidden tokens not allowed by privacy rules.
 *
 * @param string $token_string
 *   A string with one or more tokens to be validated.
 *
 * @return bool
 *   TRUE if blacklisted token has been found, otherwise FALSE.
 */
function _baidu_analytics_contains_forbidden_token($token_string) {
  // List of strings in tokens with personal identifying information not allowed
  // for privacy reasons. See section 8.1 of the Baidu Analytics terms of use
  // for more detailed information.
  //
  // This list can never ever be complete. For this reason it tries to use a
  // regex and may kill a few other valid tokens, but it's the only way to
  // protect users as much as possible from admins with illegal ideas.
  //
  // User tokens are not prefixed with colon to catch 'current-user' and 'user'.
  //
  // TODO: If someone have better ideas, share them, please!
  $token_blacklist = array(
    ':author]',
    ':author:edit-url]',
    ':author:url]',
    ':author:path]',
    ':current-user]',
    ':current-user:original]',
    ':fid]',
    ':mail]',
    ':name]',
    ':uid]',
    ':one-time-login-url]',
    ':owner]',
    ':owner:cancel-url]',
    ':owner:edit-url]',
    ':owner:url]',
    ':owner:path]',
    'user:cancel-url]',
    'user:edit-url]',
    'user:url]',
    'user:path]',
    'user:picture]',
    // addressfield_tokens.module
    ':first-name]',
    ':last-name]',
    ':name-line]',
    ':mc-address]',
    ':thoroughfare]',
    ':premise]',
    // realname.module
    ':name-raw]',
    // token.module
    ':ip-address]',
  );

  return preg_match('/' . implode('|', array_map('preg_quote', $token_blacklist)) . '/i', $token_string);
}

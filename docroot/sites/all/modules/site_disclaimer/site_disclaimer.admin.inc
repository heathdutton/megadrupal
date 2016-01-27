<?php

/**
 * @file
 * Administration settings for Site Disclaimer module.
 */

define('SITE_DISCLAIMER_NODE_TITLE', t('I prefer to specify the title of the post'));
define('SITE_DISCLAIMER_NODE_ID', t('I prefer to specify the node id'));

/**
 * Get an associative array keyed by langcode of all checkbox label translations.
 */
function _site_disclaimer_checkbox_label_all_translations() {
  $ret = array();
  if (module_exists('i18n_variable')) {
    $languages = language_list();
    $value_und = i18n_variable_get('site_disclaimer_checkbox_label', 'und', '');
    foreach ($languages as $langcode => $language) {
      $value = i18n_variable_get('site_disclaimer_checkbox_label', $langcode, $value_und);
      if (!empty($value)) {
        $ret[$langcode] = filter_xss_admin($value);
      }
    }
  }
  else {
    global $language;
    $ret[$language->language] = filter_xss_admin(variable_get('site_disclaimer_checkbox_label', SITE_DISCLAIMER_DEFAULT_CHECKBOX_LABEL));
  }
  return $ret;
}

/**
 * Multiple-use function to process settings for:
 * - initial settings form build
 * - settings form validation
 * - settings form rebuild (as part of '#ajax' process)
 */
function _site_disclaimer_admin_decode_settings(&$form_state, $validate = FALSE) {
  $form_state += array('values' => array());
  $is_form_submitted = !empty($form_state['values']['op']);

  $orig_mode = isset($form_state['#site_disclaimer_mode']) ? $form_state['#site_disclaimer_mode']
    : (isset($form_state['values']['site_disclaimer_node_id']) ? 'node_id' : 'node_title');
  $mode = $orig_mode;
  $form_state['#site_disclaimer_mode_switch'] = FALSE;
  if ($is_form_submitted) {
    if ($form_state['values']['op'] == SITE_DISCLAIMER_NODE_ID) {
      $mode = 'node_id';
      $form_state['#site_disclaimer_mode_switch'] = $mode;
    }
    elseif ($form_state['values']['op'] == SITE_DISCLAIMER_NODE_TITLE) {
      $mode = 'node_title';
      $form_state['#site_disclaimer_mode_switch'] = $mode;
    }
    if (!$validate) {
      // Ignore the switch while validating
      $mode = $orig_mode;
    }
  }
  if (!$is_form_submitted && !$validate) {
    // Start the form in preferred node title mode
    $mode = 'node_title';
  }
  $form_state['#site_disclaimer_mode'] = $mode;

  $title = '';
  $nid = '';
  if ($orig_mode == 'node_title') {
    if (!$is_form_submitted) {
      $title = variable_get('site_disclaimer_node_title', '');
    }
    elseif (!empty($form_state['values']['site_disclaimer_node_title'])) {
      $title = $form_state['values']['site_disclaimer_node_title'];
    }
    if (!empty($title)) {
      $nodes = node_load_multiple(array(), array('title' => $title));
      if (empty($nodes)) {
        if ($is_form_submitted) {
          form_set_error('site_disclaimer_node_title', t('No post was found with "!title" title.', array('!title' => $title)));
        }
      }
      else {
        $node = array_shift($nodes);
        $nid = $node->nid;
      }
    }
  }
  else {
    // $orig_mode == 'node_id'
    if (!$is_form_submitted) {
      $nid = variable_get('site_disclaimer_node_id', '');
    }
    elseif (!empty($form_state['values']['site_disclaimer_node_id'])) {
      $nid = $form_state['values']['site_disclaimer_node_id'];
    }
    if (!empty($nid)) {
      $node = node_load($nid);
      if (empty($node)) {
        if ($is_form_submitted) {
          form_set_error('site_disclaimer_node_id', t('No post was found with nid !nid.', array('!nid' => $nid)));
        }
      }
      else {
        $title = $node->title;
      }
    }
  }
  $form_state['values']['site_disclaimer_node_title'] = $title;
  $form_state['values']['site_disclaimer_node_id'] = $nid;
  $form_state['#site_disclaimer_nid'] = $nid;
}

/**
 * Menu callback; show settings form.
 * @see site_disclaimer_admin_settings_validate()
 *
 */
function site_disclaimer_admin_settings($form, &$form_state) {
  $langcode = NULL;
  // Get the language of the multiplingual form to show preview in that language
  if (module_exists('variable_realm')) {
    module_load_include('form.inc', 'variable_realm');
    $langcode = variable_realm_form_key_current('language');
  }

  _site_disclaimer_admin_decode_settings($form_state);

  // Adding the fieldset for node specification.
  $form['site_disclaimer_text'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<div id="fieldset-wrapper">',
    '#suffix' => '</div>',
    '#title' => t('Node to include with Site Disclaimer'),
    '#description' => t('This node typically contains legal statements such as Site Disclaimer, Terms of Use, etc. It is either included entirely in the Site Disclaimer form, or linked from the [x] ckeckbox label (if @link token is used). The node should already exist.'),
  );
  if ($form_state['#site_disclaimer_mode'] == 'node_title') {
    $form['site_disclaimer_text']['site_disclaimer_node_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Node title'),
      '#default_value' => $form_state['values']['site_disclaimer_node_title'],
      '#description' => t('Enter the <em>title</em> of an existing node. Leave empty and also use no token in the [x] checkbox label below to disable Site Disclaimer.'),
      '#autocomplete_path' => 'site_disclaimer/autocomplete',
    );
  }
  else {
    $form['site_disclaimer_text']['site_disclaimer_node_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Node id'),
      '#default_value' => $form_state['values']['site_disclaimer_node_id'],
      '#description' => t('Enter the <em>id</em> of an existing node. Leave empty and also use no token in the [x] checkbox label below to disable Site Disclaimer.'),
    );
  }
  // D7 AHAH is happy when we change '#value' of existing button (in D6 this does not work).
  $form['site_disclaimer_text']['site_disclaimer_pick_node_id'] = array(
    '#type' => 'button',
    '#weight' => 10,
    '#value' => ($form_state['#site_disclaimer_mode'] == 'node_title') ?  SITE_DISCLAIMER_NODE_ID : SITE_DISCLAIMER_NODE_TITLE,
    '#ajax' => array(
      'callback' => 'site_disclaimer_js',
      'wrapper' => 'fieldset-wrapper',
    ),
  );

  // Adding the fieldset for form specification.
  $form['site_disclaimer_form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site Disclaimer form settings'),
  );
  $form['site_disclaimer_form']['site_disclaimer_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($form_state['values']['site_disclaimer_title']) ? $form_state['values']['site_disclaimer_title'] : variable_get('site_disclaimer_title', SITE_DISCLAIMER_DEFAULT_TITLE),
    '#description' => t('Enter a title to put on the Site Disclaimer form.'),
  );
  $form['site_disclaimer_form']['site_disclaimer_fieldset'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enclose in a fieldset'),
    '#default_value' => isset($form_state['values']['site_disclaimer_fieldset']) ? $form_state['values']['site_disclaimer_fieldset'] : variable_get('site_disclaimer_fieldset', 1),
    '#description' => t('Enclose the Site Disclaimer form and the [x] checkbox in a fieldset. Helps visually group the form as typically themes display fieldset in a box / add borders.'),
  );
  $form['site_disclaimer_form']['site_disclaimer_checkbox_label'] = array(
    '#type' => 'textfield',
    '#maxlength' => 256, // Increase max length to allow lengthier fields (Site Disclaimer can be wordy)
    '#title' => t('[x] checkbox label'),
    '#default_value' => isset($form_state['values']['site_disclaimer_checkbox_label']) ? $form_state['values']['site_disclaimer_checkbox_label'] : variable_get('site_disclaimer_checkbox_label', SITE_DISCLAIMER_DEFAULT_CHECKBOX_LABEL),
    '#description' => t('Enter a Site Disclaimer statement that visitors have to accept, something like <b>I agree with these terms</b>, or <b>I CERTIFY THAT I AM OVER THE AGE OF 18 YEARS OLD</b>.!tokens!example<p>You may want to link to the Terms page if you prefer not to show the full text of the Terms in the registration form.</p><p>If you use any tokens, the full text of the Terms will not be shown.</p>',
      array(
        '!tokens' => t('<p>The following tokens are substituted on the label:</p><ul><li>@link - a link to the Site Disclaimer node (must be provided above).</li><li>@"Post Title" - a link to a post with title <b>Post Title</b>. If post title contains a double quote, prepend it with a backslash \".</li></ul>'),
        '!example' => t('<p>For example, the label can be: "I agree with the @link", without quotes.</p>'),
      )
    ),
  );
  $form['site_disclaimer_form']['site_disclaimer_node_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Max height'),
    '#default_value' => isset($form_state['values']['site_disclaimer_node_height']) ? $form_state['values']['site_disclaimer_node_height'] : variable_get('site_disclaimer_node_height', ''),
    '#description' => t('Leave empty to show Site Disclaimer form with text in full height. Enter something like <b>20em</b> or <b>120px</b> to set the height of the Site Disclaimer form. Vertical scrollbar is added automatically if text height exceeds the max height setting.'),
  );

  $form['site_disclaimer_advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site Disclaimer version'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $version = variable_get('site_disclaimer_version', 1);
  $options = array(
    $version => t('@version (current version)', array('@version' => $version)),
    $version + 1 => t('@version (add a new version)', array('@version' => $version + 1)),
  );

  $form['site_disclaimer_advanced']['site_disclaimer_version'] = array(
    '#type' => 'radios',
    '#title' => t('Add new version'),
    '#description' => t('WARNING: Changing Site Disclaimer version is not reversible. It will require every already registered user to accept the new Site Disclaimer terms upon their login before they can do anything else on the website.'),
    '#options' => $options,
    '#default_value' => isset($form_state['values']['site_disclaimer_version']) ? $form_state['values']['site_disclaimer_version'] : $version,
  );

  $form['site_disclaimer_advanced']['site_disclaimer_version_details'] = array(
    '#type' => 'textarea',
    '#title' => t('Changes'),
    '#description' => t('Describe changes in the latest version. Note: if you are not changing version, this description will be shown to all users who have not accepted the current version.'),
    '#default_value' => isset($form_state['values']['site_disclaimer_version_details']) ? $form_state['values']['site_disclaimer_version_details'] : variable_get('site_disclaimer_version_details', ''),
  );

  //Show a preview of Site Disclaimer forms
  $form['site_disclaimer_preview'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preview'),
    '#description' => t('This is a preview of current configuration. To see how your changes will look, you have to save configuration.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['site_disclaimer_preview']['user_register'] = array(
    '#type' => 'fieldset',
    '#title' => t('User registration page'),
    '#description' => '',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $preview_form = array();
  $form_state_tmp = array();
  site_disclaimer_form_user_register_form_alter($preview_form, $form_state_tmp, 'form_user_register', $langcode, TRUE);
  if (!empty($preview_form)) {
    $preview_form['#type'] = 'item';
    // Use of drupal_render will make the form not affecting the submit process
    $form['site_disclaimer_preview']['user_register']['form']['#markup'] = drupal_render($preview_form);
  }
  else {
    $form['site_disclaimer_preview']['user_register']['#description'] = '<br />' . t('(Site Disclaimer disabled)');
  }

  $form['site_disclaimer_preview']['accept_new'] = array(
    '#type' => 'fieldset',
    '#title' => t('Accept changes page'),
    '#description' => '',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  module_load_include('inc', 'site_disclaimer', 'site_disclaimer.pages');
  $preview_confirm = array();
  $form_state_tmp = array();
  $preview_confirm = site_disclaimer_confirm($preview_confirm, $form_state_tmp, $langcode, TRUE);
  if (!empty($preview_confirm)) {
    $preview_confirm['#type'] = 'item';
    // Use of drupal_render will make the form not affecting the submit process
    $form['site_disclaimer_preview']['accept_new']['form_confirm']['#markup'] = drupal_render($preview_confirm);
  }
  else {
    $form['site_disclaimer_preview']['accept_new']['form_confirm']['#markup'] = '<br />' . t('(Site Disclaimer disabled)');
  }

  $form = system_settings_form($form);
  // Add our handler after the one system_settings_form() adds, so all variables get saved.
  $form['#submit'][] = 'site_disclaimer_admin_settings_submit';
  return $form;
}

/**
 * Validate the site_disclaimer_admin_settings form.
 *
 * @see site_disclaimer_admin_settings()
 */
function site_disclaimer_admin_settings_validate($form, &$form_state) {
  _site_disclaimer_admin_decode_settings($form_state, TRUE);
  // Validate form only in case of settings save
  if ($form_state['#site_disclaimer_mode_switch']) {
    return;
  }

  // Check if settings require node id (for @link)
  $checkbox_label = filter_xss_admin($form_state['values']['site_disclaimer_checkbox_label']);
  $use_link = (strpos($checkbox_label, '@link') !== FALSE);
  $use_named_links = (strpos($checkbox_label, '@&quot;') !== FALSE);

  // Validate named links in 'site_disclaimer_checkbox_label'
  $output = _site_disclaimer_checkbox_label_substitute_links($checkbox_label, $linked_nodes, $linked_errors);
  foreach ($linked_errors as $titla => $error) {
    form_set_error('site_disclaimer_checkbox_label', $error);
  }
  $form_state['#site_disclaimer_named_links'] = $linked_nodes; // Save for site_disclaimer_admin_settings_submit() handler

  // Validate $use_link and 'site_disclaimer_node_id'/'site_disclaimer_node_title'
  // Set message about Site Disclaimer configuration
  if ( $form_state['#site_disclaimer_mode'] = 'node_id') {
    if (empty($form_state['values']['site_disclaimer_node_id'])) {
      // Empty means "disabled", unless @link is set
      if ($use_link) {
        form_set_error('site_disclaimer_node_id', t('You used @link token. You must specify a node <em>nid</em>.'));
      }
      else {
        drupal_set_message($use_named_links ? t('Site Disclaimer will use links in the checkbox.') : t('Site Disclaimer disabled.'));
      }
    }
  }
  elseif (empty($form_state['values']['site_disclaimer_node_title'])) {
    // Empty means "disabled", unless @link is set
    if ($use_link) {
      form_set_error('site_disclaimer_node_title', t('You used @link token. You must specify a node title.'));
    }
    else {
      drupal_set_message($use_named_links ? t('Site Disclaimer will use links in the checkbox.') : t('Site Disclaimer disabled.'));
    }
  }
}

/**
 * Submit the site_disclaimer_admin_settings form.
 *
 * @see site_disclaimer_admin_settings()
 */
function site_disclaimer_admin_settings_submit($form, &$form_state) {
  if ($form_state['#site_disclaimer_mode'] = 'node_id') {
    $nid = $form_state['values']['site_disclaimer_node_id'];
  }
  else {
    $nid = $form_state['#site_disclaimer_nid'];
  }
  // Rebuild allowed paths
  _site_disclaimer_prep_allowed_paths($nid);
}

/**
 * Helper function for autocompletion
 */
function site_disclaimer_autocomplete($string = '') {
  $matches = array();
  if ($string != '') {
    $result = db_select('node', 'n')
      ->fields('n', array('nid', 'title'))
      ->condition('n.title', '%' . db_like($string) . '%', 'LIKE')
      ->condition('n.status', 1)
      ->range(0, 10)
      ->addTag('node_access')
      ->execute();

    foreach ($result as $node) {
      $matches[$node->title] = $node->title;
    }
  }

  drupal_json_output($matches);
}

/**
 * Menu callback for AHAH addition.
 */
function site_disclaimer_js($form, &$form_state) {
  return $form['site_disclaimer_text'];
}

/**
 * Get the path of an URL (copied from _linkchecker_absolute_content_path() linkchecker.module).
 *
 * @param $url
 *   The http/https URL to parse.
 *
 * @return
 *   Full qualified URL with absolute path of the URL.
 */
function _site_disclaimer_absolute_content_path($url) {
  global $base_url;

  // Parse the URL and make sure we can handle the schema.
  $uri = @parse_url($url);

  if ($uri == FALSE) {
    return NULL;
  }

  if (!isset($uri['scheme'])) {
    return NULL;
  }

  // Break if the schema is not supported.
  if (!in_array($uri['scheme'], array('http', 'https'))) {
    return NULL;
  }

  $scheme = isset($uri['scheme']) ? $uri['scheme'] . '://' : '';
  $user = isset($uri['user']) ? $uri['user'] . ($uri['pass'] ? ':' . $uri['pass'] : '') . '@' : '';
  $port = isset($uri['port']) ? $uri['port'] : 80;
  $host = $uri['host'] . ($port != 80 ? ':' . $port : '');
  $path = isset($uri['path']) ? $uri['path'] : '/';

  // Glue the URL variables.
  $absolute_url = $scheme . $user . $host . $path;

  // Modification to the logic of _linkchecker_absolute_content_path():
  // We want to honor website absolute_url so the relative links don't break when drupal is
  // installed in webserver's subdirectory, e.g. http://www.example.com/drupal/
  if (strpos($base_url . '/',  $absolute_url) === 0) {
    return $base_url . '/';
  }

  // Find the last slash and remove all after the last slash to get the path.
  $last_slash = strrpos($absolute_url, '/');
  $absolute_content_url = drupal_substr($absolute_url, 0, $last_slash + 1);

  return $absolute_content_url;
}

/**
 * Extract links from content (copied and modified from _linkchecker_extract_links() linkchecker.module).
 *
 * @param string $text
 *    The text to be scanned for links.
 *
 * @param string $content_path
 *    Path to the content that is currently scanned for links. This value is
 *    required to build full qualified links from relative links. Relative links
 *    are not extracted from content, if path is not provided.
 *
 * @return array
 *    Array whose keys are fully qualified and unique URLs found in the
 *    content, and whose values are arrays of actual text (raw URLs or paths)
 *    corresponding to each fully qualified URL.
 */
function _site_disclaimer_extract_links($text = '', $content_path = NULL) {
  // Just in case the linkchecker.module is more up to date.
  //if (module_exists('linkchecker')) return _linkchecker_extract_links($text, $path);
  global $base_root;

  // Finds all hyperlinks in the content.
  $matches_a = array(1 => NULL);
  if (1 || variable_get('linkchecker_extract_from_a', 1) == 1) {
    // Extract all chars in the href value, except double and single quotes.
    $pattern_a = '/<(?:a|area)\s[^>]*href=["\']([^"\']*)["\'][^>]*>/i';
    preg_match_all($pattern_a, $text, $matches_a);
  }

  // Finds all audio links in the content.
  $matches_audio = array(1 => NULL);
  if (0 && variable_get('linkchecker_extract_from_audio', 1) == 1) {
    $pattern_audio = '/<audio\s[^>]*src=["\']([^"\']*)["\'][^>]*>/i';
    preg_match_all($pattern_audio, $text, $matches_audio);
  }

  // Finds embed tags with links in the content.
  $matches_embed = array();
  if (0 && variable_get('linkchecker_extract_from_embed', 0) == 1) {
    $pattern_embed_src = '/<embed\s[^>]*src=["\']([^"\']*)["\'][^>]*>/i';
    $pattern_embed_pluginurl = '/<embed\s[^>]*pluginurl=["\']([^"\']*)["\'][^>]*>/i';
    $pattern_embed_pluginspage = '/<embed\s[^>]*pluginspage=["\']([^"\']*)["\'][^>]*>/i';

    preg_match_all($pattern_embed_src, $text, $matches_embed_src);
    preg_match_all($pattern_embed_pluginurl, $text, $matches_embed_pluginurl);
    preg_match_all($pattern_embed_pluginspage, $text, $matches_embed_pluginspage);

    $matches_embed = array_merge(
      (array) $matches_embed_src[1],
      (array) $matches_embed_pluginurl[1],
      (array) $matches_embed_pluginspage[1]
    );
  }

  // Finds iframe tags with links in the content.
  $matches_iframe = array(1 => NULL);
  if (1 || variable_get('linkchecker_extract_from_iframe', 0) == 1) {
    $pattern_iframe = '/<iframe\s[^>]*src=["\']([^"\']*)["\'][^>]*>/i';
    preg_match_all($pattern_iframe, $text, $matches_iframe);
  }

  // Finds img tags with links in the content.
  $matches_img = array(1 => NULL);
  if (1 || variable_get('linkchecker_extract_from_img', 0) == 1) {
    $pattern_img = '/<img\s[^>]*src=["\']([^"\']*)["\'][^>]*>/i';
    preg_match_all($pattern_img, $text, $matches_img);
  }

  // Finds object/param tags with links in the content.
  $matches_object = array();
  if (0 && variable_get('linkchecker_extract_from_object', 0) == 1) {
    // TODO's:
    //  * Allow flipped order of attributes in "param".
    //  * Try to extract links in unkown "flashvars" values (for e.g. file=http://, data=http://).
    $pattern_object_data = '/<object\s[^>]*data=["\']([^"\']*)["\'][^>]*>/i';
    $pattern_object_codebase = '/<object\s[^>]*codebase=["\']([^"\']*)["\'][^>]*>/i';
    $pattern_param = '/<param\s[^>]*((name|src)=["\'](archive|filename|href|movie|src|url)["\']\s[^>]*)+value=["\']([^"\']*)["\'][^>]*>/i';

    preg_match_all($pattern_object_data, $text, $matches_object_data);
    preg_match_all($pattern_object_codebase, $text, $matches_object_codebase);
    preg_match_all($pattern_param, $text, $matches_param);

    $matches_object = array_merge(
      (array) $matches_object_data[1],
      (array) $matches_object_codebase[1],
      (array) $matches_param[4]
    );
  }

  // Finds source tags with links in the content.
  $matches_source = array(1 => NULL);
  if (0 || variable_get('linkchecker_extract_from_source', 0) == 1) {
    $pattern_source = '/<source\s[^>]*src=["\']([^"\']*)["\'][^>]*>/i';
    preg_match_all($pattern_source, $text, $matches_source);
  }

  // Finds video tags with links in the content.
  $matches_video = array();
  if (0 || variable_get('linkchecker_extract_from_video', 0) == 1) {
    $pattern_video_poster = '/<video\s[^>]*poster=["\']([^"\']*)["\'][^>]*>/i';
    $pattern_video_src = '/<video\s[^>]*src=["\']([^"\']*)["\'][^>]*>/i';

    preg_match_all($pattern_video_poster, $text, $matches_video_poster);
    preg_match_all($pattern_video_src, $text, $matches_video_src);

    $matches_video = array_merge(
      (array) $matches_video_poster[1],
      (array) $matches_video_src[1]
    );
  }

  // Merge all extracted links into one array.
  $urls = array_merge(
    (array) $matches_a[1],
    (array) $matches_audio[1],
    (array) $matches_embed,
    (array) $matches_iframe[1],
    (array) $matches_img[1],
    (array) $matches_object,
    (array) $matches_source[1],
    (array) $matches_video
  );

  // Remove empty values.
  $urls = array_filter($urls);
  // Remove duplicate urls.
  $urls = array_unique($urls);

  // What type of links should be checked?
  $linkchecker_check_links_types = variable_get('linkchecker_fqdn_only', 1);

  $links = array();
  foreach ($urls as $url) {
    // Decode HTML links into plain text links.
    $url_decoded = decode_entities($url);

    // FIXME: #1149596 HACK - Encode spaces in URLs, so validation equals TRUE and link gets added.
    $url_encoded = str_replace(' ', '%20', $url_decoded);

    // Full qualified URLs.
    if ((1 || $linkchecker_check_links_types != 2) && valid_url($url_encoded, TRUE)) {
      // Add to Array and change HTML links into plain text links.
      $links[$url_decoded][] = $url;
    }
    // Skip mailto:, javascript:, etc.
    elseif (preg_match('/^\w[\w.+]*:/', $url_decoded)) {
      continue;
    }
    // Local URLs.
    elseif ((1 || $linkchecker_check_links_types != 1) && valid_url($url_encoded, FALSE)) {
      // Get full qualified url with base path of content.
      $absolute_content_path = _site_disclaimer_absolute_content_path($content_path);

      // Absolute local URLs need to start with [/].
      if (preg_match('!^/!', $url_decoded)) {
        // Add to Array and change HTML encoded links into plain text links.
        $links[$base_root . $url_decoded][] = $url;
      }
      // Anchors and URL parameters like "#foo" and "?foo=bar".
      elseif (!empty($content_path) && preg_match('!^[?#]!', $url_decoded)) {
        // Add to Array and change HTML encoded links into plain text links.
        $links[$content_path . $url_decoded][] = $url;
      }
      // Relative URLs like "./foo/bar" and "../foo/bar".
      elseif (!empty($absolute_content_path) && preg_match('!^\.{1,2}/!', $url_decoded)) {
        // Build the URI without hostname before the URI is normalized and
        // dot-segments will be removed. The hostname is added back after the
        // normalization has completed to prevent hostname removal by the regex.
        // This logic intentionally does not implement all the rules definied in
        // RFC 3986, section 5.2.4 to show broken links and over-dot-segmented
        // URIs; e.g. http://example.com/../../foo/bar.
        // For more information, see http://drupal.org/node/832388.
        $path = substr_replace($absolute_content_path . $url_decoded, '', 0, drupal_strlen($base_root));

        // Remove './' segments where possible.
        $path = str_replace('/./', '/', $path);

        // Remove '../' segments where possible. Loop until all segments are
        // removed. Taken over from _drupal_build_css_path() in common.inc.
        $last = '';
        while ($path != $last) {
          $last = $path;
          $path = preg_replace('`(^|/)(?!\.\./)([^/]+)/\.\./`', '$1', $path);
        }

        // Glue the hostname and path to full-qualified URI.
        $links[$base_root . $path][] = $url;
      }
      // Relative URLs like "test.png".
      elseif (!empty($absolute_content_path) && preg_match('!^[^/]!', $url_decoded)) {
        $links[$absolute_content_path . $url_decoded][] = $url;
      }
      else {
        // TODO: Are there more special cases the module need to handle?
      }
    }
  }

  return $links;
}

/**
 * Create 'site_disclaimer_allow_nodes' variable from links found in:
 * - all translations of Checkbox labels,
 * - all translations of Site Disclaimer nodes,
 * @param $nid
 *  Site Disclaimer node nid.
 * @param $delete_nid
 *  If supplied, nid of node being deleted.
 */
function _site_disclaimer_prep_allowed_paths($nid, $delete_nid = NULL) {
  $allow_nids = array($nid);

  $languages = language_list();
  $nids = _site_disclaimer_nid_all_translations($nid);

  // Get all translations of checkbox label and process links (@link and @"Post title")
  $checkbox_labels = _site_disclaimer_checkbox_label_all_translations();
  foreach ($checkbox_labels as $langcode => $checkbox_label) {
    $use_link = (strpos($checkbox_label, '@link') !== FALSE);
    if ($use_link && !empty($nids[$langcode])) {
      $allow_nids[] = $nids[$langcode];
    }
    _site_disclaimer_checkbox_label_substitute_links($checkbox_label, $linked_nodes);
    $allow_nids += $linked_nodes;
  }

  // All Site Disclaimer node translations may be shown - extract links from them
  $links = array();
  foreach ($nids as $langcode => $tnid) {
    $node = node_load($tnid);
    if ($node) {
      $items = field_get_items('node', $node, 'body');
      if ($items) {
        $terms = field_view_value('node', $node, 'body', $items[0]);
        $node_langcode = isset($node->language) ? $node->language : 'und';
        // Get the absolute node path for extraction of relative links.
        $path = url('node/' . $node->nid, array('language' => ($node_langcode == 'und') ? NULL : $languages[$node_langcode], 'absolute' => TRUE));
        // Extract all links in a node.
        $urls = array_keys(_site_disclaimer_extract_links($terms['#markup'], $path));
        $links += array($langcode => array());
        $links[$langcode] += $urls;
      }
    }
  }

  // Parse all links, extract only node ids
  global $base_url;
  $len = drupal_strlen($base_url) + 1; // Remove base and slash prefix
  foreach ($links as $langcode => $urls) {
    foreach ($urls as $link) {
      // Ignore external links
      if (strpos($link, $base_url . '/') === 0) {
        $start = $len;
        // Allow both clean and non-clean urls
        $no_clean_url = strpos($link, '?q=');
        if ($no_clean_url !== FALSE) {
          $start = $no_clean_url + 3;
        }
        $end = strpos($link, '?', $start);
        if ($end === FALSE) {
          $end = strpos($link, '#', $start);
        }
        if ($end === FALSE) {
          $end = drupal_strlen($link);
        }
        $link = drupal_substr($link, $start, ($end - $start));
        $path = drupal_get_normal_path($link, $langcode);
        if (!$path || $path == $link) {
          $path = drupal_get_normal_path($link, '');
        }
        if (!$path || $path == $link) {
          $path = drupal_get_normal_path($link, language_default('language'));
        }
        $ret = preg_match('!^node/([0-9]+)!i', $path, $matches);
        if ($ret == 1 && is_numeric($matches[1]) && (!isset($delete_nid) || $delete_nid != $matches[1])) {
          $allow_nids[] = $matches[1];
        }
      }
    }
  }

  $allow_nids = array_unique($allow_nids);
  // 'site_disclaimer_allow_nodes' lists all nodes that will avoid a goto to an acceptance form.
  variable_set('site_disclaimer_allow_nodes', $allow_nids);
}


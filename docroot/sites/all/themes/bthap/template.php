<?php

/**
 * Implements hook_html_head_alter().
 */
function bthap_html_head_alter(&$elements) {
  // Optimize the mobile viewport.
  $elements['bthap_viewport'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width',
    ),
  );

  // Include a humans.txt file, if it exists in your document root.
  $humanstxt_file = DRUPAL_ROOT . '/humans.txt';
  $elements['bthap_humanstxt'] = array(
    '#type' => 'html_tag',
    '#tag' => 'link',
    '#attributes' => array(
      'rel' => 'author',
      'href' =>  base_path() . 'humans.txt',
    ),
    '#access' => theme_get_setting('bthap_humanstxt') && file_exists($humanstxt_file),
  );

  // Add HTML5 shiv for Internet Explorer. @todo make configurable and provide
  // local copy fallback.
  $elements['bthap_html5_shiv'] = array(
    '#type' => 'html_tag',
    '#tag' => 'script',
    '#value' => '',
    '#attributes' => array(
      'src' => 'http://html5shim.googlecode.com/svn/trunk/html5.js',
    ),
    '#browsers' => array(
      'IE' => 'lt IE 9',
      '!IE' => FALSE,
    ),
  );

  // Container for all generic Internet Explorer headers.
  $elements['bthap_ie'] = array(
    '#prefix' => "<!--[if IE]>\n",
    '#suffix' =>  "<![endif]-->\n",
  );

  // @see http://html5boilerplate.com/docs/The-markup/#make-sure-the-latest-version-of-ie-is-used.
  // Force IE browsers to render with Google Chrome Frame plugin, if installed
  // locally. Otherwise, force IE browsers to never fall back to compatibility
  // mode ( IE 7 mode). However, sometimes we need to allow the option to
  // fallback to compatibility mode when we're debugging for IE 7 (but want to
  // use IE8's slightly better developer tools) So you can disable this feature
  // in the theme settings.
  $elements['bthap_ie']['bthap_ie_chrome_compatibility'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'http-equiv' => 'X-UA-Compatible',
      'content' => 'IE=edge,chrome=1',
    ),
    '#access' => theme_get_setting('bthap_ie_chrome_compatibility'),
  );

  // Kill IE6's pop-up-on-mouseover toolbar for images that can interfere with
  // certain designs and be pretty distracting in general.
  $elements['bthap_ie']['bthap_suppress_ie6_image_toolbar'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'http-equiv' => 'imagetoolbar',
      'content' => 'false',
    ),
    '#access' => theme_get_setting('bthap_suppress_ie6_image_toolbar'),
  );

  // Support "Pin Sites" for Internet Explorer 9+. @todo this is out-of-scope
  // and we should let http://drupal.org/project/pinned_site handle this.
  $elements['bthap_ie']['bthap_ie9_ps_app_name'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'http-equiv' => 'application-name',
      'content' => theme_get_setting('bthap_ie9_ps_app_name'),
    ),
    '#access' => theme_get_setting('bthap_ie9_ps_app_name') != FALSE,
  );

  $elements['bthap_ie']['bthap_ie9_ps_tooltip'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'http-equiv' => 'msapplication-tooltip',
      'content' => theme_get_setting('bthap_ie9_ps_tooltip'),
    ),
    '#access' => theme_get_setting('bthap_ie9_ps_tooltip') != FALSE,
  );

  $elements['bthap_ie']['bthap_ie9_ps_starturl'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'http-equiv' => 'msapplication-starturl',
      'content' => url(theme_get_setting('bthap_ie9_ps_starturl'), array('absolute' => TRUE, 'https' => (bool) theme_get_setting('bthap_ie9_ps_starturl_https'))),
    ),
    '#access' => theme_get_setting('bthap_ie9_ps_starturl') != FALSE,
  );

  $elements['bthap_ie']['bthap_ie9_ps_color'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'http-equiv' => 'msapplication-navbutton-color',
      'content' => theme_get_setting('bthap_ie9_ps_color'),
    ),
    '#access' => theme_get_setting('bthap_ie9_ps_color') != FALSE,
  );
}

/**
 * Implements hook_preprocess_html().
 */
function bthap_process_html(&$vars) {
  // Aggragate the usual attributes that are placed in the html tag into a
  // single variable. Classes should not be added to this array since
  // contextual classes will be applied conditionally by the browser.
  $html_attributes = array(
    'xmlns' => 'http://www.w3.org/1999/xhtml',
    'xml:lang' => $vars['language']->language,
    'lang' => $vars['language']->language,
    'version' => 'XHTML+RDFa 1.0',
    'dir' => $vars['language']->dir,
  );

  $vars['html_attributes'] = drupal_attributes($html_attributes) . $vars['rdf_namespaces'];
}


/**
 * Implements hook_js_alter().
 */
function bthap_js_alter(&$javascript) {
  if (theme_get_setting('bthap_footer_js')) {
    // Force all scripts to the footer, unless they are explicit about being in
    // the header.
    foreach ($javascript as &$js) {
      $js['scope'] = (isset($js['force header']) && $js['force header']) ? 'header' : 'footer';
    }
  }
}

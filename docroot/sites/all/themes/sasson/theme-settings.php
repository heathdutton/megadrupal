<?php

/**
 * @file
 * Theme settings for the sasson
 */
function sasson_form_system_theme_settings_alter(&$form, &$form_state) {

  global $theme_key;

  drupal_add_css(drupal_get_path('theme', 'sasson') .'/stylesheets/theme-settings.css');
  drupal_add_js(drupal_get_path('theme', 'sasson') .'/scripts/themeSettings.js');

  $select_toggle = '<br>' .
  l(t('select all'), '#', array('attributes' => array('class' => 'select-all'))) . ' | ' .
  l(t('select none'), '#', array('attributes' => array('class' => 'select-none')));

  $form['sasson_settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => -10,
    '#prefix' => '<h3>' . t('Theme configuration') . '</h3>',
  );

  /**
   * Responsive Layout Settings
   */
  $form['sasson_settings']['sasson_layout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Responsive Layout Settings'),
  );
  $form['sasson_settings']['sasson_layout']['sasson_responsive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable responsive layout'),
    '#attributes' => array(
      'class' => array('enable-extension'),
    ),
    '#description' => t("Disable if you don't want your site layout to adapt to small devices, this enables both the css3 media-queries that takes care of adapting the layout and the 'viewport' meta tag that makes sure mobile devices properly display your layout."),
    '#default_value' => theme_get_setting('sasson_responsive'),
  );
  $form['sasson_settings']['sasson_layout']['sasson_responsive_approach'] = array(
    '#type' => 'radios',
    '#title' => t('Desktop or Mobile first'),
    '#options' => array(
        'desktop_first' => t('Desktop first'),
        'mobile_first' => t('Mobile first'),
      ),
    '#description' => t('Select they way your responsive layout should be constructed. desktop-first means we start with desktop size page and reduce accordingly, mobile-first means we start with a very simple layout and build on top of that.'),
    '#default_value' => theme_get_setting('sasson_responsive_approach'),
  );

  $form['sasson_settings']['sasson_layout']['media_break_points'] = array(
    '#markup' => '<strong>' . t('Media queries break-point are being set via <code>!settings</code>. Copy it to your sub-theme and set your own values.', array('!settings' => l('_settings.scss', 'http://drupalcode.org/project/sasson.git/blob/refs/heads/7.x-3.x:/sass/_settings.scss', array('attributes' => array('target'=>'_blank'))))) . '</strong>',
  );

  $form['sasson_settings']['sasson_layout']['responsive_menus'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Responsive menus'),
  );
  $form['sasson_settings']['sasson_layout']['responsive_menus']['sasson_responsive_menus_width'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => t('Responsive menus page width'),
    '#description' => t("Set the width in which the selected menus turn into a select menu, or 0 to disable."),
    '#default_value' => theme_get_setting('sasson_responsive_menus_width'),
  );
  $form['sasson_settings']['sasson_layout']['responsive_menus']['sasson_responsive_menus_selectors'] = array(
    '#type' => 'textfield',
    '#title' => t('Responsive menus selectors'),
    '#description' => t("Enter some CSS selectors for the menus you want to alter."),
    '#default_value' => theme_get_setting('sasson_responsive_menus_selectors'),
  );
  $form['sasson_settings']['sasson_layout']['responsive_menus']['sasson_responsive_menus_autohide'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto-hide the standard menu'),
    '#default_value' => theme_get_setting('sasson_responsive_menus_autohide'),
  );

  /**
   * CSS settings
   */
  $form['sasson_settings']['sasson_css'] = array(
    '#type' => 'fieldset',
    '#title' => t('CSS settings'),
  );
  $form['sasson_settings']['sasson_css']['sasson_reset'] = array(
    '#type' => 'fieldset',
    '#title' => t('CSS Reset VS Normalize'),
  );
  $form['sasson_settings']['sasson_css']['sasson_reset']['sasson_cssreset'] = array(
    '#type' => 'radios',
    '#options' => array(
        'normalize' => t('Use !normalize from !h5bp.', array('!normalize' => l('normalize.css', 'http://necolas.github.com/normalize.css/', array('attributes' => array('target'=>'_blank'))), '!h5bp' => l('HTML5 Boilerplate', 'http://html5boilerplate.com/', array('attributes' => array('target'=>'_blank'))))),
        'reset' => t("Use !meyer's CSS reset.", array('!meyer' => l('Eric Meyer', 'http://meyerweb.com/eric/tools/css/reset/', array('attributes' => array('target'=>'_blank'))))),
        'none' => t("None"),
      ),
    '#description' => t('Normalize.css makes browsers render all elements more consistently and in line with modern standards, while preserving useful defaults.<br>
      Reset.css will reset css values (e.g. set 0) to reduce browser inconsistencies in things like default line heights, margins and font sizes.'),
    '#default_value' => theme_get_setting('sasson_cssreset'),
  );
  $form['sasson_settings']['sasson_css']['sasson_forms'] = array(
    '#type' => 'fieldset',
    '#title' => t('Forms styling'),
  );
  $form['sasson_settings']['sasson_css']['sasson_forms']['sasson_formalize'] = array(
    '#type' => 'checkbox',
    '#title' => t("Use !formalize to reset your forms.", array('!formalize' => l('Formalize', 'http://formalize.me/', array('attributes' => array('target'=>'_blank'))))),
    '#description' => t('Apply consistent and cross-browser styles to all forms.'),
    '#default_value' => theme_get_setting('sasson_formalize'),
  );
  // Disable CSS
  require_once drupal_get_path('theme', 'sasson') . '/includes/assets.inc';
  $form['sasson_settings']['sasson_css']['sasson_disablecss'] = array(
    '#type' => 'fieldset',
    '#title' => t('Disable CSS files'),
  );
  $form['sasson_settings']['sasson_css']['sasson_disablecss']['sasson_disable_css'] = array(
    '#type' => 'checkbox',
    '#title' => t("Enable this extension."),
    '#attributes' => array(
      'class' => array('enable-extension'),
    ),
    '#description' => t('Disable all CSS files included by core and contrib modules or choose specific CSS files to disable.'),
    '#default_value' => theme_get_setting('sasson_disable_css'),
  );
  $form['sasson_settings']['sasson_css']['sasson_disablecss']['modules'] = array(
    '#type' => 'fieldset',
    '#title' => t('Per module'),
    '#description' => t('Disable all CSS files from selected modules.') . $select_toggle,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['sasson_settings']['sasson_css']['sasson_disablecss']['modules']['sasson_disable_css_modules'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Modules'),
    '#options' => sasson_get_modules_list(),
    '#default_value' => theme_get_setting('sasson_disable_css_modules') ? theme_get_setting('sasson_disable_css_modules') : array(),
  );
  $form['sasson_settings']['sasson_css']['sasson_disablecss']['files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Specific CSS files'),
    '#description' => t('Disable specific CSS files from core and contrib modules.') . $select_toggle,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['sasson_settings']['sasson_css']['sasson_disablecss']['files']['sasson_disable_css_files'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Disable specific css files.'),
    '#options' => sasson_get_assets_list(),
    '#default_value' => theme_get_setting('sasson_disable_css_files') ? theme_get_setting('sasson_disable_css_files') : array(),
  );

  /**
   * JavaScript Settings
   */
  $form['sasson_settings']['sasson_js'] = array(
    '#type' => 'fieldset',
    '#title' => t('JavaScript Settings'),
  );
  $form['sasson_settings']['sasson_js']['sasson_js_footer_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Move JavaScript to footer'),
  );
  $form['sasson_settings']['sasson_js']['sasson_js_footer_wrapper']['sasson_js_footer'] = array(
    '#type' => 'checkbox',
    '#title' => t('Move all scripts to the footer.'),
    '#description' => t('Yahoo! Exceptional Performance team recommends <a href="http://developer.yahoo.com/performance/rules.html#js_bottom">placing scripts at the bottom of your page</a> because of the way browsers download components.') . '<br>' .
      t('This will move all your scripts to the bottom of your page. You can still force a script to go in the <code>head</code> by setting <code>"force_header" => TRUE</code> in your !drupal_add_js options array.', array('!drupal_add_js' => l('drupal_add_js', 'http://api.drupal.org/api/drupal/includes%21common.inc/function/drupal_add_js', array('attributes' => array('target'=>'_blank'))))),
    '#default_value' => theme_get_setting('sasson_js_footer'),
  );
  $form['sasson_settings']['sasson_js']['sasson_latestjs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Latest jQuery from Google'),
  );
  $form['sasson_settings']['sasson_js']['sasson_latestjs']['sasson_latest_jquery'] = array(
    '#type' => 'checkbox',
    '#title' => t('Replace core\'s jQuery with latest version from Google\'s CDN.'),
    '#default_value' => theme_get_setting('sasson_latest_jquery'),
    '#description' => t('<strong>Note:</strong> this might break scripts that depend on deprecated jQuery features.'),
  );
  // Disable JS
  $form['sasson_settings']['sasson_js']['sasson_disablejs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Disable JS files'),
  );
  $form['sasson_settings']['sasson_js']['sasson_disablejs']['sasson_disable_js'] = array(
    '#type' => 'checkbox',
    '#title' => t("Enable this extension."),
    '#attributes' => array(
      'class' => array('enable-extension'),
    ),
    '#description' => t('Disable all JS files included by core and contrib modules or choose specific JS files to disable.'),
    '#default_value' => theme_get_setting('sasson_disable_js'),
  );
  $form['sasson_settings']['sasson_js']['sasson_disablejs']['modules'] = array(
    '#type' => 'fieldset',
    '#title' => t('Per module'),
    '#description' => t('Disable all JS files from selected modules.') . $select_toggle,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['sasson_settings']['sasson_js']['sasson_disablejs']['modules']['sasson_disable_js_modules'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Modules'),
    '#options' => sasson_get_modules_list(),
    '#default_value' => theme_get_setting('sasson_disable_js_modules') ? theme_get_setting('sasson_disable_js_modules') : array(),
  );
  $form['sasson_settings']['sasson_js']['sasson_disablejs']['files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Specific JS files'),
    '#description' => t('Disable specific JS files from core and contrib modules.') . $select_toggle,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['sasson_settings']['sasson_js']['sasson_disablejs']['files']['sasson_disable_js_files'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Disable specific JS files.'),
    '#options' => sasson_get_assets_list('js'),
    '#default_value' => theme_get_setting('sasson_disable_js_files') ? theme_get_setting('sasson_disable_js_files') : array(),
  );
  $form['sasson_settings']['sasson_js']['sasson_disablejs']['jquery'] = array(
    '#type' => 'fieldset',
    '#title' => t('Core jQuery & jQuery UI'),
    '#description' => t('Disable specific jQuery & jQuery UI files from core.') . $select_toggle,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['sasson_settings']['sasson_js']['sasson_disablejs']['jquery']['sasson_disable_jquery_files'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Disable core jQuery & jQuery UI files.'),
    '#options' => sasson_get_assets_list('js', 'jquery'),
    '#default_value' => theme_get_setting('sasson_disable_jquery_files') ? theme_get_setting('sasson_disable_jquery_files') : array(),
  );
  $form['sasson_settings']['sasson_js']['sasson_disablejs']['sasson_js'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sasson\'s JS'),
  );
  $form['sasson_settings']['sasson_js']['sasson_disablejs']['sasson_js']['sasson_disable_sasson_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable all JS from Sasson.'),
    '#description' => t('<strong>Note:</strong> this will break Sasson\'s scripts like the file-watcher and mobile menus if they are enabled, be warned.'),
    '#default_value' => theme_get_setting('sasson_disable_sasson_js'),
  );

  /**
   * HTML5 IE support
   */
  $form['sasson_settings']['sasson_html5'] = array(
    '#type' => 'fieldset',
    '#title' => t('HTML5 IE support'),
  );
  $form['sasson_settings']['sasson_html5']['sasson_force_ie'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force latest IE rendering engine (even in intranet) & Chrome Frame'),
    '#default_value' => theme_get_setting('sasson_force_ie'),
  );
  $form['sasson_settings']['sasson_html5']['sasson_html5shiv'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable HTML5 elements in IE'),
    '#description' => t('Makes IE understand HTML5 elements via <a href="!shivlink">HTML5 shiv</a>. disable if you use a different method.', array('!shivlink' => 'http://code.google.com/p/html5shiv/')),
    '#default_value' => theme_get_setting('sasson_html5shiv'),
  );
  $form['sasson_settings']['sasson_html5']['sasson_ie_comments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add IE specific classes'),
    '#description' => t('This will add conditional classes to the html tag for IE specific styling. see this <a href="!post">post</a> for more info.', array('!post' => 'http://paulirish.com/2008/conditional-stylesheets-vs-css-hacks-answer-neither/')),
    '#default_value' => theme_get_setting('sasson_ie_comments'),
  );
  $form['sasson_settings']['sasson_html5']['sasson_prompt_cf'] = array(
    '#type' => 'select',
    '#title' => t('Prompt old IE users to update'),
    '#default_value' => theme_get_setting('sasson_prompt_cf'),
    '#options' => drupal_map_assoc(array(
       'Disabled',
       'IE 6',
       'IE 7',
       'IE 8',
       'IE 9',
    )),
      '#description' => t('Set the latest IE version you would like the prompt box to show on or disable if you want to support old IEs.'),
  );

  /**
   * Fonts
   */
  $form['sasson_settings']['sasson_fonts'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t("
      Set a custom font to be used across the site. you may override typography settings in you sub-theme's css/sass/scss files.<br>
      <strong>Note:</strong> Only fonts from !webfont are supported at the moment, if this is not enough you should check out !fontyourface module.",
      array('!webfont' => l('google web fonts', 'http://www.google.com/webfonts', array('attributes' => array('target'=>'_blank'))), '!fontyourface' => l('@font-your-face', 'http://drupal.org/project/fontyourface', array('attributes' => array('target'=>'_blank'))))),
    '#title' => t('Fonts'),
  );
  $form['sasson_settings']['sasson_fonts']['sasson_font'] = array(
    '#type' => 'textfield',
    '#title' => t('Font name'),
    '#description' => t("Enter the font name from Google web fonts."),
    '#default_value' => theme_get_setting('sasson_font'),
  );
  $form['sasson_settings']['sasson_fonts']['sasson_font_fallback'] = array(
    '#type' => 'textfield',
    '#title' => t('Font fallback'),
    '#description' => t("Enter the font names you would like as a fallback in a comma separated list. e.g. <code>'Times New Roman', Times, serif</code>."),
    '#default_value' => theme_get_setting('sasson_font_fallback'),
  );
  $form['sasson_settings']['sasson_fonts']['sasson_font_selectors'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS selectors'),
    '#description' => t("Enter some CSS selectors for the fonts to apply to. if none is provided it will default to a <code>.sasson-font-face</code> class"),
    '#default_value' => theme_get_setting('sasson_font_selectors'),
  );

  /**
   * Development Settings
   */
  $form['sasson_settings']['sasson_development'] = array(
    '#type' => 'fieldset',
    '#title' => t('Development'),
  );

  $form['sasson_settings']['sasson_development']['sasson_watch'] = array(
    '#type' => 'fieldset',
    '#title' => t('File Watcher'),
  );
  $form['sasson_settings']['sasson_development']['sasson_watch']['sasson_watcher'] = array(
    '#type' => 'checkbox',
    '#title' => t('Watch for file changes and automatically refresh the browser.'),
    '#attributes' => array(
      'class' => array('enable-extension'),
    ),
    '#description' => t('With this feature on, you may enter a list of URLs for files to be watched, whenever a file is changed, your browser will automagically update itself.<br><strong>Turn this off when not actively developing.</strong>'),
    '#default_value' => theme_get_setting('sasson_watcher'),
  );
  $form['sasson_settings']['sasson_development']['sasson_watch']['sasson_watch_file'] = array(
    '#type' => 'textarea',
    '#title' => t('Files to watch'),
    '#description' => t('Enter the internal path of the files to be watched. one file per line. no leading slash.<br> e.g. sites/all/themes/sasson/stylesheets/sasson.scss<br>Lines starting with a semicolon (;) will be ignored.<br><strong>Keep this list short !</strong> Watch only the files you currently work on.'),
    '#rows' => 3,
    '#default_value' => theme_get_setting('sasson_watch_file'),
  );

  $form['sasson_settings']['sasson_development']['sasson_grid'] = array(
    '#type' => 'fieldset',
    '#title' => t('Grid background'),
  );
  $form['sasson_settings']['sasson_development']['sasson_grid']['sasson_show_grid'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show grid background layer.'),
    '#description' => t('Display a visible background grid, for easier elements positioning'),
    '#default_value' => theme_get_setting('sasson_show_grid'),
  );

  $form['sasson_settings']['sasson_development']['sasson_overlay'] = array(
    '#type' => 'fieldset',
    '#title' => t('Design Overlay'),
  );
  $form['sasson_settings']['sasson_development']['sasson_overlay']['sasson_overlay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable overlay image'),
    '#attributes' => array(
      'class' => array('enable-extension'),
    ),
    '#description' => t('With this feature on, you may upload an image that will be place as a draggable overlay image over your HTML for easy visual comparison. you may also set different overlay opacity.'),
    '#default_value' => theme_get_setting('sasson_overlay'),
  );
  $form['sasson_settings']['sasson_development']['sasson_overlay']['sasson_overlay_file'] = array(
    '#type' => 'managed_file',
    '#title' => t('Upload overlay image'),
    '#upload_location' => file_default_scheme() . '://sasson/overlay/',
    '#default_value' => theme_get_setting('sasson_overlay_file'),
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
    ),
  );
  $form['sasson_settings']['sasson_development']['sasson_overlay']['sasson_overlay_opacity'] = array(
    '#type' => 'select',
    '#title' => t('Overlay opacity'),
    '#default_value' => theme_get_setting('sasson_overlay_opacity'),
    '#options' => drupal_map_assoc(array(
       '0.1',
       '0.2',
       '0.3',
       '0.4',
       '0.5',
       '0.6',
       '0.7',
       '0.8',
       '0.9',
       '1',
    )),
  );

  $form['sasson_settings']['sasson_development']['sasson_mail_dev'] = array(
    '#type' => 'fieldset',
    '#title' => t('HTML Mail debugger'),
    '#description' => t('Useful tools for HTML mail development.') . $select_toggle,
  );
  $debug_file = DRUPAL_ROOT . '/' . file_directory_temp() . '/mail_debug/mail_debug.html';
  $form['sasson_settings']['sasson_development']['sasson_mail_dev']['sasson_mail_to_file'] = array(
    '#type' => 'checkbox',
    '#title' => t('Write mail messages to file'),
    '#description' => t('All mail messages will be written to <code>!file</code>', array('!file' => $debug_file)),
    '#default_value' => theme_get_setting('sasson_mail_to_file'),
  );
  $form['sasson_settings']['sasson_development']['sasson_mail_dev']['sasson_mail_to_log'] = array(
    '#type' => 'checkbox',
    '#title' => t('Write mail messages to log'),
    '#description' => t('All mail messages will be logged to the !db.', array('!db' => l('database', 'admin/reports/dblog'))),
    '#default_value' => theme_get_setting('sasson_mail_to_log'),
  );
  $form['sasson_settings']['sasson_development']['sasson_mail_dev']['sasson_mail_devel'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display development messages'),
    '#description' => t('Show a development message after mail is sent (requires devel module)'),
    '#default_value' => theme_get_setting('sasson_mail_devel'),
  );
  $form['sasson_settings']['sasson_development']['sasson_mail_dev']['sasson_mail_prevent'] = array(
    '#type' => 'checkbox',
    '#title' => t('Prevent mail from being sent'),
    '#description' => t('Prevent outgoing mail messages from being sent while developing.'),
    '#default_value' => theme_get_setting('sasson_mail_prevent'),
  );

  /**
   * General Settings
   */
  $form['sasson_settings']['sasson_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
  );

  $admin_theme = variable_get('admin_theme', 0);
  if (!empty($admin_theme) && $admin_theme != $theme_key) {
    $form['sasson_settings']['sasson_general']['theme_settings'] = $form['theme_settings'];
    $form['sasson_settings']['sasson_general']['logo'] = $form['logo'];
    $form['sasson_settings']['sasson_general']['favicon'] = $form['favicon'];
    unset($form['theme_settings']);
    unset($form['logo']);
    unset($form['favicon']);
  }
  else {
    $form['theme_settings']['#collapsible'] = TRUE;
    $form['theme_settings']['#collapsed'] = TRUE;
    $form['logo']['#collapsible'] = TRUE;
    $form['logo']['#collapsed'] = TRUE;
    $form['favicon']['#collapsible'] = TRUE;
    $form['favicon']['#collapsed'] = TRUE;
  }

  $form['sasson_settings']['sasson_general']['sasson_breadcrumb'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumbs'),
  );
  $form['sasson_settings']['sasson_general']['sasson_breadcrumb']['sasson_breadcrumb_hideonlyfront'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide the breadcrumb if the breadcrumb only contains the link to the front page.'),
    '#default_value' => theme_get_setting('sasson_breadcrumb_hideonlyfront'),
  );
  $form['sasson_settings']['sasson_general']['sasson_breadcrumb']['sasson_breadcrumb_showtitle'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show page title on breadcrumb.'),
    '#default_value' => theme_get_setting('sasson_breadcrumb_showtitle'),
  );
  $form['sasson_settings']['sasson_general']['sasson_breadcrumb']['sasson_breadcrumb_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Breadcrumb separator'),
    '#default_value' => theme_get_setting('sasson_breadcrumb_separator'),
  );

  $form['sasson_settings']['sasson_general']['sasson_rss'] = array(
    '#type' => 'fieldset',
    '#title' => t('RSS'),
  );
  $form['sasson_settings']['sasson_general']['sasson_rss']['sasson_feed_icons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Feed Icons'),
    '#default_value' => theme_get_setting('sasson_feed_icons'),
  );
}

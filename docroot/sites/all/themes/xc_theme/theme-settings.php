<?php

function xc_theme_form_system_theme_settings_alter(&$form, $form_state) {

  // Add some CSS so we can style our form in any theme, i.e. in Seven.
  drupal_add_css(drupal_get_path('theme', 'xc_theme') . '/css/theme-settings.css', array('group' => CSS_THEME));

  // Layout settings
  $form['at'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => -10,
    '#default_tab' => 'defaults',
  );
  if (theme_get_setting('layout_enable_settings') == 'on') {
    $form['at']['page_layout'] = array(
      '#type' => 'fieldset',
      '#title' => t('Page Layout'),
      '#description' => t('Use these settings to set the width of the page, each sidebar, and to set the position of sidebars.'),
    );
    if (theme_get_setting('layout_enable_width') == 'on') {
      $form['at']['page_layout']['layout_width'] = array(
        '#type' => 'select',
        '#title' => t('Page width'),
        '#prefix' => '<div class="page-width">',
        '#suffix' => '</div>',
        '#default_value' => theme_get_setting('layout_width'),
        '#options' => array(
          '600px' => t('600px'),
          '660px' => t('660px'),
          '720px' => t('720px'),
          '780px' => t('780px'),
          '840px' => t('840px'),
          '900px' => t('900px'),
          '960px' => t('960px'),
          '1020px' => t('1020px'),
          '1080px' => t('1080px'),
          '1140px' => t('1140px'),
          '1200px' => t('1200px'),
          '1260px' => t('1260px'),
          '1320px' => t('1320px'),
          '1380px' => t('1380px'),
          '1440px' => t('1440px'),
          '1500px' => t('1500px'),
          '80%'  => t('80%'),
          '85%'  => t('85%'),
          '90%'  => t('90%'),
          '95%'  => t('95%'),
          '100%' => t('100%'),
        ),
        //'#attributes' => array('class' => 'field-layout-width'),
      );
    } // endif width
    if (theme_get_setting('layout_enable_sidebars') == 'on') {
      $form['at']['page_layout']['layout_sidebar_first_width'] = array(
        '#type' => 'select',
        '#title' => t('Sidebar first width'),
        '#prefix' => '<div class="sidebar-width"><div class="sidebar-width-left">',
        '#suffix' => '</div>',
        '#default_value' => theme_get_setting('layout_sidebar_first_width'),
        '#options' => array(
          '60' => t('60px'),
          '120' => t('120px'),
          '160' => t('160px'),
          '180' => t('180px'),
          '240' => t('240px'),
          '300' => t('300px'),
          '320' => t('320px'),
          '360' => t('360px'),
          '420' => t('420px'),
          '480' => t('480px'),
          '540' => t('540px'),
          '600' => t('600px'),
          '660' => t('660px'),
          '720' => t('720px'),
          '780' => t('780px'),
          '840' => t('840px'),
          '900' => t('900px'),
          '960' => t('960px'),
        ),
        //'#attributes' => array('class' => 'sidebar-width-select'),
      );
      $form['at']['page_layout']['layout_sidebar_last_width'] = array(
        '#type' => 'select',
        '#title' => t('Sidebar last width'),
        '#prefix' => '<div class="sidebar-width-right">',
        '#suffix' => '</div></div>',
        '#default_value' => theme_get_setting('layout_sidebar_last_width'),
        '#options' => array(
          '60' => t('60px'),
          '120' => t('120px'),
          '160' => t('160px'),
          '180' => t('180px'),
          '240' => t('240px'),
          '300' => t('300px'),
          '320' => t('320px'),
          '360' => t('360px'),
          '420' => t('420px'),
          '480' => t('480px'),
          '540' => t('540px'),
          '600' => t('600px'),
          '660' => t('660px'),
          '720' => t('720px'),
          '780' => t('780px'),
          '840' => t('840px'),
          '900' => t('900px'),
          '960' => t('960px'),
        ),
      );
    } //endif layout sidebars
    if (theme_get_setting('layout_enable_method') == 'on') {
      $form['at']['page_layout']['select_method'] = array(
        '#type' => 'fieldset',
        '#title' => t('Sidebar Position'),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
      );
      $form['at']['page_layout']['select_method']['layout_method'] = array(
        '#type' => 'radios',
        '#description' => t('The sidebar layout descriptions are for LTR (left to right languages), these will flip in RTL mode.'),
        '#default_value' => theme_get_setting('layout_method'),
        '#options' => array(
          0 => t('<strong>Layout #1</strong> <span class="layout-type-0">Standard three column layout — Sidebar first | Content | Sidebar last</span>'),
          1 => t('<strong>Layout #2</strong> <span class="layout-type-1">Two columns on the right — Content | Sidebar first | Sidebar last</span>'),
          2 => t('<strong>Layout #3</strong> <span class="layout-type-2">Two columns on the left — Sidebar first | Sidebar last | Content</span>'),
        ),
      );
      $form['at']['page_layout']['layout_enable_settings'] = array(
        '#type' => 'hidden',
        '#value' => theme_get_setting('layout_enable_settings'),
      );
    } // endif layout method
  } // endif layout settings
  // Breadcrumbs
  $form['at']['breadcrumb'] = array(
    '#type' => 'fieldset',
    '#weight' => '96',
    '#title' => t('Breadcrumb'),
  );
  $form['at']['breadcrumb']['breadcrumb_display'] = array(
    '#type' => 'select',
    '#title' => t('Display breadcrumb'),
    '#default_value' => theme_get_setting('breadcrumb_display'),
    '#options' => array(
      'yes' => t('Yes'),
      'no' => t('No'),
    ),
  );
  $form['at']['breadcrumb']['breadcrumb_separator'] = array(
    '#type'  => 'textfield',
    '#title' => t('Breadcrumb separator'),
    '#description' => t('Text only. Dont forget to include spaces.'),
    '#default_value' => theme_get_setting('breadcrumb_separator'),
    '#size' => 8,
    '#maxlength' => 10,
    '#states' => array(
      'visible' => array(
          '#edit-breadcrumb-display' => array(
            'value' => 'yes',
          ),
      ),
    ),
  );
  $form['at']['breadcrumb']['breadcrumb_home'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the homepage link in breadcrumbs'),
    '#default_value' => theme_get_setting('breadcrumb_home'),
    '#states' => array(
      'visible' => array(
          '#edit-breadcrumb-display' => array(
            'value' => 'yes',
          ),
      ),
    ),
  );
  // Search Settings
  $form['at']['search_results'] = array(
    '#type' => 'fieldset',
    '#weight' => '97',
    '#title' => t('Search Results'),
    '#description' => t('What additional information should be displayed in your search results?'),
  );
  $form['at']['search_results']['search_snippet'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display text snippet'),
    '#default_value' => theme_get_setting('search_snippet'),
  );
  $form['at']['search_results']['search_info_type'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display content type'),
    '#default_value' => theme_get_setting('search_info_type'),
  );
  $form['at']['search_results']['search_info_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display author name'),
    '#default_value' => theme_get_setting('search_info_user'),
  );
  $form['at']['search_results']['search_info_date'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display posted date'),
    '#default_value' => theme_get_setting('search_info_date'),
  );
  $form['at']['search_results']['search_info_comment'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display comment count'),
    '#default_value' => theme_get_setting('search_info_comment'),
  );
  $form['at']['search_results']['search_info_upload'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display attachment count'),
    '#default_value' => theme_get_setting('search_info_upload'),
  );
  // Search_info_separator
  $form['at']['search_results']['search_info_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Search info separator'),
    '#description' => t('Modify the separator. The default is a hypen with a space before and after.'),
    '#default_value' => theme_get_setting('search_info_separator'),
    '#size' => 8,
    '#maxlength' => 10,
  );
  // Horizonatal login block
  if (theme_get_setting('horizontal_login_block_enable') == 'on') {
    $form['at']['login_block'] = array(
      '#type' => 'fieldset',
      '#weight' => '99',
      '#title' => t('Login Block'),
    );
    $form['at']['login_block']['horizontal_login_block'] = array(
      '#type' => 'checkbox',
      '#title' => t('Horizontal Login Block'),
      '#default_value' => theme_get_setting('horizontal_login_block'),
      '#description' => t('Checking this setting will enable a horizontal style login block (all elements on one line). Note that if you are using OpenID this does not work well and you will need a more sophistocated approach than can be provided here.'),
    );
  } // endif horizontal block settings
  // Development settings
  $form['at']['dev'] = array(
    '#type' => 'fieldset',
    '#weight' => '100',
    '#title' => t('Development'),
    '#description' => t('These settings allow you to add or remove CSS classes and markup from the output. WARNING: These settings are for the theme developer! Changing these settings may break your site. Make sure you really know what you are doing before changing these.'),
  );
  // Add or remove extra classes
  $form['at']['dev']['classes'] = array(
    '#type' => 'container',
    '#title' => t('CSS Classes'),
    '#description' => t('xc_theme prints many heplful classes for templates by default - these settings allow you to expand on these by adding extra classes to pages, articles, comments, blocks, menus and item lists.'),
  );
  $form['at']['dev']['classes']['extra_page_classes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pages: ') . '<span class="description">' . t('add page-path, add/edit/delete (for workflow states), and a language class (i18n).') . '</span>',
    '#default_value' => theme_get_setting('extra_page_classes'),
  );
  $form['at']['dev']['classes']['extra_article_classes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Articles: ') . '<span class="description">' . t('add promoted, sticky, preview, language, odd/even classes and build mode classes such as .article-teaser and .article-full.') . '</span>',
    '#default_value' => theme_get_setting('extra_article_classes'),
  );
  $form['at']['dev']['classes']['extra_comment_classes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Comments: ') . '<span class="description">' . t('add anonymous, author, viewer, new, and odd/even classes.') . '</span>',
    '#default_value' => theme_get_setting('extra_comment_classes'),
  );
  $form['at']['dev']['classes']['extra_block_classes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Blocks: ') . '<span class="description">' . t('add odd/even, block region and block count classes.') . '</span>',
    '#default_value' => theme_get_setting('extra_block_classes'),
  );
  $form['at']['dev']['classes']['extra_menu_classes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Menus: ') . '<span class="description">' . t('add an extra class based on the menu link ID (mlid).') . '</span>',
    '#default_value' => theme_get_setting('extra_menu_classes'),
  );
  $form['at']['dev']['classes']['extra_item_list_classes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Item-lists: ') . '<span class="description">' . t('add first, last and odd/even classes.') . '</span>',
    '#default_value' => theme_get_setting('extra_item_list_classes'),
  );
  // Menu Links Settings
  $form['at']['dev']['menu_links'] = array(
    '#type' => 'container',
    '#title' => t('Modify Links'),
    '#description' => t('Inject span tags into menu items to help out themers.'),
  );
  // Add spans to theme_links
  $form['at']['dev']['menu_links']['menu_item_span_elements'] = array(
    '#type' => 'checkbox',
    '#title' => t('Wrap menu item text in <code><span></code> elements. ') . '<span class="description">' . t('Note: this does not work for Superfish menus, which includes its own feature for doing this.') . '</span>',
    '#default_value' => theme_get_setting('menu_item_span_elements'),
  );
}

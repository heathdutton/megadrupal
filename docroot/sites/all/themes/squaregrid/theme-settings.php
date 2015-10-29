<?php
// Responsive design support settings
function squaregrid_form_system_theme_settings_alter(&$form, &$form_state) {
  // Workaround for core bug http://drupal.org/node/943212 affecting admin themes.
  if (isset($form_id)) {
    return;
  }

// @TODO: Rewrite theme settings as appropriate for fluid grid.

// Theme development tools fieldset
$form['squaregrid_dev_settings'] = array(
  '#type'   => 'fieldset',
  '#title'  => t('Theme Development'),
  '#description' => t(''),
  // '#collapsible' => TRUE,
  // '#collapsed' => TRUE,
  '#weight' => '-50',
);

    // checkbox option to display the grid
    $form['squaregrid_dev_settings']['showgrid'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display the grid'),
      '#default_value' => theme_get_setting('showgrid'),
      '#description' => t('Show outlines on blocks and reveal grid values. Descriptive, but not pretty.'),
    );

       // Field subgrouping - visible only for responsive options
      $form['squaregrid_dev_settings']['devtip'] = array(
        '#type'   => 'item',
        '#description' => t('<strong>Note:</strong> The <em>rebuild theme registry</em> setting has been removed from theme settings to follow best practices. During theme development, use <a href="!link">Devel module</a> for settings to refresh your templates on every pageload.', array('!link' => 'http://drupal.org/project/devel')),
        '#states' => array(
          'invisible' => array(
            ':input[name="showgrid"]' => array('checked' => FALSE),
          ),
        ),
      );


// Fieldgroup: Browser support widget settings
$form['html5_browser_settings'] = array(
  '#type'   => 'fieldset',
  '#title'  => t('Browser support'),
//  '#description' => t('Use these settings to add javascript-based HTML5 support for older browsers. Disable this if you prefer to add support in your own theme or via methods such as <a href="!link">Modernizr</a>.', array('!link' => 'http://drupal.org/project/modernizr')),
  '#weight' => '-45',
);

  $form['html5_browser_settings']['html5shiv'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Add html5shiv.js script to every page.'),
      '#default_value' => theme_get_setting('html5shiv'),
      '#description'   => t('This script is the defacto way to enable use of HTML5 sectioning elements in legacy Internet Explorer, as well as default HTML5 styling in Internet Explorer 6 - 9, Safari 4.x (and iPhone 3.x), and Firefox 3.x. For more info, read <a href="!link">The Story of the HTML5 Shiv</a>.', array('!link' => 'http://paulirish.com/2011/the-history-of-the-html5-shiv/')),
    );

  $form['html5_browser_settings']['mobilewebapp'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Allow iOS Safari to operate in "mobile web app" mode.'),
      '#default_value' => theme_get_setting('mobilewebapp'),
      '#description'   => t('On iOS, you can set the browser to run in full-screen mode. Be sure you want to do this. For more info, read <a href="!link">Apple Developer documentation on Apple-Specific Meta Tag Keys</a>.', array('!link' => 'https://developer.apple.com/library/safari/#documentation/appleapplications/reference/SafariHTMLRef/Articles/MetaTags.html')),
    );

// Fieldgroup: Width settings
$form['max_mobile'] = array(
  '#type'   => 'fieldset',
  '#title'  => t('Responsive Design Breakpoint and Maximum Width'),
  '#description' => t(''),
  '#weight' => '-40',
);

   // Selection for design break point between mobile and grid
    $form['max_mobile']['break_point'] = array(
      '#type'     => 'select',
      '#title'    => t('Maximum mobile width:'),
      '#required' => TRUE,
      '#options'  => array(
        'break360'     => t('360px'),
        'break480'     => t('480px'),
        'break640'     => t('640px'),
        'break768'     => t('768px'),
        'break800'     => t('800px'),
        ),
      '#description' => t('IN DEVELOPMENT. Common device widths are presented as options. On devices and browser windows wider than this setting, the theme will be rendered in the grid layout. Your selection depends upon your design.'),
      '#default_value' => theme_get_setting('break_point'),
    );

    // Selection for maximum width of grid layout
    $form['max_mobile']['max_width'] = array(
      '#type'       => 'select',
      '#title'      => t('Maximum grid layout width:'),
      '#required'   => TRUE,
      '#options'    => array(
        'px900'     => t('900px'),
        'px1000'    => t('1000px'),
        'px1100'    => t('1100px'),
        'px1200'    => t('1200px'),
        'px1300'    => t('1300px'),
        'px1400'    => t('1400px'),
        'px1500'    => t('1500px'),
        'pc100'     => t('no limit'),
        ),
      '#description' => t('No matter how wide the browser, the content layout will be no wider than this setting. On narrower browser windows and devices, the grid will scale proportionately in a fluid manner.'),
      '#default_value' => theme_get_setting('max_width'),
    );

// Fieldgroup: Width settings
// TODO: define realistic width points, pull variables into preprocess page function
$form['grid_widths'] = array(
  '#type'   => 'fieldset',
  '#title'  => t('Column widths and placement'),
  '#description' => t('All widths are defined in terms of square grid units. The grid is 35 units across. For example, a main content area of 21 grid units would be 21/35ths of the page width, or 60%. It is recommended to leave a 1-unit gutter between content areas. However, this may vary depending upon your design and theming techniques, so you have full control over width and placement of each column. <em>Displayed percentages are rounded from actual values.</em>'),
  '#weight' => '-30',
);

    // Fieldgroup: Layout settings
    $form['grid_widths']['columns3'] = array(
      '#type'   => 'fieldset',
      '#title'  => t('3-column settings'),
      '#description' => t('<strong>These settings apply <em>only</em> when both "supplementary content" regions are displaying.</strong> Use the positioning settings to set the order of the columns.'),
      '#weight' => '-20',
    );

      // Fieldgroup: 3-column settings
      $form['grid_widths']['columns3']['set_main_3'] = array(
        '#type'   => 'fieldset',
        '#title'  => t('Main content'),
        '#description' => t(''),
      );

        // width of main content when 3 columns
        $form['grid_widths']['columns3']['set_main_3']['c3_main_width'] = array(
          '#type'     => 'select',
          '#title'    => t('Grid width of main content:'),
          '#required' => TRUE,
          '#options'  => array(
            'sg-7'   => t(' 7 grid squares = 20%'),
            'sg-8'   => t(' 8 grid squares = 22.9%'),
            'sg-9'   => t(' 9 grid squares = 25.7%'),
            'sg-10'  => t('10 grid squares = 28.6%'),
            'sg-11'  => t('11 grid squares = 31.4%'),
            'sg-12'  => t('12 grid squares = 34.3%'),
            'sg-13'  => t('13 grid squares = 37.1%'),
            'sg-14'  => t('14 grid squares = 40%'),
            'sg-15'  => t('15 grid squares = 42.9%'),
            'sg-16'  => t('16 grid squares = 45.7%'),
            'sg-17'  => t('17 grid squares = 48.6%'),
            'sg-18'  => t('18 grid squares = 51.4% **(default)**'),
            'sg-19'  => t('19 grid squares = 54.3%'),
            'sg-20'  => t('20 grid squares = 57.1%'),
            'sg-21'  => t('21 grid squares = 60%'),
            'sg-22'  => t('22 grid squares = 62.9%'),
            'sg-23'  => t('23 grid squares = 65.7%'),
            'sg-24'  => t('24 grid squares = 68.6%'),
            'sg-25'  => t('25 grid squares = 71.4%'),
            'sg-26'  => t('26 grid squares = 74.3%'),
            'sg-27'  => t('27 grid squares = 77.1%'),
            'sg-28'  => t('28 grid squares = 80%'),
            ),
          '#description' => t(''),
          '#default_value' => theme_get_setting('c3_main_width'),
        );

        // placement of main content when 3 columns
        $form['grid_widths']['columns3']['set_main_3']['c3_main_push'] = array(
          '#type'     => 'select',
          '#title'    => t('Position of main content:'),
          '#required' => TRUE,
          '#options'  => array(
            'push-0'          => t('Flush left'),
            'push-1'    => t('--> 1 grid square'),
            'push-2'   => t('--> 2 grid squares'),
            'push-3'   => t('-->  3 grid squares'),
            'push-4'   => t('-->  4 grid squares'),
            'push-5'   => t('-->  5 grid squares'),
            'push-6'   => t('-->  6 grid squares'),
            'push-7'   => t('-->  7 grid squares'),
            'push-8'   => t('-->  8 grid squares'),
            'push-9'   => t('-->  9 grid squares **(default)**'),
            'push-10'  => t('--> 10 grid squares'),
            'push-11'  => t('--> 11 grid squares'),
            'push-12'  => t('--> 12 grid squares'),
            'push-13'  => t('--> 13 grid squares'),
            'push-14'  => t('--> 14 grid squares'),
            'push-15'  => t('--> 15 grid squares'),
            'push-16'  => t('--> 16 grid squares'),
            'push-17'  => t('--> 17 grid squares'),
            'push-18'  => t('--> 18 grid squares'),
            'push-19'  => t('--> 19 grid squares'),
            'push-20'  => t('--> 20 grid squares'),
            'push-21'  => t('--> 21 grid squares'),
            'push-22'  => t('--> 22 grid squares'),
            'push-23'  => t('--> 23 grid squares'),
            'push-24'  => t('--> 24 grid squares'),
            'push-25'  => t('--> 25 grid squares'),
            'push-26'  => t('--> 26 grid squares'),
            'push-27'  => t('--> 27 grid squares'),
            'push-28'  => t('--> 28 grid squares'),
            'push-29'  => t('--> 29 grid squares'),
            'push-30'  => t('--> 30 grid squares'),
            'push-31'  => t('--> 31 grid squares'),
            'push-32'  => t('--> 32 grid squares'),
            'push-33'  => t('--> 33 grid squares'),
            'push-34'  => t('--> 34 grid squares'),
            ),
          '#default_value' => theme_get_setting('c3_main_push'),
          '#description' => t('Set to a value matching the sum of grid values of any regions set to display to the left, or set to flush placement. Add one more grid value to add a one-grid gutter between regions.'),
        );

      // Fieldgroup: Primary supplementary settings
      $form['grid_widths']['columns3']['c3_supp1'] = array(
        '#type'   => 'fieldset',
        '#title'  => t('Primary supplementary content'),
        '#description' => t(''),
      );

        // width of supplementary 1 when 3 columns
        $form['grid_widths']['columns3']['c3_supp1']['c3_supp1_width'] = array(
          '#type'     => 'select',
          '#title'    => t('Grid width of primary supplementary content:'),
          '#required' => TRUE,
          '#options'  => array(
            'sg-1'   => t(' 1 grid square =  2.9%'),
            'sg-2'   => t(' 2 grid squares =  5.7%'),
            'sg-3'   => t(' 3 grid squares =  8.6%'),
            'sg-4'   => t(' 4 grid squares = 11.4%'),
            'sg-5'   => t(' 5 grid squares = 14.3%'),
            'sg-6'   => t(' 6 grid squares = 17.1%'),
            'sg-7'   => t(' 7 grid squares = 20%'),
            'sg-8'   => t(' 8 grid squares = 22.9% **(default)**'),
            'sg-9'   => t(' 9 grid squares = 25.7%'),
            'sg-10'  => t('10 grid squares = 28.6%'),
            'sg-11'  => t('11 grid squares = 31.4%'),
            'sg-12'  => t('12 grid squares = 34.3%'),
            'sg-13'  => t('13 grid squares = 37.1%'),
            'sg-14'  => t('14 grid squares = 40%'),
            'sg-15'  => t('15 grid squares = 42.9%'),
            'sg-16'  => t('16 grid squares = 45.7%'),
            'sg-17'  => t('17 grid squares = 48.6%'),
            'sg-18'  => t('18 grid squares = 51.4%'),
            'sg-19'  => t('19 grid squares = 54.3%'),
            'sg-20'  => t('20 grid squares = 57.1%'),
            'sg-21'  => t('21 grid squares = 60%'),
            ),
          '#default_value' => theme_get_setting('c3_supp1_width'),
        );

        // placement of supp 1 content when 3 columns
        $form['grid_widths']['columns3']['c3_supp1']['c3_supp1_push'] = array(
          '#type'     => 'select',
          '#title'    => t('Position of primary supplementary content:'),
          '#required' => TRUE,
          '#options'  => array(
            'push-0'          => t('Flush left **(default)**'),
            'push-1'    => t('--> 1 grid square'),
            'push-2'   => t('--> 2 grid squares'),
            'push-3'   => t('-->  3 grid squares'),
            'push-4'   => t('-->  4 grid squares'),
            'push-5'   => t('-->  5 grid squares'),
            'push-6'   => t('-->  6 grid squares'),
            'push-7'   => t('-->  7 grid squares'),
            'push-8'   => t('-->  8 grid squares'),
            'push-9'   => t('-->  9 grid squares'),
            'push-10'  => t('--> 10 grid squares'),
            'push-11'  => t('--> 11 grid squares'),
            'push-12'  => t('--> 12 grid squares'),
            'push-13'  => t('--> 13 grid squares'),
            'push-14'  => t('--> 14 grid squares'),
            'push-15'  => t('--> 15 grid squares'),
            'push-16'  => t('--> 16 grid squares'),
            'push-17'  => t('--> 17 grid squares'),
            'push-18'  => t('--> 18 grid squares'),
            'push-19'  => t('--> 19 grid squares'),
            'push-20'  => t('--> 20 grid squares'),
            'push-21'  => t('--> 21 grid squares'),
            'push-22'  => t('--> 22 grid squares'),
            'push-23'  => t('--> 23 grid squares'),
            'push-24'  => t('--> 24 grid squares'),
            'push-25'  => t('--> 25 grid squares'),
            'push-26'  => t('--> 26 grid squares'),
            'push-27'  => t('--> 27 grid squares'),
            'push-28'  => t('--> 28 grid squares'),
            'push-29'  => t('--> 29 grid squares'),
            'push-30'  => t('--> 30 grid squares'),
            'push-31'  => t('--> 31 grid squares'),
            'push-32'  => t('--> 32 grid squares'),
            'push-33'  => t('--> 33 grid squares'),
            'push-34'  => t('--> 34 grid squares'),
            ),
          '#default_value' => theme_get_setting('c3_supp1_push'),
          '#description' => t('Set to a value matching the sum of grid values of any regions set to display to the left, or set to flush placement. Add one more grid value to add a one-grid gutter between regions.'),
        );

      // Fieldgroup: Secondary supplementary settings
      $form['grid_widths']['columns3']['c3_supp2'] = array(
        '#type'   => 'fieldset',
        '#title'  => t('Secondary supplementary content'),
        '#description' => t(''),
      );

      // width of supplementary 2 when 3 columns
      $form['grid_widths']['columns3']['c3_supp2']['c3_supp2_width'] = array(
        '#type'     => 'select',
        '#title'    => t('Grid width of secondary supplementary content:'),
        '#required' => TRUE,
        '#options'  => array(
          'sg-1'   => t(' 1 grid square =  2.9%'),
          'sg-2'   => t(' 2 grid squares =  5.7%'),
          'sg-3'   => t(' 3 grid squares =  8.6%'),
          'sg-4'   => t(' 4 grid squares = 11.4%'),
          'sg-5'   => t(' 5 grid squares = 14.3%'),
          'sg-6'   => t(' 6 grid squares = 17.1%'),
          'sg-7'   => t(' 7 grid squares = 20% **(default)**'),
          'sg-8'   => t(' 8 grid squares = 22.9%'),
          'sg-9'   => t(' 9 grid squares = 25.7%'),
          'sg-10'  => t('10 grid squares = 28.6%'),
          'sg-11'  => t('11 grid squares = 31.4%'),
          'sg-12'  => t('12 grid squares = 34.3%'),
          'sg-13'  => t('13 grid squares = 37.1%'),
          'sg-14'  => t('14 grid squares = 40%'),
          'sg-15'  => t('15 grid squares = 42.9%'),
          'sg-16'  => t('16 grid squares = 45.7%'),
          'sg-17'  => t('17 grid squares = 48.6%'),
          'sg-18'  => t('18 grid squares = 51.4%'),
          'sg-19'  => t('19 grid squares = 54.3%'),
          'sg-20'  => t('20 grid squares = 57.1%'),
          'sg-21'  => t('21 grid squares = 60%'),
          ),
        '#default_value' => theme_get_setting('c3_supp2_width'),
      );

      // placement of supp 2 content when 3 columns
      $form['grid_widths']['columns3']['c3_supp2']['c3_supp2_push'] = array(
        '#type'     => 'select',
        '#title'    => t('Position of secondary supplmentary content:'),
        '#required' => TRUE,
        '#options'  => array(
          'push-0'   => t('Flush left'),
          'push-1'   => t('--> 1 grid square'),
          'push-2'   => t('--> 2 grid squares'),
          'push-3'   => t('-->  3 grid squares'),
          'push-4'   => t('-->  4 grid squares'),
          'push-5'   => t('-->  5 grid squares'),
          'push-6'   => t('-->  6 grid squares'),
          'push-7'   => t('-->  7 grid squares'),
          'push-8'   => t('-->  8 grid squares'),
          'push-9'   => t('-->  9 grid squares'),
          'push-10'  => t('--> 10 grid squares'),
          'push-11'  => t('--> 11 grid squares'),
          'push-12'  => t('--> 12 grid squares'),
          'push-13'  => t('--> 13 grid squares'),
          'push-14'  => t('--> 14 grid squares'),
          'push-15'  => t('--> 15 grid squares'),
          'push-16'  => t('--> 16 grid squares'),
          'push-17'  => t('--> 17 grid squares'),
          'push-18'  => t('--> 18 grid squares'),
          'push-19'  => t('--> 19 grid squares'),
          'push-20'  => t('--> 20 grid squares'),
          'push-21'  => t('--> 21 grid squares'),
          'push-22'  => t('--> 22 grid squares'),
          'push-23'  => t('--> 23 grid squares'),
          'push-24'  => t('--> 24 grid squares'),
          'push-25'  => t('--> 25 grid squares'),
          'push-26'  => t('--> 26 grid squares'),
          'push-27'  => t('--> 27 grid squares'),
          'push-28'  => t('--> 28 grid squares **(default)**'),
          'push-29'  => t('--> 29 grid squares'),
          'push-30'  => t('--> 30 grid squares'),
          'push-31'  => t('--> 31 grid squares'),
          'push-32'  => t('--> 32 grid squares'),
          'push-33'  => t('--> 33 grid squares'),
          'push-34'  => t('--> 34 grid squares'),
          ),
        '#default_value' => theme_get_setting('c3_supp2_push'),
        '#description' => t('Set to a value matching the sum of grid values of any regions set to display to the left, or set to flush placement. Add one more grid value to add a one-grid gutter between regions.'),
      );

    // Fieldgroup: Layout settings
    $form['grid_widths']['columns2'] = array(
      '#type'   => 'fieldset',
      '#title'  => t('2-column settings'),
      '#description' => t('<strong>These settings apply <em>only</em> when one "supplementary content" region is displaying.</strong> Use the positioning settings to set the order of the columns.'),
      '#weight' => '-15',
    );

      // Fieldgroup: 2-column settings
      $form['grid_widths']['columns2']['c2_s1'] = array(
        '#type'   => 'fieldset',
        '#title'  => t('Main plus primary supplementary'),
        '#description' => t('These settings apply only with main content and <em>primary</em> supplementary content displaying.'),
      );

      // Fieldgroup: 2-column settings
      $form['grid_widths']['columns2']['c2_s1']['c2_s1_main'] = array(
        '#type'   => 'fieldset',
        '#title'  => t('Main content'),
        '#description' => t(''),
      );

        // width of main content when 2 columns
        $form['grid_widths']['columns2']['c2_s1']['c2_s1_main']['c2_s1_main_width'] = array(
          '#type'     => 'select',
          '#title'    => t('Grid width of main content:'),
          '#required' => TRUE,
          '#options'  => array(
            'sg-7'   => t(' 7 grid squares = 20%'),
            'sg-8'   => t(' 8 grid squares = 22.9%'),
            'sg-9'   => t(' 9 grid squares = 25.7%'),
            'sg-10'  => t('10 grid squares = 28.6%'),
            'sg-11'  => t('11 grid squares = 31.4%'),
            'sg-12'  => t('12 grid squares = 34.3%'),
            'sg-13'  => t('13 grid squares = 37.1%'),
            'sg-14'  => t('14 grid squares = 40%'),
            'sg-15'  => t('15 grid squares = 42.9%'),
            'sg-16'  => t('16 grid squares = 45.7%'),
            'sg-17'  => t('17 grid squares = 48.6%'),
            'sg-18'  => t('18 grid squares = 51.4%'),
            'sg-19'  => t('19 grid squares = 54.3%'),
            'sg-20'  => t('20 grid squares = 57.1%'),
            'sg-21'  => t('21 grid squares = 60% **(default)**'),
            'sg-22'  => t('22 grid squares = 62.9%'),
            'sg-23'  => t('23 grid squares = 65.7%'),
            'sg-24'  => t('24 grid squares = 68.6%'),
            'sg-25'  => t('25 grid squares = 71.4%'),
            'sg-26'  => t('26 grid squares = 74.3%'),
            'sg-27'  => t('27 grid squares = 77.1%'),
            'sg-28'  => t('28 grid squares = 80%'),
            ),
          '#description' => t(''),
          '#default_value' => theme_get_setting('c2_s1_main_width'),
        );

        // placement of main content when 2 columns
        $form['grid_widths']['columns2']['c2_s1']['c2_s1_main']['c2_s1_main_push'] = array(
          '#type'     => 'select',
          '#title'    => t('Position of main content:'),
          '#required' => TRUE,
          '#options'  => array(
            'push-0'  => t('Flush left'),
            'push-1'   => t('--> 1 grid square'),
            'push-2'   => t('--> 2 grid squares'),
            'push-3'   => t('-->  3 grid squares'),
            'push-4'   => t('-->  4 grid squares'),
            'push-5'   => t('-->  5 grid squares'),
            'push-6'   => t('-->  6 grid squares'),
            'push-7'   => t('-->  7 grid squares'),
            'push-8'   => t('-->  8 grid squares'),
            'push-9'   => t('-->  9 grid squares'),
            'push-10'  => t('--> 10 grid squares'),
            'push-11'  => t('--> 11 grid squares'),
            'push-12'  => t('--> 12 grid squares'),
            'push-13'  => t('--> 13 grid squares'),
            'push-14'  => t('--> 14 grid squares **(default)**'),
            'push-15'  => t('--> 15 grid squares'),
            'push-16'  => t('--> 16 grid squares'),
            'push-17'  => t('--> 17 grid squares'),
            'push-18'  => t('--> 18 grid squares'),
            'push-19'  => t('--> 19 grid squares'),
            'push-20'  => t('--> 20 grid squares'),
            'push-21'  => t('--> 21 grid squares'),
            ),
          '#default_value' => theme_get_setting('c2_s1_main_push'),
          '#description' => t('Set to a value matching the sum of grid values of any regions set to display to the left, or set to flush placement. Add one more grid value to add a one-grid gutter between regions.'),
        );

      // Fieldgroup: Primary supplementary settings
      $form['grid_widths']['columns2']['c2_s1']['c2_s1_supp1'] = array(
        '#type'   => 'fieldset',
        '#title'  => t('Primary supplementary content'),
        '#description' => t(''),
      );

        // width of supplementary 1 when 2 columns
        $form['grid_widths']['columns2']['c2_s1']['c2_s1_supp1']['c2_s1_supp1_width'] = array(
          '#type'     => 'select',
          '#title'    => t('Grid width of primary supplementary content:'),
          '#required' => TRUE,
          '#options'  => array(
            'sg-1'   => t(' 1 grid square =  2.9%'),
            'sg-2'   => t(' 2 grid squares =  5.7%'),
            'sg-3'   => t(' 3 grid squares =  8.6%'),
            'sg-4'   => t(' 4 grid squares = 11.4%'),
            'sg-5'   => t(' 5 grid squares = 14.3%'),
            'sg-6'   => t(' 6 grid squares = 17.1%'),
            'sg-7'   => t(' 7 grid squares = 20%'),
            'sg-8'   => t(' 8 grid squares = 22.9%'),
            'sg-9'   => t(' 9 grid squares = 25.7%'),
            'sg-10'  => t('10 grid squares = 28.6%'),
            'sg-11'  => t('11 grid squares = 31.4%'),
            'sg-12'  => t('12 grid squares = 34.3%'),
            'sg-13'  => t('13 grid squares = 37.1% **(default)**'),
            'sg-14'  => t('14 grid squares = 40%'),
            'sg-15'  => t('15 grid squares = 42.9%'),
            'sg-16'  => t('16 grid squares = 45.7%'),
            'sg-17'  => t('17 grid squares = 48.6%'),
            'sg-18'  => t('18 grid squares = 51.4%'),
            'sg-19'  => t('19 grid squares = 54.3%'),
            'sg-20'  => t('20 grid squares = 57.1%'),
            'sg-21'  => t('21 grid squares = 60%'),
            ),
          '#default_value' => theme_get_setting('c2_s1_supp1_width'),
        );

        // placement of supp 1 content when 2 columns
        $form['grid_widths']['columns2']['c2_s1']['c2_s1_supp1']['c2_s1_supp1_push'] = array(
          '#type'     => 'select',
          '#title'    => t('Position of primary supplementary content:'),
          '#required' => TRUE,
          '#options'  => array(
            'push-0'          => t('Flush left **(default)**'),
            'push-1'    => t('--> 1 grid square'),
            'push-2'   => t('--> 2 grid squares'),
            'push-3'   => t('-->  3 grid squares'),
            'push-4'   => t('-->  4 grid squares'),
            'push-5'   => t('-->  5 grid squares'),
            'push-6'   => t('-->  6 grid squares'),
            'push-7'   => t('-->  7 grid squares'),
            'push-8'   => t('-->  8 grid squares'),
            'push-9'   => t('-->  9 grid squares'),
            'push-10'  => t('--> 10 grid squares'),
            'push-11'  => t('--> 11 grid squares'),
            'push-12'  => t('--> 12 grid squares'),
            'push-13'  => t('--> 13 grid squares'),
            'push-14'  => t('--> 14 grid squares'),
            'push-15'  => t('--> 15 grid squares'),
            'push-16'  => t('--> 16 grid squares'),
            'push-17'  => t('--> 17 grid squares'),
            'push-18'  => t('--> 18 grid squares'),
            'push-19'  => t('--> 19 grid squares'),
            'push-20'  => t('--> 20 grid squares'),
            'push-21'  => t('--> 21 grid squares'),
            'push-22'  => t('--> 22 grid squares'),
            'push-23'  => t('--> 23 grid squares'),
            'push-24'  => t('--> 24 grid squares'),
            'push-25'  => t('--> 25 grid squares'),
            'push-26'  => t('--> 26 grid squares'),
            'push-27'  => t('--> 27 grid squares'),
            'push-28'  => t('--> 28 grid squares'),
            'push-29'  => t('--> 29 grid squares'),
            'push-30'  => t('--> 30 grid squares'),
            'push-31'  => t('--> 31 grid squares'),
            'push-32'  => t('--> 32 grid squares'),
            'push-33'  => t('--> 33 grid squares'),
            'push-34'  => t('--> 34 grid squares'),
            ),
          '#default_value' => theme_get_setting('c2_s1_supp1_push'),
          '#description' => t('Set to a value matching the sum of grid values of any regions set to display to the left, or set to flush placement. Add one more grid value to add a one-grid gutter between regions.'),
        );


      // Fieldgroup: 2-column settings
      $form['grid_widths']['columns2']['c2_s2'] = array(
        '#type'   => 'fieldset',
        '#title'  => t('Main plus secondary supplementary'),
        '#description' => t('These settings apply only with main content and <em>secondary</em> supplementary content displaying.'),
      );

      // Fieldgroup: 2-column settings
      $form['grid_widths']['columns2']['c2_s2']['c2_s2_main'] = array(
        '#type'   => 'fieldset',
        '#title'  => t('Main content'),
        '#description' => t(''),
      );

        // width of main content when 3 columns
        $form['grid_widths']['columns2']['c2_s2']['c2_s2_main']['c2_s2_main_width'] = array(
          '#type'     => 'select',
          '#title'    => t('Grid width of main content:'),
          '#required' => TRUE,
          '#options'  => array(
            'sg-7'   => t(' 7 grid squares = 20%'),
            'sg-8'   => t(' 8 grid squares = 22.9%'),
            'sg-9'   => t(' 9 grid squares = 25.7%'),
            'sg-10'  => t('10 grid squares = 28.6%'),
            'sg-11'  => t('11 grid squares = 31.4%'),
            'sg-12'  => t('12 grid squares = 34.3%'),
            'sg-13'  => t('13 grid squares = 37.1%'),
            'sg-14'  => t('14 grid squares = 40%'),
            'sg-15'  => t('15 grid squares = 42.9%'),
            'sg-16'  => t('16 grid squares = 45.7%'),
            'sg-17'  => t('17 grid squares = 48.6%'),
            'sg-18'  => t('18 grid squares = 51.4%'),
            'sg-19'  => t('19 grid squares = 54.3%'),
            'sg-20'  => t('20 grid squares = 57.1%'),
            'sg-21'  => t('21 grid squares = 60% **(default)**'),
            'sg-22'  => t('22 grid squares = 62.9%'),
            'sg-23'  => t('23 grid squares = 65.7%'),
            'sg-24'  => t('24 grid squares = 68.6%'),
            'sg-25'  => t('25 grid squares = 71.4%'),
            'sg-26'  => t('26 grid squares = 74.3%'),
            'sg-27'  => t('27 grid squares = 77.1%'),
            'sg-28'  => t('28 grid squares = 80%'),
            ),
          '#description' => t(''),
          '#default_value' => theme_get_setting('c2_s2_main_width'),
        );

        // placement of main content when 2 columns
        $form['grid_widths']['columns2']['c2_s2']['c2_s2_main']['c2_s2_main_push'] = array(
          '#type'     => 'select',
          '#title'    => t('Position of main content:'),
          '#required' => TRUE,
          '#options'  => array(
            'push-0'  => t('Flush left **(default)**'),
            'push-1'   => t('--> 1 grid square'),
            'push-2'   => t('--> 2 grid squares'),
            'push-3'   => t('-->  3 grid squares'),
            'push-4'   => t('-->  4 grid squares'),
            'push-5'   => t('-->  5 grid squares'),
            'push-6'   => t('-->  6 grid squares'),
            'push-7'   => t('-->  7 grid squares'),
            'push-8'   => t('-->  8 grid squares'),
            'push-9'   => t('-->  9 grid squares'),
            'push-10'  => t('--> 10 grid squares'),
            'push-11'  => t('--> 11 grid squares'),
            'push-12'  => t('--> 12 grid squares'),
            'push-13'  => t('--> 13 grid squares'),
            'push-14'  => t('--> 14 grid squares'),
            'push-15'  => t('--> 15 grid squares'),
            'push-16'  => t('--> 16 grid squares'),
            'push-17'  => t('--> 17 grid squares'),
            'push-18'  => t('--> 18 grid squares'),
            'push-19'  => t('--> 19 grid squares'),
            'push-20'  => t('--> 20 grid squares'),
            'push-21'  => t('--> 21 grid squares'),
            ),
          '#default_value' => theme_get_setting('c2_s2_main_push'),
          '#description' => t('Set to a value matching the sum of grid values of any regions set to display to the left, or set to flush placement. Add one more grid value to add a one-grid gutter between regions.'),
        );

      // Fieldgroup: Primary supplementary settings
      $form['grid_widths']['columns2']['c2_s2']['c2_s2_supp2'] = array(
        '#type'   => 'fieldset',
        '#title'  => t('Secondary supplementary content'),
        '#description' => t(''),
      );

        // width of supplementary 1 when 3 columns
        $form['grid_widths']['columns2']['c2_s2']['c2_s2_supp2']['c2_s2_supp2_width'] = array(
          '#type'     => 'select',
          '#title'    => t('Grid width of secondary supplementary content:'),
          '#required' => TRUE,
          '#options'  => array(
            'sg-1'   => t(' 1 grid square =  2.9%'),
            'sg-2'   => t(' 2 grid squares =  5.7%'),
            'sg-3'   => t(' 3 grid squares =  8.6%'),
            'sg-4'   => t(' 4 grid squares = 11.4%'),
            'sg-5'   => t(' 5 grid squares = 14.3%'),
            'sg-6'   => t(' 6 grid squares = 17.1%'),
            'sg-7'   => t(' 7 grid squares = 20%'),
            'sg-8'   => t(' 8 grid squares = 22.9%'),
            'sg-9'   => t(' 9 grid squares = 25.7%'),
            'sg-10'  => t('10 grid squares = 28.6%'),
            'sg-11'  => t('11 grid squares = 31.4%'),
            'sg-12'  => t('12 grid squares = 34.3%'),
            'sg-13'  => t('13 grid squares = 37.1% **(default)**'),
            'sg-14'  => t('14 grid squares = 40%'),
            'sg-15'  => t('15 grid squares = 42.9%'),
            'sg-16'  => t('16 grid squares = 45.7%'),
            'sg-17'  => t('17 grid squares = 48.6%'),
            'sg-18'  => t('18 grid squares = 51.4%'),
            'sg-19'  => t('19 grid squares = 54.3%'),
            'sg-20'  => t('20 grid squares = 57.1%'),
            'sg-21'  => t('21 grid squares = 60%'),
            ),
          '#default_value' => theme_get_setting('c2_s2_supp2_width'),
        );

        // placement of supp 1 content when 3 columns
        $form['grid_widths']['columns2']['c2_s2']['c2_s2_supp2']['c2_s2_supp2_push'] = array(
          '#type'     => 'select',
          '#title'    => t('Position of secondary supplementary content:'),
          '#required' => TRUE,
          '#options'  => array(
            'push-0'          => t('Flush left'),
            'push-1'    => t('--> 1 grid square'),
            'push-2'   => t('--> 2 grid squares'),
            'push-3'   => t('-->  3 grid squares'),
            'push-4'   => t('-->  4 grid squares'),
            'push-5'   => t('-->  5 grid squares'),
            'push-6'   => t('-->  6 grid squares'),
            'push-7'   => t('-->  7 grid squares'),
            'push-8'   => t('-->  8 grid squares'),
            'push-9'   => t('-->  9 grid squares'),
            'push-10'  => t('--> 10 grid squares'),
            'push-11'  => t('--> 11 grid squares'),
            'push-12'  => t('--> 12 grid squares'),
            'push-13'  => t('--> 13 grid squares'),
            'push-14'  => t('--> 14 grid squares'),
            'push-15'  => t('--> 15 grid squares'),
            'push-16'  => t('--> 16 grid squares'),
            'push-17'  => t('--> 17 grid squares'),
            'push-18'  => t('--> 18 grid squares'),
            'push-19'  => t('--> 19 grid squares'),
            'push-20'  => t('--> 20 grid squares'),
            'push-21'  => t('--> 21 grid squares'),
            'push-22'  => t('--> 22 grid squares **(default)**'),
            'push-23'  => t('--> 23 grid squares'),
            'push-24'  => t('--> 24 grid squares'),
            'push-25'  => t('--> 25 grid squares'),
            'push-26'  => t('--> 26 grid squares'),
            'push-27'  => t('--> 27 grid squares'),
            'push-28'  => t('--> 28 grid squares'),
            'push-29'  => t('--> 29 grid squares'),
            'push-30'  => t('--> 30 grid squares'),
            'push-31'  => t('--> 31 grid squares'),
            'push-32'  => t('--> 32 grid squares'),
            'push-33'  => t('--> 33 grid squares'),
            'push-34'  => t('--> 34 grid squares'),
            ),
          '#default_value' => theme_get_setting('c2_s2_supp2_push'),
          '#description' => t('Set to a value matching the sum of grid values of any regions set to display to the left, or set to flush placement. Add one more grid value to add a one-grid gutter between regions.'),
        );

    // Fieldgroup: one-golumn settings
    $form['grid_widths']['columns1'] = array(
      '#type'   => 'fieldset',
      '#title'  => t('1-column settings'),
      '#description' => t('These settings apply <em>only</em> when the main content region displays alone, with no "sidebars".'),
      '#weight' => '-10',
    );

       // width of main content when 3 columns
        $form['grid_widths']['columns1']['c1_main_width'] = array(
          '#type'     => 'select',
          '#title'    => t('Grid width of main content:'),
          '#required' => TRUE,
          '#options'  => array(
            'sg-18'  => t('18 grid squares = 51.4%'),
            'sg-19'  => t('19 grid squares = 54.3%'),
            'sg-20'  => t('20 grid squares = 57.1%'),
            'sg-21'  => t('21 grid squares = 60%'),
            'sg-22'  => t('22 grid squares = 62.9%'),
            'sg-23'  => t('23 grid squares = 65.7%'),
            'sg-24'  => t('24 grid squares = 68.6%'),
            'sg-25'  => t('25 grid squares = 71.4%'),
            'sg-26'  => t('26 grid squares = 74.3%'),
            'sg-27'  => t('27 grid squares = 77.1%'),
            'sg-28'  => t('28 grid squares = 80%'),
            'sg-29'  => t('29 grid squares = '),
            'sg-30'  => t('30 grid squares = '),
            'sg-31'  => t('31 grid squares = '),
            'sg-32'  => t('32 grid squares = '),
            'sg-33'  => t('33 grid squares = '),
            'sg-34'  => t('34 grid squares = '),
            'sg-35'  => t('35 grid squares = 100% **(default)**'),
            ),
          '#description' => t(''),
          '#default_value' => theme_get_setting('c1_main_width'),
        );

        // placement of main content when 2 columns
        $form['grid_widths']['columns1']['c1_main_push'] = array(
          '#type'     => 'select',
          '#title'    => t('Position of main content:'),
          '#required' => TRUE,
          '#options'  => array(
            'push-0'  => t('Flush left **(default)**'),
            'push-1'   => t('--> 1 grid square'),
            'push-2'   => t('--> 2 grid squares'),
            'push-3'   => t('-->  3 grid squares'),
            'push-4'   => t('-->  4 grid squares'),
            'push-5'   => t('-->  5 grid squares'),
            'push-6'   => t('-->  6 grid squares'),
            'push-7'   => t('-->  7 grid squares'),
            'push-8'   => t('-->  8 grid squares'),
            'push-9'   => t('-->  9 grid squares'),
            'push-10'  => t('--> 10 grid squares'),
           ),
          '#default_value' => theme_get_setting('c1_main_push'),
          '#description' => t('Set to a value matching the sum of grid values of any regions set to display to the left, or set to flush placement. Add one more grid value to push main content in from left page edge.'),
        );





}

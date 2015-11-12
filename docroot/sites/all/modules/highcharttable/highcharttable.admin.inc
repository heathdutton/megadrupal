<?php

/**
 * @file
 * highcharttable.admin.inc
 *
 * Configuration options for HighchartTable module.
 */

/**
 * Menu callback for admin configuration settings.
 */
function highcharttable_admin_configure_form($form, &$form_state) {

  $page = filter_input(INPUT_GET, 'destination');

  // Retain tree-hierarchy in values, prevent the form from being flattened.
  $form['#tree'] = TRUE;

  // Use array_values to make sure keys are numeric and start at 0.
  $decorations = array_values(variable_get('highcharttable_decorations', array()));

  if (!isset($form_state['num-decorations'])) {
    $form_state['num-decorations'] = max(0, count($decorations));
  }

  $form['decorations'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t('Chart decorations on this site'),
    // The following id refers to the #ajax wrappers below.
    '#prefix' => '<div id="decorations-wrapper">',
    '#suffix' => '</div>',
  );
  $form['decorations']['#attached']['css'][] = drupal_get_path('module', 'highcharttable') . '/css/highcharttable.admin.css';

  for ($i = 0; $i < $form_state['num-decorations']; $i++) {
    $decoration_params = $decorations[$i]['decoration-params'];
    $form['decorations'][$i] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => !empty($page) && !_highcharttable_path_matches_decoration($page, $decorations[$i]),
      '#title' => t('Chart decoration #@no', array('@no' => $i + 1)),
    );
    $form['decorations'][$i]['decoration-params'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="decoration-params">',
      '#suffix' => '</div>',
    );
    $form['decorations'][$i]['decoration-params']['chart-type'] = array(
      '#type' => 'select',
      '#title' => t('Chart type'),
      '#options' => array(
        'column' => t('Column (Bar)'),
        'line' => t('Line'),
        'spline' => t('Spline (smoothed line)'),
        'area' => t('Area'),
        'pie' => t('Pie'),
      ),
      '#default_value' => isset($decoration_params['chart-type']) ? $decoration_params['chart-type'] : 'column',
      '#description' => t('The type of chart to appear above the table.'),
    );
    $form['decorations'][$i]['decoration-params']['subtitle'] = array(
      '#type' => 'textfield',
      '#size' => 30,
      '#title' => t('Chart subtitle'),
      // Note that Highcharts JS accepts very few HTML tags.
      '#default_value' => isset($decoration_params['subtitle']) ? filter_xss_admin($decoration_params['subtitle']) : '',
      '#description' => t('You may use the <b>&lt;b>bold&lt;/b></b> and <i>&lt;i>italic&lt;/i></i> tags. The <em>subtitle</em> appears above the chart. The <em>title</em> is taken from the table caption, if present.'),
    );
    $form['decorations'][$i]['decoration-params']['legend'] = array(
      '#type' => 'select',
      '#title' => t('Legend style'),
      '#options' => array(
        '' => t('No legend'),
        'horizontal' => t('Items shown horizontally'),
        'vertical' => t('Items shown vertically'),
      ),
      '#default_value' => isset($decoration_params['legend']) ? $decoration_params['legend'] : 'horizontal',
      '#description' => t("The chart's legend style."),
    );
    $form['decorations'][$i]['decoration-params']['color-1'] = array(
      '#type' => 'textfield',
      '#size' => 30,
      '#title' => t('Colour 1'),
      '#default_value' => isset($decoration_params['color-1']) ? $decoration_params['color-1'] : '',
      '#description' => t('Color of the first series. You may use a #RGB code, like <em>#ffb6c1</em> or one of the <a target="colornames" href="@url">standard color names</a>, like <em>lightpink</em>. <br/>Leave blank to use the HighchartTable presets.', array(
        '@url' => url('http://www.w3schools.com/tags/ref_colornames.asp'),
      )),
    );
    $form['decorations'][$i]['decoration-params']['color-2'] = array(
      '#type' => 'textfield',
      '#size' => 30,
      '#title' => t('Colour 2'),
      '#default_value' => isset($decoration_params['color-2']) ? $decoration_params['color-2'] : '',
      '#description' => t('Color of the second series, if present.'),
    );
    $form['decorations'][$i]['decoration-params']['color-3'] = array(
      '#type' => 'textfield',
      '#size' => 30,
      '#title' => t('Colour 3'),
      '#default_value' => isset($decoration_params['color-3']) ? $decoration_params['color-3'] : '',
      '#description' => t('Color of the third series, if present.'),
    );
    $form['decorations'][$i]['decoration-params']['xaxis'] = array(
      '#type' => 'textfield',
      '#size' => 3,
      '#title' => t('Column to use as the primary (horizontal) axis'),
      '#default_value' => isset($decoration_params['xaxis']) ? (int)$decoration_params['xaxis'] : 1,
      '#description' => t('The leftmost column is column 1.<br/>For this feature to work you must select the patched variant of the HighchartTable library (bottom of this page).'),
    );
    $form['decorations'][$i]['decoration-params']['formatter'] = array(
      '#type' => 'select',
      '#title' => t('Vertical axis label formatter'),
      '#options' => array(
        '' => t('Highcharts default'),
        'highcharttableDollarFormatter' => t('prepend $'),
        'highcharttableEuroFormatter' => t('prepend €'),
        'highcharttablePoundFormatter' => t('prepend £'),
        'highcharttableNumberFormatter1Decimal' => t('1 decimal'),
        'highcharttableNumberFormatter2Decimals' => t('2 decimals'),
        'highcharttableNumberFormatter3Decimals' => t('3 decimals'),
        'highcharttableDollarFormatter2Decimals' => t('$ + 2 decimals'),
        'highcharttableEuroFormatter2Decimals' => t('€ + 2 decimals'),
        'highcharttablePoundFormatter2Decimals' => t('£ + 2 decimals'),
        'highcharttablePercentFormatter' => t('Percentage (append %)'),
      ),
      '#default_value' => isset($decoration_params['formatter']) ? $decoration_params['formatter'] : '',
      '#description' => t('Does not apply to Pie charts.'),
      // This works fine when chart-type widget is 'radios', not when 'select'.
      '#states' => array(
        'invisible' => array("input[name='decorations[$i][decoration-params][chart-type]'" => array('value' => 'pie')),
      ),
    );
    $form['decorations'][$i]['decoration-params']['suppress-invalid-series'] = array(
      '#type' => 'select',
      '#title' => t('Invalid data'),
      '#options' => array(
        0 => t('Do not suppress'),
        1 => t('Suppress columns with 1 or more invalid cells'),
        2 => t('Suppress columns with no valid cells'),
      ),
      '#default_value' => isset($decoration_params['suppress-invalid-series']) ? $decoration_params['suppress-invalid-series'] : 2,
      '#description' => t('Does not apply to the horizontal axis column.'),
    );
    $form['decorations'][$i]['decoration-params']['height'] = array(
      '#type' => 'textfield',
      '#size' => 5,
      '#title' => t('Height'),
      '#field_suffix' => ' ' . t('px'),
      '#default_value' => isset($decoration_params['height']) ? $decoration_params['height'] : '',
      '#description' => t('The height of the chart in pixels.'),
    );
    $form['decorations'][$i]['decoration-params']['swap-axes'] = array(
      '#type' => 'checkbox',
      '#title' => t('Swap axes'),
      '#default_value' => isset($decoration_params['swap-axes']) ? $decoration_params['swap-axes'] : FALSE,
      '#description' => t('Invert horizontal and vertical axes. Does not apply to Pie charts.<br/><strong>Example:</strong> ticking this box for a <em>Column</em> chart will transform it into a horizontal bar chart.'),
      // This works fine when chart-type widget is 'radios', not when 'select'.
      '#states' => array(
        'invisible' => array("input[name='decorations[$i][decoration-params][chart-type]'" => array('value' => 'pie')),
      ),
    );
    $form['decorations'][$i]['decoration-params']['labels'] = array(
      '#type' => 'checkbox',
      '#title' => t('Superimpose data labels'),
      '#default_value' => isset($decoration_params['labels']) ? $decoration_params['labels'] : FALSE,
      '#description' => t('Tick to display the value of each data point.'),
    );

    $form['decorations'][$i]['pages-and-selector'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="pages-and-selector">',
      '#suffix' => '</div>',
    );
    $pages_and_selector = $decorations[$i]['pages-and-selector'];
    $form['decorations'][$i]['pages-and-selector']['include-pages'] = array(
      '#type' => 'textarea',
      '#rows' => 2,
      '#title' => t('Pages containing tables to be adorned with the above chart decoration'),
      '#default_value' => isset($pages_and_selector['include-pages']) ? $pages_and_selector['include-pages'] : '',
      '#description' => t('Enter relative paths, one per line. Do not start with a slash. You may use path aliases. <em>&lt;front&gt;</em> means the front page. The asterisk <em>*</em> is the wildcard character, while a leading tilde <em>~</em> is the exclusion character. So <em>~admin/*</em> denotes all pages that do <em>not</em> have a path starting with <em>admin/</em>.<br/>Blank out this field to remove the entire chart decoration.'),
    );
    $form['decorations'][$i]['pages-and-selector']['table-selector'] = array(
      '#type' => 'textfield',
      '#size' => 32,
      '#title' => t('CSS-selector for tables targeted'),
      '#default_value' => isset($pages_and_selector['table-selector']) ? $pages_and_selector['table-selector'] : '',
      '#description' => t('If you have only one table on each of the pages mentioned on the left, then you can leave this empty. If you have more and wish to decorate a subset, then target individual tables using a CSS/jQuery selector.<br/><strong>Example:</strong> <em>table:eq(1)</em>&nbsp; targets the 2nd table on the page only.'),
    );
    $form['decorations'][$i]['pages-and-selector']['hide-table'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide table'),
      '#default_value' => isset($pages_and_selector['hide-table']) ? $pages_and_selector['hide-table'] : FALSE,
      '#description' => t('Tick to hide the table that the chart was generated from.'),
    );
  }

  $form['decorations']['add-another'] = array(
    '#type' => 'submit',
    '#value' => empty($form_state['num-decorations']) ? t('Add chart decoration') : t('Add another chart decoration'),
    '#weight' => 1,
    '#submit' => array('_highcharttable_add_decoration_submit'),
    '#ajax' => array(
      'callback' => '_highcharttable_decoration_js',
      'wrapper' => 'decorations-wrapper',
      // 'fade', 'none' or 'slide'.
      'effect' => 'fade',
      // 'fast', 'slow' or number of millisec.
      'speed' => 'slow',
    ),
  );
  if ($form_state['num-decorations'] > 0) {
    $form['decorations']['remove'] = array(
      '#type' => 'submit',
      '#value' => t('Remove last decoration'),
      '#weight' => 2,
      '#submit' => array('_highcharttable_remove_decoration_submit'),
      '#ajax' => array(
        'callback' => '_highcharttable_decoration_js',
        'wrapper' => 'decorations-wrapper',
        // 'fade', 'none' or 'slide'.
        'effect' => 'fade',
        // 'fast', 'slow' or number of millisec.
        'speed' => 'fast',
      ),
    );
  }

  $global_settings = variable_get('highcharttable_global_settings', array());
  $form['global-settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Global settings'),
  );
  $form['global-settings']['use-patched-library'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the patched variant of the HighchartTable library'),
    '#default_value' => isset($global_settings['use-patched-library']) ? $global_settings['use-patched-library'] : TRUE,
    '#description' => t('The patched variant comes included with the module. It does not need to be downloaded. However you do need to have the Highcharts JS library on your system.'),
  );
  $form['global-settings']['animation'] = array(
    '#type' => 'select',
    '#title' => t('Animation'),
    '#options' => array(
      '' => t('Highcharts default'),
      'fast' => t('Faster'),
      'slow' => t('Slower'),
      'off' => t('Off'),
    ),
    '#default_value' => isset($global_settings['animation']) ? $global_settings['animation'] : '',
    '#description' => t('If you want to generate PDFs select <em>Off</em>.'),
  );
  $form['global-settings']['contextual-links'] = array(
    '#type' => 'radios',
    '#title' => t('Contextual links to insert, configure and delete charts'),
    '#options' => array(
      '' => t('Links visible to privileged users through the cogwheel symbol when hovered'),
      'plain' => t('Plain links, always visible to privileged users'),
      'off' => t('Off, no contextual links'),
    ),
    '#default_value' => isset($global_settings['contextual-links']) ? $global_settings['contextual-links'] : '',
    '#description' => t('The first two options allow users with the <em>Add and configure chart decorations</em> permission to insert a chart above a table through a single click.'),
  );
  $form['global-settings']['contextual-links-exclude-pages'] = array(
    '#type' => 'textarea',
    '#rows' => 6,
    '#title' => t('Pages that must <em>not</em> display the contextual links mentioned above'),
    '#default_value' => isset($global_settings['contextual-links-exclude-pages']) ? $global_settings['contextual-links-exclude-pages'] : HIGHCHARTTABLE_CONTEXTUAL_LINKS_EXCLUDE_PAGES,
    '#description' => t('Enter relative paths, one per line. Do not start with a slash. You may use path aliases. The asterisk <em>*</em> is the wildcard character, while a leading tilde <em>~</em> is the exclusion character. So <em>~admin/*</em> denotes all pages that do <em>not</em> have a path starting with <em>admin/</em>.'),
    '#states' => array(
      'invisible' => array('input[name="global-settings[contextual-links]"' => array('value' => 'off')),
    ),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  //$form['#submit'][] = 'highcharttable_admin_configure_form_submit';
  $form['#theme'] = 'system_settings_form';
  return $form;
}

/**
 * Submit handler for the "Add another decoration" button.
 *
 * Increments the counter and forces a form rebuild.
 */
function _highcharttable_add_decoration_submit($form, &$form_state) {
  $form_state['num-decorations']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "Remove" button.
 *
 * Decrements the counter and forces a form rebuild.
 */
function _highcharttable_remove_decoration_submit($form, &$form_state) {
  if ($form_state['num-decorations'] > 0) {
    $form_state['num-decorations']--;
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Ajax callback in response to new rows.
 *
 * At this point the $form has already been rebuilt. All we have to do here is
 * tell AJAX what part of the browser form needs to be updated.
 */
function _highcharttable_decoration_js($form, &$form_state) {
  // Return the updated table, so that ajax.inc can issue commands to the
  // browser to update only the targeted sections of the page.
  return $form['decorations'];
}

/**
 * Validate the configuration form submission.
 *
 * @param array $form
 *   The submitted configuration form.
 * @param array $form_state
 *   The submitted configuration form state.
 */
function highcharttable_admin_configure_form_validate(&$form, &$form_state) {
  // Clear out the form of stuff we do not wish to save, like buttons.
  form_state_values_clean($form_state);

  foreach ($form_state['values']['decorations'] as $key => $decoration) {
    if (empty($decoration['pages-and-selector']['include-pages'])) {
      // If a decoration has no pages to show itself on, remove it altogether.
      unset($form_state['values']['decorations'][$key]);
    }
  }
  // Make sure that keys are numeric, consecutive and start at 0.
  $form_state['values']['decorations'] = array_values($form_state['values']['decorations']);
}

/**
 * Form submit handler for admin settings.
 *
 * @param array $form
 *   The form
 * @param array $form_state
 *   The form state
 */
function highcharttable_admin_configure_form_submit($form, &$form_state) {

  variable_set('highcharttable_decorations', $form_state['values']['decorations']);
  variable_set('highcharttable_global_settings', $form_state['values']['global-settings']);

  // A change in the library files to be included requires clearing of the
  // Libraries cache. A call to libraries_flush_caches() is not sufficient here.
  // @todo maybe check whether libraries were actually changed...
  cache_clear_all('*', 'cache_libraries', TRUE);

  drupal_set_message(t('The chart decorations have been saved.'));
}

<?php

/**
 * @file
 * label_trash.admin.inc
 *
 * Configuration options for Label Trash module.
 */

/**
 * Menu callback for admin configuration settings.
 */
function label_trash_admin_configure($form, &$form_state) {

  // Retain tree-hierarchy in values, prevent the form from being flattened.
  $form['#tree'] = TRUE;

  $decorations = variable_get('label_trash_decorations', array());
  if (!isset($form_state['num-decorations'])) {
    $form_state['num-decorations'] = max(0, count($decorations));
  }

  $form['decorations'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t('Input field decorations'),
    // The following id refers to the #ajax wrappers below.
    '#prefix' => '<div id="decorations-wrapper">',
    '#suffix' => '</div>',
  );
  $form['decorations']['#attached']['css'][] = drupal_get_path('module', 'label_trash') . '/css/label_trash.admin.css';

  for ($i = 1; $i <= $form_state['num-decorations']; $i++) {
    $form['decorations'][$i] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#title' => t('Input field decoration #@no', array('@no' => $i)),
    );
    $decoration_params = $decorations[$i]['decoration-params'];
    $form['decorations'][$i]['decoration-params'] = array(
      '#type' => 'markup',
      '#prefix' => '<div class="decoration-params">',
      '#suffix' => '</div>',
    );
    $form['decorations'][$i]['decoration-params']['label-style'] = array(
      '#type' => 'radios',
      '#title' => t('Convert field label'),
      '#options' => array(
        0 => t('to a static placeholder'),
        1 => t('to a floating placeholder'),
      ),
      '#default_value' => isset($decoration_params['label-style']) ? $decoration_params['label-style'] : 1,
      '#description' => t(''),
    );
    $form['decorations'][$i]['decoration-params']['transition-duration'] = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#title' => t('Animation duration'),
      '#field_suffix' => ' ' . t('seconds'),
      '#default_value' => isset($decoration_params['transition-duration']) ? $decoration_params['transition-duration'] : LABEL_TRASH_DEFAULT_TRANSISTION_TIME,
      '#description' => t('Values from 0 to 0.3 are common.'),
      '#states' => array(
        'invisible' => array(
          'input[name="decorations[' . $i . '][decoration-params][label-style]"]' => array(
            'value' => 0,
          ),
        ),
      ),
    );
    $form['decorations'][$i]['decoration-params']['floatlabel-offset-top-end'] = array(
      '#type' => 'textfield',
      '#size' => 10,
      '#title' => t('Float label offset'),
      '#default_value' => isset($decoration_params['floatlabel-offset-top-end']) ? $decoration_params['floatlabel-offset-top-end'] : LABEL_TRASH_DEFAULT_VERTICAL_OFFSET,
      '#description' => t('Measured vertically down from the top of the input box. You may use any unit, e.g., <strong>px</strong> or <strong>em</strong>.'),
      '#states' => array(
        'invisible' => array(
          'input[name="decorations[' . $i . '][decoration-params][label-style]"]' => array(
            'value' => 0,
          ),
        ),
      ),
    );
    /*
    $form['decorations'][$i]['decoration-params']['label-css'] = array(
      '#title' => t('Font label styling'),
      '#type' => 'markup',
      '#prefix' => '<div class="decoration-params-label-css">',
      '#suffix' => '</div>',
    );*/
    $form['decorations'][$i]['decoration-params']['label-css']['data-class'] = array(
      '#type' => 'textfield',
      '#size' => 32,
      '#title' => t('Float label CSS class name'),
      '#default_value' => isset($decoration_params['label-css']['data-class']) ? $decoration_params['label-css']['data-class'] : '',
      '#description' => t('Optional. Either new or existing in a loaded stylesheet. If new, specify CSS properties in the next box.'),
      '#states' => array(
        'invisible' => array(
          'input[name="decorations[' . $i . '][decoration-params][label-style]"]' => array(
            'value' => 0,
          ),
        ),
      ),
    );
    $form['decorations'][$i]['decoration-params']['label-css']['properties'] = array(
      '#type' => 'textfield',
      '#size' => 32,
      '#title' => t('Float label CSS properties'),
      '#default_value' => isset($decoration_params['label-css']['properties']) ? $decoration_params['label-css']['properties'] : '',
      '#description' => t('<em>property:value</em> pairs separated by semi-colons. Example: <strong>color: red;&nbsp;&nbsp;margin-left: 10px</strong>'),
      '#states' => array(
        'invisible' => array(
          'input[name="decorations[' . $i . '][decoration-params][label-style]"]' => array(
            'value' => 0,
          ),
        ),
        'invisible' => array(
          'input[name="decorations[' . $i . '][decoration-params][label-css][data-class]"' => array(
            'value' => '',
          ),
        ),
      ),
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
      '#title' => t('Pages to be adorned with the above decoraton'),
      '#default_value' => isset($pages_and_selector['include-pages']) ? $pages_and_selector['include-pages'] : LABEL_TRASH_DEFAULT_PAGE_INCLUSIONS,
      '#description' => t("Enter relative paths, one per line. Do not start with a slash. You may use path aliases. <em>&lt;front&gt;</em> means the front page. The asterisk <em>*</em> is the wildcard character, i.e. <em>admin/*</em> denotes all pages that have a path starting with <em>admin/</em>"),
    );
    $form['decorations'][$i]['pages-and-selector']['exclude-pages'] = array(
      '#type' => 'textarea',
      '#rows' => 3,
      '#title' => t('Exceptions: pages excluded'),
      '#default_value' => isset($pages_and_selector['exclude-pages']) ? $pages_and_selector['exclude-pages'] : LABEL_TRASH_DEFAULT_PAGE_EXCLUSIONS,
      '#description' => t('One relative path per line.'),
    );
    $form['decorations'][$i]['pages-and-selector']['label-selector'] = array(
      '#type' => 'textfield',
      '#size' => 50,
      '#title' => t('jQuery selector for targeted input fields'),
      '#default_value' => isset($pages_and_selector['label-selector']) ? $pages_and_selector['label-selector'] : '',
      '#description' => t('Defaults to <strong>@default-selector</strong>, which is usually ok. If you use the patched JS, you can target <strong>textarea</strong>s too.', array(
        '@default-selector' => LABEL_TRASH_DEFAULT_LABEL_SELECTOR,
      )),
    );
  }

  $form['decorations']['add-another'] = array(
    '#type' => 'submit',
    '#value' => empty($form_state['num-decorations']) ? t('Add input field decoration') : t('Add another input field decoration'),
    '#weight' => 1,
    '#submit' => array('_label_trash_add_decoration_submit'),
    '#ajax' => array(
      'callback' => '_label_trash_decoration_js',
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
      '#submit' => array('_label_trash_remove_decoration_submit'),
      '#ajax' => array(
        'callback' => '_label_trash_decoration_js',
        'wrapper' => 'decorations-wrapper',
        // 'fade', 'none' or 'slide'.
        'effect' => 'fade',
        // 'fast', 'slow' or number of millisec.
        'speed' => 'fast',
      ),
    );
  }

  $form['global-settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Global settings'),
  );
  $global_settings = variable_get('label_trash_global_settings', array());

  $form['global-settings']['label-implementation'] = array(
    '#type' => 'radios',
    '#title' => t('Select Float Label implementation'),
    '#options' => array(
      LABEL_TRASH_ORIGINAL_LABEL_IMPLEMENTATION => t('floatlabel.js by <a href="@clubdesign">ClubDesign</a>: <a href="@floatlabel">download</a> and unzip to <em>sites/all/libraries</em>; also configure <a target="project" href="@project">jQuery update</a> for v1.8 or later', array(
        '@clubdesign' => url('http://clubdesign.github.io/floatlabels.js'),
        '@floatlabel' => url('https://github.com/clubdesign/floatlabels.js/tarball/master'),
        '@project' => url('http://drupal.org/project/jquery_update'),
      )),
     LABEL_TRASH_PATCHED_LABEL_IMPLEMENTATION => t("floatlabel.patched.js: included to work out of the box with core's jQuery as is"),
    ),
    '#default_value' => isset($global_settings['label-implementation']) ? (int)$global_settings['label-implementation'] : LABEL_TRASH_PATCHED_LABEL_IMPLEMENTATION,
    '#description' => t('The patched variant comes included with this module. It does not need to be downloaded.'),
  );
  $form['global-settings']['use-module-css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Label Trash styling'),
    '#default_value' => isset($global_settings['use-module-css']) ? $global_settings['use-module-css'] : TRUE,
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['#submit'][] = 'label_trash_admin_configure_form_submit';
  $form['#theme'] = 'system_settings_form';
  return $form;
}

/**
 * Submit handler for the "Add another decoration" button.
 *
 * Increments the counter and forces a form rebuild.
 */
function _label_trash_add_decoration_submit($form, &$form_state) {
  $form_state['num-decorations']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "Remove" button.
 *
 * Decrements the counter and forces a form rebuild.
 */
function _label_trash_remove_decoration_submit($form, &$form_state) {
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
function _label_trash_decoration_js($form, &$form_state) {
  // Return the updated table, so that ajax.inc can issue commands to the
  // browser to update only the targeted sections of the page.
  return $form['decorations'];
}

/**
 * Form submit handler for admin settings.
 *
 * @param array $form
 *   The form
 * @param array $form_state
 *   The form state
 */
function label_trash_admin_configure_form_submit($form, &$form_state) {

  // Clear out the form from stuff, like buttons, we do not wish to save.
  // @todo recursively sanitize (check_plain) all leaf values?)
  form_state_values_clean($form_state);

  variable_set('label_trash_decorations', $form_state['values']['decorations']);
  variable_set('label_trash_global_settings', $form_state['values']['global-settings']);

  // A change in the library files to be included requires clearing of the
  // Libraries cache. A call to libraries_flush_caches() is not sufficient here.
  cache_clear_all('*', 'cache_libraries', TRUE);

  drupal_set_message(t('Decorations and global configuration have been saved.'));
}

<?php

/**
 * @file
 * PageFlip MegaZine3 module administrative interface.
 */


/**
 * Build our admin settings form.
 */
function pageflip_mz3_viewer_settings_form($form_state) {
  $form = array();

  $form['megazine3_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to MegaZine3'),
    '#description' => t("Filesystem path to MegaZine3 relative to this Drupal installation. No leading or trailing /. This directory must contain the MZ3 distribution's <em>js</em> and <em>megazine</em> folders."),
    '#default_value' => variable_get('pageflip_mz3_viewer_megazine3_path', 'sites/all/libraries/mz3'),
  );
  $form['flash_fallback'] = array(
    '#type' => 'textarea',
    '#title' => ('Flash fallback content'),
    '#description' => t('The default no-Flash message in case Flash does not load. If you only enter a nid (node id), this node will be displayed.'),
    '#default_value' => variable_get('pageflip_mz3_viewer_flash_fallback', t(PAGEFLIP_MZ3_VIEWER_FLASH_FALLBACK)),
  );
  $form['back_cover_nid'] = array(
    '#type' => 'textfield',
    '#title' => t('Back cover node id'),
    '#description' => t('Node id of a PageFlip Page to use as the back cover for all books viewed in the MegaZine3 Viewer.'),
    '#default_value' => variable_get('pageflip_mz3_viewer_back_cover_nid', NULL),
  );

  // Potential future work: Add a field for selecting which ad group(s) to pull ads from

  $form['player_params'] = array(
    '#type' => 'fieldset',
    '#title' => t('MegaZine3 player parameters'),
  );
  $form['player_params']['allow_fullscreen'] = array(
    '#type' => 'select',
    '#title' => t('Allow fullscreen?'),
    '#options' => array(
      'true' => t('Yes'),
      'false' => t('No'),
    ),
    '#default_value' => variable_get('pageflip_mz3_viewer_allow_fullscreen', 'true'),
  );
  $form['player_params']['bg_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#default_value' => variable_get('pageflip_mz3_viewer_bg_color', '#333333'),
  );

  $form['tags'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tag attributes'),
  );
  $form['tags']['book_attributes'] = array(
    '#type' => 'textarea',
    '#title' => t('Book tag attributes (global default)'),
    '#description' => t('Ex: <em>pagewidth="640" pageheight="720"</em> . This text will be included verbatim within opening book tags.'),
    '#default_value' => pageflip_mz3_viewer_book_attributes(),
  );
  $form['tags']['front_cover_attributes'] = array(
    '#type' => 'textarea',
    '#title' => t('Front cover page tag attributes (global default)'),
    '#description' => t('Ex: <em>stiff="true"</em> . This text will be included verbatim within the opening page tag for all front covers.'),
    '#default_value' => variable_get('pageflip_mz3_viewer_front_cover_attributes', 'stiff="true"'),
  );
  $form['tags']['page_attributes'] = array(
    '#type' => 'textarea',
    '#title' => t('Normal page tag attributes (global default)'),
    '#description' => t('Ex: <em>stiff="true"</em> . This text will be included verbatim within the opening page tags for all inside pages.'),
    '#default_value' => variable_get('pageflip_mz3_viewer_page_attributes', ''),
  );
  $form['tags']['back_cover_attributes'] = array(
    '#type' => 'textarea',
    '#title' => t('Back cover page tag attributes (global default)'),
    '#description' => t('Ex: <em>stiff="true"</em> . This text will be included verbatim within the opening page tag for all back covers.'),
    '#default_value' => variable_get('pageflip_mz3_viewer_back_cover_attributes', 'stiff="true"'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}

/**
 * Validate our admin settings form.
 */
function pageflip_mz3_viewer_settings_form_validate($form, &$form_state) {
  // make sure we can find these files under the path given on the settings form
  $files = array(
    '/js/swfobject.js',
    '/js/swfaddress.js',
    '/js/expressInstall.swf',
    '/megazine/megazine.js',
    '/megazine/megazine.swf',
    '/megazine/preloader.swf',
  );
  foreach ($files as $file) {
    $filename = $form_state['values']['megazine3_path'] . $file;
    if (!is_file($filename)) {
      form_set_error('megazine3_path', t("Can't find needed file @filename", array('@filename' => $filename)));
      return FALSE;
    }
  }
  if (!empty($form_state['values']['flash_fallback']) && is_numeric($form_state['values']['flash_fallback']) && !node_load($form_state['values']['flash_fallback'])) {
    form_set_error('flash_fallback', t("Can't load node @nid", array('@nid' => $form_state['values']['flash_fallback'])));
    return FALSE;
  }
  if (!node_load($form_state['values']['back_cover_nid'])) {
    form_set_error('back_cover_nid', t("Can't load node @nid", array('@nid' => $form_state['values']['back_cover_nid'])));
    return FALSE;
  }
  return TRUE;
}

/**
 * Process validated admin settings form.
 */
function pageflip_mz3_viewer_settings_form_submit($form, &$form_state) {
  $variables = array(
    'megazine3_path',
    'flash_fallback',
    'back_cover_nid',
    'allow_fullscreen',
    'bg_color',
    'book_attributes',
    'front_cover_attributes',
    'page_attributes',
    'back_cover_attributes',
  );
  foreach ($variables as $var) {
    variable_set('pageflip_mz3_viewer_'. $var, $form_state['values'][$var]);
  }
}

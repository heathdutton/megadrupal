<?php
// $Id$

/**
 * @file
 * dev_banner admin pages
 */


/**
 * Displays the form for the standard settings tab.
 *
 * @return
 *   array A structured array for use with Forms API.
 */
function dev_banner_admin_settings() {
  $banner_defs = _dev_banner_definitions();
  
  
  // path to custom images
  $custom_image_path = file_default_scheme() . ':/' . '/dev_banner';

  $form['dev_banner_general'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('General settings'),
    '#collapsible'   => TRUE,
    '#collapsed'     => FALSE,
  );
  $form['dev_banner_general']['dev_banner_enabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('enable Development Banner'),
    '#default_value' => variable_get('dev_banner_enabled', TRUE),
    '#description'   => t('Click to display Development Banner'),
  );

  $form['dev_banner_general']['dev_banner_position'] = array(
    '#type'          => 'select',
    '#title'         => t('Development Banner position'),
    '#options'       => array(
      'nw' => t('Top Left'),
      'ne' => t('Top Right'),
      'se' => t('Bottom Right'),
      'sw' => t('Bottom Left'),
    ),
    '#default_value' => variable_get('dev_banner_position', 'nw'),
    '#prefix'        => '<div class="dpo-start">',
    '#suffix'        => '</div><div class="clearfix"></div>',
    '#description'   => 'updating...',
  );
  $form['dev_banner_general']['dev_banner_sticky'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Make banner sticky'),
    '#description'   => t('If checked, the development banner will not scroll with the page.'),
    '#default_value' => variable_get('dev_banner_sticky', 1),
  );
  $form['dev_banner_images'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Select images'),
    '#collapsible'   => TRUE,
    '#collapsed'     => FALSE,
  );
  $form['dev_banner_images']['advice'] = array(
    '#type'          => 'markup',
    '#markup'        => t('<p>Select a set of images for displaying Development Banners.  You may create your own custom images. They must be in PNG format, measure 72px by 72px, and located in the dev_banner subdirectory of your files path (!fpath). Please see the README for more details.</p>', array('!fpath' => $custom_image_path)),
  );

  // build options
  $options = array();
  foreach ($banner_defs['styles'] as $style) {
    $options[] = preg_replace('@_@', ' ', t(ucfirst($style)));
  }
  
  $default_image = variable_get('dev_banner_image_set', 0);
  $form['dev_banner_images']['dev_banner_image_set'] = array(
    '#type'          => 'radios',
    '#options'       => $options,
    '#default_value' => $default_image,
    '#prefix'        => '<div class="si-start">',
    '#suffix'        => '</div>',
  );

  
  $form['mapping'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('URL mapping'),
    '#collapsible'   => TRUE,
    '#collapsed'     => FALSE,
  );
  $form['mapping']['advice'] = array(
    '#type'          => 'markup',
    '#markup'        => t('<p>Enter host names for your development, staging, and test servers. The name should contain only the fully qualified domain name; i.e. <em>www.myserver.com</em>. If the host name matches the current server, the corresponding banner will be displayed.</p><p>If the site includes a port number, include it; for "http://staging.anothersite.com:8080", you would enter "staging.anothersite.com:8080"</p><p>If the site\'s base URL is in a subdirectory, include it (but not the following slash); for "http://www.testsite.com/stage", you would enter "www.testsite.com/stage"</p>'),
  );
  $form['mapping']['dev_banner_url_devel'] = array(
    '#type'          => 'textfield',
    '#title'         => t('development server host name'),
    '#size'          => 60,
    '#maxlength'     => 128,
    '#default_value' => variable_get('dev_banner_url_devel', ''),
    '#collapsible'   => TRUE,
    '#collapsed'     => FALSE,
  );
  $form['mapping']['dev_banner_url_stage'] = array(
    '#type'          => 'textfield',
    '#title'         => t('staging server host name'),
    '#size'          => 60,
    '#maxlength'     => 128,
    '#default_value' => variable_get('dev_banner_url_stage', ''),
    '#collapsible'   => TRUE,
    '#collapsed'     => FALSE,
  );
  $form['mapping']['dev_banner_url_test'] = array(
    '#type'          => 'textfield',
    '#title'         => t('test server host name'),
    '#size'          => 60,
    '#maxlength'     => 128,
    '#default_value' => variable_get('dev_banner_url_test', ''),
    '#collapsible'   => TRUE,
    '#collapsed'     => FALSE,
  );

  return system_settings_form($form);
}


/***
* AJAX callback to return list of images for a placement
* 
* placement is arg(2)
* 
*/
function dev_banner_admin_refresh_images() {
  // get banner definitions
  $banner_defs = _dev_banner_definitions();
  $placement = arg(2);
  
  $var = array();

 // get images for the form
  foreach ($banner_defs['styles'] as $idx => $val) {
    $style = $val;
    $image_list = array();
    if ($style == 'custom') {
      $image_base = $banner_defs['custom'] . '/' . $placement;
      if (!file_prepare_directory($image_base, FILE_MODIFY_PERMISSIONS)) {
        // path doesn't exist; don't bother looking for custom images
        $image_base = NULL;
      }
    }
    else {
      $image_base = drupal_get_path('module', 'dev_banner') . '/images/' . $placement;
    }
    
    if ($image_base) {
      // build list of images
      $i = 0;
      foreach ($banner_defs['names'] as $banner) {
        $ipath = $image_base . '/' . $style . '_' . $banner . '.png';
        $title = "development banner";
        $var['ul#dev-banner-images-' . $idx . ' li.dbi-' . $i] = theme_image($variables = array('path' => $ipath, 'title' => $title, 'alt' => $title, 'width' => DEV_BANNER_IMAGE_WIDTH, 'height' => DEV_BANNER_IMAGE_HEIGHT));
        $i++;
      }
    }
  }

  return drupal_json_output($var);
}


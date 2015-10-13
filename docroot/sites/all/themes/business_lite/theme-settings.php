<?php // $Id$
include_once 'template.php';
/**
* Advanced theme settings for images in banner.
*/
function business_lite_form_system_theme_settings_alter(&$form, $form_state) {
      $form['orbit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Slider/Orbit transition speed'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  ); 

  $form['orbit']['settings']['banner_delay'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#title' => t('Choose the banner delay'),
    '#default_value' => theme_get_setting('banner_delay'),
  );
     $form['orbit']['settings']['animation_speed'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#title' => t('Choose the animation speed'),
    '#default_value' => theme_get_setting('animation_speed'),
  );
   $form['orbit']['settings']['caption_animation_speed'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#title' => t('Choose the caption animation speed'),
    '#default_value' => theme_get_setting('caption_animation_speed'),
  );
  // Banners elements
  $form['banner'] = array(
    '#type' => 'fieldset',
    '#title' => t('Banner configuration'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  /*$form['banner'] = array(
    '#type' => 'fieldset',
    '#title' => t('Banner managment'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  ); */



  // Image upload section ======================================================
  $banners = business_lite_get_banners();
  
  $form['banner']['images'] = array(
    '#type' => 'vertical_tabs',
    '#title' => t('Banner images'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $i = 0;
  foreach ($banners as $image_data) {
    $form['banner']['images'][$i] = array(
      '#type' => 'fieldset',
      //'#title' => t('Image !number', array('!number' => $i + 1)),
      '#title' => t('Image !number: !title', array('!number' => $i + 1, '!title' => $image_data['image_title'])),
      '#weight' => $i,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      // Add image config form to $form
      'image' => _business_lite_banner_form($image_data),
    );
    

    $i++;
  }

  $form['banner']['image_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload a new banner'),
    '#weight' => $i,
  );

  $form['#submit'][]   = 'business_lite_settings_submit';
  return $form;
}

/**
 * Save settings data.
 */
function business_lite_settings_submit($form, &$form_state) {
  $settings = array();
  
  // Update image field
  foreach ($form_state['input']['images'] as $image) {
    if (is_array($image)) {
      $image = $image['image'];

      if ($image['image_delete']) {
        // Delete banner file
        file_unmanaged_delete($image['image_path']);
        // Delete banner thumbnail file
        file_unmanaged_delete($image['image_thumb']);
      } else {
        // Update image
        $settings[] = $image;
      }
    }
  }

  // Check for a new uploaded file, and use that if available.
  if ($file = file_save_upload('image_upload')) {
    $file->status = FILE_STATUS_PERMANENT;
    if ($image = _business_lite_save_image($file)) {
      // Put new image into settings
      $settings[] = $image;
    }
  }
  
  // Save settings
  business_lite_set_banners($settings);
}

/**
 * Provvide default installation settings for marinelli.
 */
function _business_lite_install() {
  // Deafault data
  $file = new stdClass;
  $banners = array();
  // Source base for images

  $src_base_path = drupal_get_path('theme', 'business_lite');
  $default_banners = theme_get_setting('default_banners');
  //print_r($default_banners);
  // Put all image as banners
  foreach ($default_banners as $i => $data) {
    $file->uri = $src_base_path . '/' . $data['image_path'];
    $file->filename = $file->uri;

    $banner = _business_lite_save_image($file);
    unset($data['image_path']);
    $banner = array_merge($banner, $data);
    $banners[$i] = $banner;
  }

  // Save banner data
  business_lite_set_banners($banners);

  // Flag theme is installed
  variable_set('theme_business_lite_first_install', FALSE);
}


/**
 * Save file uploaded by user and generate setting to save.
 *
 * @param <file> $file
 *    File uploaded from user
 *
 * @param <string> $banner_folder
 *    Folder where save image
 *
 * @param <string> $banner_thumb_folder
 *    Folder where save image thumbnail
 *
 * @return <array>
 *    Array with file data.
 *    FALSE on error.
 */
function _business_lite_save_image($file, $banner_folder = 'public://banner/', $banner_thumb_folder = 'public://banner/thumb/') {
  // Check directory and create it (if not exist)
  _business_lite_check_dir($banner_folder);
  _business_lite_check_dir($banner_thumb_folder);

  $parts = pathinfo($file->filename);
  $destination = $banner_folder . $parts['basename'];
  $setting = array();

  $file->status = FILE_STATUS_PERMANENT;

  // Copy temporary image into banner folder
  if ($img = file_copy($file, $destination, FILE_EXISTS_REPLACE)) {
    // Generate image thumb
    $image = image_load($destination);
    $small_img = image_scale($image, 300, 100);
    $image->source = $banner_thumb_folder . $parts['basename'];
    image_save($image);

    // Set image info
    $setting['image_path'] = $destination;
    $setting['image_thumb'] = $image->source;
    $setting['image_title'] = '';
    $setting['image_description'] = '';
    $setting['image_url'] = '<front>';
    $setting['image_visibility'] = '*';
    return $setting;
  }

  return FALSE;
}

/**
 * Check if folder is available or create it.
 *
 * @param <string> $dir
 *    Folder to check
 */
function _business_lite_check_dir($dir) {
  // Normalize directory name
  $dir = file_stream_wrapper_uri_normalize($dir);

  // Create directory (if not exist)
  file_prepare_directory($dir,  FILE_CREATE_DIRECTORY);
}

/**
 * Generate form to mange banner informations
 *
 * @param <array> $image_data
 *    Array with image data
 *
 * @return <array>
 *    Form to manage image informations
 */
function _business_lite_banner_form($image_data) {
  $img_form = array();

  // Image preview
  $img_form['image_preview'] = array(
      '#markup' => theme('image', array('path' => $image_data['image_thumb'])),
  );

  // Image path
  $img_form['image_path'] = array(
      '#type' => 'hidden',
      '#value' => $image_data['image_path'],
  );

  // Thumbnail path
  $img_form['image_thumb'] = array(
      '#type' => 'hidden',
      '#value' => $image_data['image_thumb'],
  );
      // Image title
    $img_form['image_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Caption'),
      '#default_value' => $image_data['image_title'],
    );

    // Image description
    $img_form['image_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#default_value' => $image_data['image_description'],
    );

  // Link url
  $img_form['image_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Url'),
      '#default_value' => $image_data['image_url'],
  );

  // Delete image
  $img_form['image_delete'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete image.'),
      '#default_value' => FALSE,
  );
      // Image weight
    $img_form['image_weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#default_value' => $image_data['image_weight'],
    );

  return $img_form;
}


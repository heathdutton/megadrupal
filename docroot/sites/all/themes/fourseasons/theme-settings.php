<?php
include_once 'template.php';
/**
 * Implements hook_form_system_theme_settings_alter().
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */

function fourseasons_form_system_theme_settings_alter(&$form, &$form_state)  {

// Color picker
	$_path = drupal_get_path('theme', 'fourseasons');
	drupal_add_js($_path . '/scripts/colorpicker/js/colorpicker.js');
	drupal_add_css($_path . '/scripts/colorpicker/css/colorpicker.css');
	$js = "
    function colorPickerAttach(which) {
      jQuery(which).ColorPicker({
        onSubmit: function(hsb, hex, rgb) {
          jQuery(which).val(hex);
		  jQuery(which).ColorPickerHide();
        },
        onBeforeShow: function () {
          jQuery(this).ColorPickerSetColor(this.value);
        }
      })
      .bind('keyup', function(){
        jQuery(this).ColorPickerSetColor(this.value);
      });
    }
  ";
  drupal_add_js($js, 'inline');
  // end color picker

  $defaultcolor = (theme_get_setting('fourseasons-basecolor')) ? theme_get_setting('fourseasons-basecolor') : 'FF7302';
  
  $form['configuration'] = array(
    '#type' => 'fieldset',
	'#title' => t('Configuration'),
	'#collapsible' => false,
  );
  
  $form['configuration']['fourseasons-basecolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Four Seasons Base Color'),
	'#size' => 12,
	'#maxlength' => 6,
	'#description' => t('Choose the basecolor. Default is #FF7302'),
	'#default_value' => $defaultcolor,
	'#suffix' => "
      <script type='text/javascript'>
        jQuery(document).ready(function(){
          colorPickerAttach('#edit-fourseasons-basecolor');
        });
      </script>"
	);
	 $i = 0;
$form['header'] = array(
    '#type' => 'fieldset',
    '#title' => t('Header management'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

$form['header']['image_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload a new banner'),
    '#weight' => $i,
);
// Image upload section ======================================================

  $banners = fourseasons_get_banners();
  
  $form['header']['images'] = array(
    '#type' => 'vertical_tabs',
    '#title' => t('Header images'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  
  foreach ($banners as $image_data) {
    $form['header']['images'][$i] = array(
      '#type' => 'fieldset',
      '#title' => t('Image !number: !title', array('!number' => $i + 1, '!title' => $image_data['image_title'])),
      '#weight' => $i,
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      // Add image config form to $form
      'image' => _fourseasons_header_form($image_data),
    );

    $i++;
  }

  $form['#submit'][]   = 'fourseasons_settings_submit';

  return $form;
}

function fourseasons_settings_submit($form, &$form_state) {
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
    if ($image = _fourseasons_save_image($file)) {
      // Put new image into settings
      $settings[] = $image;
    }
  }

  // Save settings
  fourseasons_set_banners($settings);
}

function _fourseasons_check_dir($dir) {
  // Normalize directory name
  $dir = file_stream_wrapper_uri_normalize($dir);

  // Create directory (if not exist)
  file_prepare_directory($dir,  FILE_CREATE_DIRECTORY);
}

function _fourseasons_save_image($file, $banner_folder = 'public://banner/', $banner_thumb_folder = 'public://banner/thumb/') {
  // Check directory and create it (if not exist)
  _fourseasons_check_dir($banner_folder);
  _fourseasons_check_dir($banner_thumb_folder);

  $parts = pathinfo($file->filename);
  $destination = $banner_folder . $parts['basename'];
  $setting = array();

  $file->status = FILE_STATUS_PERMANENT;
  
  // Copy temporary image into banner folder
  if ($img = file_copy($file, $destination, FILE_EXISTS_REPLACE)) {
    // Generate image thumb
    $image = image_load($destination);
    $small_img = image_scale($image, 330, 70);
    $image->source = $banner_thumb_folder . $parts['basename'];
    image_save($image);
	
	//force the image to be the exact size while trying to maintain aspect ratio
	$_resize_image = image_load($destination);
		image_scale_and_crop($_resize_image, 990, 220);
		image_save($_resize_image);
	
    // Set image info
    $setting['image_path'] = $destination;
    $setting['image_thumb'] = $image->source;
    $setting['image_title'] = '';
    $setting['image_description'] = '';
    $setting['image_url'] = '<front>';
    $setting['image_published'] = TRUE;
    $setting['image_visibility'] = '*';

    return $setting;
  }
  
  return FALSE;
}
function _fourseasons_header_form($image_data) {
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
      '#title' => t('Title'),
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

    // Image visibility
    $img_form['image_visibility'] = array(
      '#type' => 'textarea',
      '#title' => t('Visibility'),
      '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
      '#default_value' => $image_data['image_visibility'],
    );
	
    // Image is published
    $img_form['image_published'] = array(
      '#type' => 'checkbox',
      '#title' => t('Published'),
      '#default_value' => $image_data['image_published'],
    );

    // Delete image
    $img_form['image_delete'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete image.'),
      '#default_value' => FALSE,
    );

    return $img_form;
}

function fourseasons_install() {
  // Deafault data
  $file = new stdClass;
  $banners = array();
  // Source base for images
  $src_base_path = drupal_get_path('theme', 'fourseasons');
  $default_banners = theme_get_setting('default_banners');
  // Put all image as banners
  foreach ($default_banners as $i => $data) {
    $file->uri = $src_base_path . '/' . $data['image_path'];
    $file->filename = $file->uri;

    $banner = _fourseasons_save_image($file);
    unset($data['image_path']);
    $banner = array_merge($banner, $data);
    $banners[$i] = $banner;
  }

  // Save banner data
  fourseasons_set_banners($banners);

  // Flag theme is installed
  variable_set('theme_fourseasons_first_install', FALSE);
}
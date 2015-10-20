<?php
/**
 * @file
 * Contains the page and form callbacks for the scribble module.
 */

/**
 * System configuration form for the scribble module.
 */
function scribble_configuration_form($form, &$form_state) {
  $form['scribble_image_file_type'] = array(
    '#type' => 'select',
    '#title' => t('Scribble image file type'),
    '#options' => array('jpeg' => t('JPEG'), 'png' => t('PNG')),
    '#default_value' => variable_get('scribble_image_file_type', 'png'),
    '#description' => t('The image type (file extension) of the merged snapshots. Please note that using PNG with low or compression level 0 results in larger filesize of the merged images, which will lead to long loading times on slideshow pages. The lowest file sizes at best qualities can be achieved by using PNG at compression level 9.'),
  );

  $form['scribble_image_jpeg_quality'] = array(
    '#type' => 'textfield',
    '#title' => t('JPEG quality'),
    '#size' => 3,
    '#default_value' => variable_get('scribble_image_jpeg_quality', 100),
    '#description' => t('Image quality of merged images when using JPEG image type. Use a value between 0 and 100 where 100 is best quality and results in larger filesize. Low quality results in ugly pixels after saving a drawing, recommended value is 100.'),
    '#states' => array(
      'visible' => array(
        ':input[name="scribble_image_file_type"]' => array('value' => 'jpeg'),
      ),
    ),
    '#element_validate' => array('scribble_settings_validate_jpeg_quality'),
  );

  $form['scribble_image_png_quality'] = array(
    '#type' => 'textfield',
    '#title' => t('PNG compression level'),
    '#size' => 3,
    '#default_value' => variable_get('scribble_image_png_quality', 9),
    '#description' => t('Image quality of merged images when using PNG image type. Use a value between 0 and 9 where 0 stands for no compression. No compression leads to large file sizes when using a background color other than white. Compression level 9 is recommended.'),
    '#states' => array(
      'visible' => array(
        ':input[name="scribble_image_file_type"]' => array('value' => 'png'),
      ),
    ),
    '#element_validate' => array('scribble_settings_validate_png_quality'),
  );

  $form['scribble_allow_image_injection'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow adding images'),
    '#description' => t("Do only allow this if your scribbles aren't public accessible. <b>Enabling this option might cause legal issues if images with copyrights get added.</b>"),
    '#default_value' => variable_get('scribble_allow_image_injection', 0),
  );

  $form['scribble_save_prompt'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show confirm dialog before saving.'),
    '#description' => t('Display a JS confirm dialog each time a user attempts to save changes on a scribble.'),
    '#default_value' => variable_get('scribble_save_prompt', TRUE),
  );

  $form['scribble_inject_prompt'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show confirm dialog when dropping an injected image.'),
    '#description' => t('Display a JS confirm dialog each time a user attempts to inject and image into the scribble.'),
    '#default_value' => variable_get('scribble_inject_prompt', TRUE),
  );

  // Include slideshow settings.
  scribble_add_slideshow_settings_elements($form, variable_get('scribble_slideshow_settings'));

  return system_settings_form($form);
}

/**
 * Form element validation handler for the jpeg quality fields.
 */
function scribble_settings_validate_jpeg_quality($element, &$form_state) {
  $value = $element['#value'];
  if ($form_state['values']['scribble_image_file_type'] == 'jpeg') {
    if ((!empty($value) && !is_numeric($value)) || $value > 100 || $value < 0) {
      form_error($element, t('%name must be between 0 and 100.', array('%name' => $element['#title'])));
    }
  }
}

/**
 * Form element validation handler for the png quality field.
 */
function scribble_settings_validate_png_quality($element, &$form_state) {
  $value = $element['#value'];
  if ($form_state['values']['scribble_image_file_type'] == 'png') {
    if ((!empty($value) && !is_numeric($value)) || $value > 9 || $value < 0) {
      form_error($element, t('%name must be between 0 and 9.', array('%name' => $element['#title'])));
    }
  }
}

/**
 * Page callback for the blackboard.
 *
 * @param object $scribble
 *   The loaded scribble entity.
 *
 * @return array
 *   The render array for the given entity.
 */
function scribble_blackboard_page($scribble) {
  return entity_view('scribble', array($scribble));
}

/**
 * Page callback for the snapshot administration.
 *
 * @param object $scribble
 *   The loaded scribble entity.
 *
 * @return array
 *   The render content array of the snapshot administration.
 */
function scribble_image_list_page($scribble) {
  drupal_set_title(scribble_page_title($scribble, SCRIBBLE_TITLE_IMAGE_LIST));
  return entity_view('scribble', array($scribble), 'snapshot_administration');
}

/**
 * AJAX callback for the "Save" button.
 *
 * Creates an image from the data passed by via post request.
 * // @todo make the folder /scribble configurable on the settings form.
 */
function scribble_save_ajax() {
  $scribble_id = $_POST['scribble_id'];
  if (!empty($_POST['imagedata']) && scribble_ajax_action_access($scribble_id)) {
    $data = $_POST['imagedata'];
    list($scribble, $path, $snapshot_path, $file_name, $snapshot_file_name) = scribble_prepare_folders($scribble_id);

    // Need to remove the stuff at the beginning of the image data string.
    $data = substr($data, strpos($data, ",") + 1);

    // Decode the data, create an image resource and save image.
    $data = base64_decode($data);
    $snapshot_resource = imagecreatefromstring($data);

    // If something went wrong return the old filename as background.
    if (empty($snapshot_resource)) {
      watchdog(
        'scribble',
        "Saving new snapshot to scribble @scribble wasn't possible, creating snapshot failed.",
        array('@scribble' => $scribble_id)
      );
      drupal_json_output(array('file_name' => scribble_get_newest($scribble_id)));
      drupal_exit();
    }

    // Make the background transparent.
    imagesavealpha($snapshot_resource, TRUE);
    // Write the snapshot file.
    imagepng($snapshot_resource, $snapshot_path . '/' . $snapshot_file_name);

    // Merge the created snapshot with the latest image. This is necessary
    // because there might be a newer image saved than the loaded background
    // on the scribble. That way, overwriting the latest image is prevented.
    $snapshot_resource = imagecreatefrompng($snapshot_path . '/' . $snapshot_file_name);
    scribble_merge_snapshot_to_newest($scribble, $snapshot_resource, $path, $file_name, $snapshot_file_name);

    // Print new filename as JSON to update the div behind the canvas.
    drupal_json_output(array('file_name' => $file_name));
    drupal_exit();
  }
  // @todo add some output for error display on clientside.
}

/**
 * AJAX callback for the "Save" button.
 *
 * Creates an image from the data passed by via post request.
 */
function scribble_add_ajax() {
  $scribble_id = $_POST['scribble_id'];
  // @todo check more post values
  if (!empty($_POST['img_url']) && scribble_ajax_action_access($scribble_id)) {

    // Create the folders  if they don't exist yet, get scribble and file names.
    list($scribble, $path, $snapshot_path, $file_name, $snapshot_file_name) = scribble_prepare_folders($scribble_id);

    // Create an image resource for the image that gets added.
    $url_parts = explode('/', $_POST['img_url']);
    $added_file_name = array_pop($url_parts);
    $extension = explode('.', $added_file_name);
    $extension = str_replace('jpg', 'jpeg', $extension[1]);
    $function = 'imagecreatefrom' . $extension;
    if (!function_exists($function)) {
      watchdog(
        'scribble',
        "Adding image to scribble @scribble wasn't possible due to unexpected file format of image: @src",
        array('@src' => $_POST['imag_url'], '@scribble' => $scribble_id)
      );
      return;
    }
    $add_img_resource = $function($_POST['img_url']);

    // Create a transparent image as base for the snapshot.
    $snapshot_resource = imagecreatetruecolor($scribble->width, $scribble->height);
    $black = imagecolorallocate($snapshot_resource, 0, 0, 0);
    imagecolortransparent($snapshot_resource, $black);

    // If something went wrong return the old filename as background.
    if (empty($snapshot_resource) || empty($add_img_resource)) {
      watchdog(
        'scribble',
        "Adding image to scribble wasn't possible, creating snapshot from @src to scribble @scribble failed.",
        array('@src' => $_POST['imag_url'], '@scribble' => $scribble_id)
      );
      drupal_json_output(array('file_name' => scribble_get_newest($scribble_id)));
      drupal_exit();
    }

    // Merge the image that gets added to the transparent background.
    imagecopy($snapshot_resource, $add_img_resource, $_POST['dst_x'], $_POST['dst_y'], 0, 0, $_POST['img_width'], $_POST['img_height']);
    imagepng($snapshot_resource, $snapshot_path . '/' . $snapshot_file_name);

    // Merge the snapshot file with the newest or a blank image.
    scribble_merge_snapshot_to_newest($scribble, $snapshot_resource, $path, $file_name, $snapshot_file_name);

    imagedestroy($add_img_resource);
    drupal_json_output(array('file_name' => $file_name));
    drupal_exit();
  }
  // @todo add some output for error display on clientside.
}

/**
 * Confirm form to remove an image from a scribble.
 *
 * @param object $scribble
 *   The loaded scribble entity.
 * @param int $fid
 *   The file id of the image to remove.
 */
function scribble_remove_image_form($form, &$form_state, $scribble, $fid) {
  // Add file id and scribble id to load in submit handler.
  $form['scribble'] = array(
    '#type' => 'value',
    '#value' => $scribble->sid,
  );
  $form['file_id'] = array(
    '#type' => 'value',
    '#value' => $fid,
  );

  $snapshot_file = file_load($fid - 1);

  // Add info text and snapshot that will be removed.
  $info_text = t('The snapshot below will be removed from all following scribble images of the images created after the deleted will be recreated.');
  $backup_text = t('You can backup your images by downloading them !link before you delete the images.', array(
    '!link' => l(t('here'), 'scribble/' . $scribble->sid . '/download'),
  ));
  $form['info_wrapper'] = array(
    '#type' => 'container',
    'info' => array(
      '#markup' => "<span class='scribble-info-text'>$info_text</span><br />",
    ),
    'backup' => array(
      '#markup' => "<span class='scribble-info-text'>$backup_text</span><br />",
    ),
    'snapshot' => array(
      '#markup' => theme('image', array('path' => image_style_url('large', $snapshot_file->uri))),
    ),
  );

  // Load the file to display its name in the question.
  $file = file_load($fid);
  $question = t('Are you sure you want to remove image !image from scribble !scribble?', array(
    '!image' => $file->filename,
    '!scribble' => $scribble->label,
  ));
  // Redirect to image list if action get cancelled by the user.
  $path = 'admin/config/media/scribble/' . $scribble->sid . '/image-list';
  return confirm_form($form, check_plain($question), $path);
}

/**
 * Submit handler for the remove image confirm form.
 *
 * Removes the image and the corresponding transparent snapshot from the
 * scribble. Afterwards all following snapshots will be recreated and  the
 * file_managed table as well as the entity get updated.
 */
function scribble_remove_image_form_submit($form, &$form_state) {
  // Load the scribble.
  $scribble = entity_load_single('scribble', $form_state['values']['scribble']);

  $fid = $form_state['values']['file_id'];

  // Delete the image and the transparent snapshot files and field items.
  foreach (array('scribble_image', 'scribble_image_snapshots') as $field_name) {
    $items = field_get_items('scribble', $scribble, $field_name);
    // Loop field values to find the file.
    foreach ($items as $delta => $item) {
      if ($item['fid'] == $fid) {
        // Save the id of the last clean file for later.
        if ($field_name == 'scribble_image') {
          if ($delta == (count($items) - 1)) {
            // The deleted image is the last image of the scribble, don't set
            // merge base image, there isn't any merging necessary at all.
            $merge_base_resource = FALSE;
          }
          elseif ($delta == 0) {
            // The deleted image is the first image of the scribble, so the base
            // image for recreation is a blank image with background color.
            $merge_base_resource = scribble_create_blank_image_filled($scribble);
            $merge_base_fid = -1;
          }
          else {
            // The deleted image isn't the first nor the last image of the
            // scribble, so the base image is the one before the deleted image.
            $function = $field_name == 'scribble_image' ? 'imagecreatefrom' . variable_get('scribble_image_file_type', 'png') : 'imagecreatefrompng';
            $merge_base_resource = $function($items[$delta - 1]['uri']);
            $merge_base_fid = $items[$delta - 1]['fid'];
          }
        }
        // Load the file to remove.
        $removed_file = file_load($item['fid']);
        // Delete the file from the file_managed table and file system.
        file_delete($removed_file, TRUE);
        // Also remove the record in the field table.
        unset($scribble->{$field_name}[LANGUAGE_NONE][$delta]);
        // The snapshot fid is always the image id - 1 so to remove the snapshot
        // decrease the fid by 1.
        $fid--;
      }
    }
  }

  $scribble->save();

  if ($merge_base_resource !== FALSE) {
    $items = field_get_items('scribble', $scribble, 'scribble_image_snapshots');
    foreach ($items as $item) {
      // Start with merging from the snapshot that follows the snapshot which
      // corresponds the merge base image.
      if ($item['fid'] > ($merge_base_fid - 1)) {
        // Load the snapshot to merge the base with and merge images.
        $merge_snapshot_resource = imagecreatefrompng($item['uri']);
        imagecopy($merge_base_resource, $merge_snapshot_resource, 0, 0, 0, 0, $scribble->width, $scribble->height);

        // Imagepng() isn't able to handle stream wrapper uris, create a path.
        $file_name = 'scribble_' . $scribble->sid . '_' . $item['fid'] . '_' . date('d-m-Y_H-i-s') . '.' . variable_get('scribble_image_file_type', 'png');
        $file_path = variable_get('scribble_files_directory', 'sites/default/files/scribble') . '/' . $scribble->sid . '/' . $file_name;
        // Write the new merged image to the concatenated path.
        scribble_write_image_file($file_path, $merge_base_resource);

        // Get rid of the resources in order to use the variables again.
        imagedestroy($merge_base_resource);
        imagedestroy($merge_snapshot_resource);

        // Update the image file also in the db.
        $file = file_load($item['fid'] + 1);
        // Delete the old file.
        unlink($file->uri);
        // Save the file with the updated image name and uri.
        $file->uri = 'public://scribble' . '/' . $scribble->sid . '/' . $file_name;
        $file->filename = $file_name;
        file_save($file);

        // Use the just created image as merge base for the next image.
        $merge_base_resource = imagecreatefrompng($file->uri);

        // Create image style derivate.
        $derivative_uri = image_style_path('large', $file->uri);
        $style = image_style_load('large');
        image_style_create_derivative($style, $file->uri, $derivative_uri);
      }
    }
  }

  // Redirect to the image list again.
  $form_state['redirect'] = 'admin/config/media/scribble/' . $scribble->sid . '/image-list';
}

/**
 * Page callback for the scribble image slideshow.
 *
 * Will display the images of the a given scribble rendered as slideshow using
 * the formatter of the field_slideshow module if there are any images drawn.
 *
 * @param object $scribble
 *   The scribble entity to display the images of.
 *
 * @return array
 *   Renderable array of the scribble_image field.
 */
function scribble_snapshot_slideshow_page($scribble) {
  drupal_set_title(scribble_page_title($scribble, SCRIBBLE_TITLE_SLIDESHOW));

  // Check if there are any images drawn on the scribble yet.
  $items = field_get_items('scribble', $scribble, 'scribble_image');
  if (!$items) {
    return array('#markup' => t('No images were drawn on this scribble yet.'));
  }

  $display_settings = array(
    'label' => 'hidden',
    'type' => 'slideshow',
    'weight' => '-1',
    'settings' => $scribble->scribble_slideshow_settings,
    'module' => 'field_slideshow',
  );

  return theme('scribble_slideshow', array(
    'scribble' => $scribble,
    'slideshow' => field_view_field('scribble', $scribble, 'scribble_image', $display_settings),
  ));
}

/**
 * Collects scribble images in a zip archive.
 *
 * Sends the archive with the images to the browser for downloading.
 *
 * @param object $scribble
 *   The loaded scribble entity.
 *
 * @return mixed
 *   Markup containing message or nothing.
 */
function scribble_get_images_archive($scribble) {
  // Check if there are any images.
  if ($items = field_get_items('scribble', $scribble, 'scribble_image')) {

    // Create filename, loop images and add them to the archive.
    $file_name = variable_get('file_temporary_path', '/tmp') . '/scribble_' . $scribble->name . '.zip';
    $archive = new ZipArchive();
    $archive->open($file_name, ZIPARCHIVE::OVERWRITE);
    foreach ($items as $item) {
      $archive->addFile(drupal_realpath($item['uri']), $item['filename']);
    }

    // Get file size, filename and send headers.
    $file_size = filesize($file_name);
    $base_name = basename($file_name);
    header('Content-Type: application/zip');
    header('Content-Disposition: attachment; filename="' . $base_name . '"');
    header('Content-Length: ' . $file_size);
    readfile($file_name);

    // Delete file and destroy resource.
    unlink($file_name);
    $archive->close();
    return;
  }
  // In case there are no images yet go back to the image list.
  return array(
    '#markup' => t('No images were drawn on this scribble yet.'),
  );
}

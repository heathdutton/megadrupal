<?php
/**
 * @file
 * Functions from parse offers and category
 */

/**
 * @defgroup ces_import4ces_offers Parse offers from CES
 * @ingroup ces_import4ces
 * @{
 * Functions from parse offers
 */

/**
 * Parse setting.
 */
function ces_import4ces_parse_offers($import_id, $data, $row, &$context, $width_ajax = TRUE) {

  if (isset($context['results']['error'])) {
    return;
  }
  $tx = db_transaction();
  try {
    ob_start();
    $context['results']['import_id'] = $import_id;
    $import = ces_import4ces_import_load($import_id);
    $import->row = $row;

    // Having no category offers, create one for which indicates the lack of it.
    $category = !empty($data['Category']) ? $data['Category'] : 'unclassified';

    $category_id = ces_import4ces_get_category($category, $import);
    $offer = array(
      'type' => 'offer',
      'user' => $data['Advertiser'],
      'title' => $data['Title'],
      'body' => $data['Description'],
      'category' => $category_id,
      'keywords' => $data['Keys'],
      'state' => (($data['Hidden'] == 0) ? 1 : 0),
      'created' => strtotime($data['DateAdded']),
      'modified' => time(),
      'expire' => strtotime($data['DateExpires']),
      'rate' => $data['Rate'],
      'image' => $data['Image'],
    );

    $extra_info = array(
      'UID' => $offer['user'],
      'Subcat' => $data['Keys'],
      'ConRate' => $data['ConRate'],
    );

    // Find uid from user.
    $query = db_query('SELECT uid FROM {users} where name=:name', array(':name' => $offer['user']));
    $offer_user_id = $query->fetchColumn(0);

    if (empty($offer_user_id) || !$offer_user_id) {
      $m = t('The user @user was not found in offer import row @row. It may be a
      user from another exchange not yet imported.', array('@user' => $data['Advertiser'], '@row' => $row));
      $context['results']['error'] = $m;
      throw new Exception($m);
    }

    $offer['user'] = $offer_user_id;

    $o = (object) $offer;
    $o->ces_offer_rate = array(LANGUAGE_NONE => array(array('value' => $offer['rate'])));
    unset($o->rate);
    $offer = ces_offerwant_save($o);

    if (!empty($offer->image)) {

      $file = 'https://www.community-exchange.org/pics/' . $data['Image'];
      $parts = explode(".", $file);
      $extension = end($parts);
      $directory = file_default_scheme() . '://' . variable_get('ces_offerswants_picture_path', 'ces_offerswants_pictures');
      file_prepare_directory($directory, FILE_CREATE_DIRECTORY);
      $name_image = 'picture-' . $offer->id . '-' . REQUEST_TIME . '.' . $extension;
      $destination = file_stream_wrapper_uri_normalize($directory . '/' . $name_image);
      if (ces_import4ces_download_image($file, $destination)) {
        $file = new stdClass();
        $file->uid = $offer_user_id;
        $file->filename = $name_image;
        $file->uri = $destination;
        $file->status = 1;
        $file->filemime = image_type_to_mime_type(exif_imagetype($destination));
        file_save($file);
        file_usage_add($file, 'ces_offerswants', 'ces_offerwant', $offer->id);
        $offer->image = $file->fid;
        // Re-save the entity with the new image file id.
        $offer = ces_offerwant_save($offer);
      }
      else {
        throw new Exception(t('Image @file could not be downloaded.', array('@file' => $file)));
      }
    }

    db_insert('ces_import4ces_objects')->fields(
      array(
        'import_id' => $import_id,
        'object' => 'offers',
        'object_id' => $offer->id,
        'row' => $row,
        'data' => serialize($extra_info),
      ))->execute();
    ces_import4ces_update_row($import_id, $row);
    ob_end_clean();
  }
  catch (Exception $e) {
    ob_end_clean();
    $tx->rollback();
    $context['results']['error'] = check_plain($e->getMessage());
    $_SESSION['ces_import4ces_row_error']['row']  = $row;
    $_SESSION['ces_import4ces_row_error']['m']    = $e->getMessage();
    $_SESSION['ces_import4ces_row_error']['data'] = $data;
    if ($width_ajax) {
      $result = array('status' => FALSE, 'data' => check_plain($e->getMessage()));
      die(json_encode($result));
    }
    else {
      ces_import4ces_batch_fail_row($import_id, array_keys($data), array_values($data), $row, $context);
    }
  }
}
/** @} */

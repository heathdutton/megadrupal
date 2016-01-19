<?php
/**
 * @file
 * Giant Bomb data property handler.
 */

class GiantBombImageProperty extends GiantBombBaseProperty {
  function value() {
    $url = trim($this->value->super_url);

    module_load_include('inc', 'giantbomb', 'includes/giantbomb.files');


    if ($file = giantbomb_file_exists($url)) {
      return $file->fid;
    }

    $filename = 'super';
    $extension = '.image';
    $directory = 'public://giantbomb/covers/' . $this->object['endpoint'] . '/' . $this->object['type'] . '/' . $this->object['id'] . '/';
    if (file_prepare_directory($directory, FILE_CREATE_DIRECTORY + FILE_MODIFY_PERMISSIONS)) {
      $image = file_get_contents($url);
      $file = file_save_data($image, $directory . $filename . $extension);
      file_usage_add($file, 'giantbomb', 'object_' . $this->object['endpoint'] . '_' . $this->object['type'], $this->object['id']);
      if ($image_info = image_get_info($file->uri)) {
        if (!empty($image_info['extension'])) {
          $extension = '.' . $image_info['extension'];
          file_move($file, $directory . $filename . $extension);
        }
        if (is_numeric($file->fid) && $file->fid > 0) {
          $fid = $file->fid;
          $result = db_insert('giantbomb_files')
          ->fields(array(
            'fid' => $fid,
            'endpoint' => $this->object['endpoint'],
            'type' => $this->object['type'],
            'object_id' => $this->object['id'],
            'url' => $url,
          ))
          ->execute();
        }
      }
    } else {
      watchdog('giantbomb', 'Could not create directory @directory.', array(
      '@directory' => $directory,
      ), WATCHDOG_ERROR);
    }

    return $fid;
  }

  function validate() {
    return isset($this->value->super_url) && valid_url($this->value->super_url);
  }
}
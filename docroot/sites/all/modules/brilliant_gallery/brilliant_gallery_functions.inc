<?php

// Return true if the filename has a permitted extension.
function brilliant_gallery_testext($filename) {
  $testending = substr(strtolower($filename), -4, 4);
  if ($testending == ".jpg" or substr(strtolower($filename), -5, 5) == ".jpeg" or $testending == ".gif" or $testending == ".png") {
    return true;
  }
  // Otherwise return false.
  return false;
}

// Load Directory Into Array
function load_dir_to_array($absolpath, $imagewidth, $fullresolutionmaxwidth, $brilliant_gallery_sort, $imagecrop, $maximumnumbertoshow = '') {
  $poct = -1;
  $retval_dimensions = array();
  $actualpath = realpath(FILE_DIRECTORY_PATH) . $absolpath;
  $handle = @opendir($actualpath);
  $imagemaxh = 0;

  // Get a list of files that should be hidden (based on .picasa.ini, if present)
  $hidden_file_names = brilliant_gallery_get_picasa_hidden_imagenames($actualpath);

  // Load the directory into an array first.
  $filearray = array();
  while ($file = @readdir($handle)) {

    /*
    $testending = strtolower(substr($file, -4, 4));
    if (strtolower($testending) <> '.mpg' and strtolower($testending) <> '.swf' and strtolower($testending) <> '.mov' and strtolower($testending) <> '.avi') {
      continue;
    }
    */
    if (!brilliant_gallery_testext($file)) continue;

    // Hide images listed as hidden if they are present in .picasa.ini
    if (in_array($file, $hidden_file_names)) continue;

    $filearray[] = $file;

  }
  @closedir($handle);

  if (empty($filearray)) {
    watchdog('Brilliant Gal', 'No displayable images in ' . $absolpath . '!');
  }
  else {
    foreach ($filearray as $file) {
      # Index of a real image or something we are going to display.
      $poct += 1;
      $retval_dimensions[$poct]['file'] = $file;
      $retval_dimensions[$poct]['imgcrop'] = $imagecrop;

      # Is image horizontally or vertically oriented?
      $temp = getimagesize($actualpath . '/' . $file);
      if ($temp === false) {
        continue;
      }
      if ((($temp[0] - $temp[1]) >= 0) OR $maximumnumbertoshow == 1) {
        // This is a horizontal image.
        // Treat single images just as horizontal images (no need to fit their height to the height of horizontals)! The specified width of an image is authoritative for both horizontal images and also vertical if they show alone.
        if ($imagecrop == 'yes') {
          $retval_dimensions[$poct]['imgw'] = $imagewidth;
          $retval_dimensions[$poct]['imgh'] = $imagewidth;
        }
        else {
          $retval_dimensions[$poct]['imgw'] = $imagewidth;
          $retval_dimensions[$poct]['imgh'] = round(($temp[1] / $temp[0]) * $imagewidth);
          #watchdog('Brilliant Gal','1imgw: '.$imagewidth.'/'.$imagecrop);
        }
        $retval_dimensions[$poct]['imgwbig'] = $fullresolutionmaxwidth;
        $retval_dimensions[$poct]['imghbig'] = round(($temp[1] / $temp[0]) * $fullresolutionmaxwidth);
      }
      else {
        // This is a vertical image
        if ($imagecrop == 'yes') {
          $retval_dimensions[$poct]['imgw'] = round(($temp[0] / $temp[1]) * $imagewidth);
          $retval_dimensions[$poct]['imgh'] = $imagewidth;
        }
        else {
          $retval_dimensions[$poct]['imgw'] = round(($temp[0] / $temp[1]) * ($temp[0] / $temp[1]) * $imagewidth);
          $retval_dimensions[$poct]['imgh'] = round(($temp[0] / $temp[1]) * $imagewidth);
        }
        $retval_dimensions[$poct]['imgwbig'] = round(($temp[0] / $temp[1]) * ($temp[0] / $temp[1]) * $fullresolutionmaxwidth);
        $retval_dimensions[$poct]['imghbig'] = round(($temp[0] / $temp[1]) * $fullresolutionmaxwidth);
      }
      // In $imagemaxh collect the maximum vertical size of the gallery.
      if ($imagemaxh < $retval_dimensions[$poct]['imgh']) {
        $imagemaxh = $retval_dimensions[$poct]['imgh'];
      }
    }
  }

  if ($brilliant_gallery_sort == '1') {
    @sort($retval_dimensions);
  }
  else {
    shuffle($retval_dimensions);
  }
  return array($retval_dimensions, $imagemaxh);
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function brilliant_gallery_brokenimage($msg, $width = 150, $height = 30) {
  header('Content-type: image/jpeg');
  $im = imagecreatetruecolor($width, $height);
  $bgc = imagecolorallocate($im, 0, 0, 0);
  $tc = imagecolorallocate($im, 255, 255, 255);
  imagefilledrectangle($im, 0, 0, 150, 30, $bgc);
  imagestring($im, 1, 5, 5, $msg, $tc);
  imagejpeg($im);
  exit();
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function brilliant_gallery_image_properties_set($bgimgproperties_array) {
  // Binds image property array with its hash that is present in the cached file name and in the URL.

  $bgimgproperties_hash = md5(serialize($bgimgproperties_array));

  // Let's keep the array in cache. Check if it is in cache. If not, write it.
  // TODO Please convert this statement to the D7 database API syntax.
  $countie = db_query("SELECT COUNT(*) FROM {brilliant_gallery_image_arrays} WHERE hash = '" . $bgimgproperties_hash . "'")->fetchField();
  if ($countie == 0) {
    // Run an insert
    //echo 1;
    $fields = new stdClass();
    $fields->hash = $bgimgproperties_hash;
    $fields->array = serialize($bgimgproperties_array);
    //$fields->datetime = date('Y-m-d H:i:s', REQUEST_TIME);
    $fields->datetime = REQUEST_TIME;
    drupal_write_record("brilliant_gallery_image_arrays", $fields);
  }
  else {
    // Run an update
    $change = array(
      //'datetime' => date('Y-m-d H:i:s', REQUEST_TIME),
      'datetime' => REQUEST_TIME,
      'hash' => $bgimgproperties_hash,
    );
    drupal_write_record('brilliant_gallery_image_arrays', $change, 'hash');
  }

  return $bgimgproperties_hash;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function brilliant_gallery_image_properties_get($bgimgproperties_hash) {

  // TODO Please convert this statement to the D7 database API syntax.
  $bgimgproperties_array = db_query("SELECT array FROM {brilliant_gallery_image_arrays} WHERE hash = '" . $bgimgproperties_hash . "' LIMIT 1")->fetchField();
  $bgimgproperties_array = unserialize($bgimgproperties_array);

  return $bgimgproperties_array;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function brilliant_gallery_get_extension($imagename) {
  $imagename = explode(".", $imagename);
  $extension = strtolower(array_pop($imagename));
  return $extension;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function brilliant_gallery_get_days_in_seconds($days) {
  return $days * 24 * 3600;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function brilliant_gallery_deliver_image() {

  $queryexplode = explode('/', $_GET['q']);
  //$bgimgproperties_hash_position = sizeof($queryexplode)+1;
  $bgimgproperties_hash = array_pop($queryexplode); // Get the last item in the query.
  $bgimgproperties_hash = substr($bgimgproperties_hash, strlen('bg_cached_resized_')); // Remove prefix.
  //return (base64_decode($bgimgproperties_hash));
  //include 'image.inc';
  // The file name contains a hash token that corresponds to the image parameter array; this relation is in cache.
  $bgimgproperties_hash_array = explode('.', $bgimgproperties_hash);
  $bgimgproperties_hash_array = $bgimgproperties_hash_array[0];
  $bgimgproperties_array = brilliant_gallery_image_properties_get($bgimgproperties_hash_array);
  //print_r($bgimgproperties_array); flush(); exit();
  if (!is_array($bgimgproperties_array)) {
    watchdog('Brilliant Gal', 'Wrong image: ' . $bgimgproperties_hash);
    return;
  }
  foreach ($bgimgproperties_array as $key => $val) {
    //$showthis .= $val.' -- ';
    $_GET[$key] = $val;
  }


  /* Check for bad URL inputs */
  $urlpath = $_GET['imgp'];
  //brilliant_gallery_brokenimage($urlpath.' :0: '.time(),1900,500); exit();
  if (sizeof($_GET) > 5 ||
      strpos($urlpath, "://") !== false ||
      strpos($urlpath, "..") !== false ||
      preg_match('/\D/', ($_GET['imgw'] . $_GET['imgh'])) > 0 ||
      ($_GET['imgw'] + $_GET['imgh']) < 10 ||
      ($_GET['imgw'] + $_GET['imgh']) > 20000
  ) {
    header("HTTP/1.0 404 Not Found");
    exit();
  }

  $imagepath = realpath(FILE_DIRECTORY_PATH . $urlpath);

  // Crucial - to suppress Devel (if installed and enabled) output appearing in the generated XML!
  $GLOBALS['devel_shutdown'] = FALSE;

  $my_data = resizeimage_wrapper_dbcache(FALSE, $imagepath, $bgimgproperties_hash);

  // http://be.php.net/getimagesize
  $imgsize = @getimagesize($imagepath);
  $head = "Content-type: {$imgsize['mime']}";
  header($head);
  echo $my_data;
  // IMPORTANT to exit() - otherwise some process after BG adds strings and breaks the image!
  exit();

  return;
}

function resizeimage_wrapper_dbcache($reset = FALSE, $imagepath, $bgimgproperties_hash) {
  $bgcachexpire = brilliant_gallery_get_days_in_seconds(variable_get('brilliant_gallery_cache_duration', 90)); // Cache expiration time in days.
  static $my_data;

  $pcache = variable_get('brilliant_gallery_pcache', BRILLIANT_GALLERY_DEFAULT_CACHE_DIR);
  //brilliant_gallery_check_or_create_dir($pcache);
  $foqen = FILE_DIRECTORY_PATH . '/' . $pcache . '/bg_cached_resized_' . $bgimgproperties_hash;
  $lastchanged = @filemtime($foqen); // Last file modification time, or FALSE on error.

  if ($lastchanged === FALSE OR (REQUEST_TIME - $lastchanged > $bgcachexpire)) {
    // If the image is expired, we need to actively delete it, for the case that it was removed / hidden by the owner.
    @unlink($foqen);
    $my_data = resizeimage($_GET['imgp'], $_GET['imgw'], $_GET['imgh'], @$_GET['imgcrop'], $imagepath);
    $image = $my_data;
    // It happens that the file size is 0 (image not fetched). In such case, don't write it.
    if (strlen($image) > 0) {
      $fp = fopen($foqen, 'w');
      fwrite($fp, $image);
      fclose($fp);
    }
  }


  return $my_data;
}

function resizeimage($imgp, $imgw, $imgh, $imgcrop, $imagepath) {

  $imagepathexploded = explode('.', strtolower($imagepath));
  $suffix = end($imagepathexploded);
  if ($suffix == "gif") {
    $img = @imagecreatefromgif($imagepath);
    if (!$img) {
      brilliant_gallery_brokenimage("Error loading GIF", $imgw, $imgh);
    }
  }
  else if ($suffix == "jpg" or $suffix == "jpeg") {
    $img = @imagecreatefromjpeg($imagepath);
    if (!$img) {
      brilliant_gallery_brokenimage("Error loading JPG", $imgw, $imgh);
    }
  }
  else if ($suffix == "png") {
    $img = @imagecreatefrompng($imagepath);
    if (!$img) {
      brilliant_gallery_brokenimage("Error loading PNG", $imgw, $imgh);
    }
  }
  # Resize the image
  $src_h = ImageSY($img);
  $src_w = ImageSX($img);
  $dst_img = 0;
  if ($imgcrop == 'yes') {
    if ($src_h > $src_w) {
      // portrait
      $dst_img = imagecreatetruecolor($imgh, $imgh);
      imagecopyresampled($dst_img, $img, 0, 0, 0, ($src_h - $src_w) / 2, $imgh, $imgh, $src_w, $src_w);
    }
    else {
      // landscape
      $dst_img = imagecreatetruecolor($imgw, $imgw);
      imagecopyresampled($dst_img, $img, 0, 0, ($src_w - $src_h) / 2, 0, $imgw, $imgw, $src_h, $src_h);
    }
  }
  else {
    $dst_img = imagecreatetruecolor($imgw, $imgh);
    imagecopyresampled($dst_img, $img, 0, 0, 0, 0, $imgw, $imgh, $src_w, $src_h);
  }
  $img = $dst_img;
  ob_start();
  if ($suffix == "gif") {
    Imagegif($img);
  }
  else if ($suffix == "jpg" or $suffix == "jpeg") {
    Imagejpeg($img, NULL, 90);
  }
  else if ($suffix == "png") {
    Imagepng($img, NULL, 9);
  }
  $result = ob_get_clean();
  #ImageDestroy($img);
  //$result = serialize(array($head, base64_encode($result)));
  $result = $result;
  return $result;
}

function replace_brilliant_gallery_tags($str) {
  # Old format - still supported
  # [bg|path/to/gallery_folder|colcountoverride|widthoverride|sortorrandomoverride|maximumnumbertoshow|colouroverride|beginfromoverride|caption-yes-no-text]
  # New format - allows multiline tags (strips out HTML), works with parameter = value attribution (position independent!)
  #
  #
  #

  $matchlink = '';
  $orig_match = '';
  $matches = '';
  preg_match_all("/(\[)bg(\|)[^\]]*(\])/s", $str, $matches);
  foreach ($matches[0] as $match) {
    $omatch = $match;
    $orig_match[] = $omatch;
    $match = substr($match, 1, (strlen($match) - 2));

    // Remove HTML tags
    $match = strip_tags($match);

    // Create an array of parameter attributions
    $match = explode("|", $match);
    #watchdog('Brilliant Gal', 'Got the new arrayx: '.vacilando_echo_array($match));
    $allowed_params = brilliant_gallery_get_allowed_params();

    // Remove enclosing spaces and get rid of empty parameter attributions.
    $newmatch = array(); // Collect for the legacy style $match array.
    $newgenmatch = array();
    $isnewgettag = true;
    foreach ($match as $val) {
      $tmp = trim($val);
      if ($tmp <> '') {
        $tmp2 = explode(' = ', $tmp);
        #watchdog('Brilliant Gal', $tmp.' // '.sizeof($tmp2).' ('.$tmp2[0].')');
        if (sizeof($tmp2) == 2) {
          // It's possibly a new generation tag
          $tmp2[0] = strtolower(trim($tmp2[0]));
          $tmp2[1] = trim($tmp2[1]);
          // Check if it uses a valid parameter name.
          // The value MAY be none here, to allow re-setting some parameters.
          if (in_array($tmp2[0], $allowed_params)) {
            $newgenmatch[$tmp2[0]] = $tmp2[1];
          }
          else {
            $msg = 'Parameter ' . $tmp2[0] . ' is invalid!';
            #drupal_set_message($msg);
            watchdog('Brilliant Gal', $msg);
          }
        }
        else {
          // Takes care of compatibility with old-style BG tags (one line, no attribution).
          // If one or more of the parameters does not use attribution, the whole tag is treated as an old generation one. Except for 'bg', which is not an attribution parameter.
          if ($tmp <> 'bg') {
            $isnewgettag = false;
          }
          $newmatch[] = $tmp;
        }
      }
    }

    if (!$isnewgettag) {
      // Plain old tag
      $match = $newmatch;
    }
    else {
      // OK, we've got the new generation params in $newgenmatch
      // The order of params to feed into render_brilliant_gallery() is the same as with the old-style tag.
      $match = array();
      foreach ($newgenmatch as $key => $val) {
        $match[array_search($key, $allowed_params)] = $val; // E.g. 'location = myalbum' will become $match[1] = 'myalbum'
      }
    }

    $pathurl_btsync_check = brilliant_gallery_pathurl_btsync_check($match[1]);
    // If this is a BT Sync folder (secret), change $match[1]
    if ($pathurl_btsync_check) $match[1] = $pathurl_btsync_check;

    // If we are using thumbshowbyname and there is just 1 image, force thumbmaxshow to 1 (prevents BG from adjusting the height as it does for a set of images).
    if (@$match[13] <> '' AND strpos(@$match[13], ',') === FALSE) {
      $match[5] = 1;
    }

    // Cache the result if it comes from a non-random tag
    $bgcachexpire = brilliant_gallery_get_days_in_seconds(variable_get('brilliant_gallery_cache_duration', 90)); // Cache expiration time in days.
    $mbgtag = md5($omatch);
    #########x TEMPORARILY SET TO FALSE!!
    if ($cache = cache_get('bg_gallery_table_' . $mbgtag) and !empty($cache->data)) {
      $galhere = $cache->data;
    }
    else {
      # render_brilliant_gallery takes parameters in a fixed order:
      # 1 path/to/gallery_folder
      # 2 colcountoverride
      # 3 widthoverride
      # 4 sortorrandomoverride
      # 5 maximumnumbertoshow
      # 6 colouroverride
      # 7 beginfromoverride
      # 8 caption-yes-no-text
      #watchdog('Brilliant Gal', 'before: '.vacilando_echo_array($match));
      $galhere = render_brilliant_gallery($match);
      cache_set('bg_gallery_table_' . $mbgtag, $galhere, 'cache', brilliant_gallery_get_time_this_many_seconds_from_now($bgcachexpire));
    }

    $matchlink[] = $galhere;
  }
  $str = str_replace($orig_match, $matchlink, $str);
  return $str;
}

function brilliant_gallery_pathurl_btsync_check($pathurl){
  // Only continue if it looks like a BT Sync secret (must be 33 chars); see http://forum.bittorrent.com/topic/29304-rules-for-valid-secret/
  preg_match("/[A-Z2-7]{33}/", $pathurl, $output_array);
  if (!array_key_exists(0, $output_array)) {
    return FALSE;
  }
  // Only continue if the main gallery folder has been configured.
  if (variable_get('brilliant_gallery_folder', '') == '') break;
  $btsyncdir = 'public://'.variable_get('brilliant_gallery_folder', '').'/btsync';
  $dirtest = file_prepare_directory($btsyncdir, FILE_CREATE_DIRECTORY);
  // Only continue if the special btsync directory exists and is writable
  if (!$dirtest) {
    watchdog('Brilliant Gal', 'Special folder "btsync" cannot be created or is not writable in ' . $btsyncdir = 'public://'.variable_get('brilliant_gallery_folder', ''));
    return FALSE;
  }
  // OK now let's check or add the particular sync folder
  $btsyncgallerydir = $btsyncdir.'/'.$pathurl;
  $dirtest = file_prepare_directory($btsyncgallerydir, FILE_CREATE_DIRECTORY);
  // Only continue if the special btsync directory exists and is writable
  if (!$dirtest) {
    watchdog('Brilliant Gal', 'Sync folder "'.$pathurl.'" cannot be created or is not writable in ' . $btsyncdir);
    return FALSE;
  }
  if (function_exists('btsync_method_callback')){
    btsync_method_callback('add_folder', array(
      'dir' => drupal_realpath($btsyncgallerydir), // Did not work without drupal_realpath here.
      'secret' => $pathurl,
    ));
  }
  else {
    watchdog('Brilliant Gal', 'BT Sync gallery formatter selected but the BitTorrent Sync API module ("btsync") is not enabled.');
    return FALSE;
  }
  $pathurl = 'btsync/'.$pathurl;
  return $pathurl;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function brilliant_gallery_get_time_this_many_seconds_from_now($period) {
  $period = REQUEST_TIME + $period;
  return $period;
}

/**
 * Get a list of files that should be hidden (based on .picasa.ini, if present)
 */
function brilliant_gallery_get_picasa_hidden_imagenames($absolute_album_path){

  // Skip processing if there is no .picasa.ini inside
  if (file_exists($absolute_album_path."/.picasa.ini")){

    // Skip if the .picasa.ini file lists no hidden images
    $picasaini = file_get_contents($absolute_album_path."/.picasa.ini");
    if (strpos($picasaini, "hidden=yes") !== FALSE){
      $picasaini_lines = file($absolute_album_path."/.picasa.ini");
      $picasaini_lines = array_map('trim', $picasaini_lines);

      $dir_files = scandir($absolute_album_path);
      // Skip ., .., and .picasa.ini
      $dir_files = array_diff($dir_files, array('..', '.', '.picasa.ini'));

      $filenames = array();
      foreach ($dir_files as $key => $filename) {

            /* Check if the filename is hidden in .picasa.ini
             * Hidden entry example:
             * [02_011_08a.jpg]
             * rotate=rotate(2)
             * backuphash=53332
             * hidden=yes
             */
            $searched_filename = '['.$filename.']';
            $is_in_picasaini = array_search($searched_filename, $picasaini_lines);
            // If the file name is not in .picasa.ini, skip
            if ($is_in_picasaini === FALSE) continue;
            // Ok so it's in .picasa.ini, but is it currently hidden?
            $ishidden = FALSE;
            foreach($picasaini_lines as $key=>$val){
              // Iterate to the line AFTER the matching file name
              if ($key <= $is_in_picasaini) continue;
              // If the line is "hidden=yes" then the file name is hidden
              if ($val == 'hidden=yes') {
                $ishidden = TRUE;
                break;
              }
              // If we reach a line starting with "[" then we assume that the searched file name is NOT hidden
              if (substr($val, 0, 1) == '[') break;
            }
            if (!$ishidden) continue;

            $filenames[] = $filename;

      }

    }

  }

  return $filenames;

}

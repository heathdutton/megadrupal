<?php

/* TODO FormAPI image buttons are now supported.
  FormAPI now offers the 'image_button' element type, allowing developers to
  use icons or other custom images in place of traditional HTML submit buttons.

  $form['my_image_button'] = array(
  '#type'         => 'image_button',
  '#title'        => t('My button'),
  '#return_value' => 'my_data',
  '#src'          => 'my/image/path.jpg',
  ); */

/* TODO Remove $row argument from db_result() method
  The $row argument of db_result() was removed from the database abstraction
  layer in 6.x core, as it was a database dependent option. Developers need to
  use other handling to replace the needs of this method. */

/* TODO New user_mail_tokens() method may be useful.
  user.module now provides a user_mail_tokens() function to return an array
  of the tokens available for the email notification messages it sends when
  accounts are created, activated, blocked, etc. Contributed modules that
  wish to make use of the same tokens for their own needs are encouraged
  to use this function. */

/* TODO
  There is a new hook_watchdog in core. This means that contributed modules
  can implement hook_watchdog to log Drupal events to custom destinations.
  Two core modules are included, dblog.module (formerly known as watchdog.module),
  and syslog.module. Other modules in contrib include an emaillog.module,
  included in the logging_alerts module. See syslog or emaillog for an
  example on how to implement hook_watchdog.
  function example_watchdog($log = array()) {
  if ($log['severity'] == WATCHDOG_ALERT) {
  mysms_send($log['user']->uid,
  $log['type'],
  $log['message'],
  $log['variables'],
  $log['severity'],
  $log['referer'],
  $log['ip'],
  format_date($log['timestamp']));
  }
  } */

/* TODO Implement the hook_theme registry. Combine all theme registry entries
  into one hook_theme function in each corresponding module file.
  function views_theme() {
  return array(
  );
  } */

/**
 * Implements hook_views_tables().
 */
function brilliant_gallery_views_tables() {
  $tables['brilliant_gallery'] = array(
    'name' => 'brilliant_gallery',
    /*
      'join' => array(
      'left' => array(
      'table' => 'node',
      'field' => 'nid',
      ),
      'right' => array(
      'field' => 'nid',
      ),
      ),
     */
    'fields' => array(
      'node' => array(
        'name' => t('Brilliant Gallery: A random image'),
        'handler' => array(
          'brilliant_gallery_views_handler_image_img' => t('Random image'),
        #'brilliant_gallery_views_handler_image_img_link' => t('bgImage with link'),
        ),
        /*
          'option' => array(
          '#type' => 'select',
          '#options' => 'brilliant_gallery_views_handler_filter_image_size',
          ),
         */
        'notafield' => true,
        'sortable' => false,
        'help' => t('Display one random image from the respective gallery.'),
      ),
    ),
  );
  return $tables;
}

/**
 * Views handler for displaying the image.
 */
function brilliant_gallery_views_handler_image_img($fieldinfo, $fielddata, $value, $data) {

  $tmp = $data->nid;
  $query = "SELECT SQL_CACHE `field_gallery_value` FROM `content_type_page` WHERE `nid` = '" . $tmp . "' ORDER BY `vid` DESC LIMIT 1";
  // TODO Please convert this statement to the D7 database API syntax.
  $string = db_query($query)->fetchField();
  $string = str_replace(array('[bg|', ']'), '', $string);
  if (strpos($string, '|') !== false) {
    $string = substr($string, 0, (strpos($string, '|')));
  }
  if ($string == '') {
    return;
  }
  # Now get a list of images and choose one of them.
  $absolpath = realpath(FILE_DIRECTORY_PATH . '/' . variable_get('brilliant_gallery_folder', '') . '/' . $string);
  # Load Directory Into Array
  $poct = -1;
  $retval = array();
  $handle = @opendir($absolpath);
  while ($file = readdir($handle)) {
    $poct += 1;
    $testending = strtolower(substr($file, -4, 4));
    if (strtolower($testending) <> '.jpg' and strtolower($testending) <> 'jpeg' and strtolower($testending) <> '.gif' and strtolower($testending) <> '.png') {
      continue;
    }
    $retval[$poct] = $file;
  }
  closedir($handle);
  #print_r( $retval );
  $randimg = mt_rand(0, count($retval));
  $result = $absolpath . '/' . $retval[$randimg];
  $temp = getimagesize($result);
  #$imagewidth = variable_get('brilliant_gallery_maximagewidth', 150);
  # Hard-coded height for this purpose.
  $imgh = 100;
  $imgw = round(($temp[0] / $temp[1]) * $imgh);
  # Get this module's path:
  $modulepath = url(drupal_get_path('module', 'brilliant_gallery'), array('absolute' => TRUE));
  # url() ads i18n codes to the URL ... we need to remove them here...
  if (BG_LANGCODE <> '') {
    $modulepath = str_replace('/' . BG_LANGCODE . '/', '/', $modulepath);
  }
  # Non-clean URLs need removing ?q=
  $modulepath = str_replace("?q=", "", $modulepath);
  $result = '<a href="' . file_create_url($modulepath . '/image.php?imgp=' . base64_encode($absolpath . '/' . $retval[$randimg]) . '&imgw=' . $imgw * 6 . '&imgh=' . $imgh * 6) . '"';
  //$setname = mt_rand(1, 9999999);
  $setname = md5($absolpath);
  $overbrowser = variable_get('brilliant_gallery_overbrowser', 'colorbox');
  switch ($overbrowser) {

    case 'colorbox':
      $result .= ' class="colorbox" rel="bg-' . $setname . '"';
      break;

    case 'lightbox':
      $result .= ' rel="lightbox[' . $setname . ']"';
      break;

    default:
      break;

  }

  if ($showcaption <> '') {
    $result .= ' title="' . $caption . '"';
  }
  $result .= '>';
  # width="' . $imgw . '"
  $result .= '<img style="display: block;border:0;align:right" src="' . file_create_url($modulepath . '/image.php?imgp=' . base64_encode($absolpath . '/' . $retval[$randimg]) . '&imgw=' . $imgw . '&imgh=' . $imgh) . '" />';
  $result .= '</a>';
  return $result;
}

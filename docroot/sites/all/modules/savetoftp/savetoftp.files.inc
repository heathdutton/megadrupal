<?php

/**
 * @file
 * Handles the file processes and html parsing for exporting a Drupal node.
 * Returns a FTP connection, export path, and exportable item to the main module
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function savetoftp_create_directories($everything) {
  $folders = array();
  $export_folder = '/savetoftp';
  $export_path = $_SERVER['DOCUMENT_ROOT'] . base_path() . file_directory_path() . $export_folder;
  // First delete existing directory to avoid build-up
  if(is_dir($export_path)) {
    file_unmanaged_delete_recursive($export_path);
  }
  mkdir($export_path);
}

/**
 * Utility function to process the html of the node
 * @param $evertyhing boolean Set to true if want to pass all site files
 * @param $links boolean Set to true if want to use absolute links instead
 * @param $anon boolean Set to true if want html as if viewed anonymously
 * @return $file_place string Location of the locally saved html file
 */
function savetoftp_data_process($everything, $links, $anon) {
  $node = menu_get_object();
  $html_name_array = explode("/", drupal_get_path_alias('node/' . $node->nid));
  $html_name = end($html_name_array);
  $html_name = check_plain(str_replace(" ", "", $html_name)) . '.html';
  // Turn on CSS / JS aggregation to cut down hugely on # of file transfers
  $css_aggregation = variable_get('preprocess_css');
  $js_aggregation = variable_get('preprocess_js');
  variable_set('preprocess_css', 1);
  variable_set('preprocess_js', 1);
  $html_headers = array();
  if (!$anon) {
    $cookie_headers = $_COOKIE;
    foreach ($cookie_headers as $key => $value) {
      $html_headers['Cookie'] = $html_headers['Cookie'] . $key . '=' . $value . '; ';
    }
  }
  $stage_site = url('node/' . $node->nid, array('absolute' => TRUE));
  if ($anon) {
    global $user;
    $current_user = $user;
    drupal_save_session(FALSE);
    $user = user_load(0);
    $html_piece = drupal_http_request($stage_site, array('headers' => $html_headers));
    if (!$html_piece->data) {
      drupal_set_message(t('There was a problem getting the Drupal page html'), 'error');
    }
    $html_original = $html_piece->data;
    $user = $current_user;
    drupal_save_session(TRUE);
  }
  else {
    $html_piece = drupal_http_request($stage_site, array('headers' => $html_headers));
    if (!$html_piece->data) {
      drupal_set_message(t('There was a problem getting the Drupal page html'), 'error');
    }
    $html_original = $html_piece->data;
  }
  if($everything) {
   // Initialize arrays to list all of the files we need to copy
   $files = array();
    // Use QueryPath to find the files we need
    $qp_links = htmlqp($html_original, 'link');
    $qp_javascripts = htmlqp($html_original, 'script');
    $qp_images = htmlqp($html_original, 'img');
    foreach($qp_links as $qp_link) {
      if($qp_link->attr('rel') == 'stylesheet') {
        $files[] = parse_url($qp_link->attr('href'), PHP_URL_PATH);
      }
    }
    foreach($qp_javascripts as $qp_js) {
      if($qp_js->attr('src') != NULL) {
        $files[] = parse_url($qp_js->attr('src'), PHP_URL_PATH);
      }
    }
    foreach($qp_images as $qp_img) {
      if($qp_img->attr('src') != NULL) {
        $files[] = parse_url($qp_img->attr('src'), PHP_URL_PATH);
      }
    }
    $html_fixed = @qp($html_original, array('ignore_parser_warnings' => TRUE));
    // Make all css, js, and image paths relative in the HTML 
    htmlqp($html_fixed, 'link')->each('savetoftp_qp_base_href');
    htmlqp($html_fixed, 'script')->each('savetoftp_qp_base_src');
    htmlqp($html_fixed, 'img')->each('savetoftp_qp_base_src');
    $html_fixed = $html_fixed->html();
    // Call function to copy all files over
    $result = savetoftp_copy_files($files);
  }
  elseif ($links) {
    $name = 'http://' . $_SERVER['SERVER_NAME'];
    $pattern_css = '/stylesheet=\"all\" href=\"/';
    $new_pattern_css = 'stylesheet="all" href="' . $name;
    $html_fixed = preg_replace($pattern_css, $new_pattern_css, $html_original);
    $pattern_js_and_images = '/src=\"/';
    $new_pattern_js_and_images = 'src="' . $name;
    $html_fixed = preg_replace($pattern_js_and_images, $new_pattern_js_and_images, $html_original);
  }
  $file_place = file_directory_path() . '/savetoftp/' . $html_name;
  $file = fopen($file_place, "w");
  if (!fwrite($file, $html_fixed)) {
    drupal_set_message(t('Unable to save the html file locally at @file_place. Check your files settings.', array('@file_place' => $file_place)));
  }
  fclose($file);
  // Set CSS / JS aggregation back to their original states
  variable_set('preprocess_css', $css_aggregation ? 1 : 0);
  variable_set('preprocess_js', $js_aggregation ? 1 : 0);
  return $file_place; // return location of where we save the file
}

/**
 * Utility function for setting href to basename only
 * @param $index int The index of the QP object
 * @param $item object A QueryPath object
 * @return boolean true continues the flow
 */ 

function savetoftp_qp_base_href($index, $item) {
  $src = $item->getAttribute('href');
  $item->setAttribute('href', basename($src));
  return TRUE; 
}

/**
 * Utility function for setting src to basename only
 * @param $index int The index of the QP object
 * @param $item object A QueryPath object
 * @return boolean true continues the flow
 */ 

function savetoftp_qp_base_src($index, $item) {
  $src = $item->getAttribute('src');
  $item->setAttribute('src', basename($src));
  return TRUE; 
}

/**
 * Copy files to the public files directory in anticipation of sending to FTP
 * @param $files array A listing of file paths relative to DRUPAL_ROOT
 * @return $success boolean True on successful copy, false on error
 */

function savetoftp_copy_files($files) {
  foreach($files as $file) {
    $path = file_unmanaged_copy(DRUPAL_ROOT . $file, DRUPAL_ROOT . '/' . file_directory_path() . '/savetoftp/' . basename($file), FILE_EXISTS_REPLACE);
    if(!$path) {
      drupal_set_message(t('Failed to copy @file to the default files directory @dir', array('@file' => $file, '@dir' => file_directory_path() . '/savetoftp')));
    } 
  }
}

/**
 * Utility function to connect to the FTP service
 * @param $form_state array The values from our form entries
 * @return $fp mixed On error returns boolean false otherwise a file pointer
 */
function savetoftp_ftp_connect($form_state) {
  $ftp_server = check_plain($form_state['values']['ftp_server']);
  $ftp_uname = check_plain($form_state['values']['ftp_user']);
  $ftp_pass = check_plain($form_state['values']['ftp_pass']);
  $ftp_directory = check_plain($form_state['values']['ftp_direct']);
  $ftp_directory_array = explode('/', $ftp_directory);
  if (!$fp = ftp_connect($ftp_server, 21, 30)) {
    drupal_set_message('Connection failed. Check to make sure the FTP server name is correct', 'error');
    return;
  }
  if (!ftp_login($fp, $ftp_uname, $ftp_pass)) {
    drupal_set_message('Login failed. Check your username and password and try again.', 'error');
    return FALSE;
  }
  if (!ftp_pasv($fp, TRUE)) {
    drupal_set_message(t('Unable to set mode to passive'), 'warning');
  }
  $origin = ftp_pwd($fp);
  foreach ($ftp_directory_array as $fd) {
    if (!empty($fd)) {
      if (!@ftp_chdir($fp, $fd)) {
        if (!@ftp_mkdir($fp, $fd)) {
          drupal_set_message(t('Unable to make directory: @fd. Try changing the directory path to something else.', array('@fd' => $fd)), 'error');
        }
        if (!@ftp_chdir($fp, $fd)) {
          drupal_set_message(t('Unable to change directory to: @fd. Are you sure this directory exisists on the server?', array('@fd' => $fd)), 'error');
        }
      }
    }
  }
  return $fp;
}

/**
 * Utility function to handle file transfers
 * @param $fp object an ftp connection object
 * @param $remote_file string the file name of the file
 * @param $file_place string the full file path to the file
 * @param $everything boolean sends the css,js,and image files when TRUE
 */
function savetoftp_ftp_files($fp, $remote_file, $file_place, $everything) {
  $path = file_directory_path() . '/' . 'savetoftp/';
  if ($everything) {
    savetoftp_ftp_recurse($fp, $path);
  }
  else {
    if (!ftp_put($fp, $remote_file, $file_place, FTP_BINARY)) {
      drupal_set_message(t('Was unable to put the @remote_file file on the server.', array('@remote_file' => $remote_file)), 'warning');
    }
  }
  return;
}

/**
 * Utility function to recurse through a directory
 * And send the files in it to the ftp server
 * @param $fp object The ftp server connection
 * @param $path string The path of the directory to look in for files
 */
function savetoftp_ftp_recurse($fp, $path) {
  drupal_set_time_limit(0);
  $dir = new DirectoryIterator($path);
  foreach($dir as $file) {
    if (!$file->isDot() && !$file->isDir()) {
      $file_to_open = $file->getPathname();
      $remote_file = $file->getFilename();
      if (!ftp_put($fp, $remote_file, $file_to_open, FTP_BINARY)) {
        drupal_set_message(t('Was unable to put the @remote_file on the server.', array('@remote_file' => $remote_file)), 'warning');
      }
      else {
        $files_transferred[] = $remote_file;
      }
    }
  }
  if(!empty($files_transferred)) {
    drupal_set_message(t('Successfully transferred the following files @files to the server.', array('@files' => implode(',', $files_transferred))));
  }
  return;
}

<?php
/**
 * @file
 * isbn2node internal functions
 */


/*
 * Get book data from external database
 * @param $isbn = ISBN of book
 * @return an array of values
 */
function _isbn2book_fetch_data($isbn = '') {
  // This data ought to be cached locally since ISBNdb only allows a certain number of calls each day

  switch (variable_get('isbn2node_db')) { 
    case 'isbndb' : 
      // Form the url
      $url = str_replace(array('%access_key', '%isbn_value'),
        array(variable_get('isbn2node_isbndb_access_key', ''), $isbn),
        ISBN2NODE_ISBNDB_URL);
    
      // Get and parse the XML
      $xml = file_get_contents($url);
      $p = xml_parser_create();
      xml_parse_into_struct($p, $xml, $vals, $index);
      $vals['success'] = $vals['1']['attributes']['TOTAL_RESULTS'];
      xml_parser_free($p);
    break;
  
    case 'openlib' :
      $url = str_replace('%isbn_value',
        $isbn,
        ISBN2NODE_OPENLIBRARY_BOOK_URL);

      // Get and parse the Json
      $json = file_get_contents($url);
      // drupal_set_message(vad($json));
      $vals = json_decode($json);
      if (count($vals) > 1) {
        $vals->success = 1;
      }
      else {
        $vals->success = FALSE;
      }

    break;
  }

  return $vals;
}


/*
 * Check that the settings have been set before allowing use of module forms
 * @param &$form a form array
 * @return TRUE on success, false on missing sertup and form altered to show error
 */
function _isbn2node_check_settings(&$form) {
  $ok = TRUE;
  if (!variable_get('isbn2node_db', FALSE)) {
    drupal_set_message(t('You must You must select a book database to use before using this feature.'), 'error');

    $form['error'] = array(
    '#markup' => l(t('Select a database here'), 'admin/config/content/isbn2node', array('query' => array('destination' => 'node/add/isbn2node_book/isbn2node'))),
    '#weight' => 150,
    );
    
    $ok = FALSE;
  
  }
  elseif ((variable_get('isbn2node_db', '') == 'isbndb') && !variable_get('isbn2node_isbndb_access_key', FALSE)) {
    drupal_set_message(t('You must enter an ISBNdb access key in the settings for this ISBN2node module before using this feature.'), 'error');

    $form['error'] = array(
    '#markup' => l(t('Enter your access key here'), 'admin/config/content/isbn2node', array('query' => array('destination' => 'node/add/isbn2node_book/isbn2node'))),
    '#weight' => 150,
    );

    $ok = FALSE;

  }
  return $ok;
}



/*
 * Return the fields we want from the XML returned from ISBNdb
 * and their corresponding Drupal field names
 * @param $fields, if set the attribute fields are returned, else return main fields
 * @return an array of XML-fields and their corresponding Drupal fields
 */
function _isbn2node_isbndb_tags($fields = FALSE) {
  if ($fields) {
    return array(
      'EDITION_INFO' => 'isbn2node_edition',
      'PHYSICAL_DESCRIPTION_TEXT' => 'isbn2node_physical_description',
    );
  }
  else {
    return array(
      'TITLE' => 'title',
      'AUTHORSTEXT' => 'isbn2node_author',
      'PUBLISHERTEXT' => 'isbn2node_publisher',
      'DETAILS' => 'isbn2node_details',
    );
  }
}


/*
 * Return the fields we want from the Json returned from Open Library
 * and their corresponding Drupal field names
 * @return an array of Json-fields and their corresponding Drupal fields
 */
function _isbn2node_openlib_tags() {
  return array(
    'number_of_pages' => 'isbn2node_physical_description',
    'title' => 'title',
    'subtitle' => 'isbn2node_subtitle',
    'authors' => array(0 => array('name' => 'isbn2node_author')),
    'publishers' => array(0 => array('name' => 'isbn2node_publisher')),
    'weight' => 'isbn2node_details',
    'publish_date' => 'isbn2node_year',
  );
}


/*
 * Fix ISBN to be digits only
 * @param $isbn An ISBN
 * @return The sanitized ISBN
 */
function _isbn2book_normalize_isbn($isbn) {
  return trim(str_replace(array(' ', '-'), '', $isbn));
}


/*
 * Find cover image
 * @param $isbn An ISBN
 * @return url of the cover file if found or FALSE if not
 */
function _isbn2node_fetch_cover($isbn, $size = 'L') {
  // Form the url
  $url = str_replace(array('%isbn', '%size'),
    array($isbn, $size),
    ISBN2NODE_OPENLIBRARY_COVER_URL);

  // Try to get the size
  $size = @getimagesize($url);

  // Openlibrary returns a 1x1 pixel image on failure. Anything bigger is OK
  if ($size[0] > 1) {
    return $url;
  }
  else {
    return FALSE;
  }
}


/*
 * Parse values from book databases
 * @param $vals or object with book data
 * @param $isbn ISBN to work on
 * @return array with fields corresponding to the isbn2node content type
 */
function _isbn2node_parse($vals, $isbn) {
  $isbn2book_data = array();
  switch (variable_get('isbn2node_db', '')) {
    case 'isbndb' : 
      // Work on the returned values
      if ($vals['1']['attributes']['TOTAL_RESULTS']) {
    
        $interesting = _isbn2node_isbndb_tags();
    
        foreach ($vals as $v) {

          // Get extra ISBN for backup cover check
          if (is_array($v)) {
            if (isset($v['attributes']) && is_array($v['attributes'])) {
              $a = $v['attributes'];
              if (is_array($a)) {
                if (isset($a['ISBN'])) {
                  $isbn2book_data['xisbn'] = $a['ISBN'];
                }
              }
            }
          }

          if (isset($interesting[$v['tag']])) {
            switch ($v['tag']) {
              case 'DETAILS' :
                // drupal_set_message(vad($v['attributes']));
                $fields = _isbn2node_isbndb_tags(TRUE);
                foreach ($fields as $xml_field => $drupal_field) {
                  switch ($xml_field) {
                    case 'EDITION_INFO' :
                      // Handle edition/year
                      $parts = explode(';', $v['attributes'][$xml_field]);
                      if (isset($parts[1])) {
                        $isbn2book_data['isbn2node_year'] = trim($parts[1]); 
                      }
                      $v['attributes'][$xml_field] = $parts[0];
                    default :
                      $isbn2book_data[$drupal_field] = $v['attributes'][$xml_field];
                  }
                }
                break;
              default :
                if (isset($v['value'])) {
                  $isbn2book_data[$interesting[$v['tag']]] = $v['value'];
                }
                break;
            }
          }
        }
      }
      break;
    
    case 'openlib' :
      $qual = 'ISBN:' . $isbn;
      if (isset($vals->$qual)) {
        $vals = $vals->$qual;
        $interesting = _isbn2node_openlib_tags();
        
        foreach ($interesting as $json => $drupal) {
          $value = '';
          if (is_array($drupal)) {
            foreach ($drupal as $js => $dr) {
              foreach ($dr as $j => $d) {
                if (isset($vals->$json)) {
                  $value = $vals->$json;
                  $value = $value[0]->$j;
                  $isbn2book_data[$d] = $value;
                }
              }
            }
          }
          else {
            if (isset($vals->$json)) {
              $value = $vals->$json;
            }
            $isbn2book_data[$drupal] = ($drupal == 'isbn2node_physical_description') ? t('@pages pages', array('@pages' => $value)) : $value;
          }
        }
      }
      break;
  }
  return $isbn2book_data;
}


/*
 * Make and save a node based on a fields array
 * @param $fields array of node fields for isbn2node_book content type
 */
function _isbn2node_make_node($fields) {
  global $user;

  $node = new stdClass();
  $node->title = $fields['title'];
  $node->type = 'isbn2node_book';
  $node->created = time();
  $node->changed= time();
  $node->status = 1;
  $node->promote = 1;
  $node->sticky = 0;
  $node->uid = $user->uid;
  $node->language = 'en';
  $node->timestamp = time();
  $node->revision = 0;

  // Remove title
  unset($fields['title']);

  // Handle the rest of the fields
  foreach ($fields as $field => $value) {
    if ($field == 'isbn2node_cover') {
      $cover = array('fid' => 0);
      // Handle special structure for image field
      foreach ($value as $k => $v) {
        $cover[$k] = $v;
      }
    $node->$field = array('und' => array(0 => $cover));
    }
    else {
      // Transfer normal field
      $node->$field = array('und' => array(0 => array('value' => $value)));
    }
  }

  node_submit($node);
  node_save($node);

  if (isset($node->nid)) {
    drupal_set_message(t('Created !title from ISBN @isbn.', array('!title' => l($node->title, 'node/' . $node->nid), '@isbn' => $fields['isbn2node_isbn'])));
  }
  else {
    drupal_set_message(t('Failed creating book from ISBN @isbn.', array('@isbn' => $fields['isbn2node_isbn'])));
  }
  
}


/*
 * Get cover file and create file object in database
 * @param $isbn ISBN to look for
 * @return populated file object on succes, empty file object (->fid = 0) on failure
 */
function _isbn2node_save_cover($isbn, $xisbn = '') {
  // Create empty file object
  $file = new stdClass();
  $file->fid = 0;

  // Use backup ISBN if set
  $isbn = $xisbn ? $xisbn : $isbn;

  // Create URL and set filename of local file
  $url = _isbn2node_fetch_cover($isbn);
  
  if ($url) {
    $parts = pathinfo($url);
  
    $filename = 'cover_' . $isbn . '.' . $parts['extension'];

    // Fetch cover file
    $file_temp = file_get_contents($url);
    if ($file_temp) {
      // Create file in database
      $file = file_save_data($file_temp, 'public://' . $filename, FILE_EXISTS_RENAME);
    }
  }
  
  return $file;
}


/*
 * Check for ISBN already in database
 * @param $isbn ISBN to look for
 * @return FALSE if none found, assoc array of results if any found
 */
function _isbn2node_isbn_exists($isbn) {
  return db_select('field_data_isbn2node_isbn', 'nid')
    ->fields('nid')
    ->condition('isbn2node_isbn_value', $isbn, '=')
    ->execute()
    ->fetchAssoc();
}


/*
 * Search for title in external database
 * @param $title = Book title
 * @return an array of result values
 */
function _isbn2book_search_title($title) {
  // This data ought to be cached locally since ISBNdb only allows a certain number of calls each day

  switch (variable_get('isbn2node_db')) { 
    case 'isbndb' : 
      // Form the url
      $url = str_replace(array('%access_key', '%title_value'),
        array(variable_get('isbn2node_isbndb_access_key', ''), urlencode($title)),
        ISBN2NODE_ISBNDB_TITLE_URL);

      // Get and parse the XML
      $xml = file_get_contents($url);
      $p = xml_parser_create();
      xml_parse_into_struct($p, $xml, $vals, $index);
      $vals['success'] = $vals['1']['attributes']['TOTAL_RESULTS'];
      xml_parser_free($p);
    break;
  
    case 'openlib' :
      $url = str_replace('%isbn_value',
        $isbn,
        ISBN2NODE_OPENLIBRARY_BOOK_URL);

      // Get and parse the Json
      $json = file_get_contents($url);
      // drupal_set_message(vad($json));
      $vals = json_decode($json);
      if (count($vals) > 1) {
        $vals->success = 1;
      }
      else {
        $vals->success = FALSE;
      }

    break;
  }

  return $vals;
}

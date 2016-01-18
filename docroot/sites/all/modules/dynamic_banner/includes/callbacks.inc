<?php
// $Id$

/**
 * @file
 * Dynamic Banner Admin Pages and various other functions to make them work
 * Most of the code in this file was derived from path module
 */

/**
 * Return a listing of all defined URL aliases.
 * When filter key passed, perform a standard search on the given key,
 * and return the list of matching URL aliases.
 */
function dynamic_banner_admin_page() {
  $output = ""; // default 

  // grab the filter if the user set one
  $filter = dynamic_banner_build_filter_query();
  
  // Add the filter form above the overview table. // fix bug first //
  //$output['dynamic_banner_admin_filter_form'] = drupal_get_form('dynamic_banner_admin_filter_form');
  
  // construct the headers of the table
  $header = array(
    array(
      'data'  => t('Url'),
      'field' => 'd.path',
      'sort'  => 'asc',
    ),
    array(
      'data'  => t('ImgUrl'),
    ),
    array(
      'data'  => t('Text'),
      'field' => 'd.text',
    ),
    array(
      'data'  => t('Link'),
      'field' => 'd.link',
    ),
    array(
      'data'  => t('Mode'),
      'field' => 'd.mode',
    ),
    array(
      'data'    => t('Operations'),
      'colspan' => '2',
    ),
  );

  // contruct the db call for the list
  // load all data fields and attach pager and sorter function in
  $query = db_select('dynamic_banner', 'd')->extend('PagerDefault')->extend('TableSort'); // alias the table name to d
  $query->fields('d') // Select all fields from the node table. 
    ->limit(20) // Load the first 20 rows.
    ->orderByHeader($header);
  
  // find if the filter has returned a where clause and add it in before executing
  if (!empty($filter['where'])) {
    $query->where($filter['where'], $filter['args']);// not working yet
  }
  $result = $query->execute();

  // start constructing the individual rows
  $rows = array();
  foreach ($result as $data) {
    $image = dynamic_banner_image_handler($data->imgurl, $data->imgfid);
    $rows[] = array('data' => 
      array(
        $data->path,
        $image,
        $data->text,
        $data->link,
        $data->mode,
        l(t('edit'), "admin/structure/banners/edit/". $data->dbid),
        l(t('delete'), "admin/structure/banners/delete/" . $data->dbid)
      )
    );
  }
  
  // construct the call for the theme function to run on this
  $output['dynamic_banner_table'] = array(
    '#theme'  => 'table', 
    '#header' => $header, 
    '#rows'   => $rows, 
    '#empty'  => t('No Banners Found.'),
  );
  
  // adds the pager buttons to the bottom of the table
  $output['dynamic_banner_pager'] = array('#theme' => 'pager');

  // let drupal handle print and echo
  return $output;
}
/**
 * Needed for the filtering of the banners page
 */
function dynamic_banner_build_filter_query() {
  if (empty($_SESSION['dynamic_banner_filter'])) {
    return;
  }

  $filters = dynamic_banner_filters();

  // Build query
  $where = $args = array();
  foreach ($_SESSION['dynamic_banner_filter'] as $key => $filter) {
    $filter_where = array();
    foreach ($filter as $value) {
      $filter_where[] = $filters[$key]['where'];
      $args[] = $value;
    }
    if (!empty($filter_where)) {
      $where[] = '(' . implode(' OR ', $filter_where) . ')';
    }
  }
  $where = !empty($where) ? implode(' AND ', $where) : '';

  return array(
    'where' => $where, 
    'args' => $args,
  );
}

/**
 * The specific filters that can be used for banners
 */
function dynamic_banner_filters() {
  $filters = array();
  $filters['type'] = array(
    'title'   => t('Type'),
    'where'   => 'd.url ?',
    'options' => array('NOT LIKE %* AND NOT LIKE %!','LIKE %*','LIKE %!'));
  return $filters;
}

/**
 * Return a form to filter Banners.
 *
 * ingroup forms
 * see dynamic_banner_admin_filter_form_submit()
 */
function dynamic_banner_admin_filter_form($form) {
  $filters = dynamic_banner_filters();
  
  $form['filters'] = array(
    '#type'        => 'fieldset', 
    '#title'       => t('Filter dynamic banner'), 
    '#collapsible' => TRUE, 
    '#collapsed'   => empty($_SESSION['dynamic_banner_filter']),
  );
  
  foreach ($filters as $key => $filter) {
    $form['filters']['status'][$key] = array(
      '#title'    => $filter['title'], 
      '#type'     => 'select', 
      '#multiple' => TRUE, 
      '#size'     => 8, 
      '#options'  => $filter['options'],
    );
    if (!empty($_SESSION['dynamic_banner_filter'][$key])) {
      $form['filters']['status'][$key]['#default_value'] = $_SESSION['dynamic_banner_filter'][$key];
    }
  }
  
  $form['filters']['actions'] = array(
    '#type'       => 'actions', 
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['filters']['actions']['submit'] = array(
    '#type'  => 'submit', 
    '#value' => t('Filter'),
  );
  if (!empty($_SESSION['dynamic_banner_filter'])) {
    $form['filters']['actions']['reset'] = array(
      '#type'  => 'submit', 
      '#value' => t('Reset'),
    );
  }

  return $form;
}

/**
 * Validate result from dynamic banner administrative filter form.
 */
function dynamic_banner_admin_filter_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == t('Filter') && empty($form_state['values']['type'])) {
    form_set_error('type', t('You must select something to filter by.'));
  }
}

/**
 * Process result from dynamic banner administrative filter form.
 */
function dynamic_banner_admin_filter_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  $filters = dblog_filters();
  switch ($op) {
    case t('Filter'):
      foreach ($filters as $name => $filter) {
        if (isset($form_state['values'][$name])) {
          $_SESSION['dynamic_banner_filter'][$name] = $form_state['values'][$name];
        }
      }
      break;
    case t('Reset'):
      $_SESSION['dynamic_banner_filter'] = array();
      break;
  }
  return 'admin/structure/banners/list/';
}

/**
 * Process filter form submission when the Reset button is pressed.
 */
function dynamic_banner_admin_filter_form_submit_reset($form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/banners/list';
}

/**
 * The main form dealing with dynamic banner 
 * There is now only one form for dynamic banner to deal with unlink in the d6 version
 *
 * INPUT: arg(4) is from the url path of this form
 */
function dynamic_banner_admin_form($form, &$form_state) {
  $dbid = arg(4); // the last portion of the url there must be a better way of doing this
  
  // This is used by the file handler, It is needed to accept files
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  
  $file_path = drupal_get_path('module', 'file');
  
  // default the variables here
  $banner = NULL; // prevent bugs nulify the variable
  $default_flag = FALSE; // enable variable in this scope
  
  // hide it so that the user may not change this element
  $form['dbid'] = array(
    '#type'          => 'hidden',
    '#required'      => FALSE,
  );

  if ($dbid == '') {
    // this will disable the path field for the default banner
    if (strrpos($_GET['q'], "/default")) {
      drupal_set_title(t('Default Banner'));
      // load the default if there is one
      $banner = dynamic_banner_find_load_default();
    }
    else {
      drupal_set_title(t('New Banner'));
      $form['dbid']['#value'] = NULL;
    }
  }
  else {
    // The dbid is set so a banner must exist load it
    $banner = dynamic_banner_load_banner($dbid);
    drupal_set_title(t("Edit Banner") . " '" . $banner->path . "'");
    $form['dbid']['#value'] = $dbid;
  }
  
  // this will prevent the used from changing this field once the default has been loaded
  // it deals with a bug if the person chose to edit the specific banner for default rather than pressing default
  if ($banner && $banner->path == 'DEFAULT') {
    $default_flag = TRUE;
  }
  
  // disable the path form element when the default flag is out
  if (!$default_flag) {
    $form['path'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Banner Path'),
      '#default_value' => $banner ? $banner->path : "",
      '#size'          => 45,
      '#maxlength'     => 250,
      '#description'   => t('Specify an existing url path you wish to put a banner on. For example: home, user* (wild card), content! (random). Enter a path as it appears in the url of your site.'),
      '#field_prefix'  => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
      '#required'      => TRUE,
    );
  }
  else {
    $form['path'] = array(
      '#type'          => 'hidden',
      '#title'         => t('Banner Path'),
      '#value'         => 'DEFAULT',
    );
  }
  
  // if the module exists add the autocomplete path
  // i might have to do my own autocomplete here cause mpac doesnt really do what i need it to do
  if ( module_exists('mpac') ) {
    $form['path']['#autocomplete_path'] = 'mpac/autocomplete/alias';
  }
  
  $form['image_type'] = array(
    '#type'    => 'radios',
    '#options' => drupal_map_assoc(array(t('Use Existing Image(s)'), t('Upload New Image(s)'))),
    '#title'   => t('Choose image type.')
  );
  
  if ( $banner && isset($banner->imgurl)) {
    $form['image_type']['#default_value'] = t('Use Existing Image(s)');
  }
  if ( $banner && isset($banner->imgfid)) {
    $form['image_type']['#default_value'] = t('Upload New Image(s)');
  }

  /**
   * Note: There are two form elements for the same thing 
   * They are both not required but only one is needed for proper handling
   * When we are loading an old banner load the url into imgurl
   * When we are uploading a new image the validator will upload the image store it and fill in imgurl for you
   * Only use one method no mix and matching
   * When reading the data use checks to see which method was used
   */
  $form['imgurl'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Typeout the url of the image'),
    '#default_value' => $banner ? $banner->imgurl : '',
    '#description'   => t('Specify an image(s) for the banner to display.'),
    '#field_prefix'  => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
    //'#required'    => TRUE,
    '#states'        => array(
      'visible'      => array(
        ':input[name="image_type"]' => array('value' => t('Use Existing Image(s)')),
      ),
    ),
  );
  
  /** 
   * Since upon pressing the delete button on the image the fid is set to 0
   * We need to save is because we still need to delete that image.
   */
  $form['oldimagefid'] = array(
    '#type'          => 'hidden',
    '#required'      => FALSE,
    '#value'         => $banner ? $banner->imgfid : '',
  );

  $form['image'] = array(
    '#title'              => t('Choose Image File'),
    '#type'               => 'managed_file',
    '#default_value'      => $banner ? $banner->imgfid : '',
    '#attached'           => array(
      'js'                => array($file_path . '/file.js'),
     ),
    '#progress_indicator' => 'throbber',
    '#progress_message'   => NULL,
    '#upload_location'    => variable_get('dynamic_banner_file_save_path', BANNER_DEFAULT_SAVE_LOCATION),
    '#description'        => t('Specify an image(s) for the banner to display.'),
    //'#required'         => TRUE,
    '#states'             => array(
      'visible'           => array(
        ':input[name="image_type"]' => array('value' => t('Upload New Image(s)')),
      ),
    ),
  );
  $form['#validate'][] = 'dynamic_banner_upload_image_validate';

  $form['text'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Text'),
    '#default_value' => $banner ? $banner->text : '',
    '#maxlength'     => 250,
    '#size'          => 45,
    '#description'   => t('Specify the text to associate with this banner [comma seperated for randoms, also must match amount of elements from images] (optional).'),
    '#required'      => FALSE,
  );

  $form['link'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Link'),
    '#default_value' => $banner ? $banner->link : '',
    '#maxlength'     => 250,
    '#size'          => 45,
    '#description'   => t('Specify the link you want your banner to point to (optional).'),
    '#required'      => FALSE,
  );

  $form['mode'] = array(
    '#type'          => 'radios',
    '#title'         => t('Mode'),
    '#options'       => drupal_map_assoc(array(t('normal'), t('time_based'), t('rotating'), t('fade'))),
    '#default_value' => $banner ? $banner->mode : BANNER_DEFAULT_BANNER_MODE,
    '#description'   => t('What mode do you want this banner to display under (this is different than display setting)'),
    '#required'      => TRUE,
  );
  /*
  $form['time_on'] = array(
    '#type'          => 'date',
    '#title'         => t('Start Time'),
    '#description'   => t('Specify the time you want your banner to start displaying (optional).'),
    '#required'      => FALSE,
    '#states'        => array(
      'visible'      => array(
        ':input[name="mode"]' => array('value' => t('time_based')),
      ),
    ),
  );
  
  $form['time_off'] = array(
    '#type'          => 'date',
    '#title'         => t('End Time'),
    '#description'   => t('Specify the time you want your banner to stop displaying (optional).'),
    '#required'      => FALSE,
    '#states'        => array(
      'visible'      => array(
        ':input[name="mode"]' => array('value' => t('time_based')),
      ),
    ),
  );*/

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Banner'),
  );

  return $form;
}
/**
 * Validate/submit handler used for handling image uploads
 */
function dynamic_banner_upload_image_validate($form, &$form_state) {
  // This is not needed, I use this to use the same validate function
  // for several fields.
  /*$file = file_save_upload($form['values']['image'], array(
    'file_validate_is_image'   => array(),
    'file_validate_extensions' => array('png gif jpg jpeg'),
  ));*/
  if ( isset($form_state['values']['image']) && $form_state['values']['image'] != 0) {
    $file = file_load($form_state['values']['image']);

    if ($file) {
      // Get the image info to get the correct extension for the uploaded file.
      /*$info = image_get_info($file->filepath);
      if (file_move($file, 'destination/filename'. $info['extension'], FILE_EXISTS_REPLACE)) {
        // Mark the file for permanent storage.
        file_set_status($file, FILE_STATUS_PERMANENT);
        // Update the files table.
        drupal_write_record('files', $file, 'fid');*/
      // Change status to permanent.
      $file->status = FILE_STATUS_PERMANENT;

      // When a module is managing a file, it must manage the usage count.
      // Here we increment the usage count with file_usage_add().
      file_usage_add($file, 'dynamic_banner', 'banner', 1);

      // Save the file again for permanent status
      file_save($file);
    }
    else {
      form_set_error($form_state['values']['image'], t('Failed to write the uploaded file to the folder.'));
    }
  }
  // Delete the image
  elseif ($form_state['values']['image'] == 0) {
    dynamic_banner_image_delete($form_state['values']['oldimagefid']);
  }
}

/**
 * Verify that the Banner is valid
 * It makes sure that the sql does not throw errors
 */
function dynamic_banner_admin_form_validate($form, &$form_state) {
  // For a banner to exist it needs a path that it is assigned to and an image, thats it.
  if (isset($form_state['values']['path']) &&
      (isset($form_state['values']['image']) || isset($form_state['values']['imgurl']))) {

    $path = $form_state['values']['path'];

    if ($path != 'DEFAULT') {
      // check db before altering the path variable
      // check for more than one of the same path banners
      if (db_query("SELECT COUNT(path) FROM {dynamic_banner} WHERE path = :path", array(':path' => $path))->fetchField() > 1) {
        form_set_error('path', t('The path %path is already in use.', array('%path' => $path)));
        return;
      }
      // path is not clean at this point because of wildcard and random must chop those characters off
      // find the * or wildcard
      $wild_position = strrpos($path, "*");
      if ($wild_position !== FALSE) {
        $path = drupal_substr($path, 0, $wild_position);
      }
      // find the ! or random
      $rand_position = strrpos($path, "!");
      if ($rand_position !== FALSE) {
        $path = drupal_substr($path, 0, $rand_position);
      }

      if (drupal_lookup_path('source', $path)) {
        // We are making a new banner previous checks should be enough to deal with validation
        if ( $dbid != 0) {
          return;
        }
      }
      else {
        form_set_error('path', t('The path %path is not known by drupal.', array('%path' => $path)));
        return;
      }
    }
  }
  else {
    form_set_error('path', t('There was a problem with the required fields please check the form and try again.'));
    return;
  }
}

/**
 * Save a new Banner to the database
 */
function dynamic_banner_admin_form_submit($form, &$form_state) {
  // define a sort of struct array for display mode for form translation
  //$mode_struct = array('normal', 'time_based', 'rotating', 'fade');
  
  // extra validation check to make sure
  if ($form_state['values']['image_type'] == t('Use Existing Image(s)')) {
    $imgurl = $form_state['values']['imgurl'];
  }
  else {
    $imgurl = NULL;
  }
  if ($form_state['values']['image_type'] == t('Upload New Image(s)')) {
    $imgfid = $form_state['values']['image'];
  }
  else {
    $imgfid = NULL;
  }
  
  $path     = $form_state['values']['path'];
  $text     = $form_state['values']['text'];
  $link     = $form_state['values']['link'];
  $mode     = $form_state['values']['mode'];
  //$time_on  = $form_state['values']['time_on'];// these are arrays we need a time handler
  //$time_off = $form_state['values']['time_off'];
  $dbid     = $form_state['values']['dbid'];
  
  $time_on  = NULL;
  $time_off = NULL;

  // Save the banner
  dynamic_banner_set_banner($path, $imgurl, $imgfid, $text, $link, $mode, $time_on, $time_off, $dbid);

  drupal_set_message(t('The banner has been saved.'));
  $form_state['redirect'] = 'admin/structure/banners';
}

/**
 * Set a banner for a given path, preventing duplicates.
 * Note if dbid comes in null then we are creating a banner
 */
function dynamic_banner_set_banner($path, $imgurl, $imgfid, $text, $link, $mode, $time_on, $time_off, $dbid = NULL) {
  if (!$mode) {
    $mode = BANNER_DEFAULT_BANNER_MODE;
  }

  // First we check if we are dealing with an existing alias and delete or modify it based on dbid.
  // we dont need to do a complicated check here because the code already made it for us
  if ($dbid) {
    // Update the existing banner.
    db_update('dynamic_banner')->fields(array(
      'path'       => $path,
      'imgurl'     => $imgurl,
      'imgfid'     => $imgfid,
      'text'       => $text,
      'link'       => $link,
      'mode'       => $mode,
      'start_time' => $time_on,
      'end_time'   => $time_off,
    ))->condition('dbid', $dbid)->execute();
  }
  else {
    db_insert('dynamic_banner')->fields(array(
      'path'       => $path,
      'imgurl'     => $imgurl,
      'imgfid'     => $imgfid,
      'text'       => $text,
      'link'       => $link,
      'mode'       => $mode,
      'start_time' => $time_on,
      'end_time'   => $time_off,
    ))->execute();
  }
}

/**
 * Menu callback; confirms deleting a Banner
 */
function dynamic_banner_admin_delete_confirm($form, $form_state) {
  // the last part of the url
  $dbid = arg(4);

  $banner = dynamic_banner_load_banner($dbid);
  if (user_access('administer dynamic_banner')) {
    $form['dbid'] = array(
      '#type' => 'hidden',
      '#value' => $dbid,
    );
    $output = confirm_form($form, 
      t('Are you sure you want to delete banner %title?', array('%title' => $banner->path)), 
      isset($_GET['destination']) ? $_GET['destination'] : 'admin/structure/banners');
  }
  return $output;
}

/**
 * Execute banners deletion
 */
function dynamic_banner_admin_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    dynamic_banner_admin_delete($form_state['values']['dbid']);
    $form_state['redirect'] = 'admin/structure/banners';
    return;
  }
}

/**
 * Post-confirmation; delete a Banner
 */
function dynamic_banner_admin_delete($dbid = 0) {
  db_delete('dynamic_banner')->condition('dbid', $dbid)->execute();
  drupal_set_message(t('The banner has been deleted, the image still exists though'));
}

/**
 * Fetch a specific banner from the database.
 */
function dynamic_banner_load_banner($dbid) {
  $query = db_select('dynamic_banner', 'd');
  $query->condition('d.dbid', $dbid, '=')
    ->fields('d');
  $result = $query->execute()->fetchObject();

  if ($result) {
    return $result;
  }
  return NULL;
}

/**
 * Find the default banner and return all of it's attributes
 */
function dynamic_banner_find_load_default() {
  $query = db_select('dynamic_banner', 'd');
  $query->condition('d.path', 'DEFAULT', '=')
    ->fields('d');
  $result = $query->execute()->fetchObject();

  if ($result) {
    return $result;
  }
  
  // do not return null for this
  $blank_banner = new stdClass();
  $blank_banner->dbid = 0;
  $blank_banner->path = 'DEFAULT';
  $blank_banner->imgurl = '';
  $blank_banner->mode = 'normal';
  $blank_banner->text = '';
  $blank_banner->link = '';
  $blank_banner->imgfid = '';
  return $blank_banner;
}

/**
 * A page that will display a form for changing how dynamic banner will function
 */
function dynamic_banner_settings($form, $form_state) {
  // todo need to find a better way of doing this
  $errors_current_setting = 1;
  if ( variable_get('dynamic_banner_display_errors', BANNER_DEFAULT_ERROR) ) {
    $errors_current_setting = 0;
  }

  $form['display_setting'] = array(
    '#type'          => 'radios',
    '#title'         => t('Display Setting'),
    '#options'       => drupal_map_assoc(array(t('url'), t('text'), t('urltext'), t('urllink'))),
    '#default_value' => variable_get( 'dynamic_banner_display_setting', BANNER_DEFAULT_OUTPUT ),
    '#description'   => t('What display pattern do you want the module to follow in the template file'),
    '#required'      => TRUE,
  );

  $form['display_errors'] = array(
    '#type'          => 'radios',
    '#title'         => t('Display Errors?'),
    '#options'       => array(t('yes'), t('no')),
    '#default_value' => $errors_current_setting,
    '#description'   => t('If dynamic banner can not find a banner for the current page do you want it to display an error?'),
    '#required'      => TRUE,
  );// does this still work when there is a default banner

  $form['image_save_path'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Image save path'),
    '#default_value' => variable_get('dynamic_banner_file_save_path', BANNER_DEFAULT_SAVE_LOCATION),
    '#description'   => t('This will be the path all banners get saved to when using the upload utility. \'public://\' is your sites files folder. '),
    '#required'      => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * when the settings form submits this function will save the settings for use
 */
function dynamic_banner_settings_submit($form, &$form_state) {
  $display = $form_state['values']['display_setting'];
  variable_set('dynamic_banner_display_setting', $display);

  $errors = $form_state['values']['display_errors'];

  // todo fix this
  if ($errors == 0) {
    variable_set('dynamic_banner_display_errors', TRUE);
  }
  else {
    variable_set('dynamic_banner_display_errors', FALSE);
  }

  variable_set('dynamic_banner_file_save_path', $form_state['values']['image_save_path']);

  $form_state['redirect'] = 'admin/structure/banners';
}

/**
 * This function will load imgurl if there is no url for img
 * then it will load the fids into path format
 *
 * Input 1: The imgurl(s) that we are loading [maybe csv]
 * Input 2: The imgfid(s) that we are loading [maybe csv]
 */
function dynamic_banner_image_handler($imgurl, $imgfid) {
  // we have found the imgurl already in the right format return it
  if ($imgurl && $imgurl != '') {
    return $imgurl;
  }
  else {
    if ( strrpos($imgfid, ',') ) {
      // split the plain string into an array
      $all_fids = explode(",", $imgfid);
      // load all files at once
      $all_files = file_load_multiple($all_fids);
      $retval = "";// default the return string
      // go into all the loaded files
      foreach ($all_files as $file) {
        // if this is the first time through do not add a comma to the string
        if ($retval != "") {
          $retval .= ",";
        }
        // have to translate the public string in the uri back into something browsers understand
        $retval .= str_replace('public://', variable_get('file_public_path', '') . '/', $file->uri);
      }
      return $retval;
    }
    else {
      $file = file_load($imgfid);
      // have to translate the public string in the uri back into something browsers understand
      $file_path = str_replace('public://', variable_get('file_public_path', '') . '/', $file->uri);
      return $file_path;
    }
  }
}

/**
 * This function will split the csv fid variable if it needs to be split
 * And then delete those images from the file system and thier values in the db
 */
function dynamic_banner_image_delete($fid) {
  if ( strrpos($fid, ',') ) {
    // split the plain string into an array
    $all_fids = explode(",", $imgfid);
    // load all files at once
    $all_files = file_load_multiple($all_fids);
    
    foreach($all_files as $file) {
      if ($file) {
        // When a module is managing a file, it must manage the usage count.
        // Here we decrement the usage count with file_usage_delete().
        file_usage_delete($file, 'dynamic_banner', 'banner', 1);

        // The file_delete() function takes a file object and checks to see if
        // the file is being used by any other modules. If it is the delete
        // operation is cancelled, otherwise the file is deleted.
        file_delete($file);
      }
    
      drupal_set_message(t('The image @image_name was removed.', array('@image_name' => $file->filename)));
    }
  }
  else {
    $file = $fid ? file_load($fid) : FALSE;
    
    if ($file) {
      // When a module is managing a file, it must manage the usage count.
      // Here we decrement the usage count with file_usage_delete().
      file_usage_delete($file, 'dynamic_banner', 'banner', 1);

      // The file_delete() function takes a file object and checks to see if
      // the file is being used by any other modules. If it is the delete
      // operation is cancelled, otherwise the file is deleted.
      file_delete($file);
    }
    
    drupal_set_message(t('The image @image_name was removed.', array('@image_name' => $file->filename)));
  }
}

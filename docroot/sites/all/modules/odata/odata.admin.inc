<?php
/**
 *  @file
 *  This file contains the admnistrative UI.
 */

 /**
  * This function provides a form for creating a new OData web service 
  */
function odata_create_endpoints_form($form, &$form_state, $edit = NULL) {
  
  $form = array();

  $form['title'] = array(
    '#type' => 'textfield', 
    '#title' => t('Title'), 
    '#default_value' => '', 
  '#description' => t('This name will be used from the Views module, in the "Show" drop down menu.'), 
    '#required' => TRUE,
  );
  
  $form['uri'] = array(
    '#type' => 'textfield', 
    '#title' => t('Endpoint URI'), 
    '#description' => t('The URL of the endpoint.'), 
    '#default_value' => '', 
    '#required' => TRUE,
  );
  
  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Add'),
  );

  return $form;
}

/**
 *  Validates the input of odata_create_endpoints_form
 */
function odata_create_endpoints_form_validate($form, &$form_state) {

  if ( $form_state['values']['title'] != check_plain($form_state['values']['title']) || strstr($form_state['values']['title'], ' ') ) {
    form_set_error( 'title', t('Try a simple title') );
  }

  if (!valid_url($form_state['values']['uri'], TRUE)) {
    form_set_error( 'uri', t('Enter a valid uri. e.g. http://example.com/odataservice/') );
  }
  $result = db_query('SELECT title FROM {odata_endpoints} WHERE title= :title', array(':title' => $form_state['values']['title'] ));
  foreach ($result as $k) {
    if ($k->title == $form_state['values']['title'] ) {
      form_set_error( 'title', t('Title already exists') );
    }
  }
}

/**
 *  It reads the input of odata_create_endpoints_form
 */
function odata_create_endpoints_form_submit($form, &$form_state) {
  global $base_url;
  $uri = $form_state['values']['uri'];
  if ( strrpos( $uri, '/') == drupal_strlen($uri)-1  ) {
    $uri=substr($uri, 0, drupal_strlen($uri)-1);
  }
  drupal_goto( $base_url . '/admin/structure/odata/create/get-structure/' . urlencode($form_state['values']['title']) . '/?uri=' . urlencode($uri));
}

/**
 *  In this form, you can choose the Collection of the OData service that you want to work with
 */
function odata_get_structure_endpoints_form($form, &$form_state, $edit = NULL) {
  $form = array();
  $title = arg(5);
  $uri =  (isset($_GET['uri']))?$_GET['uri']:NULL;
  $serial_options=array();

  if (!empty($title )) {
    $result = db_query('SELECT url,options FROM {odata_endpoints} WHERE title=:title', array(':title' => $title));
    $serial_options=array();
    foreach ($result as $row) {
      $uri= $row->url;
      $serial_options=unserialize($row->options);
    }
  }

  if (valid_url( $uri ) ) {
    $uri_metadata = $uri . '/$metadata';
    $reply = drupal_http_request($uri_metadata);
    $replydata = $reply->data;
    $supertree = odata_xml2array2($replydata, $uri);

  $tree = odata_get_entity_properties($supertree);

    $entitiesops=array();
  $not_default = !is_array($serial_options) || count($serial_options)!=0;
    foreach ($tree as $keys => $values) {
      $entitiesops[$keys] = $keys;
    if ( is_array($serial_options) && count($serial_options)==0 )$serial_options=$keys;
    }

    $form['collections'] = array(
      '#type' => 'radios',
      '#options' => $entitiesops,
      '#default_value' => $serial_options,
      '#title' => t('Available collection sets'),
      '#description' => t('Select the collection you want to add.'),
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );

    if ( $not_default ) {
      $form['cancel'] = array(
        '#type' => 'markup',
        '#markup' => l( t('Cancel'), 'admin/structure/odata/list' )
      );
    }
  
    $form['uri'] = array( '#type' => 'hidden', '#value' => $uri);
    $form['title'] = array( '#type' => 'hidden', '#value' => $title);
  }
  return $form;
}

/**
 *  It validates odata_get_structure_endpoints_form
 */
function odata_get_structure_endpoints_form_validate($form, &$form_state) {
  if ( $form_state['values']['title'] != check_plain($form_state['values']['title']) || strstr($form_state['values']['title'], ' ') ) {
    form_set_error( 'title', t('Try the wizard from the beginning'));
  }
  if (!valid_url($form_state['values']['uri'])) {
    form_set_error( 'uri', t('Try the wizard from the beginning'));
  }
}

/**
 *  Saves the changes of odata_get_structure_endpoints_form
 */
function odata_get_structure_endpoints_form_submit($form, &$form_state) {
  global $base_url;
  $result = db_query('SELECT url FROM {odata_endpoints} WHERE title=:title', array(':title' => $form_state['values']['title']));
  $newuri=FALSE;
  foreach ($result as $row) {
    $newuri= $row->url;
  }
  $serialchecks = array();
  /* foreach ($form_state['values']['collections'] as $key => $value) {
    if ($value!==0) {
      $serialchecks[]= $key;
    }
  }*/
  $serialchecks = serialize( $form_state['values']['collections'] );
  
  if (!$newuri) {
    $ret= db_insert('odata_endpoints')->fields(array('title' => $form_state['values']['title'], 'url' => $form_state['values']['uri'], 'options' => $serialchecks ))->execute();
  }
  else {
    $ret= db_update('odata_endpoints')->fields(array('url' => $form_state['values']['uri'], 'options' => $serialchecks ))->condition('title', $form_state['values']['title'] )->execute();
  }
  drupal_set_message(t('Your settings have been saved.'));
  $url_clear_cache = array('!url' => l(t('clear your caches'), 'admin/config/development/performance'));
  drupal_set_message(t('It is recommended to !url.', $url_clear_cache), 'warning' );
  drupal_goto( 'admin/structure/odata/' );
}

/**
 *  Returns a list of all Web Services endpoints
 */
function odata_get_endpoints_form() {
  global $base_url;
  $result = db_query('SELECT title,url FROM {odata_endpoints}');
  if ($result->rowCount()<=0) {
    return t('You have not create any oData endpoints yet.');
  }
  $header = array( array( 'data' => 'Title'), array( 'data' => 'URI' ), array( 'data' => 'Manage' ) );

  $rows=array();
  foreach ($result as $dbrow) {
    $edithref= 'admin/structure/odata/create/get-structure/' . urlencode($dbrow->title);
    $deletehref= 'admin/structure/odata/' . urlencode($dbrow->title) . '/delete/';
    $rows[]=array( 
      array( 'data' => $dbrow->title),
      array( 'data' => $dbrow->url),
      array( 'data' => l( t('Edit'), $edithref ) . ' | ' . l( t('Delete'), $deletehref )),
    );
  }
  /*
  $form['description'] = array(
      '#type' => 'markup',
    '#markup' => 
    );
  //*/
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 *  Creates a confirm deletion dialog
 */
function odata_delete_endpoints_form($form, &$form_state, $edit = NULL) {
  $form = array();
  
  $result = db_query('SELECT title FROM {odata_endpoints} WHERE title=:title', array(':title' => arg(3) ));
  $endpoint=NULL;
  foreach ($result as $k => $v) {
    $endpoint = $v;
  }
  
  if (!empty($endpoint)) {
    $form['title'] = array(
      '#type' => 'hidden', 
      '#default_value' => check_plain($endpoint->title)
    );
  
    $form['text'] = array(
      '#type' => 'markup', 
      '#markup' => '<div>' . t('Are you sure that you want to delete') . ' ' . check_plain($endpoint->title) . '?' . '</div>'
    );
  
    $form['submit'] = array(
      '#type' => 'submit', 
      '#value' => t('Delete'), 
      '#submit' => array( 'odata_delete_endpoints_form_submit')
    );
  }
  $form['cancel'] = array(
    '#type' => 'markup', 
    '#markup' => l( t('Cancel'), 'admin/structure/odata/list' ) 
  );

  return $form;
}

/**
 *  It does the deletion of the OData web service
 */
function odata_delete_endpoints_form_submit($form, &$form_state, $edit = NULL) {
  $title = $form_state['values']['title'];
  $result = db_delete('odata_endpoints')->condition('title', $title)->execute();
  drupal_set_message( t('You succesfully deleted the @title collection set.', array('@title' => $title)) );
  drupal_goto( 'admin/structure/odata/list');
}

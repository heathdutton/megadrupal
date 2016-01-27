<?php
// $Id: simple_page_title.admin.inc,v 1.1 2011/02/02 22:13:28 jeebsuk Exp $

/**
 * @file
 * Simple Page Title - Administration Pages & Functions
 *
 * @author
 * Jamie Giberti <jagiberti@gmail.com>
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function simple_page_title_admin_page() {
  $html = '';
  $form = drupal_get_form('simple_page_title_custom_title_form');

  $html .= drupal_render($form);
  
//  $query = db_select('simple_page_title', 'spt')->extend('PagerDefault');
//  $query->fields('spt', array('pt_id', 'page_title', 'path'));
//  $query->limit(10);
//  $result = $query->execute();

  $header = array(
    array(
      'data' => t('Path'),
      'field' => 'path',
      'sort' => 'asc',
    ),
    array(
      'data' => t('Page Title'),
      'field' => 'page_title',
    ),
  );  
  
  $query = db_select('simple_page_title', 'spt')->extend('TableSort')->extend('PagerDefault');
  $query->fields('spt', array('pt_id', 'page_title', 'path'));
  $query->orderByHeader($header);
  $query->limit(25);
  $result = $query->execute();
  
  //$sql .= tablesort_sql($header);
  //$result = pager_query($sql, 50, 0, NULL, $keys);

  $rows = array();
  foreach ($result as $data) {
    $row = array(t('@path', array('@path' => check_plain($data->path))), t('@page_title', array('@page_title' => $data->page_title)), l(t('edit'), "admin/structure/simple-page-title/edit/" . $data->pt_id), l(t('delete'), "admin/structure/simple-page-title/delete/" . $data->pt_id));
    $rows[] = $row;
  }

  if (empty($rows)) {
    $empty_message = t('No Custom Page Titles available.');
    $rows[] = array(array('data' => $empty_message), '');
  }

  $html .= theme('table', array('header' => $header, 'rows' => $rows));
  $html .= theme('pager', array('tags' => NULL, 'element' => 0));

  return $html;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function simple_page_title_admin_edit_page($page_title_object = NULL) {
  $html = '';
  
  $form = drupal_get_form('simple_page_title_custom_title_form', $page_title_object);

  $html .= drupal_render($form);

  return $html;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function simple_page_title_custom_title_form($form, $form_state, $page_title_object = NULL) {
  $form = array();

  $mode = (empty($page_title_object)) ? 'submit' : 'update';
  $form['mode'] = array(
    '#type' => 'hidden',
    '#value' => $mode,
  );

  if ($mode == 'update') {
    $form['original_path'] = array(
      '#type' => 'hidden',
      '#value' => (isset($page_title_object->path)) ? $page_title_object->path : NULL,
    );
  }

  $form['description'] = array(
    '#type' => 'markup',
    '#value' => '<p style="margin-top: 10px;">' . t('You may use this form to create custom &lt;title&gt; tag content. If you wish to edit or delete an existing title please find it in the table below.') . '</p>',
  );

  $form['path'] = array(
    '#type' => 'textfield',
    '#length' => 30,
    '#maxlength' => 200,
    '#title' => 'Path',
    '#description' => t('Please enter the path of the page you would like to apply a new html title tag to e.g. node/5 or news/my-piece-of-news. Do not include the beginning "/" after the domain name. '),
    '#required' => TRUE,
    '#disabled' => FALSE,
    '#default_value' => (isset($page_title_object->path)) ? $page_title_object->path : NULL,
  );

  $form['page_title'] = array(
    '#type' => 'textfield',
    '#length' => 30,
    '#maxlength' => 255,
    '#title' => 'Page Title',
    '#description' => t('Please enter your custom page title.'),
    '#required' => TRUE,
    '#disabled' => FALSE,
    '#default_value' => (isset($page_title_object->page_title)) ? $page_title_object->page_title : NULL,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function simple_page_title_custom_title_form_validate($form, &$form_state) {

  module_load_include('inc', 'simple_page_title', 'simple_page_title.db');
  if ($form_state['values']['mode'] === 'submit') {
    $path_count = simple_page_title_get_page_title_for_path_count($form_state['values']['path'], $form_state['values']['page_title']);
    if ($path_count > 0) {
      form_set_error('path', t("This path already has a custom page title, please edit the existing entry"));
    }
  }

  if ($form_state['values']['mode'] === 'update') {
    if ($form_state['values']['path'] !== $form_state['values']['original_path']) {
      form_set_error('path', t('You have changed the path, if you wish to submit a new page title please use the add new page title form'));
    }
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function simple_page_title_custom_title_form_submit($form_id, $form_state) {

  module_load_include('inc', 'simple_page_title', 'simple_page_title.db');
  if ($form_state['values']['mode'] == 'submit') {
    //strip html tags from the fields just incase the user does anything stupid

    $clean_page_title = strip_tags($form_state['values']['page_title']);
    $title_submit = simple_page_title_add_page_title_to_db($form_state['values']['path'], $clean_page_title);

    if (!empty($title_submit)) {
      drupal_set_message(t("The custom page title for the path %path has been saved.", array('%path' => $form_state['values']['path'])));
    }
  }

  if ($form_state['values']['mode'] == 'update') {
    //strip html tags from the fields just incase the user does anything stupid

    $clean_page_title = strip_tags($form_state['values']['page_title']);
    $title_update = simple_page_title_update_page_title_for_path($form_state['values']['path'], $clean_page_title);

    if (!empty($title_update)) {
      drupal_set_message(t("The custom page title for the path %path has been updated.", array('%path' => $form_state['values']['path'])));
    }
  }

  drupal_goto('admin/structure/simple-page-title');

}

/**
 * Admin page for providing delete functionality of a single simple_page_title entry.
 */
function simple_page_title_admin_path_delete($page_title_object = NULL) {
  $html = '';
  
  $form = drupal_get_form('simple_page_title_path_delete_form', $page_title_object);
  
  $html .= drupal_render($form);
  
  return $html;
}

/**
 * Instance of hook _form. 
 * Admin form for providing delete functionality of a single simple_page_title entry.
 */
function simple_page_title_path_delete_form($form, $form_state, $page_title_object) {
  $form = array();
  $form['path'] = array(
    '#type' => 'value',
    '#value' => $page_title_object->path,
  );

  $output = confirm_form($form, 
      t('Are you sure you want to delete custom title for path %path?', array('%path' => $page_title_object->path)), 'admin/content/simple-page-title');
  return $output;
}

/**
 * Instance of hook _form_submit.
 * Function to delete the selected simple page title entry.
 */
function simple_page_title_path_delete_form_submit($form, $form_state) {
  module_load_include('inc', 'simple_page_title', 'simple_page_title.db');
  if ($form_state['values']['confirm']) {
    $delete_status = simple_page_title_delete_page_title_for_path($form_state['values']['path']);
    if ($delete_status) {
      drupal_set_message("The Page Title For Path " . $form_state['values']['path'] . " has been removed");
    }
  }
  drupal_goto("admin/structure/simple-page-title");
}

/**
 * A page that will display the module weight adjustment form if the page_title module is also installed.
 */
function simple_page_title_admin_settings_page() {
  module_load_include('inc', 'simple_page_title', 'simple_page_title.db');
  $html = '';

  drupal_set_title("Simple Page Title Module Settings");
  $html .= '<p>' . t('If you make use of the page_title module, you may have some instances where you wish the page_title module to 
        take precedence over this module. To do so, reduce the weight of this module to less than the page_title module. 
        E.g. If the page_title module is weight 0, make this module weight -1. If the module weight is set to less than -20 
        you will need to perform a database query') . '<p>';

  if (module_exists('page_title')) {
    $html .= '<p>' . t('The current weight of the page_title module is ') . '<strong>' . simple_page_title_get_module_weight('page_title') . '</strong></p>';
  
    $module_weight = simple_page_title_get_module_weight('simple_page_title');

    $form = drupal_get_form('simple_page_title_set_module_weight_form', $module_weight);

    $html .= drupal_render($form);
  }
  else {
    $html .= '<p>' . t('The page_title module is ') . '<strong>' . t('not currently installed') . '</strong></p>';
  }

  return $html;
}

/**
 * A form to adjust the weight of this module
 */
function simple_page_title_set_module_weight_form($form, $form_state, $module_weight) {
  $form = array();

  $i = -20;
  while ($i < 21) {
    $options_array[$i] = $i;
    $i++;
  }
  //$option_array 


  $form['module_weight'] = array(
    '#type' => 'select',
    '#title' => t('Current Weight of Simple Page Title Module'),
    '#description' => t('Please adjust this as desired'),
    '#default_value' => $module_weight,
    '#options' => $options_array,
    '#disabled' => FALSE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Update Module Weight',
  );

  return $form;
}

/**
 * The submit hook for the weight adjustment form
 */
function simple_page_title_set_module_weight_form_submit($form_id, $form_state) {
  module_load_include('inc', 'simple_page_title', 'simple_page_title.db');
  $update_success = simple_page_title_update_module_weight('simple_page_title', $form_state['values']['module_weight']);
  if ($update_success) {
    drupal_set_message(t('Simple Page Title Module weight has been updated'));
  }
  drupal_goto('admin/structure/simple-page-title');
}

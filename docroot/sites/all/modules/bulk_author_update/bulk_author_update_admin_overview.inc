<?php
/**
 * @file
 * This files renders bulk author update form.
 */

/**
 * Displays the bulk author update dashboard.
 */
function bulk_author_update_admin_overview() {
  $module_path = drupal_get_path('module', 'bulk_author_update');
  drupal_add_js($module_path . '/js/' . 'bulk_author_update.js');
  drupal_add_css($module_path . '/css/' . 'bulk_author_update.css');
  $bau_vars = array();
  $content_type = array();
  $ctypes = node_type_get_types();
  if (count($ctypes) > 0) {
    foreach ($ctypes as $ct) {
      $value = 'bulk_author_update_' . $ct->type;
      $bau_vars[$value] = variable_get($value);
      $selected_content_type = variable_get($value);
      if ($selected_content_type) {
        $content_type[] = $ct->type;
      }
    }
  }
  global $base_url;
  $bau_vars['request_url'] = $base_url . "/bulkauthorupdate/user";
  $bau_vars['redirect_url'] = $base_url . "/admin/content/bulkauthorupdate";
  $bau_vars['ajax_image'] = $module_path . "/images/ajax-loader.gif";
  $bau_vars['error_message'] = t("Something went wrong.Please try again.");
  $bau_vars['validation_message'] = t("Please select the checkboxes.");
  $bau_vars['user_message'] = t("Please enter user name.");
  $bau_vars['user_not_found'] = t("User is not found. Please enter more relevant keywords.");
  drupal_add_js(array('vars' => $bau_vars), 'setting');

  if (count($content_type) == 0) {
    foreach ($ctypes as $ct) {
      $content_type[] = $ct->type;
    }
  }
  // print"<pre>";print_r($content_type);exit;
  $output = '';
  $header = array(
    array('data' => '<input type="checkbox" class="parent_check" id="parent_check" onclick="validate_data(this)" />'),
    array('data' => t('Node ID')),
    array('data' => t('Title')),
    array('data' => t('Author')),
  );
  $query = db_select("node", "n");
  $query->leftJoin('users', 'u', 'n.uid = u.uid');
  $query->fields('n', array('nid', 'title', 'uid'));
  $query->fields('u', array('name'));
  $query->condition('n.type', $content_type, 'IN');
  $query = $query->extend('TableSort')->extend('PagerDefault')->limit(variable_get('bulk_author_update_display_records', 50));
  $query->orderByHeader($header);
  $result = $query->execute();
  $rows = array();
  while ($record = $result->fetchObject()) {
    $nid = $record->nid;
    $author = $record->name;
    $node_title = $record->title;
    $link_title = l($node_title, "node/$nid");
    $rows[] = array(
      "<input type='checkbox' value='$nid' class='child_check' onclick='validate_individual(this)' />",
      $nid,
      $link_title,
      $author,
    );
  }

  $output .= theme('table',
    array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array(),
      'sticky' => TRUE,
      'caption' => '',
      'colgroups' => array(),
      'empty' => t("No Records Found."),
    )
  ) . theme('pager');

  $build['content'] = array(
    'form' => drupal_get_form('bulk_author_update_info_form'),
    'output' => array('#markup' => $output),
  );
  return $build;
}

/**
 * Bulk author update information form.
 */
function bulk_author_update_info_form() {
  $form = array();
  $form['suggest_user'] = array(
    '#type' => 'textfield',
    '#title' => 'username',
    '#required' => TRUE,
    '#attributes' => array('onkeyup' => array('bulk_author_update_suggest_user(this)')),
  );
  $form['submit'] = array(
    '#type' => 'item',
    '#markup' => "<input value='Apply Bulk Author Update' class='form-submit' type='button' onclick='return bulk_author_update()'>",
  );
  $form['auto_list'] = array(
    '#type' => 'item',
    '#markup' => "<ul id='auto_list'></ul>",
    '#prefix' => "<div id='auto_list_container'>",
    '#suffix' => '</div>',
  );
  $form['bulk_update'] = array(
    '#type' => 'item',
    '#markup' => "<div id='update_all' class='col10'>" . l(t("Update all the content (Selected content types)"), "bulkauthorupdate/all") . "</div>",
  );
  $form['details_container'] = array(
    '#type' => 'item',
    '#markup' => "<div id='details_container'></div>",
  );
  return $form;
}

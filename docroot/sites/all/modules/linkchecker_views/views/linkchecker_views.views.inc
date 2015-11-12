<?php

/**
 * @file
 * Views callbacks for the linkchecker module.
 */

/**
 * Implements hook_views_pre_execute()
 */
function linkchecker_views_views_pre_execute(&$view)  {
  // Note: UNION is not supported in views UI.

  //drupal_set_message('<pre>' . print_r($view, TRUE) . '</pre>');

  // Extend query for general broken links reporting.
  if ($view->name == 'linkchecker_views_reports' && $view->current_display == 'linkchecker_admin_report_page')  {
    // Search for broken links in nodes and comments and blocks of all users.
    // @todo Try to make UNION'ed subselect resultset smaller.
    $subquery4 = db_select('linkchecker_node', 'ln')
    ->distinct()
    ->fields('ln', array('lid'));

    $subquery3 = db_select('linkchecker_comment', 'lc')
    ->distinct()
    ->fields('lc', array('lid'));

    $subquery2 = db_select('linkchecker_block_custom', 'lb')
    ->distinct()
    ->fields('lb', array('lid'));

    // UNION all linkchecker type tables.
    $subquery1 = db_select($subquery2->union($subquery3)->union($subquery4), 'q1')->fields('q1', array('lid'));

    // Extend the views query.
    $query = $view->build_info['query'];
    $query->innerJoin($subquery1, 'q2', 'q2.lid = linkchecker_link.lid');

    $view->build_info['count_query'] = $query->countQuery();
  }
  elseif ($view->name == 'linkchecker_views_reports' && $view->current_display == 'linkchecker_views_user_report_page') {
    global $user;

    $ignore_response_codes = preg_split('/(\r\n?|\n)/', variable_get('linkchecker_ignore_response_codes', "200\n206\n302\n304\n401\n403"));

    // Build query for broken links in nodes of the current user.
    $subquery2 = db_select('node', 'n');
    $subquery2->innerJoin('node_revision', 'r', 'r.vid = n.vid');
    $subquery2->innerJoin('linkchecker_node', 'ln', 'ln.nid = n.nid');
    $subquery2->innerJoin('linkchecker_link', 'll', 'll.lid = ln.lid');
    $subquery2->condition('ll.last_checked', 0, '<>');
    $subquery2->condition('ll.status', 1);
    $subquery2->condition('ll.code', $ignore_response_codes, 'NOT IN');
    $subquery2->condition(db_or()
      ->condition('n.uid', $user->uid)
      ->condition('r.uid', $user->uid)
    );
    $subquery2->distinct();
    $subquery2->fields('ll', array('lid'));

    if (variable_get('linkchecker_scan_comments', 0)) {
      // Build query for broken links in nodes and comments of the current user.
      $subquery3 = db_select('comment', 'c');
      $subquery3->innerJoin('linkchecker_comment', 'lc', 'lc.cid = c.cid');
      $subquery3->innerJoin('linkchecker_link', 'll', 'll.lid = lc.lid');
      $subquery3->condition('ll.last_checked', 0, '<>');
      $subquery3->condition('ll.status', 1);
      $subquery3->condition('ll.code', $ignore_response_codes, 'NOT IN');
      $subquery3->condition('c.uid', $user->uid);
      $subquery3->distinct();
      $subquery3->fields('ll', array('lid'));

      // UNION the linkchecker_node and linkchecker_comment tables.
      $subquery1 = db_select($subquery2->union($subquery3), 'q1')->fields('q1', array('lid'));
    }
    else {
      // Build query for broken links in nodes of the current user.
      $subquery1 = db_select($subquery2, 'q1')->fields('q1', array('lid'));
    }

    // Extend the views query.
    $query = $view->build_info['query'];
    $query->innerJoin($subquery1, 'q2', 'q2.lid = linkchecker_link.lid');

    $view->build_info['count_query'] = $query->countQuery();
  }
}

/**
 * Implements hook_views_data().
 */
function linkchecker_views_views_data() {
  $data['linkchecker_link'] = array(
    'table' => array(
      'group' => t('Broken links'),
    ),
    // Describe table fields to views.
    'lid' => array(
      'title' => t('Link ID'),
      'help' => t("The unique linkchecker link ID."),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'url' => array(
      'title' => t('URL'),
      'help' => t('The URL of a link in a broken links report.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    // HTTP status code.
    'code' => array(
      'title' => t('HTTP status code'),
      'help' => t('The status code returned by HTTP requests to the link.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'linkchecker_views_handler_filter_in_code',
      ),
    ),
    // Error message.
    'error' => array(
      'title' => t('Error message'),
      'help' => t('The error message received from the remote server while doing link checking.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
    ),
    // Fail count.
    'fail_count' => array(
      'title' => t('Fail count'),
      'help' => t('The number of times a request for the URL has failed.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    // Last checked unix timestamp.
    'last_checked' => array(
      'title' => t('Last checked'),
      'help' => t('Unix timestamp of the last link check.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
    ),
    // Status (TRUE|FALSE if the link check is enabled|disabled).
    'status' => array(
      'title' => t('Status'),
      'help' => t('Whether or not the link should be checked for being broken.'),
      'field' => array(
        'handler' => 'views_handler_field_boolean',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_boolean_operator',
        'label' => t('Check link status'),
        'type' => 'enabled-disabled',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'operations' => array(
      'title' => t('Operations'),
      'help' => t('List of links to configure the link and edit contents using the link'),
      'field' => array(
        'handler' => 'views_handler_field_linkchecker_views_link_operations',
        'no group by' => TRUE,
      ),
    ),
  );

  return $data;
}

function _linkchecker_views_is_internal_url_bis(&$link) {
  global $base_url;

  if (strpos($link->url, $base_url) === 0) {
    $link->internal = trim(substr($link->url, strlen($base_url)), " \t\r\n\0\\/");
    return TRUE;
  }
}

function _linkchecker_views_operations($lid, $url, $account = NULL) {
  $access_edit_link_settings = user_access('edit link settings');
  $access_administer_blocks = user_access('administer blocks');
  $access_administer_redirects = user_access('administer redirects');

  $link = new stdClass();
  $link->lid = $lid;
  $link->url = $url;

  $nids = _linkchecker_link_node_ids($link, $account);
  $cids = _linkchecker_link_comment_ids($link, $account);
  $bids = _linkchecker_link_block_ids($link);

  $links = array();

  // Show links to link settings.
  if ($access_edit_link_settings) {
    $links[] = array('make_link' => true, 'name' => t('Edit link settings'), 'path' => url('linkchecker/' . $link->lid . '/edit', array('query' => drupal_get_destination())));
  }

  // Show link to nodes having this broken link.
  foreach ($nids as $nid) {
    $links[] = array('make_link' => true, 'name' => t('Edit node @node', array('@node' => $nid)), 'path' => url('node/' . $nid . '/edit', array('query' => drupal_get_destination())));
  }

  // Show link to comments having this broken link.
  if (module_exists('comment') && variable_get('linkchecker_scan_comments', 0)) {
    foreach ($cids as $cid) {
      $links[] = array('make_link' => true, 'name' => t('Edit comment @comment', array('@comment' => $cid)), 'path' => url('comment/' . $cid . '/edit', array('query' => drupal_get_destination())));
    }
  }

  // Show link to blocks having this broken link.
  if ($access_administer_blocks) {
    foreach ($bids as $bid) {
      $links[] = array('make_link' => true, 'name' => t('Edit block @block', array('@block' => $bid)), 'path' => url('admin/structure/block/manage/block/' . $bid . '/configure', array('query' => drupal_get_destination())));
    }
  }

  // Show link to redirect this broken internal link.
  if (module_exists('redirect') && $access_administer_redirects && _linkchecker_views_is_internal_url_bis($link)) {
    $links[] = array('make_link' => true, 'name' => t('Create redirection'), 'path' => url('admin/config/search/redirect/add', array('query' => array('source' => $link->internal, drupal_get_destination()))));
  }

  return $links;
}

class views_handler_field_linkchecker_views_link_operations extends views_handler_field_prerender_list {

  function construct() {
    parent::construct(); 
    $this->additional_fields['lid'] = array('table' => 'linkchecker_link', 'field' => 'lid');;
    $this->additional_fields['url'] = array('table' => 'linkchecker_link', 'field' => 'url');;
  }

  function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['lid'];
  }

  function pre_render(&$values) {
    $lids = array();
    $this->items = array();

    foreach ($values as $result) {
      $lids[] = $this->get_value($result, null, TRUE);
    }
 
    $lid_alias = $this->aliases['lid'];
    $url_alias = $this->aliases['url'];
    foreach ($values as $id => $result) {
      $lid = $result->{$lid_alias};
      $url = $result->{$url_alias};
      $this->items[$lid] = _linkchecker_views_operations($lid, $url, null);
    }
  }

  function render_item($count, $item) {
    return $item['name'];
  }
}

/**
 * Implements hook_views_default_views().
 */
function linkchecker_views_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'linkchecker_views_reports';
  $view->description = 'Display a list of broken links.';
  $view->tag = 'default';
  $view->base_table = 'linkchecker_link';
  $view->human_name = 'Broken links';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = TRUE;

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Broken links';
  $handler->display->display_options['items_per_page'] = 0;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access broken links report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'url' => 'url',
    'code' => 'code',
    'error' => 'error',
    'lid' => 'lid',
    'operations' => 'operations',
  );
  $handler->display->display_options['style_options']['default'] = 'url';
  $handler->display->display_options['style_options']['info'] = array(
    'url' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'code' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'error' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'lid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No broken links have been found.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Broken links: Link ID */
  $handler->display->display_options['fields']['lid']['id'] = 'lid';
  $handler->display->display_options['fields']['lid']['table'] = 'linkchecker_link';
  $handler->display->display_options['fields']['lid']['field'] = 'lid';
  $handler->display->display_options['fields']['lid']['label'] = '';
  $handler->display->display_options['fields']['lid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['lid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['lid']['alter']['text'] = 'Edit link settings';
  $handler->display->display_options['fields']['lid']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['lid']['alter']['path'] = 'linkchecker/[lid]/edit?destination=admin/reports/linkchecker';
  $handler->display->display_options['fields']['lid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['lid']['separator'] = '';
  /* Field: Broken links: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'linkchecker_link';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['url']['alter']['path'] = '[url]';
  $handler->display->display_options['fields']['url']['alter']['max_length'] = '40';
  $handler->display->display_options['fields']['url']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['url']['alter']['trim'] = TRUE;
  /* Field: Broken links: HTTP status code */
  $handler->display->display_options['fields']['code']['id'] = 'code';
  $handler->display->display_options['fields']['code']['table'] = 'linkchecker_link';
  $handler->display->display_options['fields']['code']['field'] = 'code';
  $handler->display->display_options['fields']['code']['label'] = 'Response';
  $handler->display->display_options['fields']['code']['separator'] = '';
  /* Field: Broken links: Error message */
  $handler->display->display_options['fields']['error']['id'] = 'error';
  $handler->display->display_options['fields']['error']['table'] = 'linkchecker_link';
  $handler->display->display_options['fields']['error']['field'] = 'error';
  $handler->display->display_options['fields']['error']['label'] = 'Error';
  /* Field: Broken links: Operations */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'linkchecker_link';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['type'] = 'ul';
  $handler->display->display_options['fields']['operations']['label'] = 'Operations';
  /* Filter criterion: Broken links: HTTP status code */
  $handler->display->display_options['filters']['code']['id'] = 'code';
  $handler->display->display_options['filters']['code']['table'] = 'linkchecker_link';
  $handler->display->display_options['filters']['code']['field'] = 'code';
  $handler->display->display_options['filters']['code']['operator'] = 'not in';
  //$handler->display->display_options['filters']['code']['value'] = preg_split('/(\r\n?|\n)/', variable_get('linkchecker_ignore_response_codes', "200\n206\n302\n304\n401\n403"));
  $handler->display->display_options['filters']['code']['value'] = array();
  $handler->display->display_options['filters']['code']['group'] = 1;
  /* Filter criterion: Broken links: Last checked */
  $handler->display->display_options['filters']['last_checked']['id'] = 'last_checked';
  $handler->display->display_options['filters']['last_checked']['table'] = 'linkchecker_link';
  $handler->display->display_options['filters']['last_checked']['field'] = 'last_checked';
  $handler->display->display_options['filters']['last_checked']['operator'] = '!=';
  $handler->display->display_options['filters']['last_checked']['value']['value'] = '0';
  $handler->display->display_options['filters']['last_checked']['group'] = 1;
  /* Filter criterion: Broken links: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'linkchecker_link';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;

  /* Display: Admin Broken links */
  $handler = $view->new_display('page', 'Admin Broken links', 'linkchecker_views_admin_report_page');
  $handler->display->display_options['display_description'] = 'Shows a list of broken links in content.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/reports/linkchecker';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Broken links';
  $handler->display->display_options['menu']['description'] = 'Shows a list of broken links in content.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: User Broken links */
  $handler = $view->new_display('page', 'User Broken links', 'linkchecker_user_report_page');
  $handler->display->display_options['display_description'] = 'Shows a list of broken links in authors content.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'user/%/linkchecker';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Broken links';
  $handler->display->display_options['menu']['description'] = 'Shows a list of broken links in authors content.';
  $handler->display->display_options['menu']['weight'] = '3';
  $handler->display->display_options['menu']['name'] = 'user-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $translatables['linkchecker_reports'] = array(
    t('Defaults'),
    t('Broken links'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('No broken links have been found.'),
    t('Edit link settings'),
    t('.'),
    t('URL'),
    t('Response'),
    t('Error'),
    t('Operations'),
    t('<ul><li>[nid]</li><li>[lid]</li></ul>'),
    t('Admin Broken links'),
    t('Shows a list of broken links in content.'),
    t('User Broken links'),
    t('Shows a list of broken links in authors content.'),
  );

  $views[$view->name] = $view;
  return $views;
}

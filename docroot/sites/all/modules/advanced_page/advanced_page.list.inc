<?php
/**
 * @file
 * Advacne page administration listing.
 */

/**
 * Implements menu callback.
 */
function page_manager_list() {
  return _node_table();
}

/**
 * Function for displaying table.
 */
function _node_table() {
  // Table header.
  $header = array(
    array('data' => t('Title'), 'field' => 'title', 'sort' => 'asc'),
    array('data' => t('Node ID'), 'field' => 'nid'),
    array('data' => t('Type'), 'field' => 'type'),
    array('data' => t('Created'), 'field' => 'created'),
    array('data' => t('Published')),
    array('data' => t('Action')),
  );

  // Create the SQL query to fetch node data for advanced_page.
  $query = db_select('node', 'n')
    ->condition('type', 'advanced_page', '=');

  // Enable pager and show 50 per page.
  $query->extend('PagerDefault')->limit(50);

  // Enable Table sorting.
  $query->extend('TableSort');

  // Field to sort on is picked from $header.
  $query->orderByHeader($header);

  // Add up fields from the node table to be fetched.
  $query->fields('n', array(
      'nid',
      'title',
      'type',
      'created',
      'status',
      ));

  $results = $query->execute();

  $rows = array();
  foreach ($results as $node) {
    $rows[] = array(
      'data' => array(
        l($node->title, 'node/' . $node->nid . '/edit'),
        $node->nid,
        $node->type,
        format_date($node->created),
        $node->status ? "Yes" : "No",
        l(t("edit"), 'node/' . $node->nid . '/edit'),
      ),
    );
  }

  // Theme the html table.
  $variables = array();
  $variables['header'] = $header;
  $variables['rows'] = $rows;
  $variables['caption'] = t('List of static pages');
  $variables['sticky'] = TRUE;
  $variables['empty'] = t('No nodes created...');
  $html = theme('table', $variables);

  // Append pager.
  $html .= theme('pager', array(
    'tags' => array(),
    )
  );

  return ($html);
}

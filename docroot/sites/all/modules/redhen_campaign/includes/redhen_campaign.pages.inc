<?php

/**
 * @file
 * Page callbacks for redhen campaigns.
 */

module_load_include('inc', 'redhen', 'includes/redhen.forms');

/**
 * Page callback for campaign overview page.
 */
function redhen_campaign_campaign_admin() {

  // Setup the header for both the query and table.
  $header = array(
    'type' => array(
      'field' => 'type',
      'type' => 'property',
      'data' => 'Type',
      'sort' => 'asc',
      'specifier' => 'type',
    ),
    'label' => array(
      'field' => 'label',
      'type' => 'property',
      'data' => 'Name',
      'sort' => 'asc',
      'specifier' => 'label',
    ),
    'updated' => array(
      'field' => 'updated',
      'type' => 'property',
      'data' => 'Updated',
      'sort' => 'desc',
      'specifier' => 'updated',
    ),
    'operations' => array(
      'data' => t('Operations'),
    ),
  );

  // Need to ensure the query doesn't execute when posing the form.
  $result = FALSE;
  if (!isset($_POST['form_id'])) {
    $bundle = isset($_GET['bundle']) ? $_GET['bundle'] : '';
    $properties = isset($_GET['properties']) ? $_GET['properties'] : array();
    $fields = isset($_GET['fields']) ? $_GET['fields'] : array();
    $result = redhen_filter_query('redhen_campaign', $header, $bundle, $properties, $fields);
  }

  $campaigns = array();
  $rows = array();
  if ($result) {
    $campaigns = redhen_campaign_load_multiple(array_keys($result['redhen_campaign']));

    if (!empty($campaigns)) {
      $destination = drupal_get_destination();
      foreach ($campaigns as $campaign) {
        $uri = entity_uri('redhen_campaign', $campaign);
        $redhen_campaign_type = redhen_campaign_type_load($campaign->type);

        $data = array(
          'type' => check_plain($redhen_campaign_type->label),
          'label' => array(
            'data' => array(
              '#type' => 'link',
              '#title' => $campaign->label,
              '#href' => $uri['path'],
            ),
          ),
          'updated' => redhen_format_date($campaign->updated, 'short'),
        );

        $row_classes = array();
        $row_classes[] = 'active';

        $rows[$campaign->campaign_id] = array(
          'data' => $data,
          'class' => $row_classes,
        );

        // Build a list of all the accessible operations for the campaign.
        $ops = array();
        if (redhen_campaign_access('update', $campaign)) {
          $ops['edit'] = array(
            'title' => t('edit'),
            'href' => $uri['path'] . '/edit',
            'query' => $destination,
          );
        }
        if (redhen_campaign_access('delete', $campaign)) {
          $ops['delete'] = array(
            'title' => t('delete'),
            'href' => $uri['path'] . '/delete',
            'query' => $destination,
          );
        }
        if (count($ops) > 1) {
          // Render an unordered list of operations links.
          $rows[$campaign->campaign_id]['data']['operations'] = array(
            'data' => array(
              '#theme' => 'links__node_operations',
              '#links' => $ops,
              '#attributes' => array('class' => array('links', 'inline')),
            ),
          );
        }
        elseif (!empty($ops)) {
          // Render the first and only operation as a link.
          $link = reset($ops);
          $rows[$campaign->campaign_id]['data']['operations'] = array(
            'data' => array(
              '#type' => 'link',
              '#title' => $link['title'],
              '#href' => $link['href'],
              '#options' => array('query' => $link['query']),
            ),
          );
        }
        else {
          unset($header['operations']);
        }
      }
    }
  }

  return array(
    'form' => drupal_get_form('redhen_filter_form', 'redhen_campaign'),
    'campaigns' => array(
      '#theme' => 'redhen_campaign_list',
      '#campaigns' => $campaigns,
      '#header' => $header,
      '#rows' => $rows,
    ),
  );
}

/**
 * Page callback for campaign overview page.
 */
function redhen_campaign_campaign_pages_admin() {

  // Setup the header for both the query and table.
  $header = array(
    'type' => array(
      'field' => 'type',
      'type' => 'property',
      'data' => 'Type',
      'sort' => 'asc',
      'specifier' => 'type',
    ),
    'label' => array(
      'field' => 'label',
      'type' => 'property',
      'data' => 'Name',
      'sort' => 'asc',
      'specifier' => 'label',
    ),
    'updated' => array(
      'field' => 'updated',
      'type' => 'property',
      'data' => 'Updated',
      'sort' => 'desc',
      'specifier' => 'updated',
    ),
    'operations' => array(
      'data' => t('Operations'),
    ),
  );

  // Need to ensure the query doesn't execute when posing the form.
  $result = FALSE;
  if (!isset($_POST['form_id'])) {
    $bundle = isset($_GET['bundle']) ? $_GET['bundle'] : '';
    $properties = isset($_GET['properties']) ? $_GET['properties'] : array();
    $fields = isset($_GET['fields']) ? $_GET['fields'] : array();
    $result = redhen_filter_query('redhen_campaign_page', $header, $bundle, $properties, $fields);
  }

  $campaign_pages = array();
  $rows = array();
  if ($result) {
    $campaign_pages = redhen_campaign_page_load_multiple(array_keys($result['redhen_campaign_page']));

    if (!empty($campaign_pages)) {
      $destination = drupal_get_destination();
      foreach ($campaign_pages as $page) {
        $uri = entity_uri('redhen_campaign_page', $page);
        $redhen_campaign_page_type = redhen_campaign_page_type_load($page->type);

        $data = array(
          'type' => check_plain($redhen_campaign_page_type->label),
          'label' => array(
            'data' => array(
              '#type' => 'link',
              '#title' => $page->label,
              '#href' => $uri['path'],
            ),
          ),
          'updated' => redhen_format_date($page->updated, 'short'),
        );

        $row_classes = array();
        $row_classes[] = 'active';

        $rows[$page->page_id] = array(
          'data' => $data,
          'class' => $row_classes,
        );

        // Build a list of all the accessible operations for the campaign.
        $ops = array();
        if (redhen_campaign_access('update', $page)) {
          $ops['edit'] = array(
            'title' => t('edit'),
            'href' => $uri['path'] . '/edit',
            'query' => $destination,
          );
        }
        if (redhen_campaign_access('delete', $page)) {
          $ops['delete'] = array(
            'title' => t('delete'),
            'href' => $uri['path'] . '/delete',
            'query' => $destination,
          );
        }
        if (count($ops) > 1) {
          // Render an unordered list of operations links.
          $rows[$page->page_id]['data']['operations'] = array(
            'data' => array(
              '#theme' => 'links__node_operations',
              '#links' => $ops,
              '#attributes' => array('class' => array('links', 'inline')),
            ),
          );
        }
        elseif (!empty($ops)) {
          // Render the first and only operation as a link.
          $link = reset($ops);
          $rows[$page->page_id]['data']['operations'] = array(
            'data' => array(
              '#type' => 'link',
              '#title' => $link['title'],
              '#href' => $link['href'],
              '#options' => array('query' => $link['query']),
            ),
          );
        }
        else {
          unset($header['operations']);
        }
      }
    }
  }

  return array(
    'form' => drupal_get_form('redhen_filter_form', 'redhen_campaign_page'),
    'campaigns' => array(
      '#theme' => 'redhen_campaign_page_list',
      '#pages' => $campaign_pages,
      '#header' => $header,
      '#rows' => $rows,
    ),
  );
}

/**
 * Page callback for adding a campaign.
 *
 * @param string $type
 *   The type of campaign to return a form for.
 */
function redhen_campaign_types_list_page($type = 'default') {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);

  // Bypass the redhen_campaign/add listing if only one type is available.
  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }

  $render = array(
    '#theme' => 'redhen_campaign_add_list',
    '#content' => $content,
  );

  return $render;
}

/**
 * Page callback for adding a campaign.
 */
function redhen_campaign_add_page($type) {
  $campaign = entity_get_controller('redhen_campaign')->create(array('type' => $type));
  return drupal_get_form('redhen_campaign_campaign_form', $campaign);
}

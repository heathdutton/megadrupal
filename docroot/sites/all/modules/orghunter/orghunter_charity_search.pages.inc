<?php

/**
 * Charity Search results page.
 */
function orghunter_charity_search_results_page() {
  $values = array_merge(array(
    'search_term' => '',
    'ein' => '',
    'state' => '',
    'city' => '',
    'zip_code' => '',
    'category' => '',
    'eligible' => '',
  ), $_GET);
  if (
    !$values['search_term'] &&
    !$values['ein'] &&
    !$values['state'] &&
    !$values['city'] &&
    !$values['zip_code'] &&
    !$values['category'] &&
    !$values['eligible']
  ) {
    drupal_goto('orghunter/charitysearch');
  }

  $page = isset($values['page']) && is_numeric($values['page']) ? $values['page'] : 0;
  $values['category'] = $values['category'] == '?' ? '' : $values['category'];
  $rows =
  $values['rows'] = variable_get('orghunter_charity_search_rows', ORGHUNTER_CHARITY_SEARCH_API_ROWS_DEFAULT);
  $start =
  $values['start'] = $page * $rows;

  $results = orghunter_charity_search_results($values);
  $items = array();
  $total = 0;

  if (isset($results[0])) {
    $total = $results[0]['recordCount'];
  }

  pager_default_initialize($total, $rows);

  if (empty($values['search_term'])) {
    drupal_set_title(format_plural($total,
      '1 Charity found',
      '@count charities found'
    ));
  }
  else {
    drupal_set_title(format_plural($total,
      '1 Charity found for “@search_term”',
      '@count charities found for “!search_term”',
      array(
        '!search_term' => filter_xss($values['search_term']),
      ))
    );
  }

  if (empty($results)) {
    module_load_include('inc', 'orghunter_charity_search', 'orghunter_charity_search.forms');

    $return['no_results']['message'] = array(
      '#markup' => '<p>' . t('No charities could be found with your criteria. Try again with different search parameters.') . '</p>',
    );
    $return['no_results']['form'] = drupal_get_form('orghunter_charity_search_form');
  }
  else {
    foreach ($results as $result) {
      $items[] = theme('orghunter_charity_search_result_charity', array(
        'ein' => $result['ein'],
        'name' => $result['charityName'],
        'title' => l($result['charityName'], $result['url'], array('attributes' => array(
          'title' => t('@charity at OrgHunter', array('@charity' => $result['charityName']))
        ))),
        'url' => $result['url'],
        'category' => $result['category'],
        'city' => $result['city'],
        'state' => $result['state'],
        'zip' => $result['zipCode'],
        'deductibility' => $result['deductibilityCd'],
        'eligible' => $result['eligibleCd'],
        'status' => $result['statusCd'],
        'donation' => $result['donationUrl'],
      ));
    }

    $path = drupal_get_path('module', 'orghunter_charity_search');
    $return['results'] = array(
      '#theme' => 'orghunter_charity_search_results',
      '#results' => array(
        '#theme' => 'item_list',
        '#attributes' => array(
          'id' => 'orghunter-charity-search-results',
          'class' => array('orghunter-charity-search-results'),
        ),
        '#items' => $items,
        '#attached' => array(
          'css' => array(
            $path . '/css/orghunter_charity_search.result_charity.css',
          ),
        ),
      ),
      '#pager' => array(
        '#theme' => 'pager',
      )
    );
  }

  return $return;
}

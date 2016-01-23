<?php
/**
 * @file
 * Callback functions for report pages for the Yandex.Webmaster module.
 */

/**
 * Display summary statistics for the Yandex.Webmaster module.
 */
function yandex_webmaster_reports_form($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'yandex_webmaster') . '/css/yandex_webmaster.css');
  $form = array();
  // Get host list.
  $hostlist = yandex_webmaster_get_hosts();
  // Get selected host id.
  $form['branding'] = yandex_webmaster_branding_item();
  $form['selector'] = yandex_webmaster_selector_item();

  // Get statistics.
  $header = array(
    'timestamp' => array('data' => t('Date'), 'field' => 'ywr.timestamp'),
    'tcy' => array('data' => t('TCY'), 'field' => 'ywr.tcy'),
    'url_count' => array('data' => t('URL count'), 'field' => 'ywr.url_count'),
    'index_count' => array('data' => t('Indexed pages'), 'field' => 'ywr.index_count'),
    'links_count' => array('data' => t('External links'), 'field' => 'ywr.links_count'),
    'url_errors' => array('data' => t('URL errors'), 'field' => 'ywr.url_errors'),
  );
  $stats = db_select('yandex_webmaster_reports', 'ywr')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('ywr')
    ->condition('ywr.host_id', variable_get('yandex_webmaster_basic_host_id', 0))
    ->limit(100)
    ->orderByHeader($header)
    ->execute();

  $is_chart = module_exists('chart');
  if ($is_chart) {
    $chart = array(
      '#chart_id' => 'statistics_chart',
      '#title' => t('Statistics'),
      '#type' => CHART_TYPE_LINE,
      '#size' => chart_size(960, 250),
      '#grid_lines' => chart_grid_lines(25, 9.5, 1, 3),
      '#adjust_resolution' => TRUE,
      '#data_colors' => array(
        'FF8000',
        '803000',
        '0090C0',
        '30A020',
        'FF0000',
      ),
      '#legends' => array(
        t('TCY'),
        t('URL count'),
        t('Indexed pages'),
        t('External links'),
        t('URL errors'),
      ),
      '#line_styles' => array(2, 2, 2, 2, 2),
    );
  }
  $options = array();
  foreach ($stats as $aid => $stat) {
    $options[$aid] = array(
      'timestamp' => format_date($stat->timestamp),
      'tcy' => number_format($stat->tcy, 0, '', ' '),
      'url_count' => number_format($stat->url_count, 0, '', ' '),
      'url_errors' => number_format($stat->url_errors, 0, '', ' '),
      'index_count' => number_format($stat->index_count, 0, '', ' '),
      'links_count' => number_format($stat->links_count, 0, '', ' '),
    );
    if ($is_chart) {
      $chart['#labels'][] = date('d.m.Y', $stat->timestamp);
      $chart['#data']['tcy'][] = $stat->tcy;
      $chart['#data']['url_count'][] = $stat->url_count;
      $chart['#data']['index_count'][] = $stat->index_count;
      $chart['#data']['links_count'][] = $stat->links_count;
      $chart['#data']['url_errors'][] = $stat->url_errors;
    }
  }
  $form['stat'] = array(
    '#theme' => 'table',
    '#type' => 'tableselect',
    '#header' => $header,
    '#rows' => $options,
    '#options' => $options,
    '#empty' => t('No statistics available.'),
  );

  if ($is_chart) {
    $form['chart'] = array(
      '#markup' => theme('chart', array('chart' => $chart)),
    );
  }

  return $form;
}

/**
 * Callback function for yandex_webmaster_report_form().
 */
function yandex_webmaster_reports_form_submit($form, &$form_state) {
  variable_set('yandex_webmaster_basic_host_id', $form_state['values']['selector']['options']);
  drupal_goto('admin/reports/yandex_webmaster/' . $form_state['values']['selector']['options']);
}

/**
 * Display top clicks and shows report for the Yandex.Webmaster module.
 */
function yandex_webmaster_reports_top_form($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'yandex_webmaster') . '/css/yandex_webmaster.css');
  $form = array();
  // Get host list.
  $hostlist = yandex_webmaster_get_hosts();
  // Get selected host id.
  $id = isset($hostlist[variable_get('yandex_webmaster_basic_host_id', 0)]) ? variable_get('yandex_webmaster_basic_host_id', 0) : 0;
  // Get statistics.
  $stats = yandex_webmaster_get_top_clicks($id);
  if (isset($stats['error'])) {
    drupal_set_message(t('An error occured while getting clicks statistics. @error !link.', array('@error' => filter_xss($stats['error']), '!link' => l(t('Yandex.Webmaster settings page'), 'admin/config/services/yandex_webmaster'))), 'error');
    return $form;
  }
  $form['branding'] = yandex_webmaster_branding_item();
  $form['selector'] = yandex_webmaster_selector_item();

  // Empty statistics.
  if ($stats['total-clicks-count'] == 0) {
    $form['summary'] = array(
      '#type' => 'item',
      '#markup' => t('Summary click count for last seven days') . ': '
      . $stats['total-clicks-count'] . '<br />'
      . t('Summary shows count for last seven days') . ': '
      . $stats['total-shows-count'],
    );
    return $form;
  }

  $form['procent'] = array(
    '#type' => 'item',
    '#markup' => t('Percentage of shows count to the number of clicks') . ': ' . $stats['shows-clicks-procent'] . '%',
  );

  // Display top clicks list.
  $form['clicks_summary'] = array(
    '#type' => 'item',
    '#markup' => t('Click count for last seven days') . ': '
      . $stats['total-clicks-count'] . '<br />'
      . t('Procent of popular query clicks') . ': '
      . $stats['top-clicks-percent'] . '%',
  );
  $form['clicks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Top clicks for last seven days'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $clicks = array();
  foreach ($stats['clicks'] as $info) {
    $clicks[] = array(
      $info['clicks-top-rank'],
      $info['query'],
      $info['count'],
      $info['position'],
      $info['is-custom'],
    );
  }
  $form['clicks']['table'] = array(
    '#type' => 'markup',
    '#markup' => theme('table', array(
      'header' => array(
        '#',
        t('Query'),
        t('Click count'),
        t('Position'),
        t('User query'),
      ),
      'rows' => $clicks,
    ))
  );

  // Display top shows list.
  $form['shows_summary'] = array(
    '#type' => 'item',
    '#markup' => t('Shows count for last seven days') . ': '
      . $stats['total-shows-count'] . '<br />'
      . t('Procent of popular query shows') . ': '
      . $stats['top-shows-percent'] . '%',
  );
  $form['shows'] = array(
    '#type' => 'fieldset',
    '#title' => t('Top shows for last seven days'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $shows = array();
  foreach ($stats['shows'] as $info) {
    $shows[] = array(
      $info['shows-top-rank'],
      $info['query'],
      $info['count'],
      $info['position'],
      $info['is-custom'],
    );
  }
  $form['shows']['table'] = array(
    '#type' => 'markup',
    '#markup' => theme('table', array(
      'header' => array(
        '#',
        t('Query'),
        t('Shows count'),
        t('Position'),
        t('User query'),
      ),
      'rows' => $shows,
    ))
  );
  return $form;
}

/**
 * Callback function for yandex_webmaster_report_top_form().
 */
function yandex_webmaster_reports_top_form_submit($form, &$form_state) {
  variable_set('yandex_webmaster_basic_host_id', $form_state['values']['selector']['options']);
  drupal_goto('admin/reports/yandex_webmaster/top/' . $form_state['values']['selector']['options']);
}

/**
 * Get Yandex.API top clicks.
 *
 * @param $host_id
 *   Yandex.API host ID.
 *
 * @return array
 *   Top clicks array.
 */
function yandex_webmaster_get_top_clicks($host_id = 0) {
  $stats = yandex_webmaster_get_query('top-queries', $host_id);
  if (isset($stats['error'])) {
    return $stats;
  }
  $data = $stats['result'];
  $xml = yandex_webmaster_parse_xml($data);
  // Fill host stats.
  $shows_count = current($xml->{'top-queries'}->{'total-shows-count'});
  $click_count = current($xml->{'top-queries'}->{'total-clicks-count'});
  $stats['total-shows-count'] = number_format($shows_count, 0, '', ' ');
  $stats['total-clicks-count'] = number_format($click_count, 0, '', ' ');
  // If no statistics.
  if ($shows_count == 0) {
    if ($shows_count == 0) {
      return $stats;
    }
    // Avoid devision by zero.
    $shows_count = 1;
  }
  $stats['shows-clicks-procent'] = number_format(100 - ($shows_count - $click_count) / $shows_count * 100, 2, '.', ' ');
  $stats['top-shows-percent'] = number_format(current($xml->{'top-queries'}->{'top-shows-percent'}), 2, '.', ' ');
  $stats['top-clicks-percent'] = number_format(current($xml->{'top-queries'}->{'top-clicks-percent'}), 2, '.', ' ');
  $stats['shows'] = array();
  $stats['clicks'] = array();
  // Fill shows.
  $shows = $xml->xpath('//top-shows/top-info');
  foreach ($shows as $info) {
    $stats['shows'][] = array(
      'query' => current($info->query),
      'count' => (int) current($info->count),
      'position' => (int) current($info->position),
      'shows-top-rank' => (int) current($info->{'shows-top-rank'}),
      'is-custom' => current($info->{'is-custom'}) == 'false' ? t('No') : t('Yes'),
    );
  }
  // Fill clicks.
  $clicks = $xml->xpath('//top-clicks/top-info');
  foreach ($clicks as $info) {
    $stats['clicks'][] = array(
      'query' => current($info->query),
      'count' => (int) current($info->count),
      'position' => (int) current($info->position),
      'clicks-top-rank' => (int) current($info->{'clicks-top-rank'}),
      'is-custom' => current($info->{'is-custom'}) == 'false' ? t('No') : t('Yes'),
    );
  }
  return $stats;
}

/**
 * Display last indexed pages report for the Yandex.Webmaster module.
 */
function yandex_webmaster_reports_last_form($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'yandex_webmaster') . '/css/yandex_webmaster.css');
  $form = array();
  // Get host list.
  $hostlist = yandex_webmaster_get_hosts();
  // Get selected host id.
  $id = isset($hostlist[variable_get('yandex_webmaster_basic_host_id', 0)]) ? variable_get('yandex_webmaster_basic_host_id', 0) : 0;
  // Get statistics.
  $stats = yandex_webmaster_get_last_index($id);
  if (isset($stats['error'])) {
    drupal_set_message(t('An error occured while getting last indexed pages statistics. @error !link.', array('@error' => filter_xss($stats['error']), '!link' => l(t('Yandex.Webmaster settings page'), 'admin/config/services/yandex_webmaster'))), 'error');
    return $form;
  }
  $form['branding'] = yandex_webmaster_branding_item();
  $form['selector'] = yandex_webmaster_selector_item();

  $host = variable_get('yandex_webmaster_basic_host_id', 0);
  $form['index_summary'] = array(
    '#type' => 'item',
    '#markup' => t('Summary count of indexed pages') . ': '
      . $stats['index-count']
      . '<br />'
      . t('Indexed pages for last seven days') . ': '
      . count($stats['url']),
  );
  $form['view_all'] = array(
    '#type' => 'item',
    '#markup' => l(t('View all indexed pages'), yandex_webmaster_domain_name() . 'site/indexed-pages.xml?host=' . $host . '&path=*', array(
      'attributes' => array(
        'target' => '_blank',
        'title' => t('Open in new window'),
      ),
    )),
  );

  // Display last indexed pages table.
  if (count($stats['url'])) {
    $list = array();
    $counter = 0;
    foreach ($stats['url'] as $url) {
      ++$counter;
      $list[] = array(
        $counter,
        urldecode($url),
      );
    }
    $form['table'] = array(
      '#title' => t('Indexed pages for last seven days'),
      '#type' => 'markup',
      '#markup' => theme('table', array(
        'header' => array(
          '#',
          t('Address'),
        ),
        'rows' => $list,
      ))
    );
  }

  return $form;
}

/**
 * Callback function for yandex_webmaster_report_last_form().
 */
function yandex_webmaster_reports_last_form_submit($form, &$form_state) {
  variable_set('yandex_webmaster_basic_host_id', $form_state['values']['selector']['options']);
  drupal_goto('admin/reports/yandex_webmaster/last/' . $form_state['values']['selector']['options']);
}

/**
 * Get Yandex.API last indexed pages list.
 *
 * @param $host_id
 *   Yandex.API host ID.
 *
 * @return array
 *   Last indexed pages array.
 */
function yandex_webmaster_get_last_index($host_id = 0) {
  $stats = yandex_webmaster_get_query('indexed-urls', $host_id);
  if (isset($stats['error'])) {
    return $stats;
  }
  $data = $stats['result'];
  $xml = yandex_webmaster_parse_xml($data);
  // Fill last indexed pages stats.
  $stats['index-count'] = number_format(current($xml->{'index-count'}), 0, '', ' ');
  $stats['url'] = array();
  $links = $xml->{'last-week-index-urls'}->url;
  $stats['url'] = array();
  foreach ($links as $url) {
    $stats['url'][] = current($url);
  }
  return $stats;
}

/**
 * Display excluded from the index pages report for the Yandex.Webmaster module.
 */
function yandex_webmaster_reports_excluded_form($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'yandex_webmaster') . '/css/yandex_webmaster.css');
  $form = array();
  // Get host list.
  $hostlist = yandex_webmaster_get_hosts();
  // Get selected host id.
  $id = isset($hostlist[variable_get('yandex_webmaster_basic_host_id', 0)]) ? variable_get('yandex_webmaster_basic_host_id', 0) : 0;
  // Get statistics.
  $stats = yandex_webmaster_get_excluded_pages($id);
  if (isset($stats['error'])) {
    drupal_set_message(t('An error occured while getting excluded pages statistics. @error !link.', array('@error' => filter_xss($stats['error']), '!link' => l(t('Yandex.Webmaster settings page'), 'admin/config/services/yandex_webmaster'))), 'error');
    return $form;
  }
  $form['branding'] = yandex_webmaster_branding_item();
  $form['selector'] = yandex_webmaster_selector_item();

  $form['excluded_summary'] = array(
    '#type' => 'item',
    '#markup' => t('Summary count of excluded pages') . ': ' . $stats['url-errors'],
  );

  // Display excluded pages table.
  if (count($stats['url'])) {
    $list = array();
    $host = (int) variable_get('yandex_webmaster_basic_host_id', 0);
    foreach ($stats['url'] as $info) {
      $list[] = array(
        'data' => array(
          $info['severity'],
          yandex_webmaster_code_description($info['code']),
          $info['count'],
          l(t('View'), yandex_webmaster_domain_name() . 'site/errors/types.xml?host=' . $host . '&code=' . $info['code'], array(
            'attributes' => array(
              'target' => '_blank',
              'title' => t('Open in new window'),
            ),
          )),
        ),
        'class' => array(drupal_strtolower($info['severity'])),
      );
    }
    $form['table'] = array(
      '#title' => t('External links for last seven days'),
      '#type' => 'markup',
      '#markup' => theme('table', array(
        'header' => array(
          t('Severity'),
          t('Error type'),
          t('Count'),
          t('Actions'),
        ),
        'rows' => $list,
      ))
    );
  }

  return $form;
}

/**
 * Callback function for yandex_webmaster_report_excluded_form().
 */
function yandex_webmaster_reports_excluded_form_submit($form, &$form_state) {
  variable_set('yandex_webmaster_basic_host_id', $form_state['values']['selector']['options']);
  drupal_goto('admin/reports/yandex_webmaster/errors/' . $form_state['values']['selector']['options']);
}

/**
 * Get Yandex.API last indexed pages list.
 *
 * @param $host_id
 *   Yandex.API host ID.
 *
 * @return array
 *   Last indexed pages array.
 */
function yandex_webmaster_get_excluded_pages($host_id = 0) {
  $stats = yandex_webmaster_get_query('excluded-urls', $host_id);
  if (isset($stats['error'])) {
    return $stats;
  }
  $data = $stats['result'];
  $xml = yandex_webmaster_parse_xml($data);
  // Fill excluded pages stats.
  $stats['url-errors'] = number_format(current($xml->{'url-errors'}->attributes()->{'count'}), 0, '', ' ');
  $stats['url'] = array();
  $links = $xml->{'url-errors'}->{'url-errors-with-code'};
  foreach ($links as $url) {
    $stats['url'][] = array(
      'code' => current($url->attributes()->{'code'}),
      'count' => current($url->count),
      'severity' => current($url->severity),
    );
  }
  return $stats;
}

/**
 * Display external links report for the Yandex.Webmaster module.
 */
function yandex_webmaster_reports_links_form($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'yandex_webmaster') . '/css/yandex_webmaster.css');
  $form = array();
  // Get host list.
  $hostlist = yandex_webmaster_get_hosts();
  // Get selected host id.
  $id = isset($hostlist[variable_get('yandex_webmaster_basic_host_id', 0)]) ? variable_get('yandex_webmaster_basic_host_id', 0) : 0;
  // Get statistics.
  $stats = yandex_webmaster_get_external_links($id);
  if (isset($stats['error'])) {
    drupal_set_message(t('An error occured while getting external links statistics. @error !link.', array('@error' => filter_xss($stats['error']), '!link' => l(t('Yandex.Webmaster settings page'), 'admin/config/services/yandex_webmaster'))), 'error');
    return $form;
  }
  $form['branding'] = yandex_webmaster_branding_item();
  $form['selector'] = yandex_webmaster_selector_item();

  $host = variable_get('yandex_webmaster_basic_host_id', 0);
  $form['index_summary'] = array(
    '#type' => 'item',
    '#markup' => t('Summary count of external links') . ': '
      . $stats['links-count']
      . '<br />'
      . t('External links for last seven days') . ': '
      . count($stats['url']),
  );
  $form['view_all'] = array(
    '#type' => 'item',
    '#markup' => l(t('View all external links'), 'http://webmaster.yandex.com/site/indexed/links.xml?host=' . $host . '&path=*', array(
      'attributes' => array(
        'target' => '_blank',
        'title' => t('Open in new window'),
      ),
    )),
  );

  // Display last indexed pages table.
  if (count($stats['url'])) {
    $list = array();
    $counter = 0;
    foreach ($stats['url'] as $url) {
      ++$counter;
      $list[] = array(
        $counter,
        urldecode($url),
      );
    }
    $form['table'] = array(
      '#title' => t('External links for last seven days'),
      '#type' => 'markup',
      '#markup' => theme('table', array(
        'header' => array(
          '#',
          t('Address'),
        ),
        'rows' => $list,
      ))
    );
  }

  return $form;
}

/**
 * Callback function for yandex_webmaster_report_links_form().
 */
function yandex_webmaster_reports_links_form_submit($form, &$form_state) {
  variable_set('yandex_webmaster_basic_host_id', $form_state['values']['selector']['options']);
  drupal_goto('admin/reports/yandex_webmaster/links/' . $form_state['values']['selector']['options']);
}

/**
 * Get Yandex.API external links list.
 *
 * @param $host_id
 *   Yandex.API host ID.
 *
 * @return array
 *   External links array.
 */
function yandex_webmaster_get_external_links($host_id = 0) {
  $stats = yandex_webmaster_get_query('incoming-links', $host_id);
  if (isset($stats['error'])) {
    return $stats;
  }
  $data = $stats['result'];
  $xml = yandex_webmaster_parse_xml($data);
  // Fill last indexed pages stats.
  $stats['links-count'] = number_format(current($xml->{'links-count'}), 0, '', ' ');
  $links = $xml->{'last-week-links'}->url;
  $stats['url'] = array();
  foreach ($links as $url) {
    $stats['url'][] = current($url);
  }
  return $stats;
}

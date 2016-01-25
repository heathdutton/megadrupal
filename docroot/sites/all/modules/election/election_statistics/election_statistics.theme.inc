<?php
/**
 * @file
 * Theme and preprocess functions for the Election Statistics module.
 */

/**
 * Theme the election statistics block.
 */
function theme_election_statistics_block($variables) {
  $totals = $variables['totals'];
  $rows = array(
    array(t('Voters'), number_format($totals['voters'])),
    array(t('Ballots'), number_format($totals['ballots'])),
    array(t('Votes'), number_format($totals['votes'])),
    array(t('Abstentions'), number_format($totals['abstentions'])),
  );
  return theme('table', array(
    'header' => array(),
    'rows' => $rows,
    'attributes' => array('class' => array('election-statistics')),
  ));
}

/**
 * Theme election statistics.
 */
function theme_election_statistics($variables) {

  $output = render($variables['content']);

  return $output;
}

/**
 * Preprocess function for election statistics.
 */
function template_preprocess_election_statistics(&$variables) {

  $election = $variables['election'];

  $variables['content'] = array();

  $totals = election_statistics_get_totals($election);
  if ($totals['ballots']) {
    $posts = _election_statistics_get_posts($election);
    $totals['posts'] = election_statistics_get_post_totals($election, $posts);
  }

  $post_name = election_get_post_name($election->type);

  $header = array(
    'voters' => t('Voters'),
    'ballots' => t('Ballots'),
    'votes' => t('Votes'),
    'abstentions' => t('Abstentions'),
  );
  $rows = array();
  $voter_avg = $totals['voters'] ? $totals['ballots'] / $totals['voters'] : 0;
  $rows[0] = array(
    'voters' => number_format($totals['voters']),
    'ballots' => t('@ballots (@avg each)', array(
      '@ballots' => number_format($totals['ballots']),
      '@avg' => number_format($voter_avg, 1),
    )),
    'votes' => number_format($totals['votes']),
    'abstentions' => number_format($totals['abstentions']),
  );

  $election_table = array(
    '#caption' => t('Election totals'),
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );

  $variables['content']['election_table'] = $election_table;

  $posts_table = NULL;
  if (!empty($posts)) {

    $header = array(
      'post' => check_plain(drupal_ucfirst($post_name)),
      'voters' => t('Voters'),
      'ballots' => t('Ballots'),
      'votes' => t('Votes'),
      'abstentions' => t('Abstentions'),
    );

    $rows = array();
    foreach ($posts as $post_id => $post) {

      $post_totals = isset($totals['posts'][$post_id]) ? $totals['posts'][$post_id] : FALSE;

      $rows[] = array(
        'post' => l($post->title, election_post_uri_path($post)),
        'voters' => $post_totals ? number_format($post_totals['voters']) : 0,
        'ballots' => $post_totals ? number_format($post_totals['ballots']) : 0,
        'votes' => $post_totals ? number_format($post_totals['votes']) : 0,
        'abstentions' => $post_totals ? number_format($post_totals['abstentions']) : 0,
      );

    }

    $posts_table = array(
      '#caption' => t('Totals per @post', array('@post' => $post_name)),
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );

  }

  $variables['content']['posts_table'] = $posts_table;

  $top_agents_table = NULL;
  $top_agents = election_statistics_get_top_count($election, 'agent');
  if (count($top_agents)) {
    $header = array(t('Voters'), t('User agent'));
    $rows = array();
    $browscap = module_exists('browscap');
    if ($browscap) {
      $header[] = t('Platform');
      $header[] = t('Browser');
      $header[] = t('Version');
    }
    foreach ($top_agents as $record) {
      $row = array(
        number_format($record['voters']),
        array(
          'data' => check_plain($record['agent']),
          'class' => array('user-agent'),
        ),
      );
      if ($browscap) {
        $browser = browscap_get_browser($record['agent']);
        $row[] = isset($browser['platform']) ? check_plain($browser['platform']) : 'unknown';
        $row[] = isset($browser['browser']) ? check_plain($browser['browser']) : 'unknown';
        $row[] = isset($browser['version']) ? check_plain($browser['version']) : 'unknown';
      }
      $rows[] = $row;
    }
    $top_agents_table = array(
      '#caption' => t('Top 10 user agents'),
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#attributes' => array('class' => array('election-statistics-user-agents')),
    );
    if (!$browscap) {
      $top_agents_table['#rows'][] = array(
        array(
          'data' => t(
            'Install the !Browscap module to display browser and platform details in this table.',
            array('!Browscap' => l(t('Browscap'), 'http://drupal.org/project/browscap'))
          ),
          'colspan' => 3,
        ),
      );
    }
  }
  $variables['content']['top_agents_table'] = $top_agents_table;

  $top_ips_table = NULL;
  $top_ips = election_statistics_get_top_count($election, 'ip');
  if (count($top_ips)) {
    $header = array(t('Voters'), t('IP address'));
    $rows = array();
    foreach ($top_ips as $record) {
      $rows[] = array(
        number_format($record['voters']),
        check_plain($record['ip']),
      );
    }
    $top_ips_table = array(
      '#caption' => t('Top 10 IP addresses'),
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );
  }
  $variables['content']['top_ips_table'] = $top_ips_table;

  $variables['totals'] = $totals;

}

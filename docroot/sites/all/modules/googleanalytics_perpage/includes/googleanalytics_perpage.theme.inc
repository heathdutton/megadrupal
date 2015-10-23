<?php

/**
 * @file googleanalytics_perpage.theme.inc
 */


/**
 * Theme function for the per-page snippets form.
 */
function theme_googleanalytics_perpage_snippet_table($variables) {
  $snippet = $variables['form'];
  
  $header = array(
    t('Name'),
    t('Status'),
    t('Snippet'),
    t('Path(s)'),
    '',
  );
  foreach ($snippet as $k => $v) {
    if (is_numeric($k)) {
      unset(
        $v['title']['#title'],
        $v['snippet']['#title'],
        $v['paths']['#title']
      );
      $row[0] = drupal_render($v['title']);
      $row[1] = drupal_render($v['active']);
      $row[2] = drupal_render($v['snippet']);
      $row[3] = drupal_render($v['paths']);
      $row[4] = drupal_render($v['gappid']) . drupal_render($v['remove']);
      $rows[] = $row;
    }
  }
  $rows[] = array(
    0 => t('Human-readable name, for reference of what this snippet is used for.'),
    1 => '',
    2 => t('Before you add your custom code, you should read the !ga_overview and the !ga_api documentation. <strong>Do not include the &lt;script&gt; tags</strong>, and always end your code with a semicolon (;).', array(
      '!ga_overview' => l('Google Analytics Tracking Code - Functional Overview', 'http://code.google.com/apis/analytics/docs/gaConceptsOverview.html'),
      '!ga_api' => l('Google Analytics Tracking API', 'http://code.google.com/apis/analytics/docs/gaJSApi.html'))),
    3 => t('Enter one site-path per line. Asterisk wildcard is acceptable. For example, to include all user pages, enter <strong>user/*</strong>'),
    4 => '',
  );

  return theme('table', array('header' => $header, 'rows' => $rows));
}
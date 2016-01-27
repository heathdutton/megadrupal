<?php

/**
 * Overview of all tests.
 */
function testswarm_tests($githash = '', $theme = '', $karma = '', $module = '') {
  $sql = "SELECT caller, theme, githash, count(caller) as count, total,
      count(caller) as num_runs,
      100 * (avg(failed) / total) as failed,
      avg(runtime) as runtime,
      min(timestamp) as first_run, max(timestamp) as last_run
    FROM {testswarm_test} q";

  $where = array();
  $params = array();
  $pagetitle = array();
  $filter_failures = FALSE;

  // Filter by githash
  if (!empty($githash) && $githash != 'ALL') {
    if ($githash === 'LATEST') {
      $githash = variable_get('testswarm_githash', '');
    }
    $pagetitle[] = _testswarm_short_githash($githash);
    $where[] = " githash = :githash ";
    $params[':githash'] = check_plain($githash);
  }

  // Filter by theme
  if (!empty($theme) && $theme != 'ALL') {
    $pagetitle[] = check_plain($theme);
    $where[] = " theme = :theme ";
    $params[':theme'] = check_plain($theme);
  }

  // Filter by karma
  if (!empty($karma) && $karma != 'ALL') {
    $pagetitle[] = check_plain($karma);
    $where[] = " karma = :karma ";
    $params[':karma'] = check_plain($karma);
  }

  // Only show failures
  if (isset($_GET['filter-failures']) && !empty($_GET['filter-failures'])) {
    $pagetitle[] = 'only failures';
    $where[] = " failed <> 0 ";
    $filter_failures = TRUE;
  }

  if (!empty($where)) {
    $sql .= ' WHERE ' . implode(' AND ', $where);
    drupal_set_title('Overview of all tests - ' . implode(', ', $pagetitle));
  }
  else {
    drupal_set_title('Overview of all tests');
  }
  $sql .= " GROUP BY caller, theme, githash
    ORDER BY caller, theme, githash";
    $result = db_query($sql, $params);

  foreach ($result as $row) {
    $data[$row->caller][] = array(
      'githash' => $row->githash,
      'theme' => $row->theme,
      'num_runs' => $row->num_runs,
      'total' => $row->total,
      'failed' => $row->failed,
      'runtime' => $row->runtime,
      'first_run' => $row->first_run,
      'last_run' => $row->last_run,
    );
  }

  $output = '';
  $output .= '<p>' . l('Run all tests', 'testswarm-run-all-tests') . '</p>';
  $output .= '<p>Current/Latest drupal version: ' . l(_testswarm_short_githash(), 'testswarm-tests/' . variable_get('testswarm_githash', ''));
  $output .= ' | ' . l('Show all tests', 'testswarm-tests/ALL/ALL');
  $output .= '</p>';

  if (!$filter_failures) {
    $output .= l('Show only failures', $_GET['q'], array('query' => array('filter-failures' => 'yes')));
  }

  $current_githash = empty($githash) ? 'ALL' : $githash;
  $current_theme = empty($theme) ? 'ALL' : $theme;
  $current_karma = empty($karma) ? 'ALL' : $karma;

  // Filter on theme
  $testswarm_themes = testswarm_themes_to_test();
  $output .= '<p>Filter by theme: ' . l('All themes', 'testswarm-tests/' . $current_githash);
  foreach ($testswarm_themes as $testswarm_theme) {
    $output .= ' | ' . l($testswarm_theme, 'testswarm-tests/' . $current_githash . '/' . $testswarm_theme);
  }
  $output .= '</p>';

  // Filter on karma
  $karmas = testswarm_get_karmas();
  $output .= '<p>Filter by karma: ' . l('All karmas', 'testswarm-tests/' . $current_githash . '/' . $current_theme);
  foreach ($karmas as $karma => $points) {
    $output .= ' | ' . l($karma . ' (' . $points . ')', 'testswarm-tests/' . $current_githash . '/' . $current_theme . '/' . $karma);
  }
  $output .= '</p>';

  $header = array(
    'githash',
    'theme',
    '# runs',
    '# tests',
    '% failed tests',
    'time taken (ms)',
    'first run',
    'last run',
  );

  $tests = testswarm_defined_tests($module);
  foreach ($tests as $caller => $test) {
    $output .= '<h2>' . $test['module'] . '::' . $caller . '</h2>';
    if (isset($test['description'])) {
      $output .= '<p><em>' . $test['description'] . '</em></p>';
    }
    $output .= '<p>'
      . (empty($githash) ? l('Details', 'testswarm-tests/detail/' . $caller) : l('Details', 'testswarm-tests/detail/' . $caller . '/' . $githash));

    foreach ($testswarm_themes as $testswarm_theme) {
      $output .= (testswarm_user_can_run_test($test) ? ' | ' . l('Test now in ' . $testswarm_theme, 'testswarm-run-a-test/' . $caller, array('query' => array('testswarm-theme' => $testswarm_theme, 'destination' => 'testswarm-tests'))) : '');
    }

    foreach ($testswarm_themes as $testswarm_theme) {
      $output .= (testswarm_user_can_run_test($test) ? ' | ' . l('Test manually in ' . $testswarm_theme, 'testswarm-run-a-test/' . $caller, array('query' => array('testswarm-theme' => $testswarm_theme, 'destination' => 'testswarm-tests', 'debug' => 'on'))) : '');
    }

    $output .= (user_access('administer testswarm tests') ? ' | ' . l('Clear test details', 'testswarm-tests/clear/' . $caller, array('query' => array('destination' => 'testswarm-tests'))) : '')
      . '</p>';
    $rows = array();
    if (isset($data[$caller])) {
      foreach ($data[$caller] as $rowdata) {
        $rows[] = array(
          'data' => array(
            l(_testswarm_short_githash($rowdata['githash']), 'testswarm-tests/' . $rowdata['githash']),
            $rowdata['theme'],
            $rowdata['num_runs'],
            $rowdata['total'],
            round($rowdata['failed'], 2) . '%',
            $rowdata['runtime'],
            format_date($rowdata['first_run'], 'short'),
            format_date($rowdata['last_run'], 'short'),
          ),
          'class' => ($rowdata['failed'] == 0 ? array('testswarm-passed') : array('testswarm-failed')),
        );
      }
      $output .= theme('table', array(
        'header' => $header,
        'rows' => $rows,
      ));
    }
  }

  drupal_add_css(drupal_get_path('module', 'testswarm') . '/testswarm.css');
  return $output;
}

/**
 * Detailed information of a test.
 */
function testswarm_test_details($caller, $githash = '') {
  $output = '';
  $tests = testswarm_defined_tests();
  if (!empty($tests)) {
    $test = $tests[$caller];
    if ($test) {

      $where = array('qt.caller = :caller');
      $params = array(':caller' => check_plain($caller));
      $pagetitle = array();

      if (!empty($githash) && $githash != 'ALL') {
        $where[] = " githash = :githash ";
        $params[':githash'] = check_plain($githash);
        $pagetitle[] = _testswarm_short_githash($githash);
      }

      // Only show failures
      $filter_failures = FALSE;
      if (isset($_GET['filter-failures']) && !empty($_GET['filter-failures'])) {
        $pagetitle[] = 'only failures';
        $where[] = " failed <> 0 ";
        $filter_failures = TRUE;
      }

      drupal_set_title('TestSwarm tests details - ' . implode(', ', $pagetitle));
      $sql = "SELECT qt.* FROM {testswarm_test} qt";
      $sql .= ' WHERE ' . implode(' AND ', $where);
      $sql .= " ORDER BY timestamp DESC
        LIMIT 50";
      $result = db_query($sql, $params);

      $output .= '<h2>' . $test['module'] . '::' . $caller . '</h2>';
      if (!$filter_failures) {
        $output .= l('Show only failures', $_GET['q'], array('query' => array('filter-failures' => 'yes')));
      }

      $output .= '<p>';
      $testswarm_themes = testswarm_themes_to_test();
      foreach ($testswarm_themes as $testswarm_theme) {
        $output .= (testswarm_user_can_run_test($test) ? l('Test now in ' . $testswarm_theme, 'testswarm-run-a-test/' . $caller, array('query' => array('testswarm-theme' => $testswarm_theme, 'destination' => $_GET['q']))) . ' | ' : '');
      }

      foreach ($testswarm_themes as $testswarm_theme) {
        $output .= (testswarm_user_can_run_test($test) ? l('Test manually in ' . $testswarm_theme, 'testswarm-run-a-test/' . $caller, array('query' => array('testswarm-theme' => $testswarm_theme, 'destination' => $_GET['q'], 'debug' => 'on'))) . ' | ' : '');
      }

      $output .= (user_access('administer testswarm tests') ? l('Clear test details', 'testswarm-tests/clear/' . $caller, array('query' => array('destination' => 'testswarm-tests/detail/' . $caller))) : '')
        . '</p>';

      $header = array(
        'githash',
        'karma',
        'theme',
        'browser',
        'ua',
        '# tests',
        '# failed',
        'time taken (ms)',
        'timestamp',
        'details'
      );

      $rows = array();
      foreach ($result as $rowdata) {
        $browser = get_browser($rowdata->useragent);
        $rows[] = array(
          'data' => array(
            _testswarm_short_githash($rowdata->githash),
            $rowdata->karma,
            $rowdata->theme,
            $browser->browser . ' (' . $browser->parent . ')',
            check_plain($rowdata->useragent),
            $rowdata->total,
            $rowdata->failed,
            $rowdata->runtime,
            format_date($rowdata->timestamp, 'short'),
            l('Details', 'testswarm-tests/detail/' . $caller . '/hash/' . $rowdata->githash),
          ),
          'class' => ($rowdata->failed == 0 ? array('testswarm-passed') : array('testswarm-failed')),
        );
      }

      $output .= theme('table', array(
        'header' => $header,
        'rows' => $rows,
      ));

      drupal_add_css(drupal_get_path('module', 'testswarm') . '/testswarm.css');
    }
  }
  return $output;
}

/**
 * Detailed information of one git hash.
 */
function testswarm_test_details_hash($caller, $githash) {
  $output = '';
  $tests = testswarm_defined_tests();
  if (!empty($tests)) {
    $test = $tests[$caller];
    if ($test) {
      $where = array('qt.caller = :caller');
      $params = array(':caller' => check_plain($caller));
      $pagetitle = array();

      if (!empty($githash) && $githash != 'ALL') {
        $where[] = " githash = :githash ";
        $params[':githash'] = check_plain($githash);
        $pagetitle[] = _testswarm_short_githash($githash);
      }

      // Only show failures
      $filter_failures = FALSE;
      if (isset($_GET['filter-failures']) && !empty($_GET['filter-failures'])) {
        $pagetitle[] = 'only failures';
        $where[] = " qtrd.result <> 1 ";
        $filter_failures = TRUE;
      }

      drupal_set_title('TestSwarm verbose tests details - ' . implode(', ', $pagetitle));
      $sql = "SELECT qt.theme, qt.useragent, qt.caller, qtr.*, qtrd.* FROM {testswarm_test} qt
        INNER JOIN {testswarm_test_run} qtr ON qt.id = qtr.qt_id
        INNER JOIN {testswarm_test_run_detail} qtrd ON qtr.id = qtrd.tri";
      $sql .= ' WHERE ' . implode(' AND ', $where);
      $sql .= " ORDER BY qtr.id DESC
        LIMIT 50";
      $result = db_query($sql, $params);

      $output .= '<h2>' . $test['module'] . '::' . $caller . '</h2>';
      if (!$filter_failures) {
        $output .= l('Show only failures', $_GET['q'], array('query' => array('filter-failures' => 'yes')));
      }

      $output .= '<p>'
        . (testswarm_user_can_run_test($test) ? l('Test now', 'testswarm-run-a-test/' . $caller, array('query' => array('destination' => 'testswarm-tests/detail/' . $caller))) : '')
        . (testswarm_user_can_run_test($test) ? ' | ' . l('Test manually', 'testswarm-run-a-test/' . $caller, array('query' => array('debug' => 'on'))) : '')
        . (user_access('administer testswarm tests') ? ' | ' . l('Clear test details', 'testswarm-tests/clear/' . $caller, array('query' => array('destination' => 'testswarm-tests/detail/' . $caller))) : '')
        . '</p>'
        . '<p>Drupal version: ' . l(_testswarm_short_githash($githash), 'http://drupalcode.org/project/drupal.git/commit/' . $githash) . '</p>';


      // Group by module/test
      $data = array();
      foreach ($result as $rowdata) {
        $data[$rowdata->module][$rowdata->name][] = $rowdata;
      }

      $output .= '<h2>' . $test['module'] . '::' . $caller . '</h2>';
      $header = array(
        'theme',
        'browser',
        'ua',
        'test',
        'result',
        'message',
        'actual',
        'expected',
        'details',
      );

      foreach ($data as $module => $moduledata) {
        $output .= '<h3>' . $module . '</h3>';
        foreach ($moduledata as $testname => $testdata) {
          $output .= '<h4>' . $testname . '</h4>';
          $rows = array();
          foreach ($testdata as $rowdata) {
            $browser = get_browser($rowdata->useragent);
            $rows[] = array(
              'data' => array(
                $rowdata->theme,
                $browser->browser . ' (' . $browser->parent . ')',
                check_plain($rowdata->useragent),
                $testname,
                $rowdata->result,
                check_plain($rowdata->message),
                check_plain($rowdata->actual),
                check_plain($rowdata->expected),
                l('Details', 'testswarm-tests/detail/' . $caller . '/tests/' . $rowdata->tri),
              ),
              'class' => ($rowdata->result == 1 ? array('testswarm-passed') : array('testswarm-failed')),
            );
          }
          $output .= theme('table', array(
            'header' => $header,
            'rows' => $rows,
          ));
        }
      }

      drupal_add_css(drupal_get_path('module', 'testswarm') . '/testswarm.css');
    }
  }
  return $output;
}

/**
 * Detailed information of one test run.
 */
function testswarm_test_details_tests($caller, $id) {
  $output = '';
  $tests = testswarm_defined_tests();
  if (!empty($tests)) {
    $test = $tests[$caller];
    if ($test) {
      $where = array('qtrd.tri = :id');
      $params = array(':id' => check_plain($id));
      $pagetitle = array();

      // Only show failures
      $filter_failures = FALSE;
      if (isset($_GET['filter-failures']) && !empty($_GET['filter-failures'])) {
        $pagetitle[] = 'only failures';
        $where[] = " qtrd.result <> 1 ";
        $filter_failures = TRUE;
      }

      drupal_set_title('TestSwarm tests details of a run - ' . implode(', ', $pagetitle));
      $sql = "SELECT qt.theme, qt.useragent, qtr.*, qtrd.* FROM {testswarm_test} qt
        INNER JOIN {testswarm_test_run} qtr ON qt.id = qtr.qt_id
        INNER JOIN {testswarm_test_run_detail} qtrd ON qtr.id = qtrd.tri";
      $sql .= ' WHERE ' . implode(' AND ', $where);
      $sql .= " ORDER BY qtr.id DESC
        LIMIT 50";
      $result = db_query($sql, $params);

      // Group by module/test
      $data = array();
      $browserinfo = '';

      foreach ($result as $rowdata) {
        $data[$rowdata->module][$rowdata->name][] = $rowdata;
        if (empty($browserinfo)) {
          $browser = get_browser($rowdata->useragent);
          $browserinfo = $rowdata->theme . '::' . $browser->browser . ' - ' . $browser->parent . ' (' . $rowdata->useragent . ') by ' . $rowdata->karma;
          $output .= '<p>' . $browserinfo . '</p>';
        }
      }

      $output .= '<h2>' . $test['module'] . '::' . $caller . '</h2>';
      if (!$filter_failures) {
        $output .= l('Show only failures', $_GET['q'], array('query' => array('filter-failures' => 'yes')));
      }

      $header = array(
        'test',
        'result',
        'message',
        'actual',
        'expected',
      );

      foreach ($data as $module => $moduledata) {
        $output .= '<h3>' . $module . '</h3>';
        foreach ($moduledata as $testname => $testdata) {
          $output .= '<h4>' . $testname . '</h4>';
          $rows = array();
          foreach ($testdata as $rowdata) {
            $rows[] = array(
              'data' => array(
                $testname,
                $rowdata->result,
                check_plain($rowdata->message),
                check_plain($rowdata->actual),
                check_plain($rowdata->expected),
              ),
              'class' => ($rowdata->result == 1 ? array('testswarm-passed') : array('testswarm-failed')),
            );
          }
          $output .= theme('table', array(
            'header' => $header,
            'rows' => $rows,
          ));
        }
      }

      drupal_add_css(drupal_get_path('module', 'testswarm') . '/testswarm.css');
    }
  }
  return $output;
}

/*
 * Set karma form
 */

function testswarm_set_karma_form($form, &$form_state) {
  $form = array();

  $form['karma'] = array(
    '#type' => 'textfield',
    '#title' => t('Karma name'),
    '#required' => TRUE,
    '#default_value' => isset($_COOKIE['karma']) ? $_COOKIE['karma'] : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save karma name')
  );

  return $form;
}

/**
 * Process reinstall menu form submissions.
 */
function testswarm_set_karma_form_submit($form, &$form_state) {
  setrawcookie('karma', check_plain($form_state['values']['karma']), REQUEST_TIME + 31536000, '/');
  drupal_set_message(t('Karma cookie set.'));
}

/**
 * Page callback. Overview of browserstack workers.
 */
function testswarm_browserstack_status($form, $form_state) {
  $form['run_tests'] = array(
    '#type' => 'submit',
    '#value' => t('Run Browser Tests'),
    '#submit' => array('testswarm_run_browserstack_tests'),
  );
  $form['delete_workers'] = array(
    '#type' => 'submit',
    '#value' => t('Delete Workers'),
    '#submit' => array('testswarm_delete_browserstack_workers'),
  );

  $workers = testswarm_get_workers();
  $header = array(t('ID'), t('Browser'), t('Browser Version'), t('Status'));
  $rows = array();
  if ($workers) {
    foreach ($workers->data as $worker) {
      $row = array(
        'data' => array(
          $worker['id'],
          $worker['browser']['name'],
          $worker['browser']['version'],
          $worker['status'],
        ),
        'class' => array($worker['status']),
      );
      $rows[] = $row;
    }
  }
  $form['workers'] = array(
    '#type' => 'item',
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No workers were retreived.'),
  );

  return $form;
}

function testswarm_framed_form($form, &$form_state, $caller, $theme) {
  $form = array();
// http://cv20120305.ubuntu006.attiks.office/testswarm-run-a-test/createpagecontent?testswarm-theme=bartik&destination=testswarm-tests&debug=on
  $test = '';
  $tests = testswarm_defined_tests();
  if (array_key_exists($caller, $tests)) {
    $test = $tests[$caller];
  }

  if ($test) {
    drupal_add_css(drupal_get_path('module', 'testswarm') . '/qunit/qunit.css');
    drupal_add_js(drupal_get_path('module', 'testswarm') . '/qunit/qunit.js', array('weight' => JS_LIBRARY));
    drupal_add_js(drupal_get_path('module', 'testswarm') . '/testswarm.admin.js', array('weight' => JS_LIBRARY));
    drupal_add_js(drupal_get_path('module', 'testswarm') . '/testswarm.js', array('weight' => JS_LIBRARY));
    foreach ($test['js'] as $js => $info) {
      drupal_add_js($js);
    }
    
    $karma = '';
    if (isset($_COOKIE['karma']) && !empty($_COOKIE['karma'])) {
      $karma = $_COOKIE['karma'];
    }
    
    $settings = array(
      'caller' => $caller,
      'theme' => check_plain($theme),
      'karma' => $karma,
      'token' => drupal_get_token($caller, TRUE),
      'debug' => 'on', // No auto redirect on iframed tests,
    );
    drupal_add_js(array('testswarm' => $settings), 'setting');
    
    $form['qunit'] = array(
      '#type' => 'markup',
      '#markup' =>  '<h1 id="qunit-header">Drupal TestSwarm</h1>
        <h2 id="qunit-banner"></h2>
        <div id="qunit-testrunner-toolbar"></div>
        <h2 id="qunit-userAgent"></h2>
        <ol id="qunit-tests"></ol>
        <div id="xtestswarm-fixture"></div>',
    );

    // @TODO: Posts are passing ?testswarm-theme, but it doesn't have any effect, defaulting the default theme.
    $form['iframe'] = array(
      '#type' => 'markup',
      '#markup' => '<iframe name="testframe" width="100%" height="400" src="/' . $test['path'] . '?testswarm-theme=' . $theme . '"></iframe>',
    );
  }
  
  return $form;
}

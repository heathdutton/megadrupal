<?php

/**
 * Implements settings for the module.
 */
function travisbuilder_settings_form($form_state) {
  $configs = _travisbuilder_configs();
  $prefix = 'travisbuilder_';

  if ($configs['last']) {
    $form = array(
      '#prefix' => t('Last built: @time',  array('@time' => format_date($configs['last'], 'long')))
    );
  }

  $form[$prefix . 'private'] = array(
    '#type' => 'checkbox',
    '#title' => t('Private account'),
    '#default_value' => $configs['private'],
    '#description' => t('Enable if using the paid, private version of Travis.'),
  );

  // @todo Use a temporary token generated each time.
  // http://docs.travis-ci.com/api/#creating-a-temporary-github-token
  $form[$prefix . 'access_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Access token'),
    '#default_value' => $configs['access_token'],
    '#description' => t('Access token allowing authentication with your account. Generate via travis-cli utility.'),
  );

  $form[$prefix . 'schedule'] = array(
    '#type' => 'select',
    '#title' => t('Build minimum lifetime'),
    '#default_value' => $configs['schedule'],
    '#description' => t('This build lifetime will be enforced to avoid build overlap.'),
    '#options' => array(
      0 => t('None'),
      900 => t('!num minutes', array('!num' => '15')),
      3600 => t('!num hour', array('!num' => '1')),
      21600 => t('!num hours', array('!num' => '6')),
      86400 => t('!num day', array('!num' => '1')),
      604800 => t('!num week', array('!num' => '1')),
      2592000 => t('!num month', array('!num' => '1')),
    ),
  );

  // @ todo Allow separate repos to trigger as ctools configs.
  $form[$prefix . 'repo'] = array(
    '#type' => 'textfield',
    '#title' => t('Travis repository'),
    '#default_value' => $configs['repo'],
    '#description' => t('Travis repo to trigger a build. Example: username/my-project-repo'),
  );

  $form[$prefix . 'branch'] = array(
    '#type' => 'textfield',
    '#title' => t('Default branch'),
    '#default_value' => $configs['branch'],
    '#description' => t('Branch of repository on which to trigger the recent build. Can be overridden within each Rule. Examples: master, deploy'),
  );

  return system_settings_form($form);
}


/**
 * Allows triggering a build via UI.
 */
function travisbuilder_build_confirm_form($form_state) {
  $configs = _travisbuilder_configs();
  $prefix = t('Restart a Travis build of the configured repository and branch.');
  if ($configs['last']) {
    $prefix .= '<br>' . t('Last built: @time',  array('@time' => format_date($configs['last'], 'long')));
  }

  // @todo Use text field for branch selection.
  return array(
    '#prefix' => $prefix,
    'ignore' => array(
      '#type' => 'checkbox',
      '#title' => t('Ignore minimum lifetime'),
      '#description' => t('Forces a new build, even if the configured amount of time has not passed.'),
    ),
    '#submit' => array('_travisbuilder_trigger_build_submit'),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Restart build'),
    ),
  );
}


/**
 * Submit action for triggering build.
 */
function _travisbuilder_trigger_build_submit($form, &$form_state) {
  $configs = _travisbuilder_configs();
  $ignore = $form_state['values']['ignore'];

  // @todo Use text field for branch selection.
  $result = _travisbuilder_trigger_build($configs['branch'], $ignore);

  // Inform the user.
  $type = ($result['status']) ? 'status' : 'error';
  drupal_set_message(t($result['message'], $result['vars']), $type);
}


/**
 * Deliver a page for build history.
 *
 * @return string
 *   Page content.
 */
function travisbuilder_build_list_page() {
  drupal_set_title(t('Travis Build History'));
  $output = '<p>' . t('Recent Travis builds.') . '</p>';
  $output .= _travisbuilder_list_builds();

  return $output;
}


/**
 * Provide a list of recent builds for display.
 *
 * Resources:
 *   http://docs.travis-ci.com/api/#builds
 *   http://docs.travis-ci.com/api/#jobs
 *
 * @return array
 *   Renderable table array of recent builds.
 */
function _travisbuilder_list_builds() {
  $configs = _travisbuilder_configs();
  $client = travisbuilder_client($configs['repo']);
  $repo_client = $client->fetchRepository($configs['repo']);
  $url = ($configs['private']) ? TRAVISBUILDER_URL_PRIVATE : TRAVISBUILDER_URL;

  // Render array.
  $table = array(
    'header' => array(
      'build' => t('Build'),
      'status' => t('Status'),
      'message' => t('Message'),
      'commit' => t('Commit'),
      'time' => t('Completed'),
    ),
    'empty' =>  t('No builds available.'),
  );
  $rows = array();

  if ($repo_client) {
    // Collect content.
    $builds = $repo_client->getBuilds();
    foreach ($builds as $build) {
      $build_url = format_string($url . '/!repo/builds/!id', array(
        '!repo' => check_plain($configs['repo']),
        '!id' => $build->getId(),
      ));
      switch ($build->getResult()) {
        case 0:
          $status = t('Pass');
          break;
        case 1:
          $status = t('Fail');
          break;
        default:
          $status = t('N/A');
          break;
      }
      // Build table row.
      $rows[] = array(
        'build' => l('#' . $build->getNumber(), $build_url),
        'status' => $status,
        'message' => $build->getMessage(),
        'commit' => substr($build->getCommit(), 0, 10) . ' (' . $build->getBranch() . ')',
        'time' => $build->getFinishedAt()->format('m-d-Y H:i:s'),
      );
    }
    // Construct render array.
    $table['rows'] = $rows;
  }

  return theme('table', $table);
}

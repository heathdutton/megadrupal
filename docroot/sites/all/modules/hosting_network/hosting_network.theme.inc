<?php

/**
 * @file
 * Contains theme functions for hosting_network.
 */

function theme_hosting_network_platform_page($variables) {
  $platform = $variables['platform'];
  $sites = $variables['sites'];
  $tasks = $variables['tasks'];
  $server_nid = $variables['server_nid'];

  $server = node_load($server_nid);

  $html = '';
  $html .= '<p><h2>' . $platform->title . '</h2></p>';
  $html .= '<p><strong>Server:</strong> ' . $server->title . '</p>';
  $html .= '<div>';
  $html .= '<p><strong>Tasks</strong></p>';
  $html .= theme('hosting_network_task_list', array('tasks' => $tasks, 'server_nid' => $server_nid, 'target_node' => $platform, 'display_run_buttons' => TRUE));
  $html .= '</div>';
  $html .= '<div>';
  $html .= '<p><strong>Sites</strong></p>';
  $html .= theme('hosting_network_site_list', array('server_nid' => $server_nid, 'platform' => $platform, 'sites' => $sites));
  $html .= '<p><strong>Total sites: </strong> ' . count($sites) . '</p>';
  $html .= '</div>';

  return $html;

}

function theme_hosting_network_site_page($variables) {
  $platform = $variables['platform'];
  $site = $variables['site'];
  $tasks = $variables['tasks'];
  $server_nid = $variables['server_nid'];

  $server = node_load($server_nid);
  if (!empty($site->login_link)) {
    $link = ' <strong>[' . l('Log in', $site->login_link, array('attributes' => array('target'=>'_blank'))) . ']</strong>';
    $title = '<h2>' . $site->title . '</h2>' . $link;
  }
  else {
    $link = ' <strong>[' . l('Go to site', $site->title, array('attributes' => array('target'=>'_blank'))) . ']</strong>';
    $title = '<h2>' . $site->title . '</h2>' . $link;
  }

  $platform_link = l($platform->title, 'hosting_network/platform/' . $server_nid . '/' . $platform->nid);

  $html = '';
  $html .= '<p>' . $title . '</p>';
  $html .= '<p><strong>Platform:</strong> ' . $platform_link;
  $html .= '<br/><strong>Server:</strong> ' . $server->title . '</p>';
  $html .= '<div>';
  $html .= theme('hosting_network_task_list', array('tasks' => $tasks, 'server_nid' => $server_nid, 'target_node' => $site, 'display_run_buttons' => TRUE));
  $html .= '</div>';

  return $html;

}

function theme_hosting_network_site_list($variables) {
  $sites = $variables['sites'];
  $platform = $variables['platform'];
  $server_nid = $variables['server_nid'];

  $header = array(
    t('Site'),
    t('Profile'),
    t('Created'),
  );

  $rows[] = array();

  $total = 0;
  foreach ($sites as $node) {
    $row = array();
    $row[] = array(
      'class' => array('hosting-status'),
      'data' => '<a href="/hosting_network/site/' . $server_nid . '/' . $node->nid . '">' . $node->title . '</a>',
    );
    $row[] = $node->profile;
    $row[] = hosting_format_interval($node->created);
    $total += 1;
    $rows[] = array(
      'data' => $row,
      'class' => array(_hosting_site_list_class($node->site_status, $node->verified)),
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('hosting-table'))));
}

function theme_hosting_network_platform_list($variables) {
  $platforms = $variables['platforms'];
  $server_nid = $variables['server_nid'];
  $show_sites = variable_get('hosting_network_show_sites', TRUE);
  $max_sites = variable_get('hosting_network_max_sites_to_display', 10);

  $header = array(
    t('Platform'),
    t('Release'),
    t('Server'),
    t('Verified'),
    t('Sites'),
  );

  $rows[] = array();

  $total = 0;
  foreach ($platforms as $node) {
    $platform_link = '/hosting_network/platform/' . $server_nid . '/' . $node->nid;
    $row = array();
    $row[] = array(
      'class' => array('hosting-status'),
      'data' => '<a href="' . $platform_link . '"><strong>' . $node->title . '</strong></a>',
    );
    $version = !empty($node->release) ? $node->release->version : '';
    $title = !empty($node->release) ? $node->release->title : '';
    $release = sprintf("%s %s", $title, $version);
    $row[] = $release;
    $row[] = $node->web_server;
    $row[] = hosting_format_interval($node->verified);
    $total += count($node->sites);
    $row[] = count($node->sites);
    $rows[] = array(
      'data' => $row,
      'class' => array(_hosting_platform_list_class($node->platform_status)),
    );

    if ($show_sites) {
      $platform_total = 0;
      foreach($node->sites as $site) {
        if ($platform_total >= $max_sites) {
          continue;
        }
        $row = array();
        $row[] = '';
        $row[] = array(
          'class' => array('hosting-status'),
          'data' => '<a href="/hosting_network/site/' . $server_nid . '/' . $site->nid . '">' . $site->title . '</a>',
        );
        $row[] = t('Profile: '). $site->profile;
        $row[] = t('Created:') . hosting_format_interval($site->created);
        $row[] = '';
        $rows[] = array(
          'data' => $row,
          'class' => array(_hosting_site_list_class($site->site_status, $site->verified)),
        );
        $platform_total += 1;
        if ($platform_total >= $max_sites && count($node->sites) > $max_sites) {
          $row = array();
          $row[] = '';
          $link = '<a href="' . $platform_link . '"><strong>' . t('Details') . '</strong></a> (' . (count($node->sites) - $max_sites) . ' more sites)';
          $row[] = array(
            'data' => $link,
          );
          $rows[] = array(
            'data' => $row,
          );
        }
      }
    }
  }
  $row = array();
  $row[] = array(
    'data' => t('Total sites hosted'),
    'colspan' => 4,
  );
  $row[] = $total;
  $rows[] = $row;

  return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('class' => array('hosting-table'))));
}

function theme_hosting_network_task_list($variables) {
  $tasks = $variables['tasks'];
  $server_nid = $variables['server_nid'];
  $display_run_buttons = $variables['display_run_buttons'];
  $target = $variables['target_node'];

  $output = '';

  $headers[] = t('Task');
  $headers[] = array(
    'data' => t('Actions'),
    'class' => array('hosting-actions'),
  );

  $rows = array();

  if (empty($tasks)) {
    return '';
  }

  foreach ($tasks as $task => $info) {
    $row = array();

    $info = (array) $info;

    if (!isset($info['nid']) && !$info['task_permitted']) {
      // just don't show those tasks, since we'll not be able to run them
      continue;
    }

    $row['type'] = array(
      'data' => $info['title'],
      'class' => array('hosting-status'),
    );

    $actions = array();
    if ($display_run_buttons) {
      if (isset($info['task_status']) && ($info['task_status'] == 0)) {
        $actions['cancel'] = _hosting_task_button(t('Cancel'), '', t("Cancel the task and remove it from the queue"), 'hosting-button-stop', FALSE);
      }
      else {
        // server/node/target_type/task_type
        $path = sprintf("hosting_network/run/%s/%s/%s/%s", $server_nid, $target->nid, $target->type ,$task) ;
        $actions['run'] = _hosting_task_button(t('Run'), $path, '', 'hosting-button-run', $info['task_permitted'], FALSE, TRUE);
      }
    }

    $actions['log'] = _hosting_task_button(t('View'), isset($info['nid']) ? 'hosting_network/task/' . $server_nid . '/' . $info['nid'] : '<front>', t("Display the task log"), 'hosting-button-log', isset($info['nid']), TRUE, FALSE);
    $row['actions'] = array(
      'data' => implode('', $actions),
      'class' => array('hosting-actions'),
    );

    $rows[] = array(
      'data' => $row,
      'class' => array(hosting_task_status_class($info['task_status'])),
    );
  }
  $output .= theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('class' => array('hosting-table'))));
  return $output;
}

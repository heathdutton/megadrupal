<?php

function gdrush_drush_command() {
  $items['gdrush-get-panels'] = array(
    'description' => 'get panels from commands',
    'aliases' => array('gdgp'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'outputformat' => array(
      'default' => 'table',
    )
  );

  $items['gdrush-core'] = array(
    'description' => 'Gdrush Core Panels',
    'gui' => array(
      'path' => 'panels',
      'panels' => array(
        'dashboard' => array(
          'title' => 'Dashboard',
          'file' => 'dashboard.html'
        ),
        'modules' => array(
          'title' => 'Modules',
          'file' => 'modules.html'
        ),
        /*array(
          'title' => 'Servers & Migration',
          'file' => 'servers-migration.qml'
        ),*/
      ),
    ),
  );
  return $items;
}

function drush_gdrush_get_panels(){
  $commands = drush_get_commands();
  $panels = array();
  foreach($commands as $command_name => $command){
    if(isset($command['gui']['panels'])){
      $panels_path = isset($command['gui']['path']) ? $command['gui']['path'] : '';
      foreach($command['gui']['panels'] as $id => $panel){
        $panel_info['id'] = $id;
        $panel_info['title'] = $panel['title'];
        $panel_path = $command['path'] . '/' . $panels_path . '/' . $command['command'] . '--' . $panel['file'];
        $panel_info['html'] = file_get_contents($panel_path);
        $panels[] = $panel_info;
      }
    }
  }
  return $panels;
}
<?php
// $Id: devel_info_php_info.inc,v 1.1.2.1 2010/07/14 11:51:51 flanker Exp $

/**
 * @file
 *   php_info block functions for the devel_info module.
 */


/**
 * Block help text.
 */
function devel_info_php_info_block_help() {

  $output = '<p>'. t('This block displays information about the current state of PHP.') .'</p>';

  return $output;
}


/**
 * Block description.
 */
function devel_info_php_info_block_info() {

  $block = array(
    'info' => t('DI: PHP Info'),
    'cache' => DRUPAL_NO_CACHE,
  );

  return $block;
}


/**
 * Block configuration form.
 */
function devel_info_php_info_block_configure() {

  $config = devel_info_php_info_get_variable();

  $form = array();
  $form['php_info_block_behavior'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Block behavior'),
    '#description' => t('Configure block behavior.'),
    '#default_value' => array(
      $config['block_content'],
      $config['block_footer'],
    ),
    '#options' => array(
      'block_content' => t('Show block content.'),
      'block_footer' => t('Show block footer links.'),
    ),
  );
  $form['block_content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block content'),
  );
  // PHP
  $form['block_content']['php_info_php'] = array(
    '#type' => 'checkboxes',
    '#title' => t('PHP'),
    '#description' => t('Display PHP related information.'),
    '#default_value' => array(
      $config['version'],
      $config['max_execution_time'],
      $config['max_input_time'],
      $config['memory_limit'],
      $config['post_max_size'],
      $config['register_globals'],
      $config['upload_max_filesize'],
    ),
    '#options' => array(
      'version' => t('Display PHP version. %value', array('%value' => 'phpversion()')),
      'max_execution_time' => t('Display PHP max execution time. %value', array('%value' => "ini_get('max_execution_time')")),
      'max_input_time' => t('Display PHP max input time. %value', array('%value' => "ini_get('max_input_time')")),
      'memory_limit' => t('Display PHP memory limit. %value', array('%value' => "ini_get('memory_limit')")),
      'post_max_size' => t('Display PHP post max size. %value', array('%value' => "ini_get('post_max_size')")),
      'register_globals' => t('Display PHP register globals. %value', array('%value' => "ini_get('register_globals')")),
      'upload_max_filesize' => t('Display PHP upload max filesize. %value', array('%value' => "ini_get('upload_max_filesize')")),
    ),
  );
  // PHP extension
  $form['block_content']['php_info_php_extension'] = array(
    '#type' => 'checkboxes',
    '#title' => t('PHP extension'),
    '#description' => t('Display PHP extension related information.'),
    '#default_value' => array(
      $config['extension_curl'],
      $config['extension_date'],
      $config['extension_dom'],
      $config['extension_filter'],
      $config['extension_gd'],
      $config['extension_hash'],
      $config['extension_json'],
      $config['extension_pcre'],
      $config['extension_pdo'],
      $config['extension_session'],
      $config['extension_simplexml'],
      $config['extension_spl'],
      $config['extension_xml'],
    ),
    '#options' => array(
      'extension_curl' => t('Display if extension %extension is loaded. %value', array('%extension' => 'curl', '%value' => "extension_loaded('curl')")),
      'extension_date' => t('Display if extension %extension is loaded. %value', array('%extension' => 'date', '%value' => "extension_loaded('date')")),
      'extension_dom' => t('Display if extension %extension is loaded. %value', array('%extension' => 'dom', '%value' => "extension_loaded('dom')")),
      'extension_filter' => t('Display if extension %extension is loaded. %value', array('%extension' => 'filter', '%value' => "extension_loaded('filter')")),
      'extension_gd' => t('Display if extension %extension is loaded. %value', array('%extension' => 'gd', '%value' => "extension_loaded('gd')")),
      'extension_hash' => t('Display if extension %extension is loaded. %value', array('%extension' => 'hash', '%value' => "extension_loaded('hash')")),
      'extension_json' => t('Display if extension %extension is loaded. %value', array('%extension' => 'json', '%value' => "extension_loaded('json')")),
      'extension_pcre' => t('Display if extension %extension is loaded. %value', array('%extension' => 'pcre', '%value' => "extension_loaded('pcre')")),
      'extension_pdo' => t('Display if extension %extension is loaded. %value', array('%extension' => 'PDO', '%value' => "extension_loaded('pdo')")),
      'extension_session' => t('Display if extension %extension is loaded. %value', array('%extension' => 'session', '%value' => "extension_loaded('session')")),
      'extension_simplexml' => t('Display if extension %extension is loaded. %value', array('%extension' => 'SimpleXML', '%value' => "extension_loaded('SimpleXML')")),
      'extension_spl' => t('Display if extension %extension is loaded. %value', array('%extension' => 'SPL', '%value' => "extension_loaded('SPL')")),
      'extension_xml' => t('Display if extension %extension is loaded. %value', array('%extension' => 'xml', '%value' => "extension_loaded('xml')")),
    ),
  );

  return $form;
}


/**
 * Block configuration saving.
 */
function devel_info_php_info_block_save($edit) {

  $var = array();
  $var['block_content'] = $edit['php_info_block_behavior']['block_content'];
  $var['block_footer'] = $edit['php_info_block_behavior']['block_footer'];
  // PHP
  $var['version'] = $edit['php_info_php']['version'];
  $var['max_execution_time'] = $edit['php_info_php']['max_execution_time'];
  $var['max_input_time'] = $edit['php_info_php']['max_input_time'];
  $var['memory_limit'] = $edit['php_info_php']['memory_limit'];
  $var['post_max_size'] = $edit['php_info_php']['post_max_size'];
  $var['register_globals'] = $edit['php_info_php']['register_globals'];
  $var['upload_max_filesize'] = $edit['php_info_php']['upload_max_filesize'];
  // PHP extension
  $var['extension_curl'] = $edit['php_info_php_extension']['extension_curl'];
  $var['extension_date'] = $edit['php_info_php_extension']['extension_date'];
  $var['extension_dom'] = $edit['php_info_php_extension']['extension_dom'];
  $var['extension_filter'] = $edit['php_info_php_extension']['extension_filter'];
  $var['extension_gd'] = $edit['php_info_php_extension']['extension_gd'];
  $var['extension_hash'] = $edit['php_info_php_extension']['extension_hash'];
  $var['extension_json'] = $edit['php_info_php_extension']['extension_json'];
  $var['extension_pcre'] = $edit['php_info_php_extension']['extension_pcre'];
  $var['extension_pdo'] = $edit['php_info_php_extension']['extension_pdo'];
  $var['extension_session'] = $edit['php_info_php_extension']['extension_session'];
  $var['extension_simplexml'] = $edit['php_info_php_extension']['extension_simplexml'];
  $var['extension_spl'] = $edit['php_info_php_extension']['extension_spl'];
  $var['extension_xml'] = $edit['php_info_php_extension']['extension_xml'];

  variable_set('devel_info_php_info', serialize($var));
}


/**
 * Block content.
 */
function devel_info_php_info_block_view() {

  $config = devel_info_php_info_get_variable();

  if ($config['block_content']) {

    $item = array();
    $item_ext = array();

    // PHP
    if ($config['version']) {
      $item[] = t('Version: %value', array('%value' => phpversion()));
    }
    if ($config['max_execution_time']) {
      $item[] = t('Max execution time: %value', array('%value' => ini_get('max_execution_time')));
    }
    if ($config['max_input_time']) {
      $item[] = t('Max input time: %value', array('%value' => ini_get('max_input_time')));
    }
    if ($config['memory_limit']) {
      $item[] = t('Memory limit: %value', array('%value' => ini_get('memory_limit')));
    }
    if ($config['post_max_size']) {
      $item[] = t('Post max size: %value', array('%value' => ini_get('post_max_size')));
    }
    if ($config['register_globals']) {
      $item[] = t('Register globals: %value', array('%value' => ini_get('register_globals')));
    }
    if ($config['upload_max_filesize']) {
      $item[] = t('Upload max filesize: %value', array('%value' => ini_get('upload_max_filesize')));
    }

    // PHP extension
    if ($config['extension_curl']) {
      $item_ext[] = t('curl: %value', array('%value' => extension_loaded('curl')));
    }
    if ($config['extension_date']) {
      $item_ext[] = t('date: %value', array('%value' => extension_loaded('date')));
    }
    if ($config['extension_dom']) {
      $item_ext[] = t('dom: %value', array('%value' => extension_loaded('dom')));
    }
    if ($config['extension_filter']) {
      $item_ext[] = t('filter: %value', array('%value' => extension_loaded('filter')));
    }
    if ($config['extension_gd']) {
      $item_ext[] = t('gd: %value', array('%value' => extension_loaded('gd')));
    }
    if ($config['extension_hash']) {
      $item_ext[] = t('hash: %value', array('%value' => extension_loaded('hash')));
    }
    if ($config['extension_json']) {
      $item_ext[] = t('json: %value', array('%value' => extension_loaded('json')));
    }
    if ($config['extension_pcre']) {
      $item_ext[] = t('pcre: %value', array('%value' => extension_loaded('pcre')));
    }
    if ($config['extension_pdo']) {
      $item_ext[] = t('PDO: %value', array('%value' => extension_loaded('PDO')));
    }
    if ($config['extension_session']) {
      $item_ext[] = t('session: %value', array('%value' => extension_loaded('session')));
    }
    if ($config['extension_simplexml']) {
      $item_ext[] = t('SimpleXML: %value', array('%value' => extension_loaded('SimpleXML')));
    }
    if ($config['extension_spl']) {
      $item_ext[] = t('SPL: %value', array('%value' => extension_loaded('SPL')));
    }
    if ($config['extension_xml']) {
      $item_ext[] = t('xml: %value', array('%value' => extension_loaded('xml')));
    }
  }

  $block = array();
  $block['subject'] = t('DI: PHP Info');
  $block['content'] = !empty($item) ? theme('item_list', array('items' => $item)) : '';
  $block['content'] .= !empty($item_ext) ? theme('item_list', array('items' => $item_ext, 'title' => t('PHP extension'))) : '';
  $block['content'] .= devel_info_get_block_link('php_info', $config);

  return $block;
}


/**
 * Block behavior.
 */
function devel_info_php_info_block_behavior($action, $mode) {

  if ($action == 'block_content') {

    $var = devel_info_php_info_get_variable();
    $var['block_content'] = $mode == 'show' ? 'block_content' : 0;

    variable_set('devel_info_php_info', serialize($var));
  }
}


/**
 * Block uninstall.
 */
function devel_info_php_info_block_uninstall() {

  variable_del('devel_info_php_info');
}


/**
 * Helper function; block configuration options.
 */
function devel_info_php_info_get_variable() {

  $default = array();
  $default['block_content'] = 'block_content';
  $default['block_footer'] = 'block_footer';
  // PHP
  $default['version'] = 'version';
  $default['max_execution_time'] = 'max_execution_time';
  $default['max_input_time'] = 'max_input_time';
  $default['memory_limit'] = 'memory_limit';
  $default['post_max_size'] = 'post_max_size';
  $default['register_globals'] = 'register_globals';
  $default['upload_max_filesize'] = 'upload_max_filesize';
  // PHP extension
  $default['extension_curl'] = 0;
  $default['extension_date'] = 0;
  $default['extension_dom'] = 0;
  $default['extension_filter'] = 0;
  $default['extension_gd'] = 0;
  $default['extension_hash'] = 0;
  $default['extension_json'] = 0;
  $default['extension_pcre'] = 0;
  $default['extension_pdo'] = 0;
  $default['extension_session'] = 0;
  $default['extension_simplexml'] = 0;
  $default['extension_spl'] = 0;
  $default['extension_xml'] = 0;

  $custom = variable_get('devel_info_php_info', array());

  return !empty($custom) ? array_merge($default, unserialize($custom)) : $default;
}

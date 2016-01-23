<?php

/**
 *
 * @file
 * I know the design is stupid, Guilty as charged.
 */

function _oven_drush_require() {
  $here = explode('/', dirname(__FILE__));
  array_pop($here);
  require_once implode('/', $here) . '/oven.module';
}
_oven_drush_require();

function oven_drush_command() {
  $items['oven-cook'] = [
    'aliases' => ['cook'],
    'callback' => 'oven_drush_cook',
    'description' => 'Create a module containing an entity (Cook an entity type).',
    'arguments' => ['machine_name' => 'Machine name of generated entity.'],
    'options' => [],
    'examples' => [],
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL,
  ];

  foreach (__OvenTokenInfoName::getTokensObject(FALSE) as $token) {
    $name = $token->get('name');
    $description = $token->get('description');
    $items['oven-cook']['options'][$name] = $description;
  }

  return $items;
}

function oven_drush_help($section) {
  // @TODO
}

function __oven_log_drush_err($err_no) {
  $msg = __oven_error_msg($err_no);
  drush_log($msg, 'error');
}

function __oven_set_drush_err($err_no) {
  $msg = __oven_error_msg($err_no);
  drush_set_error($msg);
}

function _oven_drush_generate_handle_machine_name($machine_name) {
  $machine = new __OvenTokenInfoName('machine', $machine_name);
  if($err = $machine->validate()) {
    __oven_set_drush_err($err);
    return FALSE;
  }
  else {
    __OvenMachineName::set($machine_name);
    return $machine_name;
  }
}

function _oven_drush_generate_handle_arguments($machine_name) {
  $v = [];
  $all_tokens = __OvenTokenInfoName::getTokensObject(FALSE);

  foreach (__OvenTokenInfoName::getTokensObject(FALSE) as $token) {
    if($value = drush_get_option($token->get('name'), NULL)) {
      $token->set($value);
      if($err = $token->validate()) {
        __oven_set_drush_err($err);
        return FALSE;
      }
      else {
        $v[$token->get('name')] = $token;
        unset($all_tokens[$token->get('name')]);
      }
    }
  }

  foreach ($all_tokens as $token) {
    while(TRUE) {
      $default = $token->defaultValue();
      $prompt = $token->get('label') . " (h:help)";
      $value = drush_prompt($prompt, $default, TRUE, FALSE);
      if ($value == 'h') {
        drush_log($token->get('description') . "\n", 'ok');
      }
      else {
        $token->set($value);
        if($err = $token->validate()) {
          __oven_log_drush_err($err);
        }
        else {
          break;
        }
      }
    };

    $token->processValue();
    $v[$token->get('name')] = $token;
  }

  // No option was specified so no error anyway.
  return $v;
}

/**
 * Is not the name lovely? Cooks a new entity type based on entityspice.
 */
function oven_drush_cook($machine_name=NULL) {
  if(!$machine_name = _oven_drush_generate_handle_machine_name($machine_name)) {
    drush_set_error('Invalid machine name given');
    return;
  }

  $ok = drush_confirm('Do you want to use default options?');
  if($ok) {
    $tokens = _oven_default_arguments($machine_name);
  }
  elseif(!$values = _oven_drush_generate_handle_arguments($machine_name)) {
    drush_log('Enter "h" to see each arguments help, leave empty for default value.' . "\n", 'ok');
    return;
  }

  $tmp = drush_tempdir();
  list(, ,$src) = _oven_ekran($tmp, $tokens, $machine_name);

  // @todo move to custom folder
  $dest = _oven_pm_download_destination('module');
  $fulldest = $dest . '/' . $machine_name;

  // Check if install location already exists.
  if (is_dir($fulldest)) {
    if (drush_confirm(dt('Install location !location already exists. Do you want to overwrite it?', array('!location' => $fulldest)))) {
      drush_delete_dir($fulldest, TRUE);
      $skip = FALSE;
    }
    else {
      drush_log(dt("Skip installation of !project to !dest.", array('!project' => $machine_name, '!dest' => $fulldest)), 'warning');
      $skip = TRUE;
    }
  }

  if(!$skip) {
    drush_op('_drush_recursive_copy', $src, $fulldest);
  }

  drush_log(dt("removing tmp dir !dir skipped. too lazy to do it, you do it!", array('!dir' => $src)), 'warning');
}

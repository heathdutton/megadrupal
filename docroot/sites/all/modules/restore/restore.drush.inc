<?php
/**
 * @file
 * Drush commands for the restore module.
 */

/**
 * Implements hook_drush_command().
 */
function restore_drush_command() {
  $items = array();

  // Drush command to display a list of scripts, and operations.
  $items['restore-list'] = array(
    'description' => 'Displays a list of all the available restore scripts with a status.',
    'callback arguments' => array(array(), FALSE),
    'options' => array(
      'operations' => array(
        'description' => 'List the script operations and their statuses.',
      ),
      'summary' => array(
        'description' => 'Display the summary of each operation.',
      ),
      'machine' => array(
        'description' => 'Display the output in a machine readable format.',
      ),
    ),
    'aliases' => array('rsl'),
  );

  // Drush command to display the status of a single restore script.
  $items['restore-status'] = array(
    'description' => 'Displays the status of the provided restore script.',
    'callback arguments' => array(array(), FALSE),
    'options' => array(
      'script' => array(
        'description' => 'The restore scripts name/id.',
      ),
      'operations' => array(
        'description' => 'List the script operations and their statuses.',
      ),
      'summary' => array(
        'description' => 'Display the summary of each operation.',
      ),
      'machine' => array(
        'description' => 'Display the output in a machine readable format.',
      ),
    ),
    'aliases' => array('rss'),
  );

  // Drush command to revert all scripts.
  $items['restore-revert-all'] = array(
    'description' => 'Revert all restore scripts.',
    'callback arguments' => array(array(), FALSE),
    'options' => array(),
    'aliases' => array('rsra'),
  );

  // Drush command to revert a specific script.
  $items['restore-revert'] = array(
    'description' => 'Output a report.',
    'callback arguments' => array(array(), FALSE),
    'options' => array(
      'script' => array(
        'description' => 'The restore scripts name/id.',
      ),
      'operations' => array(
        'description' => 'A comma seperated list of specific operations to revert.',
      ),
    ),
    'aliases' => array('rsr'),
  );

  return $items;
}

/**
 * Helper function to style the text.
 */
function _drush_restore_print_colour($text, $colour, $background = null) {
  $colours = array(
    "fg" => array(
      "black" => "0;30", "dark-gray" => "1;30",
      "blue" => "0;34", "light-blue" => "1;34",
      "green" => "0;32", "light-green" => "1;32",
      "cyan" => "0;36", "light-cyan"=> "1;36",
      "red" => "0;31", "light-red" => "1;31",
      "purple" => "0;35", "light-purple" => "1;35",
      "brown" => "0;33", "yellow" => "1;33",
      "light-gray" => "0;37", "white" => "1;37",
    ),
    "bg" => [
      "black" => "40", "red" => "41",
      "green" => "42", "yellow" => "43",
      "blue" => "44", "magenta" => "45",
      "cyan" => "46", "light-gray" => "47",
    ],
  );

  $fg = "";
  $bg = "";
  $e = "";
  if (isset($colours["fg"][$colour])) {
    $fg = "\033[{$colours["fg"][$colour]}m";
    $fge = "\033[0m";
  }

  if (isset($colours["bg"][$background])) {
    $bg = "\033[{$colours["bg"][$background]}m";
    $bge = "\033[0m";
  }

  return "{$fg}{$bg}{$text}{$bge}{$fge}";
}

/**
 * Helper function to output the colour legend.
 */
function _drush_restore_legend() {
  return array(
    'raw' => ' ' . dt('unknown') . ' ' .
      ' ' . dt('error') . ' ' .
      ' ' . dt('active') . ' ' .
      ' ' . dt('overriden') . ' ' .
      ' ' . dt('missing') . ' ' .
      ' ' . dt('conflicts') . ' ',
    'formatted' => _drush_restore_print_colour(' ' . dt('unknown') . ' ', "white", "red") .
      _drush_restore_print_colour(' ' . dt('error') . ' ', "white", "red") .
      _drush_restore_print_colour(' ' . dt('active') . ' ', "white", "green") .
      _drush_restore_print_colour(' ' . dt('overridden') . ' ', "black", "magenta") .
      _drush_restore_print_colour(' ' . dt('missing') . ' ', "black", "light-gray") .
      _drush_restore_print_colour(' ' . dt('conflicts') . ' ', "black", "yellow"),
  );
}

/**
 * Helper function to get the status of a script or an operation.
 */
function _drush_restore_status($obj, $human_readble = true) {
  if ($obj === FALSE) {
    // If the $obj is FALSE, usually caused by a failed operation load.
    return _drush_restore_print_colour(dt('unknown'), "white", "red");
  }

  // Declare the status details.
  $statuses = array(
    RestoreScript::STATUS_ACTIVE => array(
      'fg' => 'white',
      'bg' => 'green',
      'label' => dt('active'),
    ),
    RestoreScript::STATUS_OVERRIDEN => array(
      'fg' => 'black',
      'bg' => 'magenta',
      'label' => dt('overriden'),
    ),
    RestoreScript::STATUS_MISSING => array(
      'fg' => 'black',
      'bg' => 'light-gray',
      'label' => dt('missing'),
    ),
    RestoreScript::STATUS_ERROR => array(
      'fg' => 'white',
      'bg' => 'red',
      'label' => dt('error'),
    ),
  );

  $status = $obj->status();

  $no_conflicts = 0;
  if ($obj instanceof RestoreScript) {
    $conflicts = $obj->conflicts();
    if ($conflicts) {
      foreach ($conflicts as $script) {
        foreach ($script['operations'] as $operation) {
          if ($operation['conflict'] === TRUE) {
            $no_conflicts++;
          }
          else {
            $no_conflicts += count($operation['conflict']);
          }
        }
      }
    }
  }

  if ($obj->total() + $no_conflicts != $obj->noActive()) {
    $result = array();

    // The status value.
    $result["raw"][] = $human_readble ?
      " {$statuses[$status]['label']} " :
      "[status: {$statuses[$status]['label']}]";
    $result["formatted"][] = _drush_restore_print_colour(" {$statuses[$status]['label']} ", $statuses[$status]['fg'], $statuses[$status]['bg']);
    

    // Display the number of active states.
    if ($obj->noActive() > 0) {
      $result["raw"][] = $human_readble ?
        " {$obj->noActive()} " :
        "[active: {$obj->noActive()}]";
      $result["formatted"][] = _drush_restore_print_colour(" {$obj->noActive()} ", "white", "green");
    }

    // Display the number of overriden states.
    if ($obj->noOverriden() > 0) {
      $result["raw"][] = $human_readble ?
        " {$obj->noOverriden()} " :
        "[overriden: {$obj->noOverriden()}]";
      $result["formatted"][] = _drush_restore_print_colour(" {$obj->noOverriden()} ", "black", "magenta");
    }

    // Display the number of missing states.
    if ($obj->noMissing() > 0) {
      $result["raw"][] = $human_readble ?
        " {$obj->noMissing()} " :
        "[missing: {$obj->noMissing()}]";
      $result["formatted"][] = _drush_restore_print_colour(" {$obj->noMissing()} ", "black", "light-gray");
    }

    // Display the number of conflicts.
    if ($no_conflicts > 0) {
      $result["raw"][] = $human_readble ?
        " {$no_conflicts} " :
        "[conflict: {$no_conflicts}]";
      $result["formatted"][] = _drush_restore_print_colour(" {$no_conflicts} ", "black", "yellow");
    }

    return array(
      "raw" => implode("", $result["raw"]),
      "formatted" => implode("", $result["formatted"]),
    );
  }

  $result["raw"] = $human_readble ?
    " {$statuses[$status]['label']} " :
    "[status: {$statuses[$status]['label']}]";
  $result["formatted"] = _drush_restore_print_colour(" {$statuses[$status]['label']} ", $statuses[$status]['fg'], $statuses[$status]['bg']);
  
  return $result;
}

/**
 * A helper function to get the screen width.
 */
function _drush_restore_screen_width() {
  $screen_width = &drupal_static(__FUNCTION__);
  if (!isset($screen_width)) {
    $screen_width = (int) exec("tput cols");
  }

  return $screen_width;
}

/**
 * A helper function to display the status of a script.
 */
function _drush_restore_script_status($script, $display_operations, $display_summary, $human_readable) {
  $screen_width = _drush_restore_screen_width();
  $status_width = 30;

  $script_status = _drush_restore_status($script, $human_readable);
  if ($human_readable) {
    $line = str_pad("  {$name}", $screen_width - $status_width, ' ', STR_PAD_RIGHT);
    $status_len = strlen($script_status['raw']);
    $status = str_pad('', $status_width - $status_len, ' ') . $script_status['formatted'];
    drush_print($line . $status);
  }
  else {
    drush_print("[sc] {$name} - {$script_status['raw']}");
  }

  if ($display_operations || $display_summary) {
    foreach ($script as $idx => $operation) {
      $op_status = _drush_restore_status($operation, $human_readable);

      if ($human_readable) {
        $op_line = str_pad("    [{$idx}] " . ($operation ? $operation->title() : t('Unknown')), $screen_width - $status_width, ' ');

        $status_len = strlen($op_status['raw']);

        $status = str_pad('', $status_width - $status_len - 2, ' ') . $op_status['formatted'];
        drush_print($op_line . $status);
      }
      else {
        drush_print("  [op] [index: {$idx}] " . ($operation ? $operation->title() : t('Unknown')) . " {$op_status['raw']}");
      }

      if ($display_summary) {
        $summary = $operation ? $operation->summary(true) : array();

        if ($human_readable) {
          $summary_line = '          ';
          $len = strlen($summary_line);

          $keys = array_keys($summary);
          $last = end($keys);
          foreach ($summary as $key => $details) {
            if ($len + strlen($details['label']) + 2 > $screen_width) {
              drush_print($summary_line);
              $summary_line = '          ';
              $len = strlen($summary_line);
            }
            else {
              $len += strlen($details['label']) + 2;
              if ($details['status'] == 'active') {
                $summary_line .= _drush_restore_print_colour(" {$details['label']} ", "white", "green");
              }
              elseif ($details['status'] == 'overriden') {
                $summary_line .= _drush_restore_print_colour(" {$details['label']} ", "black", "magenta");
              }
              else {
                $summary_line .= _drush_restore_print_colour(" {$details['label']} ", "black", "light-gray");
              }
            }

            if ($last == $key) {
              drush_print($summary_line);
            }
          }
        }
        else {
          foreach ($summary as $key => $details) {
            drush_print("    [sm] {$details['label']} [status: {$details['status']}]");
          }
        }
      }
    }
  }
}

/**
 * The callback for the 'restore-list' drush command.
 */
function drush_restore_list() {
  $scripts = restore_scripts();
  if ($scripts && count($scripts)) {
    $display_operations = drush_get_option('operations', NULL) !== NULL;
    $display_summary = drush_get_option('summary', NULL) !== NULL;
    $human_readable = drush_get_option('machine', NULL) === NULL;

    foreach ($scripts as $name => $script) {
      _drush_restore_script_status($script, $display_operations, $display_summary, $human_readable);
    }

    if ($human_readable) {
      $screen_width = _drush_restore_screen_width();

      $legend = _drush_restore_legend();
      drush_print(str_pad('', $screen_width - strlen($legend["raw"]) - 1, ' ') . $legend["formatted"]);
    }
  }
  else {
    return drush_log(dt('There are no restore scripts available.'), 'notice');
  }
}

/**
 * The callback for the 'restore-status' drush command.
 */
function drush_restore_status() {
  $display_operations = drush_get_option('operations', NULL) !== NULL;
  $display_summary = drush_get_option('summary', NULL) !== NULL;
  $human_readable = drush_get_option('machine', NULL) === NULL;

  $name = drush_get_option('script', NULL);
  $script = restore_load($name);
  if ($script) {
    _drush_restore_script_status($script, $display_operations, $display_summary, $human_readable);
    if ($human_readable) {
      $screen_width = _drush_restore_screen_width();
      
      $legend = _drush_restore_legend();
      drush_print(str_pad('', $screen_width - strlen($legend["raw"]) - 1, ' ') . $legend["formatted"]);
    }
  }
  else {
    drush_set_error(DRUSH_APPLICATION_ERROR, dt('Unable to find the restore script "@name".', array('@name' => $name)));
  }
}

/**
 * The callback for the 'restore-revert-all' drush command.
 */
function drush_restore_revert_all() {
  $scripts = restore_scripts();
  if ($scripts && count($scripts)) {
    $screen_width = _drush_restore_screen_width();

    foreach ($scripts as $key => $script) {
      try {
        $script->restore();
        drush_print(
          str_pad("  {$key}", $screen_width - strlen(' [ success ]'), ' ') .
          ' [ ' . _drush_restore_print_colour('success', 'light-green') . ' ]'
        );
      } catch (Exception $e) {
        drush_print(
          str_pad("  {$key}", $screen_width - strlen(' [ error ]'), ' ') .
          ' [ ' . _drush_restore_print_colour('error', 'red') . ' ]'
        );
        
        drush_print("  " . $e->getMessage());
      }
    }
  }
  else {
    return drush_log(dt('There are no restore scripts available.'), 'notice');
  }
}

/**
 * The callback for the 'restore-revert' drush command.
 */
function drush_restore_revert() {
  $name = drush_get_option('script', NULL);
  $operations_value = drush_get_option('operations', NULL);
  $operations = $operations_value !== null ? explode(',', $operations) : array();

  $script = restore_load($name);
  if ($script) {
    $screen_width = _drush_restore_screen_width();
    
    try {
      $script->restore($operations);
      drush_print(
        str_pad("  {$name}", $screen_width - strlen(' [ success ]'), ' ') .
        ' [ ' . _drush_restore_print_colour('success', 'light-green') . ' ]'
      );
    } catch (Exception $e) {
      drush_print(
        str_pad("  {$name}", $screen_width - strlen(' [ error ]'), ' ') .
        ' [ ' . _drush_restore_print_colour('error', 'red') . ' ]'
      );

      drush_print("  " . $e->getMessage());
    }
  }
  else {
    drush_set_error(DRUSH_APPLICATION_ERROR, dt('Unable to find the restore script "@name".', array('@name' => $name)));
  }
}

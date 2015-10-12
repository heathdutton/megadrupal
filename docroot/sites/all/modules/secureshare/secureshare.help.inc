<?php

/**
 * Implementation of hook_help()
 *
 * @param $path
 * @param $arg
 * @return string
 */
function secureshare_help($path, $arg) {
  switch ($path) {
    case 'admin/help/#secureshare':
      return '<p>' . t('#todo help...') . '</p>';
      break;
  }

}
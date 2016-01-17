<?php

function crush_command_eval($parsed_command) {
  ob_start();
  print eval(implode('', $parsed_command['args']));
  $output = ob_get_contents();
  ob_end_clean();
  return array($output);
}


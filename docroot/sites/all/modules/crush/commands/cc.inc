<?php

function crush_command_cc($parsed_command) {
  drupal_flush_all_caches();
  return 'success';
}


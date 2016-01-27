<?php

function crush_command_cron($parsed_command) {
  drupal_cron_run();
  return 'success';
}


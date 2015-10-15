<?php

function private_innovation_drush_command() {
  $commands = array();
  $commands['private-innovation'] = array(
    'aliases' => array('pi'),
    'description' => t("Counts the number of lines of private innovation."),
  );
  return $commands;
}

function drush_private_innovation() {
  $results = private_innovation_results();
  $totals = private_innovation_totals($results);
  echo t("PHP:   ") . sprintf('%8s', number_format($totals['php'])) . "\n";
  echo t("CSS:   ") . sprintf('%8s', number_format($totals['css'])) . "\n";
  echo t("JS:    ") . sprintf('%8s', number_format($totals['js'])) . "\n";
  echo t("Total: ") . sprintf('%8s', number_format($totals['total'])) . "\n";
}

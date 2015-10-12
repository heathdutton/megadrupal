<?php

namespace Drupal\api_webhook;

use Symfony\Component\Process\Process;

class Hook {

  private $project;
  private $branch;

  function __construct($branch) {
    $this->branch = $branch;
    $this->project = db_select('api_project', 'p')
      ->fields('p')
      ->condition('project_name', $branch->project)
      ->execute()
      ->fetchObject();
  }

  public function execute() {
    $this->updateSourceCode();
    $this->reparseSourceCode();
  }

  private function updateSourceCode() {
    $destinations = explode(PHP_EOL, $this->branch->directories);
    foreach (explode(PHP_EOL, $this->branch->download_url) as $i => $url) {
      if (($url = trim($url)) && ('-' !== $url) && !empty($destinations[$i])) {
        $this->updateSourceCodeItem($url, $destinations[$i]);
      }
    }
  }

  private function updateSourceCodeItem($url, $destination) {
    $base_dir = dirname($destination);
    $file_name = basename($url);
    $commands[] = sprintf('rm -rf %s', $destination);
    $commands[] = sprintf('mkdir -p %s', $base_dir);
    $commands[] = sprintf('wget %s -O %s/%s', $url, $base_dir, $file_name);
    $commands[] = sprintf('tar zxf %s/%s -C %s', $base_dir, $file_name, $base_dir);
    $commands[] = sprintf('rm -f %s/%s', $base_dir, $file_name);
    foreach ($commands as $cmd) {
      drush_print_r($cmd);
      (new Process($cmd))->run();
    }
  }

  private function reparseSourceCode() {
    // Mark the files for reparsing.
    api_mark_for_reparse($this->branch->branch_id) > 0 ? 'status' : 'error';

    // Mark branch for update.
    $this->branch->last_updated = 0;
    api_save_branch($this->branch);
  }
}

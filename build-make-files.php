<?php
/**
 * Download all drupal.org projects and create make scripts to assemble them all
 * at https://www.drupal.org/about-drupalorg/api
 */

require_once('Requests.php');

if (isset($argv[1]) && $argv[1]){
  $cores = array($argv[1]);
} else {
  $cores = array(7, 8);
}

$project_types = array(
  'project_module',
  'project_theme',
  // 'project_theme_engine'
);

/**
 * Asynchronously check projects for minimum requirements for a drush make
 * to run correctly.
 *
 * @param $data
 * @param $info
 */
$release_history_callback = function($data, $info) {
  global $file, $project_type;
  $url_segments = explode('/', $info['url']);
  $project = $url_segments[4];
  if (
    !strpos($data, '<error>No release history available') // Must have a release
    // && strpos($data, '<type>') // Not required since we'll be mandating the type, allows some to squeeze through.
    && strpos($data, '<recommended_major>') // Recommended major is how drush determines if there is a stable release or not.
    && strpos($data, '<status>published</status>') // Must have a published release
  ){
    $type = str_replace('project_', '', $project_type);
    fwrite($file, 'projects[' . $project . '][type] = ' . $type . PHP_EOL);
    print '    ' . $project . ' ✓' . PHP_EOL;
  } else {
    print '    ' . $project . ' ✗' . PHP_EOL;
  }
};

foreach ($cores as $core){
  $header = <<<EOT
; This contains all projects of Drupal $core available by the drupal.org API.
; This could be useful for mirroring, statistical analysis,
; and high-availability make/build processes.
; This make script assumes we always desire the latest version of all projects,
; and will be updated periodically with new projects.

api = 2
core = $core.x

projects[drupal][type] = core
projects[drupal][version] = $core.x

EOT;
  $file_name = 'drupal-org-' . $core . '.make';
  if (file_exists($file_name)){
    unlink($file_name);
  }
  $file = fopen($file_name, 'w');
  fwrite($file, $header);
  foreach ($project_types as $project_type){
    fwrite($file, PHP_EOL . "; All projects of type: $project_type" . PHP_EOL . PHP_EOL);

    $url = 'https://www.drupal.org/api-d' . $core .'/node.json?type=' . $project_type;
    do {
      print $url . PHP_EOL;
      $options = array(
        'http' => array(
          'user_agent' => 'Creating code-search mirror for https://github.com/heathdutton',
          'header' => 'Accept: application/json',
        ),
      );
      $context = stream_context_create($options);
      $response = file_get_contents($url, false, $context);
      $data = json_decode($response);
      $project_block = '';
      if ($data->list){
        $release_history_urls = array();
        foreach ($data->list as $project) {
          // Only get full projects that have been downloaded
          if ($project->field_project_type == 'full'
            && $project->field_download_count > 0
            && $project->field_project_has_releases == TRUE
            && !is_numeric($project->field_project_machine_name)
          ) {

            // Check that the module has a release history (otherwise drush make cannot work with it)
            $release_history_urls[] = 'https://updates.drupal.org/release-history/' . $project->field_project_machine_name . '/' . $core . '.x';
          }
        }

        $requests = new Requests();
        $requests->process($release_history_urls, $release_history_callback);

      }

      $url = isset($data->next) ? str_replace('node?', 'node.json?', $data->next) : FALSE;
      print PHP_EOL;
    } while ($url);

  }
  fclose($file);
}

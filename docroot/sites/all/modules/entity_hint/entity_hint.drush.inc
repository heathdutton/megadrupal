<?php

/**
 * Implements hook_drush_command().
 */
function entity_hint_drush_command() {
  $items = [];

  $items['entity-hint'] = [
    'description' => 'Generate metadata hints for entities.',
    'callback'    => 'entity_hint_drush_command_callback',
    'options'     => [
      'type'      => 'Generate code for certain entity type.',
      'namespace' => 'Namespace for generated class',
    ],
    'examples'    => [
      'drush entity-hint > private/files/entity-hints.php'
    ],
  ];

  $items['entity-efq-hint'] = [
    'description' => 'Generate EFQ wrapper for the entity.',
    'callback'    => 'entity_hint_efq_drush_callback',
    'options'     => [
      'type'      => 'Generate code for certain entity type.',
      'namespace' => 'Namespace for generated class',
    ],
    'examples'    => [
      'drush entity-efq-hint --namespace=Drupal --type=profile2 > private/files/entity-efq-hints.php'
    ],
  ];

  return $items;
}

function entity_hint_drush_command_callback() {
  require_once __DIR__ . '/entity_hint.metadata.php';

  $namespace = drush_get_option('namespace', NULL);
  $types = drush_get_option_list('type', NULL) ?: array_keys(entity_get_info());

  $output = [];
  foreach ($types as $type) {
    $output[] = entity_hint($type, NULL, $namespace);

    foreach (array_keys(entity_get_info($type)['bundles']) as $bundle) {
      $output[] = entity_hint($type, $bundle);
    }
  }

  print "<?php\n\n" . implode("\n\n", $output);
}

function entity_hint_efq_drush_callback() {
  require_once __DIR__ . '/entity_hint.efq.php';

  $namespace = drush_get_option('namespace', NULL);
  $types = drush_get_option_list('type', NULL) ?: array_keys(entity_get_info());

  $output = [];
  foreach ($types as $type) {
    $output[] = entity_hint_efq($type, NULL, $namespace);

    foreach (array_keys(entity_get_info($type)['bundles']) as $bundle) {
      $output[] = entity_hint_efq($type, $bundle);
    }
  }

  print "<?php\n\n" . implode("\n\n", $output);
}

<?php
/**
 * @file
 * Field Complete Progress - Provides blocks to display a progress bar of Field completion - blocks.
 */

/**
 * Implements hook_block_view().
 */
function _fc_progress_block_view($entity_type) {
  $entity = fc_get_page_entity($entity_type);

  // No entity available, so skip it.
  if (empty($entity)) {
    return array();
  }
  $entity_info = entity_get_info($entity_type);

  return array(
    'subject' => t('@title Progress', array('@title' => $entity->{$entity_info['entity keys']['label']})),
    'content' => array(
      'bar' => array(
        '#theme' => 'fc_progress_bar',
        '#entity_type' => $entity_type,
        '#entity' => $entity,
      ),
      'next' => array(
        '#theme' => 'fc_progress_next',
        '#entity_type' => $entity_type,
        '#entity' => $entity,
      ),
      '#attached' => array(
        'css' => array(
          drupal_get_path('module', 'fc_progress') . '/fc_progress.css',
        ),
      ),
    ),
  );
}

function fc_progress_preprocess_fc_progress_bar(&$variables) {
  $entity_type = $variables['entity_type'];
  $entity = $variables['entity'];
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  if (empty($entity->fc)) {
    $entity->fc = fcComplete::load($entity_type, $entity);
  }

  $variables['fc_complete'] = $entity->fc->complete;
  $variables['fc_percentage'] = $entity->fc->percentage;
  $variables['fc_completeness'] = $entity->fc->completeness;

  $variables['percent_complete'] = t('@pc% complete', array('@pc' => $entity->fc->percentage));

  $variables['classes_array'][] = "fc-progress-bar-{$entity_type}";
  $variables['classes_array'][] = "fc-progress-bar-{$entity_type}-{$bundle}";
  $variables['classes_array'][] = 'fc-progress-bar-' . ($entity->fc->complete?'complete':'incomplete');

  $variables['theme_hook_suggestions'][] = "fc-progress-bar";
  $variables['theme_hook_suggestions'][] = "fc-progress-bar--{$entity_type}";
  $variables['theme_hook_suggestions'][] = "fc-progress-bar--{$entity_type}--{$bundle}";
  $variables['theme_hook_suggestions'][] = "fc-progress-bar--{$entity_type}--{$id}";
}

function fc_progress_preprocess_fc_progress_next(&$variables) {
  $entity_type = $variables['entity_type'];
  $entity_info = entity_get_info($entity_type);
  $entity = $variables['entity'];
  list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

  if (empty($entity->fc)) {
    $entity->fc = fcComplete::load($entity_type, $entity);
  }

  if ($field_name = array_search(FALSE, $entity->fc->completeness)) {
    $instance = field_info_instance($entity_type, $field_name, $bundle);

    $variables['field_label'] = $instance['label'];
    $variables['entity_title'] = $entity->{$entity_info['entity keys']['label']};

    $interval = 100 / count($entity->fc->completeness);
    $numfields = count(array_filter($entity->fc->completeness)) + 1;
    $variables['new_percentage'] = (int) ($interval * $numfields);

    $args = array(
      '%field_label' => $variables['field_label'],
      '!entity_title' => $variables['entity_title'],
      '!new_percentage' => $variables['new_percentage'],
    );
    $variables['next_message'] = t('Completing %field_label will bring !entity_title to !new_percentage%.', $args);
    if ($variables['new_percentage'] >= 100) {
      $variables['classes_array'][] = "fc-progress-next-100";
    }
  }
  else {
    $variables['field_label'] = '';
    $variables['new_precentage'] = '';
    $variables['next_message'] = '';
    $variables['classes_array'][] = "fc-progress-next-none";
  }

  $variables['classes_array'][] = "fc-progress-next-{$entity_type}";
  $variables['classes_array'][] = "fc-progress-next-{$entity_type}-{$bundle}";

  $variables['theme_hook_suggestions'][] = "fc-progress-next";
  $variables['theme_hook_suggestions'][] = "fc-progress-next--{$entity_type}";
  $variables['theme_hook_suggestions'][] = "fc-progress-next--{$entity_type}--{$bundle}";
  $variables['theme_hook_suggestions'][] = "fc-progress-next--{$entity_type}--{$id}";
}

//=====================================================================================

/**
 * Figure out the profile2 entity for this page (if there is one)
 */
function _fc_progress_block_view_profile2() {
  $entity = fc_get_page_entity($entity_type);
  $router_item = menu_get_item();
  foreach ($router_item['page_arguments'] as $arg) {
    if ($arg instanceof Profile) {
      $entity = $arg;
      break;
    }
  }

  return $entity;
}

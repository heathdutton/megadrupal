<?php

/**
 * @file
 * Implements dialogs for nodequeue reference field.
 */

/**
 * Implements hook_references_dialog_widgets().
 */
function nodequeue_reference_references_dialog_widgets() {
  return array(
    'nodequeue_reference_autocomplete' => array(
      'element_type' => 'textfield',
      'format' => '$label [qid: $entity_id]',
      'operations' => array(
        'edit' => array(
          'function' => 'nodequeue_reference_dialog_reference_reference_edit_link',
          'title' => t('Edit queue settings dialog'),
        ),
        'view' => array(
          'function' => 'nodequeue_reference_dialog_reference_reference_view_link',
          'title' => t('View queue content dialog'),
        ),
        'add' => array(
          'function' => 'nodequeue_reference_dialog_reference_reference_add_link',
          'title' => t('Add new queue dialog'),
        ),
      ),
    ),
  );
}

/**
 * Add link callback for nodequeue references.
 */
function nodequeue_reference_dialog_reference_reference_add_link($element, $widget_settings, $field, $instance) {
  // Hide add link for non-empty default value.
  if (!empty($element['#default_value']) || !empty($element['#value'])) {
    return array();
  }

  $add_links = array();
  if (user_access('administer nodequeue')) {
    $add_links[] = array(
      'title' => t('Create nodequeue'),
      'href' => 'admin/structure/nodequeue/add/nodequeue',
      'query' => array(
        'nodequeue-new-dialog' => 1,
      ),
    );
  }
  
  return $add_links;
}

/**
 * Edit link callback for nodequeue references.
 */
function nodequeue_reference_dialog_reference_reference_edit_link($element, $widget_settings, $field, $instance) {
  if (isset($element['#default_value']) || isset($element['#value'])) {
    if (isset($element['#default_value'])) {
      $value = $element['#default_value'];
    } 
    else {
      // Take "label [nid:id]', match the id from parenthesis.
      if (preg_match('/^(?:\s*|(.*) )?\[\s*qid\s*:\s*(\d+)\s*\]$/', $element['#value'], $matches)) {
        list(, $title, $value) = $matches;
      } 
      else {
        return array();
      }
    }

    $queues = nodequeue_load_queues(array($value));
    if (nodequeue_queue_access($queues[$value])) {
      return array(
        array(
          'title' => t('Edit nodequeue'),
          'href' => 'admin/structure/nodequeue/' . $value . '/edit',
        ),
      );
    }
  }
  return array();
}

/**
 * View link callback for nodequeue references.
 */
function nodequeue_reference_dialog_reference_reference_view_link($element, $widget_settings, $field, $instance) {
  if (isset($element['#default_value']) || isset($element['#value'])) {
    if (isset($element['#default_value'])) {
      $value = $element['#default_value'];
    } 
    else {
      // Take "label [nid:id]', match the id from parenthesis.
      if (preg_match('/^(?:\s*|(.*) )?\[\s*qid\s*:\s*(\d+)\s*\]$/', $element['#value'], $matches)) {
        list(, $title, $value) = $matches;
      } 
      else {
        return array();
      }
    }

    $queues = nodequeue_load_queues(array($value));
    $subqueues = nodequeue_load_subqueues_by_queue(array($value));
    $sqid = current(array_keys($subqueues));
    if (nodequeue_queue_access($queues[$value])) {
      return array(
        array(
          'title' => t('Modify items'),
          'href' => "admin/structure/nodequeue/$value/view/$sqid",
        ),
      );
    }
  }
  return array();
}

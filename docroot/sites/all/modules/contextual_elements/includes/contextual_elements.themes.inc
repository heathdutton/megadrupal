<?php
/**
 * @file
 * Theme functions.
 */
/**
 * The top administrative page listing the instances.
 */
function theme_contextual_elements_instances_table_form($vars) {
  $form = $vars['form'];
  $inst = contextual_elements_get_elements();
  $rows = array();

  // Build the table of instances
  foreach ($inst as $i) {
    // Build list of operations
    $ops = array();
    if (user_access('view contextual element details')) {
      $ops[] = l(t('view'), "admin/config/search/contextual-elements/{$i->cid}/view");
    }
    if (user_access('edit contextual elements')) {
      $ops[] = l(t('edit'), "admin/config/search/contextual-elements/{$i->cid}/edit", array('query' => drupal_get_destination()));
    }
    if (user_access('delete contextual elements')) {
      $ops[] = l(t('delete'), "admin/config/search/contextual-elements/{$i->cid}/delete", array('query' => drupal_get_destination()));
    }

    $row = array();
    $row[] = "<strong>{$i->name}</strong><br /><em>{$i->description}</em>";
    $row[] = drupal_render($form['data'][$i->cid]['status']);
    $row[] = drupal_render($form['data'][$i->cid]['debug']);
    $row[] = drupal_render($form['data'][$i->cid]['weight']);
    $row[] = implode('&nbsp;|&nbsp;', $ops);
    $rows[] = array('data' => $row, 'class' => array('draggable status-' . $i->status));
  }

  // Empty message
  if (empty($rows)) {
    $output = t('No contextual elements have been defined. !link.', array('!link' => l(t('Create one now'), 'admin/config/search/contextual-elements/add')));
  }
  // Theme the table of instances.
  else {
    $output = theme('table', array('header' => $form['headers']['#value'], 'rows' => $rows, 'attributes' => array('id' => 'contextual-elements-instances-table')));
    // Render remaining form elements
    $output .= drupal_render_children($form);
  }

  // Add the Drag n Drop functionality to the table.
  drupal_add_tabledrag('contextual-elements-instances-table', 'order', 'sibling', 'weight-drag');

  return $output;
}

/**
 * Layout for the custom variables table in the admin settings form.
 */
function theme_contextual_elements_admin_gavar_table($variables) {
  $form = $variables['form'];

  $header = array(
    array('data' => t('Slot')),
    array('data' => t('Name')),
    array('data' => t('Value')),
    array('data' => t('Scope')),
  );

  $rows = array();
  foreach (element_children($form['slots']) as $key => $id) {
    $rows[] = array(
      'data' => array(
        drupal_render($form['slots'][$id]['slot']),
        drupal_render($form['slots'][$id]['name']),
        drupal_render($form['slots'][$id]['value']),
        drupal_render($form['slots'][$id]['scope']),
      ),
    );
  }

  $output = drupal_render($form['ga_vars_name']);
  $output .= theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render($form['gavars_custom_var_description']);
  $output .= drupal_render($form['gavars_custom_var_token_tree']);

  return $output;
}
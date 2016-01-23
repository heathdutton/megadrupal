<?php

/**
 * @file
 * Admin for eck_weight_form.
 */

/**
 * Build the eck_weight_form form.
 *
 * @return array
 *   A form array set for theming by theme_eck_weight_form()
 */
function eck_weight_form($form, &$form_state, $entity_type = NULL, $bundle = NULL) {
  // Identify that the elements in 'example_items' are a collection, to
  // prevent Form API from flattening the array when submitted.
  $form['eck_weight']['#tree'] = TRUE;

  // Cache entity type, entity info and bundle in form state.
  $form_state['#entity_type'] = $entity_type;
  $form_state['#entity_info'] = entity_get_info($entity_type->name);
  $form_state['#bundle'] = $bundle;

  // Fetch the ECK entity data from the database, ordered by weight ascending.
  $result = eck_weight_get_entities($form_state);
  // Iterate through each database result.
  foreach ($result as $item) {
    // Create a form entry for this item.
    // Each entry will be an array using the the unique id for that item as
    // the array key, and an array of table row data as the value.
    $form['eck_weight'][$item->id] = array(
      // We'll use a form element of type '#markup' to display the item id.
      'id' => array(
        '#markup' => $item->id,
      ),
      // We'll use a form element of type '#markup' to display the item label.
      'label' => array(
        '#markup' => check_plain($item->label),
      ),
      // The 'weight' field will be manipulated as we move the items around in
      // the table using the tabledrag activity.  We use the 'weight' element
      // defined in Drupal's Form API.
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $item->weight,
        '#delta' => 10,
        '#title_display' => 'invisible',
      ),
      'weight_original' => array(
        '#type' => 'value',
        '#value' => $item->weight,
      ),
    );
  }

  // Now we add our submit button, for submitting the form results.
  //
  // The 'actions' wrapper used here isn't strictly necessary for tabledrag,
  // but is included as a Form API recommended practice.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save Changes'));
  return $form;
}

/**
 * Helper function to get all ECK entities of a type.
 *
 * @param array $form_state
 *   The form state array.
 *
 * @return array
 *   A list of entity objects loaded from the database.
 */
function eck_weight_get_entities(&$form_state) {
  // Get the entity base table.
  $info = $form_state['#entity_info'];
  $bundle = $form_state['#bundle'];
  if (isset($info['base table'])) {
    $table = $info['base table'];
  }
  // Make sure the entity has a label key.
  if (isset($info['entity keys']['label'])) {
    $label = $info['entity keys']['label'];
  }
  $weight = eck_weight_get_weigh_property($form_state['#entity_type']);

  // Save the base table and the weight_field to be used in submit handler.
  $form_state['params'] = array(
    'table' => $table,
    'weight_field' => $weight,
  );
  // Validate the table, label and bundle.
  if (!isset($table) || !isset($label) || !isset($bundle->name) || !isset($weight)) {
    return array();
  }

  // Run the query against the ECK entity type database.
  $query = db_select($table, 'eck')
      ->condition('type', $bundle->name, '=')
      ->orderBy($weight, 'ASC');
  // Add our fields alias, to make sure we always have the same keys.
  $query->addField('eck', 'id');
  $query->addField('eck', $weight, 'weight');
  $query->addField('eck', $label, 'label');
  // @TODO: would be nice to add a pager and be able to load more items.
  $query->range(0, 100);
  try {
    $result = $query->execute();
  }
  catch (Exception $e) {
    return array();
  }

  if (!$result) {
    return array();
  }
  return $result->fetchAllAssoc('id');
}

/**
 * Theme callback for the eck_weight_form form.
 *
 * @return array
 *   The rendered eck_weight_form form
 */
function theme_eck_weight_form($variables) {
  $form = $variables['form'];

  // Initialize the variable which will store our table rows.
  $rows = array();

  $elements = element_children($form['eck_weight']);
  if (!empty($elements)) {
    // Iterate over each element in our $form['eck_weight'] array.
    foreach (element_children($form['eck_weight']) as $id) {

      // Before we add our 'weight' column to the row, we need to give the
      // element a custom class so that it can be identified in the
      // drupal_add_tabledrag call.
      $form['eck_weight'][$id]['weight']['#attributes']['class'] = array('eck-weight-item');

      // We are now ready to add each element of our $form data to the $rows
      // array, so that they end up as individual table cells when rendered
      // in the final table.  We run each element through the drupal_render()
      // function to generate the final html markup for that element.
      $rows[] = array(
        'data' => array(
          // Add our 'label' column.
          drupal_render($form['eck_weight'][$id]['label']),
          // Add our 'id' and 'weight_original' in one column.
          drupal_render($form['eck_weight'][$id]['id']) .
          drupal_render($form['eck_weight'][$id]['weight_original']),
          // Add our 'weight' column.
          drupal_render($form['eck_weight'][$id]['weight']),
        ),
        // Add tabledrag support.
        'class' => array('draggable'),
      );
    }
  }
  $header = array(t('Label'), t('Entity ID'), t('Weight'));
  $table_id = 'eck-weigth-table';

  // Render our table as tabledrag in output.
  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('There are no entities.'),
    'attributes' => array('id' => $table_id),
  ));

  // Render any remaining form elements.
  $output .= drupal_render_children($form);

  // We now call the drupal_add_tabledrag() function in order to add the
  // tabledrag.js goodness onto our page.
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'eck-weight-item');

  return $output;
}

/**
 * Submit callback for the eck_weight_form form.
 *
 * Updates the 'weight' column for each element in our table, taking into
 * account that item's new order after the drag and drop actions have been
 * performed.
 */
function eck_weight_form_submit($form, &$form_state) {
  // Because the form elements were keyed with the item ids from the database,
  // we can simply iterate through the submitted values.
  foreach ($form_state['values']['eck_weight'] as $id => $item) {
    // Update items that have a new 'weight'.
    if ($item['weight'] != $item['weight_original']) {
      db_update($form_state['params']['table'])
          ->fields(array(
            $form_state['params']['weight_field'] => $item['weight'],))
          ->condition('id', $id, '=')
          ->execute();
    }
  }
}

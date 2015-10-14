<?php
/**
 * @file
 * The actual references manager admin page.
 */

/**
 * Menu callback to view the references manager page.
 */
function references_manager_manage($form, $form_state, $entity_type, $entity, $field) {
  // If the $entity isn't an object then something went wrong.
  if (!is_object($entity)) {
    drupal_set_message("There was a problem loading the requested object.", 'error');
    return $form;
  }

  // Strip off the 'ref-' part of the field name to get the correct field name.
  $field_name = str_replace(REFERENCES_MANAGER_MENU_PREFIX, '', $field);

  // Load the field settings.
  list($entity_id, $revision_id, $bundle) = entity_extract_ids($entity_type, $entity);
  $params = array(
    'field_name' => $field_name,
    'entity_type' => $entity_type,
    'bundle' => $bundle,
  );
  $instances = field_read_instances($params);
  $instance = array_pop($instances);
  $field = field_info_field($field_name);

  // Reload the entity, see if this fixes the problem.
  $function = $entity_type . '_load';

  // Add some hidden values to pass through to the Submit callback.
  $form['field_name'] = array(
    '#type' => 'value',
    '#value' => $field_name,
  );
  $form['entity_type'] = array(
    '#type' => 'value',
    '#value' => $entity_type,
  );
  $form['bundle'] = array(
    '#type' => 'value',
    '#value' => $bundle,
  );
  $form['entity_id'] = array(
    '#type' => 'value',
    '#value' => $entity_id,
  );
  $form['revision_id'] = array(
    '#type' => 'value',
    '#value' => $revision_id,
  );

  // The view mode is saved as the 'references_manager' value.
  $view_mode = $instance['settings']['references_manager'];

  // If there are no fields to work with, just show an error message.
  if (empty($entity->{$field_name}[$entity->language])) {
    drupal_set_message(t('There are no items to sort on this node.'));
  }

  else {
    // A brief preable.
    $form['preample'] = array(
      '#markup' => t("Drag 'n drop the items below to assign a new sort order."),
      '#prefix' => '<p class="references-manager-preamble">',
      '#suffix' => '</p>',
    );

    // Add a container to store all of the items in a tree.
    $form['items'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('references-manager-items')),
    );

    // Store the original item for later.
    $original_order = array();

    // Build each item with a hidden form field to track its original position.
    foreach ($entity->{$field_name}[$entity->language] as $ctr => $item) {
      $item_id = array_pop($item);
      $original_order[] = $item_id;
      $function = $entity_type . '_load';
      $item = $function($item_id);
      $function = $entity_type . '_view';
      $item = $function($item, $view_mode);
      $form['items']['item_' . $ctr] = array(
        '#type' => 'hidden',
        '#value' => $item_id,
        '#prefix' => '<div class="references-manager-item">',
        '#suffix' => drupal_render($item) . "</div>\n",
        '#attributes' => array(
          'class' => array('references-manager-field'),
        ),
      );
    }

    // Buttons.
    $form['actions'] = array(
      '#type' => 'container',
      'submit' => array(
        '#type' => 'submit',
        '#value' => t("Save sort order"),
      ),
    );

    // We need some core CSS & JS for this.
    drupal_add_css('misc/ui/jquery.ui.core.min.css');
    drupal_add_js('misc/ui/jquery.ui.core.min.js', array('scope' => 'footer'));
    drupal_add_js('misc/ui/jquery.ui.widget.min.js', array('scope' => 'footer'));
    drupal_add_js('misc/ui/jquery.ui.mouse.min.js', array('scope' => 'footer'));
    drupal_add_js('misc/ui/jquery.ui.sortable.min.js', array('scope' => 'footer'));

    // Make the list sortable.
    drupal_add_js('(function ($) {$("form#references-manager-manage .references-manager-items").sortable();})(jQuery);', array('type' => 'inline', 'scope' => 'footer'));

    // Load the module's custom CSS.
    drupal_add_css(drupal_get_path('module', 'references_manager') . '/references_manager.css');
  }

  return $form;
}

/**
 * Form submit callback for the references manager page.
 *
 * Save the new order.
 */
function references_manager_manage_submit($form, &$form_state) {
  // Make the code simpler.
  $input = $form_state['input'];
  $values = $form_state['values'];
  $field_name = $values['field_name'];
  $entity_type = $values['entity_type'];
  $bundle = $values['bundle'];
  $entity_id = $values['entity_id'];
  $revision_id = $values['revision_id'];

  // Load the field settings & entity info.
  $params = array(
    'field_name' => $field_name,
    'entity_type' => $entity_type,
    'bundle' => $bundle,
  );
  $instances = field_read_instances($params);
  $instance = array_pop($instances);
  $field = field_info_field($field_name);
  $function = $entity_type . '_load';
  $entity = $function($entity_id, $revision_id);

  // Get a list of field names to work from.
  $primary_key = array_pop(array_keys($field['columns']));

  // Extract a list of items.
  $items = array();
  foreach ($input as $key => $value) {
    $matches = array();
    preg_match('/item_(\d*)/', $key, $matches);
    if (isset($matches[1]) && is_numeric($matches[1])) {
      $key_x = $matches[1];
      $items[] = $value;
    }
  }

  // Update the data & revision tables for this field.
  foreach ($items as $ctr => $id) {
    $query_args = array(
      ':delta' => $ctr,
      ':entity_type' => $entity_type,
      ':bundle' => $bundle,
      ':entity_id' => (int) $entity_id,
      ':revision_id' => (int) $revision_id,
      ':language' => $entity->language,
      ':id' => (int) $id,
    );
    $keys = array('entity_type', 'entity_id', 'revision_id', 'language', 'delta');
    foreach (array('field_data', 'field_revision') AS $table) {
      db_update($table . '_' . $field_name)
        ->fields(array($field_name . '_' . $primary_key => $id))
        ->condition('entity_type', $entity_type)
        ->condition('bundle', $bundle)
        ->condition('entity_id', (int) $entity_id)
        ->condition('revision_id', (int) $revision_id)
        ->condition('language', $entity->language)
        ->condition('delta', (int) $ctr)
        ->execute();
    }
    cache_clear_all('field:' . $entity_type . ':' . $entity_id, 'cache_field');
  }

  drupal_set_message(t("The sorting order has been updated."));
}

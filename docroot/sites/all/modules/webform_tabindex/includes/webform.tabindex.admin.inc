<?php

/**
 * @file
 * Webform module tabindex attribute handling.
 */

/**
 * Provides interface for update the position of tabindex in webform elements.
 */

/**
 * The table-based listing of all components for this webform in tabindex order.
 */
function webform_tabindex_form($form, $form_state, $node) {
  $form = array(
    '#tree' => TRUE,
    '#node' => $node,
    'components' => array(),
  );

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  $query = db_select('webform_component', 'wp');
  $query->leftJoin('webform_tabindex', 'wt', 'wp.nid = wt.nid AND wp.cid = wt.cid');
  $query->fields('wt', array('tabindex', 'skip'));
  $query->fields('wp', array('nid', 'cid', 'name', 'type', 'form_key'));
  $query->condition('wp.nid', $node->nid);
  $query->orderBy('wt.tabindex', 'ASC');
  $query->groupBy('wp.nid, wp.cid');
  $result = $query->execute();
  $count = $result->rowCount();

  if ($count) {
    // Use to set weight of tabindex if corresponding record does not exists
    // into webform_tabindex module table.
    $counter = 1;
    // Iterate through each database result.
    foreach ($result as $item) {
      // Create a form entry for this item.
      // Each entry will be an array using the the unique id for that item as
      // the array key, and an array of table row data as the value.
      $form['tabindex_items'][$item->cid] = array(
        // We'll use a form element of type '#markup' to display the item name.
        'name' => array(
          '#markup' => check_plain($item->name),
        ),
        // We'll use a form element of type '#markup' to display the item type.
        'type' => array(
          '#markup' => check_plain($item->type),
        ),
        // We'll use a form element of type '#markup' to display the item
        // form_key.
        'form_key' => array(
          '#markup' => check_plain($item->form_key),
        ),
        
        'checkbox' => array(
          '#type' => 'checkbox',
          '#title' => '',
          '#default_value' => isset($item->skip) ? $item->skip : 0,
          '#return_value' => 1,
          '#title-display' => 'invisible',
        ),
        
        'operation' => array(
          '#markup' => l(t('Edit'), 'node/' . $node->nid . '/webform/components/' . $item->cid, array('query' => drupal_get_destination())),
        ),
        // The 'weight' field will be manipulated as we move the items around in
        // the table using the tabledrag activity.  We use the 'weight' element
        // defined in Drupal's Form API.
        'weight' => array(
          '#type' => 'textfield',
          '#title' => '',
          '#default_value' => isset($item->tabindex) ? $item->tabindex : $counter,
          '#maxlength' => 3,
          '#title-display' => 'invisible',
          '#size' => 4,
        ),
      );
      $counter++;
    }
  }

  // Now we add our submit button, for submitting the form results.
  // The 'actions' wrapper used here isn't strictly necessary for tabledrag,
  // but is included as a Form API recommended practice.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Changes'),
    '#validate' => array('webform_tabindex_form_validate'),
    '#submit' => array('webform_tabindex_from_submit'),
  );
  return $form;
}

/**
 * Validate handler for webform_tabindex_form().
 */
function webform_tabindex_form_validate($form, &$form_state) {
  $tabindex_items = $form_state['values']['tabindex_items'];
  if (is_array($tabindex_items) && count($tabindex_items)) {
    foreach ($tabindex_items as $cid => $item) {
      if (trim($item['weight']) == '' || $item['weight'] <= 0) {
        form_error($form['components'], t('!name: Tabindex number should be a positive integer value.', array('!name' => $form['tabindex_items'][$cid]['name']['#markup'])) . theme('item_list', array('tabindex_items' => $item))
        );
      }
    }
  }
}

/**
 * Theme the node components form. Use a table to organize the tabindex.
 *
 * @return array
 *   Formatted HTML form, ready for display.
 */
function theme_webform_tabindex_form($variables) {

  $form = $variables['form'];

  // Initialize the variable which will store our table rows.
  $rows = array();

  // Iterate over each element in our $form['tabindex_items'] array.
  foreach (element_children($form['tabindex_items']) as $cid) {
    // We need to give the element a custom class so that it can be identified
    // in the drupal_add_tabledrag call.
    // This could also have been done during the form declaration by adding
    // '#attributes' => array('class' => 'webform-tabindex-weight'),
    $form['tabindex_items'][$cid]['weight']['#attributes']['class'] = array('webform-tabindex-weight');

    // We are now ready to add each element of our $form data to the $rows
    // array, so that they end up as individual table cells when rendered
    // in the final table.  We run each element through the drupal_render()
    // function to generate the final html markup for that element.
    $rows[] = array(
      'data' => array(
        // Add forms element 'name' column.
        drupal_render($form['tabindex_items'][$cid]['name']),
        // Add forms element 'type' column.
        drupal_render($form['tabindex_items'][$cid]['type']),
        // Add forms element 'form_key' column.
        drupal_render($form['tabindex_items'][$cid]['form_key']),
        drupal_render($form['tabindex_items'][$cid]['checkbox']),
        // Add forms element 'weight' column.
        drupal_render($form['tabindex_items'][$cid]['weight']),
        // Add forms element 'operation' column.
        drupal_render($form['tabindex_items'][$cid]['operation']),
      ),
      // To support the tabledrag behaviour, we need to assign each row of the
      // table a class attribute of 'draggable'. This will add the 'draggable'
      // class to the <tr> element for that row when the final table is
      // rendered.
      'class' => array('draggable'),
    );
  }

  // We now define the table header values.  Ensure that the 'header' count
  // matches the final column count for your table.
  $header = array(t('Name'),
    t('Type'),
    t('Form  Key'),
    t('Skip'),
    t('Tabindex Number'),
    'Operations',
  );

  // We also need to pass the drupal_add_tabledrag() function an id which will
  // be used to identify the <table> element containing our tabledrag form.
  // Because an element's 'id' should be unique on a page, make sure the value
  // you select is NOT the same as the form ID used in your form declaration.
  $table_id = 'webform-tabindex-table';

  // We can render our tabledrag table for output.
  $output = theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('id' => $table_id),
  ));

  // And then render any remaining form elements (such as our submit button).
  $output .= drupal_render_children($form);

  // We now call the drupal_add_tabledrag() function in order to add the
  // tabledrag.js goodness onto our page.
  // For a basic sortable table, we need to pass it:
  // - the $table_id of our <table> element,
  // - the $action to be performed on our form items ('order'),
  // - a string describing where $action should be applied ('siblings'),
  // - and the class of the element containing our 'weight' element.
  drupal_add_tabledrag($table_id, 'order', 'sibling', 'webform-tabindex-weight');
  return $output;
}

/**
 * Submit handler for webform_tabindex_form() to save tab/navigation order.
 */
function webform_tabindex_from_submit($form, &$form_state) {
  $nid = $form_state['values']['nid'];
  foreach ($form_state['values']['tabindex_items'] as $cid => $item) {
    db_merge('webform_tabindex')
      ->key(array('nid' => $nid, 'cid' => $cid))
      ->fields(array('tabindex' => $item['weight'], 'skip' => (int) $item['checkbox']))->execute();
  }
}

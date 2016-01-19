<?php

/**
 * @file
 * dquarks module component handling.
 */

/**
 * Provides interface and database handling for editing components of a dquarks.
 *
 * @author IP-TECH
 */

/**
 * Overview page of all components for this dquarks.
 */
function dquarks_components_page($node, $page_number = 1) {
  $output = drupal_get_form('dquarks_components_form', $node);

  return array(
    '#theme' => 'dquarks_components_page',
    '#node' => $node,
    '#form' => $output,
  );
}

/**
 * Theme the output of the main components page.
 *
 * This theming provides a way to toggle between the editing modes if Form
 * Builder module is available.
 */
function theme_dquarks_components_page($variables) {
  $node = $variables['node'];
  $form = $variables['form'];
  return drupal_render($form);
}

/**
 * The table-based listing of all components for this dquarks.
 */
function dquarks_components_form($form, $form_state, $node) {
  $form = array(
    '#tree' => TRUE,
    '#node' => $node,
    'components' => array(),
  );

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  $options = array();
  foreach ($node->dquarks['components'] as $cid => $component) {
    $options[$cid] = check_plain($component['name']);
    $form['components'][$cid]['cid'] = array(
      '#type' => 'hidden',
      '#default_value' => $component['cid'],
    );
    $form['components'][$cid]['pid'] = array(
      '#type' => 'hidden',
      '#default_value' => $component['pid'],
    );
    $form['components'][$cid]['weight'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#title' => t('Weight'),
      '#default_value' => $component['weight'],
    );
    $form['components'][$cid]['mandatory'] = array(
      '#type' => 'checkbox',
      '#title' => t('Mandatory'),
      '#default_value' => $component['mandatory'],
      '#access' => !in_array($component['type'], array('direction')
      ),
    );
    $form['components'][$cid]['email'] = array(
      '#type' => 'checkbox',
      '#title' => t('E-mail'),
      '#default_value' => $component['email'],
      '#access' => !in_array($component['type'], array('direction')
      ),
    );
  }
  $form['add']['name'] = array(
    '#type' => 'textfield',
    '#size' => 24,
  );

  $form['add']['type'] = array(
    '#type' => 'select',
    '#options' => dquarks_component_options(),
    '#weight' => 3,
    '#default_value' => (isset($_GET['cid']) && isset($node->dquarks['components'][$_GET['cid']])) ? $node->dquarks['components'][$_GET['cid']]['type'] : 'textfield',
  );
  $form['add']['mandatory'] = array(
    '#type' => 'checkbox',
  );
  $form['add']['email'] = array(
    '#type' => 'checkbox',
    '#default_value' => 1,
  );
  $form['add']['cid'] = array(
    '#type' => 'hidden',
    '#default_value' => '',
  );
  $form['add']['pid'] = array(
    '#type' => 'hidden',
    '#default_value' => (isset($_GET['cid']) && isset($node->dquarks['components'][$_GET['cid']])) ? $node->dquarks['components'][$_GET['cid']]['pid'] : 0,
  );
  $form['add']['weight'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#delta' => count($node->dquarks['components']) > 10 ? count($node->dquarks['components']) : 10,
    '#default_value' => (isset($_GET['cid']) && isset($node->dquarks['components'][$_GET['cid']])) ? $node->dquarks['components'][$_GET['cid']]['weight'] + 1 : count($node->dquarks['components']),
  );

  $form['add']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#weight' => 45,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 45,
  );
  $form['publish'] = array(
    '#type' => 'submit',
    '#value' => t('Publish'),
    '#weight' => 50,
    '#access' => !$node->status,
  );
  $form['unpublish'] = array(
    '#type' => 'submit',
    '#value' => t('Unpublish'),
    '#weight' => 50,
    '#access' => $node->status,
  );
  return $form;
}

/**
 * Theme the node components form. Use a table to organize the components.
 *
 * @param array $variables
 *   Contains the form array.
 *
 * @return string
 *   Formatted HTML form, ready for display.
 */
function theme_dquarks_components_form($variables) {
  $form = $variables['form'];
  drupal_add_css(drupal_get_path('module', 'dquarks') . '/css/dquarks.css');
  drupal_add_js(drupal_get_path('module', 'dquarks') . '/js/dquarks.js');

  $form['components']['#attached']['library'][] = array('dquarks', 'admin');

  drupal_add_tabledrag('dquarks-components', 'order', 'sibling', 'dquarks-weight');
  drupal_add_tabledrag('dquarks-components', 'match', 'parent', 'dquarks-pid', 'dquarks-pid', 'dquarks-cid');

  $node = $form['#node'];

  $header = array(
    t('Name'),
    t('Type'),
    t('Value'),
    t('Mandatory'),
    t('E-mail'),
    t('Weight'),
    array(
      'data' => t('Operations'),
      'colspan' => 3,
    ),
  );
  $rows = array();

  $page1 = array(t('Page') . ' ' . 1 . ' :', '', '', '', '', '', '', '', '');

  // Add a row containing form elements for a new item.
  unset($form['add']['name']['#title'], $form['add_type']['#description']);
  $form['add']['name']['#attributes']['rel'] = t('New component name');
  $form['add']['name']['#attributes']['class'] = array('dquarks-default-value');
  $form['add']['cid']['#attributes']['class'] = array('dquarks-cid');
  $form['add']['pid']['#attributes']['class'] = array('dquarks-pid');
  $form['add']['weight']['#attributes']['class'] = array('dquarks-weight');
  $row_data = array(
    drupal_render($form['add']['name']),
    drupal_render($form['add']['type']),
    '',
    drupal_render($form['add']['mandatory']),
    drupal_render($form['add']['email']),
    drupal_render($form['add']['cid']) . drupal_render($form['add']['pid']) . drupal_render($form['add']['weight']),
    array('colspan' => 3, 'data' => drupal_render($form['add']['add'])),
  );
  $add_form = array(
    'data' => $row_data,
    'class' => array(
      'draggable', 'dquarks-add-form',
    ),
  );
  $form_rendered = FALSE;

  if (!empty($node->dquarks['components'])) {
    $add_form['class'] = array('draggable');
    $component_tree = array();
    $page_count = 1;
    _dquarks_components_tree_build($node->dquarks['components'], $component_tree, 0, $page_count);
    $component_tree = _dquarks_components_tree_sort($component_tree);

    /**
     * Build the table rows.
     *
     * @param object $node
     *   The node object.
     * @param int $cid
     *   The component id.
     * @param array $component
     *   The current component.
     * @param int $level
     *   The current level
     * @param array $form
     *   An array of form items.
     * @param array $rows
     *   An array of component of quiz.
     * @param array $add_form
     *   The form to add
     * @param int $num_page
     *   The number of page
     */
    function _dquarks_components_form_rows($node, $cid, $component, $level, &$form, &$rows, &$add_form, &$num_page = 1) {
      // Create presentable values.
      if (drupal_strlen($component['value']) > 30) {
        $component['value'] = drupal_substr($component['value'], 0, 30);
        $component['value'] .= '...';
      }
      $component['value'] = check_plain($component['value']);

      // Remove individual titles from the mandatory and weight fields.
      unset($form['components'][$cid]['mandatory']['#title']);
      unset($form['components'][$cid]['pid']['#title']);
      unset($form['components'][$cid]['weight']['#title']);
      unset($form['components'][$cid]['email']['#title']);

      // Add special classes for weight and parent fields.
      $form['components'][$cid]['cid']['#attributes']['class'] = array('dquarks-cid');
      $form['components'][$cid]['pid']['#attributes']['class'] = array('dquarks-pid');
      $form['components'][$cid]['weight']['#attributes']['class'] = array('dquarks-weight');

      // Build indentation for this row.
      $indents = '';
      for ($n = 1; $n <= $level; $n++) {
        $indents .= '<div class="indentation">&nbsp;</div>';
      }

      // Add each component to a table row.
      $row_data = array(
        $indents . filter_xss($component['name']),
        t($component['type']),
        ($component['value'] == '') ? '-' : $component['value'],
        drupal_render($form['components'][$cid]['mandatory']),
        drupal_render($form['components'][$cid]['email']),
        drupal_render($form['components'][$cid]['cid']) . drupal_render($form['components'][$cid]['pid']) . drupal_render($form['components'][$cid]['weight']),
        l(t('Edit'), 'node/' . $node->nid . '/dquarks/components/' . $cid, array('query' => drupal_get_destination())),
        l(t('Clone'), 'node/' . $node->nid . '/dquarks/components/' . $cid . '/clone', array('query' => drupal_get_destination())),
        l(t('Delete'), 'node/' . $node->nid . '/dquarks/components/' . $cid . '/delete', array('query' => drupal_get_destination())),
      );


      $row_class = array('draggable');
      if ($component['type'] != 'fieldset') {
        $row_class[] = 'tabledrag-leaf';
      }
      if ($component['type'] == 'direction') {
        $row_class[] = 'end_page';
      }
      $rows[] = array('data' => $row_data, 'class' => $row_class);
      if (isset($component['children']) && is_array($component['children'])) {
        foreach ($component['children'] as $cid => $component) {
          _dquarks_components_form_rows($node, $cid, $component, $level + 1, $form, $rows, $add_form, $num_page);
        }
      }


      // Add the add form if this was the last edited component.
      if (isset($_GET['cid']) && $component['cid'] == $_GET['cid'] && $add_form) {
        $add_form['data'][0] = $indents . $add_form['data'][0];
        $rows[] = $add_form;
        $add_form = FALSE;
      }

      if ($component['type'] == 'direction') {
        $num_page++;
        $saut_page = array(
          t('Page') . ' ' . $num_page . ' :', '', '', '', '', '', '', '', '',
        );
        $rows[] = array('data' => $saut_page, 'class' => array('start_page'));
      }
    }

    $show_page_num = 1;
    foreach ($component_tree['children'] as $cid => $component) {
      _dquarks_components_form_rows($node, $cid, $component, 0, $form, $rows, $add_form, $show_page_num);
    }
  }
  else {
    $rows[] = array(array('data' => t('No Components, add a component below.'), 'colspan' => 9));
  }

  // Append the add form if not already printed.
  if ($add_form) {
    $rows[] = $add_form;
  }

  $output = '';
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'dquarks-components'),
          )
  );
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Implements hook_validate().
 *
 * @param array $form
 *   An array of form items.
 * @param array $form_state
 *   An array of form values items.
 */
function dquarks_components_form_validate($form, &$form_state) {
  // Check that the entered component name is valid.
  if ($form_state['values']['op'] == t('Add') && drupal_strlen(trim($form_state['values']['add']['name'])) <= 0) {
    form_error($form['add']['name'], t('When adding a new component, the name field is required.'));
  }

  // Check that no two components end up with the same form key.
  $duplicates = array();
  $parents = array();
  if (isset($form_state['values']['components'])) {
    foreach ($form_state['values']['components'] as $cid => $component) {
      $form_key = $form['#node']->dquarks['components'][$cid]['form_key'];
      if (isset($parents[$component['pid']]) && ($existing = array_search($form_key, $parents[$component['pid']])) && $existing !== FALSE) {
        if (!isset($duplicates[$component['form_key']])) {
          $duplicates[$component['form_key']] = array($existing);
        }
        $duplicates[$component['form_key']][] = $cid;
      }
      $parents[$component['pid']][$cid] = $form_key;
    }
  }

  if (!empty($duplicates)) {
    $error = t('The form order failed to save because the following elements have same form keys and are under the same parent. Edit each component and give them a unique form key, then try moving them again.');
    $items = array();
    foreach ($duplicates as $form_key => $cids) {
      foreach ($cids as $cid) {
        $items[] = $form['#node']->dquarks['components'][$cid]['name'];
      }
    }

    form_error(check_plain($form['components']), check_plain($error) . theme('item_list', check_plain($items)));
  }
}

/**
 * Implements hook_submit().
 *
 * @param array $form
 *   An array of form items.
 * @param array $form_state
 *   An array of form values items.
 *
 * @return int
 *   The id of node.
 */
function dquarks_components_form_submit($form, &$form_state) {
  $node = node_load($form_state['values']['nid']);

  // Update all mandatory and weight values.
  foreach ($node->dquarks['components'] as $cid => $component) {
    if ($component['pid'] != $form_state['values']['components'][$cid]['pid'] || $component['weight'] != $form_state['values']['components'][$cid]['weight'] || $component['mandatory'] != $form_state['values']['components'][$cid]['mandatory'] || $component['email'] != $form_state['values']['components'][$cid]['email']) {
      $component['weight'] = $form_state['values']['components'][$cid]['weight'];
      $component['mandatory'] = $form_state['values']['components'][$cid]['mandatory'];
      $component['email'] = $form_state['values']['components'][$cid]['email'];
      $component['pid'] = $form_state['values']['components'][$cid]['pid'];
      $component['nid'] = $node->nid;
      dquarks_component_update($component);
    }
  }

  if (isset($_POST['op']) && $_POST['op'] == t('Publish')) {
    $node->status = 1;
    node_save($node);
    drupal_set_message(t('Your dquarks has been published.'));
    return 'node/' . $node->nid;
  }if (isset($_POST['op']) && $_POST['op'] == t('Unpublish')) {
    $node->status = 0;
    node_save($node);
    drupal_set_message(t("This Autoforfait quiz is currently unpublished. After finishing your changes to the Autoforfait quiz, use the <em>Publish</em> button below."));
    return 'node/' . $node->nid;
  }
  elseif (isset($_POST['op']) && $_POST['op'] == t('Add')) {
    $component = $form_state['values']['add'];
    $form_state['redirect'] = array(
      'node/' . $node->nid . '/dquarks/components/new/' . $component['type'],
      array(
        'query' => array(
          'name' => $component['name'],
          'mandatory' => $component['mandatory'],
          'email' => $component['email'],
          'pid' => $component['pid'],
          'weight' => $component['weight'],
        ),
      ),
    );
  }
  else {
    drupal_set_message(t('The component positions and mandatory values have been updated.'));

    // Since dquarks components have been updated but the node itself has not
    // been saved, it is necessary to explicitly clear the cache to make sure
    // the updated dquarks is visible to anonymous users.
    cache_clear_all();

    // Clear the entity cache if Entity Cache module is installed.
    if (module_exists('entitycache')) {
      cache_clear_all($node->nid, 'cache_entity_node');
    }
  }
}

/**
 *  Edit component.
 *
 * @param array $form
 *   An array of form items.
 * @param array $form_state
 *   An array of form values items.
 * @param object $node
 *   The node id.
 * @param array $component
 *   The component of quiz.
 * @param boolean $clone
 *   True, if a copy of component.
 *
 * @return array
 *   The details of component.
 */
function dquarks_component_edit_form($form, $form_state, $node, $component, $clone = FALSE) {
  drupal_set_title(t('Edit component: @name (@type)', array('@name' => $component['name'], '@type' => t($component['type']))), PASS_THROUGH);

  $form['#attached']['library'][] = array('dquarks', 'admin');
  $form['#tree'] = TRUE;

  // Print the correct field type specification.
  // We always need: name and description.
  $form['type'] = array(
    '#type' => 'value',
    '#value' => $component['type'],
  );
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );
  $form['cid'] = array(
    '#type' => 'value',
    '#value' => isset($component['cid']) ? $component['cid'] : NULL,
  );
  $form['clone'] = array(
    '#type' => 'value',
    '#value' => $clone,
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#default_value' => $component['name'],
    '#title' => t('Label'),
    '#description' => t('This is used as a descriptive label when displaying this quiz element.'),
    '#required' => TRUE,
    '#weight' => -2,
    '#maxlength' => 255,
  );

  $form['extra']['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($component['extra']['description']) ? $component['extra']['description'] : '',
    '#title' => t('Description'),
    '#description' => t('A short description of the field used as help for the user when he/she uses the form.') . theme('dquarks_token_help'),
    '#weight' => -1,
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => FALSE,
    '#weight' => 4,
  );

  $form['advanced']['form_key'] = array(
    '#type' => 'textfield',
    '#default_value' => empty($component['form_key']) ? _dquarks_safe_name($component['name']) : $component['form_key'],
    '#title' => t('Field Key'),
    '#description' => t('Enter a machine readable key for this form element. May contain only alphanumeric characters and underscores. This key will be used as the name attribute of the form element. This value has no effect on the way data is saved, but may be helpful if doing custom form processing.'),
    '#required' => TRUE,
    '#weight' => -9,
  );
  $form['advanced']['mandatory'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mandatory'),
    '#default_value' => ($component['mandatory'] == '1' ? TRUE : FALSE),
    '#description' => t('Check this option if the user must enter a value.'),
    '#weight' => 2,
    '#access' => !in_array($component['type'], array('direction')),
  );
  $form['advanced']['email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include in e-mails'),
    '#default_value' => ($component['email'] == '1' ? TRUE : FALSE),
    '#description' => t('If checked, submitted values from this component will be included in e-mails.'),
    '#weight' => 2,
    '#access' => !in_array($component['type'], array('direction')),
  );

  if (variable_get('dquarks_enable_fieldset', TRUE) && is_array($node->dquarks['components'])) {
    $options = array('0' => t('Root'));
    foreach ($node->dquarks['components'] as $existing_cid => $value) {
      if ($value['type'] == 'fieldset' && (!isset($component['cid']) || $existing_cid != $component['cid'])) {
        $options[$existing_cid] = $value['name'];
      }
    }
    $form['advanced']['pid'] = array(
      '#type' => 'select',
      '#title' => t('Parent Fieldset'),
      '#default_value' => $component['pid'],
      '#description' => t('Optional. You may organize your form by placing this component inside another fieldset.'),
      '#options' => $options,
      '#weight' => 3,
    );
  }
  $form['advanced']['weight'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#title' => t('Weight'),
    '#default_value' => $component['weight'],
    '#description' => t('Optional. In the menu, the heavier items will sink and the lighter items will be positioned nearer the top.'),
    '#weight' => 4,
  );

  // Add the fields specific to this component type:
  // Load all component types.
  dquarks_load_components();
  $edit_function = '_dquarks_edit_' . $component['type'];
  $additional_form_elements = array();
  if (function_exists($edit_function)) {
    // Call the component render function.
    $additional_form_elements = $edit_function($component, $node, $form_state);
  }
  else {
    drupal_set_message(t('The Autoforfait quiz component of type @type does not have an edit function defined.', array('@type' => $component['type'])));
  }

  // Merge the additional fields with the current fields:
  if (isset($additional_form_elements['extra'])) {
    $extra_fields_copy = $form['extra'];
    $form['extra'] = array_merge($extra_fields_copy, $additional_form_elements['extra']);
    unset($additional_form_elements['extra']);
  }

  if (isset($additional_form_elements['advanced'])) {
    $advanced_fields_copy = $form['advanced'];
    $form['advanced'] = array_merge($advanced_fields_copy, $additional_form_elements['advanced']);
    unset($additional_form_elements['advanced']);
  }

  $form = array_merge($form, $additional_form_elements);

  // Add the submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
  );
  $form['apply'] = array(
    '#type' => 'submit',
    '#value' => t('Apply'),
    '#weight' => 6,
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#weight' => 7,
  );
  return $form;
}

/**
 * Field name validation for the dquarks unique key. Must be alphanumeric.
 */
function dquarks_component_edit_form_validate($form, &$form_state) {
  $node = node_load($form_state['values']['nid']);

  if (!preg_match('/^[a-z0-9_]+$/i', $form_state['values']['form_key'])) {
    form_set_error('form_key', t('The field key %field_key is invalid. Please include only lowercase alphanumeric characters and underscores.', array('%field_key' => $form_state['values']['form_key'])));
  }

  foreach ($node->dquarks['components'] as $cid => $component) {
    if (($component['cid'] != $form_state['values']['cid'] || $form_state['values']['clone']) && ($component['pid'] == $form_state['values']['pid']) && (strcasecmp($component['form_key'], $form_state['values']['form_key']) == 0)) {
      form_set_error('form_key', t('The field key %field_key is already in use by the field labeled %existing_field. Please use a unique key.', array('%field_key' => $form_state['values']['form_key'], '%existing_field' => $component['name'])));
    }
  }

  // Let the field do any additional validation.
  dquarks_load_components($form_state['values']['type']);
  $validate_function = '_dquarks_edit_validate_' . $form_state['values']['type'];
  if (function_exists($validate_function)) {
    $validate_function($form, $form_state);
  }
}

/**
 * Implements hook_submit().
 *
 * @param array $form
 *   An array of form items.
 * @param array $form_state
 *   An array of form values items.
 */
function dquarks_component_edit_form_submit($form, &$form_state) {

  // Ensure a dquarks record exists.
  $node = node_load($form_state['values']['nid']);
  dquarks_ensure_record($node);

  if (isset($_POST['op']) && $_POST['op'] == t('Add')) {
    $rule = $form_state['values']['add'];
    $form_state['redirect'] = array('node/' . $node->nid . '/dquarks/components/' . $rule['cid'] . '/addrule', 'test_name=' . urlencode($rule['test_name']) . '&test_type=' . $rule['test_type'] . '&tested_field=' . $rule['tested_field'] . '&testvalue=' . urlencode($rule['testvalue']) . '&next_jump=' . $rule['next_jump'] . '&weight=' . $rule['weight']);
  }
  elseif (isset($_POST['op']) && $_POST['op'] == t('Cancel')) {
    if (isset($form_state['cid']['#value'])) {
      $cid = $form_state['cid']['#value'];
      $form_state['redirect'] = array('node/' . $node->nid . '/dquarks/components/' . $cid);
    }
    else {
      $form_state['redirect'] = array('node/' . $node->nid . '/dquarks/components/');
    }
  }
  elseif (isset($_POST['op']) && $_POST['op'] === t('Add Rule')) {
    $cid = $form_state['values']['cid'];
    $component = $node->dquarks['components'][$cid];
    _dquarks_addrule_direction($form_state, $node, $component);
  }
  else {
    // Remove empty extra values.
    if (isset($form_state['values']['extra'])) {
      foreach ($form_state['values']['extra'] as $key => $value) {
        if ($value === '') {
          unset($form_state['values']['extra'][$key]);
        }
      }
    }

    // Remove empty attribute values.
    if (isset($form_state['values']['extra']['attributes'])) {
      foreach ($form_state['values']['extra']['attributes'] as $key => $value) {
        if ($value === '') {
          unset($form_state['values']['extra']['attributes'][$key]);
        }
      }
    }
    // Remove empty choice values.
    if (isset($form_state['values']['extra']['items']['labels'])) {
      $items = array('choices', 'labels', 'notes');
      foreach ($items as $keys) {
        unset($form_state['values']['extra']['items'][$keys]['add']);
      }
    }
    $cid = $form['cid']['#value'];
    if ($form_state['values']['clone']) {
      dquarks_component_clone($node, $form_state['values']);
      drupal_set_message(t('Component %name cloned.', array('%name' => $form_state['values']['name'])));
    }
    elseif (!empty($form_state['values']['cid'])) {
      dquarks_component_update($form_state['values']);
      drupal_set_message(t('Component %name updated.', array('%name' => $form_state['values']['name'])));
    }
    else {
      $cid = dquarks_component_insert($form_state['values']);
      drupal_set_message(t('New component %name added.', array('%name' => $form_state['values']['name'])));
    }

    // Since dquarks components have been updated but the node itself has not
    // been saved, it is necessary to explicitly clear the cache to make sure
    // the updated dquarks is visible to anonymous users.
    cache_clear_all();

    // Clear the entity cache if Entity Cache module is installed.
    if (module_exists('entitycache')) {
      cache_clear_all($node->nid, 'cache_entity_node');
    }
    if (isset($_POST['op']) && $_POST['op'] == t('Apply')) {
      $form_state['redirect'] = array('node/' . $node->nid . '/dquarks/components', array('query' => array('cid' => $cid)));
    }
    else {
      $form_state['redirect'] = array('node/' . $node->nid . '/dquarks/components', isset($cid) ? array('query' => array('cid' => $cid)) : array());
    }
  }
}

/**
 * Return a confirmation message to delete submission.
 *
 * @param array $form
 *   An array of form items.
 * @param array $form_state
 *   An array of form values items.
 * @param object $node
 *   The node id.
 * @param array $component
 *   The current component.
 *
 * @return string
 *   Message of confirmation
 */
function dquarks_component_delete_form($form, $form_state, $node, $component) {
  $cid = $component['cid'];

  $form = array();
  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );
  $form['component'] = array(
    '#type' => 'value',
    '#value' => $component,
  );

  if (dquarks_component_feature($node->dquarks['components'][$cid]['type'], 'group')) {
    $question = t('Delete the %name fieldset?', array('%name' => $node->dquarks['components'][$cid]['name']));
    $description = t('This will immediately delete the %name fieldset and all children elements within %name from the %dquarks dquarks. This cannot be undone.', array('%name' => $node->dquarks['components'][$cid]['name'], '%dquarks' => $node->title));
  }
  else {
    $question = t('Delete the %name component?', array('%name' => $node->dquarks['components'][$cid]['name']));
    $description = t('This will immediately delete the %name component from the %dquarks dquarks. This cannot be undone.', array('%name' => $node->dquarks['components'][$cid]['name'], '%dquarks' => $node->title));
  }

  return confirm_form($form, $question, 'node/' . $node->nid . '/dquarks/components', $description, t('Delete'));
}

/**
 * Implements hook_submit().
 *
 * @param array $form
 *   An array of form items.
 * @param array $form_state
 *   An array of form values items.
 */
function dquarks_component_delete_form_submit($form, &$form_state) {
  // Delete the component.
  $node = $form_state['values']['node'];
  $component = $form_state['values']['component'];
  dquarks_component_delete($node, $component);
  drupal_set_message(t('Component %name deleted.', array('%name' => $component['name'])));

  // Check if this dquarks still contains any information.
  unset($node->dquarks['components'][$component['cid']]);
  dquarks_check_record($node);

  // Since dquarks components have been updated but the node itself has not
  // been saved, it is necessary to explicitly clear the cache to make sure
  // the updated dquarks is visible to anonymous users.
  cache_clear_all();

  // Clear the entity cache if Entity Cache module is installed.
  if (module_exists('entitycache')) {
    cache_clear_all($node->nid, 'cache_entity_node');
  }

  $form_state['redirect'] = 'node/' . $node->nid . '/dquarks/components';
}

/**
 * Insert a new component into the database.
 *
 * @param array $component
 *   A full component containing fields from the component form.
 */
function dquarks_component_insert(&$component) {
  // Allow modules to modify the component before saving.
  foreach (module_implements('dquarks_component_presave') as $module) {
    $function = $module . '_dquarks_component_presave';
    $function($component);
  }

  $component['value'] = isset($component['value']) ? $component['value'] : NULL;
  $component['mandatory'] = isset($component['mandatory']) ? $component['mandatory'] : 0;

  if (!isset($component['cid'])) {
    if (lock_acquire('dquarks_component_insert_' . $component['nid'], 5)) {
      $next_id_query = db_select('dquarks_component')->condition('nid', $component['nid']);
      $next_id_query->addExpression('MAX(cid) + 1', 'cid');
      $component['cid'] = $next_id_query->execute()->fetchField();
      if ($component['cid'] == NULL) {
        $component['cid'] = 1;
      }
      lock_release('dquarks_component_insert_' . $component['nid']);
    }
    else {
      watchdog('dquarks', 'A dquarks component could not be saved because a timeout occurred while trying to acquire a lock for the node. Details: <pre>@component</pre>', array('@component' => print_r($component, TRUE)));
      return FALSE;
    }
  }

  db_insert('dquarks_component')
          ->fields(array(
            'nid' => $component['nid'],
            'cid' => $component['cid'],
            'pid' => $component['pid'],
            'form_key' => $component['form_key'],
            'name' => $component['name'],
            'type' => $component['type'],
            'value' => (string) $component['value'],
            'extra' => serialize($component['extra']),
            'mandatory' => (int) $component['mandatory'],
            'weight' => $component['weight'],
            'email' => $component['email'],
          ))
          ->execute();

  // Post-insert actions.
  module_invoke_all('dquarks_component_insert', $component);

  return $component['cid'];
}

/**
 * Update an existing component with new values.
 *
 * @param array $component
 *   A full component containing a nid, cid, and all other fields from the
 *   component form. Additional properties are stored in the extra array.
 */
function dquarks_component_update($component) {
  // Allow modules to modify the component before saving.
  foreach (module_implements('dquarks_component_presave') as $module) {
    $function = $module . '_dquarks_component_presave';
    $function($component);
  }

  $component['value'] = isset($component['value']) ? $component['value'] : NULL;
  $component['mandatory'] = isset($component['mandatory']) ? $component['mandatory'] : FALSE;
  db_update('dquarks_component')
          ->fields(array(
            'pid' => $component['pid'],
            'form_key' => $component['form_key'],
            'name' => $component['name'],
            'type' => $component['type'],
            'value' => isset($component['value']) ? $component['value'] : '',
            'extra' => serialize($component['extra']),
            'mandatory' => (int) $component['mandatory'],
            'weight' => $component['weight'],
            'email' => $component['email'],
          ))
          ->condition('nid', $component['nid'])
          ->condition('cid', $component['cid'])
          ->execute();

  // Post-update actions.
  $update_function = '_dquarks_after_update_' . $component['type'];
  if (function_exists($update_function)) {
    $update_function($component);
  }


  module_invoke_all('dquarks_component_update', $component);
}

/**
 * Delete the component already selected.
 *
 * @param object $node
 *   The node id.
 * @param array $component
 *   The component to delete.
 */
function dquarks_component_delete($node, $component) {
  // Check if a delete function is available for this component. If so,
  // load all submissions and allow the component to delete each one.
  dquarks_component_include($component['type']);
  $delete_function = '_dquarks_delete_' . $component['type'];
  if (function_exists($delete_function)) {
    require_once  drupal_get_path('module', 'dquarks') . '/includes/dquarks.submissions.inc';
    $submissions = dquarks_get_submissions($node->nid);
    foreach ($submissions as $submission) {
      if (isset($submission->data[$component['cid']])) {
        dquarks_component_invoke($component['type'], 'delete', $component, $submission->data[$component['cid']]['value']);
      }
    }
  }

  // Remove database entries.
  db_delete('dquarks_component')
          ->condition('nid', $node->nid)
          ->condition('cid', $component['cid'])
          ->execute();
  db_delete('dquarks_submitted_data')
          ->condition('nid', $node->nid)
          ->condition('cid', $component['cid'])
          ->execute();

  // Delete all elements under this element.
  $result = db_select('dquarks_component', 'c')
          ->fields('c')
          ->condition('nid', $node->nid)
          ->condition('pid', $component['cid'])
          ->execute();
  foreach ($result as $row) {
    $child_component = $node->dquarks['components'][$row->cid];
    dquarks_component_delete($node, $child_component);
  }

  // Post-delete actions.
  module_invoke_all('dquarks_component_delete', $component);
}

/**
 * Recursively insert components into the database.
 *
 * @param object $node
 *   The node object containing the current dquarks.
 * @param array $component
 *   A full component containing fields from the component form.
 */
function dquarks_component_clone(&$node, &$component) {
  $original_cid = $component['cid'];
  unset($component['cid']);
  $new_cid = dquarks_component_insert($component);
  if ($component['type'] == 'fieldset') {
    foreach ($node->dquarks['components'] as $cid => $child_component) {
      if ($child_component['pid'] == $original_cid) {
        $child_component['pid'] = $new_cid;
        dquarks_component_clone($node, $child_component);
      }
    }
  }
  return $new_cid;
}

/**
 * Check if a component has a particular feature.
 *
 * @see hook_dquarks_component_info()
 */
function dquarks_component_feature($type, $feature) {
  $components = dquarks_components();
  $defaults = array(
    'csv' => TRUE,
    'email' => TRUE,
    'email_address' => FALSE,
    'email_name' => FALSE,
    'required' => TRUE,
    'title_display' => TRUE,
    'title_inline' => TRUE,
    'conditional' => TRUE,
    'spam_analysis' => FALSE,
    'group' => FALSE,
    'attachment' => FALSE,
    'private' => TRUE,
  );
  return isset($components[$type]['features'][$feature]) ? $components[$type]['features'][$feature] : !empty($defaults[$feature]);
}

/**
 * Create a list of components suitable for a select list.
 *
 * @param object $node
 *   The dquarks node.
 * @param array $component_filter
 *   Either an array of components, or a string containing a feature name (csv,
 *   email, required, conditional) on which this list of components will be
 *   restricted.
 * @param boolean $indent
 *   Indent components placed under fieldsets with hyphens.
 * @param boolean $optgroups
 *   Determine if pagebreaks should be converted to option groups in the
 *   returned list of options.
 */
function dquarks_component_list($node, $component_filter = NULL, $indent = TRUE, $optgroups = FALSE) {
  $options = array();
  $page_names = array();

  $components = is_array($component_filter) ? $component_filter : $node->dquarks['components'];
  $feature = is_string($component_filter) ? $component_filter : NULL;

  foreach ($components as $cid => $component) {
    if (!isset($feature) || dquarks_component_feature($component['type'], $feature) || ($indent && dquarks_component_feature($component['type'], 'group'))) {
      $prefix = '';
      $page_num = $component['page_num'];
      $page_index = 'p' . $page_num;
      if ($indent && ($parent_count = count(dquarks_component_parent_keys($node, $component)) - 1)) {
        $prefix = str_repeat('-', $parent_count);
      }
      if ($optgroups && $component['type'] == 'pagebreak') {
        $page_names[$page_index] = $component['name'];
      }
      elseif ($optgroups && $page_num > 1) {
        $options[$page_index][$cid] = $prefix . $component['name'];
      }
      else {
        $options[$cid] = $prefix . $component['name'];
      }
    }
  }

  // Convert page breaks into optgroups.
  if ($optgroups) {
    $grouped_options = $options;
    $options = array();
    foreach ($grouped_options as $key => $values) {
      if (is_array($values) && isset($page_names[$key])) {
        $options[$page_names[$key]] = $values;
      }
      else {
        $options[$key] = $values;
      }
    }
  }

  return $options;
}

/**
 * A Form API process function to expand a component list into checkboxes.
 */
function dquarks_component_select($element) {
  // Split the select list into checkboxes.
  foreach ($element['#options'] as $key => $label) {
    $indents = 0;
    $label = preg_replace('/^([\-])+/', '', $label, -1, $indents);
    $element[$key] = array(
      '#title' => $label,
      '#type' => 'checkbox',
      '#default_value' => array_search($key, $element['#value']) !== FALSE,
      '#return_value' => $key,
      '#parents' => array_merge($element['#parents'], array($key)),
      '#indent' => $indents,
    );
  }

  $element['#theme_wrappers'] = array();
  $element['#type'] = 'dquarks_component_select';
  $element['#theme'] = 'dquarks_component_select';
  $element['#attached'] = array(
    'library' => array(
      array('dquarks', 'admin'),
    ),
    'js' => array(
      'misc/tableselect.js' => array(),
    ),
  );

  return $element;
}

/**
 * Theme the contents of a dquarks component select element.
 */
function theme_dquarks_component_select($variables) {
  $element = $variables['element'];

  $rows = array();
  $header = array();
  if (!isset($element['#all_checkbox']) || $element['#all_checkbox']) {
    $header = array(array('class' => array('select-all'), 'data' => ' ' . t('Include all components')));
  }
  foreach (element_children($element) as $key) {
    $rows[] = array(
      theme('indentation', array('size' => $element[$key]['#indent'])) . drupal_render($element[$key]),
    );
  }

  $element['#type'] = 'fieldset';
  $element['#value'] = NULL;
  $element['#attributes']['class'] = array('dquarks-component-select-table');
  if (!isset($element['#collapsible']) || $element['#collapsible']) {
    $element['#attributes']['class'][] = 'collapsible';
  }
  if (!isset($element['#collapsed']) || $element['#collapsed']) {
    $element['#attributes']['class'][] = 'collapsed';
  }

  if (empty($rows)) {
    $element['#children'] = t('No available components.');
  }
  else {
    $element['#children'] = '<div class="dquarks-component-select-wrapper">' . theme('table', array('header' => $header, 'rows' => $rows)) . '</div>';
  }

  return theme('fieldset', array('element' => $element));
}

/**
 * Find a components parents within a node.
 */
function dquarks_component_parent_keys($node, $component) {
  $parents = array($component['form_key']);
  $pid = $component['pid'];
  while ($pid) {
    $parents[] = $node->dquarks['components'][$pid]['form_key'];
    $pid = $node->dquarks['components'][$pid]['pid'];
  }
  return array_reverse($parents);
}

/**
 * Populate a component with the defaults for that type.
 */
function dquarks_component_defaults(&$component) {

  dquarks_load_components();

  $function = '_dquarks_defaults_' . $component['type'];
  if (function_exists($function)) {
    $defaults = $function();
    foreach ($defaults as $key => $default) {
      if (!isset($component[$key])) {
        $component[$key] = $default;
      }
    }
    foreach ($defaults['extra'] as $extra => $default) {
      if (!isset($component['extra'][$extra])) {
        $component['extra'][$extra] = $default;
      }
    }
  }
}

/**
 * Validate an element value is unique with no duplicates in the database.
 */
function dquarks_validate_unique($element, $form_state) {
  if ($element['#value'] !== '') {
    $nid = $form_state['values']['details']['nid'];
    $sid = $form_state['values']['details']['sid'];
    $count = db_select('dquarks_submitted_data')
            ->fields('dquarks_submitted_data', array('sid'))
            ->condition('nid', $nid)
            ->condition('cid', $element['#dquarks_component']['cid'])
            ->condition('sid', $sid, '<>')
            ->condition('data', $element['#value'])
            ->range(0, 1)
            ->execute()
            ->fetchField();
    if ($count) {
      form_error($element, t('The value %value has already been submitted once for the %title field. You may have already submitted this form, or you need to use a different value.', array('%value' => $element['#value'], '%title' => $element['#title'])));
    }
  }
}

/**
 * Return the id of choice / question to the ajax callback.
 *
 * @param int $id
 *   The id of question to delete.
 */
function dquarks_component_edit_callback($id) {
  $_POST["del_question"] = $id;
  // Call the submit function.
  $page_callback_result = ajax_form_callback();
  $commands = ajax_prepare_response($page_callback_result);
  $page = array('#type' => 'ajax', '#commands' => $commands);
  ajax_deliver($page);
}

/**
 * Define rules for a component.
 *
 * @param array $form_state
 *   The current form values.
 * @param object $node
 *   The node id.
 * @param array $component
 *   The current component.
 *
 * @return Array
 *   Call to a function or display message
 */
function dquarks_component_addrule_form(&$form_state, $node, $component) {
  // Load all component types.
  dquarks_load_components();
  $addrule_function = '_dquarks_addrule_' . $component['type'];
  if (function_exists($addrule_function)) {
    // Call the component render function.
    return $addrule_function($form_state, $node, $component);
  }
  else {
    drupal_set_message(t('The Dquarks component of type @type does not have an addrule function defined.', array('@type' => $component['type'])));
  }
}

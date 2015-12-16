<?php
/**
 * @file
 * Callbacks & helper functions for the adminisration pages of uc_conditional_attributes.
 */

/**
 * Generates the form to display when adding or editing a definition.
 */
function uc_conditional_attributes_admin_general_form($form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'uc_conditional_attributes') . '/uc_conditional_attributes.css');
  drupal_add_js(drupal_get_path('module', 'uc_conditional_attributes') . '/uc_conditional_attributes.js');

  $form = array();
  // Ubercart has the default product class to '0', grr. This conflicts with
  // the unset checkbox return value.
  $product_classes = uc_conditional_attributes_get_product_classes();
  $product_classes['product'] = $product_classes['0'];
  unset($product_classes['0']);
  $form['product_classes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Product classes'),
    '#description' => t('Select one or more product classes that this definition should be added to.'),
    '#required' => TRUE,
    '#options' => $product_classes,
  );
  $options = uc_conditional_attributes_get_parent_attributes();
  $form['parent_attr'] = array(
    '#type' => 'select',
    '#title' => t('Select parent attribute'),
    '#options' => $options,
    '#ajax' => array(
      'callback' => 'uc_conditional_attributes_replace_parent_options',
      'wrapper' => 'uc_conditional_attributes_parent_options',
      'method' => 'replace',
    ),
  );
  $option_keys = array_reverse(array_keys($options), TRUE);
  $form['parent_attr_options'] = array(
    '#type' => 'select',
    '#title' => t('Select parent attribute\'s option'),
    '#options' => uc_conditional_attributes_get_attribute_options(isset($form_state['values']['parent_attr']) ? $form_state['values']['parent_attr'] : array_pop($option_keys)),
    '#multiple' => TRUE,
    '#prefix' => '<div id="uc_conditional_attributes_parent_options">',
    '#suffix' => '</div>',
  );
  $form['type'] = array(
    '#type' => 'radios',
    '#title' => t('Type of dependency'),
    '#options' => array(
      'disable' => t('Disable'),
      'enable' => t('Enable'),
    ),
    '#default_value' => 'disable',
    '#description' => t('Hold ctrl + click to select multiple values.'),
  );
  $form['dependent_attr'] = array(
    '#type' => 'select',
    '#title' => t('Select dependent attributes'),
    '#options' => uc_conditional_attributes_get_all_attributes(),
    '#multiple' => TRUE,
    '#description' => t('Hold ctrl + click to select multiple values.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Definitions'),
  );
  return $form;
}

function uc_conditional_attributes_replace_parent_options(&$form, &$form_state) {
  return $form['parent_attr_options'];
}

/**
 * Form submission callback for uc_conditional_attributes_admin_general_form().
 */
function uc_conditional_attributes_admin_general_form_submit($form, $form_state) {
  $product_classes = uc_conditional_attributes_get_product_classes();
  $invalid_dependency = FALSE;
  $multiple_dependency = FALSE;

  // Ubercart's default '0' product class ID fix, part 2.
  // @see uc_conditional_attributes_admin_general_form()
  $product_classes = $form_state['values']['product_classes'];
  $product_classes['0'] = $product_classes['product'];
  unset($product_classes['product']);
  $selected_product_classes = array_keys(array_filter($product_classes));
  foreach ($selected_product_classes as $pcid) {
    $aid = $form_state['values']['parent_attr'];
    $oid_arr = $form_state['values']['parent_attr_options'];
    $type = $form_state['values']['type'];
    foreach ($form_state['values']['dependent_attr'] as $dependent_aid) {
      //if this definition does not already exist
      if ($aid != $dependent_aid) {
        foreach ($oid_arr as $oid => $oid_name) {
          $parent_aid = db_query("SELECT aid FROM {uc_conditional_attributes} WHERE dependent_aid = :dependent_aid AND pcid = :pcid", array(':dependent_aid' => $dependent_aid, ':pcid' => $pcid))->fetchField();
          if (!$parent_aid) {
            db_insert('uc_conditional_attributes')
              ->fields(array(
              'aid' => $aid,
              'oid' => $oid,
              'dependent_aid' => $dependent_aid,
              'type' => $type,
              'pcid' => $pcid,
              ))
            ->execute();
          }
          elseif ($parent_aid == $aid) {
            if (!db_query("SELECT def_id FROM {uc_conditional_attributes} WHERE dependent_aid = :dependent_aid AND oid = :oid AND pcid = :pcid", array(':dependent_aid' => $dependent_aid, ':oid' => $oid, ':pcid' => $pcid))->fetchField()) {
              db_insert('uc_conditional_attributes')
                ->fields(array(
                'aid' => $aid,
                'oid' => $oid,
                'dependent_aid' => $dependent_aid,
                'type' => $type,
                'pcid' => $pcid,
                ))
            ->execute();
            }
          }
          else {
            $multiple_dependency = TRUE;
          }
        }
      }
      else {
        $invalid_dependency = TRUE;
      }
    }
  }
  drupal_set_message(t('Definitions updated successfully.'));
  if ($invalid_dependency) {
    drupal_set_message(t('Definition could not be added because two attributes cannot be dependent on each other.'), 'warning');
  }
  if ($multiple_dependency) {
    drupal_set_message(t('One attribute can not be dependent on multiple attributes.'), 'warning');
  }
}

/**
 * Display currently defined chained attributes
 */
function uc_conditional_attributes_definitions() {
  drupal_add_css(drupal_get_path('module', 'uc_conditional_attributes') . '/uc_conditional_attributes.css');
  drupal_add_js(drupal_get_path('module', 'uc_conditional_attributes') . '/uc_conditional_attributes.js');

  $product_classes = uc_conditional_attributes_get_product_classes();

  foreach ($product_classes as $pcid => $name) {
    $form[$pcid] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($name),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form[$pcid][$name] = array(
      '#type' => 'item',
      '#markup' => uc_conditional_attributes_definitions_grid($pcid),
    );
  }
  // rendering the output as form just because to show multiple product class with in fieldsets
  return drupal_render($form);
}

/**
 * Return definitions grid of a specific product class
 * @pcid : product class id
 */
function uc_conditional_attributes_definitions_grid($pcid) {
  $result = db_query("SELECT c.def_id, a1.name AS parent_attr, a2.name AS dependent_attr, o.name, c.pcid, c.type
                    FROM {uc_conditional_attributes} c
                    INNER JOIN {uc_attributes} a1 ON a1.aid = c.aid
                    INNER JOIN {uc_attributes} a2 ON a2.aid = c.dependent_aid
                    INNER JOIN {uc_attribute_options} o ON o.oid = c.oid
                    WHERE c.pcid = :pcid", array(':pcid' => $pcid));

  $rows = array();
  $head = array(
    t('Parent Attribute'),
    t('Parent Attribute\'s Option'),
    t('Dependent Attribute'),
    t('Type'),
    t('Actions'),
  );
  foreach ($result as $rec) {
    $rows[] = array(
      $rec->parent_attr,
      $rec->name,
      $rec->dependent_attr,
      $rec->type,
      l(t('Delete'), 'admin/store/settings/uc_conditional_attributes/definitions/delete/' . $rec->def_id, array(array('query' => 'destination=' . 'admin/store/settings/uc_conditional_attributes/definitions'))),
    );
  }
  return theme('table', array('header' => $head, 'rows' => $rows));
}

/**
 * Deletes a definition.
 *
 * TODO: Use menu argument loaders (e.g. implement foo_load()) to do away with
 * this function that serves only to check if the definition ID is value.
 */
function uc_conditional_attributes_delete_definition() {
  if (!arg(6) || !is_numeric(arg(6))) {
    drupal_access_denied();
    exit;
  }
  return drupal_get_form("uc_conditional_attributes_delete_definition_confirm_form");
}

/**
 * Form builder callback to confirm definition deletion.
 */
function uc_conditional_attributes_delete_definition_confirm_form($form) {
  return confirm_form(array(), t('Are you sure you want to delete this definition?'), 'admin/store/settings/uc_conditional_attributes', NULL, t('Delete'), t('Cancel'), 'confirm');
}

/**
 * Form submission callback for uc_conditional_attributes_delete_definition_confirm_form().
 */
function uc_conditional_attributes_delete_definition_confirm_form_submit($form, &$form_state) {
  db_delete('uc_conditional_attributes')
    ->condition('def_id', arg(6))
    ->execute();
  drupal_set_message(t('Definition has been deleted successfully.'));
  $form_state['redirect'] = 'admin/store/settings/uc_conditional_attributes';
}

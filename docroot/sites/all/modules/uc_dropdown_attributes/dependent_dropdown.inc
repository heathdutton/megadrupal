<?php

/**
 * @file
 * Administrative interface for specifying the attribute dependencies.
 *
 * These functions supply the administrative interface for specifying the
 * attribute dependencies using AJAX.  Based on code from the Examples module.
 */

/**
 * Form constructor for the uc_dropdown_attributes_product form.
 *
 * Administrative form for specifying the product attribute dependencies.
 *
 * @param object $product
 *   The product with the attribute dependencies.
 *
 * @see uc_dropdown_attributes_product_submit()
 * @see uc_dropdown_attributes_dependent_callback()
 * @see theme_uc_dropdown_attributes_product()
 * @ingroup forms
 */
function uc_dropdown_attributes_product($form, &$form_state, $product) {
  $nid = $product->nid;
  $attributes = uc_product_get_attributes($nid);
  $query = 'SELECT aid, parent_aid, parent_values, required
    FROM {uc_dropdown_attributes} WHERE nid=:nid';
  $dependencies = db_query($query, array(':nid' => $nid));

  _uc_dropdown_attributes_form($form, $form_state, $attributes, $dependencies);

  // Check for overriding product class dropdowns.
  $type = uc_dropdown_attributes_dependency_type($nid);
  if (!is_null($type) && $type == 'class') {
    $form['submit']['#value'] = t('Override product class');
  }

  return $form;
}

/**
 * Form submission handler for uc_dropdown_attributes_product().
 *
 * Write form values out to the database table.
 */
function uc_dropdown_attributes_product_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-submit') {
    $form_state['rebuild'] = FALSE;
    $nid = $form_state['build_info']['args'][0]->nid;

    $values = $form_state['values']['attributes'];
    $attributes = _uc_dropdown_attributes_extract_attributes($values);

    $result = db_delete('uc_dropdown_attributes')
              ->condition('nid', $nid)
              ->execute();
    drupal_set_message(t('Processing node :nid', array(':nid' => $nid)));
    foreach ($attributes as $aid => $attribute) {
      if ($attribute->parent_aid != 0) {
        uc_dropdown_attributes_product_create_dependency(
          $nid,
          $aid,
          $attribute->parent_aid,
          $attribute->parent_values,
          $attribute->required
        );
        drupal_set_message(t('Saved :name',
          array(':name' => $attribute->name)));
      }
    }
  }

  // edit-next or anything else will cause rebuild.
  $form_state['rebuild'] = TRUE;
}

/**
 * Form theme handler for uc_dropdown_attributes_product().
 */
function theme_uc_dropdown_attributes_product($variables) {
  return _uc_dropdown_attributes_construct_table($variables['form']);
}

/**
 * Form constructor for the uc_dropdown_attributes classes form.
 *
 * Administrative form for specifying the attribute dependencies for classes.
 *
 * @param string $class
 *   The class with the attribute dependencies.
 *
 * @see uc_dropdown_attributes_class_submit()
 * @see uc_dropdown_attributes_dependent_callback()
 * @see theme_uc_dropdown_attributes_class()
 * @ingroup forms
 */
function uc_dropdown_attributes_class($form, &$form_state, $class) {
  $attributes = uc_class_get_attributes($class);
  $query = 'SELECT aid, parent_aid, parent_values, required
    FROM {uc_dropdown_classes} WHERE pcid=:pcid';
  $dependencies = db_query($query, array(':pcid' => $class));

  _uc_dropdown_attributes_form($form, $form_state, $attributes, $dependencies);

  return $form;
}

/**
 * Form submission handler for uc_dropdown_attributes_class().
 *
 * Write form values out to the database table.
 */
function uc_dropdown_attributes_class_submit($form, &$form_state) {
  if ($form_state['clicked_button']['#id'] == 'edit-submit') {
    $form_state['rebuild'] = FALSE;
    $pcid = $form_state['build_info']['args'][0];

    $values = $form_state['values']['attributes'];
    $attributes = _uc_dropdown_attributes_extract_attributes($values);

    $result = db_delete('uc_dropdown_classes')
      ->condition('pcid', $pcid)
      ->execute();
    drupal_set_message(t('Processing class :pcid', array(':pcid' => $pcid)));
    foreach ($attributes as $aid => $attribute) {
      if ($attribute->parent_aid != 0) {
        uc_dropdown_attributes_class_create_dependency(
          $pcid,
          $aid,
          $attribute->parent_aid,
          $attribute->parent_values,
          $attribute->required
        );
        drupal_set_message(t('Saved :name',
          array(':name' => $attribute->name)));
      }
    }
  }

  // edit-next or anything else will cause rebuild.
  $form_state['rebuild'] = TRUE;
}

/**
 * Form theme handler for uc_dropdown_attributes_class().
 */
function theme_uc_dropdown_attributes_class($variables) {
  return _uc_dropdown_attributes_construct_table($variables['form']);
}

/**
 * Internal form constructor for administration of attribute dependencies.
 *
 * This function takes the data retrieved for products and classes and
 * constructs the dependencies administration form.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The values and state of the form.
 * @param array $attributes
 *   The attributes for this node or product class.
 * @param object $dependencies
 *   The dependency information for the node or product class from the database.
 */
function _uc_dropdown_attributes_form(&$form, $form_state, $attributes,
  $dependencies) {

  $parent = array();
  $values = array();
  $required = array();
  if (isset($form_state['values'])) {
    foreach ($form_state['values']['attributes'] as $key => $attribute) {
      $parent[$key] = $attribute['parent'];
      $values[$key] = $attribute['values'];
      $required[$key] = $attribute['required'];
    }
  }
  else {
    foreach ($dependencies as $item) {
      $parent[$item->aid] = $item->parent_aid;
      $values[$item->aid] = unserialize($item->parent_values);
      $required[$item->aid] = $item->required;
    }
  }

  $form['#tree'] = TRUE;
  $form['intro'] = array(
    '#markup' => '<p>' . t('Since drop down attributes may not appear, they cannot be always required.  The required checkbox applies only when the dropdown attribute appears.  Any dropdown attribute is also checked under the attributes table to make sure it is not required there as this would cause validation errors.') . '</p><p>' . t('Unless you know what you are doing, all dependent (child) attributes should be marked as required on this page.') . '</p>',
  );

  foreach ($attributes as $attribute) {
    $form['attributes'][$attribute->aid]['attribute'] = array(
      '#markup' => $attribute->name,
    );

    $options = array();
    $options[0] = 'None';
    foreach ($attributes as $option) {
      if ($option->aid != $attribute->aid) {
        $options[$option->aid] = $option->name;
      }
    }
    $selected = array_key_exists($attribute->aid, $parent) ?
                $parent[$attribute->aid] : 0;
    $form['attributes'][$attribute->aid]['parent'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $selected,
      '#ajax' => array(
        'callback' => 'uc_dropdown_attributes_dependent_callback',
        'wrapper' => 'dropdown-' . $attribute->aid . '-replace',
      ),
    );

    $options = array();
    if ($selected == 0) {
      $type = 'select';
    }
    else {
      $parent_attributes = uc_attribute_load($selected);
      if (count($parent_attributes->options) == 0) {
        $type = 'textfield';
      }
      else {
        $type = 'select';
        foreach ($parent_attributes->options as $oid => $option) {
          $options[$oid] = $option->name;
        }
      }
    }
    if ($type == 'select') {
      $form['attributes'][$attribute->aid]['values'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#prefix' => '<div id="dropdown-' . $attribute->aid . '-replace">',
        '#suffix' => '</div>',
        '#options' => $options,
      );
      if (array_key_exists($attribute->aid, $values)) {
        $form['attributes'][$attribute->aid]['values']['#default_value']
          = $values[$attribute->aid];
      }
    }
    else {
      $form['attributes'][$attribute->aid]['values'] = array(
        '#type' => 'textfield',
        '#prefix' => '<div id="dropdown-' . $attribute->aid . '-replace">',
        '#suffix' => '</div>',
      );
    }

    $form['attributes'][$attribute->aid]['required'] = array(
      '#type' => 'checkbox',
      '#returned_value' => 1,
      '#default_value' => array_key_exists($attribute->aid, $required) ? $required[$attribute->aid] : 0,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('OK'),
  );
}

/**
 * Callback for AJAX for the uc_dropdown_attributes_product form.
 */
function uc_dropdown_attributes_dependent_callback($form, $form_state) {
  $wrapper = explode('-',
    $form_state['triggering_element']['#ajax']['wrapper']);
  $attribute = $wrapper[1];
  return $form['attributes'][$attribute]['values'];
}

/**
 * Construction of table for administration of attribute dependencies.
 *
 * Common theming code for the table in the administrative interface for the
 * node or product class.
 *
 * @param array $form
 *   The form array.
 *
 * @return string
 *   Rendered html for the table.
 */
function _uc_dropdown_attributes_construct_table($form) {
  $output = '';
  $output .= drupal_render($form['intro']);
  $headers = array(
    t('Header'),
    t('Depends on'),
    t('With values'),
    t('Required'),
  );
  $rows = array();
  foreach (element_children($form['attributes']) as $aid) {
    if (is_numeric($aid)) {
      $row = array();
      $row[] = drupal_render($form['attributes'][$aid]['attribute']);
      $row[] = drupal_render($form['attributes'][$aid]['parent']);
      $row[] = drupal_render($form['attributes'][$aid]['values']);
      $row[] = drupal_render($form['attributes'][$aid]['required']);
      $rows[] = $row;
    }
  }
  $output .= theme('table', array('header' => $headers, 'rows' => $rows));

  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Helper function extracting data from the administration form into an array.
 *
 * Extracts information for the attributes from the administration form into an
 * array to prepare for insertion into a database table.
 *
 * @param array $values
 *   The attributes array from the values stored in the $form_state array.
 *
 * @return array
 *   An array of attribute information.
 */
function _uc_dropdown_attributes_extract_attributes($values) {
  $attributes = array();
  foreach ($values as $aid => $value) {
    if (!isset($attributes[$aid])) {
      $attributes[$aid] = new stdClass();
    }
    $attribute = uc_attribute_load($aid);
    $attributes[$aid]->name = $attribute->name;
    foreach ($value as $field => $field_value) {
      switch ($field) {
        case 'parent':
          $attributes[$aid]->parent_aid = $field_value;
          break;

        case 'values':
          $attributes[$aid]->parent_values = $field_value;
          break;

        case 'required':
          $attributes[$aid]->required = $field_value;
          break;
      }
    }
  }
  return $attributes;
}

<?php
/**
 * @file
 * Defines admin functionality for the attribute entity.
 */

/**
 * The attribute edit form for use inside other forms.
 */
function drpg_attribute_embedded_form($form, &$form_state, $attribute, $op = 'edit', $entity_type = NULL, $attribute_count = 0) {

  if ($attribute != NULL) {
    $attribute_id = $attribute->attribute_id;
    $entity_id = $attribute->entity_id;
    $type = $attribute->type;
    $attribute_name = $attribute->attribute_name;
    $value = $attribute->value;
  }
  else {
    $attribute_id = isset($form_state['attribute_id']) ? $form_state['attribute_id'] : '';
    $entity_id = isset($form_state['entity_id']) ? $form_state['entity_id'] : '';
    $type = isset($form_state['type']) ? $form_state['type'] : '';
    $attribute_name = isset($form_state['attribute_name']) ? $form_state['attribute_name'] : '';
    $value = isset($form_state['value']) ? $form_state['value'] : '';
  }

  $form['attribute_id'] = array(
    '#type' => 'hidden',
    '#default_value' => $attribute_id,
  );

  $form['entity_id'] = array(
    '#type' => 'hidden',
    '#default_value' => $entity_id,
  );

  $form['type'] = array(
    '#type' => 'hidden',
    '#default_value' => $type,
  );

  $form['attribute_name'] = array(
    '#title' => t('Attribute Name'),
    '#type' => 'select',
    '#options' => drpg_attribute_names(),
    '#default_value' => $attribute_name,
  );

  $form['value'] = array(
    '#title' => t('Value'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $value,
  );

  $form['delete'] = array(
    '#type' => 'button',
    '#value' => t('Delete attribute'),
    '#name' => 'delete-attribute-' . $attribute_count,
  );

  return $form;
}

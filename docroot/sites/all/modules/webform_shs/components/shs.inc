<?php

/**
 * @file
 * Webform simple hierarchical select component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_shs() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'vid' => NULL,
      'multiple' => NULL,
      'description' => '',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_shs($component) {
  $options = array();
  foreach (taxonomy_get_vocabularies() as $vocabulary) {
    $options[$vocabulary->vid] = $vocabulary->name;
  }

  $form['extra']['vid'] = array(
    '#type' => 'select',
    '#title' => t('Vocabulary'),
    '#options' => $options,
    '#default_value' => $component['extra']['vid'],
  );
  $form['extra']['multiple'] = array(
    '#type' => 'checkbox',
    '#title' => t('Multiple'),
    '#default_value' => $component['extra']['multiple'],
    '#description' => t('Check this option if the user should be allowed to choose multiple values.'),
    '#weight' => 0,
  );

  return $form;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_shs($component, $value = NULL) {
  global $language;
  $vocabulary = taxonomy_vocabulary_load($component['extra']['vid']);

  if (empty($vocabulary)) {
    $form_item['markup'] = array(
      '#markup' => '<div class="form-item">' . t('An invalid vocabulary is selected. Please change it in the options.') . '</div>',
    );
    return $form_item;
  }

  // Add fake item for next level.
  $parents[] = array('tid' => 0);

  $element_settings = array(
    'create_new_terms' => FALSE,
    'create_new_levels' => FALSE,
    'required' => FALSE,
    'language' => $language,
  );

  if (!empty($component['required'])) {
    $element_settings['required'] = TRUE;
  }

  $multiple = !empty($component['extra']['multiple']);

  // Generate a random hash to avoid merging of settings by drupal_add_js.
  // This is necessary until http://drupal.org/node/208611 lands for D7.
  $js_hash = _shs_create_hash();
  $node = node_load($component['nid']);
  $component_parents = webform_component_parent_keys($node, $component);
  $identifier = 'submitted[' . implode('][', $component_parents) . ']';
  // Create settings needed for our js magic.
  $settings_js = array(
    'shs' => array(
      $identifier => array(
        $js_hash => array(
          'vid' => $vocabulary->vid,
          'settings' => $element_settings,
          'default_value' => isset($value) ? $value[0] : $component['value'],
          'parents' => $parents,
          'multiple' => $multiple,
          'any_label' => variable_get('views_exposed_filter_any_label', 'new_any') == 'old_any' ? t('<Any>') : t('- Any -'),
          'any_value' => 'All',
        ),
      ),
    ),
  );

  drupal_alter(array('shs_js_settings', "shs_{$identifier}_js_settings"), $settings_js, $identifier, $vocabulary->vid);
  // Add settings.
  drupal_add_js($settings_js, 'setting');
  // Add behavior.
  drupal_add_js(drupal_get_path('module', 'shs') . '/js/shs.js');

  $form_item = array(
    '#type' => 'textfield',
    '#title' => $component['name'],
    '#required' => $component['required'],
    '#weight' => $component['weight'],
    '#description' => $component['extra']['description'],
    '#default_value' => $component['value'],
    '#prefix' => '<div class="webform-component-' . $component['type'] . '" id="webform-component-' . $component['form_key'] . '">',
    '#suffix' => '</div>',
    '#attributes' => array(
      'class' => array('element-invisible', 'shs-enabled'),
    ),
  );

  if (isset($value)) {
    $form_item['#default_value'] = $value[0];
  }

  return $form_item;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_shs($component, $value, $format = 'html') {
  $display = array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => $format == 'html' ? 'webform_display_markup' : 'webform_display_textfield',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#post_render' => array('webform_element_wrapper'),
    '#component' => $component,
    '#format' => $format,
    '#value' => '',
    '#field_prefix' => '',
    '#field_suffix' => '',
  );

  $newline = $format == 'html' ? '<br/>' : "\n";
  foreach (_webform_shs_get_labels_for_output($value) as $label) {
    $display['#value'] .= check_plain($label) . $newline;
  }

  return $display;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_shs($component, $value) {
  $output = '';

  foreach (_webform_shs_get_labels_for_output($value) as $label) {
    $output .= check_plain($label) . '<br/>';
  }

  return $output;
}

function _webform_shs_get_labels_for_output($value) {
  $labels = array();
  $value = (array) $value;

  foreach ($value as $item => $val) {
    if ($val !== '') {
      if (strpos($val, '+') !== FALSE) {
        // In format of '52+62+64,63'
        $val_parts = explode('+', $val);
        $children = explode(',', end($val_parts));
        foreach ($children as $child) {
          $term = taxonomy_term_load($child);
          $labels[] = $term ? _webform_shs_get_label($child) : $child;
        }
      }
      else {
        $term = taxonomy_term_load($val);
        $labels[] = $term ? _webform_shs_get_label($val) : $val;
      }
    }
  }

  return $labels;
}

function _webform_shs_get_label($tid) {
  $label_parts = array();
  $term = taxonomy_term_load($tid);
  foreach (taxonomy_get_parents_all($tid) as $parent) {
    $label_parts[] = entity_label('taxonomy_term', $parent);
  }
  $label_parts = array_reverse($label_parts);

  return implode(' » ', $label_parts);
}

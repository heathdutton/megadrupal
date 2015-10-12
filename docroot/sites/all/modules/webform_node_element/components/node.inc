<?php
/**
 * @file
 * Webform module node component.
 */

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_node() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'private' => 0,
    'extra' => array(
      'nid' => 0,
      'title' => 0,
      'view_mode' => 'full',
    ),
  );
}

/**
 * Implements _webform_theme_component().
 */
function _webform_theme_node() {
  return array(
    'webform_node_element_node' => array(
      'render element' => 'element',
    ),
    'webform_display_node' => array(
      'render element' => 'element',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_node($component) {
  $form = array();

  // Load the available view modes for 'node' entities, so users can also
  // choose custom ones if they have defined any.
  // Maybe RSS and search results shouldn't be listed. Maybe. Dunno.
  $view_modes = array();
  $data = entity_get_info('node');
  foreach ($data['view modes'] as $key => $value) {
    $view_modes[$key] = $value['label'];
  }

  $form['value'] = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#title' => t('Node'),
    '#default_value' => $component['value'],
    '#description' => t('Enter a valid node id, whose body will be shown as markup in the webform.') . theme('webform_token_help'),
    '#weight' => -1,
    '#element_validate' => array('_webform_edit_validate_node'),
  );
  $form['extra']['nid'] = array(
    '#type' => 'hidden',
    '#default_value' => $component['extra']['nid'],
  );
  $form['extra']['view_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Display Mode'),
    '#default_value' => $component['extra']['view_mode'],
    '#description' => t('Choose how you want the referenced node to be displayed.'),
    '#options' => $view_modes,
  );
  $form['extra']['title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override Title'),
    '#default_value' => $component['extra']['title'],
    '#description' => t('If checked, the title of the referenced node will be used as the component label.'),
    '#options' => array(0, 1),
  );
  $form['extra']['description'] = array();
  $form['display'] = array('#type' => 'markup'); // Hide the display options.

  // If ctools is present, use node title autocomplete.
  if (module_exists('ctools')) {
    $form['value']['#description'] = t('Enter a valid node id, or start typing to find the node whose body will be shown as markup in the webform.') . theme('webform_token_help');
    $form['value']['#autocomplete_path'] = 'ctools/autocomplete/node';
    unset($form['value']['#size']);
  }

  return $form;
}

/**
 * Element validation callback. Ensure a valid node ID was entered.
 */
function _webform_edit_validate_node($element, &$form_state) {
  if (!empty($element['#value'])) {

    // If ctools, the value might be in the form of "node title [nid: N]".
    if (module_exists('ctools')) {
      _webform_node_autocomplete_validate($element);
    }

    $node = node_load($element['#value']);
    if (empty($node->nid) || (empty($node->status) && !user_access('administer nodes'))) {
      form_error($element, t('Invalid node'));
    }

    // Store the node id that we'll use. #value is just for show :-)
    $form_state['values']['extra']['nid'] = $node->nid;
  }
  return TRUE;
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_node($component, $value = NULL, $filter = TRUE) {
  $node = node_load($component['extra']['nid']);

  // Add a bit of error checking; the target node may have been deleted.
  if (empty($node->nid)) {
    drupal_set_message(t('Unable to load referenced node id !nid', array('!nid' => $component['extra']['nid'])), 'error');
    return array();
  }

  $content = node_view($node, $component['extra']['view_mode']);

  // If there is a read-more link, default that to a new browser window.
  if (!empty($content['links']['node']['#links']['node-readmore'])) {
    $content['links']['node']['#links']['node-readmore']['attributes']['target'] = '_new';
  }

  $element = array(
    '#type' => 'hidden',
    '#title' => $component['extra']['title'] ? check_plain($node->title) : $component['name'],
    '#weight' => $component['weight'],
    '#default_value' => render($content),
    '#input_format' => 'html',
    '#theme_wrappers' => array('webform_node_element_node'),
    '#webform_component' => $component,
  );

  $element['#field_value'] = $element['#default_value'];
  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_node($component, $value, $format = 'text') {
  $node = node_load($component['extra']['nid']);
  $content = node_view($node, $component['extra']['view_mode']);

  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_node',
    '#theme_wrappers' => array('webform_display_node'),
    '#format' => 'text',
    '#markup' => render($content),
    '#webform_component' => $component,
  );
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_node($component, $sids = array()) {
  $node = node_load($component['extra']['nid']);
  return array(
    array(t('Nid'), t('<a href="!link">@nid</a>', array('!link' => url('node/'. $node->nid), '@nid' => $node->nid))),
    array(t('Title'), check_plain($node->title)),
    array(t('Type'), $node->type),
    array(t('Display Mode'), $component['extra']['view_mode'])
  );
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_node($component, $value) {
  $node = node_load($component['extra']['nid']);
  $content = node_view($node, $component['extra']['view_mode']);

  return render($content);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_node($component, $export_options) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $component['name'];
  return $header;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_node($component, $export_options, $value) {
  $node = node_load($component['extra']['nid']);
  $content = node_view($node, $component['extra']['view_mode']);

  return render($content);
}

/**
 * Overridable theme function.
 */
function theme_webform_display_node($variables) {
  $element = $variables['element'];
  return $element['#format'] == 'html' ? check_plain($element['#markup']) : $element['#markup'];
}

/**
 * Helper to pull apart a possible ctools node autocomplete submission.
 *
 * This is lifted verbatim from the ctools_node_content_type_edit_form_validate()
 * function at http://drupalcontrib.org/api/drupal/contributions--ctools--plugins--content_types--node--node.inc/7/source
 *
 * @param &$element
 *   A reference to the form element submitted by the user.
 */
function _webform_node_autocomplete_validate(&$element) {
  $nid = $element['#value'];
  $preg_matches = array();

  $match = preg_match('/\[id: (\d+)\]/', $nid, $preg_matches);
  if (!$match) {
    $match = preg_match('/^id: (\d+)/', $nid, $preg_matches);
  }

  if ($match) {
    $nid = $preg_matches[1];
  }
  if (is_numeric($nid)) {
    $node = db_query('SELECT nid FROM {node} WHERE nid = :nid', array(':nid' => $nid))->fetchObject();
  }
  else {
    $node = db_query('SELECT nid FROM {node} WHERE LOWER(title) = LOWER(:title)', array(':title' => $nid))->fetchObject();
  }
  if ($node) {
    $element['#value'] = $node->nid;
  }

  return;
}

/**
 * Override for theme_webform_element().
 *
 * This renders a webform node element and tacks the chosen node content
 * onto the end of the hidden form field.
 */
function theme_webform_node_element_node($variables) {

  $element = $variables['element'];

  // You know what? We're always a node.
  $type = 'node';
  $parents = str_replace('_', '-', implode('--', array_slice($element['#parents'], 1)));

  $wrapper_classes = array(
   'form-item',
   'webform-component',
   'webform-component-' . $type,
  );

  if (isset($element['#webform_component']['extra']['title_display']) && $element['#webform_component']['extra']['title_display'] == 'inline') {
    $wrapper_classes[] = 'webform-container-inline';
  }
  $output = '<div class="' . implode(' ', $wrapper_classes) . '" id="webform-component-' . $parents . '">' . "\n";
  $required = !empty($element['#required']) ? '<span class="form-required" title="' . t('This field is required.') . '">*</span>' : '';

  // If #title is not set, we don't display any label or required marker.
  if (!isset($element['#title'])) {
    $element['#webform_component']['extra']['title_display'] = 'none';
  }
  $prefix = isset($element['#field_prefix']) ? '<span class="field-prefix">' . _webform_filter_xss($element['#field_prefix']) . '</span> ' : '';
  $suffix = isset($element['#field_suffix']) ? '<span class="field-suffix">' . _webform_filter_xss($element['#field_suffix']) . '</span>' : '';

  // Because we're a hidden field, the rendered node content won't show up
  // on the page. However, #field_value congtains a copy. Tack that on to
  // the element output here.
  $value  = isset($element['#field_value']) ? '<span class="field-value">' . $element['#field_value'] . '</span> ' : '';

  switch ($element['#webform_component']['extra']['title_display']) {
    case 'inline':
    case 'before':
    case 'invisible':
      $output .= ' ' . theme('form_element_label', $variables);
      $output .= ' ' . $prefix . $element['#children'] . $value . $suffix . "\n";
      break;

    case 'after':
      $output .= ' ' . $prefix . $element['#children'] . $value . $suffix;
      $output .= ' ' . theme('form_element_label', $variables) . "\n";
      break;

    case 'none':
    case 'attribute':
      // Output no label and no required marker, only the children.
      $output .= ' ' . $prefix . $element['#children'] . $value . $suffix . "\n";
      break;
  }

  if (!empty($element['#description'])) {
    $output .= ' <div class="description">' . $element['#description'] . "</div>\n";
  }

  $output .= "</div>\n";

  return $output;
}

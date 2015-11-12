<?php
/**
 * @file
 * Theme functions for Sortable Grid module
 */

/**
 * Display the view as an HTML list element
 */
function template_preprocess_views_view_sgrid(&$vars) {
  $handler  = $vars['view']->style_plugin;

  $vars['class'] = drupal_clean_css_identifier($handler->options['class']);
  $vars['wrapper_class'] = drupal_clean_css_identifier($handler->options['wrapper_class']);
  $vars['wrapper_prefix'] = '';
  $vars['wrapper_suffix'] = '';
  $vars['list_type_prefix'] = '<' . $handler->options['type'] . '>';
  $vars['list_type_suffix'] = '</' . $handler->options['type'] . '>';
  if ($vars['wrapper_class']) {
    $vars['wrapper_prefix'] = '<div class="' . $vars['wrapper_class'] . '">';
    $vars['wrapper_suffix'] = '</div>';
  }
  if ($vars['class']) {
    $vars['list_type_prefix'] = '<' . $handler->options['type'] . ' class="' . $vars['class'] . '">';

  }
      // Add the 'end of line' class to the appropriate row
      $sgrid_end_of_line = array();
      foreach ($vars['rows'] as $id => $row) {
        if (is_int(($id + 1) / $handler->options['row_length'])) {
            $vars['sgrid_end_of_line'][$id + 1] = ' sgrid-line-end';
        }
      }
      // Provide the row length setting to the template for Javascript management
      $vars['row_length'] = $handler->options['row_length'];
  template_preprocess_views_view_unformatted($vars);
}

function template_preprocess_views_view_row_sgrid_style(&$vars) {
  $view = $vars['view'];

  // Loop through the fields for this view.
  $inline = FALSE;
  $vars['fields'] = array(); // ensure it's at least an empty array.
  foreach ($view->field as $id => $field) {
    // render this even if set to exclude so it can be used elsewhere.
    $field_output = $view->style_plugin->get_field($view->row_index, $id);
    $empty = $field_output !== 0 && empty($field_output);
    if (empty($field->options['exclude']) && (!$empty || (empty($field->options['hide_empty']) && empty($vars['options']['hide_empty'])))) {
      $object = new stdClass();
      $object->handler = &$view->field[$id];

      $object->element_type = $object->handler->element_type(TRUE);
      if ($object->element_type) {
        $class = '';
        if ($object->handler->options['element_default_classes']) {
          $class = 'field-content';
        }

        if ($classes = $object->handler->element_classes()) {
          if ($class) {
            $class .= ' ';
          }
          $class .=  $classes;
        }

        $field_output = '<' . $object->element_type . ' class="' . $class . '">' . $field_output . '</' . $object->element_type . '>';
      }

      // Protect ourself somewhat for backward compatibility. This will prevent
      // old templates from producing invalid HTML when no element type is selected.
      if (empty($object->element_type)) {
        $object->element_type = 'span';
      }

      $object->content = $field_output;
      if (isset($view->field[$id]->field_alias) && isset($vars['row']->{$view->field[$id]->field_alias})) {
        $object->raw = $vars['row']->{$view->field[$id]->field_alias};
      }
      else {
        $object->raw = NULL; // make sure it exists to reduce NOTICE
      }

      $object->inline = !empty($vars['options']['inline'][$id]);

      if (!empty($vars['options']['separator']) && $inline && $object->inline && $object->content) {
        $object->separator = filter_xss_admin($vars['options']['separator']);
      }

      $object->class = drupal_clean_css_identifier($id);
      $inline = $object->inline;
      $object->inline_html = $object->handler->element_wrapper_type(TRUE, TRUE);

      if ($object->inline_html === '') {
        $object->inline_html = $object->inline ? 'span' : 'div';
      }

      // Set up the wrapper HTML.
      $object->wrapper_prefix = '';
      $object->wrapper_suffix = '';

      if ($object->inline_html) {
        $class = '';
        if ($object->handler->options['element_default_classes']) {
          $class = "views-field views-field-" . $object->class;
        }

        if ($classes = $object->handler->element_wrapper_classes()) {
          if ($class) {
            $class .= ' ';
          }
          $class .= $classes;
        }

        $object->wrapper_prefix = '<' . $object->inline_html;
        if ($class) {
          $object->wrapper_prefix .= ' class="' . $class . '"';
        }
        $object->wrapper_prefix .= '>';
        $object->wrapper_suffix = '</' . $object->inline_html . '>';
      }

      // Set up the label for the value and the HTML to make it easier
      // on the template.
      $object->label = check_plain($view->field[$id]->label());
      $object->label_html = '';
      $object->element_label_type = $object->handler->element_label_type(TRUE);
      if ($object->element_label_type && $object->label) {
        $class = '';
        if ($object->handler->options['element_default_classes']) {
          $class = 'views-label-' . $object->class;
        }

        $element_label_class = $object->handler->element_label_classes();
        if ($element_label_class) {
          if ($class) {
            $class .= ' ';
          }

          $class .= $element_label_class;
        }

        $object->label_html = '<' . $object->element_label_type . ' class="' . $class . '">';
        $object->label_html .= $object->label;
        if ($object->handler->options['element_label_colon']) {
          $object->label_html .= ': ';
        }
        $object->label_html .= '</' . $object->element_label_type . '>';
      }

      $vars['fields'][$id] = $object;
    }
    // Add the row's nid to available variables for views-view-row-sgrid-style.tpl.php
    if (isset($vars['row']->tid)) {
      $vars['sgrid_nid'] = $vars['row']->tid;
    }
    elseif (isset($vars['row']->entity_id))  {
      $vars['sgrid_nid'] = $vars['row']->entity_id;
    }
    else {
      $vars['sgrid_nid'] = $vars['row']->nid;
    }
  }
}
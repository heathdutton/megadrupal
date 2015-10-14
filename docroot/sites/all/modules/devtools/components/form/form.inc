<?php

/**
 * Disables & hides a form element.
 * @param  [type] $element [description]
 * @return [type]          [description]
 */
function form_hide(&$element) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $element['#attributes']['disabled'] = TRUE;
    $element['#attributes']['class'][] = 'devtools-form-hide';
  }
}

/**
 * Determines the button which was clicked to trigger form submission and
 * returns its machine name.
 * @param  [type] $form_state [description]
 * @return [type]             [description]
 */
function form_get_op($form_state) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $operation = FALSE;  
    
    foreach ($form_state['buttons'] as $button) {
      if ($button['#value'] == $form_state['values']['op']) {
        $operation = $button['#parents'][0];
        break;
      }
    }

    return $operation;
  }
}

/**
 * Returns the 'numerical' children elements.
 * @param  [type] $element [description]
 * @return [type]          [description]
 */
function element_num_children(&$element) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    $children = array();
    foreach ($element as $key => &$value) {
      if ($key{0} !== '#' && is_numeric($key) && (is_array($value) || is_object($value))) {
        $children[$key] = &$value;
      }
    }

    return $children;
  }
}

/**
 * Attaches Maxlength_JS's functionality to element.
 * @param  [type] $element [description]
 * @param  [type] $count   [description]
 * @param  [type] $label   [description]
 * @return [type]          [description]
 */
function element_maxlength(&$element, $count, $label) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    if (module_exists('maxlength')) {
      $element['#maxlength'] = $count;
      $element['#maxlength_js'] = TRUE;

      $element['#attributes']['maxlength_js_label'] = array(
        $label,
      );

      maxlength_pre_render($element);
    }
  }
}

/**
 * Populates the specified $fields in $form with the default $values,
 * which should come directly from $form_state['values']. Used in
 * multi-step forms, when form values must be manually pre-populated
 * with previously submitted data from the last time the user went
 * through this particular step.
 * 
 * @param  [type] $fields [description]
 * @param  [type] $values [description]
 * @param  [type] $form   [description]
 * @return [type]         [description]
 */
function form_set_defaults($fields, $values, &$form) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    if (!is_array($fields)) {
      $fields = array(
        $fields,
      );
    }

    $i = 1;
    foreach ($fields as $field_key) {
      if (isset($values[$field_key])) {
        $default_value = '';

        $tree = array();
        flatten_tree($values[$field_key], $tree);
        
        $field_info = field_info_field($field_key);
        // multi-value field
        if (($field_info['cardinality'] > 1) || ($field_info['cardinality'] == -1)) {
          // aggregate the separate values into a single array
          $default_value = array();
          foreach ($tree as $key_tree) {
            $default_value[] = array_pop($key_tree);
          }
          
          // for multi-value fields, the #default_value is set on the element immediately above
          // the separate field values. In other words, we need to set the value at the point of
          // divergence in our trees, so let's compare the first two and find that point:
          if (sizeof($tree) > 1) {
            $divergence = array_flat_divergence($tree[0], $tree[1]);
          }
          else {
            $divergence = current($tree[0]);
          }
          
          // navigate down the tree to the point of divergence to find the element we want to set
          // defaults on
          $element = &$form[$field_key];

          foreach ($key_tree as $path_child) {
            $element = &$element[$path_child];

            if ($path_child === $divergence) {
              break;
            }
          }

          $element['#default_value'] = $default_value;
        }
        // single-value field
        else {
          foreach ($tree as $key_tree) {
            // $key_tree will be an array of keys used to access the final value, plus the value itself
            // ex.
            //  for $data['und'][0]['value'] = 'test';,
            //  the $key_tree will be:
            //    array('und', '0', 'value', 'test')

            // get the submitted field value
            $field_val = array_pop($key_tree);

            // navigate down the tree to the deepest child element
            $element = &$form[$field_key];
            foreach ($key_tree as $path_child) {
              $element = &$element[$path_child];
            }

            // set the submitted value as the default value on the deepest child
            $element['#default_value'] = $field_val;
          }
        }
      }
    }
  }
}

/**
 * Prepares error message text for given form field.
 * @param  [type] $field_name    [description]
 * @param  [type] $field_message [description]
 * @param  [type] $delta_path    [description]
 * @return [type]                [description]
 */
function form_prepare_error_text($field_name, $field_message, $delta_path) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    // default "empty field" message
    if ($field_name == $field_message) {
      $error_text = t('Field is required.');
    }
    // customized message
    else {
      if (!is_array($delta_path)) {
        $delta_path = array($delta_path);
      }

      $item_delta = array_pop($delta_path);
      $args = array(
        '!delta'          => $item_delta,
        '!delta_friendly' => $item_delta + 1,
        '!field_name'     => $field_name,
      );

      if ($size = sizeof($delta_path)) {
        for ($i = $size - 1; $i >= 0; $i--) {
          $delta_key = '!delta_minus_' . ($size - $i);
          $args[$delta_key] = array_pop($delta_path);
          $args[$delta_key . '_friendly'] = $args[$delta_key] + 1;
        }
      }

      $error_text = t($field_message, $args);
    }

    return $error_text;
  }
}

/**
 * Recursively loops through state values, looking for specified field structure
 * and setting custom error messages when empty values are found. Assumes field
 * values are stored in ['values'].
 * 
 * @param  [type] $fields   [description]
 * @param  [type] $values   [description]
 * @param  [type] $lang     [description]
 * @param  array  $key_path [description]
 * @return [type]           [description]
 */
function form_check_fields($fields, $values, $lang, $key_path = array(), $delta_path = array()) {
  list($handler, $handler_data) = devtools_func_handler(__FUNCTION__, func_get_args());

  if ($handler) {
    return $handler_data;
  }
  else {
    foreach ($fields as $field_name => $data) {
      if (isset($values[$field_name])) {
        if (is_array($data)) {
          foreach ($values[$field_name][$lang] as $delta => $item) {
            $child_kp = $key_path;
            $child_kp[] = $field_name;
            $child_kp[] = $lang;
            $child_kp[] = $delta;

            $child_dp = $delta_path;
            $child_dp[] = $delta;

            form_check_fields($data, $item, $lang, $child_kp, $child_dp);
          }
        }
        else {
          foreach ($values[$field_name][$lang] as $delta => $item) {
            $val = trim($item['value']);

            if (is_null($val) || $val == '' || $val === FALSE) {
              $child_kp = $key_path;
              $child_kp[] = $field_name;
              $child_kp[] = $lang;

              $error_key = implode('][', $child_kp);

              // get the current set of errors, which should contain an element with the
              // key matching our $error_key; we'll use this to identify the error text
              // which was set, and remove it from $_SESSION further down
              $errors = form_get_errors();

              $child_dp = $delta_path;
              $child_dp[] = $delta;

              $error_text = form_prepare_error_text($field_name, $data, $child_dp);

              // remove previous error messages on this element in form_set_error's 
              // static cache
              $form = &drupal_static('form_set_error', array());
              if (isset($form[$error_key])) {
                unset($form[$error_key]);
              }

              // assign our new error
              form_set_error($error_key, $error_text);

              // remove the previous error messsage from the $_SESSION
              if (isset($errors[$error_key])) {
                $index = array_search($errors[$error_key], $_SESSION['messages']['error']);
                
                if ($index !== FALSE) {
                  unset($_SESSION['messages']['error'][$index]);

                  // now that we've removed an element from the error messages, we must
                  // rebuild the keys in this array, otherwise Drupal might not display
                  // the error messages (if, for example, we removed index 0)
                  $_SESSION['messages']['error'] = array_merge($_SESSION['messages']['error']);
                }
              }
            }
          }
        }
      }
    }
  }
}
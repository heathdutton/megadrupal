<?php

class TemplateApiVerticalTabsInputType extends TemplateApiInputType {
  public function form($input_definition, $input_values) {
    $form = array();

    $form['vertical_tabs'] = array(
      '#type' => 'vertical_tabs',
      '#title' => $input_definition['label'],
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      '#description' => isset($input_definition['description']) ? $input_definition['description'] : '',
    );

    if (isset($input_definition['children'])) {
      foreach ($input_definition['children'] as $key=>$input) {
        $value = isset($input_values['vertical_tabs'][$key]) ? $input_values['vertical_tabs'][$key] : NULL;
        $form['vertical_tabs'][$key] = template_api_get_input_type_controller($input['type'])
          ->getForm($input, $value);
      }
    }

    return $form;
  }

  public function getVariable($input_definition, $input_values) {
    $output = array();
    foreach ($input_values['vertical_tabs'] as $key => $value) {
      if (is_array($value)) {
        $input_type = $input_definition['children'][$key]['type'];
        $input_def = $input_definition['children'][$key];
        foreach ($value as $type => $input_value) {
          $output[$key] = template_api_get_input_type_controller($input_type)->getVariable($input_def, $value);
        }
      }
    }

    return $output;
  }

  public function isEmpty($input_definition, $input_values) {
    if (array_key_exists('vertical_tabs', $input_values)) {
      foreach ($input_values['vertical_tabs'] as $key => $value) {
        if (is_array($value)) {
          $input_type = $input_definition['children'][$key]['type'];
          $input_def = $input_definition['children'][$key];
          foreach ($value as $type => $input_value) {
            if (!template_api_get_input_type_controller($input_type)->isEmpty($input_def, $value)) {
              return FALSE;
            }
          }
        }
      }
    }

    return TRUE;
  }

  // Regular text values get saved on update, but files don't
  // So this update function is mainly for files
  public function update($input_definition, $input_values) {
    if (!empty($input_definition['children'])) {
      foreach ($input_definition['children'] as $vertical_tab_child_index => $vertical_tab_child) {
        // If it's a fieldset, it has children, else it doesn't
        if ($vertical_tab_child['type'] == 'fieldset') {
          // Loop through children
          foreach ($vertical_tab_child['children'] as $fieldset_child_name => $fieldset_child) {
            $controller = template_api_get_input_type_controller($fieldset_child['type']);
            //Call the update method of the controller
            $controller->update($input_definition, $input_values['vertical_tabs'][$vertical_tab_child_index]['fieldset'][$fieldset_child_name]);
          }
        }
        else {
          //Find the type of each input and run it's update method
        }
      }
    }

    return TRUE;
  }
}

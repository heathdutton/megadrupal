<?php
/**
 * @file
 * Form related theme functions.
 */

/**
 * Overrides theme_form_element().
 */
function strapped_form_element(&$variables) {
  // Grab the element for ease of use.
  $element = & $variables['element'];

  // Append some default element settings.
  $element += array(
    '#title_display' => 'before',
    '#form_type' => 'basic',
    '#form_type_wrapper' => FALSE, // for example, form-horizontal
    '#form_group_wrapper' => TRUE, // for example, form-group
    '#form_field_wrapper' => TRUE // for example, field-type-taxonomy-term-reference
  );

  // Make sure there is an attributes array we can add classes to.
  $attributes = array(
    'class' => array()
  );

  // Deal with webforms.
  _strapped_merge_webform_properties($element, $attributes);

  // Store the fact we are dealing with a checkbox.
  $is_checkbox = FALSE;
  if (isset($element['#type']) && $element['#type'] == "checkbox") {
    $is_checkbox = TRUE;
  }

  // Store the fact we are dealing with a radio button.
  $is_radio = FALSE;
  if (isset($element['#type']) && $element['#type'] == "radio") {
    $is_radio = TRUE;
  }

  // Horizontal forms helper.
  $is_horizontal = FALSE;
  if ($element['#form_type'] == 'horizontal') {
    $is_horizontal = TRUE;
  }

  // Inline forms helper.
  $is_inline = FALSE;
  if ($element['#form_type'] == 'inline') {
    $is_inline = TRUE;
  }

  // Basic forms helper.
  $is_basic = FALSE;
  if ($element['#form_type'] == 'basic') {
    $is_basic = TRUE;
  }



  // Add the form item class as it is required for states and behaviours to work.
  $attributes['class'][] = 'form-item';

  // Add the element's 'type' as a class for example, form-type-textfield.
  if (!empty($element['#type'])) {
    $attributes['class'][] = 'form-type-' . strtr($element['#type'], '_', '-');
    // To aid in error placement we need a class for some edgecase elements.
    if (isset($element['#range_type'])) {
      $attributes['class'][] = 'form-type-range';
      $attributes['class'][] = 'form-type-range-' . $element['#range_type'];
    }
  }

  // Add the elements 'name' as a class for example, form-item-title.
  if (!empty($element['#name'])) {
    $attributes['class'][] = 'form-item-' . drupal_clean_css_identifier($element['#name']);
  }

  // Add the classes to checkboxes based on the columns.
  if (!empty($element['#element_columns'])) {

    $classes = _strapped_grid_classes($element['#element_columns']);
    $attributes['class'] = array_merge($classes, $attributes['class'] );

  }

  // Add an id attribute for markup elements. @todo why do i do this?
  if (isset($element['#markup']) && !empty($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }

  // Add a class for disabled elements to facilitate cross-browser styling.
  if (!empty($element['#attributes']['disabled'])) {
    $attributes['class'][] = 'form-disabled';
  }

  // Add a class for the autocomplete element.
  if (!empty($element['#autocomplete_path']) && drupal_valid_path($element['#autocomplete_path'])) {
    $attributes['class'][] = 'form-autocomplete';
  }

  // Check for errors and set correct bootstrap error class.
  if (isset($element['#parents']) && form_get_error($element)) {
    $attributes['class'][] = 'has-error'; // @todo this isnt working on textareas, this should be on the form group wrapper.
  }

  /* Descriptions - If there is a description then add a class to aid in styling form errors. */
  $help = '';
  if (!empty($element['#description'])) {
    $attributes['class'][] = 'has-description';
    $help = theme('help',array('description' =>  $element['#description'] ));
  }
  else {
    $attributes['class'][] = 'no-description';
  }

  // Prefix tooltip
  if (isset($element['#prefix_tooltip']) && !empty($element['#prefix_tooltip']['title'])) {
    $element['#input_group'] = TRUE;
    $element['#field_prefix'] = theme('tooltip',array('position' => $element['#prefix_tooltip']['position'], 'title' => $element['#prefix_tooltip']['title'] ));
  }

  // Suffix tooltip
  if (isset($element['#suffix_tooltip']) && !empty($element['#suffix_tooltip']['title'])) {
    $element['#input_group'] = TRUE;
    $element['#field_suffix'] = theme('tooltip',array('position' => $element['#suffix_tooltip']['position'], 'title' => $element['#suffix_tooltip']['title'] ));
 }

  /* Prefix and suffixes. */
  if (!isset($element['#field_prefix_type'])) {
    $element['#field_prefix_type'] = 'addon';
  }
  if (!isset($element['#field_suffix_type'])) {
    $element['#field_suffix_type'] = 'addon';
  }

  $prefix = '';
  $suffix = '';
  if (isset($element['#field_prefix']) || isset($element['#field_suffix'])) {
    // Determine if "#input_group" was specified.
    if (!empty($element['#input_group'])) {
      $prefix .= '<div class="input-group">';
      $prefix .= isset($element['#field_prefix']) && (trim($element['#field_prefix']) != '') ? '<span class="input-group-' .$element['#field_prefix_type'] . '">' . $element['#field_prefix'] . '</span>' : '';
      $suffix .= isset($element['#field_suffix']) && (trim($element['#field_suffix']) != '') ? '<span class="input-group-' .$element['#field_suffix_type'] . '">' . $element['#field_suffix'] . '</span>' : '';
      $suffix .= '</div>';
    }
    else {
      $prefix .= isset($element['#field_prefix']) ? $element['#field_prefix'] : '';
      $suffix .= isset($element['#field_suffix']) ? $element['#field_suffix'] : '';
    }
  }

  // If #title is not set, we don't display any label or required marker. Also dont bother if it is empty.
  if (!isset($element['#title']) || empty($element['#title'])) {
    $element['#title_display'] = 'none';
  }

  // For basic forms there is no wrapper, for horizontal and inline there are.
  if (in_array($element['#form_type'], array('horizontal', 'inline'))) {
    $element['#form_type_wrapper'] = TRUE;
  }

  // Checkbox elements rendered as checkboxes shouldnt have the form type wrapper.
  if ((isset($element['#parent_type']) && $element['#parent_type'] == 'checkboxes')) {
    $element['#form_type_wrapper'] = FALSE;
    $element['#form_group_wrapper'] = FALSE; // @todo make form_group_wrapper just form-group and use it to kill the class.
  }

  /* Element Output - Start to generate the html of the form element. */
  $output = '';

  // #element_grid settings.
  if (isset($element['#element_grid'])) {
    // Use element_grid if it was provided.
    $element_grid = $element['#element_grid'];
  }

  // #title_grid settings.
  if (isset($element['#title_grid'])) {
    // Use element_grid if it was provided.
    $title_grid = $element['#title_grid'];
  }


  /* Build the output render array to use when rendering the element */
  $render = array();
  $render['wrappers'] = array();
  $render['wrappers']['#theme_wrappers'] = array();

  if ($is_inline) {
    // Inline elements need a touch of space after the title.
    if ($is_checkbox || $is_radio) {
      $element['#title'] = ' ' . $element['#title'];
    } else {
     // $element['#title'] = $element['#title'];
    }

  }




  // Checkbox
  if ($is_checkbox) {
    // Checkboxes need to be wrapper in the .checkbox class.
    if ((!isset($element['#element_inline']) || !$element['#element_inline'] ) ) { // @todo change to $is_element_inline?
      $render['wrappers']['#theme_wrappers'][] = 'checkbox_wrapper';
    }


    if ($is_horizontal) {
      // Checkboxes need the column wrapper outside the label
      $render['wrappers']['#theme_wrappers'][] = 'column_wrapper';
      $render['wrappers']['#grid'] = $title_grid;
    }


  }

  // Radio
  if ($is_radio) {
      // Checkboxes need to be wrapper in the .checkbox class.
      if ((!isset($element['#element_inline']) || !$element['#element_inline'] ) ) { // @todo change to $is_element_inline?
          $render['wrappers']['#theme_wrappers'][] = 'radio_wrapper';
      }


      if ($is_horizontal) {
          // Checkboxes need the column wrapper outside the label
          $render['wrappers']['#theme_wrappers'][] = 'column_wrapper';
          $render['wrappers']['#grid'] = $title_grid;
      }

  }

  // Do we want the form-type wrapper on this element
  if ($element['#form_group_wrapper']) {
    $render['wrappers']['#form_group_wrapper_attributes'] = array();
    $render['wrappers']['#theme_wrappers'][] = 'form_group_wrapper';
  }

  // Do we want the form element wrapper
  if ($element['#form_field_wrapper']) {
    $render['wrappers']['#form_field_wrapper_attributes'] = $attributes; // @todo change to field_attributes?
    $render['wrappers']['#theme_wrappers'][] = 'form_field_wrapper';
  }

  // Do we want the form-type wrapper on this element
  if ($element['#form_type_wrapper']) {
    $render['wrappers']['#form_type_wrapper_attributes'] = array();
    $render['wrappers']['#form_type'] = $element['#form_type'];
    $render['wrappers']['#theme_wrappers'][] = 'form_type_wrapper';
  }

  // Build the stub render arrays which we will progressively add to depending on settings.
  $render['wrappers']['label'] = array();
  $render['wrappers']['content'] = array();
  $render['wrappers']['content']['#theme_wrappers'] = array();


  // If this is a item element then add 'form-control-static', (but not if it is a range_integer)
  if (isset($element['#type']) && ($element['#type'] == 'item' || $element['#type'] == 'markup') && !isset($element['#range_type'])) {
    $render['wrappers']['content']['#theme_wrappers'][] = 'form_control_static_wrapper';
  }

  // Horizontal forms need a column wrapper around the element.
  if ($is_horizontal) {

    // Render the element inside a bootstrap column.
    $render['wrappers']['content']['#theme_wrappers'][] = 'column_wrapper';

    // Setup the #grid
    $render['wrappers']['content']['#grid'] = $element_grid;

    // If the title display is set to before then the title width should be full width
    if ($element['#title_display'] == 'before') {
      $element['#title_grid'] = array(
        'columns' => array(
          'md' => 12,
        ),
      );

    }

    // Render the label inside a bootstrap column if it is a checkbox.
    if ($is_checkbox || $is_radio) {
      // Checkboxes don't need the content wrapper as the element is inside the label.
      unset($render['wrappers']['content']);
    }
  }


  // Deal with inline form errors
  $error = '';
  if (isset($element['#id'])) {
    if (function_exists('ife_errors')) {
      $errors = ife_errors('get', $variables['element']['#id']) ;
      if ($errors) {
        // $error = '<div class="messages error messages-inline">' .  $errors . '</div>';
        $error = theme('error',array('message' => $errors));
      }
    } elseif (isset($element['#ife_error'] )) {
      $error = '<div class="messages error messages-inline">' .  $element['#ife_error'] . '</div>';
    }
  }


  /* Deal with checkboxes separately as they have special needs */
  if ($is_checkbox || $is_radio) {
    $variables['#children'] = ' ' . $prefix . $element['#children'] . $suffix;

    $render['wrappers']['label']['#markup'] = theme('form_element_label', $variables) ;
  } else {
    /* Different output depending on title_display */
    switch ($element['#title_display']) {
      case 'before':
      case 'unseen':
      case 'invisible':
        if ($is_checkbox && $element['#title_display'] == 'invisible') {

          $variables['#children'] = ' ' . $prefix . $element['#children'] . $suffix;

          $render['wrappers']['label']['#markup'] = theme('form_element_label', $variables);
        } else {
          $render['wrappers']['label']['#markup'] = theme('form_element_label', $variables);
          $render['wrappers']['content']['#markup'] = $prefix . $element['#children'] . $suffix  . $error . $help;
        }


        break;
      case 'inline':
        $render['wrappers']['label'] = array(
          '#markup' => theme('form_element_label', $variables)
        );

        $render['wrappers']['content']['#markup'] = $prefix . $element['#children'] . $suffix . $error . $help;


        break;
      case 'after':

        $variables['#children'] = ' ' . $prefix . $element['#children'] . $suffix;
        $render['wrappers']['label']['#markup'] = theme('form_element_label', $variables);

        if ($help) {
          $render['wrappers']['label']['#markup'] .= $help;
        }
        break;
      case 'none':
      case 'attribute':
        $render['wrappers']['content']['#markup'] = $prefix . $element['#children'] . $suffix . $help;

        $output .= drupal_render($render);
        break;
    }

  }




  // Render the array and append it.
  $output .= drupal_render($render);

  // Return our beautiful form element.
  return $output;
}


/**
 * Overrides theme_form_element_label().
 */
function strapped_form_element_label(&$variables) {

  // Gain easy access to the element.
  $element = $variables['element'];

  $is_checkbox = FALSE;
  if (isset($element['#type']) && $element['#type'] == "checkbox") {
    $is_checkbox = TRUE;
  }

    $is_radio = FALSE;
    if (isset($element['#type']) && $element['#type'] == "radio") {
        $is_radio = TRUE;
    }

  // Append some defaults.
  $element += array(
    '#title_display' => 'before',
  );


  // This is also used in the installer, pre-database setup.
  $t = get_t();

  // If there is not title
  if (!isset($element['#title'])) {
    $element['#title'] = '';
  }

  // Determine if certain things should skip for checkbox or radio elements. @todo removed to make checkboxes work proper.
//  $skip = (isset($element['#type']) && ('checkbox' === $element['#type'] || 'radio' === $element['#type']));
//
//  // If title and required marker are both empty, output no label.
//  if ((!isset($element['#title']) || $element['#title'] === '' && !$skip) && empty($element['#required'])) {
//    return '';
//  }



  // If the element is required, a required marker is appended to the label.
  $required = !empty($element['#required']) ? theme('form_required_marker', array('element' => $element)) : '';

  // If there is a tooltip then add it.
  $title_tooltip = '';
  if (isset($element['#title_tooltip']) && !empty($element['#title_tooltip']['title'])) {
    $title_tooltip = theme('tooltip',array('position' => $element['#title_tooltip']['position'], 'title' => $element['#title_tooltip']['title'] ));
  }


  // If there is a tooltip then add it. @todo make a theme function for the tooltipperoo
  $title_popover = '';
  if (isset($element['#title_popover']) && !empty ($element['#title_popover']['title']) ) {
    $title_popover = '<span class="tip glyphicon glyphicon-question-sign"  data-toggle="popover" title="' . $variables['element']['#title_popover']['title'] . '" data-content="' . $variables['element']['#title_popover']['description'] . '"></span>';
  }





  $title = '<span>' . filter_xss_admin($element['#title']) . '</span>';

  $attributes = array();



  // Checkboxes with invisible labels need special treatment  http://getbootstrap.com/css/#checkboxes-and-radios-without-la
  if (isset($element['#type']) && in_array($element['#type'],array('checkbox','radio')) && $element['#title_display'] == 'invisible') {
    $title = '<span></span>';
    $attributes['aria-label'] = filter_xss_admin($element['#title']);
  }


  // Add the radio-inline class if necessary
  if (isset($element['#type']) && isset($element['#element_inline']) && $element['#type'] == 'radio' && $element['#element_inline'] == TRUE) {
    $attributes['class'][] = 'radio-inline';
  }

  // Add the checkbox-inline class if necessary
  if (isset($element['#type']) && isset($element['#element_inline']) && $element['#type'] == 'checkbox' && $element['#element_inline'] == TRUE) {
    $attributes['class'][] = 'checkbox-inline';
  }

  // Style the label as class option to display inline with the element.
  if ($element['#title_display'] == 'after' ) {
   // $attributes['class'][] = $element['#type']; // @todo this was removed so that checkboxes dont get a .checkbox class on the label
  }
  elseif ($element['#title_display'] == 'unseen') {
    $attributes['class'][] = 'invisible';

  }
  // Show label only to screen readers to avoid disruption in visual flows.

  elseif ($element['#title_display'] == 'invisible' && !$is_checkbox && !$is_radio) {
    $attributes['class'][] = 'element-invisible';

  }
  elseif ($element['#title_display'] == 'inline' && $element['#form_type'] == 'horizontal') {

    $attributes['class'][] = 'control-label';

  }

  if (!empty($element['#id'])) {
    $attributes['for'] = $element['#id'];
  }

  // Insert radio and checkboxes inside label elements.
  $output = '';
  if (isset($variables['#children'])) {
    $output .= $variables['#children'];
  }

  $help = '';
  if (!empty($element['#description']) && $is_checkbox) {
    $help = '<span class="help-block">' . $element['#description'] . "</span>\n";
  }


  // Append label.
  $output .= $t('!title !required !tooltip !popover !help', array(
    '!title' => $title,
    '!required' => $required,
    '!tooltip' => $title_tooltip,
    '!popover' => $title_popover,
    '!help' => $help
  ));



  $render = array(
    '#theme' => 'column_wrapper',
    '#tag' => 'label',
    '#children' => $output,
    '#attributes' => $attributes,
    '#suffix' => "\n"
  );



  // Horizontal forms need the #grid parameter
  if (isset($element['#form_type']) && $element['#form_type'] == 'horizontal' && !$is_checkbox) { // @todo $is_horixontal??
    if (isset($element['#title_grid'])) {
      $render['#grid'] = $element['#title_grid'];
    }
  }



  $html = drupal_render($render);
  return $html;
}

<?php

/**
 * @file
 * Settings form.
 */

/**
 * Form builder. Configure scrollbar.
 */
function scrollbar_admin_settings() {
  $form['scrollbar_element'] = array(
    '#type' => 'textfield',
    '#title' => t('Elements to get the jScrollPane function'),
    '#default_value' => variable_get('scrollbar_element', '.field-name-body'),
    '#size' => 100,
    '#required' => TRUE,
    '#maxlength' => 800,
    '#description' => "<p>" . t('Set here the DOM elements that will get the scrollbar function.') . "</p><p>" . t('Seperate elements with a comma. Example <code>@code</code>', array('@code' => '.field-name-body, #mydiv')) . "</p><p><strong>" . t("Do not use a trailing comma") . "</strong></p><p>" . t("Finally, don't forget to use the proper CSS. Example <code>@code</code>", array('@code' => ".field-name-body {overflow:auto; \n height: 200px;}")) . "</p>",
  );
  $form['scrollbar_showArrows'] = array(
    '#type' => 'select',
    '#title' => t('Show arrows for scrollbar'),
    '#default_value' => variable_get('scrollbar_showArrows', 'true'),
    '#options' => array(
      'true' => t('yes'),
      'false' => t('no'),
    ),
    '#description' => t('Whether arrows should be shown on the generated scroll pane. When set to false only the scrollbar track and drag will be shown, if set to true then arrows buttons will also be shown.'),
  );
  $form['scrollbar_advancedOptions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['scrollbar_advancedOptions']['scrollbar_generalOptions'] = array(
    '#type' => 'fieldset',
    '#title' => t('General options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['scrollbar_advancedOptions']['scrollbar_verticalOptions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Vertical scrollbar options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => "<p>" . t('The size of the drag elements is based on the proportion of the size of the content to the size of the viewport but is contrained within the minimum and maximum dimensions given') . "</p>",
  );
  $form['scrollbar_advancedOptions']['scrollbar_horizontialOptions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Horizontial scrollbar options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => "<p>" . t('The size of the drag elements is based on the proportion of the size of the content to the size of the viewport but is contrained within the minimum and maximum dimensions given') . "</p>",
  );
  $form['scrollbar_advancedOptions']['scrollbar_generalOptions']['scrollbar_arrowScrollOnHover'] = array(
    '#type' => 'select',
    '#title' => t('Scroll element when mouse is over arrows'),
    '#default_value' => variable_get('scrollbar_arrowScrollOnHover', 'false'),
    '#options' => array(
      'true' => t('true'),
      'false' => t('false'),
    ),
    '#description' => t('Whether the arrow buttons should cause the scrollbar to scroll while you are hovering over them.'),
  );
  $form['scrollbar_advancedOptions']['scrollbar_generalOptions']['scrollbar_mouseWheelSpeed'] = array(
    '#type' => 'textfield',
    '#title' => t('Mousewheel speed multiplier'),
    '#default_value' => variable_get('scrollbar_mouseWheelSpeed', 10),
    '#size' => 5,
    '#maxlength' => 5,
    '#element_validate' => array('scrollbar_validate_integer_positive_zero'),
    '#description' => t("A multiplier which is used to control the amount that the scrollpane scrolls each time the mouse wheel is turned."),
  );
  $form['scrollbar_advancedOptions']['scrollbar_generalOptions']['scrollbar_arrowButtonSpeed'] = array(
    '#type' => 'textfield',
    '#title' => t('Arrow buttons speed multiplier'),
    '#default_value' => variable_get('scrollbar_arrowButtonSpeed', 10),
    '#size' => 5,
    '#maxlength' => 5,
    '#element_validate' => array('scrollbar_validate_integer_positive_zero'),
    '#description' => t('A multiplier which is used to control the amount that the scrollpane scrolls each time on of the arrow buttons is pressed.'),
  );
  $form['scrollbar_advancedOptions']['scrollbar_generalOptions']['scrollbar_arrowRepeatFreq'] = array(
    '#type' => 'textfield',
    '#title' => t('Arrow buttons Repeat frequency, in ms'),
    '#default_value' => variable_get('scrollbar_arrowRepeatFreq', 100),
    '#size' => 5,
    '#maxlength' => 5,
    '#element_validate' => array('scrollbar_validate_integer_positive_zero'),
    '#description' => t('The number of milliseconds between each repeated scroll event when the mouse is held down over one of the arrow keys.'),
  );
  $form['scrollbar_advancedOptions']['scrollbar_horizontialOptions']['scrollbar_horizontialGutter'] = array(
    '#type' => 'textfield',
    '#title' => t('Horizontial scrolling gap, in px'),
    '#default_value' => variable_get('scrollbar_horizontialGutter', 5),
    '#size' => 5,
    '#maxlength' => 5,
    '#element_validate' => array('scrollbar_validate_integer_positive_zero'),
    '#description' => t('Introduces a gap between the scrolling content and the scrollbar itself.'),
  );
  $form['scrollbar_advancedOptions']['scrollbar_verticalOptions']['scrollbar_verticalGutter'] = array(
    '#type' => 'textfield',
    '#title' => t('Vertical scrolling gap, in px'),
    '#default_value' => variable_get('scrollbar_verticalGutter', 5),
    '#size' => 5,
    '#maxlength' => 5,
    '#element_validate' => array('scrollbar_validate_integer_positive_zero'),
    '#description' => t('Introduces a gap between the scrolling content and the scrollbar itself.'),
  );
  $form['scrollbar_advancedOptions']['scrollbar_verticalOptions']['scrollbar_verticalDragMinHeight'] = array(
    '#type' => 'textfield',
    '#title' => t('Vertical Drag min height, in px'),
    '#default_value' => variable_get('scrollbar_verticalDragMinHeight', 0),
    '#size' => 5,
    '#maxlength' => 5,
    '#element_validate' => array('scrollbar_validate_integer_positive_zero'),
    '#description' => "<p>" . t('The smallest height that the vertical drag can have') . "</p>",
  );
  $form['scrollbar_advancedOptions']['scrollbar_verticalOptions']['scrollbar_verticalDragMaxHeight'] = array(
    '#type' => 'textfield',
    '#title' => t('Vertical Drag max height, in px'),
    '#default_value' => variable_get('scrollbar_verticalDragMaxHeight', 99999),
    '#size' => 5,
    '#maxlength' => 5,
    '#element_validate' => array('scrollbar_validate_integer_positive_zero'),
    '#description' => "<p>" . t('The maximum height that the vertical drag can have') . "</p>",
  );
  $form['scrollbar_advancedOptions']['scrollbar_verticalOptions']['scrollbar_verticalDragMinWidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Vertical Drag min width, in px'),
    '#default_value' => variable_get('scrollbar_verticalDragMinWidth', 0),
    '#size' => 5,
    '#maxlength' => 5,
    '#element_validate' => array('scrollbar_validate_integer_positive_zero'),
    '#description' => "<p>" . t('The smallest width that the vertical drag can have') . "</p>",
  );
  $form['scrollbar_advancedOptions']['scrollbar_verticalOptions']['scrollbar_verticalDragMaxWidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Vertical Drag max width, in px'),
    '#default_value' => variable_get('scrollbar_verticalDragMaxWidth', 99999),
    '#size' => 5,
    '#maxlength' => 5,
    '#element_validate' => array('scrollbar_validate_integer_positive_zero'),
    '#description' => "<p>" . t('The maximum width that the vertical drag can have') . "</p>",
  );
  $form['scrollbar_advancedOptions']['scrollbar_horizontialOptions']['scrollbar_horizontialDragMinHeight'] = array(
    '#type' => 'textfield',
    '#title' => t('Horizontial Drag min height, in px'),
    '#default_value' => variable_get('scrollbar_horizontialDragMinHeight', 0),
    '#size' => 5,
    '#maxlength' => 5,
    '#element_validate' => array('scrollbar_validate_integer_positive_zero'),
    '#description' => "<p>" . t('The smallest height that the horizontial drag can have') . "</p>",
  );
  $form['scrollbar_advancedOptions']['scrollbar_horizontialOptions']['scrollbar_horizontialDragMaxHeight'] = array(
    '#type' => 'textfield',
    '#title' => t('Horizontial Drag max height, in px'),
    '#default_value' => variable_get('scrollbar_horizontialDragMaxHeight', 99999),
    '#size' => 5,
    '#maxlength' => 5,
    '#element_validate' => array('scrollbar_validate_integer_positive_zero'),
    '#description' => "<p>" . t('The maximum height that the horizontial drag can have') . "</p>",
  );
  $form['scrollbar_advancedOptions']['scrollbar_horizontialOptions']['scrollbar_horizontialDragMinWidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Horizontial Drag min width, in px'),
    '#default_value' => variable_get('scrollbar_horizontialDragMinWidth', 0),
    '#size' => 5,
    '#maxlength' => 5,
    '#element_validate' => array('scrollbar_validate_integer_positive_zero'),
    '#description' => "<p>" . t('The smallest width that the horizontial drag can have') . "</p>",
  );
  $form['scrollbar_advancedOptions']['scrollbar_horizontialOptions']['scrollbar_horizontialDragMaxWidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Horizontial Drag max width, in px'),
    '#default_value' => variable_get('scrollbar_horizontialDragMaxWidth', 99999),
    '#size' => 5,
    '#maxlength' => 5,
    '#element_validate' => array('scrollbar_validate_integer_positive_zero'),
    '#description' => "<p>" . t('The maximum width that the horizontial drag can have') . "</p>",
  );
  $form['scrollbar_advancedOptions']['scrollbar_verticalOptions']['scrollbar_verticalArrowPositions'] = array(
    '#type' => 'select',
    '#title' => t('Show the vertical arrows relative to the vertical track'),
    '#default_value' => variable_get('scrollbar_verticalArrowPositions', 'split'),
    '#options' => array(
      'split' => t('split'),
      'before' => t('before'),
      'after' => t('after'),
      'os' => t('os'),
    ),
    '#description' => t('Where the vertical arrows should appear relative to the vertical track.'),
  );
  $form['scrollbar_advancedOptions']['scrollbar_horizontialOptions']['scrollbar_horizontialArrowPositions'] = array(
    '#type' => 'select',
    '#title' => t('Show the horizontial arrows relative to the horizontial track'),
    '#default_value' => variable_get('scrollbar_horizontialArrowPositions', 'split'),
    '#options' => array(
      'split' => t('split'),
      'before' => t('before'),
      'after' => t('after'),
      'os' => t('os'),
    ),
    '#description' => t('Where the horizontial arrows should appear relative to the horizontial track.'),
  );
  $form['scrollbar_advancedOptions']['scrollbar_generalOptions']['scrollbar_autoReinitialise'] = array(
    '#type' => 'select',
    '#title' => t('Reinitialise scrollbar'),
    '#default_value' => variable_get('scrollbar_autoReinitialise', 'false'),
    '#options' => array(
      'true' => t('true'),
      'false' => t('false'),
    ),
    '#description' => "<p>" . t('Whether scrollbar should automatically reinitialise itself periodically after you have initially initialised it.') . "</p>" .
    "<p>" . t('This can help with instances when the size of the content of the scrollpane (or the surrounding element) changes.') . "</p>" .
    "<p>" . t('However, it does involve an overhead of running a javascript function on a timer so it is recommended only to activate where necessary.') . "</p>",
  );
  $form['scrollbar_advancedOptions']['scrollbar_generalOptions']['scrollbar_autoReinitialiseDelay'] = array(
    '#type' => 'textfield',
    '#title' => t('Reinitialise Delay in ms'),
    '#default_value' => variable_get('scrollbar_autoReinitialiseDelay', 500),
    '#size' => 5,
    '#maxlength' => 5,
    '#element_validate' => array('scrollbar_validate_integer_positive_zero'),
    '#description' => t('The number of milliseconds between each reinitialisation (if autoReinitialise is true).'),
  );
  return system_settings_form($form);
}

<?php
/**
 * Quizzler implements hooks to allow it's own field modules (and yours!) to interact and extend Quizzler.
 * 
 * This file also documents the interface field modules must supply in order to work inside the system.
 */

/**********************************************************************************************************
 * Quizzler API
 * 
 * You may implement these hooks to interact with Quizzler.
 */

/**
 * 
 * Called when a user submits a quiz.
 * 
 * @param $quiz_data
 * A keyed array with the following properties:
 *   - qid							The id of the quiz
 *   - nid							The nid of the node the quiz is attached to
 *   - title						The title of the quiz
 *   - account					The account taking the quiz
 *   - answered					The count of questions answered.
 *   - total_questions  The count of questions on the quiz
 *   - correct	  			Overall amount of questions answered correctly
 *   - percent					The percentage points correct
 *   - grade						The letter grade
 */
function hook_quizzler_quiz_submitted($quiz_data) {
  
}

/**********************************************************************************************************
 * Field API
 * 
 * Implement these to create your own field definition
 */
/**
 * 
 * Return an array containing information about your field.
 * 
 * Possible values to insert here:
 * 	name: 		(required) the name of the field to display on admin forms
 * 
 */
function hook_quizzler_field_info() {
  return array(
    'name' => t("My field name")
  );
}

/**
 * 
 * Called when constructing the form the user sees when taking the quiz.
 * 
 * @param $field
 * The field instance for this element.  Make sure you pay attention to $field->required in your field elements.
 * 
 * @param $element
 * The form element
 * 
 * @return
 * The altered form element
 */
function hook_quizzler_user_element($field, $element) {
  $my_data = get_some_data($field);
  $element['my_field'] = array(
    '#type'		=> 'checkbox',
    '#title'  => $my_data['title']
  );
  
  return $element;
}

/**
 * 
 * Collect form data from the user submitted quiz and store your values to the database.
 * 
 * Unless you have special data to save, it is recommended that you store the answer with quizzler_save_answer().
 * If you store the answer in another way, you must also implement hook_quizzler_user_element_grade() to supply 
 * a grade for this question.
 * 
 * @see quizzler_save_answer()
 * 
 * @param $field
 * The field instance.
 * 
 * @param $form_state
 * The form data.
 */
function hook_quizzler_user_element_submit($field, $form_state) {
  $my_value = $form_state['my_field'];
  mymodule_store_value($my_value);
}

/**
 * 
 * Called when you arrange for an AJAX callback on a button you provide in the form.
 * 
 * Make sure your button contains the following #ajax entry
 * 
 * $form['my_button'] = array(
 *   '#type' 	=> 'button',
 *   '#value'	=> t('click me'),
 *   '#ajax'  => array(
 *     'callback'	=> 'quizzler_ajax',
 *     'wrapper'	=> 'my_module-' . $arbitrary_strings
 *   )
 * );
 * 
 * When the button is clicked, the hook 'my_module_ajax_callback' will be called.
 * It should return the section of the form to send back to the browser.
 * 
 * @param $form
 * @param $field_state
 * 
 * @return
 * A renderable array, for example $form['quizzler']['my_options']
 */
function hook_quizzler_ajax_callback($form, $field_state) {
  
}

/**
 * 
 * This hook is called when a quiz has been loaded from the database.
 * 
 * Field modules will want to implement this to populate the $quiz->fields array.
 * 
 * This example shows how a field module should implement this hook to add itself to the quiz.
 * 
 * @param $quiz
 * A quiz object.
 */
function hook_quizzler_load($quiz) {
}

/**
 * 
 * Called when a field instance is loaded.  Use this hook to populate the field with your custom data.
 * 
 * @param $field
 */
function hook_quizzler_field_load($field) {
  $my_data = mymodule_get_my_data($field);
  
  $field->my_data = $my_data;
}

/**
 * 
 * Called when the form element for a field is being created.  You can implement this hook to add your own form
 * elements to the field.
 * 
 * Be sure to implement hook_quizzler_field_submit, validate and save as well.
 * 
 * @param $element
 * The current field form element (a container)
 * 
 * @param $form_data
 * 
 * @param $field
 * The field definition
 */
function hook_quizzler_field_element_alter($element, $form_data, $field) {
  $my_setting = mymodule_get_setting($field);

  $element['my_field'] = array(
    '#type'	          => 'checkbox',
    '#title'	        => t("Activate special feature?"),
    '#default_value'	=> (isset($form_data['values']['my_field']))? $form_data['values']['my_field'] : $my_setting,
    // etc, 
  );
  
  return $element;
}

/**
 * Triggered when a form is submitted.  This does not save the data.
 * 
 * You should implement this if you need to transform custom form data and add it to a field object.
 * 
 * Standard settings (the properties that live in the base field table) will be added to the field object automatically.  You
 * are responsible for saving any additional information by implementing hook_field_element_save().
 */
function hook_quizzler_field_submit(&$field, $form_data) {
  $field->my_custom_field = $form_data['values']['my_custom_field'];
}

/**
 * Triggered when a form is validated.
 * 
 * You should implement this if you need to validate custom form data.
 * 
 * Standard settings (the properties that live in the base field table) will be validated for you.
 * 
 * If you have properly implemented hook_quizzler_field_submit() then you won't need $form_state here, as your data (correct or not)
 * will already be in $field by this point.
 */
function hook_quizzler_field_validate($field) {
  if (!$form_data['values']['my_custom_field']) {
    form_set_error('my_custom_field', t("My error message"));
  }
}

/**
 * Triggered when a field is saved.
 * 
 * You should implement this if you need to save custom form data.
 * 
 * Standard settings (the properties that live in the base field table) will be saved for you.
 */
function hook_quizzler_field_save($field) {
  db_insert('my_custom_table')->fields(array('my_field' => $field->my_value))->execute();
}


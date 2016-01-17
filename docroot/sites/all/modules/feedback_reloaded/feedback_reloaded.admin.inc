<?php

/**
 * Form callback.
 * Return feedback reloaded admin settings form.
 *
 * @see feedback_reloaded_menu()
 */
function feedback_reloaded_settings_form() {
  $form = array();

  $option = array(
    'public://feedback_reloaded_screenshots' => t('Public'),
    'private://feedback_reloaded_screenshots' => t('Private (Recommended)')
  );


  $form['feedback_reloaded_screenshot_uri'] = array(
   '#type' => 'radios',
    '#title' => t('Screenshot Storage'),
    '#options' => $option,
    '#description' => t('Select where you want to save screenshots associated with feedbacks. It is recommended that screenshots should be saved in private storage as they may contain sensitive information.<br>In case private storage is not available in your Drupal installation you need to set private file system path under Configuration > File System.'),
    '#default_value' => variable_get('feedback_reloaded_screenshot_uri'),
  );

  // If the user doesn't have a private filesystem configured then disable the private file system option
  if(!variable_get('file_private_path')) {
     $form['feedback_reloaded_screenshot_uri']['private://feedback_reloaded_screenshots']['#disabled'] = TRUE;
  }
  $form['feedback_reloaded_after_save_text'] = array(
    '#type' => 'textarea',
    '#title' => t('After feedback save message'),
    '#description' => t('Please specify the message to be shown after successfully saving feedback.'),
    '#default_value' => variable_get('feedback_reloaded_after_save_text','Feedback Submitted Successfully. Thanks for your feedback.'),
  );

	$form['feedback_reloaded_excluded_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths to exclude from feedback button display'),
    '#default_value' => variable_get('feedback_reloaded_excluded_paths', 'admin/reports/feedback'),
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );

  $form['feedback_reloaded_screenshot_method'] = array(
    '#type' => 'radios',
    '#title' => t('Select the method to create the screenshot'),
    '#options' => array(
      'java' => t('Use java on the client side'),
      'html2canvas' => t('Use the html2canvas library'),
    ),
    '#default_value' => variable_get('feedback_reloaded_screenshot_method', 'java'),
  );

  return system_settings_form($form);
}

/**
 * Feedback Type UI Controller.
 */
class FeedbackRTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage Feedback types, including fields.';
    return $items;
  }
}

/**
 * Generates the feedback type editing form.
 */
function feedback_reloaded_type_form($form, &$form_state, $feedback_reloaded_type, $op = 'edit') {

  if ($op == 'clone') {
    $feedback_reloaded_type->label .= ' (cloned)';
    $feedback_reloaded_type->type .= '_clone';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $feedback_reloaded_type->label,
    '#description' => t('The human-readable name of this feedback type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Machine-readable type name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($feedback_reloaded_type->type) ? $feedback_reloaded_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $feedback_reloaded_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'feedback_reloaded_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this feedback type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => isset($feedback_reloaded_type->description) ? $feedback_reloaded_type->description : '',
    '#description' => t('Description about the feedback type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Feedback type'),
    '#weight' => 40,
  );
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function feedback_reloaded_type_form_submit(&$form, &$form_state) {
  $feedback_reloaded_type = entity_ui_form_submit_build_entity($form, $form_state);
  feedback_reloaded_type_save($feedback_reloaded_type);
  if ($form_state['op'] === 'edit') {
    drupal_set_message(t('Feedback type %label saved.', array('%label' => $feedback_reloaded_type->getTranslation('label'))));
  }
  elseif ($form_state['op'] === 'add') {
    drupal_set_message(t('Feedback type %label created.', array('%label' => $feedback_reloaded_type->getTranslation('label'))));
  }
  $form_state['redirect'] = 'admin/structure/feedbacks';
}

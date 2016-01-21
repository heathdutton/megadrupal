<?php
/**
 * @file
 * Budget administration libraries.
 */

/**
 * The class used for iati budget entities
 */
class IatiBudgetData extends Entity {
  public $name = "IATI Budget";
  /**
   * Contructor.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'iati_budget');
  }
  /**
   * Overrides defaultLabel().
   * @see Entity::defaultLabel()
   */
  protected function defaultLabel() {
    $label = t('@value (@from - @to)', array(
        '@value' => $this->value_amount,
        '@from' => date('Y-m-d', $this->period_start),
        '@to' => date('Y-m-d', $this->period_end),
      )
    );
    return $label;
  }
  /**
   * Overide defaultUri().
   * @see Entity::defaultUri()
   */
  protected function defaultUri() {
    return array('path' => 'admin/content/iati_budget/manage/view/' . $this->bid);
  }
}

class IatiBudgetDataController extends EntityAPIController {
  /**
   * Contructor.
   */
  public function __construct($entity_type) {
    parent::__construct($entity_type);
  }

  /**
   * Create a Budget - we first set up the values that are specific
   * to our model schema but then also go through the EntityAPIController
   * function.
   *
   * @param array $values
   *   The machine-readable type of the model.
   *
   * @return string
   *   A model object with all default fields initialized.
   */
  public function create(array $values = array()) {
    global $user;
    // Add values that are specific to our Model.
    $values += array(
      'bid' => '',
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $user->uid,
      'period_start' => 0,
      'period_start_text' => '',
      'period_end_text' => '',
      'period_end' => 0,
      'value_amount' => 0,
      'value_date' => 0,
    );

    $model = parent::create($values);
    return $model;
  }
  /**
   * Override delete entity.
   * @see EntityAPIController::delete()
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    $model = parent::delete($ids, $transaction);
    return $model;
  }
}

/**
 * UI controller for Task Type.
 */
class IatiBudgetUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage Budget.';
    return $items;
  }

}

/**
 * Implements hook_form().
 */
function iati_budget_form($form, &$form_state, $entity = NULL) {

  if (!isset($entity)) {
    $entity = entity_get_controller('iati_budget')->create();
  }
  $form['value_amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Value Amount'),
    '#default_value' => $entity->value_amount,
    '#required' => TRUE,
    '#weight' => -10,
  );
  $form['value_date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Value Date'),
    '#default_value' => date('Y-m-d', $entity->value_date),
    '#required' => TRUE,
    '#date_format' => 'Y-m-d',
    '#date_year_range' => '-1:+0',
    '#weight' => -8,
  );
  $form['period_start'] = array(
    '#type' => 'date_popup',
    '#title' => t('Period Start'),
    '#default_value' => date('Y-m-d', $entity->period_start),
    '#required' => FALSE,
    '#date_format' => 'Y-m-d',
    '#date_year_range' => '-0:+10',
    '#weight' => -6,
  );
  $form['period_start_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Period Start Text'),
    '#default_value' => $entity->period_start_text,
    '#required' => FALSE,
    '#weight' => -4,
  );
  $form['period_end'] = array(
    '#type' => 'date_popup',
    '#title' => t('Period End'),
    '#default_value' => date('Y-m-d', $entity->period_end),
    '#required' => FALSE,
    '#date_format' => 'Y-m-d',
    '#date_year_range' => '-0:+10',
    '#weight' => -2,
  );
  $form['period_end_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Period End Text'),
    '#default_value' => $entity->period_end_text,
    '#required' => FALSE,
    '#weight' => -1,
  );

  field_attach_form('iati_budget', $entity, $form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 100,
  );

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function iati_budget_form_submit(&$form, &$form_state) {
  $budget = entity_ui_form_submit_build_entity($form, $form_state);
  
  $budget->period_start = iati_convert_date_string_to_timestamp($budget->period_start, 'Y-m-d');
  $budget->period_end = iati_convert_date_string_to_timestamp($budget->period_end, 'Y-m-d');
  $budget->value_date = iati_convert_date_string_to_timestamp($budget->value_date, 'Y-m-d');
  // Save and go back.
  $budget->save();
}

/**
 * Implements hook_form_validate().
*/
function iati_budget_form_validate(&$form, &$form_state) {
  // If both period start date and period start text are missing.
  if ( !isset($form_state['values']['period_start']) && $form_state['values']['period_start_text'] == '') {
    form_set_error('period_start', t('You need to fill in one of the two fields Period Start or Period Start Text.'));
  }
  // If both period start date and period start test are filled in.
  if (isset($form_state['values']['period_start']) && $form_state['values']['period_start_text'] != '') {
    form_set_error('period_start', t('You need to fill in one of the two fields Period Start or Period Start Text (Not both). Please delete of of the values.'));
  }
  // If both period end date and period end text are missing
  if (!isset($form_state['values']['period_end']) && $form_state['values']['period_end_text'] == '') {
    form_set_error('period_end', t('You need to fill in one of the two fields Period End or Period End Text.'));
  }

  // If both period start date and period start test are filled in.
  if (isset($form_state['values']['period_end']) && $form_state['values']['period_end_text'] != '') {
    form_set_error('period_end', t('Only one of the two fields Period End or Period End Text (Not both). Please delete of of the values.'));
  }
}

/**
 * Converts date string to timestamp.
 * 
 * @param string $date_string
 *   Date
 * @param string $date_format
 *   Date format
 */
function iati_convert_date_string_to_timestamp($date_string, $date_format) {
  $date = date_create_from_format($date_format, $date_string);
  return $date->getTimestamp();
}

/**
 * Generate an interface to view budget items.
 * 
 * @param object $entity
 *   Budget item
 * @param string $view_mode
 *   View mode
 */
function iati_budget_view($entity, $view_mode = 'tweaky') {
  // Our entity type, for convenience.
  $entity_type = 'iati_budget';
  // Start setting up the content.
  $entity->content = array(
    '#view_mode' => $view_mode,
  );
  // Build fields content - this is where the Field API really comes in to play.
  // The task has very little code here because it all gets taken care of by
  // field module.
  // field_attach_prepare_view() lets the fields load any data they need
  // before viewing.
  field_attach_prepare_view($entity_type, array($entity->bid => $entity), $view_mode);
  // We call entity_prepare_view() so it can invoke hook_entity_prepare_view()
  // for us.
  entity_prepare_view($entity_type, array($entity->bid => $entity));
  // Now field_attach_view() generates the content for the fields.
  $entity->content += field_attach_view($entity_type, $entity, $view_mode);

  // OK, Field API done, now we can set up some of our own data.
  $entity->content['value_amount'] = array(
    '#type' => 'item',
    '#title' => t('Value Amount'),
    '#markup' => $entity->value_amount,
    '#weight' => -10,
  );
  $entity->content['value_date'] = array(
    '#type' => 'item',
    '#title' => t('Value Date'),
    '#markup' => $entity->value_date,
    '#weight' => -9,
  );
  $entity->content['period_start'] = array(
    '#type' => 'item',
    '#title' => t('Period Start'),
    '#markup' => $entity->period_start,
    '#weight' => -8,
  );
  $entity->content['period_start_text'] = array(
    '#type' => 'item',
    '#title' => t('Period_start_text'),
    '#markup' => $entity->period_start_text,
    '#weight' => -7,
  );
  $entity->content['period_end'] = array(
    '#type' => 'item',
    '#title' => t('Period End'),
    '#markup' => $entity->period_end,
    '#weight' => -6,
  );
  $entity->content['period_end_text'] = array(
    '#type' => 'item',
    '#title' => t('Period End Text'),
    '#markup' => $entity->period_end_text,
    '#weight' => -5,
  );
  // Now to invoke some hooks. We need the language code for
  // hook_entity_view(), so let's get that.
  global $language;
  $langcode = $language->language;
  // And now invoke hook_entity_view().
  module_invoke_all('entity_view', $entity, $entity_type, $view_mode,
  $langcode);
  // Now invoke hook_entity_view_alter().
  drupal_alter(array('entity_example_basic_view', 'entity_view'),
  $entity->content, $entity_type);

  // And finally return the content.
  return $entity->content;
}

/**
 * UI controller for Task Type.
 */
class BudgetInlineEntityFormController extends EntityInlineEntityFormController {
  /**
   * Overrides EntityInlineEntityFormController::entityForm().
   */
  public function entityForm($entity_form, &$form_state) {
    $entity = $entity_form['#entity'];

    $entity_form['value_amount'] = array(
      '#type' => 'textfield',
      '#title' => t('Value Amount'),
      '#default_value' => $entity->value_amount,
      '#required' => TRUE,
      '#weight' => -10,
    );
    $entity_form['value_date'] = array(
      '#type' => 'date_popup',
      '#title' => t('Value Date'),
      '#default_value' => date('Y-m-d', strtotime($entity->value_date)),
      '#required' => TRUE,
      '#date_format' => 'Y-m-d',
      '#date_year_range' => '-1:+0',
      '#weight' => -8,
    );
    $entity_form['period_start'] = array(
      '#type' => 'date_popup',
      '#title' => t('Period Start'),
      '#default_value' => date('Y-m-d', strtotime($entity->period_start)),
      '#required' => FALSE,
      '#date_format' => 'Y-m-d',
      '#date_year_range' => '-0:+10',
      '#weight' => -6,
    );
    $entity_form['period_start_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Period Start Text'),
      '#default_value' => $entity->period_start_text,
      '#required' => FALSE,
      '#weight' => -4,
    );
    $entity_form['period_end'] = array(
      '#type' => 'date_popup',
      '#title' => t('Period End'),
      '#default_value' => date('Y-m-d', strtotime($entity->period_end)),
      '#required' => FALSE,
      '#date_format' => 'Y-m-d',
      '#date_year_range' => '-0:+10',
      '#weight' => -2,
    );
    $entity_form['period_end_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Period End Text'),
      '#default_value' => $entity->period_end_text,
      '#required' => FALSE,
      '#weight' => -1,
    );

    field_attach_form('iati_budget', $entity, $entity_form, $form_state);

    $ief_id = $entity_form['#ief_id'];
    $inline_entity_form = $form_state['inline_entity_form'][$ief_id];
    switch ($inline_entity_form['instance']['field_name']) {
      case 'field_iati_activity_budget':
        $entity_form['field_iati_org_ref']['#type'] = 'hidden';
        $entity_form['field_recipient_org']['#type'] = 'hidden';
        $entity_form['field_recipient_country']['#type'] = 'hidden';
        break;
      case 'field_total_budget':
        $entity_form['field_iati_activity_ref']['#type'] = 'hidden';
        $entity_form['field_budget_type']['#type'] = 'hidden';
        $entity_form['field_recipient_org']['#type'] = 'hidden';
        $entity_form['field_recipient_country']['#type'] = 'hidden';
        break;
      case 'field_recipient_org_budget':
        $entity_form['field_iati_activity_ref']['#type'] = 'hidden';
        $entity_form['field_budget_type']['#type'] = 'hidden';
        $entity_form['field_recipient_country']['#type'] = 'hidden';
        break;
      case 'field_recipient_country_budget':
        $entity_form['field_iati_activity_ref']['#type'] = 'hidden';
        $entity_form['field_budget_type']['#type'] = 'hidden';
        $entity_form['field_recipient_org']['#type'] = 'hidden';
        break;
    }
 
    return $entity_form;
  }
  /**
   * (non-PHPdoc)
   * @see EntityInlineEntityFormController::entityFormValidate()
   */
  public function entityFormValidate($entity_form, &$form_state) {
  	//parent::entityFormValidate($entity_form, $form_state);
  	$budget = $entity_form['#entity'];
  	$parents_path = implode('][', $entity_form['#parents']);
    
  	$amount = $form_state['values']['field_total_budget']['und']['form']['value_amount'];
  	$start_date = $form_state['values']['field_total_budget']['und']['form']['period_start'];
  	$end_date = $form_state['values']['field_total_budget']['und']['form']['period_end'];
  	 
  	if(!is_numeric($amount )) {
  		form_set_error($parents_path.'][value_amount', t('Amount has to be a numeric value'));
  	}
  	else {
  	  if($amount < 0) {
  		  form_set_error($parents_path.'][value_amount', t('Amount cant be a negative number'));
  	  }  
  	}
	
  	if($start_date > $end_date) {
  		form_set_error($parents_path.'][period_start', t('Start date can not be after end date'));
  	}
  	field_attach_form_validate('iati_budget', $budget , $entity_form, $form_state);
  	 
  	//if()
  }  
  
}

<?php
/**
 * @file
 * Manage Location Entity.
 */

/**
 * The class used for iati location entities
 */
class IatiLocationData extends Entity {
  public $name = "IATI Location";
  /**
   * Constructor.
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'iati_location');
  }

  /**
   * Override defaultLabel().
   * @see Entity::defaultLabel()
   */
  protected function defaultLabel() {
    return $this->name;
  }
  /**
   * Override defaultUri().
   * @see Entity::defaultUri()
   */
  protected function defaultUri() {
    return array('path' => 'admin/content/iati_location/manage/view/' . $this->lid);
  }
}

class IatiLocationDataController extends EntityAPIController {
  /**
   * Constructor.
   */
  public function __construct($entity_type) {
    parent::__construct($entity_type);
  }

  /**
   * Create a Location - we first set up the values that are specific
   * to our model schema but then also go through the EntityAPIController
   * function.
   * @return string
   *   A model object with all default fields initialized.
   */
  public function create(array $values = array()) {
    global $user;
    // Add values that are specific to our Model.
    $values += array(
      'lid' => 0,
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $user->uid,
      'name' => '',
      'description' => '',
    );

    $model = parent::create($values);
    return $model;
  }
  /**
   * Delete location entity.
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
class IatiLocationUIController extends EntityDefaultUIController {
  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage Location.';
    return $items;
  }
}

/**
 * Implements hook_form().
 */
function iati_location_form($form, &$form_state, $entity = NULL) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Location Name'),
  	'#description' => t("The human-readable name for the location. May be repeated in different languages. <a class='iatidoc' href='http://iatistandard.org/activities-standard/location/name/'>Read More</a>"),
    '#default_value' => $entity->name,
    '#weight' => -50,
    /*
     * The IATI standard does not make this mandatory, but its just so much nicer that we do it in the UI
     */
    '#required' => TRUE,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Location Description'),
  	'#description' =>t("A longer, human-readable description. May be repeated for different languages. <a class='iatidoc' href='http://iatistandard.org/activities-standard/location/description/'>Read More</a>"),
    '#default_value' => $entity->description,
    '#weight' => -45,
    '#required' => FALSE,
  );

  field_attach_form('iati_location', $entity, $form, $form_state);

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
function iati_location_form_submit(&$form, &$form_state) {
  $location = entity_ui_form_submit_build_entity($form, $form_state);

  $location->save();
  $form_state['redirect'] = 'admin/content/iati_location';
}

/**
 * Generate a Location entity view interface.
 *
 * @param object $entity
 *   Entity Object
 * @param string $view_mode
 *   View Mode
 */
function iati_location_view($entity, $view_mode = 'tweaky') {
  // Our entity type, for convenience.
  $entity_type = 'iati_location';
  // Start setting up the content.
  $entity->content = array(
    '#view_mode' => $view_mode,
  );
  // Build fields content - this is where the Field API really comes in to play.
  // The task has very little code here because it all gets taken care of by
  // field module.
  // field_attach_prepare_view() lets the fields load any data they need
  // before viewing.
  field_attach_prepare_view($entity_type, array($entity->lid => $entity),
      $view_mode);
  // We call entity_prepare_view() so it can invoke hook_entity_prepare_view()
  // for us.
  entity_prepare_view($entity_type, array($entity->lid => $entity));
  // Now field_attach_view() generates the content for the fields.
  $entity->content += field_attach_view($entity_type, $entity, $view_mode);

  // OK, Field API done, now we can set up some of our own data.
  $entity->content['name'] = array(
    '#type' => 'item',
    '#title' => t('Location Name'),
    '#markup' => $entity->name,
    '#weight' => -10,
  );
  $entity->content['description'] = array(
    '#type' => 'item',
    '#title' => t('Description'),
    '#markup' => $entity->description,
    '#weight' => -9,
  );


  // Now to invoke some hooks. We need the language code for
  // hook_entity_view(), so let's get that.
  global $language;
  $langcode = $language->language;
  // And now invoke hook_entity_view().
  module_invoke_all('entity_view', $entity, $entity_type, $view_mode,
  $langcode);
  // Now invoke hook_entity_view_alter().
  drupal_alter(array('iati_location_view', 'entity_view'),
  $entity->content, $entity_type);

  // And finally return the content.
  return $entity->content;
  
  
  
  
}



/**
 * UI controller for Task Type.
 */
class LocationInlineEntityFormController extends EntityInlineEntityFormController {
  /**
   * Overrides EntityInlineEntityFormController::entityForm().
   */
  public function entityForm($entity_form, &$form_state) {
    $entity = $entity_form['#entity'];
    //--------
    $entity_form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Location Name'),
        '#description' => t("The human-readable name for the location. May be repeated in different languages. <a class='iatidoc' href='http://iatistandard.org/activities-standard/location/name/'>Read More</a>"),
        '#default_value' => $entity->name,
        '#weight' => -50,
        /*
         * The IATI standard does not make this mandatory, but its just so much nicer that we do it in the UI
    */
        '#required' => TRUE,
    );
    $entity_form['description'] = array(
        '#type' => 'textfield',
        '#title' => t('Location Description'),
        '#description' =>t("A longer, human-readable description. May be repeated for different languages. <a class='iatidoc' href='http://iatistandard.org/activities-standard/location/description/'>Read More</a>"),
        '#default_value' => $entity->description,
        '#weight' => -45,
        '#required' => FALSE,
    );
    
    field_attach_form('iati_location', $entity, $entity_form, $form_state);
  /*  
    $entity_form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#weight' => 100,
    );
    //------------------------
    
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
        '#default_value' => date('Y-m-d', $entity->value_date),
        '#required' => TRUE,
        '#date_format' => 'Y-m-d',
        '#date_year_range' => '-1:+0',
        '#weight' => -8,
    );
    $entity_form['period_start'] = array(
        '#type' => 'date_popup',
        '#title' => t('Period Start'),
        '#default_value' => date('Y-m-d', $entity->period_start),
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
        '#default_value' => date('Y-m-d', $entity->period_end),
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
*/
    $ief_id = $entity_form['#ief_id'];
    // $inline_entity_form = $form_state['inline_entity_form'][$ief_id];

    return $entity_form;
  }
}

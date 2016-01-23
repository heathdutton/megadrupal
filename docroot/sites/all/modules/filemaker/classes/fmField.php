<?php

/**
 * @file
 * Class for a FileMaker field.
 */

class FmField extends FmElement {

/**********************************************************************************
 * Database Fields (Public, to work with save() in FmBase).
 *********************************************************************************/

  /**
   * Unique key in database.
   *
   * @int $fmfid
   */
  public $fmfid;

  /**
   * The label to display for the FileMaker field.
   *
   * @string $label
   */
  public $label;

  /**
   * The FileMaker data type.
   *  Examples: 'text', 'number', 'date', 'container', etc..
   *
   * @var string $type
   */
  public $type;

  /**
   * The html widget to use.
   *  Examples: 'date', 'text_field', 'text_area', etc..
   *
   * @var string $widget
   */
  public $widget;

  /**
   * The default value for this FileMaker field. Can be dynamic or static.
   *
   * @var string $default_value.
   */
  public $default_value;

  /**
   * The default value for this FileMaker field in find mode. Can be dynamic or static.
   *
   * @var string $default_find_value.
   */
  public $default_find_value;

  /**
   * The default value in create mode. Can be dynamic or static.
   *
   * @var string $default_create_value
   */
  public $default_create_value;

  /**
   * True if this FileMaker field is used in create mode, false if not.
   *
   * @var bool $include_field_in_create
   */
  public $include_field_in_create;

  /**
   * True if this FileMaker field is used in search queries, false if not.
   *
   * @var bool $searchable
   */
  public $searchable;

  /**
   * True if this FileMaker field needs an exact match, false if not.
   *
   * @var bool $exact_search
   */
  public $exact_search;

  /**
   * True if this FileMaker field is used in displaying search results, false if not.
   *
   * @var bool $include_field_in_search_results
   */
  public $include_field_in_search_results;

  /**
   * True if this FileMaker field is required for the user to fill in, false if not.
   *
   * @var bool $required
   */
  public $required;

/**********************************************************************************
 * Non-database attributes.
 *********************************************************************************/

  /**
   * The FileMaker field names associated with this layout, to be used as select
   * options in a Drupal form.
   *
   * @var array $field_options
   */
  public $field_options;

  /**
   * The FileMaker value list for this field, to be used as select
   * options in a Drupal form.
   *
   * @var array $value_list_options
   */
  public $value_list_options;


/**********************************************************************************
 * Consts.
 *********************************************************************************/

  const ID_FIELD_NAME = 'fmfid';
  const TABLE_NAME = 'filemaker_field';

/**********************************************************************************
 * Public functions.
 *********************************************************************************/

  public function get_all_as_options_for_form() { }

  public function table(array $fields) {

    if (empty($fields)) {
      return t('There are not any selected fields for this FileMaker website.');
    }

    $header = array('Weight', 'Label', 'Name', 'Widget', 'Required?', 'Actions');
    $rows = array();
    
    foreach ($fields as $field) {

      $edit_link = l('Edit', 'node/' . $field->nid . '/layout/field/' . $field->fmfid);
      $delete_link = l('Delete', 'node/' . $field->nid . '/layout/field/' . $field->fmfid . '/delete');

      $row = array(
        $field->weight,
        $field->label,
        $field->name,
        $field->widget,
        ($field->required) ? 'Yes' : 'No',
        $edit_link . ' | ' . $delete_link,
      );
      $rows[] = $row;
    }

    $variables = array('header' => $header, 'rows' => $rows);
    return theme('table', $variables);
  }

  public function admin_form() {

    // Field widget.
    $widget_options = array(
      'textfield' => 'Text Field',
      'textarea' => 'Text Area',
      'date' => 'Date',
      'hidden' => 'Hidden',
      'select' => 'Select List',
      'radios' => 'Radio Buttons',
    );

    // Options for find and create defaults.
    $default_value_options = array(
      'none' => 'None',
      'default' => 'Default Value',
      'username' => 'Drupal User Name',
      'uid' => 'Drupal User ID (uid)',
      'title' => 'Drupal Node Title',
      'nid' => 'Drupal Node ID (nid)',
    );

    $form = array();
    
    // Insert validation, submit function names, and redirect location.
    $form['#submit'][] = 'filemaker_field_submit';
    
    // Create field fieldset.
    $form['field'] = array(
      '#title' => t('FileMaker web layout field'),
      '#type' => 'fieldset',
    );

    // Name field.
    $form['field']['name'] = array(
      '#title' => t('Name'),
      '#type' => 'select',
      '#required' => true,
      '#options' => $this->field_options,
      '#default_value' => $this->name,
    );

    // Widget field.
    $form['field']['widget'] = array(
      '#title' => t('Widget'),
      '#type' => 'select',
      '#options' => $widget_options,
      '#required' => TRUE,
      '#default_value' => $this->widget,
    );
    
    // Label field.
    $form['field']['label'] = array(
      '#title' => t('Label'),
      '#type' => 'textfield',
      '#required' => true,
      '#default_value' => $this->label,
    );
    
    // Default Value field.
    $form['field']['default_value'] = array(
      '#title' => t('Default Value'),
      '#type' => 'textfield',
      '#default_value' => $this->default_value,
    );
    // Default Create Value field.
    $form['field']['default_create_value'] = array(
      '#title' => t('Default Create Value'),
      '#type' => 'select',
      '#options' => $default_value_options,
      '#required' => TRUE,
      '#default_value' => ($this->default_create_value) ? $this->default_create_value : 'default',
    );
    // Default Find Value field.
    $form['field']['default_find_value'] = array(
      '#title' => t('Default Find Value'),
      '#type' => 'textfield',
      '#type' => 'select',
      '#options' => $default_value_options,
      '#required' => TRUE,
      '#default_value' => ($this->default_find_value) ? $this->default_find_value : 'none',
    );
    // Required field.
    $form['field']['required'] = array(
      '#title' => t('Required?'),
      '#type' => 'checkbox',
      '#default_value' => $this->required,
    );
    // Weight field.
    $form['field']['weight'] = array(
      '#title' => t('Weight'),
      '#type' => 'weight',
      '#default_value' => isset($this->weight) ? $this->weight : 0,
    );

    // Create default settings fieldset.
    $form['default_settings'] = array(
      '#title' => t('Default Drupal View'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
    );

    // Create default settings fieldset.
    $form['default_settings']['hint'] = array(
      '#title' => t('About the Default Drupal View'),
      '#type' => 'item',
      '#markup' => t('The default Drupal view is what non-authenticated users will see, and can be found at: <br /><strong>node/[nid]</strong>'),
    );

    // Create default settings - create fieldset.
    $form['default_settings']['create'] = array(
      '#title' => t('Create'),
      '#type' => 'fieldset',
    );

    $form['default_settings']['create']['include_field_in_create'] = array(
      '#title' => t('Include this field in create record form?'),
      '#type' => 'checkbox',
      '#default_value' => (isset($this->include_field_in_create)) ? $this->include_field_in_create : 1,
    );
    
    // Create default settings - find fieldset.
    $form['default_settings']['find'] = array(
      '#title' => t('Find'),
      '#type' => 'fieldset',
    );

    $form['default_settings']['find']['searchable'] = array(
      '#title' => t('Searchable when default view is set to find?'),
      '#type' => 'checkbox',
      '#default_value' => $this->searchable,
    );
    
    $form['default_settings']['find']['exact_search'] = array(
      '#title' => t('Exact search?'),
      '#type' => 'checkbox',
      '#default_value' => $this->exact_search,
    );
    
    $form['default_settings']['find']['include_field_in_search_results'] = array(
      '#title' => t('Include this field in default view search results?'),
      '#type' => 'checkbox',
      '#default_value' => (isset($this->include_field_in_search_results)) ? $this->include_field_in_search_results : 1,
    );
    
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save Field'),
    );

    return $form;
  }


/**********************************************************************************
 * Protected functions.
 *********************************************************************************/

  public function set_values_from_form_state(array $form_state) {
    
    // Do we have a fmfid?
    if (isset($_SESSION['filemaker']['fmfid'])) {

      $fmfid = $_SESSION['filemaker']['fmfid'];
      unset($_SESSION['filemaker']['fmfid']);

      // Is our fmfid valid?
      if ($fmfid > 0) {
        $test_script = new FmField($fmfid);
        if ($test_script->is_valid_in_drupal) {
          $this->fmfid = $fmfid;
        }
      }
    }

    $this->nid = $form_state['build_info']['args'][0]->nid;
    $this->name = $form_state['values']['name'];
    $this->label = $form_state['values']['label'];
    $this->weight = $form_state['values']['weight'];
    $this->widget = $form_state['values']['widget'];
    $this->default_value = $form_state['values']['default_value'];
    $this->default_find_value = $form_state['values']['default_find_value'];
    $this->default_create_value = $form_state['values']['default_create_value'];
    $this->required = $form_state['values']['required'];
    $this->include_field_in_create = $form_state['values']['include_field_in_create'];
    $this->searchable = $form_state['values']['searchable'];
    $this->exact_search = $form_state['values']['exact_search'];
    $this->include_field_in_search_results = $form_state['values']['include_field_in_search_results'];

    return $this;
  }

  protected function set_values(stdClass $field) {

    if ( ! is_object($field)) {
      return FALSE;
    }

    if (isset($field->fmfid)) {
      $this->fmfid = $field->fmfid;
    }
  
    $this->fmfid = $field->fmfid;
    $this->nid = $field->nid;
    $this->name = $field->name;
    $this->label = $field->label;
    $this->weight = $field->weight;
    $this->type = $field->type;
    $this->widget = $field->widget;
    $this->default_value = $field->default_value;
    $this->default_find_value = $field->default_find_value;
    $this->default_create_value = $field->default_create_value;
    $this->required = $field->required;
    $this->include_field_in_create = $field->include_field_in_create;
    $this->searchable = $field->searchable;
    $this->exact_search = $field->exact_search;
    $this->include_field_in_search_results = $field->include_field_in_search_results;
  }
}

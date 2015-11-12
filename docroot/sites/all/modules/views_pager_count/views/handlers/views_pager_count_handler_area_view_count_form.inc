<?php

/**
 * Provides an area handler that displays a form allowing the user to specify
 * the number of items to be displayed on one view page.
 */
class views_pager_count_handler_area_view_count_form extends views_handler_area {

  /**
   * {@inheritdoc}
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['elements'] = array('default' => array(
      'input_type' => 'textfield',
      'select_options' => "10\n20\n50\n100",
      'show_submit' => TRUE,
      'show_reset' => TRUE,
    ));

    $options['labels'] = array('default' => array(
      'input_label' => 'Items per page',
      'submit_label' => 'Show',
      'reset_label' => 'Reset',
    ));

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['elements'] = array(
      '#type' => 'fieldset',
      '#title' => t('Elements'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['elements']['input_type'] = array(
      '#type' => 'select',
      '#title' => t('Input field type'),
      '#description' => t('Type of input field using which users can change number of items displayed per view page. Field label can be changed in the <em>Labels</em> section.'),
      '#options' => drupal_map_assoc(array('textfield', 'select')),
      '#default_value' => $this->options['elements']['input_type'],
    );
    $form['elements']['select_options'] = array(
      '#type' => 'textarea',
      '#title' => t('Possible values'),
      '#description' => t("Possible values which will be displayed in the dropdown field. Please provide one value per line. Note that the default view's <em>Items per page</em> value will be automatically added to this list."),
      '#default_value' => $this->options['elements']['select_options'],
      '#rows' => 3,
      '#states' => array(
        'visible' => array(
          ':input[name="options[elements][input_type]"]' => array('value' => 'select'),
        ),
      ),
    );
    $form['elements']['show_submit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show submit button'),
      '#description' => t('Enabling this option will display submit button on the form. Hiding the submit button will cause the form to get submitted when only the value of input/select field changes. Button label can be changed in the <em>Labels</em> section.'),
      '#default_value' => $this->options['elements']['show_submit'],
    );
    $form['elements']['show_reset'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show reset button'),
      '#description' => t("Enabling this option will display reset button on the form. Reset button removes the user cookie, causing the number of items per page to change back to view's default value. Button label can be changed in the <em>Labels</em> section."),
      '#default_value' => $this->options['elements']['show_reset'],
    );

    $form['labels'] = array(
      '#type' => 'fieldset',
      '#title' => t('Labels'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['labels']['input_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Input field label'),
      '#default_value' => $this->options['labels']['input_label'],
    );
    $form['labels']['submit_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Submit button label'),
      '#default_value' => $this->options['labels']['submit_label'],
    );
    $form['labels']['reset_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Reset button label'),
      '#default_value' => $this->options['labels']['reset_label'],
    );

  }

  /**
   * {@inheritdoc}
   */
  function render($empty = FALSE) {
    $form = drupal_get_form('views_pager_count_view_form', $this->options, $this->view);
    return drupal_render($form);
  }

}

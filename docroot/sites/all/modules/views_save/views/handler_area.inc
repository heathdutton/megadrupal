<?php

/**
 * @file
 * Contains ViewsSaveHandlerArea.
 */

/**
 * Provides a "Save" button for views.
 */
class ViewsSaveHandlerArea extends views_handler_area_text {


  public function option_definition() {
    $options = parent::option_definition();

    $options['bundle'] = array('default' => 'default');
    $options['help_text'] = array('default' => '', 'translatable' => TRUE);
    $options['button_text'] = array('default' => '', 'translatable' => TRUE);
    $options['anonymous'] = array('default' => 'hide');

    return $options;
  }

  /**
   * Defines a configuration form for this handler.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['bundle'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#description' => t('Select the type of saved views that should be created. Manage available types <a href="@url">here</a>.', array('@url' => url('admin/structure/views_save'))),
      '#options' => views_save_get_bundles(),
      '#default_value' => $this->options['bundle'],
    );

    $form['button_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Button label'),
      '#description' => t('The label for the initial save button that opens the popup. Defaults to "Save".'),
      '#default_value' => $this->options['button_text'],
    );

    $form['help_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Help text'),
      '#description' => t('Text that appears when the user is choosing a name for the view to be saved.'),
      '#default_value' => $this->options['help_text'],
    );

    $options = array(
      'hide' => t('Hide Save button'),
      'custom' => t('Show popup with custom text'),
    );
    $permission_url = url('admin/people/permissions', array('fragment' => 'edit-use-views-save'));
    $form['anonymous'] = array(
      '#type' => 'select',
      '#title' => t('Behavior for anonymous users'),
      '#description' => t('What should happen when an anonymous user tries to save a view. Note that this also depends on the <a href="@url">Use Views Save</a> permission.', array('@url' => $permission_url)),
      '#options' => $options,
      '#default_value' => $this->options['anonymous'],
    );

    $states['visible']['#edit-options-anonymous']['value'] = 'custom';
    $form['custom_popup'] = array(
      '#type' => 'fieldset',
      '#title' => t('Popup text for anonymous users'),
      '#states' => $states,
    );
    foreach (array('content', 'tokenize') as $key) {
      $form[$key]['#parents'] = array('options', $key);
      $form['custom_popup'][$key] = $form[$key];
      unset($form[$key]);
    }
  }

  /**
   * Displays the Save button.
   *
   * @param bool $empty
   *   Whether the Views result was empty.
   *
   * @return string
   *   The HTML that should be displayed for this area.
   */
  public function render($empty = FALSE) {
    if ($empty && empty($this->options['empty'])) {
      return '';
    }

    if (!empty($_GET['views_save'])) {
      $save = views_save_load($_GET['views_save']);
      if ($save) {
        $this->view->set_title($save->title);
        $render['save'] = entity_view('views_save', array($save->id => $save));
        $render['save'] = reset($render['save']);
        $render['save'] = reset($render['save']);
        unset($render['save']['#theme']);
      }
    }
    $render['form'] = drupal_get_form('views_save_form', $this->view, $this->options);
    return drupal_render($render);
  }

}

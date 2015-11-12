<?php
/**
 * @file
 * Definition of views_job_board_plugin.
 */

class ViewsJobBoardPluginStyle extends views_plugin_style {
  /**
   * Definition.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['company_name'] = array('default' => '');
    $options['job'] = array('default' => '');
    $options['randomize'] = array('default' => FALSE);

    $options['extra']['logo'] = array('default' => '');
    $options['extra']['sticky'] = array('default' => '');
    $options['extra']['job_type'] = array('default' => '');

    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Pre-build all of our option lists for the dials and switches that follow.
    $fields = array('' => t('<None>'));
    foreach ($this->display->handler->get_handlers('field') as $field => $handler) {
      if ($label = $handler->label()) {
        $fields[$field] = $label;
      }
      else {
        $fields[$field] = $handler->ui_name();
      }
    }

    $form['company_name'] = array(
      '#type' => 'select',
      '#title' => t('Company name'),
      '#options' => $fields,
      '#required' => TRUE,
      '#default_value' => $this->options['company_name'],
    );

    $form['job'] = array(
      '#type' => 'select',
      '#title' => t('Job posts'),
      '#options' => $fields,
      '#required' => TRUE,
      '#default_value' => $this->options['job'],
    );

    $form['extra'] = array(
      '#type' => 'fieldset',
      '#title' => t('Extra options.'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['extra']['logo'] = array(
      '#type' => 'select',
      '#title' => t("Company's logo."),
      '#options' => $fields,
      '#default_value' => $this->options['extra']['logo'],
    );

    $form['extra']['job_type'] = array(
      '#type' => 'select',
      '#title' => t('Job type'),
      '#options' => $fields,
      '#description' => t('If you want to group jobs by type.'),
      '#default_value' => $this->options['extra']['job_type'],
    );

    $form['extra']['sticky'] = array(
      '#type' => 'select',
      '#title' => t('Sticky'),
      '#options' => $fields,
      '#description' => t('If you want to stick some companies at top of board. (Special for randomized case)'),
      '#default_value' => $this->options['extra']['sticky'],
    );

    $form['randomize'] = array(
      '#type' => 'checkbox',
      '#title' => t('Randomize results'),
      '#description' => t('JS will shuffle results after rendering.'),
      '#default_value' => $this->options['randomize'],
    );
  }
}

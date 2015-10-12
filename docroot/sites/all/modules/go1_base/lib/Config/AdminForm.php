<?php
namespace Drupal\go1_base\Config;

class AdminForm {
  private $config;

  /**
   * Form structure.
   *
   * @var array
   */
  private $form;

  /**
   * Form state
   *
   * @var array
   */
  private $form_state;

  public function __construct($module, $id) {
    $this->config = go1_config($module, $id);
  }

  public function setForm($form) {
    $this->form = $form;
  }

  public function setFormState(&$form_state) {
    $this->form_state = &$form_state;
  }

  public function get() {
    $iframe = drupal_get_path('module', 'go1_base') . '/misc/html/jsonEditor.html';
    $iframe = url('<front>', array('absolute' => TRUE)) . '/' . $iframe;

    $form = $this->form;

    $form['go1_config_item'] = array(
      '#type' => 'textarea',
      '#default_value' => json_encode($this->config->getAll()),
      '#prefix' => '<iframe src="'. $iframe .'" style="width: 100%; height: 500px;"></iframe>',
      '#suffix' => '<noscript>Require javascript enabled</noscript>',
    );

    $form['go1_config_submit'] = array('#type' => 'submit', '#value' => t('Save'));

    return $form;
  }

  public function validate() {
    $json = array();

    if (empty($this->form_state['values']['go1_config_item'])) {
      $error = TRUE;
    }
    elseif (!$json = json_decode($this->form_state['values']['go1_config_item'], TRUE)) {
      $error = TRUE;
    }

    if (!empty($error)) {
      form_set_error('', 'Invalid config data');
    }
    else {
      $this->form_state['go1_config_json'] = $json;
    }
  }

  public function submit() {
    $json = &$this->form_state['go1_config_json'];
    $this->config->setAll($json);
    $this->config->write();
  }
}

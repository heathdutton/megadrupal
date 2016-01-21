<?php

/**
 * Contains CToolsTokenAccess.
 */

class CToolsTokenAccess extends CToolsAccessBase {

  /**
   * Parameter name.
   *
   * Name of the URL/POST parameter to check.
   *
   * @var string
   */
  protected $parameter_name;

  /**
   * Variable name.
   *
   * Name of the variable that holds the token to compare to.
   *
   * @var string
   */
  protected $variable_name;

  /**
   * {@inheritdoc}
   */
  public function __construct($conf, $context) {
    parent::__construct($conf, $context);
    $this->parameter_name = empty($conf['parameter_name']) ? '' : $conf['parameter_name'];
    $this->variable_name = empty($conf['variable_name']) ? '' : $conf['variable_name'];
  }

  /**
   * {@inheritdoc}
   */
  public function access() {
    if (!$value = $this->getParameterValue($this->parameter_name)) {
      return FALSE;
    }
    ctools_include('export');
    $record = ctools_export_crud_load('access_token_export', $this->variable_name);
    $token_value = empty($record->value) ? NULL : $record->value;
    return $value === $token_value;
  }

  /**
   * {@inheritdoc}
   */
  public function settingsForm() {
    ctools_include('export');
    $form['settings']['parameter_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Secret Parameter Name'),
      '#description' => t('Set the name of the query parameter.'),
      '#default_value' => $this->parameter_name,
      '#required' => TRUE,
    );

    if ($tokens = ctools_export_crud_load_all('access_token_export')) {
      $options = array(NULL => t('- Select one -'));
      foreach ($tokens as $token) {
        $options[$token->name] = $token->name;
      }
      $form['settings']['variable_name'] = array(
        '#type' => 'select',
        '#title' => t('Token'),
        '#description' => t('The name of the variable that holds the value to test against for access control. !link', array(
          '!link' => l(t('Add another token.'), 'admin/config/people/access-token/add'),
        )),
        '#options' => $options,
        '#default_value' => $this->variable_name,
        '#required' => TRUE,
      );
    }
    else {
      $form['settings']['variable_name'] = array(
        '#markup' => '<p>' . t('There are no existing tokens. Please !link.', array(
          '!link' => l(t('create one'), 'admin/config/people/access-token/add'),
        )) . '</p>'
      );
    }

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function settingsSummary() {
    if (empty($this->parameter_name) || empty($this->variable_name)) {
      return t('This plugin has not been configured yet.');
    }
    ctools_include('export');
    $value = ctools_export_crud_load('access_token_export', $this->variable_name);
    $token_value = $value ? $value->value : t('[value for "@name" not set]', array('@name' => $this->variable_name));
    return t('%query in the request parameter will grant access.', array(
      '%query' => '?' . $this->parameter_name . '=' . $token_value)
    );
  }

  /**
   * Gets the value of an HTTP parameter.
   *
   * This will get the parameter name regardless of the HTTP method.
   *
   * @param $parameter_name
   *   Name of the parameter.
   *
   * @return string
   *   The parameter value.
   */
  public function getParameterValue($parameter_name) {
    // Don't allow empty parameter names.
    if (empty($parameter_name)) {
      return '';
    }
    if (!empty($_GET[$parameter_name])) {
      return $_GET[$parameter_name];
    }
    $parameters = array();
    if (!$input = file_get_contents('php://input')) {
      return '';
    }
    parse_str($input, $parameters);
    return empty($parameters[$parameter_name]) ? '' : $parameters[$parameter_name];
  }

}

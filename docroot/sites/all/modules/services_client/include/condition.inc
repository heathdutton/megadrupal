<?php

/**
 * Interface for implementing entity condition matching.
 */
interface ServicesClientConditionInterface {

  /**
   * Retrieve condition summary for UI.
   *
   * @return string
   *   UI short summary.
   */
  public function getSummary();

  /**
   * Check if entity matches condition.
   *
   * @param stdClass $entity
   *   Drupal entity that should be matched.
   *
   * @return bool
   *   TRUE if matches condition.
   */
  public function match($entity);

}

abstract class ServicesClientConditionPlugin extends ServicesClientPlugin implements ServicesClientConditionInterface {

  /**
   * Constructor.
   */
  public function __construct($event, $config) {
    parent::__construct($event, $config);
    if (empty($this->config)) {
      $this->config = $this->getDefaultConfiguration();
    }
  }

  /**
   * Retrieve default configuration.
   *
   * @return array
   *   Default configuration values.
   */
  protected function getDefaultConfiguration() {
    return array();
  }

}

/**
 * Test property condition of entity against value.
 */
class ServicesClientPropertyCondition extends ServicesClientConditionPlugin {

  /**
   * Retrieve default configuration.
   */
  protected function getDefaultConfiguration() {
    return array(
      'property' => '',
      'condition' => NULL,
    );
  }

  /**
   * Retrieve configuration summary.
   */
  public function getSummary() {
    if (empty($this->config['property'])) {
      return '[ ' . t('Property condition - not configured') . ' ]';
    }
    else {
      return format_string('<b>@property</b> @condition <b>@value</b>', array(
        '@property' => $this->config['property'],
        '@condition' => $this->config['condition'],
        '@value' => $this->config['value'],
      ));
    }
  }

  /**
   * Check if configured property is empty.
   *
   * @param stdClass $entity
   *   Drupal entity that is tested.
   *
   * @return boolean
   *   TRUE if empty
   */
  protected function isEmpty($entity) {
    return empty($entity->{$this->config['property']});
  }

  /**
   * Check if configured property equals configured value.
   *
   * @param stdClass $entity
   *   Drupal entity that is tested.
   *
   * @return boolean
   *   TRUE if empty
   */
  protected function equals($entity) {
    if (!isset($entity->{$this->config['property']})) {
      return FALSE;
    }
    else {
      return ($entity->{$this->config['property']} == $this->config['value']);
    }
  }

  /**
   * Match entity with condition.
   *
   * @param stdClass $entity
   *   Drupal entity.
   *
   * @return boolean
   *   TRUE if entity matches condition.
   */
  public function match($entity) {
    switch ($this->config['condition']) {
      case 'empty':
        return $this->isEmpty($entity);
      case 'not_empty':
        return !$this->isEmpty($entity);
      case 'equals':
        return $this->equals($entity);
      case 'not_equals':
        return !$this->equals($entity);
      default:
        return FALSE;
    }
  }

  /**
   * Configuration form.
   */
  public function configForm(&$form, &$form_state) {
    $form['property'] = array(
      '#type' => 'textfield',
      '#title' => t('Property name'),
      '#required' => TRUE,
      '#description' => t('Enter property name like, "type" or "status"'),
      '#default_value' => isset($this->config['property']) ? $this->config['property'] : NULL,
    );

    $form['condition'] = array(
      '#type' => 'select',
      '#title' => t('Condition'),
      '#options' => array(
        'is_empty' => t('Is Empty'),
        'not_empty' => t('Not Empty'),
        'equals' => t('Equals'),
        'not_equals' => t('Not equals'),
      ),
      '#required' => TRUE,
      '#default_value' => isset($this->config['condition']) ? $this->config['condition'] : NULL,
    );

    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
      '#description' => t('Value that should be tested.'),
      '#default_value' => isset($this->config['value']) ? $this->config['value'] : NULL,
      '#states' => array(
        'invisible' => array(
          ':input[name="condition"]' => array(
            array('value' => 'is_empty'),
            array('value' => 'not_empty'),
          ),
        ),
      ),
    );
  }

  /**
   * Submit handler.
   */
  public function configFormSubmit(&$form, &$form_state) {
    $this->config['property'] = $form_state['values']['property'];
    $this->config['condition'] = $form_state['values']['condition'];
    $this->config['value'] = $form_state['values']['value'];
  }

}

class ServicesClientFieldCondition extends ServicesClientConditionPlugin {
  /**
   * Retrieve default configuration.
   */
  protected function getDefaultConfiguration() {
    return array(
      'field' => '',
      'condition' => NULL,
    );
  }

  /**
   * Retrieve configuration summary.
   */
  public function getSummary() {
    if (empty($this->config['field'])) {
      return '[ ' . t('Field condition - not configured') . ' ]';
    }
    else {
      $path = "{$this->config['field']}[{$this->config['language']}][*][{$this->config['property']}]";
      return format_string('<b>@path</b> @condition <b>@value</b>', array(
        '@path' => $path,
        '@condition' => $this->config['condition'],
        '@value' => $this->config['value'],
      ));
    }
  }

  /**
   * Check if configured property is empty.
   *
   * @param stdClass $entity
   *   Drupal entity that is tested.
   *
   * @return boolean
   *   TRUE if empty
   */
  protected function isEmpty($entity) {
    return empty($entity->{$this->config['field']}[$this->config['language']]);
  }

  /**
   * Check if configured property equals configured value.
   *
   * @param stdClass $entity
   *   Drupal entity that is tested.
   *
   * @return boolean
   *   TRUE if empty
   */
  protected function equals($entity) {
    if ($this->isEmpty($entity)) {
      return FALSE;
    }
    else {
      $items = isset($entity->{$this->config['field']}[$this->config['language']]) ? $entity->{$this->config['field']}[$this->config['language']] : NULL;
      foreach ((array) $items as $item) {
        if (isset($item[$this->config['property']]) && $item[$this->config['property']] == $this->config['value']) {
          return TRUE;
        }
      }
    }

    return FALSE;
  }

  /**
   * Match entity with condition.
   *
   * @param stdClass $entity
   *   Drupal entity.
   *
   * @return boolean
   *   TRUE if entity matches condition.
   */
  public function match($entity) {
    switch ($this->config['condition']) {
      case 'empty':
        return $this->isEmpty($entity);
      case 'not_empty':
        return !$this->isEmpty($entity);
      case 'equals':
        return $this->equals($entity);
      case 'not_equals':
        return !$this->equals($entity);
      default:
        return FALSE;
    }
  }

  /**
   * Configuration form.
   */
  public function configForm(&$form, &$form_state) {
    $form['field'] = array(
      '#type' => 'textfield',
      '#title' => t('Field name'),
      '#required' => TRUE,
      '#description' => t('Enter field name like field_first_name'),
      '#default_value' => isset($this->config['field']) ? $this->config['field'] : NULL,
    );

    $form['language'] = array(
      '#type' => 'textfield',
      '#title' => t('Language'),
      '#required' => TRUE,
      '#description' => t('Enter field language'),
      '#default_value' => isset($this->config['language']) ? $this->config['language'] : LANGUAGE_NONE,
    );

    $form['property'] = array(
      '#type' => 'textfield',
      '#title' => t('Property name'),
      '#required' => TRUE,
      '#description' => t('Enter field property name like "value"'),
      '#default_value' => isset($this->config['property']) ? $this->config['property'] : NULL,
    );

    $form['condition'] = array(
      '#type' => 'select',
      '#title' => t('Condition'),
      '#options' => array(
        'is_empty' => t('Is Empty'),
        'not_empty' => t('Not Empty'),
        'equals' => t('Equals'),
        'not_equals' => t('Not equals'),
      ),
      '#required' => TRUE,
      '#default_value' => isset($this->config['condition']) ? $this->config['condition'] : NULL,
    );

    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
      '#description' => t('Value that should be tested.'),
      '#default_value' => isset($this->config['value']) ? $this->config['value'] : NULL,
      '#states' => array(
        'invisible' => array(
          ':input[name="condition"]' => array(
            array('value' => 'is_empty'),
            array('value' => 'not_empty'),
          ),
        ),
      ),
    );
  }

  /**
   * Submit handler.
   */
  public function configFormSubmit(&$form, &$form_state) {
    $this->config['field'] = $form_state['values']['field'];
    $this->config['language'] = $form_state['values']['language'];
    $this->config['property'] = $form_state['values']['property'];
    $this->config['condition'] = $form_state['values']['condition'];
    $this->config['value'] = $form_state['values']['value'];
  }

}

class ServicesClientUserRoleCondition extends ServicesClientConditionPlugin {
  /**
   * Retrieve default configuration.
   */
  protected function getDefaultConfiguration() {
    return array(
      'roles' => array(),
      'intersect' => FALSE,
      'reverse' => FALSE,
    );
  }

  /**
   * Retrieve condition summary for UI.
   *
   * @return string
   *   UI short summary.
   */
  public function getSummary() {
    if (empty($this->config['roles'])) {
      return '[ ' . t('User roles condition - not configured') . ' ]';
    }
    else {
      $intersect = $this->config['intersect'] ? t('all') : t('at least one');

      $roles = array();
      foreach ($this->config['roles'] as $rid => $role) {
        $role = user_role_load($role);
        $roles[] = $role->name;
      }

      return format_string('User has @intersect of these roles: @roles', array(
        '@intersect' => $intersect,
        '@roles' => implode(', ', $roles),
      ));
    }
  }

  /**
   * Check if entity matches condition.
   *
   * @param stdClass $entity
   *   Drupal entity that should be matched.
   *
   * @return bool
   *   TRUE if matches condition.
   */
  public function match($entity) {
    $roles_intersect = array_intersect($this->config['roles'], $entity->roles);

    if ($this->config['intersect']) {
      $matched = $this->config['roles'] == $roles_intersect;
    }
    else {
      $matched = !empty($roles_intersect);
    }

    return ($this->config['reverse'] ? !$matched : $matched);
  }

  /**
   * Configuration form.
   */
  public function configForm(&$form, &$form_state) {
    $form['roles'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#title' => t('User Roles'),
      '#required' => TRUE,
      '#options' => user_roles(),
      '#description' => t('All users with such roles will be picked'),
      '#default_value' => isset($this->config['roles']) ? $this->config['roles'] : NULL,
    );

    $form['intersect'] = array(
      '#type' => 'checkbox',
      '#title' => t('All roles should match?'),
      '#description' => t('Otherwise user has to have at least one role.'),
      '#default_value' => isset($this->config['intersect']) ? $this->config['intersect'] : NULL,
    );

    $form['reverse'] = array(
      '#type' => 'checkbox',
      '#title' => t('Reverse'),
      '#description' => t('All users without such roles will be picked.'),
      '#default_value' => isset($this->config['reverse']) ? $this->config['reverse'] : NULL,
    );
  }

  /**
   * Submit handler.
   */
  public function configFormSubmit(&$form, &$form_state) {
    $this->config['roles'] = $form_state['values']['roles'];
    $this->config['intersect'] = $form_state['values']['intersect'];
    $this->config['reverse'] = $form_state['values']['reverse'];
  }
}

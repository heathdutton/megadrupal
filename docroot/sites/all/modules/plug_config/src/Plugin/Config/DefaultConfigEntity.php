<?php

/**
 * @file
 * Contains \Drupal\plug_config\Plugin\Config\DefaultConfigEntity.
 */

namespace Drupal\plug_config\Plugin\Config;

use \Entity;

/**
 * Class DefaultConfigEntity
 * @package Drupal\plug_config\Plugin\Config
 *
 * Default config entity class to extend.
 */
class DefaultConfigEntity extends Entity implements DefaultConfigEntityInterface {

  /**
   * The entity numeric ID.
   *
   * @var int
   */
  public $id;

  /**
   * The entity human readable name.
   *
   * @var string
   */
  public $label;

  /**
   * The entity internal machine name.
   *
   * @var string
   */
  public $name;

  /**
   * {@inheritdoc}
   */
  public static function schema() {
    return array(
      'description' => 'Stores all the config example items',
      'fields' => array(
        'id' => array(
          'description' => 'The primary identifier for a item.',
          'type' => 'serial',
          'unsigned' => TRUE,
          'not null' => TRUE,
        ),
        'label' => array(
          'description' => 'The displayed label for a item.',
          'type' => 'varchar',
          'length' => 50,
          'not null' => TRUE,
        ),
        'name' => array(
          'description' => 'The machine name for a item.',
          'type' => 'varchar',
          'length' => 50,
          'not null' => TRUE,
        ),
        // This two columns are required to store meta-information about the
        // exportable entity.
        'status' => array(
          'description' => 'The exportable status of the entity.',
          'type' => 'int',
          'not null' => TRUE,
          'default' => 0x01,
          'size' => 'tiny',
        ),
        'module' => array(
          'description' => 'The name of the providing module if the entity has been defined in code.',
          'type' => 'varchar',
          'length' => 255,
          'not null' => FALSE,
        )
      ),
      'unique keys' => array(
        'name' => array('name'),
      ),
      'indexes' => array(
        'config_name' => array('name'),
      ),
      'primary key' => array('id'),
    );

  }

  /**
   * {@inheritdoc}
   */
  public function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }

  /**
   * {@inheritdoc}
   */
  public static function accessCallback($op, $entity_type, $entity = NULL, $account = NULL) {
    // No access check performed by default. Extend this class to add custom
    // access checkers.
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function form(array $form, array &$form_state, $op = 'edit') {
    $entity_info = $this->entityInfo();
    if ($op == 'clone') {
      $this->setLabel(sprintf('%s (cloned)', $this->getLabel()));
      $this->setName('');
    }

    $form['label'] = array(
      '#title' => t('Label'),
      '#type' => 'textfield',
      '#default_value' => $this->getLabel(),
      '#description' => t('The human-readable name of this Contract type.'),
      '#required' => TRUE,
      '#size' => 30,
    );

    // Machine-readable type name.
    $form['name'] = array(
      '#type' => 'machine_name',
      '#default_value' => $this->getName(),
      '#maxlength' => 32,
      '#disabled' => $this->isLocked() && $op != 'clone',
      '#machine_name' => array(
        'exists' => array(get_called_class(), 'validateMachineName'),
        'source' => array('label'),
      ),
      '#description' => t('A unique machine-readable name for this Contract type. It must only contain lowercase letters, numbers, and underscores.'),
    );

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save @type', array('@type' => $entity_info['label'])),
      '#weight' => 40,
    );

    if (!$this->isLocked() && $op != 'add' && $op != 'clone') {
      $form['actions']['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete @type', array('@type' => $entity_info['label'])),
        '#weight' => 45,
        '#limit_validation_errors' => array(),
      );
    }
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function formSubmit(array $form, array &$form_state) {
    /** @var DefaultConfigEntityInterface $config_example */
    $config_example = entity_ui_form_submit_build_entity($form, $form_state);
    $entity_info = $config_example->entityInfo();

    if ($form_state['triggering_element']['#id'] == 'edit-delete') {
      $form_state['redirect'] = $entity_info['admin ui']['path'] .'/manage/' . $config_example->getName() . '/delete';
      return;
    }

    // Save and go back.
    $config_example->save();

    if ($form_state['op'] == 'edit') {
      drupal_set_message(t('@label @type updated', array('@label' => $entity_info['label'], '@type' => $config_example->getLabel())));
    }
    else {
      drupal_set_message(t('@label @type created', array('@label' => $entity_info['label'], '@type' => $config_example->getLabel())));
    }

    if (isset($entity_info['admin ui']['path'])) {
      // Redirect user back to list of Contract types.
      $form_state['redirect'] = $entity_info['admin ui']['path'];
    }
  }

  /**
   * Validate Callback for entity machine name.
   */
  public static function validateMachineName($value, $element, $form_state) {
    return entity_load($form_state['entity_type'], array($value));
  }

  /**
   * {@inheritdoc}
   */
  public function getLabel() {
    return $this->label;
  }

  /**
   * {@inheritdoc}
   */
  public function setLabel($label) {
    $this->label = $label;
  }

  /**
   * {@inheritdoc}
   */
  public function getName() {
    return $this->name;
  }

  /**
   * {@inheritdoc}
   */
  public function setName($name) {
    $this->name = $name;
  }

}

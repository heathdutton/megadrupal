<?php

/**
 * @file
 * Contains a
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

namespace Drupal\globals\Form;

use Drupal\ghost\Core\Form\BaseForm;
use Drupal\ghost\Core\Form\FormInterface;
use Drupal\ghost\Exception\GhostException;
use Drupal\ghost\Logger\Logger;
use Drupal\globals\Globals;
use Drupal\globals\FormElementFactory;
use Drupal\globals\GlobalItem;

/**
 * Class EditGlobal
 *
 * @package Drupal\globals\Form
 */
class EditGlobal extends BaseForm implements FormInterface {

  /**
   * Form constructor.
   *
   * @param array $form
   *   An associative array containing the structure of the form.
   * @param array $form_state
   *   An associative array containing the current state of the form.
   *
   * @return array
   *   The form structure.
   */
  public function buildForm(array &$form, array &$form_state) {
    $form_element_factory = FormElementFactory::init();

    $global_key = check_plain($form_state['build_info']['args'][1]);
    $global = $this->getGlobal($global_key);

    $form = array();

    $default_element = $global->getFormElement();
    if (is_array($default_element)) {
      $element_type = $default_element['#type'];
    }
    else {
      $element_type = $default_element;
    }

    if (empty($default_element)) {
      $form['message'] = array(
        '#markup' => t('This property is not editable'),
      );
    }

    $form['info'] = array(
      '#markup' => '<strong>' . t('Default value') . ':</strong> ' . $global->getDefaultValue(),
    );

    $behaviour_args = array(
      '#options' => array(
        'form' => t('As entered'),
        'default' => t('Reset to default'),
        'empty' => t('No value (blank)'),
      ),
    );
    $form['behavior'] = $form_element_factory->getFormElement('select', t('Behaviour'), 'form', $behaviour_args);

    $form['element'] = $form_element_factory->getFormElement($element_type, $global->getName(), $global->getValue(), $global->getFormElement());

    $form['global'] = array(
      '#type' => 'hidden',
      '#value' => $global_key,
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }

  /**
   * Form validation handler.
   *
   * @param array $form
   *   An associative array containing the structure of the form.
   * @param array $form_state
   *   An associative array containing the current state of the form.
   */
  public function validateForm(array &$form, array &$form_state) {

    // Nothing to do.
  }

  /**
   * Form submission handler.
   *
   * @param array $form
   *   An associative array containing the structure of the form.
   * @param array $form_state
   *   An associative array containing the current state of the form.
   */
  public function submitForm(array &$form, array &$form_state) {
    if (isset($form_state['values']['global'])) {
      $global_key = $form_state['values']['global'];
      $global = Globals::init()->getGlobalProperty($global_key);

      $value_to_set = '';
      switch ($form_state['values']['behavior']) {
        case 'form':
          $value_to_set = $form_state['values']['element'];
          break;

        case 'default':
          $value_to_set = $global->getDefaultValue();
          break;

        case 'empty':
          $value_to_set = '';
          break;
      }

      $global->setValue($value_to_set);
      drupal_set_message(format_string('The global setting %name was saved', array('%name' => $global->getName())));
    }

    drupal_goto('/admin/structure/globals');
  }

  /**
   * Get settings for a global.
   *
   * @param string $global_key
   *   Key to use
   *
   * @return GlobalItem
   *   The settings
   */
  protected function getGlobal($global_key) {

    try {
      return Globals::init()
        ->getGlobalProperty($global_key);
    }
    catch (GhostException $e) {
      Logger::init('globals', $e->getMessage(), array(), WATCHDOG_ERROR);
      drupal_not_found();
    }
  }

}

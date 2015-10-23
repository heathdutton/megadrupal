<?php
/**
 * @file
 * Provides integration with Rules module.
 *
 * @author Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Implements hook_rules_data_info().
 */
function boolean_rules_data_info() {
  return array(
   'number_boolean' => array(
      'label' => t('Boolean (3-states)'),
      'ui class' => 'RulesDataUINumberBoolean',
      'wrapper class' => 'EntityMetadataWrapperNumberBoolean',
      'wrap' => TRUE,
    ),
  );
}

/**
 * UI for boolean data.
 */
class RulesDataUINumberBoolean extends RulesDataUI implements RulesDataDirectInputFormInterface {

  public static function getDefaultMode() {
    return 'input';
  }

  public static function inputForm($name, $info, $settings, RulesPlugin $element) {
    $settings += array($name => isset($info['default value']) ? $info['default value'] : NULL);
    $form[$name] = array(
      '#type' => 'select',
      '#options' => boolean_value_info(),
      '#default_value' => $settings[$name],
    );
    return $form;
  }

  public static function render($value) {
    $states = boolean_value_info();
    return array(
      'content' => array('#markup' => $states[$value]),
      '#attributes' => array('class' => array('rules-parameter-number-boolean')),
    );
  }
}

/**
 * An entity wrapper class for boolean data.
 */
class EntityMetadataWrapperNumberBoolean extends EntityMetadataWrapper {

  /**
   * Returns whether $value is a valid value to set.
   */
  public function validate($value) {
    $states = boolean_value_info();
    return is_numeric($value) && in_array($value, array_keys($states));
  }
}

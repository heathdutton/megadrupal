<?php
/**
 * @file
 *  Feeds support for the "Mixed Fraction" module.
 *
 *  © 2014-2015 RedBottle Design, LLC and Five Star Tool Company.
 *  All rights reserved. This code is licensed under the GNU GPL v2 or later.
 */

/***************************************************************
 * Hook implementations
 ***************************************************************/
/**
 * Implements hook_feeds_processor_targets_alter().
 *
 * @see FeedsProcessor::getMappingTargets()
 */
function fraction_mixed_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {
  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);

    if ($info['type'] == FRMI_FIELD_TYPE_FRACTION) {
      $targets[$name] = array(
        'name'              => check_plain($instance['label']),
        'description'       => t('The @label field of the entity.', array('@label' => $instance['label'])),
        'callback'          => 'fraction_mixed_feeds_set_target',
        'summary_callback'  => 'fraction_mixed_feeds_summary_callback',
        'form_callback'     => 'fraction_mixed_feeds_form_callback',
      );
    }
  }
}

/***************************************************************
 * Feeds callback functions
 ***************************************************************/
/**
 * Callback for mapping mixed fraction fields.
 */
function fraction_mixed_feeds_set_target($source, $entity, $target, $value, $mapping = array()) {
  if (empty($value)) {
    return;
  }

  elseif (!is_array($value)) {
    $value = array($value);
  }

  $info = field_info_field($target);

  // Iterate over all values.
  $field = isset($entity->$target) ? $entity->$target : array(LANGUAGE_NONE => array());

  // Allow for multiple mappings to the same target.
  $delta = count($field[LANGUAGE_NONE]);

  foreach ($value as $delta_value) {
    if ($info['cardinality'] == $delta) {
      break;
    }

    if (is_object($delta_value) && ($delta_value instanceof FeedsElement)) {
      $delta_value = $delta_value->getValue();
    }

    if (is_scalar($delta_value)) {
      $fraction = _fraction_mixed_parse_fraction($delta_value, $mapping);

      if (!empty($fraction)) {
        $field[LANGUAGE_NONE][$delta]['numerator']    = (string)$fraction->getNumerator();
        $field[LANGUAGE_NONE][$delta]['denominator']  = (string)$fraction->getDenominator();
        $field[LANGUAGE_NONE][$delta]['fraction']     = $fraction;
      }

      $delta++;
    }
  }

  $entity->$target = $field;
}

/**
 * Mapping configuration summary for Mixed fraction fields.
 *
 * @param   array $mapping
 *                Associative array of the mapping settings.
 *
 * @param   array $target
 *                Array of target settings, as defined by the processor or
 *                hook_feeds_processor_targets_alter().
 *
 * @param   array $form
 *                The whole mapping form.
 *
 * @param   array $form_state
 *                The form state of the mapping form.
 *
 * @return        string
 *                Returns, as a string that may contain HTML, the summary to
 *                display while the full form isn't visible.
 *
 *                If the return value is empty, no summary and no option to
 *                view the form will be displayed.
 */
function fraction_mixed_feeds_summary_callback($mapping, $target, $form, $form_state) {
  return _fraction_mixed_separator_settings_summary($mapping);
}

/**
 * Settings form callback.
 *
 * @return  array
 *          The per mapping configuration form. Once the form is saved,
 *          $mapping will be populated with the form values.
 */
function fraction_mixed_feeds_form_callback($mapping, $target, $form, $form_state) {
  return _fraction_mixed_separator_settings_form($mapping);
}
<?php
/**
 * @file
 * Entity reference behavior handler for autofill module.
 */

/**
 * Class for handling module behavior.
 */
class EntityReferenceAutofillInstanceBehavior extends EntityReference_BehaviorHandler_Abstract {

  /**
   * Generate a settings form for this handler.
   */
  public function settingsForm($field, $instance) {
    // Current settings.
    $settings = $instance['settings']['behaviors']['autofill'];
    $widget = $instance['widget'];

    // Load available fields from instance bundle.
    $entity_type = $instance['entity_type'];
    $bundle_name = $instance['bundle'];
    $bundle_info = field_info_instances($entity_type, $bundle_name);

    // Field target metadata.
    $field_map = field_info_field_map();
    $target_type = $field['settings']['target_type'];

    $target_bundles = isset($field['settings']['handler_settings']['target_bundles']);
    $target_bundles = $target_bundles ? $field['settings']['handler_settings']['target_bundles'] : array();

    // Skip autofill for the reference field.
    unset($bundle_info[$field['field_name']]);

    // Create checkboxes options for available fields.
    $field_options = array();
    foreach ($bundle_info as $field_name => $field_info) {
      if (!empty($field_map[$field_name]['bundles'][$target_type])) {
        $available_bundles = $field_map[$field_name]['bundles'][$target_type];
        // Determine all targeted bundles that use this field.
        $option_bundles = empty($target_bundles) ? $available_bundles : array_intersect($target_bundles, $available_bundles);
        if (!empty($option_bundles)) {
          $field_options[$field_name] = t(
            '@field_label (@field_name): <em>Available in bundle(s) @bundles</em>',
            array(
              '@field_label' => $field_info['label'],
              '@field_name' => $field_name,
              '@bundles' => implode(', ', $option_bundles),
            )
          );
        }
      }
    }

    $form['overwrite'] = array(
      '#type' => 'checkbox',
      '#title' => t('Overwrite existing data'),
      '#description' => t('Select if you want to overwrite fields that already have values. <br/><em><strong>NOTE:</strong> Disabling this is experimental and might not work 100%. If you experience issues with fields being overridden nonetheless, please report what field type and settings this occurs on in the modules issue queue on drupal.org</em>'),
      '#default_value' => isset($settings['overwrite']) ? $settings['overwrite'] : 1,
    );

    if (empty($field_options)) {
      $no_fields_found = t('There are no common fields between this bundle and its referenced entities.');
      $usage_instructions = t('To use autofill, you need to add instances of the same fields to its referenced bundles.');

      $form['no_fields'] = array(
        '#markup' => '<div class="messages warning">' . $no_fields_found . $usage_instructions . '</div>',
        '#type' => 'item',
      );
    }
    else {
      $form['fields'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Fields'),
        '#options' => $field_options,
        '#description' => t('Select which fields from the referenced entity you want to load on changing the value of this field.'),
        '#default_value' => isset($settings['fields']) ? $settings['fields'] : array(),
      );
    }

    return $form;
  }

  /**
   * Only show handler on supported widget types.
   */
  public function access($field, $instance) {
    $is_single_value = $field['cardinality'] == 1;
    $is_supported = $is_single_value && array_key_exists($instance['widget']['type'], _entityreference_autofill_supported_widgets());
    return $is_supported;
  }

}

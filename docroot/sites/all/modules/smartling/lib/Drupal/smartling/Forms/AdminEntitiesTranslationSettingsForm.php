<?php

namespace Drupal\smartling\Forms;

class AdminEntitiesTranslationSettingsForm implements FormInterface {

  protected $settings;
  protected $logger;
  protected $fieldProcessorFactory;

  public function __construct($settings, $logger, $field_processor_factory) {
    $this->settings = $settings;
    $this->logger = $logger;
    $this->fieldProcessorFactory = $field_processor_factory;
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'smartling_admin_entities_translation_settings_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, array &$form_state) {
    $entity_rows = $this->entities_translation_crawler();

    $header = array(t('Entity Type'), t('Fields'));

    $variables = array(
      'header' => $header,
      'rows' => $entity_rows,
      'attributes' => array('class' => array('smartling-content-settings-table')),
    );

    $form['entity_translation']['types'] = array(
      '#type' => 'markup',
      '#markup' => theme('table', $variables),
    );

    $form['entity_translation']['actions'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );

    $form['#submit'][] = 'smartling_admin_entities_translation_settings_form_submit';


    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateForm(array &$form, array &$form_state) {

  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, array &$form_state) {
    $input = $form_state['input'];
    $entities = entity_get_info();
    $translate = array();

    // Obtain what fields instances are marked for translation.
    foreach ($input as $key => $value) {
      // Skip if not set to translate.
      if ($value == 0) {
        continue;
      }

      // Look for Selected Content Types and Fields.
      if (FALSE !== strpos($key, '_SEPARATOR_')) {
        // And only if set to translate.
        $parts = explode('_SEPARATOR_', $key);
        $content_type = $parts[0];
        $content_field = $parts[1];

        $translate[$content_type][$content_field] = $content_field;

        $field = field_info_field($content_field);
        $field['translatable'] = 1;
        field_update_field($field);
      }

      if (FALSE !== strpos($key, '_swap_')) {
        // And only if set to swap.

        $swap_chunks = explode('_swap_', $key);
        $legacy_property = $swap_chunks[0];
        $bundle = $swap_chunks[1];
        $entity_type = $swap_chunks[2];
        $resulting_field = $legacy_property . '_field';

        // Use the Title module to migrate the content.
        if (title_field_replacement_toggle($entity_type, $bundle, $legacy_property)) {
          $operations[] = array(
            'title_field_replacement_batch',
            array($entity_type, $bundle, $legacy_property)
          );
          // Add in config.
          $translate[$bundle][$resulting_field] = $resulting_field;

          $field = field_info_field($resulting_field);
          $field['translatable'] = 1;
          $operations[] = array('field_update_field', array($field));
        }
      }
    }

    // Define parent bundle for those fields.
    $_translate = array();
    $machine_names = array_keys($translate);
    foreach ($entities as $entity => $definition) {
      $bundles = array_keys($definition['bundles']);

      foreach ($machine_names as $name) {
        if (in_array($name, $bundles)) {
          $_translate[$entity][$name] = $translate[$name];
        }
      }
    }

    // Reset default state, since all disabled checkboxes will be ignored
    // and therefore, not updated.
    //smartling_settings_get_handler()->resetAllFieldsSettings();

    // Save the settings, considering entity bundle, to know which
    // update method to be called.
    foreach ($_translate as $k => $v) {
      if (in_array($k, array(
        'user',
        'comment',
        'field_collection_item',
        'fieldable_panels_pane'
      ))) {
        $this->settings->setFieldsSettings($k, $v);
      }
    }

    drupal_set_message(t('Your entities settings have been updated.'));

    $this->logger->info('Smartling entities and fields have been updated.', array(), TRUE);

    $redirect = url('admin/config/regional/smartling', array(
      'absolute' => TRUE,
      'fragment' => 'smartling-entities-settings',
    ));

    if (!empty($operations)) {
      $batch = array(
        'title' => t('Preparing content'),
        'operations' => $operations,
      );

      batch_set($batch);
      batch_process($redirect);
    }
    else {
      $form_state['redirect'] = $redirect;
    }
  }


  /**
   * Aggregate various types of entities and their fields.
   *
   * @return array
   *   Set of rows ready to be placed into table form element.
   *   First element is the entity name, second - checkboxes for each
   *   translatable fields for this entity.
   */
  protected function entities_translation_crawler() {
    // Entities we do not want to parse.
    $exclude = array(
      'node',
      'taxonomy_term',
      'taxonomy_vocabulary',
      'i18n_translation',
      'smartling_interface_entity',
      'file',
      'smartling_entity_data',
    );

    $entities = entity_get_info();
    $rows = array();

    // Loop through each entity type.
    foreach ($entities as $entity_type => $definition) {
      if (in_array($entity_type, $exclude)) {
        continue;
      }

      $entity_info = entity_get_info($entity_type);
      $title_fields_info = @$entity_info['field replacement'];
      $field_replacement = (!empty($title_fields_info)) ? key($entity_info['field replacement']) : NULL;

      $translate_fields = $this->settings->getFieldsSettings($entity_type);

      $bundles = array_keys($definition['bundles']);
      // Loop through each entity bundle.
      foreach ($bundles as $bundle) {
        $field_instances = field_info_instances($entity_type, $bundle);

        // Loop through each field instance of the bundle.
        foreach ($field_instances as $field) {
          $field_label = $field['label'];
          $field_machine_name = $field['field_name'];

          if ($this->fieldProcessorFactory->isSupportedField($field_machine_name)) {
            $key = $bundle . '_SEPARATOR_' . $field_machine_name;
            $form_fields[$key] = array(
              '#type' => 'checkbox',
              '#title' => check_plain($field_label),
              '#attributes' => array(
                'id' => array('edit-form-item-' . $bundle . '-separator-' . $field_machine_name),
                'name' => $bundle . '_SEPARATOR_' . $field_machine_name,
                'class' => array('field'),
              ),
              '#id' => 'edit-form-item-' . $bundle . '-separator-' . $field_machine_name,
            );

            if (!empty($translate_fields) && isset($translate_fields[$bundle][$field_machine_name])) {
              $form_fields[$key]['#attributes']['checked'] = 'checked';
            }
          }
        }


        if (!empty($field_replacement)) {
          $entity_key = $bundle . '_SEPARATOR_' . $field_replacement . '_field';
          if (!isset($form_fields[$entity_key])) {
            $form_fields[$entity_key] = array(
              '#type' => 'checkbox',
              '#title' => ucfirst($field_replacement) . ' ' . t('(Note: field will be created)'),
              '#attributes' => array(
                'id' => array('edit-form-item-' . $bundle . '-separator-' . $field_replacement),
                'name' => $field_replacement . '_swap_' . $bundle . '_swap_' . $entity_type,
                'class' => array('field'),
              ),
            );

            if (!empty($translate_fields) && isset($translate_fields[$bundle][$field_replacement . '_field'])) {
              $form_fields[$entity_key]['#attributes']['checked'] = 'checked';
            }
          }
        }

        $rows[] = array($bundle, drupal_render($form_fields));
        $form_fields = NULL;
      }
    }

    return $rows;
  }
}
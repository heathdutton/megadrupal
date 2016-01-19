<?php

namespace Drupal\smartling\Forms;

class AdminNodeTranslationSettingsForm implements FormInterface {

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
    return 'smartling_admin_node_translation_settings_form';
  }

  protected function getTranslationTypeElem($bundle) {
    $form_transl_type = array();
    if ($this->supportedType($bundle)) {
      $form_transl_type['from'] = array(
        '#type' => 'item',
        '#title' => (smartling_nodes_method($bundle)) ? t('Nodes method') : t('Fields method'),
      );
    }
    else {
      $options = array(
        0 => t('- Select Method -'),
        2 => t('Nodes method'),
        1 => t('Fields method'),
      );

      $form_transl_type['method'][$bundle] = array(
        '#type' => 'select',
        '#title' => t('Translation Method'),
        '#title_display' => 'invisible',
        '#options' => $options,
        '#required' => FALSE,
        '#default_value' => 0,
        '#attributes' => array(
          'id' => array('edit-form-item-' . $bundle . '-TT-' . $bundle),
          'name' => $bundle . '_TT_' . $bundle,
          'class' => array('content-type'),
        ),
      );
    }

    return $form_transl_type;
  }

  protected function getTranslatableFieldsElem($bundle) {
    $form_fields = array();
    // What types of fields DO we translate?
    $node_translate_fields = $this->settings->nodeGetFieldsSettings();

    if ($this->supportedType($bundle)) {
      $is_title_replaced = FALSE;
      $fields_list = field_info_instances('node', $bundle);
      if (!isset($fields_list['title_field']) && smartling_fields_method($bundle)) {
        $fields_list['title_property'] = array(
          'label' => t('Title (Note: field will be created)'),
          'field_name' => 'title_field'
        );
        $is_title_replaced = TRUE;
      }

      if (!isset($fields_list['title_field']) && smartling_nodes_method($bundle)) {
        $fields_list['title_property_field'] = array(
          'label' => t('Title'),
          'field_name' => 'title_property_field'
        );
        $is_title_replaced = TRUE;
      }

      foreach ($fields_list as $field_name => $field) {
        $field_label = $field['label'];
        $field_machine_name = $field['field_name'];

        if ($this->fieldProcessorFactory->isSupportedField($field_name)) {
          $form_fields[$field_machine_name] = array(
            '#type' => 'checkbox',
            '#title' => check_plain($field_label),
            '#attributes' => array(
              'id' => array('edit-form-item-' . $bundle . '-separator-' . $field_machine_name),
              'name' => $bundle . '_SEPARATOR_' . $field_machine_name,
              'class' => array('field'),
            ),
            '#id' => 'edit-form-item-' . $bundle . '-separator-' . $field_machine_name,
          );

          $is_in_conf = (!empty($node_translate_fields) && isset($node_translate_fields[$bundle][$field_machine_name])) ? TRUE : FALSE;

          if ($is_in_conf) {
            $form_fields[$field_machine_name]['#attributes']['checked'] = 'checked';
          }
        }
      }

      if ($is_title_replaced && isset($form_fields['title_field']) && smartling_fields_method($bundle)) {
        $form_fields['title_field']['#attributes']['name'] = 'title_swap_' . $bundle;
      }
    }
    return $form_fields;
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, array &$form_state) {
    $form['node_translation'] = array(
      'actions' => array(
        '#type' => 'actions',
      ),
    );

    $form['node_translation']['title'] = array(
      '#type' => 'item',
      '#title' => t('Which content types do you want to translate?'),
    );

    $rows = array();
    $raw_types = node_type_get_types();
    foreach ($raw_types as $value) {
      $bundle = $value->type;
      $form_fields = $this->getTranslatableFieldsElem($bundle);
      $form_transl_type = $this->getTranslationTypeElem($bundle);

      $rows[$bundle] = array(
        array(
          'data' => check_plain($value->name),
          'width' => '20%',
        ),
        array(
          'data' => drupal_render($form_transl_type),
          'width' => '20%',
        ),
        array(
          'data' => drupal_render($form_fields),
          'width' => '60%',
        ),
      );
    }

    $header = array(t('Entity Type'), t('Translation Type'), t('Fields'));

    $variables = array(
      'header' => $header,
      'rows' => $rows,
      'attributes' => array('class' => array('smartling-content-settings-table')),
    );

    $form['node_translation']['types'] = array(
      '#type' => 'markup',
      '#markup' => theme('table', $variables),
    );

    $node_translate_fields = $this->settings->nodeGetFieldsSettings();
    foreach (array_keys($node_translate_fields) as $content_type) {
      $form['node_translation']['types']['#default_value'][$content_type] = 1;
    }

    $form['node_translation']['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );

    $form['#submit'][] = 'smartling_admin_node_translation_settings_form_submit';

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
    // This is needed for the setup because of the field/node method selector.
    system_settings_form_submit($form, $form_state);

    $translate = array();
    $operations = array();

    foreach ($form_state['input'] as $key => $value) {
      // Skip if not set to translate.
      if ($value == 0) {
        continue;
      }
      // Look for Selected Content Types and Fields.
      if (FALSE !== strpos($key, '_SEPARATOR_')) {
        $parts = explode('_SEPARATOR_', $key);
        $content_type = $parts[0];
        $content_field = $parts[1];

        $translate[$content_type][$content_field] = $content_field;

        // Set this field to 'translatable'.
        // Update the field via the Field API (Instead of the direct db_update).
        if (smartling_fields_method($content_type)) {
          $field = field_info_field($content_field);
          if (!$field['translatable']) {
            $field['translatable'] = 1;
            field_update_field($field);
          }
        }
      }
      // END:  Selected Content Types and Fields.
      // Look for Selected Translation Type.
      if (FALSE !== strpos($key, '_TT_')) {
        $parts = explode('_TT_', $key);
        $content_type = $parts[0];
        if ($value == 2) {
          variable_set('language_content_type_' . $content_type, SMARTLING_NODES_METHOD_KEY);
        }
        elseif ($value == 1) {
          variable_set('language_content_type_' . $content_type, SMARTLING_FIELDS_METHOD_KEY);
        }
      }

      // Look for any nodes we need to do the Title swap for.
      if (FALSE !== strpos($key, 'title_swap_')) {
        $content_type = substr($key, strlen('title_swap_'));

        // Do the actual title replacement.
        $entity_type = 'node';
        $bundle = $content_type;
        $legacy_field = 'title';

        // Use the Title module to migrate the content.
        if (title_field_replacement_toggle($entity_type, $bundle, $legacy_field)) {
          $operations[] = array(
            'title_field_replacement_batch',
            array(
              $entity_type,
              $bundle,
              $legacy_field,
            ),
          );
          // Add in config.
          $translate[$content_type]['title_field'] = 'title_field';
          $field = field_info_field('title_field');
          $field['translatable'] = 1;
          $operations[] = array('field_update_field', array($field));
        }
      }
    }

    $this->settings->nodeSetFieldsSettings($translate);
    drupal_set_message(t('Your content types have been updated.'));
    $this->logger->info('Smartling content types and fields have been updated.', array(), TRUE);

    $redirect = url('admin/config/regional/smartling', array(
      'absolute' => TRUE,
      'fragment' => 'smartling-nodes-settings',
    ));

    if (count($operations) >= 1) {
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

  protected function supportedType($bundle) {
    return \Drupal\smartling\Processors\NodeProcessor::supportedType($bundle);
//    $transl_method = variable_get('language_content_type_' . $bundle, NULL);
//    return in_array($transl_method, array(SMARTLING_NODES_METHOD_KEY, SMARTLING_FIELDS_METHOD_KEY));
  }
}
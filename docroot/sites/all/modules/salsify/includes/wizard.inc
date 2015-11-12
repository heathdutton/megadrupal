<?php

/**
 * @file
 * includes/wizard.inc
 */

/**
 * Admin interface to set up imports.
 */
class SalsifyMigrateWizard extends MigrateUIWizard {

  /**
   * URL for the Salsify API.
   */
  const BASE_URL = 'https://app.salsify.com/api';

  /**
   * Key for access to the Salsify API.
   *
   * @var string
   */
  protected $apiKey = '';

  /**
   * List of channels, keyed by channel ID.
   *
   * @var array
   */
  protected $channels = array();

  /**
   * Number of seconds before channel runs expire.
   *
   * @var int
   */
  protected $expiration = 0;

  /**
   * The content selection step of the wizard.
   *
   * This controls which content migration steps appear later.
   *
   * @var MigrateUIStep
   */
  protected $contentSelectStep;

  /**
   * The content select step number, used in the page title.
   *
   * @var int
   */
  protected $contentSelectStepNumber;

  /**
   * The last step number, used in the page title.
   *
   * @var int
   */
  protected $lastStepNumber = 2;

  /**
   * Configurations for new product migrations, keyed by channel ID.
   *
   * @var array
   */
  protected $newProducts = array();

  /**
   * Configurations for existing product migrations, keyed by channel ID.
   *
   * @var array
   */
  protected $existingProducts = array();

  /**
   * Configurations for product display migrations, keyed by channel ID.
   *
   * Each value is an array whose keys are entity types and whose values are
   * entity bundle names.
   *
   * @var array
   */
  protected $displays = array();

  /**
   * Configuratons for attribute value migrations, keyed by attribute ID.
   *
   * @var array
   */
  protected $attributeValues = array();

  /**
   * Configuration for digital asset migration.
   *
   * @var array
   */
  protected $digitalAssets;

  /**
   * List of attributes found in this group of channels, keyed by attribute ID.
   *
   * @var array
   */
  protected $attributes = array();

  /**
   * List of migrations being set up by this wizard.
   *
   * Keys are machine names, values are migration names.
   *
   * @var array
   */
  protected $sourceMigrations = array();

  /**
   * List of existing migrations in this migration group.
   *
   * Keys are machine names, values are migration names.
   *
   * @var array
   */
  protected $groupMigrations = array();

  /**
   * List of migration dependencies.
   *
   * Keys are migration names, values are arrays of migration names that depend
   * on the migration named in the key.
   *
   * @var array
   */
  protected $allDependencies = array();

  /**
   * List of product type names keyed by machine name.
   *
   * @var array
   */
  protected $productTypeOptions = array();

  /**
   * Map of product display entity type bundles.
   *
   * @var array
   */
  protected $productDisplayMap = array();

  /**
   * List of vocabulary names keyed by machine name.
   *
   * @var array
   */
  protected $vocabularyOptions = array();

  /**
   * Variations used in product migrations.
   *
   * Variation options are keyed by channel ID.
   *
   * @var array
   */
  protected $productVariations = array();

  /**
   * Vocabularies used in attribute value migrations.
   *
   * Vocabulary machine names are keyed by attribute ID.
   *
   * @var array
   */
  protected $attributeVocabularies = array();

  /**
   * {@inheritdoc}
   */
  public function __construct() {
    parent::__construct();
    // Load product type options.
    $this->productTypeOptions = commerce_product_type_get_name();
    // Load product display node type options.
    foreach (field_info_field_map() as $field) {
      if ($field['type'] == 'commerce_product_reference') {
        foreach ($field['bundles'] as $entity_type => $bundles) {
          if (substr($entity_type, 0, 9) != 'commerce_') {
            $entity_info = entity_get_info($entity_type);
            foreach ($bundles as $bundle) {
              if (!isset($this->productDisplayMap[$entity_type][$bundle]) && isset($entity_info['bundles'][$bundle])) {
                $this->productDisplayMap[$entity_type][$bundle] = $entity_info['bundles'][$bundle]['label'];
              }
            }
          }
        }
      }
    }
    // Load vocabulary options.
    foreach (taxonomy_get_vocabularies() as $vocabulary) {
      $this->vocabularyOptions[$vocabulary->machine_name] = $vocabulary->name;
    }
    $this->addStep(t('Credentials'), 'sourceDataForm');
    $this->addStep(t('Select content'), 'contentSelectForm');
  }

  /**
   * {@inheritdoc}
   */
  public function getSourceName() {
    return t('Salsify');
  }

  /**
   * {@inheritdoc}
   */
  public function form(&$form_state) {
    if ($this->currentStep == $this->contentSelectStep) {
      // Keep the step number of the content selection step when skipping
      // intermediate steps.
      $this->stepNumber = $this->contentSelectStepNumber;
    }
    elseif ($this->currentStep == $this->lastStep) {
      // Keep the step number of the last step when skipping intermediate steps.
      $this->stepNumber = $this->lastStepNumber;
    }
    else {
      $context = $this->currentStep->getContext();
      if (is_array($context)) {
        $import = drupal_array_get_nested_value($this->{array_shift($context)}, $context, $key_exists);
        if ($key_exists && isset($import['step_number'])) {
          // Keep the step numbers, even when we don't visit them in order.
          $this->stepNumber = $import['step_number'];
        }
      }
    }
    return parent::form($form_state);
  }

  /**
   * First step: find out how to get the source data.
   *
   * @see SalsifyMigrateWizard::sourceDataFormValidate()
   */
  public function sourceDataForm(&$form_state) {
    migrate_migrations();
    $options = array();
    foreach (MigrateGroup::groups() as $name => $group) {
      $options[$name] = $group->getTitle();
    }
    if (!empty($options)) {
      $form['group'] = array(
        '#type' => 'select',
        '#title' => t('Migration group'),
        '#description' => t('Choose an existing group to add migrations to, or create a new group.'),
        '#options' => $options,
        '#default_value' => isset($options[$this->groupName]) ? $this->groupName : '',
        '#empty_option' => t('- Create new group -'),
      );
    }
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Group title'),
      '#description' => t('Enter name for new migration group.'),
      '#default_value' => $this->groupTitle,
      '#required' => empty($options),
    );
    $form['machine_name'] = array(
      '#type' => 'machine_name',
      '#title' => t('Group name'),
      '#default_value' => $this->groupName == 'default' ? '' : $this->groupName,
      '#machine_name' => array(
        'exists' => 'salsify_migrate_group_exists',
        'label' => t('Group name'),
      ),
      '#required' => empty($options),
    );
    $form['api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('API key'),
      '#description' => t('You can retrieve your API key from your Salsify profile.'),
      '#default_value' => $this->apiKey,
      '#required' => empty($options),
    );
    $form['channel_ids'] = array(
      '#type' => 'textfield',
      '#title' => t('Channel IDs'),
      '#description' => t('Enter a comma separated list of channel IDs. Define channels in the Salsify app. The channel ID is the number at the of the URL of each channel page.'),
      '#default_value' => implode(', ', array_keys($this->channels)),
      '#required' => empty($options),
    );
    $form['expiration'] = array(
      '#type' => 'select',
      '#title' => t('Expiration time'),
      '#description' => t('Number of hours to wait before checking channels for new data.'),
      '#options' => array(
        3600 => t('1 hour'),
        10800 => t('3 hours'),
        28800 => t('8 hours'),
        86400 => t('24 hours'),
      ),
      '#default_value' => $this->expiration,
      '#empty_value' => 0,
      '#required' => empty($options),
    );
    if (!empty($options)) {
      $form['name']['#states'] = $form['machine_name']['#states'] = array(
        'required' => array(':input[name="group"]' => array('value' => '')),
        'enabled' => array(':input[name="group"]' => array('value' => '')),
      );
      $form['api_key']['#states'] = $form['channel_ids']['#states'] = $form['expiration']['#states'] = array(
        'required' => array(':input[name="group"]' => array('value' => '')),
      );
    }
    if (isset($form_state['salsify_batch_errors'])) {
      // Flag errors found during batch processing.
      drupal_static('form_set_error', $form_state['salsify_batch_errors']);
      foreach (array_keys($form_state['salsify_batch_errors']) as $parents) {
        $parents = explode('][', $parents);
        $parents[] = '#validated';
        drupal_array_set_nested_value($form, $parents, TRUE);
      }
    }
    return $form;
  }

  /**
   * Create channel runs.
   *
   * @see SalsifyMigrateWizard::sourceDataForm()
   */
  protected function sourceDataFormValidate(&$form_state) {
    $channel_ids = array_filter(array_map('trim', explode(',', $form_state['values']['channel_ids'])));
    // Check for channels that need to be removed.
    foreach (array_diff(array_keys($this->channels), $channel_ids) as $channel_id) {
      unset($this->channels[$channel_id]);
    }
    $this->apiKey = trim($form_state['values']['api_key']);
    $this->expiration = (int) $form_state['values']['expiration'];
    if (empty($form_state['values']['group'])) {
      // Make sure all required fields are filled in.
      $this->groupTitle = trim($form_state['values']['name']);
      $this->groupName = trim($form_state['values']['machine_name']);
    }
    else {
      // Get settings from existing group.
      $this->groupName = $form_state['values']['group'];
      $group = MigrateGroup::getInstance($this->groupName);
      $this->groupTitle = $group->getTitle();
      $this->groupMigrations = array();
      foreach (migrate_migrations() as $migration) {
        if ($this->groupName == $migration->getGroup()->getName()) {
          $machine_name = $migration->getMachineName();
          $group_length = strlen($this->groupName);
          if (!strncasecmp($this->groupName, $machine_name, $group_length)) {
            $this->groupMigrations[$machine_name] = substr($machine_name, $group_length);
          }
          else {
            $this->groupMigrations[$machine_name] = $machine_name;
          }
          if ($migration instanceof SalsifyProductsMigration) {
            $arguments = $migration->getArguments();
            foreach ($arguments['channel_ids'] as $channel_id) {
              $this->productVariations[$channel_id] = $arguments['variation'];
            }
          }
          elseif ($migration instanceof SalsifyAttributeValuesMigration) {
            $arguments = $migration->getArguments();
            $this->attributeVocabularies[$arguments['attribute']->{'salsify:id'}] = $arguments['vocabulary'];
          }
        }
      }
      $arguments = $group->getArguments();
      if (isset($arguments['api_key']) && empty($this->apiKey)) {
        $this->apiKey = $arguments['api_key'];
      }
      if (isset($arguments['channels'])) {
        $this->channels += $arguments['channels'];
      }
      if (isset($arguments['expiration']) && empty($this->expiration)) {
        $this->expiration = $arguments['expiration'];
      }
    }
    // If the group field exists, other fields are required only if no group is
    // selected. Otherwise, Drupal validates required fields for us.
    if (isset($form_state['values']['group'])) {
      if (empty($this->groupTitle)) {
        $element = $form_state['complete form']['name'];
        form_error($element, t('@name field is required.', array('@name' => $element['#title'])));
      }
      if (empty($this->groupName)) {
        $element = $form_state['complete form']['machine_name'];
        form_error($element, t('@name field is required.', array('@name' => $element['#title'])));
      }
      if (empty($this->apiKey)) {
        $element = $form_state['complete form']['api_key'];
        form_error($element, t('@name field is required.', array('@name' => $element['#title'])));
      }
      if (empty($this->channels) && empty($channel_ids)) {
        $element = $form_state['complete form']['channel_ids'];
        form_error($element, t('@name field is required.', array('@name' => $element['#title'])));
      }
      if (empty($this->expiration)) {
        $element = $form_state['complete form']['expiration'];
        form_error($element, t('@name field is required.', array('@name' => $element['#title'])));
      }
    }
    // Process new channels.
    $channel_ids = array_diff($channel_ids, array_keys($this->channels));
    if (!empty($channel_ids)) {
      $batch = array(
        'operations' => array(),
        'finished' => 'salsify_channel_batch_finished',
        'file' => drupal_get_path('module', 'salsify') . '/includes/wizard.inc',
      );
      foreach ($channel_ids as $channel_id) {
        if (!isset($this->channels[$channel_id])) {
          $batch['operations'][] = array('salsify_channel_batch_operation', array($channel_id));
        }
      }
      batch_set($batch);
    }
  }

  /**
   * Checks the run status for a channel. This is a batch operation.
   *
   * @see salsify_channel_batch_operation()
   */
  public function channelBatchOperation($channel_id, &$context) {
    if (isset($this->channels[$channel_id])) {
      $run = $this->channels[$channel_id];
      $result = drupal_http_request(url(self::BASE_URL . "/channels/$channel_id/runs/$run[id]", array(
        'query' => array(
          'auth_token' => $this->apiKey,
        ),
      )));
    }
    elseif ($cache = cache_get("salsify:channel_run:$channel_id")) {
      $channel_run = $cache->data;
      $result = drupal_http_request(url(self::BASE_URL . "/channels/$channel_id/runs/$channel_run", array(
        'query' => array(
          'auth_token' => $this->apiKey,
        ),
      )));
    }
    else {
      // To create a channel run programmatically, HTTP POST an empty JSON
      // document to: https://app.salsify.com/api/channels/CHANNEL_ID/runs
      // where you would replace CHANNEL_ID with the ID you got from the
      // Salsify app. This call will return a JSON document that contains a
      // channel run ID along with addition information about the channel run.
      // You'll need this channel run ID to poll for status.
      $result = drupal_http_request(url(SalsifyMigrateWizard::BASE_URL . "/channels/$channel_id/runs", array(
        'query' => array(
          'auth_token' => $this->apiKey,
        ),
      )), array(
        'method' => 'POST',
        'data' => drupal_json_encode(''),
      ));
    }
    if (isset($result->headers['content-type']) && trim(reset(explode(';', $result->headers['content-type'], 2))) == 'application/json') {
      $run = drupal_json_decode($result->data);
      if (isset($run['error'])) {
        $context['results']['api_key'] = check_markup($run['error']);
      }
      else {
        if (isset($run['id'])) {
          if (!isset($this->channels[$channel_id]) && $cache === FALSE) {
            cache_set("salsify:channel_run:$channel_id", $run['id'], 'cache', REQUEST_TIME + $this->expiration);
          }
          $this->channels[$channel_id] = $run;
        }
        if ($run['status'] != 'completed') {
          $context['finished'] = 0;
        }
      }
    }
    else {
      $context['results']['channel_ids'][] = $channel_id;
    }
  }

  /**
   * Select what to import.
   *
   * @see SalsifyMigrateWizard::contentSelectFormValidate()
   */
  protected function contentSelectForm(&$form_state) {
    $source_fields = array(
      'products' => array(),
      'attribute_values' => array(),
      'digital_assets' => array(),
      'relations' => array(),
      'bundled_products' => array(),
      'attributes' => array(),
    );
    // Load default product type.
    $product_type = isset($this->productTypeOptions['product']) ? 'product' : '';
    $relation_type = array();
    $attribute_values = array();
    ksort($this->channels);
    // There is a product migration for each channel.
    foreach ($this->channels as $channel_id => &$run) {
      $source_fields['products'][$channel_id] = array();
      $source_fields['attribute_values'][$channel_id] = array();
      $source_fields['digital_assets'][$channel_id] = array();
      $source_fields['relations'][$channel_id] = array(
        'salsify:id' => t('Unique ID within the scope of all related relationships'),
        'salsify:target_product_id' => t('ID of related product'),
      );
      $source_fields['bundled_products'][$channel_id] = array(
        'salsify:id' => t('Unique ID within the scope of all bundle relationships'),
        'salsify:bundled_product_id' => t('ID of a bundled product'),
      );
      $source_fields['attributes'][$channel_id] = array(
        'salsify:id' => t('Unique ID within the set of all attributes'),
        'salsify:name' => t('Brief name for the attribute'),
        'salsify:data_type' => t('Data type of attribute values'),
        'salsify:role' => t('Role this attribute plays in various contexts'),
      );
      // Refresh the run in case the URL has expired.
      $result = drupal_http_request(url(self::BASE_URL . "/channels/$channel_id/runs/" . $run['id'], array(
        'query' => array(
          'auth_token' => $this->apiKey,
        ),
      )));
      $run = drupal_json_decode($result->data);
      // Load attributes from this channel.
      $reader = new SalsifyAttributesJSONReader($run['product_export_url'], 'salsify:id');
      foreach ($reader as $attribute) {
        $this->attributes[$attribute->{'salsify:id'}] = $attribute;
        // Allow enumerated attributes other than relation type to be mapped
        // to vocabularies.
        if ($attribute->{'salsify:data_type'} == 'enumerated' && !isset($attribute->{'salsify:role'})) {
          $attribute_values[$attribute->{'salsify:id'}][] = $channel_id;
        }
        // Save special attributes.
        if (isset($attribute->{'salsify:role'})) {
          switch ($attribute->{'salsify:role'}) {
            case 'product_id':
            case 'product_name':
              ${$attribute->{'salsify:role'}} = $attribute->{'salsify:id'};
              break;

            case 'relation_type':
              // Relation type will be used to select the default field to map
              // the relation to. It may also be used to identify product
              // variations.
              $relation_type[$channel_id] = $attribute;
              // For some reason, the relation type attribute does not list
              // relations as its entity type.
              if (!in_array('relations', $attribute->{'salsify:entity_types'})) {
                $source_fields['relations'][$channel_id][$attribute->{'salsify:id'}] = $attribute->{'salsify:name'};
              }
              break;
          }
        }
        // Save attributes by channel and Salsify entity type. This ensures
        // that we can add attributes only where they are actually used.
        foreach ($attribute->{'salsify:entity_types'} as $entity_type) {
          $source_fields[$entity_type][$channel_id][$attribute->{'salsify:id'}] = $attribute->{'salsify:name'};
        }
      }
      if (!isset($this->groupMigrations[$this->groupName . "NewProductsChannel$channel_id"]) || !isset($this->groupMigrations[$this->groupName . "ExistingProductsChannel$channel_id"])) {
        $form['products'][$channel_id]['product_type'] = array(
          '#type' => 'select',
          '#title' => t('Channel %channel_id', array('%channel_id' => $channel_id)),
          '#options' => $this->productTypeOptions,
          '#empty_value' => '',
        );
        if (isset($this->newProducts[$channel_id])) {
          $form['products'][$channel_id]['product_type']['#default_value'] = $this->newProducts[$channel_id]['product_type'];
        }
        elseif (isset($this->existingProducts[$channel_id])) {
          $form['products'][$channel_id]['product_type']['#default_value'] = $this->existingProducts[$channel_id]['product_type'];
        }
        else {
          $form['products'][$channel_id]['product_type']['#default_value'] = $product_type;
        }
        $form['products'][$channel_id]['product_id'] = array(
          '#type' => 'value',
          '#value' => isset($product_id) ? $product_id : 'id',
        );
        $form['products'][$channel_id]['product_name'] = array(
          '#type' => 'value',
          '#value' => isset($product_name) ? $product_name : 'name',
        );
        $form['products'][$channel_id]['source_fields'] = array(
          '#type' => 'value',
          '#value' => SalsifyProductsMigration::salsifyAttributes() + $source_fields['products'][$channel_id],
        );
        // Add bundled products attributes.
        foreach ($source_fields['bundled_products'][$channel_id] as $name => $desription) {
          $form['products'][$channel_id]['source_fields']['#value']["salsify:bundled_products::$name"] = t('Bundled products: @description', array('@description' => $desription));
        }
        // Add relations attributes.
        foreach ($source_fields['relations'][$channel_id] as $name => $description) {
          $form['products'][$channel_id]['source_fields']['#value']["salsify:relations::$name"] = t('Relations: @description', array('@description' => $description));
        }
        // Add digital asset attributes.
        foreach (SalsifyDigitalAssetsMigration::salsifyAttributes() + $source_fields['digital_assets'][$channel_id] as $name => $description) {
          $form['products'][$channel_id]['source_fields']['#value']["salsify:digital_assets::$name"] = t('Digital assets: @description', array('@description' => $description));
        }
      }
      // We may need to create product displays for each channel.
      $states = array();
      foreach ($this->productDisplayMap as $entity_type => $bundles) {
        $entity_info = entity_get_info($entity_type);
        if (isset($this->groupMigrations[$this->groupName . "$entity_info[label]DisplaysChannel$channel_id"])) {
          if (isset($form['displays'][$channel_id])) {
            unset($form['displays'][$channel_id]);
          }
          break;
        }
        else {
          $bundle = $entity_type == 'node' && isset($bundles['product_display']) ? 'product_display' : '';
          $form['displays'][$channel_id][$entity_type]['bundle'] = array(
            '#type' => 'select',
            '#title' => t('@entity type', array('@entity' => $entity_info['label'])),
            '#options' => $bundles,
            '#empty_value' => '',
            '#default_value' => isset($this->displays[$channel_id][$entity_type]['bundle']) ? $this->displays[$channel_id][$entity_type]['bundle'] : $bundle,
          );
          $form['displays'][$channel_id][$entity_type]['product_id'] = array(
            '#type' => 'value',
            '#value' => isset($product_id) ? $product_id : 'id',
          );
          $form['displays'][$channel_id][$entity_type]['product_name'] = array(
            '#type' => 'value',
            '#value' => isset($product_name) ? $product_name : 'name',
          );
          $form['displays'][$channel_id][$entity_type]['source_fields'] = array(
            '#type' => 'value',
            '#value' => SalsifyProductsMigration::salsifyAttributes() + $source_fields['products'][$channel_id],
          );
          // Add relations attributes.
          foreach ($source_fields['relations'][$channel_id] as $name => $description) {
            $form['displays'][$channel_id][$entity_type]['source_fields']['#value']["salsify:relations::$name"] = t('Relations: @description', array('@description' => $description));
          }
          // Add digital asset attributes.
          foreach (SalsifyDigitalAssetsMigration::salsifyAttributes() + $source_fields['digital_assets'][$channel_id] as $name => $description) {
            $form['displays'][$channel_id][$entity_type]['source_fields']['#value']["salsify:digital_assets::$name"] = t('Digital assets: @description', array('@description' => $description));
          }
        }
        $states['enabled'][':input[name="displays[' . "$channel_id][$entity_type" . '][bundle]"]'] = array('value' => '');
      }
      if (isset($form['displays'][$channel_id])) {
        $form['displays'][$channel_id] += array(
          '#type' => 'fieldset',
          '#title' => t('Channel %channel_id', array('%channel_id' => $channel_id)),
        );
        // Ensure that only one display entity is chosen.
        foreach (element_children($form['displays'][$channel_id]) as $entity_type) {
          $entity_state = $states['enabled'][':input[name="displays[' . "$channel_id][$entity_type" . '][bundle]"]'];
          unset($states['enabled'][':input[name="displays[' . "$channel_id][$entity_type" . '][bundle]"]']);
          $form['displays'][$channel_id][$entity_type]['bundle']['#states'] = $states;
          $states['enabled'][':input[name="displays[' . "$channel_id][$entity_type" . '][bundle]"]'] = $entity_state;
        }
      }
    }
    if (isset($form['products'])) {
      $form['products'] += array(
        '#type' => 'fieldset',
        '#title' => t('Product types'),
        '#description' => t('Select the product type for each channel.'),
      );
    }
    if (isset($form['displays'])) {
      $form['displays'] += array(
        '#type' => 'fieldset',
        '#title' => t('Product displays'),
        '#description' => t('Select product display entities for each channel. You can skip this if you plan to create product displays manually.'),
      );
      foreach (element_children($form['displays']) as $channel_id) {
        if (isset($form['products'][$channel_id])) {
          // Hide displays fieldset if there are no product migrations.
          $form['displays']['#states']['invisible'][':input[name="products[' . $channel_id . '][product_type]"]'] = array('value' => '');
        }
        $form['displays'][$channel_id]['#states'] = array(
          'invisible' => array(
            ':input[name="products[' . $channel_id . '][product_type]"]' => array('value' => ''),
          ),
        );
        // Hide the variations fieldset if no product migrations are selected.
        $form['variations']['#states']['invisible'][':input[name="products[' . $channel_id . '][product_type]"]'] = array('value' => '');
        $form['variations'][$channel_id] = array(
          '#type' => 'fieldset',
          '#title' => t('Channel %channel_id', array('%channel_id' => $channel_id)),
          'invisible' => array(
            ':input[name="products[' . $channel_id . '][product_type]"]' => array('value' => ''),
          ),
        );
        $options = array('fields' => t('Shared fields'));
        // We cannot set the variation type to relation if the product migration
        // already exists.
        if (!isset($this->groupMigrations[$this->groupName . "NewProductsChannel$channel_id"]) && !isset($this->groupMigrations[$this->groupName . "ExistingProductsChannel$channel_id"]) || $this->productVariations[$channel_id]['type'] == 'relation') {
          $options = array('relation' => t('Relations')) + $options;
        }
        $form['variations'][$channel_id]['type'] = array(
          '#type' => 'select',
          '#title' => t('Variation type'),
          '#options' => $options,
          '#empty_value' => '',
          '#default_value' => isset($this->productVariations[$channel_id]['type']) ? $this->productVariations[$channel_id]['type'] : '',
          '#disabled' => (isset($this->groupMigrations[$this->groupName . "NewProductsChannel$channel_id"]) || isset($this->groupMigrations[$this->groupName . "ExistingProductsChannel$channel_id"])) && $this->productVariations[$channel_id]['type'] == 'relation',
        );
        if (isset($relation_type[$channel_id])) {
          $form['variations'][$channel_id]['attribute_id'] = array(
            '#type' => 'value',
            '#value' => $relation_type[$channel_id]->{'salsify:id'},
          );
          $reader = new SalsifyAttributeValuesJSONReader($this->channels[$channel_id]['product_export_url'], $relation_type[$channel_id]->{'salsify:id'});
          $relation_types = array();
          foreach ($reader as $attribute_value) {
            $relation_types[$attribute_value->{'salsify:id'}] = $attribute_value->{'salsify:name'};
          }
          $form['variations'][$channel_id]['attribute_values'] = array(
            '#type' => 'value',
            '#value' => $relation_types,
          );
          if (isset($form['products'][$channel_id]) || (isset($this->groupMigrations[$this->groupName . "NewProductsChannel$channel_id"]) || isset($this->groupMigrations[$this->groupName . "ExistingProductsChannel$channel_id"])) && $this->productVariations[$channel_id]['type'] == 'relation') {
            if ($relation_type[$channel_id]->{'salsify:data_type'} == 'enumerated') {
              if (!empty($relation_types)) {
                // If there are relation options, display relation type
                // selector.
                $form['variations'][$channel_id]['relation_type'] = array(
                  '#type' => 'select',
                  '#title' => t('Relation type'),
                  '#description' => t('You may choose to use a specific relation type to identify product variations. If you do not choose a specific relation type, all relations will be treated as product variations.'),
                  '#options' => $relation_types,
                  '#empty_value' => '',
                  '#default_value' => isset($this->productVariations[$channel_id]['relation_type']) ? $this->productVariations[$channel_id]['relation_type'] : '',
                  '#states' => array(
                    'visible' => array(
                      ':input[name="variations[' . $channel_id . '][type]"]' => array('value' => 'relation'),
                    ),
                  ),
                  '#disabled' => (isset($this->groupMigrations[$this->groupName . "NewProductsChannel$channel_id"]) || isset($this->groupMigrations[$this->groupName . "ExistingProductsChannel$channel_id"])),
                );
              }
            }
            else {
              $form['variations'][$channel_id]['relation_type'] = array(
                '#type' => 'text',
                '#title' => t('Relation type'),
                '#description' => t('You may choose to use a specific relation type to identify product variations. If you do not choose a specific relation type, all relations will be treated as product variations.'),
                '#default_value' => isset($this->productVariations[$channel_id]['relation_type']) ? $this->productVariations[$channel_id]['relation_type'] : '',
                '#states' => array(
                  'visible' => array(
                    ':input[name="variations[' . $channel_id . '][type]"]' => array('value' => 'relation'),
                  ),
                ),
                '#disabled' => (isset($this->groupMigrations[$this->groupName . "NewProductsChannel$channel_id"]) || isset($this->groupMigrations[$this->groupName . "ExistingProductsChannel$channel_id"])),
              );
            }
          }
        }
      }
      if (isset($form['variations'])) {
        $form['variations'] += array(
          '#type' => 'fieldset',
          '#title' => t('Product variations'),
          '#description' => t('Product displays can show multiple variations of a product. There are two ways to identify product variations:
<dl>
  <dt>Relations</dt>
  <dd>The preferred way to identify variations is with relations. Master products will be imported as a display and their relations will be imported as products. This option must be used for both the product and the product display migrations of a channel.</dd>
  <dt>Shared fields</dt>
  <dd>If there is no relation that lists all the variations of a product, variations can be identified by shared field values. Products that have the same values in all the fields enabled in the product display configuration will be treated as variations.</dd>
</dl>
If you choose to create product displays without selecting a method of identifying variations, a display will be created for each product.'),
        );
      }
    }
    // There is an attribute value migration for each enumerated attribute.
    ksort($attribute_values);
    foreach ($attribute_values as $attribute_id => $channel_ids) {
      if (!isset($this->groupMigrations[$this->groupName . "AttributeValues$attribute_id"])) {
        $attribute = $this->attributes[$attribute_id];
        // Load default vocabulary.
        $vocabulary = '';
        foreach ($this->vocabularyOptions as $machine_name => $name) {
          if ($attribute_id == $machine_name || $attribute->{'salsify:name'} == $name) {
            $vocabulary = $machine_name;
            break;
          }
        }
        $form['attribute_values'][$attribute_id]['vocabulary'] = array(
          '#type' => 'select',
          '#title' => t('%name attribute values', array('%name' => $attribute->{'salsify:name'})),
          '#options' => $this->vocabularyOptions,
          '#empty_value' => '',
          '#default_value' => isset($this->attributeValues[$attribute_id]) ? $this->attributeValues[$attribute_id]['vocabulary'] : $vocabulary,
          '#description' => $attribute->{'salsify:help_text'},
        );
        $form['attribute_values'][$attribute_id]['source_fields'] = array(
          '#type' => 'value',
          '#value' => SalsifyAttributeValuesMigration::salsifyAttributes(),
        );
        // Save channels we need to load to import these attribute values.
        $form['attribute_values'][$attribute_id]['channel_ids'] = array(
          '#type' => 'value',
          '#value' => $channel_ids,
        );
        // Add attributes from all channels in which this attribute value is
        // used.
        foreach ($channel_ids as $channel_id) {
          $form['attribute_values'][$attribute_id]['source_fields']['#value'] += $source_fields['attribute_values'][$channel_id];
        }
      }
    }
    if (isset($form['attribute_values'])) {
      $form['attribute_values'] += array(
        '#type' => 'fieldset',
        '#title' => t('Vocabularies'),
        '#description' => t('Select the vocabularies for enumerated attributes. This is needed if an attribute is a hierarchical category, or if the attribute values have attributes of their own. It is also needed if you want to be able to remove terms when you roll back this migration. Otherwise a separate attribute value import can be skipped.'),
      );
    }
    // There is only one digital asset migration.
    if (!isset($this->groupMigrations[$this->groupName . 'DigitalAssets'])) {
      $form['digital_assets']['bundle'] = array(
        '#type' => 'select',
        '#title' => t('Files'),
        '#options' => array(),
        '#empty_value' => '',
        '#default_value' => isset($this->digitalAssets) ? $this->digitalAssets['bundle'] : key(field_info_bundles('file')),
        '#description' => t('Select the file type for digital assets. Performing a separate migration of digital assets is not required, but it ensures that files used by multiple products are not duplicated.'),
      );
      foreach (field_info_bundles('file') as $type => $bundle) {
        $form['digital_assets']['bundle']['#options'][$type] = $bundle['label'];
      }
      $form['digital_assets']['source_fields'] = array(
        '#type' => 'value',
        '#value' => SalsifyDigitalAssetsMigration::salsifyAttributes(),
      );
      foreach ($source_fields['digital_assets'] as $fields) {
        $form['digital_assets']['source_fields']['#value'] += $fields;
      }
    }
    if (isset($form)) {
      $form['#tree'] = TRUE;
      if ($this->currentStep == $this->lastStep) {
        $this->addStep(t('Review'), 'reviewForm');
      }
    }
    else {
      $form[] = array(
        '#markup' => '<p>' . t('No new migrations are available. Please edit existing migrations or add more channels.') . '</p>',
      );
      if ($this->currentStep != $this->lastStep) {
        unset($this->currentStep->nextStep);
        $this->lastStep = $this->currentStep;
        $this->lastStepNumber = $this->stepNumber;
      }
    }
    return $form;
  }

  /**
   * Determine what migrate configuration forms we need to show.
   *
   * @see SalsifyMigrateWizard::contentSelectForm()
   */
  protected function contentSelectFormValidate(&$form_state) {
    // Save the current step so we can control the order of additional steps.
    $current_step = $this->currentStep;
    $step_number = $this->stepNumber;
    $this->contentSelectStep = $current_step;
    $this->contentSelectStepNumber = $step_number;
    // See if there are new product steps that need to be removed.
    foreach ($this->newProducts as $channel_id => $product) {
      if (!isset($form_state['values']['products'][$channel_id])) {
        $step = $product['step'];
        $step->previousStep->nextStep = $step->nextStep;
        $step->nextStep->previousStep = $step->previousStep;
        unset($this->sourceMigrations[$this->groupName . "NewProductsChannel$channel_id"]);
        unset($this->newProducts[$channel_id]);
      }
    }
    // See if there are existing product steps that need to be removed.
    foreach ($this->existingProducts as $channel_id => $product) {
      if (!isset($form_state['values']['products'][$channel_id])) {
        $step = $product['step'];
        $step->previousStep->nextStep = $step->nextStep;
        $step->nextStep->previousStep = $step->previousStep;
        unset($this->sourceMigrations[$this->groupName . "ExistingProductsChannel$channel_id"]);
        unset($this->existingProducts[$channel_id]);
      }
    }
    // Set up product steps.
    if (isset($form_state['values']['products'])) {
      foreach ($form_state['values']['products'] as $channel_id => $product) {
        // Set up new product step.
        if (!isset($this->groupMigrations[$this->groupName . "NewProductsChannel$channel_id"])) {
          $migration_name = "NewProductsChannel$channel_id";
          if (!isset($this->newProducts[$channel_id]) || $this->newProducts[$channel_id]['product_type'] != $product['product_type']) {
            $this->newProducts[$channel_id] = $product;
            unset($this->migrations[$this->groupName . $migration_name]);
            unset($this->sourceMigrations[$this->groupName . $migration_name]);
          }
          if (!empty($product['product_type'])) {
            if (empty($form_state['values']['variations'][$channel_id]['type'])) {
              unset($this->newProducts[$channel_id]['variation']);
            }
            else {
              $this->newProducts[$channel_id]['variation'] = array_filter($form_state['values']['variations'][$channel_id]);
            }
            if (!isset($this->newProducts[$channel_id]['step'])) {
              $this->newProducts[$channel_id]['step'] = $this->addStep(t('Configure new products for channel @id', array(
                '@id' => $channel_id,
              )), 'contentProductForm', $current_step, array('newProducts', $channel_id));
              $this->sourceMigrations[$this->groupName . $migration_name] = $migration_name;
            }
            // Keep steps in order. If a migration for this step already exists,
            // skip it.
            if (!isset($this->migrations[$this->groupName . $migration_name])) {
              $current_step->nextStep = $this->newProducts[$channel_id]['step'];
              $current_step = $this->newProducts[$channel_id]['step'];
            }
            $this->newProducts[$channel_id]['step_number'] = ++$step_number;
          }
        }
        // Set up existing product step.
        if (!isset($this->groupMigrations[$this->groupName . "ExistingProductsChannel$channel_id"])) {
          $migration_name = "ExistingProductsChannel$channel_id";
          if (!isset($this->existingProducts[$channel_id]) || $this->existingProducts[$channel_id]['product_type'] != $product['product_type']) {
            $this->existingProducts[$channel_id] = $product;
            unset($this->migrations[$this->groupName . $migration_name]);
            unset($this->sourceMigrations[$this->groupName . $migration_name]);
          }
          if (!empty($product['product_type'])) {
            if (empty($form_state['values']['variations'][$channel_id]['type'])) {
              unset($this->existingProducts[$channel_id]['variation']);
            }
            else {
              $this->existingProducts[$channel_id]['variation'] = array_filter($form_state['values']['variations'][$channel_id]);
            }
            if (!isset($this->existingProducts[$channel_id]['step'])) {
              $this->existingProducts[$channel_id]['step'] = $this->addStep(t('Configure existing products for channel @id', array(
                '@id' => $channel_id,
              )), 'contentProductForm', $current_step, array('existingProducts', $channel_id));
              $this->existingProducts[$channel_id]['source_fields'] = SalsifyExistingProductsMigration::salsifyAttributes() + $product['source_fields'];
              $this->sourceMigrations[$this->groupName . $migration_name] = $migration_name;
            }
            // Keep steps in order. If a migration for this step already exists,
            // skip it.
            if (!isset($this->migrations[$this->groupName . $migration_name])) {
              $current_step->nextStep = $this->existingProducts[$channel_id]['step'];
              $current_step = $this->existingProducts[$channel_id]['step'];
            }
            $this->existingProducts[$channel_id]['step_number'] = ++$step_number;
          }
        }
      }
    }
    // See if there are product display steps that need to be removed.
    foreach ($this->displays as $channel_id => $displays) {
      if (!isset($form_state['values']['displays'][$channel_id])) {
        foreach ($displays as $entity_type => $display) {
          $entity_info = entity_get_info($entity_type);
          $step = $display['step'];
          $step->previousStep->nextStep = $step->nextStep;
          $step->nextStep->previousStep = $step->previousStep;
          unset($this->sourceMigrations[$this->groupName . "$entity_info[label]DisplaysChannel$channel_id"]);
        }
        unset($this->displays[$channel_id]);
      }
    }
    // Set up product display steps.
    if (isset($form_state['values']['displays'])) {
      foreach ($form_state['values']['displays'] as $channel_id => $displays) {
        $display_entities = array();
        foreach ($displays as $entity_type => $display) {
          $entity_info = entity_get_info($entity_type);
          $migration_name = "$entity_info[label]DisplaysChannel$channel_id";
          if (!isset($this->displays[$channel_id][$entity_type]) || $this->displays[$channel_id][$entity_type]['bundle'] != $display['bundle']) {
            $this->displays[$channel_id][$entity_type] = $display;
            unset($this->migrations[$this->groupName . $migration_name]);
            unset($this->sourceMigrations[$this->groupName . $migration_name]);
          }
          if (!empty($display['bundle'])) {
            $display_entities[] = $entity_type;
            if (empty($form_state['values']['variations'][$channel_id]['type'])) {
              unset($this->displays[$channel_id][$entity_type]['variation']);
            }
            else {
              $this->displays[$channel_id][$entity_type]['variation'] = array_filter($form_state['values']['variations'][$channel_id]);
            }
            if (!isset($this->displays[$channel_id][$entity_type]['step'])) {
              $entity_info = entity_get_info($entity_type);
              $this->displays[$channel_id][$entity_type]['step'] = $this->addStep(t('Configure product @entity display for channel @id', array(
                '@entity' => $entity_info['label'],
                '@id' => $channel_id,
              )), 'contentDisplayForm', $current_step, array(
                'displays',
                $channel_id,
                $entity_type,
              ));
              $this->sourceMigrations[$this->groupName . $migration_name] = $migration_name;
            }
            // Keep steps in order. If a migration for this step already exists,
            // skip it.
            if (!isset($this->migrations[$this->groupName . $migration_name])) {
              $current_step->nextStep = $this->displays[$channel_id][$entity_type]['step'];
              $current_step = $this->displays[$channel_id][$entity_type]['step'];
            }
            $this->displays[$channel_id][$entity_type]['step_number'] = ++$step_number;
          }
        }
        if (count($display_entities) > 1) {
          foreach ($display_entities as $entity_type) {
            form_error($form_state['complete form']['displays'][$channel_id][$entity_type]['bundle'], t('Only one display entity type should be chosen for each product type.'));
          }
        }
      }
    }
    // See if there are variation settings that need to be removed.
    foreach ($this->productVariations as $channel_id => $variation) {
      if (!isset($this->groupMigrations[$this->groupName . "NewProductsChannel$channel_id"]) && !isset($this->groupMigrations[$this->groupName . "ExistingProductsChannel$channel_id"]) && !isset($form_state['values']['variations'][$channel_id])) {
        unset($this->productVariations[$channel_id]);
      }
    }
    // Save variation settings.
    if (isset($form_state['values']['variations'])) {
      foreach ($form_state['values']['variations'] as $channel_id => $variation) {
        $this->productVariations[$channel_id] = array_filter($variation);
      }
    }
    // See if there are attribute values steps that need to be removed.
    foreach ($this->attributeValues as $attribute_id => $attribute_value) {
      if (!isset($form_state['values']['attribute_values'][$attribute_id])) {
        $migration_name = "AttributeValues$attribute_id";
        $step = $attribute_value['step'];
        $step->previousStep->nextStep = $step->nextStep;
        $step->nextStep->previousStep = $step->previousStep;
        unset($this->sourceMigrations[$this->groupName . "AttributeValues$attribute_id"]);
        unset($this->attributeValues[$attribute_id]);
        unset($this->migrations[$this->groupName . $migration_name]);
        unset($this->attributeVocabularies[$attribute_id]);
      }
    }
    // Set up attribute values steps.
    if (isset($form_state['values']['attribute_values'])) {
      foreach ($form_state['values']['attribute_values'] as $attribute_id => $attribute_value) {
        $migration_name = "AttributeValues$attribute_id";
        if (!isset($this->attributeValues[$attribute_id]) || $this->attributeValues[$attribute_id]['vocabulary'] != $attribute_value['vocabulary']) {
          $this->attributeValues[$attribute_id] = $attribute_value;
          unset($this->migrations[$this->groupName . $migration_name]);
          unset($this->sourceMigrations[$this->groupName . $migration_name]);
          unset($this->attributeVocabularies[$attribute_id]);
        }
        if (!empty($attribute_value['vocabulary'])) {
          if (!isset($this->attributeValues[$attribute_id]['step'])) {
            $this->attributeValues[$attribute_id]['step'] = $this->addStep(t('Configure @name attribute values', array(
              '@name' => $this->attributes[$attribute_id]->{'salsify:name'},
            )), 'contentVocabularyForm', $current_step, array('attributeValues', $attribute_id));
            $this->sourceMigrations[$this->groupName . $migration_name] = $migration_name;
          }
          // Keep steps in order. If a migration for this step already exists,
          // skip it.
          if (!isset($this->migrations[$this->groupName . $migration_name])) {
            $current_step->nextStep = $this->attributeValues[$attribute_id]['step'];
            $current_step = $this->attributeValues[$attribute_id]['step'];
          }
          $this->attributeValues[$attribute_id]['step_number'] = ++$step_number;
          $this->attributeVocabularies[$attribute_id] = $attribute_value['vocabulary'];
        }
      }
    }
    // Set up digital assets step.
    if (isset($form_state['values']['digital_assets'])) {
      $digital_assets = $form_state['values']['digital_assets'];
      if (!isset($this->digitalAssets) || $this->digitalAssets['bundle'] != $digital_assets['bundle']) {
        $this->digitalAssets = $digital_assets;
        unset($this->migrations[$this->groupName . 'DigitalAssets']);
        unset($this->sourceMigrations[$this->groupName . 'DigitalAssets']);
      }
      if (!empty($digital_assets['bundle'])) {
        if (!isset($this->digitalAssets['step'])) {
          $this->digitalAssets['step'] = $this->addStep(t('Configure digital assets'), 'contentFileForm', $current_step, array('digitalAssets'));
          $this->sourceMigrations[$this->groupName . 'DigitalAssets'] = 'DigitalAssets';
        }
        // Keep steps in order. If a migration for this step already exists,
        // skip it.
        if (!isset($this->migrations[$this->groupName . 'DigitalAssets'])) {
          $current_step->nextStep = $this->digitalAssets['step'];
          $current_step = $this->digitalAssets['step'];
        }
        $this->digitalAssets['step_number'] = ++$step_number;
      }
    }
    // Keep steps in order.
    $current_step->nextStep = $this->lastStep;
    $this->lastStep->previousStep = $current_step;
    $this->lastStepNumber = ++$step_number;
  }

  /**
   * Configure product import.
   *
   * @see migrate_ui_edit_mappings()
   * @see SalsifyMigrateWizard::contentProductFormValidate()
   */
  protected function contentProductForm(&$form_state) {
    list($products, $channel_id) = $this->currentStep->getContext();
    $product = $this->{$products}[$channel_id];
    $form['#tree'] = TRUE;
    $form['context'] = array(
      '#type' => 'value',
      '#value' => array(
        'type' => $products,
        'id' => $channel_id,
      ),
    );
    $preset_fields = array(
      'product_id' => array('mapping' => 'salsify:id'),
      'type' => array('default_value' => $product['product_type']),
      'sku' => array('mapping' => $product['product_id']),
      'title' => array('mapping' => $product['product_name']),
      'created' => array('mapping' => 'salsify:created_at'),
      'changed' => array('mapping' => 'salsify:updated_at'),
      'revision' => array('default_value' => 1),
      'log' => array('default_value' => t('Imported data from Salsify.')),
    );
    $form[$products][$channel_id] = $this->contentForm($form_state, array(
      'type' => $products,
      'id' => $channel_id,
    ), $preset_fields);
    return $form;
  }

  /**
   * Saves product import configuration.
   *
   * @see SalsifyMigrateWizard::contentProductForm()
   */
  protected function contentProductFormValidate(&$form_state) {
    list($products, $channel_id) = $this->currentStep->getContext();
    $product = &$this->{$products}[$channel_id];
    $product['unmigrated_sources'] = array_keys(array_filter($form_state['values'][$products][$channel_id]['unmigrated_sources']));
    $product['field_mappings'] = $form_state['values'][$products][$channel_id]['field_mappings'];
    $product['dependencies'] = array();
    $product['soft_dependencies'] = array();
    $prefix = ucfirst($products);
    $this->allDependencies[$prefix . "Channel$channel_id"] = array();
    if (isset($form_state['values'][$products][$channel_id]['dependencies'])) {
      foreach ($form_state['values'][$products][$channel_id]['dependencies'] as $dependency => $value) {
        if ($value == 1) {
          $product['dependencies'][] = $dependency;
        }
        elseif ($value == 2) {
          $product['soft_dependencies'][] = $dependency;
        }
        if (!empty($value)) {
          $this->allDependencies[$prefix . "Channel$channel_id"][] = $dependency;
        }
      }
    }
  }

  /**
   * Configure product display import.
   *
   * @see migrate_ui_edit_mappings()
   * @see SalsifyMigrateWizard::contentDisplayFormValidate()
   */
  protected function contentDisplayForm(&$form_state) {
    list(, $channel_id, $entity_type) = $this->currentStep->getContext();
    $entity_info = entity_get_info($entity_type);
    $display = $this->displays[$channel_id][$entity_type];
    $form['#tree'] = TRUE;
    $form['context'] = array(
      '#type' => 'value',
      '#value' => array(
        'type' => 'displays',
        'id' => $channel_id,
        'entity_type' => $entity_type,
      ),
    );
    $preset_fields = array();
    if ($entity_type == 'node') {
      $preset_fields += array(
        'revision' => array('default_value' => 1),
        'log' => array('default_value' => t('Imported data from Salsify.')),
      );
    }
    if (!isset($display['variation']) || $display['variation']['type'] != 'fields') {
      // Only map creation and update times if we are not using shared fields to
      // define product variations.
      switch ($entity_type) {
        case 'comment':
        case 'node':
          $preset_fields += array(
            'created' => array('mapping' => 'salsify:created_at'),
            'changed' => array('mapping' => 'salsify:updated_at'),
          );
          break;

        case 'file':
          $preset_fields += array(
            'timestamp' => array('mapping' => 'salsify:created_at'),
          );
          break;

        case 'user':
          $preset_fields += array(
            'created' => array('mapping' => 'salsify:created_at'),
            'access' => array('mapping' => 'salsify:updated_at'),
          );
          break;
      }
    }
    $preset_fields += array(
      $entity_info['entity keys']['bundle'] => array('default_value' => $display['bundle']),
    );
    if (!empty($entity_info['entity keys']['label'])) {
      $preset_fields += array(
        $entity_info['entity keys']['label'] => array('mapping' => $display['product_name']),
      );
    }
    $form['displays'][$channel_id][$entity_type] = $this->contentForm($form_state, array(
      'type' => 'displays',
      'id' => $channel_id,
      'entity_type' => $entity_type,
    ), $preset_fields);
    return $form;
  }

  /**
   * Saves product display import configuration.
   *
   * @see SalsifyMigrateWizard::contentDisplayForm()
   */
  protected function contentDisplayFormValidate(&$form_state) {
    list(, $channel_id, $entity_type) = $this->currentStep->getContext();
    $display = &$this->displays[$channel_id][$entity_type];
    $display['unmigrated_sources'] = array_keys(array_filter($form_state['values']['displays'][$channel_id][$entity_type]['unmigrated_sources']));
    $display['field_mappings'] = $form_state['values']['displays'][$channel_id][$entity_type]['field_mappings'];
    $display['dependencies'] = array();
    $display['soft_dependencies'] = array();
    $entity_info = entity_get_info($entity_type);
    $migration_name = "$entity_info[label]DisplaysChannel$channel_id";
    $this->allDependencies[$migration_name] = array();
    if (isset($form_state['values']['displays'][$channel_id][$entity_type]['dependencies'])) {
      foreach ($form_state['values']['displays'][$channel_id][$entity_type]['dependencies'] as $dependency => $value) {
        if ($value == 1) {
          $display['dependencies'][] = $dependency;
        }
        elseif ($value == 2) {
          $display['soft_dependencies'][] = $dependency;
        }
        if (!empty($value)) {
          $this->allDependencies[$migration_name][] = $dependency;
        }
      }
    }
  }

  /**
   * Configures vocabulary import.
   *
   * @see migrate_ui_edit_mappings()
   * @see SalsifyMigrateWizard::contentVocabularyFormValidate()
   */
  protected function contentVocabularyForm(&$form_state) {
    list(, $attribute_id) = $this->currentStep->getContext();
    $form['#tree'] = TRUE;
    $form['context'] = array(
      '#type' => 'value',
      '#value' => array(
        'type' => 'attribute_values',
        'id' => $attribute_id,
      ),
    );
    $form['attribute_values'][$attribute_id] = $this->contentForm($form_state, array(
      'type' => 'attribute_values',
      'id' => $attribute_id,
    ), array(
      'name' => array('mapping' => 'salsify:name'),
      'parent' => array(
        'mapping' => 'salsify:parent_id',
        'source_migration' => array($this->groupName . "AttributeValues$attribute_id"),
      ),
    ));
    return $form;
  }

  /**
   * Saves vocabulary import configuration.
   *
   * @see SalsifyMigrateWizard::contentVocabularyForm()
   */
  protected function contentVocabularyFormValidate(&$form_state) {
    list(, $attribute_id) = $this->currentStep->getContext();
    $attribute_value = &$this->attributeValues[$attribute_id];
    $attribute_value['unmigrated_sources'] = array_keys(array_filter($form_state['values']['attribute_values'][$attribute_id]['unmigrated_sources']));
    $attribute_value['field_mappings'] = $form_state['values']['attribute_values'][$attribute_id]['field_mappings'];
    $attribute_value['dependencies'] = array();
    $attribute_value['soft_dependencies'] = array();
    $this->allDependencies["AttributeValues$attribute_id"] = array();
    if (isset($form_state['values']['attribute_values'][$attribute_id]['dependencies'])) {
      foreach ($form_state['values']['attribute_values'][$attribute_id]['dependencies'] as $dependency => $value) {
        if ($value == 1) {
          $attribute_value['dependencies'][] = $dependency;
        }
        elseif ($value == 2) {
          $attribute_value['soft_dependencies'][] = $dependency;
        }
        if (!empty($value)) {
          $this->allDependencies["AttributeValues$attribute_id"][] = $dependency;
        }
      }
    }
  }

  /**
   * Configures digital assets import.
   *
   * @see SalsifyMigrateWizard::contentFileFormValidate()
   */
  protected function contentFileForm(&$form_state) {
    $form['#tree'] = TRUE;
    $form['context'] = array(
      '#type' => 'value',
      '#value' => array('type' => 'digital_assets'),
    );
    $form['digital_assets'] = $this->contentForm($form_state, array(
      'type' => 'digital_assets',
    ), array(
      'value' => array('mapping' => 'salsify:url'),
      'timestamp' => array('mapping' => 'salsify:created_at'),
      'destination_dir' => array('default_value' => "public://$this->groupName"),
      'destination_file' => array('mapping' => 'salsify:name'),
      'file_replace' => array('default_value' => FILE_EXISTS_REPLACE),
      'urlencode' => array('mapping' => 'salsifyUrlEncode'),
    ));
    return $form;
  }

  /**
   * Saves digital assets import.
   *
   * @see SalsifyMigrateWizard::contentFileForm()
   */
  protected function contentFileFormValidate(&$form_state) {
    $digital_assets = &$this->digitalAssets;
    $digital_assets['unmigrated_sources'] = array_keys(array_filter($form_state['values']['digital_assets']['unmigrated_sources']));
    $digital_assets['field_mappings'] = $form_state['values']['digital_assets']['field_mappings'];
    $digital_assets['dependencies'] = array();
    $digital_assets['soft_dependencies'] = array();
    $this->allDependencies['DigitalAssets'] = array();
    if (isset($form_state['values']['digital_assets']['dependencies'])) {
      foreach ($form_state['values']['digital_assets']['dependencies'] as $dependency => $value) {
        if ($value == 1) {
          $digital_assets['dependencies'][] = $dependency;
        }
        elseif ($value == 2) {
          $digital_assets['soft_dependencies'][] = $dependency;
        }
        if (!empty($value)) {
          $this->allDependencies['DigitalAssets'][] = $dependency;
        }
      }
    }
  }

  /**
   * Shared import configuration form.
   *
   * @param array $preset_fields
   *   Default values for some fields need to be preset.
   */
  protected function contentForm(&$form_state, $context, $preset_fields = array()) {
    form_load_include($form_state, 'inc', 'migrate_ui', 'migrate_ui.pages');
    $parents = $context;
    if (count($context) > 1) {
      $parents = array_shift($parents) . '[' . implode('][', $parents) . ']';
    }
    else {
      $parents = array_shift($parents);
    }
    $form['field_mappings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Field mappings'),
      '#collapsible' => TRUE,
      '#description' => t('For each field available in your Drupal destination, select the source field used to populate it. You can enter a default value to be applied to the destination when there is no source field, or the source field is empty in a given source item. Check the DNM (Do Not Migrate) box for destination fields you do not want populated by migration.'),
      '#theme' => array('migrate_ui_field_mapping_form'),
      '#prefix' => '<div id="field-mappings">',
      '#suffix' => '</div>',
    );
    switch ($context['type']) {
      case 'newProducts':
        $import = $this->newProducts[$context['id']];
        $instances = field_info_instances('commerce_product', $import['product_type']);
        $destination = new MigrateDestinationCommerceProduct('commerce_product', $import['product_type']);
        $migration_name = "NewProductsChannel$context[id]";
        $migration = new SalsifyDummyProductsMigration();
        // Find the product bundle reference.
        $references = array_intersect_key($instances, field_read_fields(array(
          'type' => 'commerce_product_reference',
          'entity_type' => 'commerce_product',
          'bundle' => $import['product_type'],
        )));
        foreach ($references as $instance) {
          if ($instance['display']['default']['module'] == 'commerce_product_bundle') {
            $primary_reference = $instance['field_name'];
            break;
          }
        }
        break;

      case 'existingProducts':
        $import = $this->existingProducts[$context['id']];
        $instances = field_info_instances('commerce_product', $import['product_type']);
        $destination = new SalsifyDestinationCommerceProduct('commerce_product', $import['product_type']);
        $migration_name = "ExistingProductsChannel$context[id]";
        $migration = new SalsifyDummyProductsMigration();
        // Find the product bundle reference.
        $references = array_intersect_key($instances, field_read_fields(array(
          'type' => 'commerce_product_reference',
          'entity_type' => 'commerce_product',
          'bundle' => $import['product_type'],
        )));
        foreach ($references as $instance) {
          if ($instance['display']['default']['module'] == 'commerce_product_bundle') {
            $primary_reference = $instance['field_name'];
            break;
          }
        }
        break;

      case 'displays':
        $import = $this->displays[$context['id']][$context['entity_type']];
        $instances = field_info_instances($context['entity_type'], $import['bundle']);
        // Select the most specific destination class available. Destination
        // classes don't seem to be registered anywhere, so we list the ones we
        // know here.
        switch ($context['entity_type']) {
          case 'comment':
            $destination = new MigrateDestinationComment($import['bundle']);
            break;

          case 'file':
            $destination = new MigrateDestinationFile($import['bundle']);
            break;

          case 'node':
            $destination = new MigrateDestinationNode($import['bundle']);
            break;

          case 'taxonomy_term':
            $destination = new MigrateDestinationTerm($import['bundle']);
            break;

          case 'user':
            $destination = new MigrateDestinationUser();
            break;

          default:
            $destination = new MigrateDestinationEntityAPI($context['entity_type'], $import['bundle']);
            break;
        }
        $entity_info = entity_get_info($context['entity_type']);
        $migration_name = "$entity_info[label]DisplaysChannel$context[id]";
        $migration = new SalsifyDummyProductsMigration();
        // Find the primary product reference.
        $references = array_intersect_key($instances, field_read_fields(array(
          'type' => 'commerce_product_reference',
          'entity_type' => $context['entity_type'],
          'bundle' => $import['bundle'],
        )));
        // Default to the first product reference.
        $primary_reference = key($references);
        // The primary product reference is likely to perform field injection.
        foreach ($references as $instance) {
          if ($instance['settings']['field_injection']) {
            $primary_reference = $instance['field_name'];
            break;
          }
        }
        break;

      case 'attribute_values':
        $import = $this->attributeValues[$context['id']];
        $instances = field_info_instances('taxonomy_term', $import['vocabulary']);
        $destination = new MigrateDestinationTerm($import['vocabulary']);
        $migration_name = "AttributeValues$context[id]";
        $migration = NULL;
        break;

      case 'digital_assets':
        $import = $this->digitalAssets;
        $instances = field_info_instances('file', $import['bundle']);
        $destination = new MigrateDestinationFile($import['bundle']);
        $migration_name = 'DigitalAssets';
        $migration = NULL;
        break;
    }
    // Save used source fields so we can mark them as enabled.
    $enabled_sources = array();
    // Get all migrations in this group for dependency and source migration
    // selection.
    $migrations = $this->groupMigrations + $this->sourceMigrations;
    foreach ($destination->fields($migration) as $name => $description) {
      if (is_array($description)) {
        $description = reset($description);
      }
      // Indent subfields and options to show their relationship to the parent.
      if (strpos($name, ':') > 0) {
        $description = '&nbsp;&nbsp;' . $description;
      }
      $mapping = drupal_array_get_nested_value($form_state['values'], $context + array(
        'field_mappings',
        $name,
      ), $key_exists);
      if (!$key_exists) {
        $mapping = array(
          'issue_group' => TRUE,
          'mapping' => -1,
          'default_value' => '',
          'source_migration' => array(),
        );
        if (isset($preset_fields[$name])) {
          // Apply preset mapping.
          $mapping = array_merge($mapping, $preset_fields[$name]);
        }
        elseif (isset($instances[$name])) {
          // Map complex fields.
          $field = field_info_field($name);
          switch ($field['type']) {
            case 'taxonomy_term_reference':
              $vocabulary = taxonomy_vocabulary_machine_name_load($field['settings']['allowed_values'][0]['vocabulary']);
              // See if this vocabulary is being used in an attribute values
              // migration.
              if (in_array($vocabulary->machine_name, $this->attributeVocabularies)) {
                $preset_fields["$name:source_type"] = array('default_value' => 'tid');
                $mapping['mapping'] = array_search($vocabulary->machine_name, $this->attributeVocabularies);
                $mapping['source_migration'] = array($this->groupName . "AttributeValues$mapping[mapping]");
              }
              elseif (isset($import['source_fields'][$vocabulary->machine_name])) {
                $preset_fields["$name:create_term"] = array('default_value' => 'TRUE');
                $mapping['mapping'] = $vocabulary->machine_name;
              }
              elseif (in_array($vocabulary->name, $import['source_fields'])) {
                $preset_fields["$name:create_term"] = array('default_value' => 'TRUE');
                $mapping['mapping'] = array_search($vocabulary->name, $import['source_fields']);
              }
              break;

            case 'file':
            case 'image':
              if (isset($import['source_fields']['salsify:digital_assets::salsify:id'])) {
                // Check for a file or image field to map our digital assets
                // to.
                if (isset($this->groupMigrations[$this->groupName . 'DigitalAssets']) || !empty($this->digitalAssets['bundle'])) {
                  $preset_fields["$name:file_class"] = array('default_value' => 'MigrateFileFid');
                  $preset_fields["$name:preserve_files"] = array('default_value' => TRUE);
                  $mapping['mapping'] = 'salsify:digital_assets::salsify:id';
                  $mapping['source_migration'] = array($this->groupName . 'DigitalAssets');
                }
                else {
                  $mapping['mapping'] = 'salsify:digital_assets::salsify:url';
                  $preset_fields["$name:destination_file"] = array('mapping' => 'salsify:digital_assets::salsify:name');
                  $preset_fields["$name:urlencode"] = array('mapping' => 'salsify:digital_assets::salsifyUrlEncode');
                }
              }
              break;

            case 'commerce_product_reference':
              switch ($context['type']) {
                case 'newProducts':
                case 'existingProducts':
                case 'displays':
                  $relation_type = $this->productVariations[$context['id']]['attribute_id'];
                  if ($name == $primary_reference && $instances[$name]['display']['default']['module'] == 'commerce_product_bundle') {
                    $mapping['mapping'] = 'salsify:bundled_products::salsify:bundled_product_id';
                  }
                  elseif ($name == $primary_reference && $context['type'] == 'displays') {
                    if (isset($import['variation']) && $import['variation']['type'] == 'relation') {
                      $mapping['mapping'] = 'salsify:relations::salsify:target_product_id';
                      if (isset($this->productVariations[$context['id']]['relation_type'])) {
                        $preset_fields["$name:filter_attribute"] = array('mapping' => "salsify:relations::$relation_type");
                        $preset_fields["$name:filter_value"] = array('default_value' => $this->productVariations[$context['id']]['relation_type']);
                      }
                    }
                    else {
                      $mapping['mapping'] = $import['product_id'];
                    }
                  }
                  else {
                    $mapping['mapping'] = 'salsify:relations::salsify:target_product_id';
                    if (isset($this->productVariations[$context['id']]['attribute_id'])) {
                      $preset_fields["$name:filter_attribute"] = array('mapping' => "salsify:relations::$relation_type");
                    }
                    if (isset($this->productVariations[$context['id']]['attribute_values'][$name])) {
                      // Map to source field with matching name.
                      $preset_fields["$name:filter_value"] = array('default_value' => $name);
                    }
                    else {
                      $label = $instances[$name]['label'];
                      if (isset($this->productVariations[$context['id']]['attribute_values'][$label])) {
                        // Map to source field with matching label.
                        $preset_fields["$name:filter_value"] = array('default_value' => $label);
                      }
                    }
                  }
                  foreach ($this->groupMigrations + $this->sourceMigrations as $source_migration => $source_migration_name) {
                    if (strpos($source_migration_name, 'NewProductsChannel') === 0 || strpos($source_migration_name, 'ExistingProductsChannel') === 0) {
                      $mapping['source_migration'][] = $source_migration;
                    }
                  }
                  break;
              }
              break;

            default:
              if (isset($import['source_fields'][$name])) {
                // Map to source field with matching name.
                $mapping['mapping'] = $name;
              }
              else {
                $label = $instances[$name]['label'];
                if (isset($import['source_fields'][$label])) {
                  // Map to source field with matching label.
                  $mapping['mapping'] = $label;
                }
              }
              break;
          }
        }
        elseif (isset($import['source_fields'][$name])) {
          // Map to source field with matching name.
          $mapping['mapping'] = $name;
        }
        if ($mapping['mapping'] != -1) {
          $enabled_sources[] = $mapping['mapping'];
        }
        if ($mapping['mapping'] != -1 || $mapping['default_value'] !== '') {
          $mapping['issue_group'] = FALSE;
        }
      }
      $form['field_mappings'][$name]['issue_group'] = array(
        '#type' => 'checkbox',
        '#default_value' => $mapping['issue_group'],
        '#ajax' => array(
          'callback' => 'salsify_dnm_changed',
          'wrapper' => 'field-mappings',
        ),
      );
      $form['field_mappings'][$name]['mapping'] = array(
        '#type' => 'select',
        '#title' => user_access(MIGRATE_ACCESS_ADVANCED) ? "$description [$name]" : $description,
        '#options' => array(),
        '#empty_option' => '',
        '#empty_value' => -1,
        '#default_value' => $mapping['mapping'],
        '#states' => array(
          'disabled' => array(
            ':input[name="' . $parents . '[field_mappings][' . $name . '][issue_group]"]' => array(
              'checked' => TRUE,
            ),
          ),
        ),
      );
      $form['field_mappings'][$name]['default_value'] = array(
        '#type' => 'textfield',
        '#default_value' => $mapping['default_value'],
        '#size' => 20,
        '#states' => array(
          'disabled' => array(
            ':input[name="' . $parents . '[field_mappings][' . $name . '][issue_group]"]' => array(
              'checked' => TRUE,
            ),
          ),
        ),
      );
      $form['field_mappings'][$name]['source_migration'] = array(
        '#type' => 'select',
        '#options' => $migrations,
        '#multiple' => TRUE,
        '#size' => min(4, count($migrations)),
        '#default_value' => $mapping['source_migration'],
        '#states' => array(
          'disabled' => array(
            ':input[name="' . $parents . '[field_mappings][' . $name . '][issue_group]"]' => array(
              'checked' => TRUE,
            ),
          ),
        ),
      );
    }
    $form['unmigrated_sources'] = array(
      '#type' => 'fieldset',
      '#title' => t('Source fields'),
      '#collapsible' => TRUE,
      '#description' => t('Check off any source fields that are not being mapped to destination fields. They will be removed from the select lists above.'),
    );
    $field_options = array();
    foreach ($import['source_fields'] as $name => $description) {
      // Clean up the source field description.
      if (is_array($description)) {
        $description = reset($description);
      }
      $description = strip_tags($description);
      // Limit the description length.
      if (strlen($description) > 50) {
        $short_description = substr($description, 0, 50) . '…';
      }
      else {
        $short_description = $description;
      }
      $dnm_value = drupal_array_get_nested_value($form_state['values'], $context + array(
        'unmigrated_sources',
        $name,
      ), $key_exists);
      if (!$key_exists) {
        $dnm_value = !in_array($name, $enabled_sources);
      }
      // So, if this field is not marked DNM, include it in possible source
      // options in the field mappings.
      if (!$dnm_value) {
        $field_options[$name] = user_access(MIGRATE_ACCESS_ADVANCED) ? "$short_description [$name]" : $short_description;
      }
      $form['unmigrated_sources'][$name] = array(
        '#type' => 'checkbox',
        '#title' => user_access(MIGRATE_ACCESS_ADVANCED) ? "$description [$name]" : $description,
        '#default_value' => $dnm_value,
        '#ajax' => array(
          'callback' => 'salsify_dnm_changed',
          'wrapper' => 'field-mappings',
        ),
      );
    }
    foreach (element_children($form['field_mappings']) as $name) {
      $form['field_mappings'][$name]['mapping']['#options'] += $field_options;
    }
    $dependency_options = array(
      0 => t('None'),
      1 => t('Hard'),
      2 => t('Soft'),
    );
    // Remove group name from beginning of dependencies. It is added by
    // MigrateUIWizard::addMigration().
    $source_migration_options = drupal_map_assoc($migrations);
    // A migration cannot depend on itself.
    unset($source_migration_options[$migration_name]);
    $source_migration_options = array_diff_key($source_migration_options, _migrate_ui_get_descendents($migration_name, $this->allDependencies));
    $form['dependencies'] = array(
      '#type' => 'fieldset',
      '#title' => t('Dependencies'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#theme' => 'migrate_ui_field_mapping_dependencies',
      '#description' => t('Set any dependencies on other migrations here. A <em>hard dependency</em> means that this migration is not allowed to run until the dependent migration has completed (without forcing it). A <em>soft dependency</em> places no such requirement - it simply affects the default order of migration. Note that any migrations that would lead to circular dependencies are not listed here.'),
    );
    foreach ($source_migration_options as $name) {
      if (isset($import['dependencies']) && in_array($name, $import['dependencies'])) {
        $default_value = 1;
      }
      elseif (isset($import['soft_dependencies']) && in_array($name, $import['soft_dependencies'])) {
        $default_value = 2;
      }
      else {
        $default_value = 0;
      }
      $form['dependencies'][$name] = array(
        '#type' => 'select',
        '#title' => $name,
        '#default_value' => $default_value,
        '#options' => $dependency_options,
      );
    }
    return $form;
  }

  /**
   * Saves migrations.
   */
  protected function reviewForm(&$form_state) {
    // Save arguments that apply to all migrations in this group.
    $this->groupArguments = array(
      'api_key' => $this->apiKey,
      'channels' => $this->channels,
      'expiration' => $this->expiration,
    );
    // Make the content selection step link directly to the review step.
    $this->currentStep->previousStep = $this->contentSelectStep;
    $this->contentSelectStep->nextStep = $this->currentStep;
    // Save channel migrations.
    foreach (array_keys($this->channels) as $channel_id) {
      // Save new product migration.
      if (isset($this->newProducts[$channel_id])) {
        $product = $this->newProducts[$channel_id];
        if (!empty($product['product_type'])) {
          $product['channel_ids'] = array($channel_id);
          $product['field_mappings'] = self::salsifyFieldMappings('MigrateDestinationCommerceProduct', $product, $this->attributes, array(
            'commerce_product',
            $product['product_type'],
          ));
          $this->addMigration("NewProductsChannel$channel_id", 'SalsifyNewProductsMigration', $product);
          // Link this step directly to the content selection and review steps.
          $product['step']->previousStep = $this->contentSelectStep;
          $product['step']->nextStep = $this->currentStep;
        }
      }
      // Save existing product migration.
      if (isset($this->existingProducts[$channel_id])) {
        $product = $this->existingProducts[$channel_id];
        if (!empty($product['product_type'])) {
          $product['channel_ids'] = array($channel_id);
          $product['field_mappings'] = self::salsifyFieldMappings('MigrateDestinationCommerceProduct', $product, $this->attributes, array(
            'commerce_product',
            $product['product_type'],
          ));
          $this->addMigration("ExistingProductsChannel$channel_id", 'SalsifyExistingProductsMigration', $product);
          // Link this step directly to the content selection and review steps.
          $product['step']->previousStep = $this->contentSelectStep;
          $product['step']->nextStep = $this->currentStep;
        }
      }
    }
    // Save product display migrations.
    foreach ($this->displays as $channel_id => $displays) {
      foreach ($displays as $entity_type => $display) {
        if (!empty($display['bundle'])) {
          $entity_info = entity_get_info($entity_type);
          // Select the most specific destination class available. Destination
          // classes don't seem to be registered anywhere, so we list the ones
          // we know here.
          switch ($entity_type) {
            case 'comment':
              $dest_class = 'MigrateDestinationComment';
              $dest_args = array($display['bundle']);
              break;

            case 'file':
              $dest_class = 'MigrateDestinationFile';
              $dest_args = array($display['bundle']);
              break;

            case 'node':
              $dest_class = 'MigrateDestinationNode';
              $dest_args = array($display['bundle']);
              break;

            case 'taxonomy_term':
              $dest_class = 'MigrateDestinationTerm';
              $dest_args = array($display['bundle']);
              break;

            case 'user':
              $dest_class = 'MigrateDestinationUser';
              $dest_args = array();
              break;

            default:
              $dest_class = 'MigrateDestinationEntityAPI';
              $dest_args = array(
                $entity_type,
                $display['bundle'],
              );
              break;
          }
          $display['channel_ids'] = array($channel_id);
          $display['entity_type'] = $entity_type;
          $display['field_mappings'] = self::salsifyFieldMappings($dest_class, $display, $this->attributes, $dest_args);
          $this->addMigration("$entity_info[label]DisplaysChannel$channel_id", 'SalsifyDisplaysMigration', $display);
          // Link this step directly to the content selection and review steps.
          $display['step']->previousStep = $this->contentSelectStep;
          $display['step']->nextStep = $this->currentStep;
        }
      }
    }
    // Save attribute value migrations.
    foreach ($this->attributeValues as $attribute_id => $attribute_value) {
      if (!empty($attribute_value['vocabulary'])) {
        $attribute_value['attribute'] = $this->attributes[$attribute_id];
        $attribute_value['field_mappings'] = self::salsifyFieldMappings('MigrateDestinationTerm', $attribute_value, $this->attributes, array(
          $attribute_value['vocabulary'],
        ));
        $this->addMigration("AttributeValues$attribute_id", 'SalsifyAttributeValuesMigration', $attribute_value);
        // Link this step directly to the content selection and review steps.
        $attribute_value['step']->previousStep = $this->contentSelectStep;
        $attribute_value['step']->nextStep = $this->currentStep;
      }
    }
    // Save digital asset migration.
    if (!empty($this->digitalAssets['bundle'])) {
      $digital_assets = $this->digitalAssets;
      $digital_assets['field_mappings'] = self::salsifyFieldMappings('MigrateDestinationFile', $digital_assets, $this->attributes, array(
        $digital_assets['bundle'],
      ));
      $this->addMigration('DigitalAssets', 'SalsifyDigitalAssetsMigration', $digital_assets);
      // Link this step directly to the content selection and review steps.
      $digital_assets['step']->previousStep = $this->contentSelectStep;
      $digital_assets['step']->nextStep = $this->currentStep;
    }
    // Display data about each migration.
    $form['migrations'] = array(
      '#theme' => 'salsify_form_table',
      '#header' => array(
        'machinename' => t('Task'),
        'importrows' => t('Items'),
      ),
      '#caption' => format_plural(count($this->migrations), 'The following migration will be created:', 'The following migrations will be created:'),
      '#empty' => t('No tasks are defined for this migration group.'),
    );
    if (user_access(MIGRATE_ACCESS_ADVANCED)) {
      $form['migrations']['#header']['lastthroughput'] = array(
        'data' => t('Throughput'),
        'colspan' => 2,
      );
    }
    else {
      $form['migrations']['#header']['lastthroughput']['importrows'] = 2;
    }
    foreach ($this->migrations as $machine_name => $migration) {
      $class = new ReflectionClass($migration['class_name']);
      $migration = $class->newInstance($migration['arguments'] + $this->groupArguments);
      $group_length = strlen($this->groupName);
      if (!strncasecmp($this->groupName, $machine_name, $group_length)) {
        $form['migrations'][$machine_name]['machinename']['#markup'] = substr($machine_name, $group_length);
      }
      else {
        $form['migrations'][$machine_name]['machinename']['#markup'] = $machine_name;
      }
      if (method_exists($migration, 'sourceCount')) {
        $importrows = $migration->sourceCount();
        if ($importrows < 0) {
          $importrows = t('N/A');
        }
      }
      else {
        $importrows = t('N/A');
      }
      $form['migrations'][$machine_name]['importrows']['#markup'] = $importrows;
      if (user_access(MIGRATE_ACCESS_ADVANCED)) {
        if (method_exists($migration, 'getLastThroughput')) {
          $rate = $migration->getLastThroughput();
          if ($rate == '') {
            $lastthroughput = t('Unknown');
          }
          else {
            $lastthroughput = t('!rate/min', array('!rate' => $rate));
          }
        }
        else {
          $lastthroughput = t('N/A');
        }
        $form['migrations'][$machine_name]['lastthroughput']['#markup'] = $lastthroughput;
      }
      $form['migrations'][$machine_name]['edit'] = array(
        '#type' => 'submit',
        '#value' => t('Edit'),
        '#name' => $machine_name,
        '#submit' => array('salsify_wizard_goto_step'),
        '#limit_validation_errors' => array(),
      );
      // Remove mapping tables. They will be created permanently when the
      // migration is saved.
      $migration->getMap()->destroy();
    }
    if (empty($this->groupMigrations)) {
      MigrateGroup::deregister($this->groupName);
    }
    return $form;
  }

  /**
   * Turns form values into an array of field mappings.
   *
   * @param string $destination_class
   *   The name of the migration class for the destination entity type. Used to
   *   populate mapping descriptions.
   * @param array $settings
   *   Migration settings from previous configuration forms.
   * @param array $attributes
   *   An array of attributes. Used to populate mapping descriptions.
   * @param array $arguments
   *   Arguments required by the destination class constructor.
   *
   * @return array
   *   An array of field mappings.
   */
  static public function salsifyFieldMappings($destination_class, $settings, $attributes = array(), $arguments = array()) {
    $field_mappings = array();
    $class = new ReflectionClass($destination_class);
    $destination = $class->newInstanceArgs($arguments);
    $fields = $destination->fields();
    foreach ($settings['field_mappings'] as $destination_field => $mapping) {
      if ($mapping['issue_group'] || $mapping['mapping'] == -1) {
        $mapping['mapping'] = NULL;
      }
      $field_mapping = new MigrateFieldMapping($destination_field, $mapping['mapping']);
      if ($mapping['issue_group']) {
        $field_mapping->issueGroup(t('DNM'));
        if (isset($fields[$destination_field])) {
          $field_mapping->description($fields[$destination_field]);
        }
      }
      else {
        $field_mapping->defaultValue($mapping['default_value']);
        if (isset($mapping['mapping'])) {
          if (isset($attributes[$mapping['mapping']])) {
            $field_mapping->description($attributes[$mapping['mapping']]->{'salsify:help_text'});
          }
          elseif (isset($settings['source_fields'][$mapping['mapping']])) {
            $field_mapping->description($settings['source_fields'][$mapping['mapping']]);
          }
        }
        elseif (isset($fields[$destination_field])) {
          $field_mapping->description($fields[$destination_field]);
        }
      }
      if ($mapping['source_migration'] && $mapping['source_migration'] != -1) {
        $field_mapping->sourceMigration(array_values($mapping['source_migration']));
      }
      $field_mappings[] = $field_mapping;
    }
    // Handle any source fields marked DNM.
    foreach ($settings['unmigrated_sources'] as $name) {
      $field_mapping = new MigrateFieldMapping(NULL, $name);
      $field_mapping->issueGroup(t('DNM'));
      if (isset($attributes[$name])) {
        $field_mapping->description($attributes[$name]->{'salsify:help_text'});
      }
      elseif (isset($settings['source_fields'][$name])) {
        $field_mapping->description($settings['source_fields'][$name]);
      }
      $field_mappings[] = $field_mapping;
    }
    return $field_mappings;
  }

  /**
   * Jump to a specific step of the wizard.
   */
  public function gotoStep(&$form_state) {
    $machine_name = $form_state['triggering_element']['#name'];
    $migration = $this->migrations[$machine_name];
    $this->currentStep = $migration['arguments']['step'];
    $form_state['values'] = $this->currentStep->getFormValues();
    $form_state['rebuild'] = TRUE;
  }

}

/**
 * Checks for duplicate group names.
 *
 * @param string $name
 *   The group name to check.
 *
 * @return bool
 *   TRUE if the group name is in use, FALSE otherwise.
 */
function salsify_migrate_group_exists($name) {
  migrate_migrations();
  $groups = MigrateGroup::groups();
  return isset($groups[$name]);
}

/**
 * Implements callback_batch_operation().
 */
function salsify_channel_batch_operation($channel_id, &$context) {
  $batch = &batch_get();
  $batch['form_state']['wizard']->channelBatchOperation($channel_id, $context);
}

/**
 * Implements callback_batch_finished().
 */
function salsify_channel_batch_finished($success, $results, $operations) {
  $batch = &batch_get();
  if ($success) {
    foreach ($results as $key => $error) {
      switch ($key) {
        case 'channel_ids':
          form_set_error('channel_ids', format_plural(count($error), 'Channel %channel_id was not found.', 'Channels !channel_ids and %channel_id were not found.', array(
            '%channel_id' => array_pop($error),
            '!channel_ids' => count($error) > 1 ? implode(', ', array_map('drupal_placeholder', $error)) . ',' : drupal_placeholder(reset($error)),
          )));
          break;

        default:
          form_set_error($key, filter_xss($error));
          break;
      }
    }
  }
  else {
    // An error occurred.
    form_set_error('channel_ids', t('An error occurred while processing channel %channel_id.', array('%channel_id' => $operations[0][1][0])));
  }
  if ($batch['form_state']['salsify_batch_errors'] = form_get_errors()) {
    $batch['form_state']['wizard']->gotoPreviousStep($batch['form_state']);
  }
}

/**
 * Selects the field mapping fieldset for re-rendering.
 *
 * @see migrate_ui_dnm_changed()
 */
function salsify_dnm_changed(&$form, &$form_state) {
  return drupal_array_get_nested_value($form, $form_state['values']['context'] + array('field_mappings'));
}

/**
 * Renders the wizard migrations review table.
 */
function theme_salsify_form_table($variables) {
  $elements = $variables['elements'];
  $table = array();
  // Add table headers and attributes.
  foreach (array(
    'header',
    'attributes',
    'caption',
    'colgroups',
    'sticky',
    'empty',
  ) as $key) {
    if (isset($elements["#$key"])) {
      $table[$key] = $elements["#$key"];
    }
  }
  foreach (element_children($elements) as $name) {
    $element = $elements[$name];

    $row = array('data' => array());
    if (isset($element['#attributes'])) {
      $row += $element['#attributes'];
    }
    // Render children as table cells.
    foreach (element_children($element) as $cell_key) {
      $child = &$element[$cell_key];
      // Do not render a cell for children of #type 'value'.
      if (!isset($child['#type']) || $child['#type'] != 'value') {
        $cell = array('data' => drupal_render($child));
        if (isset($child['#cell_attributes'])) {
          $cell += $child['#cell_attributes'];
        }
        $row['data'][] = $cell;
      }
    }
    $table['rows'][] = $row;
  }
  return theme('table', $table);
}

/**
 * Jumps to a specific step of the wizard.
 */
function salsify_wizard_goto_step($form, &$form_state) {
  $form_state['wizard']->gotoStep($form_state);
}

/**
 * Loads attributes from a Salsify channel.
 */
class SalsifyAttributesJSONReader extends SalsifyJSONReader {

  /**
   * {@inheritdoc}
   */
  public function next() {
    $this->salsifyNext('attributes');
  }

}

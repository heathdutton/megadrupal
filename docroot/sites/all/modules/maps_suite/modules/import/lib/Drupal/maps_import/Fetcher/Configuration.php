<?php

/**
 * @file
 * Handles configuration import operation.
 */

namespace Drupal\maps_import\Fetcher;

use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Exception\OperationException;

/**
 * MaPS Import Configuration fetcher class.
 */
class Configuration extends Fetcher {

  /**
   * The configuration type related to an attribute-object.
   *
   * @var string
   */
  const ATTRIBUTE_OBJECT = 'attribute';

  /**
   * The configuration type related to a linked object.
   *
   * @var string
   */
  const LINKED_OBJECT = 'linked';

  /**
   * The configuration type related to a criteria object.
   *
   * @var string
   */
  const CRITERIA_OBJECT = 'criteria';

  /**
   * @inheritdoc
   */
  public function __construct(Profile $profile) {
    parent::__construct($profile);
    $arguments = array(
      'config' => TRUE,
      'watch_config' => 131071,
    );

    if ($presetGroupId = $profile->getPresetGroupId()) {
      $arguments['presetsGroup'] = $presetGroupId;
    }

    $this->setArguments($arguments);

    // Differential call.
    if (variable_get('maps_import_differential', 0)) {
      if ($min_updated_date = variable_get('maps_import:configuration_diff:' . $this->getProfile()->getPid(), FALSE)) {
        $this->setArguments(array('minUpdatedDate' => $min_updated_date));
      }
    }
  }

  /**
   * @inheritdoc
   */
  public function getTitle() {
    return t('Configuration import');
  }

  /**
   * @inheritdoc
   */
  public function getDescription() {
    return t('The configuration import operation is related to the profile %profile. It first performs a request against MaPS System® Web services to get data related to configration, like language list, image presets, library items etc. Then it parses the fetched data and finally stores them in the configuration tables of the database.', array('%profile' => $this->getProfile()->getTitle()));
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return 'configuration_fetch';
  }

  /**
   * @inheritdoc
   */
  protected function fetch(&$context = NULL) {
    $this->setTimeout(self::TIMEOUT);

    return parent::fetch($context);
  }

  /**
   * Gets information about configuration elements.
   *
   * @return array
   *   Array whose keys are parsed data root element and whose values
   *   are the type of data.
   */
  protected function info() {
    return array(
      'languages' => 'language',
      'units' => 'unit',
      'link_types' => 'link_type',
      'classes' => 'class',
      'object_natures' => 'object_nature',
      'object_types' => 'object_type',
      'presets' => 'image_preset',
      'attributes' => 'attribute',
      'media_types' => 'media_type',
      'libraries' => 'library',
      'statuses' => 'status',
      'object_attribute' => 'attribute_object', // An object used as an attribute.
      'object_linked' => 'attribute_object', // An linked object outside the publication scope.
      'object_criteria' => 'object_criteria', // An linked object outside the publication scope.
      'attribute_sets' => 'attribute_set',
      'object_class_has_attribute_set' => 'object_class_has_attribute_set',
      'attribute_set_has_attribute' => 'attribute_set_has_attribute',
      'metaclasses' => 'metaclass',
    );
  }

  /**
   * @inheritdoc
   */
  protected function save() {
    $info = $this->info();

    // Open a transaction to ensure data integrity.
    $transaction = db_transaction();

    try {
      // Remove old data for current profile, only if we are not using the differential import.
      if (!(variable_get('maps_import_differential', 0) && variable_get('maps_import:configuration_diff:' . $this->getProfile()->getPid(), FALSE))) {
        db_delete($this::DB_CONF_TABLE)->condition('pid', $this->getProfile()->getPid())->execute();
        db_delete($this::DB_LIBRARY_TABLE)->condition('pid', $this->getProfile()->getPid())->execute();
      }

      foreach ($info as $key => $type) {
        if (!empty($this->data[$key])) {
          // Remember that function and method names are not case-sensitive, but
          // however we modify come characters case, only for design.
          $method = 'dataSave' . maps_suite_drupal2camelcase($key);

          $callback = method_exists($this, $method) ? array($this, $method) : array($this, 'dataSave');
          call_user_func_array($callback, array($this->data[$key], $type));
        }
      }

      // Clear object and media properties cache for current profile.
      cache_clear_all('object-properties:' . $this->getProfile()->getPid() . ':', 'cache_maps_suite', TRUE);
      cache_clear_all('media-properties:' . $this->getProfile()->getPid() . ':', 'cache_maps_suite', TRUE);
    }
    catch(Exception $e) {
      $transaction->rollback();

      if ($e instanceof OperationException) {
        $e->addContext('$profile', $this->getProfile());
        $e->watchdog();
      }
      else {
        watchdog_exception('maps_suite', $e);
      }

      if ($this->isBatch()) {
        $this->context['results']['error'] = $e->getMessage();
        throw $e;
      }

      return FALSE;
    }

    return TRUE;
  }

  /**
   * @inheritdoc
   */
  protected function processRun($index = 0, $range = 0, $last = TRUE) {
    $currentOperation = 1;
    $totalOperations = 1;

    if ($range > 0) {
       $currentOperation = ceil($index / $range) + 1;
       $totalOperations = ceil($this->getTotalOperations() / $range);
    }

    // Message displayed under the progressbar.
    $this->context['message'] = t('Fetching configuration for profile "@title" (@current/@total)', array(
      '@title' => $this->getProfile()->getTitle(),
      '@current' => $currentOperation,
      '@total' => $totalOperations,
    ));

    $this->getlog()
      ->setCurrentOperation(isset($currentOperation) ? $currentOperation : 1)
      ->setTotalOperations(isset($totalOperations) ? $totalOperations : 1);

    if ($last) {
      $this->updateImportDate();
    }

  	return parent::processRun($index, $range, $last);
  }

  /**
   * Saves "metaclass" data.
   */
  protected function dataSaveMetaclasses($data, $type) {
    foreach ($data as $idmetaclass => $metaclass) {
      if( empty($metaclass['code'])) {
        continue;
      }

      foreach ($metaclass['values'] as $id_language => $values) {
        $value = reset($values);

        db_merge($this::DB_CONF_TABLE)
          ->insertFields(array(
            'pid' => $this->getProfile()->getPid(),
            'type' => $type,
            'id' => $idmetaclass,
            'id_language' => $id_language,
            'code' => $metaclass['code'],
            'data' => serialize($metaclass['data']),
            'title' => $value['value'],
          ))
          ->updateFields(array(
            'code' => $metaclass['code'],
            'data' => serialize($metaclass['data']),
            'title' => $value['value'],
          ))
          ->key(array(
            'pid' => $this->getProfile()->getPid(),
            'type' => $type,
            'id' => $idmetaclass,
          ))
          ->execute();
      }

      $this->incrementCounter('insert', $type);
    } 
  }

  /**
   * Saves "attribute_set_has_attribute" data.
   */
  protected function dataSaveAttributeSetHasAttribute(array $data, $type) {
    foreach ($data as $idattribute_set => $attributes) {
      $attribute_ids = array();

      if (empty($attributes['idattribute'])) {
        continue;
      }

      foreach ($attributes['idattribute'] as $attribute) {
        if (is_array($attribute) && isset($attribute['value'])) {
          $attribute_ids[] = (int) $attribute['value'];
        } else {
          $attribute_ids[] = (int) $attribute;
        }
      }

      db_merge($this::DB_CONF_TABLE)
        ->insertFields(array(
          'pid' => $this->getProfile()->getPid(),
          'type' => $type,
          'id' => $idattribute_set,
          'code' => $idattribute_set,
          'data' => serialize($attribute_ids),
        ))
        ->updateFields(array(
          'code' => $idattribute_set,
          'data' => serialize($attribute_ids),
        ))
        ->key(array(
          'pid' => $this->getProfile()->getPid(),
          'type' => $type,
          'id' => $idattribute_set,
        ))
        ->execute();

      $this->incrementCounter('insert', $type);
    }
  }

  /**
   * Saves "object_class_has_attribute_set" data.
   */
  protected function dataSaveObjectClassHasAttributeSet(array $data, $type) {
    foreach ($data as $idobject_class => $attribute_sets) {
      $attribute_sets_ids = array();

      if (empty($attribute_sets['idattribute_set'])) {
        continue;
      }

      foreach ($attribute_sets['idattribute_set'] as $attribute_set) {
        if (!isset($attribute_set['value'])) {
          $attribute_sets_ids[] = (int)$attribute_set;
        }
        else {
          $attribute_sets_ids[] = (int) $attribute_set['value'];
        }
      }

      db_merge($this::DB_CONF_TABLE)
        ->insertFields(array(
          'pid' => $this->getProfile()->getPid(),
          'type' => $type,
          'id' => $idobject_class,
          'code' => $idobject_class,
          'data' => serialize($attribute_sets_ids),
        ))
        ->updateFields(array(
          'code' => $idobject_class,
          'data' => serialize($attribute_sets_ids),
        ))
        ->key(array(
          'pid' => $this->getProfile()->getPid(),
          'type' => $type,
          'id' => $idobject_class,
        ))
        ->execute();

      $this->incrementCounter('insert', $type);
    }
  }

  /**
   * Saves "object_linked" data.
   */
  protected function dataSaveObjectLinked(array $data, $type) {
    $this->saveObjects($data, self::LINKED_OBJECT);
  }

  /**
   * Saves "object_attribute" data.
   *
   * Those objects are saved in the {maps_import_objects} and not in
   * a configuration table in order to be converted as standard objects.
   */
  protected function dataSaveObjectAttribute(array $data, $type) {
    $this->saveObjects($data, self::ATTRIBUTE_OBJECT);
  }

  /**
   * Saves "object_criteria" data.
   *
   * Those objects are saved in the {maps_import_objects} and not in
   * a configuration table in order to be converted as standard objects.
   */
  protected function dataSaveObjectCriteria(array $data, $type) {
    $this->saveObjects($data, self::CRITERIA_OBJECT);
  }

  /**
   * Saves "object_attribute" data.
   */
  protected function dataSaveLibraries(array $data, $type) {
    foreach ($data as $id_attribute => $library) {
      foreach ($library as $id => $item) {
        foreach ($item['values'] as $id_language => $values) {
          $value = reset($values);

          if (empty($value['value'])) {
	          continue;
     	    }

          db_merge($this::DB_LIBRARY_TABLE)
            ->insertFields(array(
              'pid' => $this->getProfile()->getPid(),
              'id' => $id,
              'code' => $item['code'],
              'id_attribute' => $id_attribute,
              'id_language' => $id_language,
              'value' => $value['value'],
            ))
            ->updateFields(array(
              'code' => $item['code'],
              'value' => $value['value'],
            ))
            ->key(array(
              'pid' => $this->getProfile()->getPid(),
              'id' => $id,
              'id_attribute' => $id_attribute,
              'id_language' => $id_language,
            ))
            ->execute();

          $this->incrementCounter('insert', $type);
        }
      }
    }
  }

  /**
   * Saves "statuses" data.
   */
  protected function dataSaveStatuses(array $data, $type) {
    foreach ($data as $id => $status) {
      db_merge($this::DB_CONF_TABLE)
        ->insertFields(array(
          'pid' => $this->getProfile()->getPid(),
          'type' => 'status',
          'id' => $id,
          'code' => $status['value'],
          'title' => $status['value'],
        ))
        ->updateFields(array(
          'code' => $status['value'],
          'title' => $status['value'],
        ))
        ->key(array(
          'pid' => $this->getProfile()->getPid(),
          'type' => 'status',
          'id' => $id,
        ))
        ->execute();

      $this->incrementCounter('insert', $type);
    }
  }

  /**
   * Saves "presets" data.
   */
  protected function dataSavePresets(array $data, $type) {
    foreach ($data as $id => $preset) {
      db_merge($this::DB_CONF_TABLE)
        ->insertFields(array(
          'pid' => $this->getProfile()->getPid(),
          'type' => $type,
          'id' => $id,
          'code' => $id,
          'title' => $preset['name']['value'],
        ))
        ->updateFields(array(
          'code' => $id,
          'title' => $preset['name']['value'],
        ))
        ->key(array(
          'pid' => $this->getProfile()->getPid(),
          'type' => $type,
          'id' => $id,
        ))
        ->execute();

      $this->incrementCounter('insert', $type);
    }
  }

  /**
   * Saves a piece of data into the database.
   *
   * This default method should only be used on standard formatted data,
   * whose structure is:
   * array(
   *   (int) id => array(
   *     (string) attribute1 => mixed value,
   *     ...
   *     'values' => array(
   *       (int or 'und') id_language => array(
   *         (string) attributeN => mixed value,
   *         ...
   *         'value' => (string) translated title,
   *       ),
   *     ),
   *   )
   * )
   */
  protected function dataSave(array $data, $type) {
    foreach ($data as $id => $datum) {
      $ignore = array_flip(array('code', 'values', 'value'));
      $fields = array(
        'pid' => $this->getProfile()->getPid(),
        'type' => $type,
        'id' => $id,
        'code' => isset($datum['code']) ? $datum['code'] : '',
      );

      foreach ($datum['values'] as $id_language => $values) {
      	$value = reset($values);
        $fields['id_language'] = $id_language;
        $fields['title'] = isset($value['value']) ? $value['value'] : '';
        $fields['data'] = serialize(array_diff_key(array_merge($datum, $value), $ignore));

        db_merge($this::DB_CONF_TABLE)
          ->insertFields($fields)
          ->updateFields(array(
            'code' => $fields['code'],
            'title' => $fields['title'],
            'data' => $fields['data'],
          ))
          ->key(array(
            'pid' => $this->getProfile()->getPid(),
            'type' => $fields['type'],
            'id' => $fields['id'],
            'id_language' => $fields['id_language'],
          ))
          ->execute();

        $this->incrementCounter('insert', $type);
      }
    }
  }

  /**
   * @inheritdoc
   */
  protected function updateImportDate() {
    $date = date('Y-m-d H:i:s');

    if (variable_get('maps_import_differential')) {
      variable_set('maps_import:configuration_diff:' . $this->getProfile()->getPid(), $date);
    }
    else {
      variable_set('maps_import:configuration_full:' . $this->getProfile()->getPid(), $date);
      variable_set('maps_import:configuration_diff:' . $this->getProfile()->getPid(), $date);
    }
  }

}

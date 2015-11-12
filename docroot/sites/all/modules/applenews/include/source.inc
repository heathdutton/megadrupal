<?php

/**
 * @file
 * Source classes.
 */

/**
 * Interface for an Apple News source.
 */
interface ApplenewsSourceInterface {

  /**
   * Administrative label.
   */
  static public function label();

  /**
   * Implementation-specific types to limit application to.
   *
   * @see ApplenewsDestination::getSourceOptions()
   */
  static public function types();

  /**
   * Defines available attributes for the source.
   *
   * Note attributes must be alphanumeric, as export() may return a single value
   * (an associative array with these attributes as keys), or a list of values
   * (an array of such associative arrays).
   *
   * @return array
   *   Associative array of attributes. Values are either:
   *   - String label, if attribute value is provided by this class.
   *   - Array, if attribute value(s) are supplied by another source, with keys:
   *     - label - as above.
   *     - type - a value as returned by types().
   *
   * @see ApplenewsDestination::getSourceOptions()
   */
  static public function attributes();

  /**
   * Dumps data from the source.
   *
   * @return array
   *   Associative array (or a list of such) with keys defined in attributes().
   */
  public function export();

}

/**
 * Class for exporting global/literal values.
 */
class ApplenewsSourceValue implements ApplenewsSourceInterface {

  /**
   * {@inheritdoc}
   */
  static public function types() {
    return [];
  }

  /**
   * {@inheritdoc}
   */
  static public function label() {
    return t('Value');
  }

  /**
   * {@inheritdoc}
   */
  static public function attributes() {
    return [
      'value' => 'Value',
    ];
  }

  /**
   * The value.
   *
   * @var string
   */
  public $value;

  /**
   * Implements __construct().
   *
   * @param string $value
   *   Value literal.
   */
  public function __construct($value) {
    $this->value = $value;
  }

  /**
   * {@inheritdoc}
   */
  public function export() {
    return [
      'value' => $this->value,
    ];
  }

}

/**
 * Base class for exporting a Drupal entity.
 */
abstract class ApplenewsSourceEntity implements ApplenewsSourceInterface {

  /**
   * {@inheritdoc}
   */
  static public function label() {
    return t('Entity');
  }

  /**
   * {@inheritdoc}
   */
  static public function attributes() {
    return [
      'identifier' => 'Identifier',
      'label' => 'Label',
      'bundle' => 'Bundle',
      'language' => 'Language',
    ];
  }

  /**
   * Entity type.
   *
   * @var string
   */
  public $entityType;

  /**
   * Metadata wrapper for the entity.
   *
   * @var EntityDrupalWrapper
   */
  public $wrapper;

  /**
   * Implements __construct().
   *
   * @param object $entity
   *   Entity object.
   */
  public function __construct($entity) {
    $this->wrapper = entity_metadata_wrapper($this->entityType, $entity);
  }

  /**
   * {@inheritdoc}
   */
  public function export() {
    return [
      'identifier' => $this->wrapper->getIdentifier(),
      'label' => $this->wrapper->label(),
      'bundle' => $this->wrapper->getBundle(),
      'language' => $this->wrapper->getPropertyLanguage(),
    ];
  }

}

/**
 * Exports a Drupal node.
 */
class ApplenewsSourceNode extends ApplenewsSourceEntity {

  /**
   * {@inheritdoc}
   */
  static public function label() {
    return t('Node');
  }

  /**
   * {@inheritdoc}
   */
  static public function types() {
    return ['node'];
  }

  /**
   * {@inheritdoc}
   */
  static public function attributes() {
    return [
      'identifier' => 'NID',
      'label' => 'Title',
      'bundle' => 'Content Type',
      'author' => [
        'label' => 'Author',
        'type' => 'user',
      ],
      'created' => 'Date Created',
      'changed' => 'Date Changed',
      'published' => 'Published',
      'url' => 'URL',
    ] + parent::attributes();
  }

  /**
   * {@inheritdoc}
   */
  public function __construct($entity) {
    $this->entityType = 'node';
    parent::__construct($entity);
  }

  /**
   * {@inheritdoc}
   */
  public function export() {
    $values = parent::export();
    foreach ([
        'author',
        'created',
        'changed',
        'published',
        'url',
      ] as $key
    ) {
      $values[$key]
        = $this->wrapper->__isset($key) ? $this->wrapper->$key->value() : NULL;
    }
    return $values;
  }

}

/**
 * Exports a Drupal node.
 */
class ApplenewsSourceUser extends ApplenewsSourceEntity {

  /**
   * {@inheritdoc}
   */
  static public function label() {
    return t('User');
  }

  /**
   * Entity types this class applies to.
   */
  static public function types() {
    return array('user');
  }

  /**
   * {@inheritdoc}
   */
  static public function attributes() {
    return [
      'identifier' => 'UID',
      'label' => 'Name',
    ] + parent::attributes();
  }

  /**
   * {@inheritdoc}
   */
  public function __construct($entity) {
    $this->entityType = 'user';
    parent::__construct($entity);
  }

}

/**
 * Exports a Drupal field value.
 *
 * All field source classes need to extend this class.
 */
class ApplenewsSourceField implements ApplenewsSourceInterface {

  /**
   * {@inheritdoc}
   */
  static public function label() {
    return t('Field');
  }

  /**
   * {@inheritdoc}
   */
  static public function attributes() {
    return [
      'value_sanitized' => 'Value',
      'value' => 'Raw value (as entered by user, not sanitized)',
      'value_decoded' => 'Decoded value (tags stripped and entities decoded)',
    ];
  }

  /**
   * Field types this class applies to.
   */
  static public function types() {
    return [
      'text',
      'text_long',
      'text_with_summary',
      'value',
      'list',
      'list_boolean',
      'list_integer',
      'list_float',
      'list_text',
      'number_integer',
      'number_decimal',
      'number_float',
    ];
  }

  /**
   * Entity type.
   *
   * @var string
   */
  public $entityType;

  /**
   * Metadata wrapper for the entity.
   *
   * @var EntityDrupalWrapper
   */
  public $wrapper;

  /**
   * Field key.
   *
   * @var string
   */
  public $fieldName;

  /**
   * Implements __construct().
   *
   * @param string $entity_type
   *   Parent entity type.
   * @param string $field_name
   *   Entity key.
   * @param object $entity
   *   Parent entity object.
   */
  public function __construct($entity_type, $field_name, $entity) {
    $this->entityType = $entity_type;
    $this->wrapper = entity_metadata_wrapper($this->entityType, $entity);
    $this->fieldName = $field_name;
  }

  /**
   * {@inheritdoc}
   */
  public function export($value = NULL) {
    $return = [
      'value' => '',
      'value_sanitized' => '',
      'value_decoded' => '',
    ];
    if (!isset($value)) {
      try {
        $value = $this->wrapper->{$this->fieldName};
      }
      catch (EntityMetadataWrapperException $e) {
        return $return;
      }
    }
    if ($value instanceof EntityListWrapper) {
      /** @var EntityListWrapper $value */
      $return = array();
      foreach ($value as $v) {
        $return[] = $this->export($v);
      }
    }
    else {
      if ($value instanceof EntityValueWrapper) {
        /** @var EntityValueWrapper $value */
        $return['value'] = $value->raw();
        $return['value_sanitized'] = $value->value(array('sanitize' => TRUE));
        $return['value_decoded'] = $value->value(array('decode' => TRUE));
      }
      else {
        /** @var EntityStructureWrapper $value */
        try {
          $return['value'] = $value->value->raw();
        }
        catch (EntityMetadataWrapperException $e) {
          return $return;
        }
        $return['value_sanitized'] = $value->value->value(array('sanitize' => TRUE));
        $return['value_decoded'] = $value->value->value(array('decode' => TRUE));
      }
    }
    return $return;
  }

}

/**
 * Exports a Drupal file.
 */
class ApplenewsSourceFieldFile extends ApplenewsSourceField {

  /**
   * {@inheritdoc}
   */
  static public function label() {
    return t('File');
  }

  /**
   * {@inheritdoc}
   */
  static public function attributes() {
    return [
      'uri' => 'URI',
      'url' => 'URL',
      'fid' => 'File ID',
      'filename' => 'File Name',
      'filesize' => 'File Size',
      'status' => 'Status',
      'filemime' => 'File Mime Type',
      'timestamp' => 'Timestamp',
      'alt' => 'Alt Text',
      'title' => 'Title Text',
      'width' => 'Width',
      'height' => 'Height',
      'owner' => [
        'label' => 'File Owner',
        'type' => 'user',
      ],
    ];
  }

  /**
   * Field types this class applies to.
   */
  static public function types() {
    return [
      'file',
      'image',
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function export($value = NULL) {
    $return = [
      'uri' => '',
      'url' => '',
      'fid' => '',
      'filename' => '',
      'filesize' => '',
      'status' => '',
      'filemime' => '',
      'timestamp' => '',
      'alt' => '',
      'title' => '',
      'width' => '',
      'height' => '',
      'owner' => '',
    ];
    if (!isset($value)) {
      try {
        $value = $this->wrapper->{$this->fieldName};
      }
      catch (EntityMetadataWrapperException $e) {
        return $return;
      }
    }
    if ($value instanceof EntityListWrapper) {
      /** @var EntityListWrapper $value */
      $return = array();
      foreach ($value as $v) {
        $return[] = $this->export($v);
      }
    }
    else {
      $file = $value->value();
      if (!empty($file['fid'])) {
        $obj = file_load($file['fid']);
        $return = [
          'uri'       => $obj->uri,
          'url'       => file_create_url($obj->uri),
          'fid'       => $obj->fid,
          'filename'  => $obj->filename,
          'filesize'  => $obj->filesize,
          'status'    => $obj->status,
          'filemime'  => $obj->filemime,
          'timestamp' => $obj->timestamp,
          'alt'       => isset($file['alt']) ? $file['alt'] : NULL,
          'title'     => isset($file['title']) ? $file['title'] : NULL,
          'width'     => isset($file['width']) ? $file['width'] : NULL,
          'height'    => isset($file['height']) ? $file['height'] : NULL,
          'owner'     => $value->file->owner->value(),
        ];
      }
    }
    return $return;
  }

}

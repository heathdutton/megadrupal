<?php

/**
 * @file
 * changed_fields.core.inc file.
 */

/**
 * Interface CFSubjectInterface.
 */
interface CFSubjectInterface {

  /**
   * Add observer to list.
   *
   * @param CFObserverInterface $observer
   */
  public function addObserver(CFObserverInterface $observer);

  /**
   * Remove observer from list.
   *
   * @param CFObserverInterface $observer
   */
  public function removeObserver(CFObserverInterface $observer);

  /**
   *  Notify all registered observers if needed.
   */
  public function checkNodeFields();

  /**
   * Returns node object.
   */
  public function getNode();

  /**
   * Returns changed fields.
   */
  public function getChangedFields();

}

/**
 * Interface CFObserverInterface.
 */
interface CFObserverInterface {

  /**
   * Update method with info about event.
   *
   * It is called by CFNodeSubject when node fields have been changed.
   *
   * @param CFSubjectInterface $nodeSubject
   */
  public function update(CFSubjectInterface $nodeSubject);

  /**
   * Method to get observer id.
   */
  public function getId();

}

/**
 * Class CFObserver.
 */
abstract class CFObserver implements CFObserverInterface {

  /**
   * @var string
   */
  private $id;

  /**
   * @param $id
   */
  public function __construct($id) {
    $this->id = $id;
  }

  /**
   * {@inheritdoc}
   */
  abstract public function update(CFSubjectInterface $nodeSubject);

  /**
   * {@inheritdoc}
   */
  public function getId() {
    return $this->id;
  }

}

/**
 * Class CFNodeSubject.
 */
class CFNodeSubject implements CFSubjectInterface {

  /**
   * @var stdClass
   */
  private $node;

  /**
   * @var array
   */
  private $info;

  /**
   * @var array
   */
  private $changedFields;

  /**
   * @var CFDefaultFieldComparator
   */
  private $fieldComparator;

  /**
   * @var array
   */
  private $observers;

  /**
   * @param stdClass $node
   * @param array $info
   * @param CFDefaultFieldComparator $fieldComparator
   */
  public function __construct(stdClass $node, array $info, CFDefaultFieldComparator $fieldComparator) {
    $this->node = $node;
    $this->info = $info;
    $this->changedFields = array();
    $this->fieldComparator = $fieldComparator;
  }

  /**
   * {@inheritdoc}
   */
  public function addObserver(CFObserverInterface $observer) {
    $this->observers[$observer->getId()] = $observer;
  }

  /**
   * {@inheritdoc}
   */
  public function removeObserver(CFObserverInterface $observer) {
    unset($this->observers[$observer->getId()]);
  }

  /**
   * {@inheritdoc}
   */
  public function checkNodeFields() {
    foreach ($this->info as $nodeType => $fields) {
      if (isset($this->node->original) && $this->node->type == $nodeType) {
        $changedFields = array();

        foreach ($fields as $fieldName) {
          if ($fieldName == 'title') {
            $oldValue = $this->node->original->$fieldName;
            $newValue = $this->node->$fieldName;
            $fieldInfo = array('type' => 'title');
          }
          else {
            $oldValue = field_get_items('node', $this->node->original, $fieldName);
            $newValue = field_get_items('node', $this->node, $fieldName);
            $fieldInfo = field_info_field($fieldName);
          }

          $result = $this->fieldComparator->runFieldComparison($fieldInfo, $oldValue, $newValue);

          if (is_array($result)) {
            $changedFields[$fieldName] = $result;
          }
        }

        if (!empty($changedFields)) {
          $this->changedFields = $changedFields;

          foreach ($this->observers as $observer) {
            $observer->update($this);
          }
        }
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getNode() {
    return $this->node;
  }

  /**
   * {@inheritdoc}
   */
  public function getChangedFields() {
    return $this->changedFields;
  }

}

/**
 * Class CFDefaultFieldComparator.
 */
class CFDefaultFieldComparator {

  /**
   * Method that runs comparison of field values.
   *
   * @param array $fieldInfo
   * @param $oldValue
   * @param $newValue
   * @return array|bool
   */
  public function runFieldComparison(array $fieldInfo, $oldValue, $newValue) {
    $similarFields = TRUE;

    if  ($fieldInfo['type'] == 'field_collection') {
      // If collection was added or removed then we have already
      // different collections.
      if ((!$oldValue && $newValue) || ($oldValue && !$newValue)) {
        $similarFields = $this->makeResultArray($fieldInfo['type'], $oldValue, $newValue);
      } else {
        if ($oldValue && $newValue) {
          // If value was added|removed to|from multi-value field then we have
          // already different values.
          if (count($newValue) != count($oldValue)) {
            $similarFields = $this->makeResultArray($fieldInfo['type'], $oldValue, $newValue);
          } else {
            foreach ($oldValue as $key => $fc) {
              if (is_array($similarFields)) {
                break;
              }

              $oldFc = entity_load('field_collection_item', array($fc['value']));
              $oldFc = reset($oldFc);
              $newFc = $newValue[$key]['entity'];
              $fcFields = field_info_instances('field_collection_item', $fieldInfo['field_name']);

              foreach ($fcFields as $fcFieldName => $fcFieldData) {
                $fcFieldData = field_info_field($fcFieldName);
                $oldFcFieldValue = field_get_items('field_collection_item', $oldFc, $fcFieldName);
                $newFcFieldValue = field_get_items('field_collection_item', $newFc, $fcFieldName);
                $similarFields = $this->runFieldComparison($fcFieldData, $oldFcFieldValue, $newFcFieldValue);

                // If changes have been detected.
                if (is_array($similarFields)) {
                  // Make result array with old and new
                  // field collection entities.
                  $similarFields = $this->makeResultArray($fieldInfo['type'], $oldValue, $newValue);
                  break;
                }
              }
            }
          }
        }
      }
    }
    else {
      $similarFields = $this->compareFieldValues($fieldInfo['type'], $oldValue, $newValue);
    }

    return $similarFields;
  }

  /**
   * Method that returns comparable properties for existing field type.
   *
   * @param $fieldType
   * @return array
   */
  private function getComparableProperties($fieldType) {
    switch ($fieldType) {
      case 'text_with_summary':
        $properties = array(
          'value',
          'summary',
        );
        break;

      case 'text':
      case 'text_long':
      case 'number_decimal':
      case 'number_float':
      case 'number_integer':
      case 'list_float':
      case 'list_integer':
      case 'list_boolean':
      case 'list_text':
      case 'phone':
        $properties = array('value');
        break;

      case 'taxonomy_term_reference':
        $properties = array('tid');
        break;

      case 'entityreference':
        $properties = array('target_id');
        break;

      case 'image':
        $properties = array(
          'fid',
          'width',
          'height',
        );
        break;

      case 'file':
        $properties = array('fid');
        break;

      case 'date':
      case 'datetime':
      case 'datestamp':
        $properties = array(
          'value',
          'timezone',
        );
        break;

      case 'email':
        $properties = array('email');
        break;

      case 'link_field':
        $properties = array(
          'url',
          'title',
        );
        break;

      default:
        $properties = $this->getDefaultComparableProperties($fieldType);
        break;
    }

    return $properties;
  }

  /**
   * Method that returns comparable properties for extra or custom field type.
   *
   * Use it if you want to add comparison support
   * for extra or custom field types.
   *
   * @param $fieldType
   * @return array
   */
  protected function getDefaultComparableProperties($fieldType) {
    return array();
  }

  /**
   * Method that compares old and new field values.
   *
   * @param $fieldType
   * @param $oldValue
   * @param $newValue
   * @return array|bool
   */
  private function compareFieldValues($fieldType, $oldValue, $newValue) {
    $result = TRUE;
    $properties = $this->getComparableProperties($fieldType);

    // If value was added or removed then we have already different values.
    if ((!$oldValue && $newValue) || ($oldValue && !$newValue)) {
      $result = $this->makeResultArray($fieldType, $oldValue, $newValue);
    }
    else {
      if ($oldValue && $newValue) {
        // Simple comparison (for title).
        if (empty($properties)) {
          if ($newValue != $oldValue) {
            $result = $this->makeResultArray($fieldType, $oldValue, $newValue);
          }
        }
        // Compare field value properties.
        else {
          // If value was added|removed to|from multi-value field then we have
          // already different values.
          if (count($newValue) != count($oldValue)) {
            $result = $this->makeResultArray($fieldType, $oldValue, $newValue);
          }
          else {
            // Walk through each field value and compare it's properties.
            foreach ($newValue as $key => $value) {
              if (is_array($result)) {
                break;
              }

              foreach ($properties as $property) {
                if ($newValue[$key][$property] != $oldValue[$key][$property]) {
                  $result = $this->makeResultArray($fieldType, $oldValue, $newValue);
                  break;
                }
              }
            }
          }
        }
      }
    }

    return $result;
  }

  /**
   * Method that generates result array for CFDefaultFieldComparator::compareFieldValues().
   *
   * @param $fieldType
   * @param $oldValue
   * @param $newValue
   * @return array
   */
  private function makeResultArray($fieldType, $oldValue, $newValue) {
    // Return field collection item entities like field values for 'field_collection' field type.
    if ($fieldType == 'field_collection') {
      $resultOldValue = FALSE;
      $resultNewValue = FALSE;

      if ($oldValue) {
        foreach ($oldValue as $key => $fc) {
          $oldFc = entity_load('field_collection_item', array($fc['value']));
          $oldFc = reset($oldFc);
          $resultOldValue[] = $oldFc;
        }
      }

      if ($newValue) {
        foreach ($newValue as $key => $fc) {
          $resultNewValue[] = $fc['entity'];
        }
      }
    }
    else {
      $resultOldValue = $oldValue;
      $resultNewValue = $newValue;
    }

    return array(
      'old_value' => $resultOldValue,
      'new_value' => $resultNewValue,
    );
  }

}

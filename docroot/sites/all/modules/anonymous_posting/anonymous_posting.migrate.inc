<?php

/**
 * @file
 * Support for migrate module.
 *
 * With Migrate 2.4 or later, you can use the subfield syntax to set the mail,
 * homepage and hostname:
 *
 * @code
 * $this->addFieldMapping('field_anonymous_author', 'source_author_name');
 * $this->addFieldMapping('field_anonymous_author:mail', 'source_author_mail');
 * $this->addFieldMapping('field_anonymous_author:homepage', 'source_author_homepage');
 * $this->addFieldMapping('field_anonymous_author:hostname', 'source_author_hostname');
 * @endcode
 *
 * With earlier versions of Migrate, you must pass an arguments array:
 *
 * @code
 * $anonymous_author_args = array(
 *   'mail' => array('source_field' => 'source_author_mail'),
 *   'homepage' => array('source_field' => 'source_author_homepage'),
 *   'hostname' => array('source_field' => 'source_author_hostname'),
 * );
 * $this->addFieldMapping('field_anonymous_author', 'source_author_name')
 *      ->arguments($anonymous_author_args);
 * @endcode
 */

/**
 * Implements hook_migrate_api().
 */
function anonymous_posting_migrate_api() {
  return array(
    'api' => 2,
    'field handlers' => array('MigrateAnonymousAuthorFieldHandler'),
  );
}

class MigrateAnonymousAuthorFieldHandler extends MigrateFieldHandler {
  public function __construct() {
    $this->registerTypes(array('anonymous_author'));
  }

  static function arguments($mail = NULL, $homepage = NULL, $hostname = NULL) {
    $arguments = array();
    if (!is_null($mail)) {
      $arguments['mail'] = $mail;
    }
    if (!is_null($homepage)) {
      $arguments['homepage'] = $homepage;
    }
    if (!is_null($hostname)) {
      $arguments['hostname'] = $hostname;
    }
    return $arguments;
  }

  /**
   * Implementation of MigrateFieldHandler::fields().
   *
   * @param $type
   *  The field type.
   * @param $instance
   *  Instance info for the field.
   * @param Migration $migration
   *  The migration context for the parent field. We can look at the mappings
   *  and determine which subfields are relevant.
   * @return array
   */
  public function fields($type, $instance, $migration = NULL) {
    return array(
      'mail' => t('Subfield: The anonymous author email address'),
      'homepage' => t('Subfield: The anonymous author homepage'),
      'hostname' => t('Subfield: The anonymous author hostname'),
    );
  }

  public function prepare($entity, array $field_info, array $instance, array $values) {
    if (isset($values['arguments'])) {
      $arguments = $values['arguments'];
      unset($values['arguments']);
    }
    else {
      $arguments = array();
    }

    $language = $this->getFieldLanguage($entity, $field_info, $arguments);
    $values = array_filter($values);

    foreach ($values as $delta => $value) {
      $item = array();
      if (isset($arguments['mail'])) {
        $item['mail'] = $arguments['mail'];
      }
      if (isset($arguments['homepage'])) {
        $item['homepage'] = $arguments['homepage'];
      }
      if (isset($arguments['hostname'])) {
        $item['hostname'] = $arguments['hostname'];
      }
      $item['name'] = $value;
      $return[$language][$delta] = $item;
    }

    return isset($return) ? $return : NULL;
  }
}

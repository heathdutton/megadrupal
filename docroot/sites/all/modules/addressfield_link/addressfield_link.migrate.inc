<?php
/**
 * Implements hook_migrate_api().
 */
/**
 * WARNING!
 * Migrate integration is untested. What you see here is based on the
 * addressfield_phone module, from which addressfield_link was forked.
 * Uncomment and use at your own risk.
 *
function addressfield_link_migrate_api() {
  $api = array(
    'api' => 2,
    'field handlers' => array('MigrateAddressLinkFieldHandler'),
  );
  return $api;
}
//*/

/**
 * A Field Handler class that makes address_link subfield destination for migate api.
 *
 * Arguments are used to specify all link related values:
 *   'link_title',
 *   'link_url',
 */
class MigrateAddressLinkFieldHandler extends MigrateAddressFieldHandler {
  public function __construct() {
    parent::__construct();
  }
  /**
   * Provide subfields for the addressfield columns.
   */
  public function fields() {
    // Declare our arguments to also be available as subfields.
    $fields = array(
      'link_title' => t('Link title'),
      'link_url' => t('Link URL'),
    );
    return $fields;
  }

  /**
   * Implements MigrateFieldHandler::prepare().
   *
   * @param $entity
   * @param array $field_info
   * @param array $instance
    * @param array $values
   *
   * @return null
   */
  public function prepare($entity, array $field_info, array $instance, array $values) {
    $arguments = array();
    if (isset($values['arguments'])) {
      $arguments = array_filter($values['arguments']);
      unset($values['arguments']);
    }
    $language = $this->getFieldLanguage($entity, $field_info, $arguments);

    return isset($return) ? $return : NULL;
  }
}

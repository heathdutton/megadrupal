<?php
/**
 * @file
 * Subscription entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class SubsMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    // Complement type property.
    $properties['type'] = array(
      'type' => 'subs_type',
      'setter permission' => 'administer subs',
      'required' => TRUE,
      'description' => t('The subscription type.'),
    ) + $properties['type'];

    // Replace uid property with an entity relationship.
    $properties['uid'] = array(
      'label' => t('User'),
      'type' => 'user',
      'description' => t('The user id of the subscribed user.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer subs',
      'required' => TRUE,
      'schema field' => 'uid',
    );

    // Complement created property info.
    $properties['created']['type'] = 'date';
    $properties['created']['description'] = t('The date the subscription was posted.');
    $properties['created']['setter permission'] = 'administer subs';

    // Complement changed property info.
    $properties['changed']['type'] = 'date';
    $properties['changed']['description'] = t('The date the subscription was most recently updated.');

    // Complement date_start property info.
    $properties['date_start']['label'] = t('Start Date');
    $properties['date_start']['type'] = 'date';
    $properties['date_start']['description'] = t('The date the subscription started.');

    // Complement date_end property info.
    $properties['date_end']['label'] = t('End Date');
    $properties['date_end']['type'] = 'date';
    $properties['date_end']['description'] = t('The date the subscription ends.');

    // Complement status property info.
    $properties['status']['description'] = t("The subscription's status.");
    $properties['status']['options list'] = 'subs_status_options_list';
    $properties['status']['setter callback'] = 'entity_property_verbatim_set';
    $properties['status']['access callback'] = 'administer subs';
    $properties['status']['required'] = TRUE;

    return $info;
  }
}

/**
 * Implements hook_entity_property_info_alter().
 */
function subs_entity_property_info_alter(&$info) {
  // Add related subscriptions to the user object.
  $properties = &$info['user']['properties'];

  foreach (subs_get_types() as $type_name => $type) {
    $properties['subs_' . $type_name] = array(
      'type' => 'subs',
      'label' => t('@type_name subscriptions', array('@type_name' => $type->label)),
      'description' => t("The users's @type_name subscriptions.", array('@type_name' => $type->label)),
      'getter callback' => 'subs_user_get_properties',
      'bundle' => $type_name,
    );
  }
}

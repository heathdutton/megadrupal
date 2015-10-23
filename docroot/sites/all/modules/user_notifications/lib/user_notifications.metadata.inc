<?php

/**
 * @see hook_entity_property_info()
 */
class UserNotificationsMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['user_notifications_id'] = array(
      'label' => t("User Notification ID"),
      'type' => 'integer',
      'description' => t("The unique ID of the user notification."),
      'schema field' => 'user_registrations_id',
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['type'] = array(
      'label' => t("User Notification type"),
      'type' => 'token',
      'description' => t("The type of the user notification."),
      'options list' => 'user_notifcations_type_get_names',
      'required' => TRUE,
      'schema field' => 'type',
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['created'] = array(
      'label' => t("Date created"),
      'type' => 'date',
      'schema field' => 'created',
      'description' => t("The date the registration was created."),
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['updated'] = array(
      'label' => t("Date updated"),
      'type' => 'date',
      'schema field' => 'updated',
      'description' => t("The date the registration was most recently updated."),
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['recipient'] = array(
      'label' => t("Recipient entity"),
      'type' => 'user',
      'schema field' => 'recipient_uid',
      'description' => t("The user who is receiving the notification."),
      'getter callback' => 'user_notifications_property_get_recipient',
      'setter callback' => 'entity_property_verbatim_set',
    );

    $properties['author'] = array(
      'label' => t("User entity"),
      'type' => 'entity',
      'schema field' => 'author_uid',
      'description' => t("The entity for which the user notification belongs to. Usually a user."),
      'getter callback' => 'user_notifications__property_user_get',
      'setter callback' => 'user_notifications__property_user_set',
    );

    $properties['state'] = array(
      'label' => t("State entity"),
      'type' => 'user_notifications_state',
      'schema field' => 'state',
      'options list' => 'user_notifications_get_states_options',
      'description' => t("The state of the user notification."),
      'setter callback' => 'entity_property_verbatim_set',
    );
    return $info;
  }

}

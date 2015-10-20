<?php

/**
 * @file
 * Entity hooks and callbacks for registrations.
 */

/**
 * Implements hook_entity_info().
 */
function node_registration_entity_info() {
  $module_path = _node_registration_type_to_uri('node_registration');

  $bundles = array();

  // Node types + specific nodes.
  foreach (_node_registration_bundles() as $type => $name) {
    $bundles[$type] = array(
      'label' => $name . ' registration',
      'name' => $name,
      'type' => $type,
      'admin' => array(
        'path' => 'admin/structure/' . $module_path . '/manage/%node_registration_type',
        'real path' => 'admin/structure/' . $module_path . '/manage/' . _node_registration_type_to_uri($type),
        'bundle argument' => 4,
        'access arguments' => array('administer node registration'),
      ),
    );
  }

  return array(
    'node_registration' => array(
      'module' => 'node_registration',
      'label' => t('Node registration'),
      'controller class' => 'NodeRegistrationController',
      'entity class' => 'NodeRegistrationEntityClass',
      'base table' => 'node_registration',
      'fieldable' => TRUE,
      'entity keys' => array(
        'id' => 'registration_id',
        'bundle' => 'type',
      ),
      'bundle keys' => array(
        'bundle' => 'type',
      ),
      'bundles' => $bundles,
      'view modes' => array(
        'full' => array(
          'label' => t('Full Registration'),
          'custom settings' => FALSE,
        ),
        'simple' => array(
          'label' => t('Simple Registration'),
          'custom settings' => FALSE,
        ),
      ),
      'uri callback' => 'node_registration_uri',
      'label callback' => 'node_registration_label',
      'token type' => 'node-registration',
      'access callback' => 'node_registration_access_proxy',
    ),
  );
}

/**
 * Access callback proxy for entity_access(), which has a different argument order.
 */
function node_registration_access_proxy($op, $registration, $account) {
  return node_registration_access($registration, $op, $account);
}

/**
 * Retrieves a node registration bundle by its name.
 */
function node_registration_type_load($bundle_name) {
  $bundles = field_info_bundles('node_registration');

  $bundle_name = _node_registration_uri_to_type($bundle_name);
  if (isset($bundles[$bundle_name])) {
    return (object) $bundles[$bundle_name];
  }

  return FALSE;
}

/**
 * Page title callback for the Node registration type (admin) pages.
 */
function node_registration_type_page_title($bundle) {
  return $bundle['label'];
}

/**
 * URI callback for the Node registration type.
 */
function node_registration_uri($registration) {
  $registration_id = is_scalar($registration) ? (int) $registration : (int) $registration->registration_id;
  return array(
    'path' => 'registration/' . $registration_id,
  );
}

/**
 * Label callback for the Node registration type.
 */
function node_registration_label($registration) {
  $label = $registration->email;
  drupal_alter('node_registration_label', $label, $registration);
  return $label;
}

/**
 * Controller class Node Registrations
 */
class NodeRegistrationController extends EntityAPIController {

  /**
   * Override create() to add registration properties.
   */
  public function create(array $values = array()) {
    if (!isset($values['created'])) {
      $values['created'] = REQUEST_TIME;
    }
    if (!isset($values['updated'])) {
      $values['updated'] = $values['created'];
    }

    if (isset($values['node'])) {
      $node = $values['node'];
    }
    else {
      $node = node_load($values['nid']);
      $values['node'] = $node;
    }

    // The dynamic type: fields per registration type or per node.
    $values['type'] = !empty($node->registration->private_fields) ? 'node_' . $node->nid : $node->type;
    // The node type.
    $values['node_type'] = $node->type;

    $entity = parent::create($values);
    return $entity;
  }

  /**
   * Override save() to add automatic base column values from other base column values.
   */
  public function save($registration) {
    // Add `type` and `node_type` from `node` or `nid`.
    if (empty($registration->type) || empty($registration->node_type)) {
      $node = NULL;
      if (!empty($registration->node)) {
        $node = $registration->node;
        $registration->nid = $node->nid;
      }
      elseif (!empty($registration->nid)) {
        $node = node_load($registration->nid);
        $registration->node = $node;
      }

      if ($node) {
        if (empty($registration->type)) {
          $registration->type = !empty($node->registration->private_fields) ? 'node_' . $node->nid : $node->type;
        }
        if (empty($registration->node_type)) {
          $registration->node_type = $node->type;
        }
      }
    }

    // Add `weight`.
    if ((!isset($registration->weight) || $registration->weight === '') && !empty($registration->nid)) {
      $weight = db_query('SELECT MAX(weight) FROM {node_registration} WHERE nid = ?', array($registration->nid))->fetchField();
      $registration->weight = $weight === NULL ? 0 : $weight + 1;
    }

    // Add `author_uid` from session user.
    if (!isset($registration->author_uid)) {
      global $user;
      $registration->author_uid = $user->uid;
    }

    // Add `uid` from `user_name`.
    if (!isset($registration->uid) && !empty($registration->user_name)) {
      static $user_name_uid_map = array();
      $name = trim(drupal_strtolower($registration->user_name));
      if (!isset($user_name_uid_map[$name])) {
        $user_name_uid_map[$name] = db_query('SELECT uid FROM {users} WHERE name = ?', array($name))->fetchField() ?: 0;
      }
      $registration->uid = $user_name_uid_map[$name];
    }

    // Add `uid` from `user_mail`.
    if (!isset($registration->uid) && !empty($registration->user_mail)) {
      static $user_mail_uid_map = array();
      $mail = trim(drupal_strtolower($registration->user_mail));
      if (!isset($user_mail_uid_map[$mail])) {
        $user_mail_uid_map[$mail] = db_query('SELECT uid FROM {users} WHERE mail = ?', array($mail))->fetchField() ?: 0;
      }
      $registration->uid = $user_mail_uid_map[$mail];
    }

    // Add `email` from `uid`.
    if (empty($registration->email) && !empty($registration->uid)) {
      static $uid_email_map = array();
      $uid = (int) trim($registration->uid);
      if (!isset($uid_email_map[$uid])) {
        $uid_email_map[$uid] = db_query('SELECT mail FROM {users} WHERE uid = ?', array($uid))->fetchField() ?: 0;
      }
      $registration->email = $uid_email_map[$uid];
    }

    return parent::save($registration);
  }

}

/**
 * Entity class for Node Registrations
 */
class NodeRegistrationEntityClass extends Entity {

  public $nid;
  public $node;
  public $is_new = 0;
  public $registration_id = 0;
  public $language = LANGUAGE_NONE;
  public $slots = 1;
  public $attended = 0;
  public $verified = 0;
  public $cancelled = 0;
  public $waitinglist = 0;

  /**
   * Override buildContent() to add registration properties.
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $node = $this->node ?: node_load($this->nid);

    // Extra fields need this... @see _field_extra_fields_pre_render().
    $content = array('#view_mode' => $view_mode);

    // Cancel link.
    $uri = node_registration_uri($this);
    _node_registration_fake_field($content, 'cancel_link', t('Cancel'), array(
      '#type' => 'link',
      '#title' => t('Cancel registration'),
      '#href' => $uri['path'] . '/cancel',
      '#access' => node_registration_access($this, 'cancel'),
    ), FALSE);

    // Event node.
    $uri = node_uri($node);
    _node_registration_fake_field($content, 'event_node', t('Event node'), array(
      '#type' => 'link',
      '#title' => $node->title,
      '#href' => $uri['path'],
      '#access' => node_access('view', $node),
    ));

    // Rendered event node.
    $node_build = node_view($node, 'teaser');
    _node_registration_fake_field($content, 'event_node_content', t('Event node'), $node_build, FALSE);

    // Author.
    $author = user_load($this->author_uid ?: 0);
    _node_registration_fake_field($content, 'author', t('Author'), array(
      '#theme' => 'username',
      '#account' => $author,
    ));

    // Account.
    $account = user_load($this->uid ?: 0);
    _node_registration_fake_field($content, 'account', t('Registree'), array(
      '#theme' => 'username',
      '#account' => $account,
    ));

    // E-mail address.
    _node_registration_fake_field($content, 'email', t('E-mail address'), array(
      '#markup' => $this->email,
    ));

    // Slots.
    _node_registration_fake_field($content, 'slots', t('Slots'), array(
      '#markup' => $this->slots,
    ));

    // Attended.
    _node_registration_fake_field($content, 'attended', t('Attended'), array(
      '#markup' => $this->attended ? t('Yes') : t('No'),
    ));

    $content = entity_get_controller('node_registration')->buildContent($this, $view_mode, $langcode, $content);

    return array(
      '#theme' => 'node_registration',
      '#registration' => $this,
      '#content' => $content,
      '#node' => $node,
    );
  }

}

/**
 * Implements hook_entity_property_info_alter().
 */
function _node_registration_fake_field(&$renderable, $machine_name, $title, $content, $show_title = TRUE) {
  $content = isset($content[0]) ? array_values($content) : array($content);
  $items = array_fill(0, count($content), $machine_name);

  $renderable[$machine_name] = array(
    // These are necessary for correct display.
    '#theme' => 'field',
    '#field_name' => $machine_name,
    '#title' => $title,
    '#label_display' => $show_title ? 'above' : 'hidden',
    '#bundle' => $machine_name,
    '#items' => $items,
  ) + $content + array(
    // These are necessary for other modules' preprocessors.
    '#field_type' => 'custom',
    '#entity_type' => 'custom',
    '#bundle' => 'custom',
    '#object' => new stdClass,
  );
}

/**
 * Implements hook_entity_property_info_alter().
 */
function node_registration_entity_property_info_alter(&$info) {
  // NODE properties.
  $properties = &$info['node']['properties'];

  // Event start.
  $properties['event_start'] = array(
    'label' => t("Event start"),
    'description' => t("Event start according to node type setup."),
    'type' => 'date',
    'getter callback' => 'node_registration_metadata_entity_get_properties',
  );

  // Time left.
  $properties['time_left'] = array(
    'label' => t("Time left"),
    'description' => t("Time left according to node type setup."),
    'type' => 'text',
    'getter callback' => 'node_registration_metadata_entity_get_properties',
  );

  // Capacity.
  $properties['capacity'] = array(
    'label' => t("Capacity"),
    'description' => t("Number of slots available."),
    'type' => 'integer',
    'getter callback' => 'node_registration_metadata_entity_get_properties',
  );

  // Slots taken.
  $properties['slots_taken'] = array(
    'label' => t("Slots taken"),
    'description' => t("Number of slots taken."),
    'type' => 'integer',
    'getter callback' => 'node_registration_metadata_entity_get_properties',
  );

  // Slots left.
  $properties['slots_left'] = array(
    'label' => t("Slots left"),
    'description' => t("Number of open slots left."),
    'type' => 'integer',
    'getter callback' => 'node_registration_metadata_entity_get_properties',
  );

  // Unset pointer. I'm paranoid about PHP references.
  unset($properties);

  // REGISTRATION properties.
  $properties = &$info['node_registration']['properties'];

  // E-mail.
  $properties['email'] = array(
    'label' => t("E-mail address"),
    'description' => t("The registree's e-mail address."),
    'type' => 'text',
    'schema field' => 'email',
    'setter callback' => 'entity_property_verbatim_set',
  );

  // Type / bundle.
  $properties['type'] = array(
    'label' => t("Type"),
    'description' => t("The registration's entity bundle."),
    'type' => 'text',
    'schema field' => 'type',
  );

  // Node type.
  $properties['node_type'] = array(
    'label' => t("Node type"),
    'description' => t("The node type's machine name."),
    'type' => 'text',
    'schema field' => 'node_type',
  );

  // Slots.
  $properties['slots'] = array(
    'label' => t("Slots"),
    'description' => t("The number of slots consumed by this user."),
    'type' => 'integer',
    'schema field' => 'slots',
    'setter callback' => 'entity_property_verbatim_set',
  );

  // Created.
  $properties['created'] = array(
    'label' => t("Created"),
    'description' => t("The date the registration was created."),
    'type' => 'date',
    'schema field' => 'created',
    'setter callback' => 'entity_property_verbatim_set',
  );

  // Updated.
  $properties['updated'] = array(
    'label' => t("Updated"),
    'description' => t("The date the registration was updated."),
    'type' => 'date',
    'schema field' => 'updated',
    'setter callback' => 'entity_property_verbatim_set',
  );

  // Weight.
  $properties['weight'] = array(
    'label' => t("Weight"),
    'description' => t("The weight of this registration in this event. Used for waitinglist execution order and display."),
    'type' => 'integer',
    'schema field' => 'weight',
    'setter callback' => 'entity_property_verbatim_set',
  );

  // Node entity.
  $properties['nid'] = array('label' => t('Node nid'), 'type' => 'integer');
  $properties['node'] = array(
    'label' => t("Node"),
    'description' => t("The node this registration is associated with."),
    'type' => 'node',
    'schema field' => 'nid',
  );

  // Author user entity.
  $properties['author_uid'] = array('label' => t('Author uid'), 'type' => 'integer');
  $properties['author'] = array(
    'label' => t("Author"),
    'description' => t("The user that <strong>made</strong> this registration."),
    'type' => 'user',
    'schema field' => 'author_uid',
  );

  // Registree user entity.
  $properties['uid'] = array('label' => t('User uid'), 'type' => 'integer');
  $properties['user'] = array(
    'label' => t("User"),
    'description' => t("The user that's registered."),
    'type' => 'user',
    'schema field' => 'uid',
  );

  // Registree user name. Can be used to set `uid` indirectly (magic in ctrlr->save()).
  $properties['user_name'] = array(
    'label' => t("User name"),
    'description' => t("The user's username."),
    'type' => 'text',
    'schema field' => 'uid',
  );

  // Registree user mail. Can be used to set `uid` indirectly (magic in ctrlr->save()).
  $properties['user_mail'] = array(
    'label' => t("User mail"),
    'description' => t("The user's e-mail address."),
    'type' => 'text',
    'schema field' => 'uid',
  );

  // View URL.
  $properties['url'] = array(
    'label' => t("View URL"),
    'description' => t("The URL to view the registration."),
    'type' => 'uri',
    'getter callback' => 'node_registration_metadata_entity_get_properties',
  );

  // Edit URL.
  $properties['edit_url'] = array(
    'label' => t("Edit URL"),
    'description' => t("The URL to edit the registration."),
    'type' => 'uri',
    'getter callback' => 'node_registration_metadata_entity_get_properties',
  );

  // Cancel URL.
  $properties['cancel_url'] = array(
    'label' => t("Cancel URL"),
    'description' => t("The URL to cancel the registration."),
    'type' => 'uri',
    'getter callback' => 'node_registration_metadata_entity_get_properties',
  );

  // Booleans.
  $properties['attended'] = array(
    'label' => t("Attended"),
    'description' => t("Whether the registree attended the event."),
    'type' => 'boolean',
    'schema field' => 'attended',
    'setter callback' => 'entity_property_verbatim_set',
  );
  $properties['verified'] = array(
    'label' => t("Verified"),
    'description' => t("Whether the registration was verified."),
    'type' => 'boolean',
    'schema field' => 'verified',
    'setter callback' => 'entity_property_verbatim_set',
  );
  $properties['cancelled'] = array(
    'label' => t("Cancelled"),
    'description' => t("Whether the registration was cancelled."),
    'type' => 'boolean',
    'schema field' => 'cancelled',
    'setter callback' => 'entity_property_verbatim_set',
  );
  $properties['reminder_sent'] = array(
    'label' => t("Reminder sent"),
    'description' => t("Whether the registree was sent a reminder."),
    'type' => 'boolean',
    'schema field' => 'reminder_sent',
    'setter callback' => 'entity_property_verbatim_set',
  );
  $properties['waitinglist'] = array(
    'label' => t("Waiting list"),
    'description' => t("Whether the registration is on the waiting list."),
    'type' => 'boolean',
    'schema field' => 'waitinglist',
  );
}

/**
 * Callback for getting properties of an entity.
 */
function node_registration_metadata_entity_get_properties($entity, array $options, $name, $entity_type) {
  switch ($entity_type) {
    // REGISTRATION properties.
    case 'node_registration':
      switch ($name) {
        // User name.
        case 'user_name':
          $account = $entity->uid ? user_load($entity->uid) : NULL;
          return $account && $account->uid ? $account->name : '';

        // User mail.
        case 'user_mail':
          $account = $entity->uid ? user_load($entity->uid) : NULL;
          return $account && $account->uid ? $account->mail : '';

        // View URL.
        case 'url':
          $return = entity_uri($entity_type, $entity);
          return url($return['path'], $return['options'] + $options);

        // Edit URL.
        case 'edit_url':
          $return = entity_uri($entity_type, $entity);
          $return['path'] .= '/edit';
          if (!$entity->uid) {
            $return['options']['query'] = array('secret' => $entity->secret);
          }
          return url($return['path'], $return['options'] + $options);

        // Cancel URL.
        case 'cancel_url':
          $return = entity_uri($entity_type, $entity);
          $return['path'] .= '/cancel';
          if (!$entity->uid) {
            $return['options']['query'] = array('secret' => $entity->secret);
          }
          return url($return['path'], $return['options'] + $options);
      }
      break;

    case 'node':
      switch ($name) {
        // Event start.
        case 'event_start':
          $utc = $entity->registration->site_utc();
          return $utc;

        // Time left.
        case 'time_left':
          $utc = $entity->registration->site_utc();
          if ($utc) {
            $time_left = $utc - REQUEST_TIME;
            $time_left = format_interval($time_left);
            return $time_left;
          }

        // Capacity.
        case 'capacity':
          $capacity = $entity->registration->capacity();
          return $capacity;

        // Slots taken.
        case 'slots_taken':
          return node_registration_event_count($entity);

        // Slots left.
        case 'slots_left':
          $capacity = $entity->registration->capacity();
          return max(0, $capacity - node_registration_event_count($entity));
      }
      break;
  }
}

/**
 * Loads a registration by ID.
 */
function node_registration_load($registration_id, $reset = FALSE) {
  if (empty($registration_id)) {
    return FALSE;
  }

  $registrations = node_registration_load_multiple(array($registration_id), array(), $reset);
  return $registrations ? reset($registrations) : FALSE;
}

/**
 * Loads multiple registrations by ID or based on a set of matching conditions.
 */
function node_registration_load_multiple($registration_ids = FALSE, $conditions = array(), $reset = FALSE) {
  if (empty($registration_ids) && empty($conditions)) {
    return array();
  }

  return entity_load('node_registration', $registration_ids, $conditions, $reset);
}

/**
 * Deletes multiple registrations by ID.
 */
function node_registration_delete_multiple($registration_ids) {
  return entity_get_controller('node_registration')->delete($registration_ids);
}

/**
 * Saves a registration.
 */
function node_registration_save($registration) {
  // Updated NOW.
  $registration->updated = REQUEST_TIME;

  // Created NOW only if new.
  if (empty($registration->created)) {
    $registration->created = REQUEST_TIME;
  }

  // Do entity callback.
  return entity_get_controller('node_registration')->save($registration);
}

/**
 * Proxy for the fields widget handlers.
 */
function _node_registration_fields_callback($callback, $registration, &$form, &$form_state) {
  // Do field callback.
  return $callback('node_registration', $registration, $form, $form_state);
}

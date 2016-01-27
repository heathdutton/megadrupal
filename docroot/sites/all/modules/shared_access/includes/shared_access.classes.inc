<?php

/**
 * Interface for shared_access controller classes.
 *
 * All shared_access controller classes specified via the 'access controller'
 * key returned by hook_shared_access_realms() or
 * hook_shared_access_realms_alter() have to implement this interface.
 */
interface SharedAccessControllerInterface {

  /**
   * Constructor.
   *
   * @param $field_name
   *   The name of the shared_access field for which the instance is created.
   *
   * @param $settings
   *   The settings for which the instance is created.
   */
  public function __construct($realm);

  /**
   * Determines if access should be granted.
   *
   * @param $grant
   *   A structured array that contians the grant info to check access for:
   *     - op: The operation being checked for access.
   *     - gid: The grant id to check.
   *     - gstr: an optional grant string to check.
   *     - realm: The realm to check, this isn't really needed since the realm
   *              determines which SharedAccessController Class to use.
   *
   * @param $account
   *   An account to check the access against.
   *
   * @return
   *   TRUE if access should be granted, FALSE if it should not be.
   *
   */
  public function access($grant, $params);


  /**
   * Implements hook_theme for the controller class.
   *
   * @returns
   *   An array of theme information as defined in hook_theme().
   */
  public static function theme();

  /**
   * Callback for processing the shared access table form element.
   *
   * @param $element
   *   An associative array containing the properties and children of the
   *   shared_access item element. Note that $element must be taken by reference
   *   here, so processed child elements are taken over into $form_state.
   * @param $form_state
   *   The $form_state array for the form this shared_access item element
   *   belongs to.
   * @param $form_state
   *   The $form_state array for the form this shared_access item element
   *   belongs to.
   */
  public function sa_table_process(&$element, &$form_state);

  public function sa_table_add_rows($element);

}

/**
 * Interface for implementing shared_access controller specific settings.
 *
 * This interface should be implemented if the controller class needs to add/use
 * settings specific for its own behaviors.
 */
interface SharedAccessControllerSettingsInterface extends SharedAccessControllerInterface {

  public function field_instance_settings($field, $instance);

  public function field_settings($field, $instance, $has_data);
}

/**
 * Interface for implementing shared_access controller specific settings.
 *
 * This interface should be implemented if the controller class needs to add/use
 * settings specific for its own behaviors.
 */
interface SharedAccessControllerDefaultBehaviorInterface extends SharedAccessControllerInterface {

  public function field_attach_insert($entity_type, $entity);

  public function field_attach_update($entity_type, $entity);

  public function field_attach_delete($entity_type, $entity);

  public function field_attach_presave($entity_type, $entity);
}

/**
 * Interface for implementing shared_access controller specific settings.
 *
 * This interface should be implemented if the controller class needs to add/use
 * settings specific for its own behaviors.
 */
interface SharedAccessShareFormInterface extends SharedAccessControllerInterface{

    /**
   * Adds the elements necessary for displaying the sharing form and collecting
   * the proper information.
   *
   * @param $form
   *   The shared_access field's share form. Used to add configurations for this
   *   realm.
   *
   * @param $form_state
   *   The current state of the form.
   */
  public function share_form(&$form, &$form_state);

  /**
   * Performs the necessary processing of the submitted values before saving.
   *
   * @param $form
   *   The shared_access field's share form. Used to process configurations for
   *   this realm.
   *
   * @param $form_state
   *   The current state of the form.
   *
   * @param $items
   *   An alterable list of field items.
   */
  public function share_form_process($form, &$form_state, &$items);

  /**
   * Performs validation on the submitted values.
   *
   * @param $form
   *   The shared_access field's share form.
   *
   * @param $form_state
   *   The current state of the form.
   */
  public function share_form_validate(&$form, &$form_state);

}

/**
 * Interface for implementing shared_access controller specific settings.
 *
 * This interface should be implemented if the controller class needs to add/use
 * settings specific for its own behaviors.
 */
interface SharedAccessUniversalShareBoxInterface extends SharedAccessShareFormInterface {

  /**
   * Auto-complete callback for sharing values.
   *
   * @param $field
   *   The shared_access field.
   * @param $instance
   *   The shared_access field instance.
   * @param $entity_type
   *   The entity type.
   * @param $bundle_name
   *   The bundle name.
   * @param $string
   *   The string to match on.
   *
   * @return
   *   An array of matched suggestions.
   */
  public function share_usb_autocomplete($field, $instance, $entity_type, $bundle_name, $string, $prefix);

  /**
   * Process the tags returned by the Universal Share box and translate them
   * into field items.
   *
   * @param $op
   *   The op returned by the share form for the USB.
   * @param $items
   *   A reference to the field items that will be saved.
   * @param $realm_tags
   *   An array of tags returned by the USB and keyed by realm. The key
   *   'unprocessed' can be used to place items that can not be processed by the
   *   realm. Hopefully another realm will pick them up and use them.
   */
  public function process_usb_tags($op, &$items, &$realm_tags);
}

/**
 * Interface for implementing shared_access controller specific settings.
 *
 * This interface should be implemented if the controller class needs to add/use
 * settings specific for its own behaviors.
 */
interface SharedAccessPriorityAccessInterface extends SharedAccessControllerInterface {

  /**
   * Determines if access should be granted without inspecting field items, but
   * using the basic information from the entity and field.
   *
   * @param $params
   *   An array of information including entity and field info to check.
   *
   * @return
   *   TRUE if access should be granted, FALSE if it should not be.
   *
   */
  public function priority_access($params);
}

/**
 * Default implementation of SharedAccessControllerInterface.
 *
 * This class can not be used as-is. Modules wiching to supply SharedAccess
 * Controllers should extend this class.
 */
class BasicSharedAccessController implements SharedAccessControllerInterface {
  /**
   * Name of the field.
   *
   * @var string
   */
  protected $fieldName;

  /**
   * Name of the field.
   *
   * @var string
   */
  protected $realm;

  /**
   * Array of information about this field and the controller.
   *
   * @var array
   *
   * @see hook_shared_access_realms().
   * @see shared_access_get_realms().
   */
  protected $settings;

  /**
   * Constructor: sets basic variables.
   */
  public function __construct($realm) {
    $this->realm = $realm;
  }

  /**
   * Implements SharedAccessControllerInterface::access().
   */
  public function access($grant, $params) {
    return FALSE;
  }

  /**
   * Implements SharedAccessControllerInterface::theme().
   *
   * @returns
   *   An array of theme information as defined in hook_theme().
   */
  public static function theme() {
    // No theme info by default.
    return array();
  }

  /**
   * Implements SharedAccessControllerInterface::sa_table_process().
   *
   * Callback for processing the shared access table form element.
   *
   * @param $element
   *   An associative array containing the properties and children of the
   *   shared_access item element. Note that $element must be taken by reference
   *   here, so processed child elements are taken over into $form_state.
   * @param $form_state
   *   The $form_state array for the form this shared_access item element
   *   belongs to.
   * @param $form_state
   *   The $form_state array for the form this shared_access item element
   *   belongs to.
   */
  public function sa_table_process(&$element, &$form_state) {
    $items = $element['#items'];
    foreach ($items as $delta => $item) {
      if ($this->realm == $item['realm']) {
        $key = md5(implode('|',$item));
        $options = isset($form_state['complete form']['#field']['settings']['ops']) ? shared_access_get_field_ops($form_state['complete form']['#field']) : array();
        $default = (isset($items[$delta]['op']) && array_key_exists($items[$delta]['op'], $options)) ? $items[$delta]['op'] : '';
        $element[$key] = array(
          'op' => array(
            '#type' => 'select',
            '#title' => t('Op for shared access field item @delta', array('@delta' => $delta)),
            '#title_display' => 'invisible',
            '#options' => $options,
            '#default_value' => $default,
          ),
          'remove' => array(
            '#type' => 'checkbox',
            '#title' => 'remove',
          ),
        );
      }
    }
  }

  public function sa_table_add_rows($element) {
    return array();
  }
}

/**
 * Default implementation of SharedAccessControllerInterface.
 *
 * This class can not be used as-is. Modules wishing to supply SharedAccess
 * Controllers should extend this class.
 */
class SharedAccessFormController extends BasicSharedAccessController implements SharedAccessShareFormInterface {


  /**
   * Implements SharedAccessControllerInterface::share_form().
   */
  public function share_form(&$form, &$form_state) {
    // Default controller does nothing with the form.
  }

  /**
   * Implements SharedAccessControllerInterface::share_form_process().
   */
  public function share_form_process($form, &$form_state, &$items) {
    // The default behavior is to update the item op.
    foreach($items as $delta => $item) {
      if ($item['realm'] == $this->realm) {
        $key = md5(implode('|',$item));
        if (!empty($form_state['values']['share_table'][$key]['remove'])) {
          unset($items[$delta]);
        }
        elseif (isset($form_state['values']['share_table'][$key]) && ($form_state['values']['share_table'][$key]['op'] != $item['op'])) {
          $items[$delta]['op'] = $form_state['values']['share_table'][$key]['op'];
        }
      }
    }
  }

  /**
   * Implements SharedAccessControllerInterface::share_form_validate().
   */
  public function share_form_validate(&$form, &$form_state) {
    // Default controller does nothing with the form.
    return TRUE;
  }

  /**
   * Implements SharedAccessControllerInterface::sa_table_add_rows().
   *
   * Callback for processing the form element for a shared_access item.
   *
   * @param $element
   *   An associative array containing the properties and children of the
   *   shared_access item element. Note that $element must be taken by reference
   *   here, so processed child elements are taken over into $form_state.
   * @param $form_state
   *   The $form_state array for the form this shared_access item element
   *   belongs to.
   */
  public function sa_table_add_rows($element) {
    // The format for a row is:
    //   realm icon | item title | item element | actions
    $rows = array();
    foreach ($element['#items'] as $delta => $item) {
      if ($this->realm == $item['realm']) {
        $key = md5(implode('|',$item));
        $row = array();

        if (isset($element['#options'][$key]['#attributes'])) {
          $row += $element['#options'][$key]['#attributes'];
        }
        // Render the elements for this row item.
        $row['data'] = array(
          '', //add handling here for the icon.
          theme(
            array(
              "sa_item_title_$this->realm" . '__' . $element['#field']['field_name'] . '__' . $element['#entity_type'],
              "sa_item_title_$this->realm" . '__' . $element['#entity_type'],
            ),
            array('element' => $element, 'item' => $item, 'delta' => $delta)
          ),
          (isset($element[$key]['op'])) ? drupal_render($element[$key]['op']) : '',
          (isset($element[$key]['remove'])) ? drupal_render($element[$key]['remove']) : '', //add handling here for the actions.
        );
        $rows[] = $row;
      }
    }
    return $rows;
  }
}

/**
 * Extension of DefaultSharedAccessController to provide specific user related
 * access control.
 *
 * @see shared_access_shared_access_realms().
 */
class UserSharedAccessController extends SharedAccessFormController implements SharedAccessUniversalShareBoxInterface {

  /**
   * Implements SharedAccessControllerInterface::access().
   *
   * Determines if the provided user account has access to the entity
   * attached to this particular shared_access field.
   *
   * @param $grant
   *   A structured array that contians the grant info to check access for:
   *     - gid: The user id to check.
   */
  public function access($grant, $params) {
    if ($params['account']->uid == $grant['gid']) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Overrides DefaultSharedAccessController::share_form().
   */
  public function share_form(&$form, &$form_state) {
    // Use the universal share box. Add this controller type to the list and
    // add a hint for the placeholder attribute.
    $form['universal_share_box']['#share_types'][$this->realm] = t('names');
  }

  /**
   * Overrides DefaultSharedAccessController::share_form_autocomplete().
   *
   * Builds a list of username matches to send back to the universal share box.
   */
  public function share_usb_autocomplete($field, $instance, $entity_type, $bundle_name, $string, $prefix) {
    $matches = array();
    if ($string) {
      $or = db_or()
        ->condition('mail', '%' . db_like($string) . '%', 'LIKE')
        ->condition('name', '%' . db_like($string) . '%', 'LIKE');
      $result = db_select('users')
        ->fields('users', array('mail', 'name', 'uid'))
        ->condition($or)
        ->range(0, 10)
        ->execute();
      foreach ($result as $pseudo_account) {
        // In the simplest case (see user_autocomplete), the key and the value
        // are the same. Here we'll display the uid along with the username in
        // the dropdown.
        $matches[$prefix . $this->realm . ': ' . $pseudo_account->mail . ', '] = theme('sa_usertag', array('pseudo_account' => $pseudo_account));
      }
    }
    return $matches;
  }

  public function process_usb_tags($op, &$items, &$realm_tags) {
    if (!empty($realm_tags[$this->realm])) {
      $users = db_select('users')
        ->fields('users', array('uid', 'mail'))
        ->condition('mail', $realm_tags[$this->realm], 'IN')
        ->execute()
        ->fetchAllKeyed();
      if ($users) {
        $realm_tags['unprocessed'] += array_diff($realm_tags[$this->realm], $users);
        foreach($users as $uid => $mail) {
          $item = array(
            'realm' => $this->realm,
            'gid' => $uid,
            'gstr' => $mail,
            'op' => $op,
          );
          if (!(_shared_access_equal_items($item, $items)))
            $items[] = $item;
        }
      }
    }
  }


  /**
   * Overrides DefaultSharedAccessController::theme().
   *
   * @returns
   *   An array of theme information as defined in hook_theme().
   */
  public static function theme() {
    // Theme how user references are displayed in the usb autocomplete list.
    return array(
      'sa_usertag' => array(
        'variables' => array('pseudo_account' => null),
        'file' => 'includes/shared_access.theme.inc',
      ),
      'sa_item_title_user' => array(
        'variables' => array('element' => NULL, 'item' => NULL),
        'file' => 'includes/shared_access.theme.inc',
        'base hook' => 'sa_item_title',
        'pattern' => 'sa_item_title_user__',
      ),
    );
  }
}


if (module_exists('og')) {
 /**
  * Extension of DefaultSharedAccessController to provide specific user related
  * access control.
  *
  * @see shared_access_shared_access_realms().
  */
  class GroupSharedAccessController extends SharedAccessFormController implements SharedAccessUniversalShareBoxInterface {

    /**
    * Implements SharedAccessControllerInterface::access().
    *
    * Determines if the provided user account has access to the entity
    * attached to this particular shared_access field because they are
    * a member of an allowed group.
    *
    * @param $grant
    *   A structured array that contians the grant info to check access for:
    *     - gid: The group id to check.
    *     - gstr: The group type to check.
    */
    public function access($grant, $params) {
      return og_is_member($grant['gstr'], $grant['gid'], 'user', $params['account']);
    }

   /**
    * Overrides DefaultSharedAccessController::share_form().
    */
    public function share_form(&$form, &$form_state) {
      // Use the universal share box. Add this controller type to the list and
      // add a hint for the placeholder attribute.
      $form['universal_share_box']['#share_types'][$this->realm] = t('groups');
    }

   /**
    * Overrides DefaultSharedAccessController::share_form_autocomplete().
    *
    * Builds a list of group name matches to send back to the universal share box.
    */
    public function share_usb_autocomplete($field, $instance, $entity_type, $bundle_name, $string, $prefix) {
      $matches = array();
      if ($string) {
        // No point in looking unless an OG field has been added to any entity.
        if (field_info_field(OG_GROUP_FIELD)) {
          foreach(og_get_all_group_entity() as $entity_type => $entity_type_label) {
            $entity_info = entity_get_info($entity_type);
            if ($entity_ids = og_get_all_group($entity_type)) {
              foreach(entity_load($entity_type, $entity_ids) as $id => $entity) {
                $entity_label = entity_label($entity_type, $entity);
                if (FALSE !== stripos($entity_label, $string)) {
                  $matches[$prefix . $this->realm . ': ' . $entity_type . '|' . $id . ', '] = theme('sa_grouptag', array('entity_type' => $entity_type, 'entity_type_label' => $entity_type_label, 'gid' => $id, 'group_label' => $entity_label));
                }
              }
            }
          }
        }
      }
      return $matches;
    }

    public function process_usb_tags($op, &$items, &$realm_tags) {
      if (!empty($realm_tags[$this->realm])) {
        // We will validate as we go so the first step is knowing which entity
        // types are considered groups.
        $allowed_types = og_get_all_group_entity();
        $groups = array();
        foreach ($realm_tags[$this->realm] as $key => $group_info) {
          if ($group_parts = explode('|', $group_info)) {
            if (count($group_parts) == 2) {
              list($type, $id) = $group_parts;
              if (isset($groups[$type]) || $allowed_types[$type]) {
                // Retrieve the list of allowed group ids of this type.
                if (!is_array($allowed_types[$type])) {
                  $allowed_types[$type] = og_get_all_group($type);
                }
                // If the submitted group id is valid then save it.
                if (in_array($id, $allowed_types[$type])) {
                  $item = array(
                    'realm' => $this->realm,
                    'gid' => $id,
                    'gstr' => $type,
                    'op' => $op,
                  );
                  if (!(_shared_access_equal_items($item, $items))) {
                    $items[] = $item;
                  }
                  unset($realm_tags[$this->realm][$key]);
                }
              }
            }
          }
        }
        // Add any remaining group related tags to the list of unprocessed tags.
        foreach ($realm_tags[$this->realm] as $tag) {
          $realm_tags['unprocessed'][] = $this->realm . ': ' . $tag;
        }
      }
    }

   /**
    * Overrides DefaultSharedAccessController::theme().
    *
    * @returns
    *   An array of theme information as defined in hook_theme().
    */
    public static function theme() {
      // Theme how user references are displayed in the usb autocomplete list.
      return array(
        'sa_grouptag' => array(
          'variables' => array(
            'entity_type' => null,
            'entity_type_label' => null,
            'gid' => null,
            'group_label' => null,
          ),
          'file' => 'includes/shared_access.theme.inc',
        ),

        'sa_item_title_group' => array(
          'variables' => array('element' => NULL, 'item' => NULL),
          'file' => 'includes/shared_access.theme.inc',
          'base hook' => 'sa_item_title',
          'pattern' => 'sa_item_title_group__',
        ),
      );
    }
  }
}


/**
 * Extension of DefaultSharedAccessController to provide specific email address
 * related access control.
 *
 * @see shared_access_shared_access_realms().
 */
class MailSharedAccessController extends SharedAccessFormController {

}


/**
 * Extension of DefaultSharedAccessController to provide specific domain name
 * related access control.
 *
 * @see shared_access_shared_access_realms().
 *
 * SELECT SUBSTRING_INDEX(user_email,'@',-1) as domain_name FROM user_email group by domain_name
 */
class DomainSharedAccessController extends SharedAccessFormController implements SharedAccessUniversalShareBoxInterface {

  /**
   * Implements SharedAccessControllerInterface::access().
   *
   * Determines if the provided user account has access to the entity
   * attached to this particular shared_access field based on their email
   * domain.
   *
   * @param $grant
   *   A structured array that contians the grant info to check access for:
   *     - gstr: The domain to check.
   */
  public function access($grant, $params) {
    if ($strudel = strpos($params['account']->mail, '@')) {
      if (!strcasecmp($grant['gstr'], substr($params['account']->mail, $strudel))) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Overrides DefaultSharedAccessController::share_form().
   */
  public function share_form(&$form, &$form_state) {
    // Use the universal share box. Add this controller type to the list and
    // add a hint for the placeholder attribute.
    $form['universal_share_box']['#share_types'][$this->realm] = t('domain');
  }

  /**
   * Overrides DefaultSharedAccessController::share_form_autocomplete().
   *
   * Builds a list of domain matches to send back to the universal share box.
   */
  public function share_usb_autocomplete($field, $instance, $entity_type, $bundle_name, $string, $prefix) {
    $matches = array();
    if ($string) {
      $query = db_select('users', 'u')
        ->condition('u.mail', "%$string%", 'LIKE')
        ->distinct();
      $alias = $query->addExpression("SUBSTRING_INDEX(u.mail, '@', -1)", 'domain');
      $result = $query->execute();
      foreach ($result as $domain) {
        // In the simplest case (see user_autocomplete), the key and the value
        // are the same. Here we'll display the uid along with the username in
        // the dropdown.
        $matches[$prefix . $this->realm . ': @' . $domain->{$alias} . ', '] = theme('sa_domaintag', array('domain' => $domain->{$alias}));
      }
    }
    return $matches;
  }

  public function process_usb_tags($op, &$items, &$realm_tags) {
    if (!empty($realm_tags[$this->realm])) {
      foreach ($realm_tags[$this->realm] as $key => $group_info) {
      }
      $users = db_select('users')
        ->fields('users', array('uid', 'mail'))
        ->condition('mail', $realm_tags[$this->realm], 'IN')
        ->execute()
        ->fetchAllKeyed();
      if ($users) {
        $realm_tags['unprocessed'] += array_diff($realm_tags[$this->realm], $users);
        foreach($users as $uid => $mail) {
          $item = array(
            'realm' => $this->realm,
            'gid' => $uid,
            'gstr' => $mail,
            'op' => $op,
          );
          if (!(_shared_access_equal_items($item, $items)))
            $items[] = $item;
        }
      }
    }
  }


  /**
   * Overrides DefaultSharedAccessController::theme().
   *
   * @returns
   *   An array of theme information as defined in hook_theme().
   */
  public static function theme() {
    // Theme how user references are displayed in the usb autocomplete list.
    return array(
      'sa_domaintag' => array(
        'variables' => array('domain' => ''),
        'file' => 'includes/shared_access.theme.inc',
      ),
      'sa_item_title_domain' => array(
        'variables' => array('element' => NULL, 'item' => NULL),
        'file' => 'includes/shared_access.theme.inc',
        'base hook' => 'sa_item_title',
        'pattern' => 'sa_item_title_domain__',
      ),
    );
  }
}


/**
 * Extension of DefaultSharedAccessController to provide specific domain name
 * related access control.
 *
 * @see shared_access_shared_access_realms().
 */
class OwnerSharedAccessController implements SharedAccessControllerSettingsInterface, SharedAccessPriorityAccessInterface {
  /**
   * Name of the field.
   *
   * @var string
   */
  protected $realm;

  /**
   * Implements SharedAccessControllerInterface::access().
   *
   * Determines if the provided user account has access to the entity
   * attached to this particular shared_access field because they are
   * a member of an allowed group.
   *
   * @param $grant
   *   A structured array that contians the grant info to check access for:
   *     - gid: The group id to check.
   *     - gstr: The group type to check.
   */
  public function access($grant, $params) {
    return ($params['account']->uid == $grant['gid']);
  }

  /**
   * Implements SharedAccessControllerInterface::priority_access().
   *
   * Determines if the provided user account owns the entity and thus has
   * full administrative rights.
   *
   * @param $params
   *   A structured array that contians information to use when determining
   *   access.
   */
  public function priority_access($params) {
    $wrapper = entity_metadata_wrapper($params['entity_type'], $params['entity id']);
    $property = (isset($params['instance']['settings'][$this->realm]['u_prop'])) ? $params['instance']['settings'][$this->realm]['u_prop'] : NULL;
    return (isset($property) && isset($wrapper->{$property}) && $params['account']->uid == $wrapper->{$property}->raw());
  }

  public function field_instance_settings($field, $instance) {
    $owner_property = (isset($instance['settings'][$this->realm]['u_prop'])) ? $instance['settings'][$this->realm]['u_prop'] : NULL;
    $properties = entity_get_property_info($instance['entity_type']);
    $options = array();
    foreach($properties['properties'] as $property => $pinfo) {
      if (isset($pinfo['type']) && in_array($pinfo['type'], array('integer', 'user'))) $options[$property] = $pinfo['label'];
    }

    return array(
      'u_prop' => array(
        '#type' => 'select',
        '#title' => t('Owner property'),
        '#default_value' => $owner_property,
        '#options' => $options,
        '#description' => t('Select the property on the entity that is used to track the ownership of this entity. It should be an integer or \'user\' type field.'),
      ),
    );
  }

  public function field_settings($field, $instance, $has_data) {}

  /**
   * Constructor: sets basic variables.
   */
  public function __construct($realm) {
    $this->realm = $realm;
  }

  /**
   * Implements SharedAccessControllerInterface::theme().
   *
   * @returns
   *   An array of theme information as defined in hook_theme().
   */
  public static function theme() {
    // No theme info by default.
    return array(
      'sa_item_title_owner' => array(
        'variables' => array('element' => NULL, 'item' => NULL),
        'file' => 'includes/shared_access.theme.inc',
        'base hook' => 'sa_item_title',
        'pattern' => 'sa_item_title_owner__',
      ),
    );
  }

  public function sa_table_add_rows($element) {
    $rows = array();
//    dsm($element, 'OwnerSharedAccessController : sa_table_add_rows');
    if (isset($element['#instance']['settings'][$this->realm]['u_prop'])) {
      $wrapper = entity_metadata_wrapper($element['#entity_type'], $element['#entity_id']);
      $property = $element['#instance']['settings'][$this->realm]['u_prop'];
      if (isset($wrapper->{$property})) {
        $owner = '';
        // Fake the item format.
        $item = array('realm' => $this->realm, 'op' => 'owner', 'gid' => $wrapper->{$property}->raw(), 'gstr' => 'email');
        $rows[] = array(
            'data' => array(
              '', //add handling here for the icon.
              theme(
                array(
                  "sa_item_title_$this->realm" . '__' . $element['#field']['field_name'] . '__' . $element['#entity_type'],
                  "sa_item_title_$this->realm" . '__' . $element['#entity_type'],
                ),
                array('element' => $element, 'item' => $item, 'delta' => 0)
              ),
              'Owner',
              '',
            ),
            'tbody' => 'owner',
        );
      }
    }
    return $rows;
  }

  /**
   * Implements SharedAccessControllerInterface::sa_table_process().
   */
  public function sa_table_process(&$element, &$form_state) {
    $element['#tbodys']['owner'] = array('weight' => -100);
  }
}

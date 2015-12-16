<?php
/**
 * @file
 * Functions related to splash offer pages
 *
 * @ingroup splash_offer
 * @{
 */

/**
 * Sort splash offers by weight, used by uasort
 */
function splash_offer_sort($a, $b) {
  if ($a->weight == $b->weight) {
    return 0;
  }
  return $a->weight < $b->weight ? -1 : 1;
}

/**
 * Determine if a user role should see the splash
 *
 * This does NOT take history into account, so you will want to also check
   _splash_offer_has_seen() potentially
 *
 * @param SplashOfferEntity $entity
 * @param object $account
 *   Optional.
 *
 * @return bool
 *
 * @see _splash_offer_has_seen()
 */
function _splash_offer_role_access($entity, $account = NULL) {
  if ($account === NULL) {
    $account = $GLOBALS['user'];
  }
  return splash_offer_access('view', $entity, $account, 'splash_offer');
}


/**
 * Determine the current page visibility
 *
 * @param SplashOfferEntity $entity
 */
function _splash_offer_page_visibility($entity) {
  // Convert path to lowercase. This allows comparison of the same path
  // with different case. Ex: /Page, /page, /PAGE.
  $pages = drupal_strtolower($entity->access['pages'][0]);
  $page_match = TRUE;
  $visibility = $entity->access['pages']['#visibility'];
  if ($visibility < BLOCK_VISIBILITY_PHP) {
    // Convert the Drupal path to lowercase
    $path = drupal_strtolower(drupal_get_path_alias($_GET['q']));
    // Compare the lowercase internal and lowercase path alias (if any).
    $page_match = drupal_match_path($path, $pages);
    if ($path != $_GET['q']) {
      $page_match = $page_match || drupal_match_path($_GET['q'], $pages);
    }
    // When $visibility has a value of 0 (BLOCK_VISIBILITY_NOTLISTED),
    // the block is displayed on all pages except those listed in $block->pages.
    // When set to 1 (BLOCK_VISIBILITY_LISTED), it is displayed only on those
    // pages listed in $block->pages.
    $page_match = !($visibility xor $page_match);
  }
  elseif (module_exists('php')) {
    $page_match = php_eval($block->pages);
  }
  else {
    $page_match = FALSE;
  }

  // If we have a page match, we must make sure to run it through our auto_page
  // settings to unmatch if necessary
  if ($page_match && ($auto_paths = splash_offer_get_auto_pages($entity))) {
    $page_match = !drupal_match_path($_GET['q'], $auto_paths);
  }
  return $page_match;
}

/**
 * Determine the current page visibility based on active locale.
 *
 * @return bool
 */
function _splash_offer_locale_visibility($entity) {
  global $language;
  return empty($entity->access['locale']) || in_array($language->language, $entity->access['locale']);
}

/**
 * Return all auto pages
 *
 * @param SplashOfferEntity $entity
 *
 * @return string
 */
function splash_offer_get_auto_pages($entity) {
  $auto_pages = &drupal_static(__FUNCTION__, array());
  $cache_key = empty($entity->oid) ? 'new' : $entity->oid;
  if (empty($auto_pages[$cache_key])) {
    $auto_pages[$cache_key] = module_invoke_all('splash_offer_no_show_paths', $entity);
    $auto_pages[$cache_key] = implode("\n", $auto_pages[$cache_key]);
    drupal_alter('splash_offer_no_show_paths', $auto_pages[$cache_key]);
  }
  return $auto_pages[$cache_key];
}

/**
 * Determine the visibility based on device
 */
function _splash_offer_device_visibility($entity) {
  if (!module_exists('mobile_detect')
      || !$entity->access['devices']['#enabled']) {
    return TRUE;
  }
  elseif (($detect = mobile_detect_get_object())
          && ($triggers = array_merge($entity->access['devices']['phones'], $entity->access['devices']['tablets']))) {
    foreach ($triggers as $trigger) {
      if ($detect->is($trigger)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * Determine if the current user has already seen the splash
 *
 * @return bool
 */
function _splash_offer_has_seen($entity) {
  if (($entity->storage['cookies']['#enabled'])) {
    //@todo convert to include the machine name
    return !empty($_COOKIE['splash_offer:' . $entity->oid]);
  }
  else {
    return FALSE;
  }
}

/**
 * Defines our app objects
 */
class SplashOfferEntity extends Entity {

  public $name, $weight;
  public $links, $storage, $access = array();

  /**
   * Convertor
   * @param type param1
   */
  public function __construct($values = array()) {
   parent::__construct($values, 'splash_offer');
  }

  /**
   * Specifies the default label, which is picked up by label() by default.
   */
  protected function defaultLabel() {
    return $this->name;
  }
}

/**
 * Class SplashOfferEntityController
 */
class SplashOfferEntityController extends EntityAPIController implements EntityAPIControllerInterface {

  /**
   * Create a new entity.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   * @return
   *   A new instance of the entity type.
   */
  public function create(array $values = array()) {
    global $user;

    // Set the default values
    $values += array(
      'status' => SPLASH_OFFER_STATUS_ACTIVE,
      'weight' => 0,
      'links' => array(
        '#mode' => SPLASH_OFFER_BUTTON_MODE_UI,
        'yes' => array(
          'path'    => NULL,
          'text'    => '',
          'options' => array(),
        ),
        'no' => array(
          'path'    => NULL,
          'text'    => '',
          'options' => array(),
        ),
      ),
      'storage' => array(
        'cookies' => array(
          '#enabled'  => FALSE,
          '#lifetime' => 0,
          '#default'  => TRUE,
        ),
      ),
      'access' => array(
        'roles' => _splash_offer_default_roles(),
        'pages' => array(
          '#visibility' => BLOCK_VISIBILITY_NOTLISTED,
          0       => '',
        ),
        'devices' => array(
          '#enabled'  => FALSE,
          'phones'    => array(),
          'tablets'   => array(),
        ),
        'locale' => array(),
      ),
      'uid' => $user->uid,
    );
    $entity = parent::create($values);
    return $entity;
  }
}

/**
 * UI controller - may be put in any include file and loaded via the code registry.
 */
class SplashOfferUIController extends EntityDefaultUIController {

  /**
   * Translates form_submit values to entity values
   */
  public function entityFormSubmitBuildEntity($form, &$form_state) {
    $form_state[$this->entityType] = parent::entityFormSubmitBuildEntity($form, $form_state);

    // Now do our custom work to move non-fieldable values to correct places
    $entity = $form_state[$this->entityType];
    $data = $form_state['values']['data'];

    // links: #mode, yes, no
    $data['links']['#mode'] = $data['links']['mode'];
    unset($data['links']['mode']);
    $entity->links = drupal_array_merge_deep($entity->links, $data['links']);

    // storage: cookies
    foreach ($data['storage']['cookies']['fs_cookies'] as $key => $value) {
      $entity->storage['cookies']['#' . $key] = $value;
    }
    $entity->storage['cookies']['#enabled'] = $data['storage']['cookies']['enabled'];

    // access: roles
    $entity->access['roles'] = array_intersect_key(user_roles(), array_flip(array_filter($data['audience']['roles'])));

    // access: devices
    if (array_key_exists('devices', $data)) {
      $entity->access['devices']['#enabled'] = !$data['devices']['always_trigger'];
      $entity->access['devices']['phones'] = array_values(array_filter($data['devices']['fs_phones']['phones']));
      $entity->access['devices']['tablets'] = array_values(array_filter($data['devices']['fs_tablets']['tablets']));
    }

    // access: pages
    $data['audience']['pages']['#visibility'] = $data['audience']['pages']['visibility'];
    unset($data['audience']['pages']['visibility']);
    $entity->access['pages'] = $data['audience']['pages'];

    // access: locale
    if (array_key_exists('locale', $data['audience'])) {
      $entity->access['locale'] = array();
      foreach ($data['audience']['locale'] as $key => $value) {
        if (!empty($value)) {
          $entity->access['locale'][$key] = $key;
        }
      }
    }

    unset($entity->data);

    return $form_state[$this->entityType];
  }
}

/** @} */ //end of group: splash_offer

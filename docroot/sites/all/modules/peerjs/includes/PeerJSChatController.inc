<?php
/**
 * @file
 * VoiceboardController extends DrupalDefaultEntityController.
 */

/**
 * The Controller for Model entities
 */
class PeerJSChatController extends EntityAPIController {

  /**
   * Construct the entity.
   */
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  /**
   * Create a new entity.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   *
   * @return object
   *   A new instance of the entity type.
   */
  public function create(array $values = array()) {
    global $user;

    $values += array(
      'is_new' => TRUE,
      'cid' => 0,
      'uid' => $user->uid,
      'label' => '',
      'host' => '',
      'port' => '',
      'path' => '',
      'created' => time(),
    );

    $entity = parent::create($values);

    return $entity;
  }

  /**
   * Builds a structured array representing the entity's content.
   *
   * The content built for the entity will vary depending on the $view_mode
   * parameter.
   *
   * @param object $entity
   *   An entity object.
   * @param string $view_mode
   *   View mode, e.g. 'full', 'teaser'...
   * @param string $langcode
   *   (optional) A language code to use for rendering. Defaults to the global
   *   content language of the current request.
   *
   * @return array
   *   The renderable array.
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $account = user_load($GLOBALS['user']->uid);
    $content = parent::buildContent($entity, $view_mode, $langcode, $content);
    $settings = variable_get('peerjs', array(
      'general' => array(
        'server' => FALSE,
        'css' => TRUE,
      ),
      'server' => array(
        'host' => '',
        'port' => '80',
        'path2app' => '/',
      ),
      'constraints' => array(
        'audio' => TRUE,
        'video' => TRUE,
        'video_settings' => array(
          'minWidth' => 120,
          'maxWidth' => 240,
          'minHeight' => 120,
          'maxHeight' => 240,
          'minFrameRate' => 15,
          'maxFrameRate' => 15,
        ),
      ),
    ));

    // Set Peer options.
    $options = array(
      'host' => $entity->host,
      'port' => $entity->port,
      'path' => $entity->path2app,
      'debug' => 0,
    );

    // Add attached libraries.
    $content['#attached'] = array(
      'library' => array(
        array('angularjs', 'angularjs'),
      ),
      'libraries_load' => array(
        array('peerjs'),
        array('peerjs_chat'),
      ),
      'js' => array(
        array(
          'data' => array(
            'peerjs' => array(
              'entity' => $entity,
              'restws_csrf_token' => drupal_get_token('restws'),
              'baseUrl' => $GLOBALS['base_url'],
              'basePath' => base_path(),
              'modulePath' => drupal_get_path('module', 'peerjs'),
              'options' => $options,
              'user' => array(
                'uid' => $account->uid,
                'name' => $account->name,
                'picture' => isset($account->picture->uri) ? file_create_url($account->picture->uri) : file_create_url(variable_get('user_picture_default')),
                'realname' => format_username($account),
              ),
              'constraints' => array(
                'audio' => $settings['constraints']['audio'],
              ),
            ),
          ),
          'type' => 'setting',
        ),
      ),
    );

    // Add default styling if enabled.
    if ($settings['general']['css']) {
      $content['#attached']['css'] = array(
        array(
          'type' => 'file',
          'data' => drupal_get_path('module', 'peerjs') . '/css/peerjs.css',
        ),
      );
    }

    // Add video constraints settings if video is enabled.
    if ($settings['constraints']['video']) {
      $content['#attached']['js'][0]['data']['peerjs']['constraints']['video']['mandatory'] = $settings['constraints']['video_settings'];
    }
    else {
      $content['#attached']['js'][0]['data']['peerjs']['constraints']['video'] = $settings['constraints']['video'];
    }

    return $content;
  }

  /**
   * Generate an array for rendering the given entities.
   *
   * @param array $entities
   *   An array of entities to render.
   * @param string $view_mode
   *   View mode, e.g. 'full', 'teaser'...
   * @param string $langcode
   *   (optional) A language code to use for rendering. Defaults to the global
   *   content language of the current request.
   * @param bool $page
   *   (optional) If set will control if the entity is rendered: if TRUE
   *   the entity will be rendered without its title, so that it can be embeded
   *   in another context. If FALSE the entity will be displayed with its title
   *   in a mode suitable for lists.
   *   If unset, the page mode will be enabled if the current path is the URI
   *   of the entity, as returned by entity_uri().
   *   This parameter is only supported for entities which controller is a
   *   EntityAPIControllerInterface.
   *
   * @return array
   *   The renderable array, keyed by entity name or numeric id.
   */
  public function view($entities, $view_mode = 'full', $langcode = NULL, $page = NULL) {
    $element = parent::view($entities, $view_mode, $langcode, $page);
    return $element;
  }
}

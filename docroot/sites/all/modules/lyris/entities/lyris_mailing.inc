<?php
/**
 * @file
 * Separate include file to house lyris_mailing entity functionality.
 */
/**
 * Load a mailing object.
 *
 * @param $lc
 *   The Lyris Content Entity for which we are creating a mailing.
 */
function lyris_mailing(LyrisContent $lc) {
  // Mailing defaults are list specific, not content specific, so we need to
  // get the list that provided content belongs to.
  $list = lyris_object_parent($lc);
  $mailing = lyris_entity_create('lyris_mailing', $list->eid);

  // Import the SimpleMailingStruct from the LyrisContent.  That will set the
  // needed inheritted values.
  if ($ContentID = $lc->localVal('ContentID')) {
    // The ContentID is not part of the mailing struct, so we're setting it
    // as a regular parameter.
    $mailing->ContentID = $ContentID;

    $mailing->setLocalVal('ListName',  $lc->localVal('ListName'));
    $mailing->setStructVal('ListName', $lc->localVal('ListName'));

    $lyris = lyris(NULL, $mailing->server);

    if ($mailing->import = $lyris->importContent($ContentID)) {
      $mailing->mergeObjectData($mailing->import);
      $mailing->mergeStructData($mailing->import);
    }
  }

  $data = array('lyris_list' => $list);
  $mailing->setLocalVal('ReplyTo', token_replace($list->localVal('ReplyTo'), $data));

  return $mailing;
}

/**
 * Lyris Mailing Access
 */
function lyris_mailing_access($op, $mailing) {
  switch ($op) {
    case 'create':

      break;

    case 'view':

      break;

    case 'update':

      break;

    case 'delete':

      break;

    // Allow the user to deliver a mailing.  Depends on the content.
    case 'deliver':
      return (user_access('deliver any lyris mailings') || user_acces('deliver lyris mailings for ' . $mailing->localVal('ListName')));
      break;
  }

  return TRUE;
}

/**
 * Load a mailing entity.
 */
function lyris_mailing_load($eid) {
  return lyris_entity_load_entity('lyris_mailing', $eid);
}

/**
 * Lyris Mailing URI Callback
 */
function lyris_mailing_uri($entity) {
  return array(
    'path' => 'lyris/mailing/' . $entity->eid,
  );
}

/**
 * Entity class for Lyris Mailings.
 */
class LyrisMailing extends LyrisEntity {
  // The SimpleMailingStruct returned from ImportContent.
  public $import;

  /**
   * Constructor
   */
  public function __construct(array $values = array()) {
    parent::__construct('MailingStruct', 'lyris_mailing', $values);
  }

  /**
   * Implements LyrisEntityInterface::getDisplayName().
   */
  public function getDisplayName() {
    return $this->localVal('Title');
  }
}

/**
 * Entity API Controller for Lyris Content.
 */
class LyrisMailingEntityAPIController extends LyrisEntityAPIController implements EntityAPIControllerInterface {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  public function create(array $values = array()) {
    $list = lyris_list_load($values['parent_eid']);

    // Ensure the content's server matches the list's server.
    $values['server'] = $list->server;

    return parent::create($values);
  }
}

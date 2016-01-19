<?php


/**
 * Class OpenbadgeFileManager
 * This class allow the developer to store the Openbadges on the file system.
 * This class is to used with an Opigno LMS instance.
 */
class OpignoOpenbadgeFm
{

  /**
   * The prefix of the badge folder
   */
  const BADGE_FOLDER_PREFIX = 'badge_';

  /**
   * The issuer file name with extension .json
   */
  const ISSUER_FILE_NAME = 'issuer.json';

  /**
   * The badge logo file name with extension .png
   */
  const IMAGE_FILE_NAME = 'badge.png';

  /**
   * The criteria file name with extension .html
   */
  const CRITERIA_FILE_NAME = 'criteria.html';

  /**
   * The badge file name with extension .json
   */
  const BADGE_CLASS_FILE_NAME = 'class.json';

  /**
   * The prefix of the assert file
   */
  const ASSERT_FILE_PREFIX = 'assert_';


  /**
   * @var Issuer
   *   The issuer of the badge
   */
  protected $issuer;

  /**
   * @var BadgeClass The badge class
   */
  protected $badgeClass;

  /**
   * @var String The FID of the logo file
   */
  protected $logoFileID;

  /**
   * @var String The criteria in HTML format
   */
  protected $criteriaHTML;

  /**
   * @var String The folder URI where all the files will be saved
   */
  protected $specificFolderURI;

  /**
   * @var String The root folder where all the specifics folders will be
   */
  protected $baseFolderURI;

  /**
   * @var String The node ID representing the badge class
   */
  protected $nodeID;

  /**
   * @var String The drupal revision ID of the badge class
   */
  protected $versionID;


  /**
   * @param Issuer $issuer
   *   The issuer of the badge
   * @param BadgeClass $badgeClass
   *   The badge class
   * @param $logoFileID String
   *   The FID of the logo file
   * @param $criteriaHTML String
   *   The criteria in HTML format
   * @param $specificFolderURI String
   *   The folder URI where all the files will be saved
   * @param $baseFolderURI String
   *   The root folder where all the specifics folders will be
   * @param $nodeID String
   *   The node ID representing the badge class
   * @param $versionID String
   *   The drupal revision ID of the badge class
   */
  public function __construct(Issuer $issuer, BadgeClass $badgeClass, $logoFileID, $criteriaHTML, $specificFolderURI,
                               $baseFolderURI, $nodeID, $versionID)
  {
    $this->issuer = $issuer;
    $this->badgeClass = $badgeClass;
    $this->logoFileID = $logoFileID;
    $this->criteriaHTML = $criteriaHTML;
    $this->specificFolderURI = $specificFolderURI;
    $this->baseFolderURI = $baseFolderURI;
    $this->nodeID = $nodeID;
    $this->versionID = $versionID;
  }


  /**
   * Save these files :
   * - Logo file in PNG
   * - Criteria file in HTML
   * - Issuer file in JSON
   * - Badge class file in JSON
   *
   * @return bool
   *   True if success. False if fail
   */
  public function saveBasicBadgeFiles()
  {
    if (   $this->prepareFolderTree()
        && $this->saveBadgeLogoFile()
        && $this->saveCriteriaFile()
        && $this->saveIssuerFile()
        && $this->saveBadgeClassFile()
    ) {
      return true;
    }

    return false;
  }

  /**
   * Create and chmod the folders in order to prepare the tree.
   *
   * @return bool
   *   True if everything is okay. False if there was errors
   */
  public function prepareFolderTree()
  {
    return file_prepare_directory($this->specificFolderURI, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  }

  /**
   * Save the badge logo file in the good folder.
   *
   * @return bool|stdClass
   *   False if there was an error. The file object on success.
   */
  public function saveBadgeLogoFile()
  {
    $badgeLogo = file_load($this->logoFileID);

    if ($badgeLogo->uri == $this->baseFolderURI .'/'. $badgeLogo->filename) {
      $file = file_move($badgeLogo, $this->getBadgeLogoURI(), FILE_EXISTS_REPLACE);
      $this->logoFileID = ($file->fid ? $file->fid : false);
      return $this->logoFileID;
    }
    else {
      $file = file_copy($badgeLogo, $this->getBadgeLogoURI(), FILE_EXISTS_REPLACE);
      $this->logoFileID = ($file->fid ? $file->fid : false);
      return $this->logoFileID;
    }
  }


  /**
   * Update the image file ID on the drupal database.
   *
   * @param $nodeToUpdate
   * @param $newFID
   */
  public static function updateImageFileID($nodeToUpdate, $newFID)
  {
    $fieldsToUpdate = new stdClass();
    $fieldsToUpdate->nid = $nodeToUpdate->nid;
    $fieldsToUpdate->vid = $nodeToUpdate->vid;
    $fieldsToUpdate->type = $nodeToUpdate->type;
    $fieldsToUpdate->field_badge_image['und'][0]['fid'] = $newFID;
    field_attach_update('node', $fieldsToUpdate);
  }

  /**
   * Simply get the badge image FID
   *
   * @return String The FID of this badge image
   */
  public function getLogoFileID()
  {
    return $this->logoFileID;
  }

  /**
   * Save the criteria HTML file.
   *
   * @return bool|stdClass
   *   False on error. The file object created on success.
   */
  public function saveCriteriaFile()
  {
    $prefix =
      "<!DOCTYPE html>\n".
      "<html>\n".
      "<head>\n".
      "<meta charset=\"UTF-8\">".
      "<title>Opigno badge</title>\n".
      "</head>\n".
      "<body>\n";

    // Convert to UTF-8 in order to keep the accents correct
    $body = mb_convert_encoding($this->criteriaHTML, 'UTF-8');

    $suffix = "</body>\n</html>";

    return file_save_data($prefix . $body . $suffix, $this->getCriteriaFileURI());
  }

  /**
   * Save the issuer file in JSON format.
   *
   * @return bool|stdClass
   *   False on error. The file object created on success.
   */
  public function saveIssuerFile()
  {
    return file_save_data($this->issuer->toJson(), $this->getIssuerFileURI(), FILE_EXISTS_REPLACE);
  }

  /**
   * Save the badge class file in JSON format.
   *
   * @return bool|stdClass
   *   False on error. The file object created on success.
   */
  public function saveBadgeClassFile()
  {
    return file_save_data($this->badgeClass->toJson(), $this->getBadgeClassFileURI(), FILE_EXISTS_REPLACE);
  }

  /**
   * @return string The issuer file URI.
   */
  public function getIssuerFileURI()
  {
    return $this->specificFolderURI .'/'. self::ISSUER_FILE_NAME;
  }

  /**
   * @return string The badge logo URI.
   */
  public function getBadgeLogoURI()
  {
    return $this->specificFolderURI .'/'. self::IMAGE_FILE_NAME;
  }

  /**
   * @return string The criteria file URI.
   */
  public function getCriteriaFileURI()
  {
    return $this->specificFolderURI .'/'. self::CRITERIA_FILE_NAME;
  }

  /**
   * @return string The badge class file URI.
   */
  public function getBadgeClassFileURI()
  {
    return $this->specificFolderURI .'/'. self::BADGE_CLASS_FILE_NAME;
  }

  /**
   * Return the folder URI for this badge.
   *
   * @param $baseBadgeFolderURI String
   *   The base folder URI where all the badges will be saved
   * @param $openbadgeNode
   *   The drupal node representing an Openbadge
   *
   * @return string
   *   The specific folder for this badge.
   */
  public static function getSpecificBadgeFolderURI($baseBadgeFolderURI, $openbadgeNode)
  {
    return $baseBadgeFolderURI . '/' . self::BADGE_FOLDER_PREFIX . $openbadgeNode->nid . '_' . $openbadgeNode->vid;
  }

  /**
   * @param $uid Integer
   *   This assert's User ID
   *
   * @return string
   *   The file name of this assert
   */
  public static function getAssertFileName($uid)
  {
    return self::ASSERT_FILE_PREFIX . $uid .'.json';
  }

  /**
   * Check if the user already has this badge or not.
   *
   * @param $uid Integer
   *   The user ID to test
   *
   * @return bool
   *   True if he already has. False otherwise.
   */
  public function hasTheBadge($uid)
  {
    return file_exists($this->getAssertFileURI($uid));
  }

  /**
   * Return the complete URI of this assert file.
   *
   * @param $uid Integer
   *   The user ID
   *
   * @return string
   *   The URI to this assert file
   */
  public function getAssertFileURI($uid)
  {
    return $this->specificFolderURI .'/'. self::getAssertFileName($uid);
  }

  /**
   * Get the file ID (fid) of the assert for this badge
   *
   * @param $uid Integer The user ID (uid)
   *
   * @return bool|mixed false if there is no DB entry. The file DB entry if it exists
   */
  public function getAssertFileID($uid)
  {
    $fileIdDb = db_query(
      'SELECT fid FROM {file_managed} WHERE uri = :uri',
      array(':uri' => self::getAssertFileURI($uid))
    )->fetchField();

    return $fileIdDb;
  }

  /**
   * Save the assert on the disk
   *
   * @param $uid Integer
   *   The user ID
   *
   * @return bool
   *   True if the save success. False otherwise.
   */
  public function saveAssertFile($uid, $recipient = null)
  {
    if (empty($recipient))
      $recipient = user_load($uid)->mail;

    $assert = new Assert(
      $this->nodeID .'_'. $this->versionID .'_'. $uid,
      $recipient,
      file_create_url($this->getBadgeClassFileURI()),
      file_create_url($this->getAssertFileURI($uid)),
      date('c')
    );

    return (file_save_data($assert->toJson(), $this->getAssertFileURI($uid), FILE_EXISTS_REPLACE) ? true : false);
  }


  /**
   * Update the assert file by erasing the old content with the $fieldsToUpdate content.
   *
   * @param $assertUri String The URI of the assert file to update
   * @param $fieldsToUpdate Array An associative array with the fields to update
   *
   * @return bool True if the assert is correctly updated. False otherwise.
   */
  public static function updateAssertFile($assertUri, $fieldsToUpdate)
  {
    $originalAssertContent = file_get_contents($assertUri);
    if ($originalAssertContent == false)
      return false;

    $originalAssertContent = json_decode($originalAssertContent, true);
    $newAssertContent = $fieldsToUpdate + $originalAssertContent;

    $assert = new Assert(
      $newAssertContent['uid'],
      $newAssertContent['recipient'],
      $newAssertContent['badge'],
      $newAssertContent['verify']['url'],
      $newAssertContent['issuedOn']
    );

    return (file_save_data($assert->toJson(), $assertUri, FILE_EXISTS_REPLACE) ? true : false);
  }


  /**
   * Get all the people who has earned this badge
   *
   * @return Array
   *   $earners All the earners of this badge
   */
  public function getEarners()
  {
    $regex = '#^'. preg_quote(self::ASSERT_FILE_PREFIX) .'([0-9]*)'. preg_quote('.json') .'$#';
    $files = file_scan_directory($this->specificFolderURI, $regex);

    $earners = array();
    foreach($files as $file)
    {
      $matches = array();
      if (preg_match($regex, $file->filename, $matches) !== false)
        array_push($earners, $matches[1]);
    }
    return $earners;
  }


  /**
   * Get all the badges assert from the DB for a specific user.
   *
   * @param $uid Integer The user ID (uid)
   *
   * @return array An array with all the badges. (could be empty if no assert for this user)
   */
  public static function getBadgesFiles($uid)
  {
    return db_query('SELECT * ' .
      'FROM {file_managed} ' .
      'WHERE uid = :uid ' .
      'AND filename REGEXP \'^' . preg_quote(OpignoOpenbadgeFm::ASSERT_FILE_PREFIX) . '\'',
      array(':uid' => $uid))->fetchAllAssoc('fid');
  }


  /**
   * Remove the badges that are not given to any user.
   * They are removed from the file system and from the DB.
   *
   * @return bool True if there was no problem. False otherwise.
   */
  public function removeBadgeIfUnused()
  {
    if (count($this->getEarners()) == 0) {
      $isGood = true;

      // Delete the revision
      $revision = node_load(NULL, $this->versionID);
      db_delete('node_revision')
        ->condition('nid', $revision->nid)
        ->condition('vid', $revision->vid)
        ->execute();
      field_attach_delete_revision('node', $revision);

      // Delete all files
      $results = db_query(
        'SELECT f.fid FROM {file_managed} f WHERE f.uri LIKE :base_uri',
        array(':base_uri' => $this->specificFolderURI .'/%')
      )->fetchAllKeyed();

      foreach ($results as $fid => $empty)
      {
        if (file_delete(file_load($fid), true) == false) {
          watchdog(OPENBADGES_APP_NAME, 'Error while removing the file fid:'. $this->versionID);
          $isGood = false;
        }
      }

      // Then, delete the directory
      if (drupal_rmdir($this->specificFolderURI) == false) {
        watchdog(OPENBADGES_APP_NAME, 'Error while removing the directory '. $this->specificFolderURI);
        $isGood = false;
      }

      return $isGood;
    }

    return false;
  }

}
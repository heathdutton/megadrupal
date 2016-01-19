<?php

/**
 * Class OpignoOpenbadgesPresenter
 * This class allos the developer to implements the Openbadges in the Opigno instance.
 * This class contains all the methods to link with Drupal/Opigno 's hooks !
 */
class OpignoOpenbadgesPresenter
{

  /**
   * Field name of the revisions table of a badge
   */
  const FIELD_BADGE_REVISIONS = 'field_badge_revisions';

  /**
   * Field name of the badge settings form
   */
  const FIELD_BADGE_SETTINGS_REF = 'field_badge_ref';


  /**
   * Add the badge image on the information panel on the course page view
   *
   * @param $build array The page build
   */
  public static function node_view_alter_add_badge_image_info_panel(&$build)
  {
    if ($build['#node']->type == OPIGNO_COURSE_BUNDLE)
    {
      global $user;
      if (self::_check_user_can_receive_badge($build['#node']->nid, $user->uid, TRUE))
      {
        $openbadgeNode = node_load($build['#node']->field_badge_ref[LANGUAGE_NONE][0]['target_id']);

        $build['badge_image'] = array(
          '#theme' => 'field',
          '#title' => t('Badge earned'),
          '#field_type' => 'image',
          '#view_mode' => 'full',
          '#field_name' => 'badge_course_image',
          '#entity_type' => 'node',
          '#bundle' => 'openbadge',
          '#formatter' => 'image',
          '#items' => array(
            $openbadgeNode->field_badge_image['und'][0]
          )
        );
        $build['badge_image'][] = array(
          '#theme' => 'image_formatter',
          '#image_style' => 'thumbnail',
          '#item' => $openbadgeNode->field_badge_image['und'][0]
        );
      }
    }
  }


  /**
   * Unset these specific elements:
   * - The status header of the badge table (where you can click on "Modify")
   * - The remove button of this same table
   * - The save action in the badge creation form
   * - The update action in the badge updating form
   *
   * @param $form Array The form to edit
   */
  public static function form_alter_remove_unnecessaries(&$form)
  {
    // Remove the Status header and the possibility to remove the badge
    unset($form[self::FIELD_BADGE_SETTINGS_REF][LANGUAGE_NONE]['entities'][0]['actions']['ief_entity_remove']);
    unset($form[self::FIELD_BADGE_SETTINGS_REF][LANGUAGE_NONE]['entities']['#table_fields']['status']);

    // If the badge form is inserted into the course form, remove the "update" or "create" button from the inserted badge form
    if (!empty($form[self::FIELD_BADGE_SETTINGS_REF][LANGUAGE_NONE]['entities'][0]['form'])) {
      unset($form[self::FIELD_BADGE_SETTINGS_REF][LANGUAGE_NONE]['entities'][0]['form']['actions']['ief_edit_save']);
    } elseif (!empty($form[self::FIELD_BADGE_SETTINGS_REF][LANGUAGE_NONE]['form'])) {
      unset($form[self::FIELD_BADGE_SETTINGS_REF][LANGUAGE_NONE]['form']['actions']['ief_add_save']);
    }
  }


  /**
   * Add the revisions table in the form given and attach it to the badge settings.
   *
   * @param $form Array The form where we add the table
   */
  public static function form_alter_add_revisions_table(&$form)
  {
    // Array that will contains the revisions of the badge
    $form[self::FIELD_BADGE_REVISIONS]['table']['#theme'] = 'table';

    $form['#group_children'][self::FIELD_BADGE_REVISIONS] = 'group_badge_settings';
    $form[self::FIELD_BADGE_REVISIONS]['#weight'] = 100;

    $form[self::FIELD_BADGE_REVISIONS]['table']['#header'] = array(t('Rev'), t('Image'), t('Name'), t('Info'));

    // Get all the revisions and put them in the table
    $badgeNode = node_load($form[self::FIELD_BADGE_SETTINGS_REF][LANGUAGE_NONE]['entities'][0]['#entity']->nid);
    $revs = node_revision_list($badgeNode);
    $nbrRevs = count($revs);
    $revNo = $nbrRevs;
    foreach ($revs as $k => $currentRevAbstract) {
      $currentNode = node_load($badgeNode->nid, $currentRevAbstract->vid);

      $form[self::FIELD_BADGE_REVISIONS]['table']['#rows'][ $k ] = array(
        ($revNo == $nbrRevs ? t('Current') : $revNo),
        theme_image(array(
          'path'   => $currentNode->field_badge_image[LANGUAGE_NONE][0]['uri'],
          'width'  => 50,
          'height' => 50,
          'attributes' => array(),
        )),
        l($currentNode->title, 'node/'. $currentNode->nid .'/revisions/'. $currentNode->vid .'/view'),
        format_date($currentNode->revision_timestamp, 'short') . t(' by ') . $currentNode->name
      );

      $revNo--;
    }
  }


  /**
   * Hide the status field in the entity form (form introduced by the inline entity form module).
   *
   * @param $entity_form Array The entity form to edit
   * @param $defaultValue mixed The value to set to the field
   */
  public static function entity_form_alter_hide_status(&$entity_form, $defaultValue = 1)
  {
    $entity_form['status']['#access'] = false;
    $entity_form['status']['#default_value'] = $defaultValue;
  }

  /**
   * Hide the og_group_ref field in the entity form (form introduced by the inline entity form module).
   *
   * @param $entity_form Array The entity form to edit
   * @param $defaultValue mixed The value to set to the field
   */
  public static function entity_form_alter_hide_og_ref(&$entity_form, $defaultValue = array())
  {
    $entity_form['og_group_ref']['#access'] = false;
    $entity_form['og_group_ref'][LANGUAGE_NONE][0]['default']['#default_value'] = $defaultValue;
  }


  /**
   * Hide all the field of the given form.
   *
   * @param $form Array The form to hide his fields
   */
  public static function form_alter_hide_all(&$form)
  {
    foreach($form as $key => $value)
    {
      if (!preg_match('/^#/', $key))
        $form[$key]['#access'] = false;
    }
  }


  /**
   * Add a validation step that will check that if the badging system is activated, it must have a badge.
   *
   * @param $form_state Array The form state in validation
   */
  public static function node_validate_check_if_activated(&$form_state)
  {
    // If the button "add a new badge" is here and the "activate badge system" checkbox is checked,
    //   show an error
    if (!empty($form_state['values'][OpignoOpenbadgesPresenter::FIELD_BADGE_SETTINGS_REF][LANGUAGE_NONE]['actions']['ief_add'])
      && $form_state['values']['field_badge_active'][LANGUAGE_NONE][0]['value'] == 1
    ) {
      form_set_error('', t('Add a badge in order to activate it'));
    }
  }


  /**
   * Check if the user can receive a badge for this course or class.
   * Will not check if the user already have the badge !
   *
   * If $checkUserHasPassed is TRUE, will check if the user has passed the course/class
   *
   * @param $nid integer The course or class node ID
   * @param $uid integer The user ID
   *
   * @return bool True if the user can receive the badge. False otherwise
   */
  public static function _check_user_can_receive_badge($nid, $uid, $checkUserHasPassed=TRUE)
  {
    $node = node_load($nid);
    // Check if the badge system is activated for this course
    if ($node->field_badge_active[LANGUAGE_NONE][0]['value'] != 1)
    {
      return false;
    }

    // If the user is not logged in, it's an anonymous and we cannot give him a badge
    if (!user_is_logged_in())
      return false;

    // If the course/class is not public and the user is not member or active, we cannot give him the badge
    if ($node->group_access[ LANGUAGE_NONE ][0]['value'] != 0)
    {
      $membership = og_get_membership('node', $nid, 'user', $uid);
      if ($membership === false || $membership->state != 1) {
        return false;
      }
    }

    // If specified in parameters, check if the user has passed the course or class. Return false if not passed
    if ($checkUserHasPassed)
    {
      switch($node->type)
      {
        case OPIGNO_COURSE_BUNDLE:
          if(!opigno_quiz_app_user_passed($nid, $uid))
            return false;
          break;

        case 'class':
          if (self::getClassResult($nid, $uid) != OPIGNO_QUIZ_APP_PASSED)
            return false;
          break;
      }
    }

    return true;
  }


  /**
   * Do all the check before giving a badge to a user.
   *
   * @param $nid Integer The course or class ID
   * @param $uid Integer The user ID
   */
  public static function give_badge($nid, $uid)
  {
    if (self::_check_user_can_receive_badge($nid, $uid, FALSE))
    {
      $node = node_load($nid);

//      $openbadgeNid = null;
//      if (!empty($node->field_badge_ref[LANGUAGE_NONE][0]['entity']->nid))
//        $openbadgeNid = $node->field_badge_ref[LANGUAGE_NONE][0]['entity']->nid;
//      else
//        $openbadgeNid = $node->field_badge_ref[LANGUAGE_NONE][0]['target_id'];

      $openbadgeFileManager = OpignoOpenbadgesPresenter::getOpenbadgeFileManager(
        node_load($node->field_badge_ref[LANGUAGE_NONE][0]['target_id']),
        OPENBADGE_BASE_FOLDER_URI);

      // Give the badge here
      if ($openbadgeFileManager != false && !$openbadgeFileManager->hasTheBadge($uid) && $openbadgeFileManager->saveAssertFile($uid))
      {
        drupal_set_message(t('You have earned a new badge !'));

        // Send the notification mail
        $user = user_load($uid);
        if ($user->field_badge_mail_notification[LANGUAGE_NONE][0]['value'] == 1)
        {
          // All system mails need to specify the module and template key (mirrored from
          //  hook_mail()) that the message they want to send comes from.
          $module = OPENBADGES_APP_NAME;
          $key = MAIL_KEY_EARNING_BADGE;
          $to = $user->mail;
          $language = language_default();

          drupal_mail($module, $key, $to, $language);
        }

        // Sync the badge with the backpack
        if ($user->field_badge_sync_auto[LANGUAGE_NONE][0]['value'] == 1)
        {
          BackpackConnectPresenter::_issue_badge_to_all_backpacks($uid, $openbadgeFileManager->getAssertFileURI($uid));
          drupal_set_message(t('Your badge has been uploaded to your backpack'));
        }

        $hook = 'opigno_openbadge_assertion_created';
        foreach (module_implements($hook) as $module) {
          $function = $module . '_' . $hook;
          $function($nid, $openbadgeFileManager->getBadgeClassFileURI(), $openbadgeFileManager->getAssertFileURI($uid));
        }
      }

    }
  }

  /**
   * Show all the badges in a new section of a user profile.
   *
   * @param $build Array The user profile in array format
   */
  public static function user_view_alter_add_badges_section(&$build)
  {
    if ($build['#account']->field_badge_visibility[LANGUAGE_NONE][0]['value'] == 1)
    {
      // Get all the information needed
      $filesDb = db_query('SELECT uri, filename ' .
        'FROM {file_managed} ' .
        'WHERE uid = :uid ' .
        'AND filename REGEXP \'^' . preg_quote(OpignoOpenbadgeFm::ASSERT_FILE_PREFIX) . '\' ',
        array(':uid' => $build['#account']->uid))->fetchAll();

      if (count($filesDb) != 0)
      {
        // Create the badge section in the user profile
        $build['badges'] = array(
          '#type'  => 'user_profile_category',
          '#title' => t('Badges'),
          '#weight' => 4
        );

        // Add the issuer.js file
        self::add_issuer_api_js($build);

        $build['badges'][0] = array(
          '#type' => 'container'
        );

        // Get all the IDs and do a filter to get only the last VID
        $badgesIds = array();
        foreach ($filesDb as $fileDb)
        {
          $ids = self::_get_ids_from_uri($fileDb->uri);
          if (empty($badgesIds[ $ids['nid'] ]) || $badgesIds[ $ids['nid'] ]['vid'] < $ids['vid'])
          {
            $badgesIds[ $ids['nid'] ] = array(
              'vid' => $ids['vid'],
              'uri' => $ids['folderUri'] . OpignoOpenbadgeFm::IMAGE_FILE_NAME
            );
          }
        }

        // Add all the filtered badges in the $build
        foreach ($badgesIds as $badgeNID => $badgeValues)
        {
          $openbadgeNode = node_load($badgeNID, $badgeValues['vid']);

          array_push($build['badges'][0], array(
            '#markup' => l(
                theme_image(array(
                  'path'   => file_create_url($badgeValues['uri']),
                  'width'  => 100,
                  'height' => 100,
                  'alt'    => t('Badge image'),
                  'title'  => $openbadgeNode->title,
                  'attributes' => array(),
                )),
                'node/' . $badgeNID . '/revisions/' . $badgeValues['vid'] . '/view',
                array('html' => true))
          ));
        }
      }
    }
  }


  /**
   * Check the uri of this folder to extract the nid and vid.
   *
   * @param $badgeFolderUri string The badge folder URI (public://badges/badge_nid_vid/)
   *
   * @return array|bool
   *   False if impossible to get the nid and vid.
   *   An array('vid' => ..., 'nid' => ..., 'folderUri' => ...) on success
   */
  public static function _get_ids_from_uri($badgeUri)
  {
    try {
      // Getting the badge folder uri by removing the filename from the complete uri
      $filename = substr(strrchr($badgeUri, "/"), 1);
      $badgeFolderUri = substr($badgeUri, 0, -(strlen($filename)));

      // Get the node id and revision id of the badge from the uri
      $regex = '#' . preg_quote(OpignoOpenbadgeFm::BADGE_FOLDER_PREFIX) . '([0-9]+)_([0-9]+)/$#';
      $ids = array();
      preg_match($regex, $badgeFolderUri, $ids); // $ids[1] = nid  ;  $ids[2] = vid (revision id)

      return array('nid' => $ids[1], 'vid' => $ids[2], 'folderUri' => $badgeFolderUri);
    }
    catch (Exception $e) {
      return false;
    }
  }

  /**
   * Add the issuer API to the build, ready to be used
   *
   * @param $build Array The array ready for drupal_render()
   */
  public static function add_issuer_api_js(&$build)
  {
    $build['#attached']['js'] = array(
      'https://backpack.openbadges.org/issuer.js',
      drupal_get_path('module', 'opigno_mozilla_openbadges_app') . '/opigno_mozilla_openbadges_app.js'
    );
  }

  /**
   * Check all the classes of the course given in parameter and, if a class is passed, give the badge to the user.
   *
   * @param $courseID Integer The course ID
   * @param $uid Integer The user ID
   *
   * @throws Exception
   */
  public static function give_badge_if_class_passed($courseID, $uid)
  {
    //  Get all the classes linked to this course, check each class if one is passed and give badge for this class
    $results = db_query(
      'SELECT t.entity_id FROM {field_data_opigno_class_courses} t WHERE opigno_class_courses_target_id = :course_id',
      array(':course_id' => $courseID)
    )->fetchAllKeyed();

    foreach($results as $classId => $empty)
    {
      $classNode = node_load($classId);
      if (OpignoOpenbadgesPresenter::getClassResult($classNode, $uid) == OPIGNO_QUIZ_APP_PASSED)
      {
        self::give_badge($classNode->nid, $uid);
      }
    }
  }


  /**
   * Remove the operations column from the revisions table.
   *
   * @param $build Array The table as expected by drupal_render()
   */
  public static function node_revision_overview_remove_operations(&$build)
  {
    $build['node_revisions_table']['#header'][1] = t('Operations');

    unset($build['node_revisions_table']['#rows'][0][1]['colspan']);

    $nbrRows = count($build['node_revisions_table']['#rows']);
    for ($i = 1; $i < $nbrRows; $i++)
    {
      $build['node_revisions_table']['#rows'][$i][1] = array();
      $build['node_revisions_table']['#rows'][$i][1]['data'] = drupal_placeholder(t('No operation'));
      unset($build['node_revisions_table']['#rows'][$i][2]);
    }
  }


  /**
   * Show the earners of the badge.
   *
   * @param $build Array The badge view in array format ready for drupal_render
   */
  public static function node_view_alter_add_earners_field(&$build)
  {
    if ($build['#node']->type == OPENBADGE_CONTENT_TYPE_NAME)
    {
      $openbadgeFileManager = OpignoOpenbadgesPresenter::getOpenbadgeFileManager($build['#node'], OPENBADGE_BASE_FOLDER_URI);
      if ($openbadgeFileManager != false) {

        $build['badge_earners'] = array(
          '#theme'     => 'field',
          '#title'     => t('Earners'),
          '#items'     => array(),
          '#formatter' => 'list_default',
          '#weight'    => 100
        );

        $earners = $openbadgeFileManager->getEarners();
        if (count($earners) != 0) {

          foreach ($earners as $earnerID) {
            $user = user_load($earnerID);
            array_push($build['badge_earners']['#items'], $earnerID);
            array_push($build['badge_earners'], array(
              '#markup' => l($user->name, 'user/' . $earnerID)
            ));
          }
        }
        else {
          $build['badge_earners']['#items'][] = 0;
          $build['badge_earners'][] = array(
            '#markup' => t('No earner yet')
          );
        }

      }
    }
  }


  /**
   * Add the organic group reference in an openbadge object.
   *
   * @param $node stdClass The course or class node
   *
   * @throws Exception
   */
  public static function add_og_ref($node)
  {
    if (!empty($node->field_badge_ref['und'][0]['target_id']))
    {
      $badgeNode = node_load($node->field_badge_ref['und'][0]['target_id']);
      $badgeNode->og_group_ref['und'][0]['target_id'] = $node->nid;
      node_save($badgeNode);
    }
  }


  /**
   * Get the results for a class. If there is no mandatory quizzes, will return OPIGNO_QUIZ_APP_PASSED.
   *
   * @param $classNode stdClass The class node to test
   * @param $uid Integer The user ID
   *
   * @return string OPIGNO_QUIZ_APP_PASSED if success | the bad course data result if failed
   * @throws Exception
   */
  public static function getClassResult($classNode, $uid)
  {
    if ($classNode->type == 'class')
    {
      foreach($classNode->opigno_class_courses[LANGUAGE_NONE] as $courseAbstract)
      {
        $score = opigno_quiz_app_get_course_data_result($uid, $courseAbstract['target_id']);

        $courseNode = node_load($courseAbstract['target_id']);
        $reqQuizzes = opigno_quiz_app_get_all_required_quizzes($courseNode);

        // If there is required quizzes and the student did not pass, return the error
        if ($score['passed'] != OPIGNO_QUIZ_APP_PASSED && count($reqQuizzes) != 0) {
          return $score['passed'];
        }
      }

      return OPIGNO_QUIZ_APP_PASSED;
    }

    Throw new Exception('The node given in parameters is not a class node');
  }


  /**
   * Create an OpenbadgeImplementation object from a drupal node.
   *
   * @param $openbadgeNode stdClass
   *   A Drupal node
   * @param $baseBadgeFolderURI String
   *   The base folder where all the badges goes
   *
   * @return bool|OpignoOpenbadgeFm
   *   False if there was a problem. The OpenbadgeImplementation object otherwise.
   */
  public static function getOpenbadgeFileManager($openbadgeNode, $baseBadgeFolderURI)
  {
    if ($openbadgeNode->type == OPENBADGE_CONTENT_TYPE_NAME
      && !empty($openbadgeNode->field_badge_image[LANGUAGE_NONE][0]['fid'])
      && !empty($openbadgeNode->field_badge_criteria[LANGUAGE_NONE][0]['value'])
    ) {

      $logoFileID = $openbadgeNode->field_badge_image[LANGUAGE_NONE][0]['fid'];
      $criteriaHTML = $openbadgeNode->field_badge_criteria[LANGUAGE_NONE][0]['value'];
      $specificBadgeFolderURI = OpignoOpenbadgeFm::getSpecificBadgeFolderURI($baseBadgeFolderURI, $openbadgeNode);
      $description = (empty($openbadgeNode->field_badge_description[LANGUAGE_NONE][0]['value']) ? '' : $openbadgeNode->field_badge_description[LANGUAGE_NONE][0]['value']);

      $issuer = new Issuer(
        variable_get('site_name', 'Opigno LMS'),
        variable_get('site_slogan', ''),
        variable_get('base_url', 'http://'. $_SERVER['SERVER_NAME'])
      );

      $badge = new BadgeClass(
        $openbadgeNode->title,
        $description,
        file_create_url($specificBadgeFolderURI .'/'. OpignoOpenbadgeFm::IMAGE_FILE_NAME),
        file_create_url($specificBadgeFolderURI .'/'. OpignoOpenbadgeFm::CRITERIA_FILE_NAME),
        file_create_url($specificBadgeFolderURI .'/'. OpignoOpenbadgeFm::ISSUER_FILE_NAME)
      );

      return new OpignoOpenbadgeFm(
        $issuer,
        $badge,
        $logoFileID,
        $criteriaHTML,
        $specificBadgeFolderURI,
        $baseBadgeFolderURI,
        $openbadgeNode->nid,
        $openbadgeNode->vid);
    }
    else {
      return false;
    }
  }


  /**
   * Test if two nodes representing two openbadges have equals values.
   *
   * @param $openbadgeNode stdClass
   *   An openbadge node
   * @param $otherOpenbadgeNode stdClass
   *   The second openbadge node
   *
   * @return bool
   *   True if they have same values. False otherwise.
   */
  public static function formValuesAreEquals($openbadgeNode, $otherOpenbadgeNode)
  {
    if (
      $openbadgeNode->title == $otherOpenbadgeNode->title
      AND $openbadgeNode->field_badge_description[LANGUAGE_NONE][0]['value'] == $otherOpenbadgeNode->field_badge_description[LANGUAGE_NONE][0]['value']
      AND $openbadgeNode->field_badge_image[LANGUAGE_NONE][0]['fid'] == $otherOpenbadgeNode->field_badge_image[LANGUAGE_NONE][0]['fid']
      AND $openbadgeNode->field_badge_criteria[LANGUAGE_NONE][0]['value'] == $otherOpenbadgeNode->field_badge_criteria[LANGUAGE_NONE][0]['value']
      AND $openbadgeNode->field_badge_criteria[LANGUAGE_NONE][0]['format'] == $otherOpenbadgeNode->field_badge_criteria[LANGUAGE_NONE][0]['format']
    ) {
      return true;
    }
    return false;
  }

}
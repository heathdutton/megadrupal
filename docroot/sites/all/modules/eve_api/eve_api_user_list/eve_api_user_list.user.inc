<?php
/**
 * @file
 * Functions related to allowing users to alter the user settings and options.
 */

/**
 * Create the User List page.
 */
function eve_api_user_list_user_page($id = NULL) {
  $account = $GLOBALS['user'];

  if (!isset($id)) {
    drupal_set_title(t('User List'));
    $type = 0;
    $output = eve_api_user_list_user_general_page($id, $type);
  }
  else {
    $temp_id = (int) $id;

    if ("$id" == "$temp_id") {
      $result = db_query('SELECT allianceName, allianceID, corporationName, corporationID, characterName, characterID
        FROM {eve_api_characters}
        WHERE corporationID = :corporationID OR allianceID = :allianceID OR characterID = :characterID
        LIMIT 1', array(
          ':corporationID' => (int) $id,
          ':characterID' => (int) $id,
          ':allianceID' => (int) $id,
        ));
    }
    else {
      $id = urldecode($id);

      $result = db_query('SELECT allianceName, allianceID, corporationName, corporationID, characterName, characterID
        FROM {eve_api_characters}
        WHERE corporationName = :corporationName OR allianceName = :allianceName OR characterName = :characterName
        LIMIT 1', array(
          ':corporationName' => (string) $id,
          ':characterName' => (string) $id,
          ':allianceName' => (string) $id,
        ));
    }

    $row = $result->fetchAssoc();

    if (empty($row)) {
      drupal_set_title(t('User List'));
      $type = 0;
      $id = NULL;
      $output = eve_api_user_list_user_general_page($id, $type);
    }
    else {
      $is_good = eve_api_user_list_private_access((int) $row['corporationID']);

      switch ($id) {
        case (string) $row['corporationName']:
        case (int) $row['corporationID']:
          $type = 1;
          drupal_set_title(t('@name User List', array('@name' => $row['corporationName'])));
          if ($is_good) {
            $output = eve_api_user_list_user_private_page($row['corporationID']);
          }
          else {
            $output = eve_api_user_list_user_general_page($row['corporationID'], $type);
          }
          break;

        case (string) $row['allianceName']:
        case (int) $row['allianceID']:
          $type = 2;
          drupal_set_title(t('@name User List', array('@name' => $row['allianceName'])));
          $output = eve_api_user_list_user_general_page($row['allianceID'], $type);
          break;

        case (string) $row['characterName']:
        case (int) $row['characterID']:
          drupal_set_title($row['characterName']);
          $output = eve_api_user_list_user_character_page($row['characterID'], $is_good);
          break;

        default:
          drupal_set_title(t('User List'));
          $type = 0;
          $id = NULL;
          $output = eve_api_user_list_user_general_page($id, $type);
          break;
      }
    }
  }

  return $output;
}

/**
 * Create the User List page.
 */
function eve_api_user_list_user_general_page($id, $type) {
  $header = array(
    'characterName' => array(
      'data' => t('Name'),
      'field' => 'c.characterName',
    ),
    'corporationName' => array(
      'data' => t('Corporation'),
      'field' => 'c.corporationName',
    ),
    'allianceName' => array(
      'data' => t('Alliance'),
      'field' => 'c.allianceName',
    ),
    'joined' => array(
      'data' => t('Joined'),
    ),
    'access' => array(
      'data' => t('Last Login'),
    ),
  );

  $options = $output = array();

  $query = db_select('users', 'u')
    ->extend('PagerDefault')
    ->extend('TableSort');
  $query_join = $query->join('eve_api_characters', 'c', 'u.uid = c.uid');
  $query->addField($query_join, 'characterID', 'cID');
  $query->fields('u', array(
      'created',
      'access',
      'uid',
      'characterID',
    ))
    ->fields('c', array(
      'characterName',
      'corporationID',
      'corporationName',
      'corporationTicker',
      'allianceID',
      'allianceName',
      'allianceTicker',
    ));

  if (isset($id)) {
    $or = db_or();
    $or->condition('c.allianceID', $id, '=');
    $or->condition('c.corporationID', $id, '=');
    $query->condition($or);
  }

  $query->condition('c.deleted', 0, '=')
    ->condition('u.status', 1, '=')
    ->condition('u.characterID', 0, '!=')
    ->limit(20)
    ->orderByHeader($header);
  $result = $query->execute();

  if ($result->rowCount()) {
    foreach ($result->fetchAll() as $row) {
      $options[] = array(
        'characterName' => l($row->characterName, 'eve_api/user_list/' . $row->cID),
        'corporationName' => l($row->corporationName, 'eve_api/user_list/' . $row->corporationID) . check_plain(' [' . $row->corporationTicker . ']'),
        'allianceName' => ($row->allianceID == 0) ? l(t('None'), 'eve_api/user_list/0') : l($row->allianceName, 'eve_api/user_list/' . $row->allianceID) . check_plain(' [' . $row->allianceTicker . ']'),
        'joined' => ($row->characterID == $row->cID) ? check_plain(date('Y-m-d H:i:s', $row->created)) : '',
        'access' => ($row->characterID == $row->cID) ? ($row->access == 0) ? '' : check_plain(date('Y-m-d H:i:s', $row->access)) : '',
      );
    }
  }

  switch ($type) {
    default:
    case 0:
      $title = t('List of All Characters and some basic information.');
      break;

    case 1:
      $title = t('List of Corporation Characters and some basic information.');
      break;

    case 2:
      $title = t('List of Alliance Characters and some basic information.');
      break;
  }

  $output['user_list_description'] = array(
    '#type' => 'item',
    '#title' => $title,
    '#weight' => 0,
  );

  $output['user_list'] = array(
    '#type' => 'markup',
    '#markup' => theme('table', array(
      'header' => $header,
      'rows' => $options,
    )),
    '#weight' => 10,
  );

  $output['page'] = array(
    '#theme' => 'pager',
    '#weight' => 20,
  );

  return $output;
}

/**
 * Create the director User List page.
 */
function eve_api_user_list_user_private_page($corporation_id) {
  $header = array(
    'characterName' => array(
      'data' => t('Name'),
      'field' => 'c.characterName',
    ),
    'main' => array(
      'data' => t('Main Character'),
      'field' => 'u.name',
    ),
    'api' => array(
      'data' => t('API Data'),
    ),
    'joined' => array(
      'data' => t('Joined'),
      'field' => 'u.created',
    ),
    'access' => array(
      'data' => t('Last Login'),
      'field' => 'u.access',
    ),
  );

  $options = $output = array();

  $query = db_select('users', 'u')
    ->extend('PagerDefault')
    ->extend('TableSort');
  $query_join = $query->join('eve_api_characters', 'c', 'u.uid = c.uid');
  $query->join('eve_api_keys', 'ak', 'c.apiID = ak.apiID');
  $query->addField($query_join, 'characterID', 'cID');
  $result = $query->fields('u', array(
      'name',
      'created',
      'access',
      'uid',
      'characterID',
    ))
    ->fields('c', array(
      'characterName',
      'corporationID',
      'corporationName',
      'corporationTicker',
      'allianceID',
      'allianceName',
      'allianceTicker',
    ))
    ->fields('ak', array(
      'keyID',
      'vCode',
    ))
    ->condition('c.corporationID', $corporation_id, '=')
    ->condition('c.deleted', 0, '=')
    ->condition('u.status', 1, '=')
    ->condition('u.characterID', 0, '!=')
    ->limit(20)
    ->orderByHeader($header)
    ->execute();

  if ($result->rowCount()) {
    foreach ($result->fetchAll() as $row) {
      $options[] = array(
        'characterName' => l($row->characterName, 'eve_api/user_list/' . $row->cID),
        'main' => l($row->name, 'user/' . $row->uid),
        'api' => t('Key ID: @keyID<br />Verification Code: @vCode', array('@keyID' => $row->keyID, '@vCode' => $row->vCode)),
        'joined' => ($row->characterID == $row->cID) ? check_plain(date('Y-m-d H:i:s', $row->created)) : '',
        'access' => ($row->characterID == $row->cID) ? ($row->access == 0) ? '' : check_plain(date('Y-m-d H:i:s', $row->access)) : '',
      );
    }
  }

  $output['corporation_user_list_description'] = array(
    '#type' => 'item',
    '#title' => t('List of Corporation Users and private information.'),
    '#weight' => 0,
  );

  $output['corporation_user_list'] = array(
    '#type' => 'markup',
    '#markup' => theme('table', array(
      'header' => $header,
      'rows' => $options,
    )),
    '#weight' => 10,
  );

  $output['page'] = array(
    '#theme' => 'pager',
    '#weight' => 20,
  );

  return $output;
}

/**
 * Create the character User List page.
 */
function eve_api_user_list_user_character_page($character_id, $is_good) {
  global $user, $is_https;

  $output = array();

  $character_data = eve_api_get_character_info($character_id);

  if ($character_data == FALSE) {
    drupal_goto('eve_api/user_list');
  }

  $output['profile'] = array(
    '#theme_wrappers' => array('container'),
    '#title' => '',
    '#weight' => 1,
    '#attributes' => array('class' => array('profile')),
  );

  $attributes = array(
    'attributes' => array(
      'title' => t('View user profile.'),
      'class' => 'user-picture',
    ),
    'html' => TRUE,
  );

  $eve_avatar = $is_https ? variable_get('eve_api_avatar_https', 'https://image.eveonline.com/Character/') : variable_get('eve_api_avatar_http', 'http://image.eveonline.com/Character/');
  $eve_avatar .= (int) $character_data['characterID'] . '_' . variable_get('eve_api_user_picture_dimensions', '128') . '.jpg';
  $alt = t("@user's picture", array('@user' => $character_data['characterName']));

  if (module_exists('image') && file_valid_uri($eve_avatar) && $style = variable_get('user_picture_style', '')) {
    $character_picture = theme('image_style', array(
      'style_name' => $style,
      'path' => $eve_avatar,
      'alt' => $alt,
      'title' => $alt,
    ));
  }
  else {
    $character_picture = theme('image', array(
      'path' => $eve_avatar,
      'alt' => $alt,
      'title' => $alt,
    ));
  }

  $output['profile']['user_picture'] = array(
    '#markup' => l($character_picture, 'user/' . $character_data['uid'], $attributes),
    '#weight' => -10,
  );

  $output['profile']['eve_api_attributes'] = array();

  $output['profile']['eve_api_skills'] = array();

  $output['profile']['eve_api_private'] = array();

  $output['profile']['eve_api_attributes'] += array(
    '#theme_wrappers' => array('user_profile_category'),
    '#attributes' => array('class' => array('user-member')),
    '#weight' => 2,
    '#title' => t('Attributes'),
  );

  $output['profile']['eve_api_attributes']['DoB'] = array(
    '#theme' => 'user_profile_item',
    '#title' => t('Date of Birth'),
    '#markup' => check_plain($character_data['DoB']),
  );

  $output['profile']['eve_api_attributes']['race'] = array(
    '#theme' => 'user_profile_item',
    '#title' => t('Race'),
    '#markup' => check_plain($character_data['race']),
  );

  $output['profile']['eve_api_attributes']['gender'] = array(
    '#theme' => 'user_profile_item',
    '#title' => t('Gender'),
    '#markup' => check_plain($character_data['gender']),
  );

  $output['profile']['eve_api_attributes']['securityStatus'] = array(
    '#theme' => 'user_profile_item',
    '#title' => t('Security Status'),
    '#markup' => check_plain($character_data['securityStatus']),
  );

  $output['profile']['eve_api_attributes']['corporation'] = array(
    '#theme' => 'user_profile_item',
    '#title' => t('Corporation'),
    '#markup' => (($is_good) ? l($character_data['corporationName'], 'eve_api/user_list/' . $character_data['corporationID']) : l($character_data['corporationName'], 'eve_api/user_list/' . $character_data['corporationID'])) . ' (' . $character_data['corporationTicker'] . ')',
  );

  $output['profile']['eve_api_attributes']['corporationDate'] = array(
    '#theme' => 'user_profile_item',
    '#title' => t('Joined Corporation'),
    '#markup' => check_plain($character_data['corporationDate']),
  );

  if ($character_data['allianceID'] != 0) {
    $output['profile']['eve_api_attributes']['alliance'] = array(
      '#theme' => 'user_profile_item',
      '#title' => t('Alliance'),
      '#markup' => l($character_data['allianceName'], 'eve_api/user_list/' . $character_data['allianceID']) . ' (' . $character_data['allianceTicker'] . ')',
    );
  }

  if ($user->uid != $character_data['uid'] && !$is_good) {
    return $output;
  }

  if (($character_data['accessMask'] & 16777216) == 16777216) {
    $options = array();

    $options += array(
      'intelligence' => array(
        'attribute' => 'Intelligence',
        'points' => check_plain($character_data['intelligence']),
        'bonus' => isset($character_data['intelligenceBonus']) ? check_plain($character_data['intelligenceBonus']) : 0,
      ),
    );

    $options += array(
      'memory' => array(
        'attribute' => 'Memory',
        'points' => check_plain($character_data['memory']),
        'bonus' => isset($character_data['memoryBonus']) ? check_plain($character_data['memoryBonus']) : 0,
      ),
    );

    $options += array(
      'charisma' => array(
        'attribute' => 'Charisma',
        'points' => check_plain($character_data['charisma']),
        'bonus' => isset($character_data['charismaBonus']) ? check_plain($character_data['charismaBonus']) : 0,
      ),
    );

    $options += array(
      'perception' => array(
        'attribute' => 'Perception',
        'points' => check_plain($character_data['perception']),
        'bonus' => isset($character_data['perceptionBonus']) ? check_plain($character_data['perceptionBonus']) : 0,
      ),
    );

    $options += array(
      'willpower' => array(
        'attribute' => 'Willpower',
        'points' => check_plain($character_data['willpower']),
        'bonus' => isset($character_data['willpowerBonus']) ? check_plain($character_data['willpowerBonus']) : 0,
      ),
    );

    $output['profile']['eve_api_private'] += array(
      '#theme_wrappers' => array('user_profile_category'),
      '#attributes' => array('class' => array('user-member')),
      '#weight' => 3,
      '#title' => t('Private Information'),
    );

    $output['profile']['eve_api_private']['lastKnownLocation'] = array(
      '#theme' => 'user_profile_item',
      '#title' => t('Last Known Location'),
      '#markup' => check_plain($character_data['lastKnownLocation']),
    );

    $output['profile']['eve_api_private']['shipName'] = array(
      '#theme' => 'user_profile_item',
      '#title' => t('Last Known Ship Name'),
      '#markup' => check_plain($character_data['shipName']),
    );

    $output['profile']['eve_api_private']['stats'] = array(
      '#theme' => 'user_profile_item',
      '#title' => t('Stats'),
      '#markup' => theme('table', array(
        'header' => array(
          'attribute' => t('Attribute'),
          'points' => t('Points'),
          'bonus' => t('Bonus'),
        ),
        'rows' => $options,
      )),
    );
  }

  $output['profile']['eve_api_skills'] += array(
    '#theme_wrappers' => array('user_profile_category'),
    '#attributes' => array('class' => array('user-member')),
    '#weight' => 4,
    '#title' => t('Skills'),
  );

  $output['profile']['eve_api_skills']['skillPoints'] = array(
    '#theme' => 'user_profile_item',
    '#title' => t('Total Skillpoints'),
    '#markup' => check_plain($character_data['skillPoints']),
  );

  if (($character_data['accessMask'] & 131072) == 131072) {
    if ($character_data['skillInTraining'] == 1) {
      $output['profile']['eve_api_skills']['training'] = array(
        '#theme' => 'user_profile_item',
        '#title' => t('Currently Training'),
        '#markup' => t('Currently Training @typeName to Level @level. Training will end on @finish UTC EVE Time.', array(
        '@typeName' => $character_data['typeName'],
        '@level' => $character_data['trainingToLevel'],
        '@finish' => $character_data['trainingEndTime'],
        )),
      );
    }
    else {
      $output['profile']['eve_api_skills']['training'] = array(
        '#theme' => 'user_profile_item',
        '#title' => t('Currently Training'),
        '#markup' => t('Character is not currently training anything.'),
      );
    }
  }

  $character_skills = eve_api_get_character_skills($character_data['characterID']);

  drupal_add_library('system', 'drupal.vertical-tabs');

  $form['tabs'] = array('#type' => 'vertical_tabs');

  foreach ($character_skills as $skill_group => $skills) {
    $options = array();

    foreach ($skills as $skill) {
      $group_id = $skill['groupID'];

      $form['tabs'][$group_id][$skill['typeID']]['Skill Name']['#markup'] = check_plain($skill['typeName']);
      $form['tabs'][$group_id][$skill['typeID']]['Skill Level']['#markup'] = check_plain($skill['level']);
    }

    $form['tabs'][$group_id]['#type'] = 'fieldset';
    $form['tabs'][$group_id]['#title'] = check_plain($skill_group);
  }

  foreach (element_children($form['tabs']) as $id_tab) {
    if (is_int($id_tab)) {
      $rows = array();

      foreach (element_children($form['tabs'][$id_tab]) as $id_row) {
        $classes = array('');

        $rows[] = array(
          'data' => array(
            drupal_render($form['tabs'][$id_tab][$id_row]['Skill Name']),
            drupal_render($form['tabs'][$id_tab][$id_row]['Skill Level']),
          ),
          'class' => $classes,
        );
      }

      $header = array(
        'skillname' => t('Skill Name'),
        'skilllevel' => t('Skill Level'),
      );

      $table_id = 'skillgroups-' . $id_tab;

      $form['tabs'][$id_tab]['table']['#markup'] = theme('table', array(
        'header' => $header,
        'rows' => $rows,
        'attributes' => array('id' => $table_id),
      ));
    }
  }

  $output['profile']['eve_api_skills']['skills'] = array(
    '#theme' => 'user_profile_item',
    '#title' => t('Skill Sheet'),
    '#markup' => drupal_render_children($form),
  );

  if (!$is_good) {
    return $output;
  }

  $options = array();

  foreach ($character_data['characters'] as $row) {
    $options[$row['characterID']] = array(
      'characterName' => l($row['characterName'], 'eve_api/user_list/' . $row['characterID']),
      'corporation' => l($row['corporationName'], 'eve_api/user_list/' . $row['corporationID']) . check_plain(' [' . $row['corporationTicker'] . ']'),
      'alliance' => ($row['allianceID'] == 0) ? l(t('None'), 'eve_api/user_list/0') : l($row['allianceName'], 'eve_api/user_list/' . $row['allianceID']) . check_plain(' [' . $row['allianceTicker'] . ']'),
      'status' => ($row['deleted'] == 1) ? t('Deleted') : eve_api_api_error_msg($row['errorCode'], $row['errorText']),
    );
  }

  $output['profile']['eve_api_private']['characters'] = array(
    '#theme' => 'user_profile_item',
    '#title' => t('Known Characters'),
    '#markup' => theme('table', array(
      'header' => array(
        'characterName' => t('Character Name'),
        'corporation' => t('Corporation'),
        'alliance' => t('Alliance'),
        'status' => t('Status'),
      ),
      'rows' => $options,
    )),
  );

  $options = array();

  foreach ($character_data['api_keys'] as $row) {
    $options[$row['apiID']] = array(
      'keyID' => check_plain($row['keyID']),
      'vCode' => check_plain($row['vCode']),
      'status' => eve_api_api_error_msg($row['errorCode'], $row['errorText']),
    );
  }

  $output['profile']['eve_api_private']['api_keys'] = array(
    '#theme' => 'user_profile_item',
    '#title' => t('API Keys'),
    '#markup' => theme('table', array(
      'header' => array(
        'keyID' => t('Key ID'),
        'vCode' => t('Verification Code'),
        'status' => t('Status'),
      ),
      'rows' => $options,
    )),
  );

  return $output;
}

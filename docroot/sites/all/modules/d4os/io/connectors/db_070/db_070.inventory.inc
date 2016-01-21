<?php

/**
 * @package    d4os_io db 070
 * @copyright Copyright (C) 2010-2012 Wene - ssm2017 Binder ( S.Massiaux ). All rights reserved.
 * @link      http://www.d4os.org
 * @license   GNU/GPL, http://www.gnu.org/licenses/gpl-2.0.html
 * D4os is free software. This version may have been modified pursuant
 * to the GNU General Public License, and as distributed it includes or
 * is derivative of works licensed under the GNU General Public License or
 * other free or open source software licenses.
 */
class D4OS_IO_db_070_Inventory implements D4OS_IO_Inventory_Interface {

  public $response;
  public $values;

  function __construct() {
    $this->response = new stdClass();
    $this->values = array();
  }

  function ping() {
    $this->response->success = d4os_io_db_070_mysql_is_alive('os_robust', 'Robust');
    return;
  }

  function create_new_inventory($params) {
    if (!isset($params['user_uuid']))
      return FALSE;

    $user_uuid = $params['user_uuid'];
    $my_inventory = isset($params['my_inventory']) ? $params['my_inventory'] : d4os_uuid_create();
    $animations = isset($params['animations']) ? $params['animations'] : d4os_uuid_create();
    $notecards = isset($params['notecards']) ? $params['notecards'] : d4os_uuid_create();
    $calling_cards = isset($params['calling_cards']) ? $params['calling_cards'] : d4os_uuid_create();
    $landmarks = isset($params['landmarks']) ? $params['landmarks'] : d4os_uuid_create();
    $scripts = isset($params['scripts']) ? $params['scripts'] : d4os_uuid_create();
    $body_parts = isset($params['body_parts']) ? $params['body_parts'] : d4os_uuid_create();
    $sounds = isset($params['sounds']) ? $params['sounds'] : d4os_uuid_create();
    $photo_album = isset($params['photo_album']) ? $params['photo_album'] : d4os_uuid_create();
    $textures = isset($params['textures']) ? $params['textures'] : d4os_uuid_create();
    $gestures = isset($params['gestures']) ? $params['gestures'] : d4os_uuid_create();
    $clothing = isset($params['clothing']) ? $params['clothing'] : d4os_uuid_create();
    $lost_and_found = isset($params['lost_and_found']) ? $params['lost_and_found'] : d4os_uuid_create();
    $trash = isset($params['trash']) ? $params['trash'] : d4os_uuid_create();
    $objects = isset($params['objects']) ? $params['objects'] : d4os_uuid_create();

    $values = array(
      array('folderName' => 'My Inventory', 'type' => 8, 'version' => 1, 'folderID' => $my_inventory, 'agentID' => $user_uuid, 'parentFolderID' => UUID_ZERO),
      array('folderName' => 'Animations', 'type' => 20, 'version' => 1, 'folderID' => $animations, 'agentID' => $user_uuid, 'parentFolderID' => $my_inventory),
      array('folderName' => 'Notecards', 'type' => 7, 'version' => 1, 'folderID' => $notecards, 'agentID' => $user_uuid, 'parentFolderID' => $my_inventory),
      array('folderName' => 'Calling Cards', 'type' => 2, 'version' => 1, 'folderID' => $calling_cards, 'agentID' => $user_uuid, 'parentFolderID' => $my_inventory),
      array('folderName' => 'Landmarks', 'type' => 3, 'version' => 1, 'folderID' => $landmarks, 'agentID' => $user_uuid, 'parentFolderID' => $my_inventory),
      array('folderName' => 'Scripts', 'type' => 10, 'version' => 1, 'folderID' => $scripts, 'agentID' => $user_uuid, 'parentFolderID' => $my_inventory),
      array('folderName' => 'Body Parts', 'type' => 13, 'version' => 1, 'folderID' => $body_parts, 'agentID' => $user_uuid, 'parentFolderID' => $my_inventory),
      array('folderName' => 'Sounds', 'type' => 1, 'version' => 1, 'folderID' => $sounds, 'agentID' => $user_uuid, 'parentFolderID' => $my_inventory),
      array('folderName' => 'Photo Album', 'type' => 15, 'version' => 1, 'folderID' => $photo_album, 'agentID' => $user_uuid, 'parentFolderID' => $my_inventory),
      array('folderName' => 'Textures', 'type' => 0, 'version' => 1, 'folderID' => $textures, 'agentID' => $user_uuid, 'parentFolderID' => $my_inventory),
      array('folderName' => 'Gestures', 'type' => 21, 'version' => 1, 'folderID' => $gestures, 'agentID' => $user_uuid, 'parentFolderID' => $my_inventory),
      array('folderName' => 'Clothing', 'type' => 5, 'version' => 1, 'folderID' => $clothing, 'agentID' => $user_uuid, 'parentFolderID' => $my_inventory),
      array('folderName' => 'Lost And Found', 'type' => 16, 'version' => 1, 'folderID' => $lost_and_found, 'agentID' => $user_uuid, 'parentFolderID' => $my_inventory),
      array('folderName' => 'Trash', 'type' => 14, 'version' => 1, 'folderID' => $trash, 'agentID' => $user_uuid, 'parentFolderID' => $my_inventory),
      array('folderName' => 'Objects', 'type' => 6, 'version' => 1, 'folderID' => $objects, 'agentID' => $user_uuid, 'parentFolderID' => $my_inventory),
    );
    d4os_io_db_070_set_active('os_robust');
    $query = db_insert('inventoryfolders')->fields(array('folderName', 'type', 'version', 'folderID', 'agentID', 'parentFolderID'));
    foreach ($values as $record) {
      $query->values($record);
    }
    $query->execute();
    d4os_io_db_070_set_active('default');
  }

  function create_inventory_item($params) {
    $values = array(
      'assetID' => $params['asset_id'], // 1
      'assetType' => $params['asset_type'], // 2
      'inventoryName' => $params['inventory_name'], // 3
      'inventoryDescription' => $params['inventory_description'], // 4
      'inventoryNextPermissions' => $params['inventory_next_permissions'], // 5
      'inventoryCurrentPermissions' => $params['inventory_current_permissions'], // 6
      'invType' => $params['inventory_type'], // 7
      'creatorID' => $params['creator_uuid'], // 8
      'inventoryBasePermissions' => $params['inventory_base_permissions'], // 9
      'inventoryEveryOnePermissions' => $params['inventory_everyone_permissions'], // 10
      'salePrice' => $params['sale_price'], // 11
      'saleType' => $params['sale_type'], // 12
      'creationDate' => $params['creation_date'], // 13
      'groupID' => $params['group_id'], // 14
      'groupOwned' => $params['group_owned'], // 15
      'flags' => $params['flags'], // 16
      'inventoryID' => $params['inventory_id'], // 17
      'avatarID' => $params['uder_uuid'], // 18
      'parentFolderID' => $params['parent_folder_id'], // 19
    );

    d4os_io_db_070_set_active('os_robust');
    db_insert('inventoryitems')
        ->fields($values)
        ->execute();
    d4os_io_db_070_set_active('default');
  }

  function get_inventory_folder_by_path($params) {
    // build the query
    $values = array(':agentID' => $params['user_uuid']);
    $query = "SELECT * FROM {inventoryfolders} WHERE agentID = :agentID";

    // request the data
    $user_folders = array();
    d4os_io_db_070_set_active('os_robust');
    $results = db_query($query, $values)->fetchAssoc();
    foreach ($results as $result) {
      $user_folders[] = $result;
    }
    d4os_io_db_070_set_active('default');

    // no folder for the user, return an error
    if (count($user_folders) == 0) {
      return array(
        'success' => FALSE,
        'message' => t('Could not find folders for this user.')
      );
    }

    // get the parents
    $params['user_folders'] = $user_folders;
    $parents_folders = $this->get_folder_parents_by_path($params);
    $folder_uuid = array_pop($parents_folders);

    $data = array(
      'user_folders' => $user_folders,
      'folder_uuid' => $folder_uuid,
      'parents' => $parents_folders,
    );

    return array(
      'success' => TRUE,
      'message' => '',
      'data' => $data
    );
  }

  function get_folder_parents_by_path($params) {
    // get path args
    $args = explode('/', $params['path']);
    // get the first folder
    $parents_folders = array();
    foreach ($params['user_folders'] as $k => $v) {
      if ($v['folderName'] == 'My Inventory' && $v['parentFolderID'] = UUID_ZERO) {
        $parents_folders[] = $v['folderID'];
        break;
      }
    }
    // get the parents
    foreach ($args as $arg) {
      foreach ($params['user_folders'] as $k => $v) {
        if ($v['folderName'] == $arg && $v['parentFolderID'] = end($parents_folders)) {
          $parents_folders[] = $v['folderID'];
          break;
        }
      }
    }
    return $parents_folders;
  }

  function clone_folders($params) {
    $new_folders = array();
    // get the uuids
    $old_folders_uuids = array();
    foreach ($params['user_folders'] as $folder) {
      $old_folders_uuids[] = $folder->folderID;
      if ($folder->parentFolderID != UUID_ZERO) {
        $old_folders_uuids[] = $folder->parentFolderID;
      }
    }
    $old_folders_uuids = array_unique($old_folders_uuids);
    sort($old_folders_uuids);

    // generate new uuids
    $new_folders_uuids = array();
    foreach ($old_folders_uuids as $uuid) {
      $new_folders_uuids[] = d4os_uuid_create();
    }

    // replace the uuids
    foreach ($params['user_folders'] as $folder) {
      $values = $folder;

      $new_uuid_id = array_search($folder->folderID, $old_folders_uuids);
      $values->folderID = $new_folders_uuids[$new_uuid_id];

      if ($folder->parentFolderID != UUID_ZERO) {
        $new_parent_uuid_id = array_search($folder->parentFolderID, $old_folders_uuids);
        $values->parentFolderID = $new_folders_uuids[$new_parent_uuid_id];
      }

      $values->agentID = $params['avatar_dest_uuid'];
      $new_folders[] = $values;
    }

    $data = $params;
    $data['old_folders_uuids'] = $old_folders_uuids;
    $data['new_folders_uuids'] = $new_folders_uuids;
    $data['new_folders'] = $new_folders;

    return array(
      'success' => TRUE,
      'message' => '',
      'data' => $data
    );
  }

  function clone_items($params) {
    $new_items = array();

    // get the uuids
    $old_items_uuids = array();
    foreach ($params['user_items'] as $item) {
      $old_items_uuids[] = $item->inventoryID;
    }
    $old_items_uuids = array_unique($old_items_uuids);
    sort($old_items_uuids);

    // generate new uuids
    $new_items_uuids = array();
    foreach ($old_items_uuids as $uuid) {
      $new_items_uuids[] = d4os_uuid_create();
    }

    // replace the uuids
    foreach ($params['user_items'] as $item) {
      $new_uuid_id = array_search($item->inventoryID, $old_items_uuids);
      $new_parent_uuid_id = array_search($item->parentFolderID, $params['old_folders_uuids']);
      $values = $item;
      $values->inventoryID = $new_items_uuids[$new_uuid_id];
      $values->avatarID = $params['avatar_dest_uuid'];
      $values->parentFolderID = $params['new_folders_uuids'][$new_parent_uuid_id];
      $new_items[] = $values;
    }

    $data = $params;
    $data['old_items_uuids'] = $old_items_uuids;
    $data['new_items_uuids'] = $new_items_uuids;
    $data['new_items'] = $new_items;

    return array(
      'success' => TRUE,
      'message' => '',
      'data' => $data
    );
  }

  /**
   * @todo : pass by reference
   */
  function clone_inventory($params) {
    // delete actual folders and items
    d4os_io_db_070_set_active('os_robust');
    db_query("DELETE FROM {inventoryfolders} WHERE agentID = :agentID", array(':agentID' => $params['avatar_dest_uuid']));
    db_query("DELETE FROM {inventoryitems} WHERE avatarID = :avatarID", array(':avatarID' => $params['avatar_dest_uuid']));
    d4os_io_db_070_set_active('default');

    // get the folders
    $user_folders = array();
    d4os_io_db_070_set_active('os_robust');
    $results = db_query("SELECT * FROM {inventoryfolders} WHERE agentID = :agentID", array(':agentID' => $params['avatar_src_uuid']))->fetchAll();
    foreach ($results as $result) {
      $user_folders[] = $result;
    }
    d4os_io_db_070_set_active('default');

    // no folder for the user, return an error
    if (count($user_folders) == 0) {
      return array(
        'success' => FALSE,
        'message' => t('Could not find folders for this user.')
      );
    }

    // clone the folders
    $params['user_folders'] = $user_folders;
    $new_folders = $this->clone_folders($params);

    // get the items
    $user_items = array();
    d4os_io_db_070_set_active('os_robust');
    $results = db_query("SELECT * FROM {inventoryitems} WHERE avatarID = :avatarID", array(':avatarID' => $params['avatar_src_uuid']))->fetchAll();
    foreach ($results as $result) {
      $user_items[] = $result;
    }
    d4os_io_db_070_set_active('default');

    // no items for the user, return an error
    if (count($user_items) == 0) {
      return array(
        'success' => FALSE,
        'message' => t('Could not find items for this user.')
      );
    }

    // clone the items
    $params['user_items'] = $user_items;
    $params = array_merge($params, $new_folders['data']);
    $new_items = $this->clone_items($params);
    $params = array_merge($params, $new_items['data']);

    // build the query for folders
    $folders_list_values = array();
    foreach ($new_items['data']['new_folders'] as $new_folder) {
      $folders_list_values[] = array(
        'folderName' => $new_folder->folderName,
        'type' => $new_folder->type,
        'version' => $new_folder->version,
        'folderID' => $new_folder->folderID,
        'agentID' => $new_folder->agentID,
        'parentFolderID' => $new_folder->parentFolderID,
      );
    }

    // fill the base with all this data
    d4os_io_db_070_set_active('os_robust');
    $query = db_insert('inventoryfolders')->fields(array_keys($folders_list_values[0]));
    foreach ($folders_list_values as $record) {
      $query->values($record);
    }
    $query->execute();
    d4os_io_db_070_set_active('default');

    // build the query for items
    $items_list_values = array();
    foreach ($new_items['data']['new_items'] as $new_item) {
      $items_list_values[] = array(
        'assetID' => $new_item->assetID,
        'assetType' => $new_item->assetType,
        'inventoryName' => $new_item->inventoryName,
        'inventoryDescription' => $new_item->inventoryDescription,
        'inventoryNextPermissions' => $new_item->inventoryNextPermissions,
        'inventoryCurrentPermissions' => $new_item->inventoryCurrentPermissions,
        'invType' => $new_item->invType,
        'creatorID' => $new_item->creatorID,
        'inventoryBasePermissions' => $new_item->inventoryBasePermissions,
        'inventoryEveryOnePermissions' => $new_item->inventoryEveryOnePermissions,
        'salePrice' => $new_item->salePrice,
        'saleType' => $new_item->saleType,
        'creationDate' => $new_item->creationDate,
        'groupID' => $new_item->groupID,
        'groupOwned' => $new_item->groupOwned,
        'flags' => $new_item->flags,
        'inventoryID' => $new_item->inventoryID,
        'avatarID' => $new_item->avatarID,
        'parentFolderID' => $new_item->parentFolderID,
        'inventoryGroupPermissions' => $new_item->inventoryGroupPermissions,
      );
    }

    // fill the base with all this data
    d4os_io_db_070_set_active('os_robust');
    $query = db_insert('inventoryitems')->fields(array_keys($items_list_values[0]));
    foreach ($items_list_values as $record) {
      $query->values($record);
    }
    $query->execute();
    d4os_io_db_070_set_active('default');

    return array(
      'success' => TRUE,
      'message' => '',
      'data' => $params
    );
  }

  /**
   * @todo : pass by reference
   */
  function clone_appearance($params) {
    // delete actual appearance
    d4os_io_db_070_set_active('os_robust');
    db_query("DELETE FROM {Avatars} WHERE PrincipalID = :PrincipalID", array(':PrincipalID' => $params['avatar_dest_uuid']));
    d4os_io_db_070_set_active('default');

    // get the folders
    $appearance = array();
    d4os_io_db_070_set_active('os_robust');
    $results = db_query("SELECT Name, Value FROM {Avatars} WHERE PrincipalID = :PrincipalID", array(':PrincipalID' => $params['avatar_src_uuid']))->fetchAll();
    foreach ($results as $result) {
      $appearance[] = $result;
    }
    d4os_io_db_070_set_active('default');

    // no items for the user, return an error
    if (count($appearance) == 0) {
      return array(
        'success' => FALSE,
        'message' => t('Could not find appearance for this user.')
      );
    }

    // take values
    /*
      INSERT INTO `Avatars` (`PrincipalID`, `Name`, `Value`) VALUES
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'AvatarHeight', '1.990715'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'AvatarType', '1'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'METHOD', 'setavatar'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Serial', '0'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'UserID', 'f775ad73-cb26-4d76-ac1c-58d8593b54d1'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'VERSIONMAX', '0'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'VERSIONMIN', '0'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'VisualParams', '17,15,85,0,58,188,35,150,25,0,0,71,63,36,85,58,153,51,134,0,73,61,88,132,63,255,55,0,86,136,33,255,255,255,0,127,0,0,127,0,0,127,0,0,0,127,114,127,99,63,127,140,127,127,0,0,0,191,0,104,0,0,0,0,0,0,0,0,0,145,216,133,0,53,0,155,130,0,0,127,127,109,0,0,0,63,56,0,150,150,150,150,150,150,150,61,150,150,150,45,119,0,0,153,152,127,181,127,122,0,127,127,127,127,127,132,59,68,22,96,124,118,47,127,137,127,63,63,0,0,0,0,127,127,0,0,0,0,127,0,159,0,0,178,127,36,85,131,56,127,127,153,165,0,0,74,0,76,127,0,150,150,255,0,0,107,30,127,255,255,255,255,255,255,255,255,255,0,0,255,255,79,0,150,150,150,150,150,150,150,150,150,0,0,0,0,150,150,150,42,127,127,213,150,150,150,150,150,150,150,0,0,150,51,50,150,150,150'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Wearable 0:0', '49901b4c-64d5-4aee-b21c-5675026cf2b0:ea295fb0-ee17-405a-96ee-11e76b4c6568'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Wearable 1:0', '9bc3742a-662b-46e9-a407-e29344a62eee:0f3d20c7-9487-4604-b4b6-89808539de88'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Wearable 2:0', 'e1d5c309-a2f8-4f64-86e4-8acfbcd9bc3c:e75fab8a-16aa-4c42-8fa5-6e961053a5c0'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Wearable 3:0', 'a1aefbac-5f9a-4f94-b51f-0368defbce8e:3bdf0ce3-c7ba-4569-b951-bf0db63c94d0'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Wearable 4:0', 'd2bd636d-70ec-40da-aa54-b5589845d55f:d188e0eb-43b2-4df0-803b-221b485eaecb'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Wearable 5:0', 'a2669522-c2fa-4344-a1d7-4b0444b2f16c:d6439e46-44ae-420b-a34b-bb0061c0b949'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Wearable 6:0', '97ba86ad-94c5-4915-9257-255e09b6b140:094c171c-f423-40a6-9c6b-8be7b9d18514'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', 'Wearable 9:0', '2ff12cd5-ec1d-4096-8f80-170f6c91bb26:6ebee605-b2c9-4475-970f-11c08e48c979'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', '_ap_2', '0645e5ad-6ae4-4e3f-a976-7c2e0e76660c'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', '_ap_7', 'e8394d4f-a1c3-481c-b2e2-6609307c17c0'),
      ('f775ad73-cb26-4d76-ac1c-58d8593b54d1', '_ap_8', '73915a4e-9743-4897-aa66-f10fee793b25');
      UUID_INVENTORY_ITEM:UUID_ASSET
     */
    $values = array();

    for ($i = 0; $i < count($appearance); $i++) {
      if (strpos($appearance[$i]->Name, 'Wearable') !== FALSE) {
        $uuids = explode(":", $appearance[$i]->Value);
        $item = $uuids[0];
        $asset = $uuids[1];
        $query = "SELECT inventoryID FROM {inventoryitems} WHERE assetID=:assetID AND avatarID=:avatarID";

        d4os_io_db_070_set_active('os_robust');
        $result = db_query($query, array(':assetID' => $asset, ':avatarID' => $params['avatar_dest_uuid']))->fetchObject();
        d4os_io_db_070_set_active('default');
        if (!$result) {
          return;
        }
        //$appearance[$i]->Value = $result->inventoryID . ":" . $asset;
        $appearance[$i]->Value = $result->inventoryID;
      }
      elseif (strpos($appearance[$i]->Name, '_ap_') !== FALSE) {
        $query = "SELECT assetID FROM {inventoryitems} WHERE inventoryID=:inventoryID";

        d4os_io_db_070_set_active('os_robust');
        $result = db_query($query, array(':inventoryID' => $appearance[$i]->Value))->fetchObject();
        d4os_io_db_070_set_active('default');
        if (!$result) {
          return;
        }

        $query = "SELECT inventoryID FROM {inventoryitems} WHERE assetID=:assetID AND avatarID=:avatarID";

        d4os_io_db_070_set_active('os_robust');
        $result = db_query($query, array(':assetID' => $result->assetID, ':avatarID' => $params['avatar_dest_uuid']))->fetchObject();
        d4os_io_db_070_set_active('default');
        if (!$result) {
          return;
        }
        $appearance[$i]->Value = $result->inventoryID;
      }
      if ($appearance[$i]->Name == "UserID") {
        $appearance[$i]->Value = $params['avatar_dest_uuid'];
      }
      $values[] = array(
        'PrincipalID' => $params['avatar_dest_uuid'],
        'Name' => $appearance[$i]->Name,
        'Value' => $appearance[$i]->Value,
      );
    }

    // fill the base with all this data
    d4os_io_db_070_set_active('os_robust');
    $query = db_insert('Avatars')->fields(array_keys($values[0]));
    foreach ($values as $record) {
      $query->values($record);
    }
    $query->execute();
    d4os_io_db_070_set_active('default');

    return array(
      'success' => TRUE,
      'message' => '',
      'data' => $params
    );
  }

  function clone_model($params) {
    $params = $this->clone_inventory($params);
    drupal_set_message(t('Inventory folders created and default items added.'));

    $params = $this->clone_appearance($params['data']);
    drupal_set_message(t('Appearance added.'));
  }

  function delete_user_inventory($uuid) {
    // delete attachments and appearance
    d4os_io_db_070_set_active('os_robust');
    db_delete('Avatars')->condition('PrincipalID', $uuid)->execute();
    d4os_io_db_070_set_active('default');
    drupal_set_message(t('Appearance deleted.'));
    // delete folders
    d4os_io_db_070_set_active('os_robust');
    db_delete('inventoryfolders')->condition('agentID', $uuid)->execute();
    d4os_io_db_070_set_active('default');
    drupal_set_message(t('Inventory folders deleted.'));
    // delete items
    d4os_io_db_070_set_active('os_robust');
    db_delete('inventoryitems')->condition('avatarID', $uuid)->execute();
    d4os_io_db_070_set_active('default');
    drupal_set_message(t('Inventory items deleted.'));
  }

  function get_sub_folders($params) {
    $sub_folders = array();
    $query = "SELECT folderID FROM {inventoryfolders} WHERE agentID = :agentID AND parentFolderID = :parent";
    $values = array(':agentID' => $params['uuid'], ':parent' => $params['folder_uuid']);
    
    d4os_io_db_070_set_active('os_robust');
    $results = db_query($query, $values)->fetchAll();
    foreach ($results as $result) {
      $sub_folders[] = $result->folderID;
    }
    d4os_io_db_070_set_active('default');
    if (count($sub_folders)) {
      $this->sub_folders = array_merge($sub_folders, $this->sub_folders);
      foreach($sub_folders as $sub_folder) {
        $params['folder_uuid'] = $sub_folder;
        $this->get_sub_folders($params);
      }
    }
  }


  function empty_trash($params) {
    // get the root folder
    d4os_io_db_070_set_active('os_robust');
    $root_folder = db_query("SELECT folderID FROM {inventoryfolders} WHERE agentID=:agent_id and parentFolderID='00000000-0000-0000-0000-000000000000' and type = 8", array(':agent_id' => $params['uuid']))->fetchField();
    d4os_io_db_070_set_active('default');
    if ($root_folder === FALSE || is_null($root_folder)) {
      return array(
        'success' => FALSE,
        'message' => t('No root folder found'),
        'data' => array()
      );
    }

    // empty folders
    if ($params['select']['lost_and_found']) {

      // get "lost and found" folder
      d4os_io_db_070_set_active('os_robust');
      $lost_and_found_folder = db_query("SELECT folderID FROM {inventoryfolders} WHERE agentID=:agent_id and parentFolderID=:parent and type = 16", array(':agent_id' => $params['uuid'], ':parent' => $root_folder))->fetchField();
      d4os_io_db_070_set_active('default');

      // get subfolders
      $this->sub_folders = array($lost_and_found_folder);
      $params['folder_uuid'] = $lost_and_found_folder;
      $this->get_sub_folders($params);

      // delete items
      d4os_io_db_070_set_active('os_robust');
      $and = db_and()->condition('avatarID', $params['uuid'])->condition('parentFolderID', $this->sub_folders, 'IN');
      db_delete('inventoryitems')->condition($and)->execute();
      d4os_io_db_070_set_active('default');

      // delete folders
      d4os_io_db_070_set_active('os_robust');
      $and = db_and()->condition('agentID', $params['uuid'])->condition('parentFolderID', $this->sub_folders, 'IN');
      db_delete('inventoryfolders')->condition($and)->execute();
      d4os_io_db_070_set_active('default');

      drupal_set_message(t('"Lost and found" folder empty.'));
    }

    if ($params['select']['trash']) {

      // get "lost and found" folder
      d4os_io_db_070_set_active('os_robust');
      $trash_folder = db_query("SELECT folderID FROM {inventoryfolders} WHERE agentID=:agent_id and parentFolderID=:parent and type = 14", array(':agent_id' => $params['uuid'], ':parent' => $root_folder))->fetchField();
      d4os_io_db_070_set_active('default');

      // get subfolders
      $this->sub_folders = array($trash_folder);
      $params['folder_uuid'] = $trash_folder;
      $this->get_sub_folders($params);

      // delete items
      d4os_io_db_070_set_active('os_robust');
      $and = db_and()->condition('avatarID', $params['uuid'])->condition('parentFolderID', $this->sub_folders, 'IN');
      db_delete('inventoryitems')->condition($and)->execute();
      d4os_io_db_070_set_active('default');

      // delete folders
      d4os_io_db_070_set_active('os_robust');
      $and = db_and()->condition('agentID', $params['uuid'])->condition('parentFolderID', $this->sub_folders, 'IN');
      db_delete('inventoryfolders')->condition($and)->execute();
      d4os_io_db_070_set_active('default');

      drupal_set_message(t('"Trash" folder empty.'));
    }
  }
}
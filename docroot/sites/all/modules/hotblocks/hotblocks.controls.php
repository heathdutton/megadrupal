<?php

/**
 * Renders the controls (remove and update link) for a single hotblocks_item
 *
 * @param $oHotblocks_Item
 * @return string
 *  - HTML
 */
function hotblocks_item_controls($oHotblocks_Item) {
  $entity_id = $oHotblocks_Item->entity_id;
  $iDelta = $oHotblocks_Item->delta;
  $item_count = sizeof($GLOBALS['hotblock_items']);
  $minimum_items = hotblocks_get_setting('minimum_items', $iDelta);

  // We aren't going to show any item controls unless the user has access to at least one activity for the hotblock
  // This item block control function will return no text if there is no actions available for the block
  if (!hotblocks_access('any block controls', NULL, $iDelta)) {
    return array();
  }

  $render = array(
    '#type'       => 'container',
    '#attributes' => array(
      'class' => array(
        'hotblocks_item-controls',
        "hotblocks_item-$entity_id-$iDelta",
      )
    ),
    'links'       => array(
      'remove' => array(
        '#markup' => hotblocks_remove_link($oHotblocks_Item),
        // Access granted if user has access to remove items AND there is either no minimum or the item count is greater than the minimum
        '#access' => hotblocks_access('remove items from any hotblock', $entity_id, $iDelta) &&
          (empty($minimum_items) || (!empty($minimum_items) && $item_count > $minimum_items))
      ),
      'edit'   => array(
        '#markup' => hotblocks_edit_link($oHotblocks_Item),
        // Access granted if the entity is a drupal block and user can administer blocks OR it's another entity type and user has entity access
        '#access' => ($oHotblocks_Item->entity_type == 'block' && user_access('administer blocks')) ||
          ($oHotblocks_Item->entity_type != 'block' && entity_access('update', $oHotblocks_Item->entity_type, $oHotblocks_Item->entity)),
      ),
      'replace' => array(
        '#markup' => hotblocks_replace_link($oHotblocks_Item),
        '#access' => hotblocks_access('remove items from any hotblock', $entity_id, $iDelta) &&
          hotblocks_access('assign items to any hotblock', NULL, $iDelta)
      ),
    )
  );

  // @todo - Maybe? Old code returned no output at all if there were no links rendered

  return $render;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function hotblocks_remove_link($oHotblocks_Item) {
  $entity_id = $oHotblocks_Item->entity_id;
  $iDelta = $oHotblocks_Item->delta;
  $sPath = isset($_GET['path']) ? $_GET['path'] : $_GET['q'];
  $token = drupal_get_token("hotblocks-$iDelta-$oHotblocks_Item->hid");
  $aQuery = drupal_get_destination() + array('path' => $sPath, 'token' => $token, 'js' => 1);
  
  switch ($oHotblocks_Item->vis_type) {
    case 'all':
      $sText = 'Remove ' . hotblocks_terminology() . ' from all pages where this block is displayed.';
      $sImage = theme('image', array('path' => hotblocks_icon_path() . '/remove-global.png', 'alt' => $sText, 'title' => $sText));
      break;
    case 'group':
      $sText = 'Remove ' . hotblocks_terminology() . ' from all pages of this group.';
      $sImage = theme('image', array('path' => hotblocks_icon_path() . '/remove-global.png', 'alt' => $sText, 'title' => $sText));
      break;

    default:
    case 'page':
      $sText = 'Remove ' . hotblocks_terminology() . ' from this page';
      $sImage = theme('image', array('path' => hotblocks_icon_path() . '/remove.png', 'alt' => $sText, 'title' => $sText));
      break;
  }
  
  //Replace text with image if we're using icons
  if(hotblocks_get_setting('hotblocks_use_icons', TRUE)) {
    $sText = $sImage;
  }
  
  //Return the rendered link
  return l($sText, 'hotblocks/remove/' . $oHotblocks_Item->hid . '/' . $iDelta, array('query' => $aQuery, 'attributes' => array('rel' => 'hotblocks_itemblock-' . $iDelta, 'class' => array('hotblocks-remove')), 'html' => TRUE));
}

/**
 * Generate the link to replace an item in a hotblock with new content
 * @param $oHotblocks_Item
 * @return string
 */
function hotblocks_replace_link($oHotblocks_Item) {
  $entity_id = $oHotblocks_Item->entity_id;
  $iDelta = $oHotblocks_Item->delta;
  $sPath = isset($_GET['path']) ? $_GET['path'] : $_GET['q'];
  $token = drupal_get_token("hotblocks-$iDelta-$oHotblocks_Item->hid");
  $aQuery = drupal_get_destination() + array('path' => $sPath, 'token' => $token, 'replace' => $oHotblocks_Item->hid);

  $sText = 'Replace this item with new content';
  $sImage = theme('image', array('path' => hotblocks_icon_path() . '/replace.png', 'alt' => $sText, 'title' => $sText));

  // Replace text with image if we're using icons
  if(hotblocks_get_setting('hotblocks_use_icons', $iDelta)) {
    $sText = $sImage;
  }

  // Return the rendered link
  $options = array(
    'html'       => TRUE,
    'query'      => $aQuery,
    'attributes' => array(
      'rel'   => "hotblocks_itemblock-$iDelta",
      'class' => array('hb-modal', 'hotblocks-replace'),
    ),
  );
  return l($sText, "hotblocks/assign/$iDelta", $options);
}

/**
 * Generate the 'edit' link for an individual hotblock item
 */
function hotblocks_edit_link($oHotblocks_Item) {
  $sImage = theme('image', array('path' => hotblocks_icon_path() . '/edit.png', 'alt' => 'Edit'));
  $sText = hotblocks_get_setting('hotblocks_use_icons', $oHotblocks_Item->delta) ?  $sImage : 'Edit';
  $link_options = array(
    'query' => drupal_get_destination(),
    'attributes' => array('class' => array('hotblocks-edit')),
    'html' => TRUE
  );
  return l($sText, $oHotblocks_Item->edit_url, $link_options);
}

/**
 * Generates the link within the modal that assigns a particular item to the hotblock
 */
function hotblocks_assign_to_block_link($iDelta, $oHotblocks_Item) {
  if (!$oHotblocks_Item->entity_type) {
    $oHotblocks_Item->entity_type = 'node';
  }

  $text = $oHotblocks_Item->title;
  $entity_id = $oHotblocks_Item->entity_id;

  $aQuery = drupal_get_destination() + array(
    'entity_type' => $oHotblocks_Item->entity_type,
  );

  $data_hotblocks = $aQuery + array(
    'title'     => $text,
    'entity_id' => $entity_id,
    'friendlytype' => $oHotblocks_Item->friendlytype,
  );

  $aOptions = array(
    'attributes' => array(
      'class' => array('hotblocks-assign'),
    ),
    'query' => $aQuery,
  );

  // Only add 'data-hotblocks' when delta is 'wysiwyg' as this can add a lot of data to the modal request when there are
  // is tons of content to list (420 bytes per link). @todo - for scalability, javascript should derive the information in
  // data-hotblocks on its own if possible
  if($iDelta == 'wysiwyg') {
    // todo - is where data-hotblocks gets double encoded - because HTML isn't true?
    $aOptions['attributes']['data-hotblocks'] = htmlspecialchars(drupal_json_encode($data_hotblocks), ENT_QUOTES, 'UTF-8');
  }

  return l($text, 'hotblocks/assign/' . $entity_id . '/' . $iDelta, $aOptions);
}

////////////////////////////////////////////////////////////////////


/**
 * Renders the controls for a whole hotblocks_item block
 * Won't return anything if the user does not have acess to any of the hotblock's admin activities
 */
function hotblocks_block_controls($iDelta, $bBlockHasItems) {
  // When using contextual links for block controls, show some text indicating there are no items in this block
  if(hotblocks_get_setting('hotblocks_contextual_links', $iDelta) && !$bBlockHasItems) {
    return array('#markup' => "<div class='hotblocks-empty-block-with-contextual'><p>This region is currently empty and will not be displayed to end-users.</div>");
  }

  $render = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('hotblocks-controls')),
    'adminlabel' => array(
      '#type' => 'container',
      '#attributes' => array('class' => array('hotblocks-controls-text')),
      'content' => array('#markup' => 'Control ' . hotblocks_get_block_label($iDelta) .' content'),
    ),
    'assignlink' => array(
      '#markup' => hotblocks_assign_link($iDelta),
      '#access' => hotblocks_access('assign items to any hotblock', NULL, $iDelta),
    ),
    'reorderlink' => array(
      '#markup' => hotblocks_reorder_link($iDelta),
      // Must have access and more than one item to reorder
      '#access' => hotblocks_access('reorder items in any hotblock', NULL, $iDelta),
    )
  );

  return $render;
}

/**
 * Generates the link within the block to assign items there
 */
function hotblocks_assign_link($iDelta) {
  $sPath = isset($_GET['path']) ? $_GET['path'] : $_GET['q'];
  $aQuery = array ('path' => $sPath) + drupal_get_destination();
  $sImage = theme('image', array('path' => hotblocks_icon_path() . '/assign.png', 'alt' => 'Put a ' . hotblocks_terminology() . ' here'));
  $sText = hotblocks_get_setting('hotblocks_use_icons', $iDelta) ? $sImage : 'Put a ' . hotblocks_terminology() . ' here';
  $sLink = l($sText, 'hotblocks/assign/' . $iDelta, array('query' => $aQuery, 'attributes' => array('class' => array('hb-modal')), 'html' => TRUE));
  $sOutput = '<span class="hotblocks-assign-dialog">' . $sLink . '</span>';
  return $sOutput;
}

/**
 * Generates the link within the block to reorder items there
 */
function hotblocks_reorder_link($iDelta) {
  $sPath = isset($_GET['path']) ? $_GET['path'] : $_GET['q'];
  $sImage = theme('image', array('path' => hotblocks_icon_path() . '/reorder.png', 'alt' => 'Reorder ' . hotblocks_terminology(TRUE)));
  $sText = hotblocks_get_setting('hotblocks_use_icons', $iDelta) ?  $sImage : 'Reorder ' . hotblocks_terminology(TRUE);
  $sLink = l($sText, 'hotblocks/reorder/' . $iDelta, array('attributes' => array('class' => array('hotblocks-reorder')), 'html' => TRUE, 'query' => drupal_get_destination() + array('path' => $sPath)));
  $sOutput = '<span class="">' . $sLink . '</span>';
  return $sOutput;
}

//////////////CONTEXTUAL LINK RELATED////////////
/**
 * Renders the controls (remove and update link) for a single hotblock_item by adding them to the renderable array.
 *
 * @param object $hotblock_item
 * @param array $renderableArray
 * @return array
 */
function hotblocks_hotblocks_item_contextual_controls($hotblock_item, &$renderableArray) {
  $entity_id = $hotblock_item->entity_id;
  $iDelta = $hotblock_item->delta;
  $aLinks = array();

  //We aren't going to show any item controls unless the user has access to at least one activity for the hotblock
  //This item block control function will return no text if there is no actions available for the block
  if (!hotblocks_access('any block controls', NULL, $iDelta)) {
    return;
  }

  if (hotblocks_access('remove items from any hotblock', $entity_id, $iDelta)) {
    $aLinks['remove'] = hotblocks_contextual_item_remove_link($hotblock_item);
  }

  if (sizeof($aLinks) > 0) {
    $renderableArray['#contextual_links']['hotblocks-item'] = array(
      'hotblocks',
      array('entity_id' => $entity_id, 'delta' => $iDelta, 'hid' => $hotblock_item->hid)
    );
  }
}

/**
 * returns a contextual link for a hotblock 
 *
 * @param $iDelta - hotblocks block id
 */
function hotblocks_contextual_link($type, $iDelta) {
  $sPath = isset($_GET['path']) ? $_GET['path'] : $_GET['q'];
  
  $links = array(
    'assign' => array(
      'title' => t('Put a ' . hotblocks_terminology() . ' here'),
      'attributes' => array(
        'class' => array('hb-modal'),
      )
    ),
    /*'create' => array(
      'title' => t('Create a new ' . hotblocks_terminology()),
      'attributes' => array(
        'class' => array('hb-modal'),
      )
    ),*/
    'reorder' => array(
      'title' => t('Reorder ' . hotblocks_terminology(TRUE)),
      'attributes' => array(
        'class' => array('hotblocks-reorder'),
      )
    )
  );

  if ( isset($links[$type]) ) {
    $aQuery = array ('path' => $sPath) + drupal_get_destination();
    $sText = $links[$type]['title'];
    return array(
      'title' => $sText,
      'href' => 'hotblocks/'.$type.'/' . $iDelta,
      'query' => $aQuery,
      'attributes' => $links[$type]['attributes']
    );  
  }
  
  return FALSE; //unknown type parameter
}

/**
 * Return an array useable as a contextual link
 */
function hotblocks_contextual_item_remove_link($oHotblocks_Item) {
  $entity_id = $oHotblocks_Item->entity_id;
  $iDelta = $oHotblocks_Item->delta;
  $sPath = isset($_GET['path']) ? $_GET['path'] : $_GET['q'];
  $token = drupal_get_token("hotblocks-$iDelta-$oHotblocks_Item->hid");
  $aQuery = drupal_get_destination() + array('path' => $sPath, 'token' => $token, 'js' => 1);
  $sText = 'Remove ' . hotblocks_terminology() . ' from this page';
  return array(
    'title' => $sText,
    'href' => 'hotblocks/remove/' . $oHotblocks_Item->hid . '/' . $iDelta,
    'query' => $aQuery, 
    'attributes' => array('rel' => 'hotblocks_itemblock-' . $iDelta, 'class' => array('hotblocks-remove')),
  );
}
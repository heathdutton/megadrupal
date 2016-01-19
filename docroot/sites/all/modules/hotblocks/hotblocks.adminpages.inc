<?php

/**
 * Non-ajax page callback.
 * Prints a table of 'add node' links for each nodetype allowed to be a hotblocks_item
 */
function hotblocks_create_hotblocks_item_page() {
  // Render a 'create' link for each allowed bundle for each allowed entity type
  foreach (hotblocks_get_all_allowed_types() as $aEntityType) {
    foreach ($aEntityType as $bundle) {
      $aRows[] = array(l($bundle['label'], $bundle['create_url']));
    }
  }
  return theme('table', array('header' => array(), 'rows' => $aRows, 'caption' => t('Create Content')));

}

/**
 * Callback for 'overview by page'
 */
function hotblocks_overview_by_path() {
  $output = '<div>Global items will not be listed for every page that they will appear, since they would appear on every page of the site (provided their block is enabled).  Global items will be shown for the original page the were added.</div>';
  /////////////////////Hotblocks listed by page///////////////////
  $blocks = hotblocks_get_blocks();
  $pageresult = db_query("SELECT DISTINCT p.path FROM {hotblocks} p WHERE path != '' ORDER BY p.path");
  foreach ($pageresult as $oPage) {
    $result = db_query("SELECT n.title, n.type, p.* FROM {hotblocks} p LEFT JOIN {node} n ON n.nid = p.entity_id WHERE ((p.path = :path AND global = 0) OR (p.global = 1)) ORDER BY p.delta, p.weight, n.title", array(':path' => $oPage->path));
    foreach ($result as $oRow ) {
      if ($oRow->entity_type == 'block') {
        $block = db_query("SELECT * FROM {block} WHERE bid = :bid", array(':bid' => $oRow->entity_id))->fetchObject();
        $oRow->title = hotblocks_get_block_title_by_bid($block);
        $oRow->type = 'Block';
        
        $token = drupal_get_token("hotblocks-$oRow->delta-$oRow->hid");
        $aQuery = drupal_get_destination() + array('token' => $token);
        $actions = array(
          l('Configure block', "admin/structure/block/configure/$block->module/$block->delta", array('query' => drupal_get_destination())),
          l('Remove', "hotblocks/remove/$oRow->hid/$oRow->delta", array('query' => $aQuery)),
        );
      }
      elseif ($oRow->entity_type == 'node') {
        $oRow->type = node_type_get_name($oRow->type);

        $token = drupal_get_token("hotblocks-$oRow->delta-$oRow->hid");
        $aQuery = drupal_get_destination() + array('token' => $token);
        $actions = array(
          l('View', "node/$oRow->entity_id"),
          l('Edit', "node/$oRow->entity_id/edit", array('query' => drupal_get_destination())),
          l('Remove', "hotblocks/remove/$oRow->hid/$oRow->delta", array('query' => $aQuery)),
          l('Delete', "node/$oRow->entity_id/delete", array('query' => drupal_get_destination())),
        );
      }
      else {
        hotblocks_item_preload($oRow);
        $oRow->type = '(' . $oRow->entity_type . ') ' . $oRow->entity_wrapper->getBundle();
      }

      if ($oRow->global == 1) {
        $page = '(Global)';
      }
      else {
        $page = 'No'; //l(drupal_get_path_alias($oRow->path),$oRow->path);
      }

      $aTableRows[] = array(
        $oRow->entity_type == 'node' ? l($oRow->title, "node/$oRow->entity_id") : $oRow->title,
        $oRow->type,
        $page,
        $blocks[$oRow->delta], // Block label
        $oRow->weight ? $oRow->weight : '(none)',
        implode(' | ', $actions),
      );

    }
    $aHeader = array('Name', 'Content Type', 'Global', 'Block', 'Weight', 'Action');
    if (sizeof($aTableRows) < 1) {
      $aTableRows = array(array('None'));
      $aHeader = array();
    }
    $output .=  '<div class="admin-panel">' . theme('table', array('header' => $aHeader, 'rows' => $aTableRows, 'attributes' => array(), 'caption' => "<h3>Page: " . l(drupal_get_path_alias($oPage->path), $oPage->path) . "</h3>")) . '</div>';
    unset($aTableRows);
  }

  return $output;
}

/**
 * @TODO Regular block display prefers to show the following order:
 * 1. Unweighted globals (not ordered or ordered as the DB prefers)
 * 2. All weighted hotblocks (global or not) for the block and path unless we are on node/add in which case only globals.
 * 3. Unweighted non-globals for the block and path
 * However, the overview functions currently sort by weight & path (this function) block, weight, & title
 *
 *
 * @return unknown
 */
function hotblocks_overview_by_block() {
  $output = '';

  /////////////////////Hotblocks listed by block///////////////////
  $blocks = hotblocks_get_blocks();
  foreach ($blocks as $iDelta => $name) {
    $aTableRows = array();
    $result = db_query("SELECT n.title, n.type, p.* FROM {hotblocks} p LEFT JOIN {node} n ON n.nid = p.entity_id  WHERE p.delta = :delta ORDER BY p.weight, p.path", array(':delta' => $iDelta));
    foreach($result as $oRow) {
      if ($oRow->entity_type == 'block') {
        $block = db_query("SELECT * FROM {block} WHERE bid = :bid", array(':bid' => $oRow->entity_id))->fetchObject();
        $oRow->title = hotblocks_get_block_title_by_bid($block);
        $oRow->type = 'Block';

        $token = drupal_get_token("hotblocks-$oRow->delta-$oRow->hid");
        $aQuery = drupal_get_destination() + array('token' => $token);
        $actions = array(
          l('Configure block', "admin/structure/block/configure/$block->module/$block->delta", array('query' => drupal_get_destination())),
          l('Remove', "hotblocks/remove/$oRow->hid/$oRow->delta", array('query' => $aQuery)),
        );
      }
      elseif ($oRow->entity_type == 'node') {
        $oRow->type = node_type_get_name($oRow->type);

        $token = drupal_get_token("hotblocks-$oRow->delta-$oRow->hid");
        $aQuery = drupal_get_destination() + array('token' => $token);
        $actions = array(
          l('View', "node/$oRow->entity_id"),
          l('Edit', "node/$oRow->entity_id/edit", array('query' => drupal_get_destination())),
          l('Remove', "hotblocks/remove/$oRow->hid/$oRow->delta", array('query' => $aQuery)),
          l('Delete', "node/$oRow->entity_id/delete", array('query' => drupal_get_destination())),
        );
      }
      else {
        hotblocks_item_preload($oRow);
        $oRow->type = '(' .  $oRow->entity_type . ') ' . $oRow->entity_wrapper->getBundle();
      }

      if ($oRow->global == 1) {
        $page = '(Global)';
      }
      else {
        $page = l(drupal_get_path_alias($oRow->path), $oRow->path);
      }

      $aTableRows[] = array(
        $oRow->entity_type == 'node' ? l($oRow->title, "node/$oRow->entity_id") : $oRow->title,
        $oRow->type,
        $page,
        $oRow->weight ? $oRow->weight : '(none)',
        implode(' | ', $actions),
      );

    }
    $aHeader = array('Name', 'Content Type', 'Page', 'Weight', 'Action');
    if (sizeof($aTableRows) < 1) {
      $aTableRows = array(array('None'));
      $aHeader = array();
    }
    $output .=  '<div class="admin-panel">' . theme('table', array('header' => $aHeader, 'rows' => $aTableRows, 'attributes' => array(), 'caption' => "<h3>Block: $name</h3>")) . '</div>';
  }

  return $output;
}

/////////////Functions related to taxonomy overview page:
/**
 * Would be nice to have the option of showing all terms in an opened state, or remove toggling altogether so they dont have to be opened everytime
 *
 * @param $iDelta - hotblocks_item block id
 * @return
 * Rendered markup of taxonomy tree with all hotblocks_item nodes shown
 */
function hotblocks_taxonomy_page($iDelta = 0) {
  if (!module_exists('taxonomy')) {
    return 'The taxonomy module must be enabled to user this feature.';
  }
  $aTree = taxonomy_get_tree(hotblocks_get_setting('hotblocks_vocabulary', $iDelta), 0, 1);
  return '<div class="hotblocks-taxonomy">' . hotblocks_taxonomy_build($aTree, $iDelta) . '</div>';
}

/**
 * Generate category trees for taxonomies
 */
function hotblocks_taxonomy_build($aTree, $iDelta = 0, $depth = -1) {
  $output = '';
  //if($bExpandable) { //@todo provide option for always expanded?
  $sToggler = hotblocks_toggler();
  //}
  $depth++;
  foreach ($aTree as $oTerm) {
    $oTerm->depth = $depth;
    $bHasChildren = false;

    $result = db_query("SELECT tid FROM {taxonomy_term_hierarchy} WHERE parent = :parent", array(':parent' => $oTerm->tid));
    while ($row = db_fetch_object($result)) {
      $bHasChildren = true;
    }
    $count = taxonomy_term_count_nodes($oTerm->tid);
    if ($count > 0) {
      $bHasChildren = true;
    }

    $link = '<span class="hotblocks_item-folder">' . $oTerm->name . '</span>';

    //Tack on the toggler control if we are using expandable controls
    if (!$bHasChildren) {
      $sToggler = null;
      $toggleable = '';
      $toggleRow = '';
    }
    else {
      $toggleable = 'toggleable';
      $toggleRow = 'toggleRow';
    }

    //////List items in the term
    $result = taxonomy_select_nodes(array($oTerm->tid));
    $kids = null;
    while ($oRow = db_fetch_object($result)) {
      if (!hotblocks_access('assign items to any hotblock', db_query("SELECT type FROM {node} WHERE nid = :nid", array(':nid' => $oRow->entity_id))->fetchField(), $iDelta) || !hotblocks_access('view node', $oRow->entity_id)) {
        continue;
      }
      $sImage = theme('image', array('path' => hotblocks_icon_path('16x16') . '/page-icon.png', 'width' => 'Folder full', 'height' => 'Folder full', 'alt' => array('align' => 'absmiddle')));
      if ($iDelta > 0) {
        //If we have a delta, we'll provide links that will assign the hotblocks_item to that block
        $nlink = hotblocks_assign_to_block_link($iDelta, $oRow);
      }
      else {
        //Show regular node link
        $nlink = l($oRow->title, "node/$oRow->entity_id");
      }
      $kids .= '<div class="prow hb-depth-' . ($oTerm->depth + 1) . ' ' . hotblocks_even_odd() . '">' . $sImage . $nlink . '</div>';
    }

    //////Make term markup with an icon indicating whether or not it is empty
    if ($count > 0) {
      $sImage = theme('image', array('path' => hotblocks_icon_path('16x16') . '/folder-full.png', 'width' => 'Folder full', 'height' => 'Folder full', 'alt' => array('align' => 'absmiddle')));
    }
    else {
      $sImage = theme('image', array('path' => hotblocks_icon_path('16x16') . '/folder.png', 'width' => 'Folder empty', 'height' => 'Folder empty', 'alt' => array('align' => 'absmiddle')));
    }
    $output .=
      '<div rel="' . $oTerm->depth . '" class="prow ' . $toggleable . ' hb-depth-' . $oTerm->depth . ' vid-' . $oTerm->vid . ' first-letter-' . substr($oTerm->name, 0, 1) . '">' .
         '<div class="' . $toggleRow . '">' .
           $sToggler .
           $sImage .
           $link .
         '</div>' .
        hotblocks_taxonomy_build(taxonomy_get_tree($oTerm->vid, $oTerm->tid, 1), $iDelta, $depth) .
        $kids .
      '</div>';
  }
  return $output;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function hotblocks_toggler($class = "toggle", $sExtraText = null, $bExpanded = false, $bInlineStyles = true) {
  $sExtraText = $sExtraText ?  ' ' . $sExtraText : null;
  $a = '';
  $b = '';

  if ($bInlineStyles) {
    if ($bExpanded) {
      $a = ' style="display:none;" ';
    }
    else {
      $b = ' style="display:none;" ';
    }
  }

  return strtr('
	<span class="!classContainer">
		<span class="!class expand" !a>
			<a href="javascript:;" class="expand">[+]' . $sExtraText . '</a>
		</span>
		<span class="!class contract" !b>
			<a href="javascript:;" class="contract">[-]' . $sExtraText . '</a>
		</span>
	</span>', array('!class' => $class, '!a' => $a, '!b' => $b));
}

/**
 * Return the string 'hotblocks-even' or 'hotblocks-odd' in alternating succession.  Starts with 'hotblocks-even';
 * Used for css classes.
 */
function hotblocks_even_odd() {
  static $i = 'hotblocks-odd';
  if ($i == 'hotblocks-odd') {
    $i = 'hotblocks-even';
  }
  else {
    $i = 'hotblocks-odd';
  }

  return $i;
}
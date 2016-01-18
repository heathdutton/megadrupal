<?php
/**
 @file
	Template for the the BrowseNodes content_type panel for an amazon product page
*/

if ($item->BrowseNodes->BrowseNode) {
  $ProductGroup = (string)$item->ItemAttributes->ProductGroup;
  $SearchIndex = ProductGroup2SearchIndex($ProductGroup);

  print "<ul>";
  foreach ($item->BrowseNodes->BrowseNode as $BrowseNode) {
    $item = $BrowseNode;
    $line = "";
    do {
      // If it has a paren in it, it's a consolidation node, and
      // not useful to end-user
      if (strstr($item->Name, "(")) {
        $line = "";
        break;
      }

      $line .= l($item->Name, AMAZON_STORE_PATH, array('attributes' => array('rel' => 'nofollow'), 'query' => "BrowseNode={$item->BrowseNodeId}&SearchIndex={$SearchIndex}")) ." : ";
    } while ($item = $item->Ancestors->BrowseNode);
    if (strlen($line)) {
      print "<li>$line</li>";
    }
  }
  print "</ul>";
}
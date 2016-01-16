<?php
/**
* @file
* The theme system, which controls the output of Drupal.
*
* The theme system allows for nearly all output of the Drupal system to be
* customized by user themes.
*/
?>

<?php
  // adds <UL> for each menu
  function darkritual_menu_tree($variables) {
    return '<ul>' . $variables['tree'] . '</ul>';
  }

  
  function darkritual_preprocess_node(&$variables) {
    $variables['content']['links'] = FALSE; // This is to remove the 'Read more' link
    
    // changes the date published of node to display as 
    $node = $variables['node'];
    $variables['date'] = format_date($node->created, 'custom', 'jS F,Y');
}

<?php
/**
 * Node block theming.
 *
 * This template is called when nodes are placed in a block via felix.
 *
 * Use felix-node--NODETYPE.tpl.php for type specific theming.
 *
 * Available variables:
 * - $node:         Loaded node object.
 * - $content:      Content array (render array based on the view mode
 *                  configured for the Felix region, node-object contained in it
 *                  contains no title to prevent the theme layer from rendering
 *                  it (Felix uses it as the block title, if you really need the
 *                  node title, it is available in the above node object).
 * - $full_content: Content array for full node (render array based on the
 *                  'full' view mode, node-object contained in it contains
 *                  no title to prevent the theme layer to render it (Felix uses
 *                  it as the block title).
 * - $view_mode:    The view mode for this node.
 */
?>
<?php print drupal_render($content); ?>
--- Summary ---

An easy way to create blocks with a collection of node fields. 
Focused on letting customers manage most of the content on a 
page in just one node. You can create multiple blocks for a 
node with one or a combination of fields and move them in different 
regions.

--- Features ---

- Create one or more node fields blocks
- Add the node types you want to use it for
- Add the fields you want to show in the block
- Manage the formatting at the node fields block display settings
- Fully themeable

--- How to use ---

- Give the required permissions to the roles
- admin/structure/block/add-node-fields-block to create a new block
- select the node types you want to use
- select the fields you want to show in the block
- Go to the block administration page and move the block into a region
- Optional go to node display settings and change the settings for node
  fields block.
- Hide the fields in the default display and show them in the
  node fields block display.
- Optional theme your blocks by overwriting the theme functions.
  
 --- Theming ---
 
- Theme your blocks by overwriting node_fields_block_block with 
  node_fields_block_block.tpl.php.
- Theme your blocks specifically for a node type with
  node_fields_block_block--[node_type].tpl.php.
- Theme per field inside a block by overwriting node_fields_block_field
  with node_fields_block_field.tpl.php.
- Theme your fields specifically for a field with
  node_fields_block_field--[field_name].tpl.php.


--- Similar modules ---

Field as Block
Biggest difference is that with Node Fields block you can add 
multiple blocks for one node and group one or more fields. 
Field as Block is more focused on call to action blocks and 
Node Fields Block is more focused on managing most of the content 
on a page in the node.

--- Contact ---

To contact me you can send an e-mail to bvdhoek@gmail.com or find me 
on drupal.org with the username barthje.

This module is is sponsored by Synetic: http://www.synetic.nl

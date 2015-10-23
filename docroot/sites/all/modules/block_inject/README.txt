BLOCK INJECT

--------------------------------------------

The Block Inject module allows for regions to be created and injected in the 
middle of chosen node types.

--------------------------------------------

> Install and enable like usual.

--------------------------------------------

Navigate to admin/structure/block-inject and create your region(s). You can 
assign any number of node types you want the region be injected in the middle
of. 

Then add your blocks to the newly created region(s) and these will appear after 
the middle paragraph of each node of the respective content type if there are 
enough paragraphs (2).

--------------------------------------------

Block Inject also makes available an alter hook that allows you to add or change
the attributes that are applied to the wrapper div surrounding the injected
region. 

Example implementation:

/**
 * Alters injected region attributes.
 *
 * @param array $attributes
 *   Contains the node object currently being injected into (non-alterable) and
 *   the attributes array (alterable) that by default includes the id and class. 
 *   You can add more classes or change the id but also add other types of
 *   attributes if needed.
 *
 *   Additionally, it contains a clearfix boolean with which you can remove the
 *   clearfix div being added after the region.
 */
 
 function hook_block_inject_attributes_alter($attributes) {
   if ($attributes['node']->type == 'article') {
     // Adding a custom class to the region wrapper
     $attributes['attributes']['class'][] = 'article-block-inject';
     // Removing the clearfix that comes after the region wrapper
     $attributes['clearfix'] = false;
   }
 }

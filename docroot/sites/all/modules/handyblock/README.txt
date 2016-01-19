Provides blocks that contain the current entity (among other things)
as context, and that are super-easy to theme.


Project info at:
http://drupal.org/project/handyblock


This is a convenience module, for module developers and for themers.
All it really does, is automate a few hook implementations for you.
By implementing the Handy Block theme callback function, Handy Block
implements hook_theme(), hook_block_info(), and hook_block_view()
for you.

If you often have a bunch of custom modules on your site, that do
nothing except implement these three hooks (along with block callback
functions), for blocks that do little more than display some fields
for the entity currently being viewed, then Handy Block should...
well, it should come in handy! You'll be able to do the same thing
in just a few lines of your template.php file; and then, you can
delete those custom modules of yours altogether.

Basic usage:

Say you want to show the 'sidebar_text' field of your nodes in a
block. Start by implementing the 'handyblock' theme callback. Place
this code in your theme's template.php file:

/**
 * Handy Block theme callback implementation.
 */
function YOURTHEME_handyblock() {
  return array(
    'sidebar_text' => array(
      'block_info' => t('YOURTHEME sidebar text'),
      'handyblock_context' => 'curr_page_node',
      'theme_variables' => array('sidebar_text'),
    ),
  );
}

In this callback, each key of the returned array is the machine-
readable name (or the 'delta') of the block. Within each nested array,
there are the following:

  - 'block_info': the value of this gets put directly into the
    block definition array for hook_block_info(), by Handy Block. So,
    this should generally be an array containing things such as 'info',
    'weight', 'region', etc. You can also set this to a string, instead
    of an array; and Handy Block will transform it into an array, and
    will use your string as the 'info' value, before passing it on to
    hook_block_info() (this has been done in the example above).
  
  - 'handyblock_context': the Handy Block context callback to use.
    Currently supported values are: 'curr_page_node', 'curr_page_user',
    and 'curr_page_term'.
  
  - 'theme_variables': an array of variable names that you want to be
    able to pass to the template for this block. All variables that
    you pass via a Handy Block 'alter' callback (see below), should
    be listed here.

Then, add this function to your template.php file as well:

/**
 * Handy Block alter callback for block 'sidebar_text'.
 */
function YOURTHEME_handyblock_sidebar_text_alter(&$block, $context) {
  $node = $context['node'];
  if (empty($node->field_sidebar_text['und'][0]['safe_value'])) {
    $block = NULL;
    return;
  }
  
  $block['content']['#sidebar_text'] = $node->field_sidebar_text['und'][0]['safe_value'];
}

In this function, you can control block visibility (setting the
block object to NULL, as done above, effectively hides the block);
you can pass variables to the block template; and you can do pretty
much anything else you want, as part of building the block. The
$context argument contains the current page node in its 'node'
element; this has been provided by the 'curr_page_node' context
callback that was specified.

Then, in your theme directory, create a file called
handyblock-sidebar-text.tpl.php, that looks something like this:

<?php print $sidebar_text; ?>

Once you've done all that, clear your cache, and then make your new
block display, by assigning it to a region (on the 'administration ->
structure -> blocks' page).

The block should now appear for you. You can add as many blocks as you
like in this fashion. The Handy Block 'alter' callback lets you pass
whatever variables you like to the template (and it lets you easily
control block visibility, too). And each block will be displayed
exactly as you theme it, per the handyblock-BLOCKNAME.tpl.php file
that you create for each block.

Happy theming!

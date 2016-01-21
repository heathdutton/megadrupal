Chaos Blocks (ctools_block) implements a new ctools plugin type that wraps the
core blocks API in order to make it easier to write custom blocks in code.

Let's say you wanted a really simple block that has subject of 'foo' and content
of 'bar'.

Core API:

<code>
/**
 * Implements hook_block_info().
 */
function MYMODULE_block_info() {
  $blocks = array();

  $blocks['simpleblock'] = array(
    // some options in here.
  );
}

/**
 * Implements hook_block_view().
 */
function MYMODULE_block_view($delta = '') {
  $block = array();

  // I dunno about you but this big ol' switch here doesn't feel great to me.
  switch ($delta) {
    case 'simpleblock':
      $block['subject'] = t('foo');
      $block['content'] = array(
        '#markup' => t('bar'),
      );
      break;
  }

  return $block;
}
</code>

This approach gets nasty as you scale your site, adding more blocks and your
info and view hooks, until they become completely illegible.

Using ctools plugins, we setup each of the core block hooks for each module ONCE
using 100% boilerplate, reusable code, and never need to touch them again. We
create a plugin include file for each of our blocks, it will be automatically
detected and then all the logic for each block will be available in a single,
dedicated file.

Examples can be found in ctools_block_example.module, but here's the idea:

<code>
// Inside MYMODULE.module:

/**
 * Implements hook_ctools_plugin_directory().
 */
function MYMODULE_ctools_plugin_directory($module, $plugin) {
  if ($module === 'MYMODULE' && $plugin === 'block') {
    return 'plugins/' . $plugin;
  }
}

/**
 * Implements hook_ctools_plugin_type().
 */
function MYMODULE_ctools_plugin_type() {
  return ctools_block_ctools_plugin_type();
}

/**
 * Implements hook_block_info().
 */
function MYMODULE_block_info() {
  return ctools_block_handle_block_info('MYMODULE');
}

/**
 * Implements hook_block_view().
 */
function MYMODULE_block_view($delta = '') {
  return ctools_block_handle_block_view($delta, 'MYMODULE');
}

/**
 * Implements hook_block_configure().
 */
function MYMODULE_block_configure($delta = '') {
 return ctools_block_handle_block_configure($delta, 'MYMODULE');
}

/**
 * Implements hook_block_save().
 */
function MYMODULE_block_save($delta = '', $edit = array()) {
  return ctools_block_handle_block_save($delta, $edit, 'MYMODULE');
}
</code>

That code above might look scary, but you just copy and paste it every time you
create a new module then forget about it. Once you have those hooks in place,
you can create a file inside your module like
<code>plugins/block/myblock.inc</code> which, to achieve the same thing as the
core API above, simply looks like:

<code>
<?php
$plugin = array(
  'info' => 'Simple block',
  'subject' => t('foo'),
  'content' => array(
    '#markup' => t('bar'),
  ),
  'options' => array(
    // Core block options go here.
  ),
);
</code>

See how we have everything required to build and render our block in the one
place, and it's not interleaved with the definitions of other, unrelated blocks?

In addition to 'info', 'options', subject' and 'content', ctools_block supports
'subject callback', 'content callback', 'configure callback', and
'save callback' so you can write your own functions to build custom blocks that
are simple to maintain but still dynamic.

NOTE: Do NOT put theme functions in your plugin includes. Do to the way ctools
$plugin arrays work, this can lead to race conditions between theme registry
building and plugin registry building where your block will disapear.

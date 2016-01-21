<?php
/**
 * @file
 * Hook information for Extensions.
 *
 * @copyright Copyright(c) 2012 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

/**
 * Generic callback to help loading extension information.
 *
 * There are two ways to declare extensions. The first is to find a logical
 * point in your code to declare the extensions using
 * extensions_register_extension(). The second is to use one of these hooks,
 * then call extensions_parse_extensions() before accessing your extensions.
 *
 * This hook returns nothing, it is expected that you'll merely use it as a
 * wrapper for calling extensions_register_extension().
 */
function hook_extensions_declare() {

  extensions_register_extension('my_extension_type', 'my_extension_name')
    ->args(array('an_argument' => array()))
    ->className('\My\Class\Name');
}

/**
 * Generic callback to help loading extension information for a collection.
 *
 * This works the same way as hook_extensions_declare() except it only fires
 * when the COLLECTION_NAME component matches the extension collection
 * requested.
 *
 * It is preferable to use this over hook_extensions_declare, unless you don't
 * know the collection name at the time of calling extensions_parse_extensions().
 */
function hook_extensions_declare_COLLECTION_NAME() {

  extensions_register_extension('my_extension_type', 'my_extension_name')
    ->args(array('an_argument' => array()))
    ->className('\My\Class\Name');
}
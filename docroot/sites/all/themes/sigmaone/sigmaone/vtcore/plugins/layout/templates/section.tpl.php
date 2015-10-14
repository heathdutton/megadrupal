<?php
/**
 * Template wrapper for each section
 * @see layout_vtcore_preprocess_section(&$variables)
 *      in layout.plugin
 *
 * It is possible to manipulate content before
 * the content is printed by render() function
 * by changing the content of $elements['#children'] arrays
 *
 * To build the page without using html5 elements
 * you need to change the element tags string into
 * something like div
 *
 * example <<?php print $elementtag; ?> will become simply <div
 * and the closure of </<?php print $elementtag; ?>> will become
 * simply </div>
 *
 * Note when you change the html5 elements, the css will need
 * to adapt the new tag as well.
 *
 * This template is connected to .layout files
 * you can change :
 * 1. $elementtag
 * 2. $attributes_parent
 * 3. $wrappertag
 * 4. $attributes_wrapper
 *
 * directly from .layout file by invoking something like this
 * area[myarea][#tag] = div
 * area[myarea][#attributes][id] = myid
 * area[myarea][#attributes][class][@see key] = myclass
 * area[myarea][#attributes][class][@see key] = another class
 * area[myarea][#tag_wrapper] = div
 * area[myarea][#attributes_wrapper][id] = myid
 * area[myarea][#attributes_wrapper][class][@see key] = myclass
 * area[myarea][#attributes_wrapper][class][@see key] = another class
 *
 * @see Key
 * Plugin will try to merge the default classes array with user defined classes
 * thus it is adviseable to use key larger than 20 to add new class if you
 * don't want to overwrite the default classes and use specific key if corresponding
 * to the default classes if you wish to override them.
 */
?>

<<?php print $tag; ?><?php print $element_attributes; ?>>

  <?php /** Only build wrapper if configured **/?>
  <?php if ($wrapper) : ?>
    <<?php print $tag_wrapper;?> <?php print $wrapper_attributes;?>>
  <?php endif; ?>

  <?php /** Rendering content **/ ?>
  <?php print drupal_render_children($section); ?>

  <?php /** Only build wrapper closure if configured **/ ?>
  <?php if ($wrapper) : ?>
    </<?php print $tag_wrapper; ?>>
  <?php endif;?>

</<?php print $tag; ?>>
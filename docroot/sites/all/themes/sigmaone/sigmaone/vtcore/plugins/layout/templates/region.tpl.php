<?php
/**
 * Template wrapper for each region
 * @see layout_vtcore_preprocess_region(&$variables)
 *      in layout.plugin
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
 * 1. $tag
 * 2. $element_attributes
 *
 * directly from .layout file by invoking something like this
 * region[myregion][#tag] = div
 * region[myregion][#attributes][id] = myid
 * region[myregion][#attributes][class][] = myclass
 * region[myregion][#attributes][class][] = another class
 */
?>
<?php if (!empty($content) || !empty($element_attributes)) : ?>
  <<?php print $tag?> <?php print $element_attributes; ?>>
      <?php print $content; ?>
  </<?php print $tag?>>
<?php endif; ?>
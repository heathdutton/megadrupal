<?php

/**
 * Returns HTML for a template variable wrapped in an HTML element with the
 * RDF attributes.
 *
 * This is called by rdf_process() shortly before the theme system renders
 * a template file. It is called once for each template variable for which
 * additional attributes are needed. While template files are responsible for
 * rendering the attributes for the template's primary object (via the
 * $attributes variable), title (via the $title_attributes variable), and
 * content (via the $content_attributes variable), additional template
 * variables that need containing attributes are routed through this function,
 * allowing the template file to receive properly wrapped variables.
 *
 * Tip for themers: if you're already outputting a wrapper element around a
 * particular template variable in your template file, and if you don't want
 * an extra wrapper element, you can override this function to not wrap that
 * variable and instead print the following inside your template file:
 * @code
 *   drupal_attributes($rdf_template_variable_attributes_array[$variable_name])
 * @endcode
 *
 * @param $variables
 *   An associative array containing:
 *   - content: A string of content to be wrapped with attributes.
 *   - attributes: An array of attributes to be placed on the wrapping element.
 *   - context: An array of context information about the content to be wrapped:
 *     - hook: The theme hook that will use the wrapped content. This
 *       corresponds to the key within the theme registry for this template.
 *       For example, if this content is about to be used in node.tpl.php or
 *       node-[type].tpl.php, then the 'hook' is 'node'.
 *     - variable_name: The name of the variable by which the template will
 *       refer to this content. Each template file has documentation about
 *       the variables it uses. For example, if this function is called in
 *       preparing the $author variable for comment.tpl.php, then the
 *       'variable_name' is 'author'.
 *     - variables: The full array of variables about to be passed to the
 *       template.
 *   - inline: TRUE if the content contains only inline HTML elements and
 *     therefore can be validly wrapped by a <span> tag. FALSE if the content
 *     might contain block level HTML elements and therefore cannot be validly
 *     wrapped by a <span> tag. Modules implementing preprocess functions that
 *     set 'rdf_template_variable_attributes_array' for a particular template
 *     variable that might contain block level HTML must also implement
 *     hook_preprocess_rdf_template_variable_wrapper() and set 'inline' to FALSE
 *     for that context. Themes that render normally inline content with block
 *     level HTML must similarly implement
 *     hook_preprocess_rdf_template_variable_wrapper() and set 'inline'
 *     accordingly.
 *
 * @see rdf_process()
 * @ingroup themeable
 * @ingroup rdf
 */
function twbs_rdf_template_variable_wrapper($variables) {
  $output = $variables['content'];
  if (!empty($output) && !empty($variables['attributes'])) {
    $attributes = drupal_attributes($variables['attributes']);
    $output = $variables['inline'] ? "<span$attributes>$output</span>" : "<div$attributes>$output</div>";
  }
  return $output;
}

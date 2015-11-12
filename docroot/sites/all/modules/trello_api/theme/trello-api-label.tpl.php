<?php
/**
 * @file
 * Default theme implementation for trello labels.
 *
 * Available variables:
 * - $name: The (sanitized) name of the color.
 * - $color_class: The class name for the color
 *
 * @see template_preprocess()
 * @see template_preprocess_trello_api_label()
 * @see template_process()
 */
?>
<span title="<?php print $name; ?>" class="trello-api-label <?php print $color_class; ?>"><?php print $name; ?></span>

<?php
/**
 * @file
 *   Outputs the Flash XML node for piecemaker xml files
 *
 * @variables
 *   $item: An associative array. @see template_preprocess_piecemaker_xml for details
 */
?>
<Video <?php print drupal_attributes($item['#attributes']); ?>>
  <?php if (!empty($item['Image']['#attributes'])):?>
  <Image <?php print drupal_attributes($item['Image']['#attributes']);?>/>
  <?php endif;?>
</Video>
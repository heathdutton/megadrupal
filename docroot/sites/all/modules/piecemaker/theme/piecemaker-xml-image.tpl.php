<?php
/**
 * @file
 *   Outputs the Image XML node for piecemaker xml files
 *
 * @variables
 *   $item: An associative array. @see template_preprocess_piecemaker_xml for details
 */
?>
<Image <?php print drupal_attributes($item['#attributes']); ?>>
  <?php if (!empty($item['Text'])):?>
  <Text><?php print $item['Text']; ?></Text>
  <?php endif;?>
  <?php if (!empty($item['Hyperlink']['#attributes'])):?>
  <Hyperlink <?php print drupal_attributes($item['Hyperlink']['#attributes']);?>/>
  <?php endif;?>
</Image>
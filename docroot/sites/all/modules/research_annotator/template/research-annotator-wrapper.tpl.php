<?php

/**
 * @file: Default theme implementation for the annotation wrapper.
 * 
 * @note: THIS TEMPLATE HEAVILY REACTS WITH research_annotator's JQUERY. REMOVIVING OR 
 *        CHANGING EXISTING ELEMENT ATTRIBUTES MAY BE DISASTEROUS! ADDING CSS CLASSES IS SAFE.
 *
 * Available variables:
 * - $annotation_wrapper_variables: An array of renderable arrays containing the following.
 *   - annotations: The annotations and html structure associated with them.
 *   - no_annotations_msg: A message that appears when a node does not have any annotations.
 *   - form: An annotation creation form.
 *
 * @see template_preprocess_research_annotator_wrapper() 
 */
?>

<div id="research-annotations" class="block research-annotations">
  <div class="node-annotations">
    <h2><?php print t('Annotations'); ?></h2>
    <?php if(!empty($annotation_wrapper_variables['annotations'])): ?>
      <?php print render($annotation_wrapper_variables['annotations']); ?>
    <?php else: ?>
      <?php print render($annotation_wrapper_variables['no_annotations_msg']); ?>
    <?php endif; ?>
  </div>
  <?php print render($annotation_wrapper_variables['form']); ?>
</div>
  
<?php

/**
 * @file: Default theme implementation for the annotation form wrapper.
 * 
 * @note: THIS TEMPLATE HEAVILY REACTS WITH research_annotator's JQUERY. REMOVIVING OR 
 *        CHANGING EXISTING ELEMENT ATTRIBUTES MAY BE DISASTEROUS! ADDING CSS CLASSES IS SAFE.
 *
 * Available variables:
 * - $annotation_form_variables: An array of renderable arrays containing the following.
 *   - current_annotation_picture: The picture of the current user.
 *   - current_annotation_author: The username of the current user.
 *   - annotation_form: The annotation creation form.
 *
 * @see template_preprocess_research_annotator_form() 
 */
?>

<div class="annotation-form-wrapper">
  <h2><?php print t('Annotate this content'); ?></h2>
  <div class="annotation-profile">
    <?php print render($annotation_form_variables['current_annotation_picture']); ?>
    <?php print render($annotation_form_variables['current_annotation_author']); ?>
  </div>
  <?php print render($annotation_form_variables['annotation_form']); ?>
</div>
  
<?php
/**
 * @file
 * Entity reference behavior handler plugin declaration.
 */

$plugin = array(
  'title' => t('Entity reference autofill'),
  'description' => t('Populate form fields on entity selection.'),
  'class' => 'EntityReferenceAutofillInstanceBehavior',
  'behavior type' => 'instance',
);

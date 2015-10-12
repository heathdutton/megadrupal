<?php
/**
 * @file
 * Template file for the Commerce XLS Import instructions.
 */
?>

<h3><?php print t('Commerce Import'); ?></h3>

<h5><?php print t('Instructions'); ?></h5>

<p>
  <?php print t('There are 3 main functions for the Commerce Import, which are:'); ?>
</p>

<ul>
  <li><?php print t('Import Products'); ?></li>
  <li><?php print t('Validate Products Without Importing'); ?></li>
  <li><?php print t('Generate Product Templates'); ?></li>
</ul>

<p>
  <?php print t("Using the product templates, a list of products to add may be uploaded using the appropriate file upload section which will then be added to the commerce catalogue. Use the legend below to figure out which fields belong to the base product, or each variation."); ?>
</p>

<p>
  <?php print t("When an import is complete, a result document will be generated to show how the import went. If the product was imported successfully, the line will be marked green. If the product had an error, the bad fields will be marked red and the product will not be created, nor will it's variations. If a variation has an error, the field will be marked red."); ?>
</p>

<p>
  <?php print t("If you find an error and wish to change that product in question, make your changes in the error document and submit it using the regular import functionality. This will update all the products in the file and not create new ones."); ?>
</p>

<h5 id="image-description"><?php print t("Image Instructions"); ?></h5>

<p>
  <?php print t('Images may be uploaded using the "Upload Image Files" upload section at the bottom of the form.'); ?>
</p>

<p>
  <?php print t('Make sue that the permissions of the image files allow them to be readable by all.'); ?>
</p>

<div id="legend-swatches">
  <div class="excel-swatches swatch-container">
    <h6><?php print t('Field Colors'); ?></h6>

    <div id="parent-swatch">
      <div class="swatch"><span><?php print t('Parent product'); ?></span>
      </div>
      <span><?php print t('The fields necessary to create a base product'); ?></span>
    </div>

    <div id="variation-swatch">
      <div class="swatch"><span><?php print t('Variation'); ?></span></div>
      <span><?php print t('The fields that need to be filled out to successfully create a product variation'); ?></span>
    </div>

    <div id="both-swatch">
      <div class="swatch"><?php print t('Parent and variation'); ?></span></div>
      <span><?php print t('The fields required by both  base product and its variations'); ?></span>
    </div>
  </div>

  <div class="result-swatches swatch-container">
    <h6><?php print t('Result Colors'); ?></h6>

    <div id="warning-swatch">
      <div class="swatch"><span><?php print t('Warning'); ?></span></div>
      <span><?php print t("A field highlighted in this color may have something wrong with it, or it hasn't been evaluated"); ?></span>
    </div>
    <div id="error-swatch">
      <div class="swatch"><span><?php print t('Error'); ?></span></div>

      <span><?php print t('A field in this color has had some errors found'); ?></span>
    </div>

    <div id="success-swatch">
      <div class="swatch"><span><?php print t('Successfull'); ?></span></div>
      <span><?php print t('A field in highlighted in this color is correct'); ?></span>
    </div>
  </div>
</div>

<div style="clear: both;"></div>

<fieldset>
  <legend><span class="fieldset-legend"><?php print t('Field Help'); ?></span></legend>
  <div class="fieldset-wrapper">
    <div class="product-field"><h6><?php print t('title'); ?></h6>
      <span class="product-field-description"><?php print t('The title of the base product you are creating'); ?></span>
    </div>
    <div style="clear:both;"></div>

    <div class="product-field"><h6><?php print t('variation_title'); ?></h6>
      <span class="product-field-description"><?php print t('The title of the variation of the product you are creating. If left blank it will use the title of the base product.'); ?></span>
    </div>
    <div style="clear:both;"></div>

    <div class="product-field"><h6><?php print t('sku'); ?></h6>
      <span class="product-field-description"><?php print t('The sku of the product variation you are creating'); ?></span>
    </div>
    <div style="clear:both;"></div>

    <div class="product-field"><h6><?php print t('status'); ?></h6>
      <span class="product-field-description"><?php print t('The status of the product or variation. 1 if the item is active, 0 if inactive'); ?></span>
    </div>
    <div style="clear:both;"></div>

    <div class="product-field"><h6><?php print t('language'); ?></h6>
      <span class="product-field-description"><?php print t('The language short hand to create the product in. (en, fr etc)'); ?></span>
    </div>

    <div style="clear:both;"></div>
    <?php print render($product_types); ?>
  </div>
</fieldset>

<?php
/**
 * @file
 * ClickBank Download Block template file.
 *
 * To override this template, copy it to your own theme directory first, then
 * make changes in that file.
 */
?><div class="clickbank-download block">
  <?php foreach ($products as $nid => $product) : ?>
  <div class="product">
    <h3 class="product-title"><?php print $product['title']; ?></h3>
    
    <?php if(isset($product['product-image'])) :
      print render($product['product-image']);
    endif; ?>

    <div class="downloads">
      <h4>Download Now:</h4>
      <?php print render($product['downloads']); ?>
    </div>
  </div>
  <?php endforeach; ?>

  <?php if(isset($error)) : ?>
  <div class="validation-error">
    <span class="error"></span>
    <p><?php print $error; ?></p>
  </div>
  <?php endif; ?>
</div>

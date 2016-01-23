<?php $item['#localized_options']['attributes']['class'][] = 'main-navigation-item'; ?>
<div <?php print drupal_attributes($item['#localized_options']['attributes']); ?>>
  <?php unset($item['#localized_options']['attributes']['id']); ?>
  <?php print l('<span class="image"></span><span class="text">' . check_plain($item['#title']) . '</span>', $item['#href'], array('html' => TRUE, 'attributes' => $item['#localized_options']['attributes'])); ?>
</div>

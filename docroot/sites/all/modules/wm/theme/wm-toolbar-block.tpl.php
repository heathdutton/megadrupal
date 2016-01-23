<?php if ($render): ?>
<ul <?php print drupal_attributes($ul_attr); ?>>
  <?php if ($title): ?>
  <li class="header">
    <?php print theme('image', array('path' => $icon, 'attributes' => array('class' => 'header-img'))); ?>
    <span class="header-text">
            <?php print $title; ?>
          </span>
  </li>
  <?php endif; ?>

  <?php print $items; ?>
</ul>
<?php endif; ?>
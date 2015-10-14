<div class="sidebar-email">
  <p><?php print $form['signup']['#description']; ?></p>
  <br />
    <?php print drupal_render($form['signup']['email']); ?>
    <?php print drupal_render($form['submit']); ?>
    <div style="display: none;">
    <?php
      unset($form['signup']);
      unset($form['submit']);
      print drupal_render($form);
    ?>
    </div>
</div>

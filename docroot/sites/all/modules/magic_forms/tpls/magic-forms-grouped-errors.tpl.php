<?php
/**
 * @file
 * The magic forms error template.
 *
 * $anchor string
 *   The anchor ID, false if not used.
 * $element array
 *   The element array.
 * $errors array
 *   An array containing the errors.
 */
?>
<div class="magic-form-grouped-errors">
<?php if ($anchor) : ?>
  <a href="#field-anchor-<?php print $element['#id']; ?>"><?php print isset($element['#magic-forms']['group-anchor-title']) ? $element['#magic-forms']['group-anchor-title'] : $element['#title']; ?></a>
<?php endif; ?>
  <?php if (count($errors['errors']) < 2) : ?>
  <p class="magic-form-error"><?php print $errors['errors'][0]; ?></p>
  <?php else : ?>
  <ul class="magic-form-errors-list">
  <?php foreach ($errors['errors'] as $error) : ?>
      <li><?php print $error; ?></li>
  <?php endforeach; ?>
  </ul>
  <?php endif; ?>
</div>

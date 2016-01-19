<?php
/**
 * @file
 * The magic forms error template.
 *
 * $config array
 *   The forms magic form config.
 * $element array
 *   The element array.
 * $errors array
 *   An array containing the errors.
 */
?>
<div class="magic-form-errors <?php print magic_forms_config_property(MAGIC_FORMS_FIELD_ERROR_ASPREFIX, $config) ? 'prefix' : 'suffix'; ?>">
  <div class="magic-form-error-icon"></div>
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

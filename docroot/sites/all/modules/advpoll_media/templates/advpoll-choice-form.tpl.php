<?php

/**
 * @file
 * Theme implementation to display advpoll choice form with image fields.
 *
 * Variables available
 * - $choices: The radio buttons or checkboxes for the choices in the advpoll.
 * - $media: The file view output. View mode: Polls choice form
 * - $write_in: The write-in field, if it is set.
 * - $message: Poll message (validation error, etc.), if set.
 * - $submit: The vote button.
 * - $hidden: Hidden variables to be rendered for the form to function.
 *
 * @see
 * - template_preprocess_advpoll_choice_form()
 * - theme_advpoll_media().
 *
 * @ingroup themeable
 */
?>
<div class="poll advpoll">
  <div class="advpoll-choice-form advpoll-field-image-choice-form">

    <div class="choices">
      <?php foreach ($choices as $key => $choice): ?>
        <div class="choice choice-<?php print $key ?>">
          <?php if (!empty($media)): ?><?php print $media[$key] ?>
          <?php endif; ?>
          <div class="choice-text"><?php print $choice; ?></div>
        </div>
      <?php endforeach; ?>

      <?php if (isset($write_in)): ?>
        <?php print $write_in; ?>
      <?php endif; ?>
    </div>

  <?php if (isset($message)): ?>
    <?php print $message; ?>
  <?php endif; ?>

  <?php print $submit; ?>
  <?php print $hidden; ?>
  </div>
</div>

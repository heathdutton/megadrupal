<?php
/**
 * @file
 * The default template for the Bits on the Run widget.
 */
?>

<div id='botr-video-box' class='fieldset-wrapper'>
  <?php
  if(user_access('open botr dashboard')):
  ?>
  <span class='botr-dashboard-link'><?php
  echo l(t('Open dashboard'), 'botr/dashboard')
  ?></span>
  <?php
  endif;
  ?>
  <div id='botr-list-wrapper'>
    <input type='text' class='form-text' value='<?php echo t('Search videos') ?>' id='botr-search-box' />
    <ul id='botr-video-list'></ul>
    <input type='button' id='botr-upload-button' class='form-submit' value='<?php echo t('Upload a video...') ?>' />
  </div>
</div>

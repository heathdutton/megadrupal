<?php
/**
 * @file
 * Shows the upload form, which be default is hidden,
 * Adds an invisible iframe which actually gets the feedback
 * from file uploads and some empty divs which will be filled by JS.
 */
?>
<div>
  <a id="odir_file_upload_ajax_link">[<?php print t("Upload files")?>]</a>
</div>

<div id="odir_file-uploader">
  <noscript>
    <p><?php print t("Please enpable JavaScript to use file uploader.")?></p>
  </noscript>
</div>

<div id="odir_ajax_feedbacks"></div>
<div style="display:none;" id="odir_upload_process"><?php print t("Loading")?> ...</div>

<div id="odir_upload_form" style="display:none;">
  <div class="fileinputs">

    <?php print $form ?>

  </div>
  <div class="fakefile"></div>
</div>
<div style="display:none;" id="odir_upload_result" ></div>
<div><iframe id="odir_upload_target" name="odir_upload_target" style="width:00;height:00;border:0px"></iframe></div>

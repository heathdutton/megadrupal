<div id="<?php print $element['#id']; ?>"></div>
<script type="text/template" id="<?php print $element['#id']; ?>-template">
  <div class="qq-uploader-selector qq-uploader">
    <div class="qq-upload-button-selector qq-upload-button">
      <div>Upload a file</div>
    </div>
    <ul class="qq-upload-list-selector qq-upload-list">
      <li>
        <div class="qq-progress-bar-container-selector">
          <div class="qq-progress-bar-selector qq-progress-bar"></div>
        </div>
        <span class="qq-upload-spinner-selector qq-upload-spinner"></span>
        <img class="qq-thumbnail-selector" qq-max-size="100" qq-server-scale>
        <span class="qq-edit-filename-icon-selector qq-edit-filename-icon"></span>
        <span class="qq-upload-file-selector qq-upload-file"></span>
        <input class="qq-edit-filename-selector qq-edit-filename" tabindex="0" type="text">
        <span class="qq-upload-size-selector qq-upload-size"></span>
        <a class="qq-upload-cancel-selector btn-small btn-warning" href="#">Cancel</a>
        <a class="qq-upload-retry-selector btn-small btn-info" href="#">Retry</a>
        <a class="qq-upload-delete-selector btn-small btn-warning" href="#">Delete</a>
        <a class="qq-upload-pause-selector btn-small btn-info" href="#">Pause</a>
        <a class="qq-upload-continue-selector btn-small btn-info" href="#">Continue</a>
        <span class="qq-upload-status-text-selector qq-upload-status-text"></span>
        <a class="view-btn btn-small btn-info hide" target="_blank">View</a>
      </li>
    </ul>
  </div>
</script>
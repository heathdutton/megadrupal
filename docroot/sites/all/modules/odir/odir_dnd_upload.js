/* Inspired by https://developer.mozilla.org/en/Using_files_from_web_applications */
/**
 * Uploading files per drag and drop
 * (Only tested with Mozilla Firefox, should work with Google Chrome, but is not tested!)
 */

function odir_assign_file_dropper_events(divid, directory) {
  var dropbox;
  dropbox = document.getElementById(divid);
  dropbox.setAttribute('directory', directory);
  dropbox.addEventListener("dragenter", odir_upload_dropbox_dragenter, false);
  dropbox.addEventListener("dragover", odir_upload_dropbox_dragover, false);
  dropbox.addEventListener("dragleave", odir_upload_dropbox_dragleave, false);
  dropbox.addEventListener("drop", odir_upload_dropbox_drop, false);
}

function odir_uploadFinished(result) {
  (function (jQuery) {
    jQuery('#odir_upload_form').hide(200);
  })(jQuery);
}

function odir_upload_dropbox_dragenter(e) {
  e.stopPropagation();
  jQuery('#odir_upload_form').addClass('odir_dragover');
  e.preventDefault();
}

function odir_upload_dropbox_dragleave(e) {
  e.stopPropagation();
  jQuery('#odir_upload_form').removeClass('odir_dragover');
  e.preventDefault();
}

function odir_upload_dropbox_dragover(e) {
  e.stopPropagation();
  e.preventDefault();
}

function odir_upload_dropbox_drop(e) {
  e.stopPropagation();
  e.preventDefault();

  var dt = e.dataTransfer;
  var files = dt.files;
  var directory = e.currentTarget.getAttribute('directory');
  odir_file_uploader(files, directory);
}

function odir_file_uploader(files, directory) {
  var output = document.getElementById("odir_ajax_feedbacks");
  output.innerHTML = "";
  jQuery("#odir-uploading-status").css('visibility', 'visible');
  for (var i = 0; i < files.length; i++) {
    var file = files[i];
    var xhr = new XMLHttpRequest();
    var url = Drupal.settings.odir.upload_files_ajax + "/" + directory;
    xhr.open("POST", url, false);
    var data = new FormData();
    data.append("files", file);
    xhr.send(data);

    if (xhr.status === 200) {
      var my_JSON_object = {};
      my_JSON_object = JSON.parse(xhr.responseText);
      odir.show_uploaded_files(my_JSON_object);
      Drupal.settings.odir.odir_div_filecounter = xhr.responseText.filecounter;
    }
    else {
      output.innerHTML += "Error " + xhr.status + " occurred uploading your file.<br />" + xhr.responseText;
    }
  }
  jQuery("#odir-uploading-status").css('visibility', 'hidden');
}

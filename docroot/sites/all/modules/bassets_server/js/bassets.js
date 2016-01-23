;(function ($) {
  Drupal.bassets = Drupal.bassets || {};
  Drupal.bassets.FileUploaded = function(uploader, file) {
    if (file.status == plupload.DONE) {
      var base = 'fileuploads' + file.id;
      var url = Drupal.settings.basePath + 'bassetsajax/nojs';
      var element = $('<div></div>');
      var element_settings = {
          'event': 'filevalidate',
          'callback' : '_bassets_server_plupload_filevalidate',
          'url' : url
          };
      var ajax = new Drupal.ajax(base, element, element_settings);
      $.extend(ajax.submit, {"file": file});
      ajax.element.trigger('filevalidate');
      uploader.stop();
    }
  };
  Drupal.ajax.prototype.commands.pluploadFileValidate = function(ajax, response, status) {
    var uploader = $('#edit-upload').pluploadQueue();
    if (typeof response.data === 'object') {
      uploader.removeFile(response.data);
    }
    uploader.start();
  };
}(jQuery));

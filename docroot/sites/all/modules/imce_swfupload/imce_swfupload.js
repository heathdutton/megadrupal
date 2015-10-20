imceSWFU = function () {
  var settings = {};
  return {
    settings : settings,
    fileQueued : function(fileObj) {
      $('#' + settings.containerId + ' ul').append('<li id=\'' + fileObj.id + '\'><span class="imce_swfu_file">' + fileObj.name + '</span><span class="imce_swfu_percent">&nbsp;</span><input type="button" class="form-submit" value="Remove"></li>');
      $('#' + fileObj.id + ' input').click(function() {
          settings.instance.cancelUpload(fileObj.id, false);
          $('#' + fileObj.id).fadeOut('slow');
          $('#' + fileObj.id).remove();
        }
      );
    },
    setPostParam : function(name, val) {
      if(!settings.postParams) {
        settings.postParams = {};
      }
      settings.postParams[name] = val;
    },
    uploadStart : function() {
      settings.instance.setPostParams({ 'dir' : imce.conf.dir, 'sid' : settings.sid });
      for (name in settings.postParams) {
        settings.instance.addPostParam(name, settings.postParams[name]);
      }
    },
    queueError : function (fileObj, errorCode, message) {
      alert('Error code ' + errorCode + ' with message "' +  message + '" received while queueing file ' + fileObj.name);
    },
    uploadError : function (fileObj, errorCode, message) {
      if (errorCode != SWFUpload.UPLOAD_ERROR.UPLOAD_STOPPED) {
        alert('Error code ' + errorCode + ' with message "' +  message + '" received while uploading file ' + fileObj.name);
        $('#' + fileObj.id).fadeOut('slow');
        $('#' + fileObj.id).remove();
      }
      else {
        $('#' + fileObj.id + ' [class="imce_swfu_percent"]').html('&nbsp;');
      }
    },
    uploadSuccess : function (fileObj, serverData, response) {
      res = {
        status : 0,
        message : 'Server response unavailable, please refresh directory to check file upload status'
      };
      if (serverData != '') {
        eval('res = ' + serverData);
      }
      $('#' + fileObj.id).fadeOut('slow');
      $('#' + fileObj.id).remove();

      if (res.status) {
        imce.fileAdd(res.file);
        settings.instance.startUpload();
      }
      else {
        alert('Error uploading file ' + fileObj.name + ': ' + res.message);
      }
    },
    uploadProgress : function (fileObj, bytesDone, bytesTotal) {
      per = Math.floor(bytesDone/bytesTotal * 100);
      $('#' + fileObj.id + ' [class="imce_swfu_percent"]').html(per + '%');
    },
    initInterface : function () {
      if (settings.containerId) {
        $('#' + settings.containerId).append("<ul class='imce_file_list'></ul><input type='button' class='imce_swfu_button imce_swfu_start_button form-submit' value='Start Upload'><input type='button' class='imce_swfu_button imce_swfu_stop_button form-submit' value='Stop Upload'>");
        $('#' + settings.containerId + ' [class*="imce_swfu_start_button"]').click(function () {settings.instance.startUpload();});
        $('#' + settings.containerId + ' [class*="imce_swfu_stop_button"]').click(function () {settings.instance.stopUpload();});
      }
    }
  }
};


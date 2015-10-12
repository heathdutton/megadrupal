/**
* @file
* Javascript to process the video to YouTube.
*/

(function($) {
  Drupal.behaviors.youtube_uploader = {

    attach : function(context, settings) {

      // Create a fake button to send the video to youtube.
      if ($('.upload-toyoutube').length < 1) {
        $('.field-type-youtube-upload .upload-video')
            .after('<a href="javascript:;" class="button upload-toyoutube">Upload</a>')
            .end().find('.upload-toyoutube').bind("click", sendToYoutube);
      }

      // Refresh the thumb from youtube.
      $('.refresh_thumb a').each(refreshYoutubeInfo);

      function refreshYoutubeInfo() {
        // Test if binding is already done... do not know why it binds a
        // second event when the field is rebuilt ?? otherwise it
        // triggers 2 times the func.
        if (!$(this).data('events'))  {
          $(this).click(refreshYoutubeInfoClick);
        }
      }

      function refreshYoutubeInfoClick() {
        $(this).displayThrobber();
        var video_id = $(this).parents('.file-widget').find('input.youtube_hidden_id').val();
        var _this = $(this);
        // Coy the new thumb.
        $.getJSON(Drupal.settings.basePath + "youtube_uploader/refreshthumb/" + video_id, function(json) {
              _this.removeThrobber();
              if (json.error) {
                _this.parents('.video-form-preview').showErrorMessage(json.error);
              }
              else {
                // Replace the text by an image, ready to be populated.
                _this.parents('.video-form-preview').find(' .thumb span').replaceWith('<img src="" />');
                // Force refresh of thumb image.
                d = new Date();
                _this.parents('.video-form-preview').find('img').attr('src', json.src + '?' + d.getTime());
                // Add thumb_fid.
                _this.parents('.form-video-upload').find('input.thumb_fid').val(json.fid);
                // And new title.
                _this.parents('.form-video-upload').find('input.video_title').val(json.title);
                _this.parents('ul.title_options').find('li.title').text(json.title);
              }
            });
        return false;
      }

      function sendToYoutube() {
        // Remove error messages if any.
        $(this).parent('.form-managed-file').removeErrorMessage();
        $(this).unbind("click", sendToYoutube);

        // Check if title and file is set.
        var video_title = $(this).parents('.field-type-youtube-upload').find('input[type="text"]').val();
        if (video_title == '' || $(this).parents('.field-type-youtube-upload').find('input[type="file"]').val() == '') {
          $(this).parent('.form-managed-file').showErrorMessage('please provide a title AND a video file');
          $(this).bind("click", sendToYoutube);
          return false;
        }
        // Attach throbber.
        $(this).siblings('input[type=submit]').displayThrobber();
        $(this).parents('.form-managed-file').waitingMessage('Authenticating to Youtube...');

        var field_ref = $(this).parents('.field-type-youtube-upload').find('input[name$="[youtube_uploader_field_ref]"]').val();

        var _this = $(this);
        // Get token first.
        $.getJSON(Drupal.settings.basePath + "youtube_uploader/gettoken/" + Drupal.encodePath(field_ref) + '/' + Drupal.encodePath(video_title),
                function(json) {
                  if (json.error) {
                    _this.parent('.form-managed-file').showErrorMessage(json.error);
                    _this.removeThrobber();
                    $(this).bind("click", sendToYoutube);
                    return false;
                  }

                  // Post video to youtube.
                  // We make a form from the fields that will
                  // be sent to youtube.
                  _this.parents('.form-video-upload').wrap('<form class="youtube-upload" action="' + json.posturl + '?nexturl=http://' + location.hostname + Drupal.settings.basePath +  '/youtube_uploader/nexturl" method="post" enctype="multipart/form-data"></form>')
                      // Add the token.
                      .append('<input type="hidden" name="token" value="' + json.token + '" />')
                      // The event.
                      .parents('form.youtube-upload').bind("submit", submitToYoutube)
                      // Submit.
                      .submit().find('.form-video-upload').waitingMessage('Sending video...');
                });
        return false;

      }

      function submitToYoutube() {
        var options = {
            success : showResponse,
            dataType : 'json'
        };
        $(this).ajaxSubmit(options);
        return false;
      }

      // Post-submit callback.
      function showResponse(responseText, statusText, xhr, $form) {

        // Remove throbber.
        $('form.youtube-upload .form-submit').removeThrobber();

        if (responseText.status != 200) {
          $('form.youtube-upload .form-type-textfield').showErrorMessage('Error code ' + responseText.code + ' status ' + responseText.status);
          return;
        }
        // Add the id to the hidden fid field.
        $('form.youtube-upload .youtube_hidden_id').val(responseText.id);
        // Remove the fake form.
        $('.field-type-youtube-upload .form-managed-file').unwrap();
        // Remove form input value. 
        $('.field-type-youtube-upload .form-type-file input').val('');
        // Then trigger the real upload button so Drupal will rebuild everything fine.
        $('.field-type-youtube-upload .upload-video').trigger("mousedown");
      }

      // Display error message.
      $.fn.showErrorMessage = function(message) {
        if ($(this).siblings('.messages').length < 1) {
          $(this).siblings('.messages').remove();
        }
        $(this).before('<div class="messages error" style="display:none">' + message + '</div>').siblings('.messages').slideDown();
      };
      $.fn.removeErrorMessage = function(message) {
        $(this).siblings('.messages').remove();
      };

      // Display throbber.
      $.fn.displayThrobber = function() {
        $(this).after('<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div><span>&nbsp;</span></div>');
      };

      $.fn.waitingMessage = function(message) {
        $(this).find('.ajax-progress span').text(message);
      };

      // Remove throbber.
      $.fn.removeThrobber = function() {
        $(this).siblings('div.ajax-progress').remove();
      };

    }
  };

})(jQuery);

/**
 * @file
 * Ooyala embed code widget JavaScript code.
 */
Drupal.ooyala = Drupal.ooyala || {'listeners': {}, 'onCreateHandlers': {}, 'players': {} };

(function ($) {
  /**
   * AJAX success handler.
   */
  Drupal.ooyala.refreshThumbnail = function(data) {
    $('.ooyala-preview').removeClass('ooyala-progress').find('.throbber').remove();

    if (!data['error']) {
      if (data['field_id']) {
        var preview = $('#' + data['field_id']).removeClass('ooyala-preview-hidden').get(0);
      }
      else {
        var preview = $('.ooyala-preview').removeClass('ooyala-preview-hidden').get(0);
      }

      $(preview).html(data['content']);
    }

    if (data['message']) {
      alert(data['message']);
    }
  }

  /**
   * Add AJAX functionality to the thumbnail refresh link.
   */
  Drupal.behaviors.ooyalaRefreshThumbnail = {
    attach: function (context, settings) {
      $('a.ooyala-refresh', context).click(function() {
        var embed_code = $(this).parents('.ooyala-button-container').find('.ooyala-embed-code-input').val();
        var field_id = $(this).parents('.ooyala-field').find('.ooyala-preview').attr('id');
        $(this).parents('.ooyala-field').find('.ooyala-preview')
          .addClass('ooyala-progress')
          .append('<span class="throbber">&nbsp;</span>');
        $.ajax({
          url: Drupal.settings.ooyalaRefreshUrl,
          success: Drupal.ooyala.refreshThumbnail,
          dataType: 'json',
          data: { embed_code: embed_code, field_id: field_id }
        });
        return false;
      });
    }
  };

  /**
   * Dim unselected thumbnail images for a video.
   */
  Drupal.behaviors.ooyalaThumbnailSelection = {
    attach: function (context, settings) {
      $('input.ooyala-upload-thumbnails:not(.ooyala-upload-thumbnails-processed)').each(function() {
        $(this).addClass('ooyala-upload-thumbnails-processed');

        if ($(this).attr('checked')) {
          $(this).parent().addClass('ooyala-thumbnail-selected');
        }

        // For poor IE users.
        if ($.browser.msie) {
          // IE won't trigger a change event until the selection is lost.
          var ev = 'click';
          // We need this because IE doesn't select the radio button when the image
          // is clicked. See http://snook.ca/archives/javascript/using_images_as
          $(this).parent().find('img').bind('click', function() {
            $("#" + $(this).parents("label").attr("for")).click();
          });
        }
        else {
          var ev = 'change';
        }

        $(this).bind(ev, function() {
          $(this).parents('div.ooyala-upload-thumbnails').find('.ooyala-thumbnail-selected').removeClass('ooyala-thumbnail-selected');
          $(this).parent().addClass('ooyala-thumbnail-selected');
        });
      });
    }
  };
})(jQuery);


/**
 * @file
 * Apple News individual entity vertical tab helper.
 */

(function ($) {

  Drupal.behaviors.AppleNewsEntityForm = {
    attach: function(context) {

      // Channels and sections checkboxes.
      $('input[data-channel-id]').each(function() {
        var _channel_id = $(this).data('channel-id');
        if (!$(this).attr('checked')) {
          $('input[data-section-of="' + _channel_id + '"]').parent().hide();
        }
      });

      // Vertical tab summary.
      $('fieldset.applenews-options', context).drupalSetSummary(function (context) {

        if ($('.form-item-applenews-applenews-publish-flag input:checked', context).length) {

          // Check first channel from the list if non selected.
          // Otherwise the module doesn't know where to publish current content.
          setDefaultChannel(true);

          var $postdate = $('.applenews-post-date', context);
          if ($postdate[0]) {
            return Drupal.t('Published on ' + $postdate.html());
          }
          else {
            return Drupal.t('Published');
          }
        }
        else {
          setDefaultChannel(false);
          return Drupal.t('Not published');
        }

      });
      $('input[data-channel-id]').bind('click', function(e) {
        var _channel_id = $(this).data('channel-id');
        $('input[data-section-of="' + _channel_id + '"]').parent().toggle();
      });
      $('.applenews-sections').parent().css({'margin-left' : '20px'});

    }
  };

  /**
   * Show/Hide default channels based on the valude of the Publish checkbox.
   */
  function setDefaultChannel(show) {
    if ($('#edit-applenews-channels').hasClass("applenews-channels-add-form")) {
      $('input[data-channel-id]:first').attr('checked', show);
      var _channel_id = $('input[data-channel-id]:first').data('channel-id');
      if (show) {
        $('input[data-section-of="' + _channel_id + '"]').parent().show();
      }
      else {
        $('input[data-section-of="' + _channel_id + '"]').parent().hide();
      }
    }
  }

}(jQuery));

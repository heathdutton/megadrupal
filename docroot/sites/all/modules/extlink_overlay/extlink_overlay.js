/**
 * @file
 * Attaches the behaviors for extlink_overlay module.
 */

 (function ($) {
  Drupal.behaviors.extlink_overlay = {
    attach: function (context, settings) {
      curr_object = this;
      // on page load, if "Open external links in a pop-up" is checked then
      // disable checkboxes for "Display a pop-up warning when any external link
      // is clicked." and "Open external links in a new window".
      if ($('#edit-extlink-overlay-popup:checkbox').is(':checked')) {
        curr_object.disable_extlink_option();
      }
      // on clicking external link, if user has opted for overlay, then show
      // the content in iframe in overlay.
      if (Drupal.settings.extlink_overlay.extOverlayPopUp) {
        $('.' + Drupal.settings.extlink.extClass).live('click',function(event) {
          event.preventDefault();
          var $structure = '<div class="overlayOuter"><div class="overlayInner"/><iframe src=' + $(this).attr("href") + ' id="frame-content"    width="80%" height = "100%"/><div id="frame-close"/></div>';
          $('body').append($structure);
          $(".overlayOuter").fadeIn(300);
          $('iframe#frame-content').load(function() {
            $(".overlayInner").css({'background-image':'none'});
            $('iframe#frame-content').slideDown(300);
          });
        });
      }
      // close iframe on clicking close button.
      $('#frame-close').live('click',function(event) {
        $('iframe#frame-content').slideUp(300);
        $(".overlayOuter").fadeOut(300).remove();
      });
      // if "Open external links in a pop-up" is checked then
      // disable checkboxes for "Display a pop-up warning when any external link
      // is clicked." and "Open external links in a new window" else enable them.
      $('#edit-extlink-overlay-popup:checkbox').click(function () {
        if ($(this).is(':checked')) {
          curr_object.disable_extlink_option();
        } else {
          curr_object.enable_extlink_option();
        }
      });
    },

    // function to disable "Display a pop-up warning when any external link
    // is clicked." and "Open external links in a new window"
    disable_extlink_option: function () {
      $('#edit-extlink-alert:checkbox').removeAttr('checked').attr('disabled','disabled');
      $('#edit-extlink-target:checkbox').removeAttr('checked').attr('disabled','disabled');
      $('#edit-extlink-alert-text').attr('disabled','disabled');
    },
    // function to enable "Display a pop-up warning when any external link
    // is clicked." and "Open external links in a new window"
    enable_extlink_option: function () {
      $('#edit-extlink-alert:checkbox').removeAttr('disabled');
      $('#edit-extlink-target:checkbox').removeAttr('disabled');
      $('#edit-extlink-alert-text').removeAttr('disabled');
    },
  };
})(jQuery);

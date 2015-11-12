/**
 * @file
 * Timezone picker Drupal integration.
 */
(function($) {

Drupal.behaviors.timezonePicker = {};
Drupal.behaviors.timezonePicker.attach = function(context, settings) {
  $('.timezone-picker-wrapper img', context).once(function() {
    var imgTag = this;
    var timezonePickerId = imgTag.id;

    // Add the timezone picker JavaScript functionality.
    var timezonePickerSettings = {};
    if (settings.timezonePicker && settings.timezonePicker[timezonePickerId]) {
      timezonePickerSettings = settings.timezonePicker[timezonePickerId];
    }
    $(imgTag).timezonePicker(timezonePickerSettings);

    // Add geolocation behavior to the detect button (if any).
    if (navigator && navigator.geolocation) {
      var $detectButton = $('#' + imgTag.id + '-detect');

      // Callback function called after location completes or fails.
      var locationComplete = function(status) {
        if (status.code) {
          $detectButton.next('span').remove();
          $detectButton.after('<span class="message error">' + status.message + '</span>');
        }
        else {
          // Bounce the pin.
          $(imgTag).closest('.timezone-picker-wrapper').find('.timezone-picker-pin').animate({top:"-=6px"},100).animate({top:"+=6px"},100);
          $detectButton.next('span').remove();
          $detectButton.after('<span class="message status">' + Drupal.t('Location found.') + '</span>');
        }
        setTimeout(function() {
          $detectButton.next('span').remove();
        }, 2000);
      };

      $detectButton.click(function() {
        $(imgTag).timezonePicker('detectLocation', { complete: locationComplete });
        return false;
      });
    }
    else {
      $('#' + imgTag.id + '-detect').remove();
    }

  });
};

})(jQuery);
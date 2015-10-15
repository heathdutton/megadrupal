/**
 * @file
 * User Disk Quota javascript functions for module administration pages.
 */

(function($) {
  Drupal.behaviors.user_disk_quota_admin = {
    attach: function (context, settings) {
      $( ".user-disk-percentage-slider-range-max" ).slider({
        range: "max",
        min: 1,
        max: 100,
        value: settings.user_disk_quota_admin.default_percentage,
        slide: function( event, ui ) {
          $( ".user-disk-percentage-slider" ).val( ui.value );
        }
      });
      $( ".user-disk-percentage-slider" ).keyup(function(){
        var value = parseInt($(this).val(), 10);
        if (!isNaN(value) && value>=0 && value<=100) {
          $( ".user-disk-percentage-slider-range-max" ).slider( "option", "value", value);
        }
      });
    }
  };
})(jQuery);

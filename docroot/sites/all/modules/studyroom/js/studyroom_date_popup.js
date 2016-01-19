/**
 * @file
 * JavaScript for studyroom to modify date_popup time range.
 */

(function($) {
Drupal.behaviors.studyrooms = {
  attach: function (context) {
    for (var id in Drupal.settings.datePopup) {
      $('#' + id).bind('focus', Drupal.settings.datePopup[id], function(e) {
        var datePopup = e.data;
        if ($(this).hasClass('date-popup-init')) {
          if (datePopup.func == 'timepicker') {
            //console.log(datePopup.settings);
            $(this).timepicker(datePopup.settings);
            $(this).click(function(){
              $(this).focus();
            });
          } else if (datePopup.func == 'timeEntry') {
            //console.log(datePopup.settings);
            $(this).timeEntry(datePopup.settings);
            $(this).click(function(){
              $(this).focus();
            });
          }
        }
      });
    }
   }
};
})(jQuery);

/**
 * @file
 * Display chosen select elements.
 */

(function ($) {
  Drupal.behaviors.chosenTheme = {
    attach: function (context) {
      // Define selects that should not be changed by the Chosen library - for example: hierarchical selects.
      var exclude_elements = ['.shs-enabled','.shs-select','.fivestar-widget select','#edit-vote--2','#edit-date-year','#edit-date-month','#edit-date-day'];
      $("#edit-ckeditor-lang").chosen({
        width: "14em"
      });
      // General settings for all selects.
      $("select").not(exclude_elements.join(',')).chosen({
        disable_search_threshold: 10
      });
      $('select.error').next('.chosen-container').addClass('error');
    }
  }
})(jQuery);

(function ($) {
  Drupal.behaviors.waterlooBehaviours = {
    attach: function (context, settings) {

      // search form modifications
      $("#block-search-form .form-text").attr("value", "Search");

      $('#block-search-form .form-text').focus(function() {
         if($(this).attr("value") == "Search") {
          $(this).attr("value", "");
         }
      });

      $('#block-search-form .form-text').blur(function() {
        if($(this).attr("value") == "") {
          $(this).attr("value", "Search");
        }
      });
    }
  }
})(jQuery);;

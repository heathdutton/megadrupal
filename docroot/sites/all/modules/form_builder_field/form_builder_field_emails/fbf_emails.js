
(function($)
{
  Drupal.behaviors.fbfEmail = {
    /**
     * Bind additional events to the remove template buttons.
     * @param context
     * @param settings
     */
    attach: function(context, settings) {
      $(".fbf-email-elements").click(function() {
        $.getJSON($(this).data("url"), function(data) {
          var id = "#" + data.id;
          $(id).html(data.html).fadeOut().fadeIn();
          return false;
        });
        return false;
      });
    }
  };
})(jQuery);

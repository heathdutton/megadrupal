(function ($) {
  Drupal.behaviors.ToolbarToggle = {
    attach: function(context) {
      $("#toolbar-wrapper .container").append("<div id='toggle' />");
      $("#toggle").html("<a id='toggle-link' href='#'>Toolbar</a>");
      $("#toolbar-wrapper .container .inner").hide();
      $("#toggle-link").click(function() {
        $("#toolbar-wrapper .container .inner").slideToggle();
      });
    }
  }
})(jQuery);

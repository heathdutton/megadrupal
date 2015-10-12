(function ($) {

  Drupal.behaviors = Drupal.behaviors || {};

  Drupal.behaviors.admintabs = {
    attach: function () {
      var items = $("#admin-tabs");
      items.hover(
        function () {
          open.hide();
          items.show();
        },
        function () {
          items.hide();
          open.show();
        }
      );
      var open = $("#admin-tabs-open");
      items.hide();
      open.mouseover(function () {
        open.hide();
        items.show();
      });
    }
  };

})(jQuery);

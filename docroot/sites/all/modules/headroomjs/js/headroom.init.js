(function ($) {
  Drupal.behaviors.headroomjs = {
    attach: function(context, settings) {
      if (typeof settings.headroomjs != 'undefined') {
        $(settings.headroomjs.selector).headroom({
          offset : settings.headroomjs.offset,
          tolerance : settings.headroomjs.tolerance,
            classes : {
              initial : settings.headroomjs.initial_class,
              pinned : settings.headroomjs.pinned_class,
              unpinned : settings.headroomjs.unpinned_class
            }
        });
      }
    }
  };
})(jQuery);
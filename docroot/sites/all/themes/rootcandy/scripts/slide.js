/*
 * Module Slide. Simple singleton handling slide panel on top of the layout.
 *
 */
(function ($) {
  AdminCandy.Slide = (function() {

    var toggleButton = '<div id="toggle"><div id="toggle-wrap"><a id="open" class="open" href="#"><span class="panel-open">Open Panel</span></a><a id="close" style="display: none;" class="close" href="#"><span class="panel-close">Close Panel</span></a></div></div>';

    return {
      init: function(userOptions) {

        // Merge user defined options with default options.
        var options = $.extend({}, this.defaultOptions, userOptions);

        // Store the panel element.
        var $panel = $('#panel');

        // Insert the toggle button and handle its behavior.
        $(toggleButton).appendTo('#header-title').find('a').click(function() {

          // Slidetoggle the panel with the appropriate speed
          // and handle both callbacks.
          $panel.slideToggle(options.speed, function() {
            if ($panel.is(':visible')) {
              options.onExpand.call(this);
            }
            else {
              options.onCollapse.call(this);
            }
          });

          // Switch between Open and Close buttons.
          $(this).siblings().andSelf().toggle();

          // Return false to prevent following the links.
          return false;
        });
      },

      // Publicly accessible default options.
      defaultOptions: {
        // Speed of the slide effect.
        speed: 'slow',
        // Function called when the panel is expanded.
        onExpand: function() {},
        // Function called when the panel is collapsed.
        onCollapse: function() {}
      }
    };
  })();

  // Initialize the Slide module on domready using default options.
  $(function() {
    AdminCandy.Slide.init();
  });
}(jQuery));


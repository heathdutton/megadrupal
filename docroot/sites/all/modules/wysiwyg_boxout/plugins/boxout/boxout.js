(function (jQuery) {
  Drupal.wysiwyg.plugins['boxout'] = {
    /**
     * Return whether the passed node belongs to this plugin.
     */
    isNode: function(node) {
      // Check for boxout wrapper and inner elements. @TODO loop through.
      var is_boxout = (jQuery(node).is('.boxout') || jQuery(node).parent().is('.boxout')
              || jQuery(node).parent().parent().is('.boxout'));
      return (is_boxout);
    },

    /**
     * Execute the button.
     */
    invoke: function(data, settings, instanceId) {
      this.show_popup(settings, instanceId);
    },

    /**
     * Shows the popup.
     */
    show_popup: function(settings, instanceId) {
      // Check if the form is not yet on the DOM.
      if (jQuery('.boxout-popup').length == 0) {
        // Print the form on the page.
        jQuery("body").append(settings.form_markup);
      }
      // Display popup centered on screen.
      jQuery(".boxout-popup").center().show("fast", "linear", function() {
        // Listeners for buttons.
        jQuery("#edit-boxout-cancel").click(function() {
          jQuery(".boxout-popup").remove();
        });

        jQuery("#edit-boxout-insert").click(function() {
          if (typeof(jQuery("#edit-boxout-header").val()) != "undefined") {

            // Get all values.
            var style = jQuery("#edit-boxout-style").val();
            var element_type = jQuery("#edit-boxout-header-element-type").val();
            var header = jQuery("#edit-boxout-header").val();
            var body = jQuery("#edit-boxout-body").val();

            // Replace tokens.
            var content = settings.boxout_markup
                .replace(/\[element_type\]/g, element_type)
                .replace('[style]', style)
                .replace('[header]', header)
                .replace('[body]', body);

            // Insert content.
            Drupal.wysiwyg.instances[instanceId].insert(content);
          }
          // Close popup.
          jQuery(".boxout-popup").remove();
        });

        // Catch keyboard events.
        jQuery(document).keydown(function(e) {
          // Esc key pressed.
          if (e.keyCode == 27) {
            jQuery(".boxout-popup").remove();
          }
        });

        jQuery("#edit-boxout-header").focus();
      });
    },
  };
})(jQuery);

/**
 * Center the element on the screen.
 */
if (!jQuery.fn.center) {
  jQuery.fn.center = function () {
      this.css("position","absolute");
      this.css("top", Math.max(0, ((jQuery(window).height() - jQuery(this).outerHeight()) / 2) +
                                                  jQuery(window).scrollTop()) + "px");
      this.css("left", Math.max(0, ((jQuery(window).width() - jQuery(this).outerWidth()) / 2) +
                                                  jQuery(window).scrollLeft()) + "px");
      return this;
  }
}

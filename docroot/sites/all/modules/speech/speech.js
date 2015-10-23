/**
 * @file speech.js
 */

(function ($) {

  /**
   * The Drupal behaviors for speech.
   */

  Drupal.behaviors.speech = {
    attach: function(context) {

      /*
      // Add speech recognition where it needs to go.
      for (var delta in Drupal.settings.speech_selectors) {
        var selectors = Drupal.settings.speech_selectors[delta];
        for (var key in selectors)
        {
          $(selectors[key], context)
            .attr('speech', 'speech')
            .attr('x-webkit-speech', 'x-webkit-speech');
        }
      }
      */

      $("input[speech]", context).filter(".form-autocomplete").each(function() {
        if ($(this, context).css("backgroundPositionX") == "100%") {
          var width = $(this, context).width() - 26;
          $(this, context)
            .css("backgroundPositionX", width + "px");
        }
      });

      $(".speech-autosubmit", context).each(function() {
        $(this, context).bind('webkitspeechchange', function() {
            $(this, context).closest('form').submit();
        });
      });

      // Set default colours of the checkbox labels.
      $(".speech-enable-widget-popup input", context).filter(":checked").each(function() {
        $(this, context)
          .siblings("label")
          .addClass("enabled");
      });

      // Handle closing of the popup when user clicks elsewhere.
      $(document, context).click(function() {
        $(".speech-enable-widget-popup", context).hide();
      });
      $(".speech-enable-widget *", context).click(function(event) {
        event.stopPropagation();
      });

      // Set up the trigger for the popup.
      $(".speech-enable-widget-show", context).click(function() {
        $(this, context)
          .siblings(".speech-enable-widget-popup")
          .show()
          .css("display", "inline");
      });

      // Set up ajax for when checkboxes are clicked on/off.
      $(".speech-enable-widget input[type=checkbox]", context).change(function() {
        // Add throbber.
        $(this, context)
          .siblings("label")
          .append("<div class=\"ajax-progress\"><div class=\"throbber\"></div></div>");

        var value = $(this, context).is(':checked');
        var form_key = $(this, context).attr("name");
        // Build the url for Ajax.
        var url = Drupal.settings.basePath
                + "?q="
                + 'speech/ajax/'
                + form_key
                + '/'
                + value
                + '/'
                + Math.floor((1000000000 * Math.random())).toString(16);
        // Execute Ajax callback.
        $.getJSON(
          url,
          (function(element, context) {
            return function(data) {
              if (data == "true") {
                $(element, context)
                  .siblings("label")
                  .addClass("enabled");
                $(element, context)
                  .parents(".field-suffix")
                  .siblings("input, textarea")
                  .attr("x-webkit-speech", "x-webskit-speech")
                  .attr("speech", "speech");
                  $("input[speech]", context).filter(".form-autocomplete").each(function() {
                    if ($(this, context).css("backgroundPositionX") == "100%") {
                      var width = $(this, context).width() - 26;
                      $(this, context)
                        .css("backgroundPositionX", width + "px");
                    }
                  });
              }

              else {
                $(element, context)
                  .siblings("label")
                  .removeClass("enabled");
                $(element, context)
                  .parents(".field-suffix")
                  .siblings("input, textarea")
                  .removeAttr("x-webkit-speech")
                  .removeAttr("speech");
                $("input:not(input[speech])", context).filter(".form-autocomplete").each(function() {
                  var width = $(this, context).width() - 26;
                  if ($(this, context).css("backgroundPositionX") == width + "px") {
                    $(this, context)
                      .css("backgroundPositionX", "100%");
                  }
                });
              }
              // Remove throbber.
              $(element, context)
                .siblings("label")
                .find(".ajax-progress")
                .remove();
            };
          }(this, context)) // pass this and context through as element and context.
        );

      });

    }
  };

})(jQuery);

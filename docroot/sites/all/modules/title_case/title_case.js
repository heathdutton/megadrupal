(function($) {

  Drupal.behaviors.title_case = {
    attach: function(context, settings) {

      // Apply the conversions to the selector.
      $(Drupal.settings.title_case.selectors, context).each(function() {
        var sConverted = $(this).html().toTitleCase();

        // Apply the exceptions.
        var tokens = sConverted.split(' ');

        for(i in tokens) {
          $.each(Drupal.settings.title_case.exceptions, function(j, exp) {

            if (tokens[i].trim().toLowerCase() == exp.trim().toLowerCase()) {
              tokens[i] = exp;
            }
          });
        }

        $(this).html(tokens.join(' '));
      });
    }
  };

})(jQuery);

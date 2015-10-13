(function ($) {

Drupal.behaviors.contentTypes = {
  attach: function (context) {
    // Provide the vertical tab summaries.
    $('fieldset[id^="representative_image_"]', context).drupalSetSummary(function(context) {
      var vals = [];

      $(context).find('.form-item').each(function(context) {
        label = $(this).find('label').text().trim();
        field = $(this).find(':selected').text().trim();
        vals.push(label + ': ' + field);
      });

      return vals.join('<br/>');
    });
  }
};

})(jQuery);

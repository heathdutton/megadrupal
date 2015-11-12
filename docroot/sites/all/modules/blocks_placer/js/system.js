
(function($) {

Drupal.behaviors.blocks_placer_system = {
  attach: function (context, settings) {

    // Set the summary for the settings form.
    $('fieldset#edit-blocks-placer-settings', context).drupalSetSummary(function() {
      var $bp_check = $('#edit-blocks-placer-settings input:checked');
      var summary = $('label[for=' + $bp_check.attr('id') + ']', context).text();
      return Drupal.checkPlain(summary);
    });

  }
}

})(jQuery);

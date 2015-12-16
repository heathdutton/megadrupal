(function($) {

/**
 * Behavior for filtering the list of regions based on the selected layout.
 */
Drupal.behaviors.contextOmegaLayout = {
  attach: function (context, settings) {
    // Initialize the 'contextOmega' setting if it hasn't been written yet.
    settings.contextOmega = settings.contextOmega || {};
    settings.contextOmega.regions = settings.contextOmega.regions || {};

    // Select the layout <select> and apply the region filter every time its
    // value changes.
    $('.context-omega-layout-select', context).once(function() {
      $(this).change(function() {
        var selected = $(this).val();

        if (!selected.length || selected === '_default') {
          // If 'Default' was selected, show all children.
          $('[id^=context-blockform] .blocks').children().show();
        }
        else if (settings.contextOmega.regions.hasOwnProperty(selected)) {
          // Hide all elements initially.
          $('[id^=context-blockform] .blocks').children().hide();

          // Iterate over all regions of the selected layout and show the
          // corresponding rows.
          $.each(settings.contextOmega.regions[selected], function (index, value) {
            var $item = $('.context-blockform-regionlabel-' + value);
            $item.add($item.nextUntil('.label')).show();
          });
        }

        if (Drupal.contextBlockForm) {
          Drupal.contextBlockForm.setState();
        }
      });

      // Fire the change() event initially so the filter gets applied for the
      // pre-selected value as well.
      $(this).change();
    });
  }
};

})(jQuery);

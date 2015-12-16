(function ($) {
  $(document).ready(function() {
    var context = $('.nodespotbox-view-settings');
    if (context.size()) {
      // Set views display list, based on selection
      _nodespotbox_change_display(false);

      // If selection of view name changes, opdate display list
      $('.nodespotbox-views-names', context).change(function() {
        _nodespotbox_change_display(true);
      });
    }

    function _nodespotbox_change_display(reset) {
      var displays = []
      if ($('.nodespotbox-views-names').val() != 'none') {
        displays = Drupal.settings.nodespotbox[$('.nodespotbox-views-names').val()].displays;
      }

      // Build display list
      var options = '<option value="none">' + Drupal.t('None') + '</option>';
      $.each(displays, function(key, val) {
        options += '<option value="' + key + '">' + val + '</option>';
      });

      var defval = 'none';
      if (!reset) {
        defval = $('.nodespotbox-views-displays').val();
      }

      // Update selector
      $('.nodespotbox-views-displays').html(options).val(defval);
    }
  });
})(jQuery);
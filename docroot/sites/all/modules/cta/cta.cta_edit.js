(function ($) {

/**
 * Provide the summary information for the block settings vertical tabs.
 */
Drupal.behaviors.cta_cta_edit = {
  attach: function (context) {
    // check if intel form elements exist
	if ($('#edit-cta-data-ga-event').length > 0) {
        this.determineFieldDisplay();

      $('#edit-cta-selectors input').change(function () {
          Drupal.behaviors.cta_cta_edit.determineFieldDisplay();
      });
      $('#edit-cta-data-ga-event-track').change(function () {
        Drupal.behaviors.cta_cta_edit.determineFieldDisplay();
      });
    }
  },

  determineFieldDisplay: function () {
    if ($('#edit-cta-selectors input:checked').length) {
      $('#edit-cta-data-ga-event').css('display', 'block');
      if ($('#edit-cta-data-ga-event-track:checked').length) {
          $('.cta-data-ga-event-track-options-group').css('display', 'block');
      }
      else {
          $('.cta-data-ga-event-track-options-group').css('display', 'none');
      }
    }
    else {
      $('#edit-cta-data-ga-event').css('display', 'none');
    }
  }
};

})(jQuery);
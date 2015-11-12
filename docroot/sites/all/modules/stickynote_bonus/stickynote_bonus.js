(function($) {
  /**
   * Initialization
   */
  Drupal.behaviors.stickynote_bonus = {
    /**
     * Run Drupal module JS initialization.
     *
     * @param context
     * @param settings
     */
    attach: function(context, settings) {
      var settings = settings;
      $(context).find('#block-stickynote-stickynote-block').appendTo('body').addClass('webks-stickynote-bonus').show();
      $(context).find('#block-stickynote-stickynote-block').prepend('<div class="inner"></div>');
      $(context).find('#block-stickynote-stickynote-block .block-title, #block-stickynote-stickynote-block > div:not(.opener)').hide().parent();
      $(context).find('#block-stickynote-stickynote-block').append('<div class="opener"><span class="notes-count"></span><span class="ico-img"></span><span class="label">Show / Hide Notes</span></div>');

      // On opener click / toggle
      $(context).find('#block-stickynote-stickynote-block .opener').click(function() {
        $(context).find('#block-stickynote-stickynote-block').toggleClass('notes-open');
        $(context).find('#block-stickynote-stickynote-block .block-title, #block-stickynote-stickynote-block > div:not(.opener)').slideToggle("fast");
      });

      // On ESC press
      $(document).keyup(function(e) {
        if (e.keyCode == 27) {
          $(context).find('#block-stickynote-stickynote-block').removeClass('notes-open');
        }
      });

      // Check if theres already a note
      // Count -1 because the first row is the title row!
      var notesCount = 0;
      if ($(context).find('#stickynote-list .sticky-table thead').length > 0) {
        // #webksde#JP20150803: Bugfix for module bug: Only count if there is head and body.
        // If the table is empty the headers are wrongly written into tbody by stickynote! This leads to wrong count.
        // Remove the if construct around when that is fixed!
        notesCount = $(context).find('#stickynote-list .sticky-table tbody tr').length;
      }
      if (notesCount > 0) {
        $(context).find('#block-stickynote-stickynote-block').addClass("has-notes");
        $(context).find('#block-stickynote-stickynote-block .notes-count').text(notesCount);
        $(context).find('#block-stickynote-stickynote-block').attr("title", Drupal.t("@notesCount note(s)", {'@notesCount': notesCount}));
      } else {
        $(context).find('#block-stickynote-stickynote-block .notes-count').remove();
      }
    }
  };
})(jQuery);
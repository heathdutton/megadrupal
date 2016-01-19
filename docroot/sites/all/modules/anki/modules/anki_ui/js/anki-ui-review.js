(function ($) {
  Drupal.behaviors.bibliobird_flashcards = {
    attach: function (context, settings) {
      var link = $('<a id="anki-ui-show-answer" href="#"></a>')
          .text(Drupal.t('Show answer'))
          .click(function (evt) {
            $('#anki-ui-flashcard-question').hide();
            $('#anki-ui-flashcard-answer').show();
            $('#anki-ui-ease-buttons').show();
            link.remove();
            evt.preventDefault();
          })
          .insertAfter($('#anki-ui-flashcard', context));
    }
  };
})(jQuery);

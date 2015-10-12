(function($) {

Drupal.behaviors.tribuneSpellCheck = {
  attach: function(context, settings) {
    $('.tribune-spellcheck-error', context).click(function() {
      var tribune = $(this).parents('div.tribune-wrapper:first');

      var word = $(this).text();
      var correction = $(this).attr('title');
      correction = correction.replace(/,.*/, '');

      var clock = $(this).parents('.tribune-post:first').find('.tribune-clock');

      Drupal.tribune.insertText(tribune, clock.text() + ' ');
      Drupal.tribune.insertText(tribune, 's/' + word + '/' + correction + '/ ');
    });
  }
};

})(jQuery);




(function ($) {

Drupal.behaviors.lyric_sheet_chords = {
  attach: function (context, settings) {
    $('a.remove-chords', context).once('remove-chords-processed')
    .click(function(){
      $(this).parent().find('span.chord').remove();
      $(this).remove();
      return false;
    })
  }
}

}(jQuery));

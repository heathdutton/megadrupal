/**
 * @file
 * JS for the Ransom Note (ransom_note) module.
 */

(function($) {
  Drupal.behaviors.ransomNote = {
    attach: function (context, settings) {

      var selectors = Drupal.settings.ransomNote.selectors;
      var count = Drupal.settings.ransomNote.count;
      // Some children elements that need to be excluded not to break stuff.
      var excluded = '.link-wrapper, .contextual-links-wrapper, .element-invisible';
      $(selectors).each(function() {
        $(this).contents().not(excluded).each(function() {
          var chars = $(this).text().split('');
          var newText = '';
          for (i = 0; i < chars.length; i++) {
            // Do not process spaces.
            if (chars[i] != ' ') {
              var rand = Math.floor(Math.random() * count);
              newText += '<span class="ransom_note_' + rand + '">' + chars[i] + '</span>';
            }
            else {
              newText += ' ';
            }
          }
          $(this).html(newText);
        });
      });

    }
  }
}(jQuery));

/**
 * @file
 *
 * JavaScript should be made compatible with libraries other than jQuery by
 * wrapping it with an "anonymous closure". See:
 * @see  http://drupal.org/node/1446420
 * @see  http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
 */

(function ($, Drupal, window, document, undefined) {
  // Begin jQuery.
  $(document).ready(function () {
    // Prevents Widows in Post Titles.
    // @see http://css-tricks.com/preventing-widows-in-post-titles/
    $(":header").each(function() {
      var wordArray = $(this).text().split(" ");
      if (wordArray.length > 2) {
        wordArray[wordArray.length-2] += "&nbsp;" + wordArray[wordArray.length-1];
        wordArray.pop();
        $(this).html(wordArray.join(" "));
      }
    });

    // Open rel=nofollow and PDF links in a new window by adding a target=_blank
    // attribute to those anchors.
    $("a[href$='.pdf']:not(.newwindowLink-processed), a[rel='nofollow']:not(.newwindowLink-processed)").each(function () {
      $(this)
        // Add the class to any matched elements so we avoid them in the future.
        .addClass("newwindowLink-processed")
        // Update link attributes.
        .attr({
          title: $(this).attr("title") + ' [' + Drupal.t('This link will be opened in a new browser tab/window.') + ']',
          target: "_blank"
        });
    });
  });
  // End jQuery.


})(jQuery, Drupal, this, this.document);

/**
 * @file
 * Attaches the behaviors for the Editor Notes field.
 */

(function ($) {
  /**
   * Alters pager links if pager is present in Editor Notes.
   *
   * Adds fragment (#) to the pager links providing an option to remember and
   * restore active tab after page reload when navigating between pager links.
   * The problem arises because theme_pager() doesn't support fragment (#)
   * for some reasons.
   *
   * Alternative idea is to override theme_pager() and other nested
   * theme functions and add support for fragment there.
   *
   * Depends on Fieldgroup (field_group) module.
   *
   * @see https://www.drupal.org/project/field_group
   *
   * Similar issue related to Drupal 8.
   *
   * @see https://www.drupal.org/node/1293912
   */
  Drupal.behaviors.editorNoteAlterPager = {
    attach: function (context, settings) {
      if (typeof Drupal.settings.editorNoteContainer === 'string') {
        var editorNoteContainer = '#' + Drupal.settings.editorNoteContainer;
      }
      else {
        var editorNoteContainer = (typeof Drupal.settings.editorNoteContainer[0] !== undefined) ? '#' + Drupal.settings.editorNoteContainer[0] : '';
      }

      if (editorNoteContainer !== '') {
        $(editorNoteContainer + ' .pager li a').each(function() {
          var pagerLinkPath = $(this).attr('href');
          if (pagerLinkPath !== undefined && pagerLinkPath !== '' && !(pagerLinkPath.indexOf(editorNoteContainer) + 1)) {
            pagerLinkPath += editorNoteContainer;
            $(this).attr('href', pagerLinkPath);
          }
        })
      }
    }
  };
})(jQuery);

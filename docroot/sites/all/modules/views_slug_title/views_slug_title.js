/**
 * @file
 * Views Slug Title javascript behaviors.
 */

(function($) {
  /**
   * Change the summary of field group if sluggenerate input isn't checked.
   */
  Drupal.behaviors.slugFieldsetSummaries = {
    attach: function(context) {
      $('fieldset.slug-form', context).drupalSetSummary(function(context) {
        var path = $('.form-item-slugwrap-slug input').val();
        var automatic = $('.form-item-slugwrap-sluggenerate input').attr('checked');
        if (automatic) {
          return Drupal.t('Automatic slug');
        }
        if (path) {
          return Drupal.t('Slug: @slug', {'@slug': path});
        }
        else {
          return Drupal.t('No slug');
        }
      });
    }
  };
})(jQuery);

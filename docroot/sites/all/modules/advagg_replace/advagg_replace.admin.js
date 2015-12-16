/**
 * @file
 * Custom JS for the AdvAgg Replace admin pages.
 */

(function ($) {

Drupal.behaviors.advaggReplaceAutoDefault = {
  attach: function (context, settings) {
    // Automatically fill in the search string into the 'default' field.
    $('.search-string', context).change(function() {
      var $default = $('#' + this.id.replace('string', 'default') + '');
      if ($default.val() == '') {
        $default.val(this.value);
      }
    });
  }
};

/**
 * Revert the value to its default, and provide a small animated effect to
 * indicate something happened.
 */
Drupal.behaviors.advaggReplaceRevertReplacement = {
  attach: function (context, settings) {
    $('.replace-revert', context).click(function() {
      var $replace = $('#edit-css' + this.id.replace('replace-revert:', '') + '-replace');
      $replace.fadeOut();
      $replace.fadeIn();
      $replace.val(this.alt);
    });
  }
};

})(jQuery);

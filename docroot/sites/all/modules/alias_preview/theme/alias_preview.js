/**
 * Enhances node path alias entry with automatic preview on forms.
 */

(function ($) {

/**
 * Adds behaviors to load on document.ready().
 */
Drupal.behaviors.alias_preview = {

/**
 * Provides a smooth animation when the user clicks "edit" for alias preview.
 */
attach: function (context, settings) {
  // Ensures correct positioning of the preview wrapper div.
  console.log(context);
  var $preview = $('#alias-preview-wrapper');
  var editTitle = '.node-form .form-item-title';
  if ($preview.length && $(editTitle, context).length) {
    $preview.detach().appendTo(editTitle);
  }

  // Brings the user to the custom alias field with proper settings checked.
  var jumpLink = 'a.alias-preview-jump-pathauto';
  // Only attaches scroll behavior if the link exists.
  if (!$(jumpLink, context).length) {
    return;
  }
  $(jumpLink, context).once('aliasPreviewScroll', function() {
    $(this).click(function () {
      // Scrolls down to the correct vertical position on the page.
      $('html, body').animate({ scrollTop: $('.vertical-tabs').offset().top });

      // Opens up the correct vertical tab to display the URL path settings.
      $('.vertical-tab-button a:contains("URL path settings")').click();

      // Ensures the form is displaying alias settings as enabled options.
      $('#edit-path-pathauto').click().attr('checked', false).attr('value', 0);
      $('.form-item-path-alias').removeClass('form-disabled');

      // Copies the newest calculated title value into the edit field.
      var $newAlias = $('#alias-preview-wrapper .alias-preview-alias');
      var $editAlias = $('#edit-path-alias');
      if ($newAlias.length) {
        var val = $.trim($newAlias.text());
        $editAlias.val(val);
      }

      // Moves active focus into the field so the user can edit immediately.
      $editAlias.removeAttr("disabled").focus().select();
    });
  });
}

};

}(jQuery));

/**
 * List Name Validate
 *
 * Query the Lyris DB for a list name.
 */
(function ($) {
  $(document).ready(function() {
    var ind = $('#list-name-validate');

    $(".lyris-validate-name").blur(function() {
      // First add a throbber while we are querying Lyris.
      var listname = $("input[list-name='1']").val();
      if (listname.length > 0) {
        ind.load(Drupal.settings.basePath + 'lyris/ajax/list/name-validate/' + listname);
      }
    });
  });
})(jQuery);

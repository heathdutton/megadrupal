(function ($) {
  /**
   * Force a refresh when the select box is changed.
   */

  Drupal.CPS = {};

  // Helper function to add/update query parameter. There are libraries to
  // generalize URL handling, but seem too heavy weight for what we need.
  Drupal.CPS.getChangesetUrl = function(url, changeset_id) {
    // Replace the value if changeset_id is already in the URL.
    if (url.indexOf('changeset_id') !== -1) {
      url = url.replace(/([\?&])(changeset_id=)[^&#]*/, '$1$2' + changeset_id);
    }
    else {
      var separator = (url.indexOf('?') !== -1) ? '&' : '?';

      // Ensure that hash comes after query string.
      if (url.indexOf('#') !== -1) {
        var split = url.split('#');
        split[0] += separator + 'changeset_id=' + changeset_id;
        url = split.join('#');
      }
      else {
        url += separator + 'changeset_id=' + changeset_id;
      }
    }
    return url;
  };
  
  Drupal.behaviors.cps = {
    attach: function (context, settings) {
      // Add the active changeset_id to the query parameter.
      if ("cps" in settings && "changeset_id" in settings.cps) {
        var url = Drupal.CPS.getChangesetUrl(window.location.href, settings.cps.changeset_id);
        if (url !== window.location.href) {
          window.history.replaceState(null, null, url);
        }

        if (settings.cps.changeset_id != 'published') {
          $('form:not(#cps-changeset-preview-form)').once('cps-changeset-form', function () {
            $(this).append('<input type="hidden" name="changeset_id" value="' + settings.cps.changeset_id + '">');
          });
        }
      }

      // Force a refresh when the changeset select box is changed.
      $('#edit-changeset-id.form-select').once('changesetSelect').change(function() {
        var changeset_id = $(this).find(":selected").val();
        window.location.href = Drupal.CPS.getChangesetUrl(window.location.href, changeset_id);
      });
    }
  };
})(jQuery);
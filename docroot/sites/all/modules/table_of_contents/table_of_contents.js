(function ($) {
  Drupal.behaviors.tableOfContents = {
    /**
     * This assumes that the field is in a container with the class .field-name-{field_name}
     *
     * @param context
     * @param settings
     */
    attach: function(context, settings) {
      var getFragment = function (url) {
        // Remove everything before the hash character.
        var fragmentPosition = url.indexOf('#');
        if (fragmentPosition >= 0) {
          url = url.substr(fragmentPosition + 1);
        }
        else {
          return null;
        }
        // Remove the query string part.
        var queryPosition = url.indexOf('?');
        if (queryPosition >= 0) {
          url = url.substr(0, queryPosition);
        }
        return url;
      };
      for (var index = 0; index < settings.tableOfContents.length; index++) {
        var fieldData = settings.tableOfContents[index];
        // Get all the toc links that could not be mapped in backend.
        var $missedAnchorLinks = $('.block-table-of-contents a.toc-link-invalid-id');
        $('.field-name-' + fieldData.fieldName).find(fieldData.selector + ':not([id])').each(function (index, item) {
          $(item).attr('id', getFragment($($missedAnchorLinks.get(index)).attr('href')));
        });
      }
    }
  }
})(jQuery);

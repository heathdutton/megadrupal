/**
 * @file
 * OpenTags UI.
 *
 * OpenTags UI provides a number of widgets which can improve the user experience
 * of tagging contents from structured data source.
 *
 * It has the following widgets:
 *
 *   - OpenTags AutoCombo (opentagsautocombo.js)
 *   - OpenTags DropDown  (opentagsdropdown.js)
 *
 * Copyright (c) 2015 Systemik Solutions. All rights reserved.
 * http://www.systemiksolutions.com
 */

/**
 * Define the top namespace OpenTagsUI.
 */
var OpenTagsUI = {};

(function($) {

  /**
   * Namespace openAglsUI.Common.
   *
   * Provide a set of functions which can be used for any OpenTags widgets.
   */
  OpenTagsUI.Common = {

    /**
     * The entity mapping used to encode html string.
     */
    htmlEncodeMapping: {
      "&": "&amp;",
      "<": "&lt;",
      ">": "&gt;",
      '"': '&quot;',
      "'": '&#39;',
      "/": '&#x2F;'
    },

    /**
     * Encode a string to html string.
     *
     * @param string str
     *   The string needs to be encoded.
     *
     * @returns string
     *   The encoded html string.
     */
    htmlEncode: function(str) {
      return String(str).replace(/[&<>"'\/]/g, function (s) {
        return OpenTagsUI.Common.htmlEncodeMapping[s];
      });
    }
  };
}(jQuery));

/**
 * @file
 * Handles AJAX SPARQL queries, and parsing of endpoint data.
 *
 * These are functions called on pages containing JSONP SPARQL blocks. If
 * Drupal acts as a proxy for the data, the Drupal.jsonSparqlInit function is
 * skipped.
 */

(function($) {
  /**
  * Function to set up an ajax call to retrieve data from a SPARQL endpoint
  * via JSONP. The jQuery.jsonp library is included to catch errors on JSONP
  * requests. The data object is the passed to the parse function.
  */
  Drupal.jsonpSparqlInit = function (url, delta) {
    $(document).ready(function() {
      $('#sparqlloading' + delta).show();
      $.jsonp({
        url: url,
        cache: false,
        callbackParameter: "callback",
        success: function(data) {
          // ALRIGHT! We have a response, let's pass it to the parse function.
          Drupal.jsonpSparqlParse(delta, data);
        },
        error: function(object, error) {
          // Aren't we lucky. The jquery.jsonp plugin can actually catch errors.
          $('#sparqlloading' + delta).hide();
          var err = Drupal.settings.jsonpSparql['delta_' + delta]['error'];
          $('#sparqlresult' + delta).append(err)
        }
      });
    });
  }

  /**
   * Function to handle a data object in sparql endpoint json format
   * The response should be in the format specified by W3C in this document:
   * http://www.w3.org/2001/sw/DataAccess/json-sparql/
   */
  Drupal.jsonpSparqlParse = function (delta, data) {
    // Set the variable parsing to true for default, to tell the intro text
    // to do its job after parsing the rows.
    var parsing = true;
    $('#sparqlloading' + delta).show();
    // Find all bindings, and bail out (with optional error message) if there
    // are no bindings.
    if (data['results']['bindings'].length < 1) {
      var empty = Drupal.settings.jsonpSparql['delta_' + delta]['empty'];
      $('#sparqlresult' + delta).append(empty);
      $(document).ready(function() {
        $('#sparqlloading' + delta).hide();
      });
      return;
    }
    $(data['results']['bindings']).each(function(d) {
      exclude = Drupal.settings.jsonpSparql['delta_' + delta]['exclude'];
      if (exclude['if'] && this[exclude['if']]['value'] == this[exclude['in']]['value']) {
        // This row is excluded based on settings.
      }
      else {
        $('#sparqlresult' + delta).append('<div class="sparqlrow" id="sparqlrow_delta' + delta + '_row' + d + '"></div>');

        // Find all values, even if we don't know their names.
        $.each(data['results']['bindings'][d], function(i, n) {
          if (!Drupal.settings.jsonpSparql['delta_' + delta]['pres'][i]) {
            // Oh no. A configuration error. The user forgot to configure
            // what to do with this column.
            var error = Drupal.t('You have an error in your block configuration. The column "!col" is not defined in your presentation settings (under "What to do with the results"). Please check your settings.', {'!col': i})
            $('#sparqlrow_delta' + delta + '_row' + d).text(error);
            $(document).ready(function() {
              $('#sparqlloading' + delta).hide();
            });
            // Set the variable parsing to false, so the intro text is not
            // prepended.
            parsing = false;
          }
          else {
            if (Drupal.settings.jsonpSparql['delta_' + delta]['pres'][i]['discard'] == 'YES') {
              // This column is discarded, based on settings, so let's just DO
              // NOTHING!
            }
            else {
              pre = Drupal.settings.jsonpSparql['delta_' + delta]['pres'][i]['pre'];
              post = Drupal.settings.jsonpSparql['delta_' + delta]['pres'][i]['post'];
              // Replace all placeholders with actual values.
              var regexp = /\[\w+\]/g;
              var pre_array = pre.match(regexp);
              if (pre_array) {
                i = 0;
                while (i < pre_array.length) {
                  var col = pre_array[i].slice(1, -1);
                  var pre = pre.replace(pre_array[i], data['results']['bindings'][d][col]['value']);
                  i++;
                }
              }
              var post_array = post.match(regexp);
              if (post_array) {
                i = 0;
                while (i < post_array.length) {
                  var col = post_array[i].slice(1, -1);
                  var post = post.replace(post_array[i], data['results']['bindings'][d][col]['value']);
                  i++;
                }
              }
              $('#sparqlrow_delta' + delta + '_row' + d).html($('#sparqlrow_delta' + delta + '_row' + d).html() +
              pre + Drupal.checkPlain(n['value']) + post);
            }
          }
        });
      }
    });
    if (parsing === true) {
      // Prepend the intro text.
      var intro = Drupal.settings.jsonpSparql['delta_' + delta]['intro'];
      // regexp holds the regular expression used to find placeholders in the
      // intro text.
      var regexp = /\[\w+\]/g;
      // Replace all placeholders with actual values.
      var replace_array = intro.match(regexp);
      if (replace_array) {
        $.each(replace_array, function(i, n) {
          var col = n.slice(1, -1);
          intro = intro.replace(n, data['results']['bindings'][0][col]['value']);
        });
      }
      $('#sparqlresult' + delta + ' .intro').html(intro);
      $(document).ready(function() {
        $('#sparqlloading' + delta).hide();
      });
    }
  }
})(jQuery);

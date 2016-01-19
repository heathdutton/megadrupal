/**
 * @file
 * Test function to test a SPARQL query, as well as event listener to react to
 * the button click.
 *
 * In addition to this, the JS also prefills the required column in the presentation
 * tab. Not to mention the animated GIF it shows while loading!
 */

(function($) {
  Drupal.behaviors.jsonpSparql = {
    attach: function (context, settings) {
      $('.sparqltestquery').click(function() {
        // The delta of this form is the last character of the button ID.
        var delta = $(this).attr('id').slice(-1);
        var query = $('#edit-jsonp-sparql-sparql-' + delta).val();
        if (query.indexOf('[1]')>0) {
          var placeholder = prompt(Drupal.t("Enter a value for the placeholder"),"");
          query = query.replace(/\[1\]/g, placeholder);
        }
        var server = $('#edit-jsonp-sparql-server-' + delta).val();
        // The following might look hardcoded, and it kind of is. This is the
        // ajax path we registered with the .module file.
        var url = Drupal.settings.basePath + 'jsonp_sparql/ajax/test_sparql';
        // Get an animated GIF going to show we are loading stuff.
        $(this).after('<img id="removemeafter" src="' + Drupal.settings.basePath + Drupal.settings.jsonpSparql['modpath'] + '/img/loading.gif">')
        $.ajax({
          url: url,
          cache: false,
          type: "POST",
          dataType: "json",
          data: {
            "query": query,
            "url": server
          },
          callbackParameter: "callback",
          success: function(data) {
            $('#removemeafter').remove();
            if (data['code'] == '200') {
              if (!data['data']) {
                // The response was probably not JSON.
                var message = Drupal.t('We got a response, but no results. Maybe you used a valid URL, but not a valid SPARQL endpoint? Have you remembered to include the questionmark that goes before the query parameter?');
              }
              else {
                if (data['data']['results']['bindings'].length>0) {
                  var message = Drupal.formatPlural(data['data']['results']['bindings'].length, 'We got a response! The result gave us 1 row to use. Now go on with the configuration and make the site awesome.', 'We got a response! The result gave us @count rows to use. Now go on with the configuration and make the site awesome.');
                }
                else {
                  var message = Drupal.t('We got a response, but with the placeholder you provided we got no rows to use. You might want to check your query (or maybe you had a typo when entering the placeholder. The good news is that the endpoint responded!')
                }
                rows = '';
                $.each(data['data']['head']['vars'], function(i,n) {
                  var length = data['data']['head']['vars'].length - 1;
                  if (i<length) {
                    rows = rows + n + ': |' + n + '|.' + "\n";
                  }
                  else {
                    rows = rows + n + ': |' + n + '|.';
                  }
                });
                var value = $('#edit-jsonp-sparql-cols-' + delta).val();
                if (value.length<1) {
                  $('#edit-jsonp-sparql-cols-' + delta).val(rows);
                  var message = message + '\n' + Drupal.t('The presentation tab has been updated with the names of the rows. You will most likely want to head over there to edit the presentation of each row.');
                }
              }
              alert(message)
            }
            else {
              alert(Drupal.t('Oh no. An error. The server returned an error code @code with the following message: @error', {'@code': data['code'], '@error': data['error']}));
            }
          }
        })
        return false;
      })
    }
  };
})(jQuery);

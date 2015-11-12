/**
 * @file
 * Converts pre.pgn4web into iframe.pgn4web using the pgn4web board.html.
 */
(function ($) {
  Drupal.behaviors.pgn4web = {
    attach: function(context) {
      // Possible data-attributes.
      var attrs = new Array( 'l', 'h', 'ig', 'iv', 'ih', 'am', 'sm' );

      $('pre.pgn4web').each(function(index) {
        // Give this an id and hide it.
        var this_id = "pgn4web_" + index;
        $(this).attr("id", this_id).hide();

        // Construct pgn4web url.
        var pgn4web_url = Drupal.settings.pgn4web.path;
        pgn4web_url += '/board.html?';
        pgn4web_url += Drupal.settings.pgn4web.query;

        // Board specific settings.
        // TODO: should we check attribute values here?
        for (var i = 0; i < attrs.length ; i++) {
          var attr = attrs[i];
          if ($(this).attr('data-pgn4web-' + attr)) {
            pgn4web_url += '&' + attr + '=' + $(this).attr('data-pgn4web-' + attr);
          }
        };

        // Check where we have stored the PGN data:
        // - if in an external file, use pgnData=xxx
        // - if inside the <pre> tag, use pgnId=xxx
        var game_text = '';
        var iframe_classes = 'pgn4web';

        if ($(this).attr('data-pgn4web-pd')) {
          game_text = '<a href="' + $(this).attr('data-pgn4web-pd') + '"><abbr title="Portable Game Notation">PGN</abbr></a>';
          pgn4web_url += '&pd=' + $(this).attr('data-pgn4web-pd');
          iframe_classes += ' pgn4web-multiple'; // Assume a file contains multiple games.
        }
        else {
          game_text = $(this).html();
          pgn4web_url += '&pi=' + this_id;

          // Check if we have more than one game.
          // See : http://code.google.com/p/pgn4web/wiki/User_Notes_drupal
          // about this regular expression.
          var multiGamesRegexp = /\s*\[\s*\w+\s*"[^"]*"\s*\]\s*[^\s\[\]]+[\s\S]*\[\s*\w+\s*"[^"]*"\s*\]\s*/m;
          iframe_classes += ' pgn4web-' + (multiGamesRegexp.test(game_text) ? 'multiple' : 'single');
        }

        // Add an iframe.
        $(this).after("<iframe class='" + iframe_classes + "' src='" + pgn4web_url + "'>" + game_text + "</iframe>");
      });
    }
  };

})(jQuery);

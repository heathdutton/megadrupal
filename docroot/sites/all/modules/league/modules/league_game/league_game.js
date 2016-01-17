(function($)
{
Drupal.behaviors.league_game = {
  attach:function (context) {
    if (undefined != Drupal.settings.league_game.result) {
      league_game_refresh();
    }

    function league_game_refresh() {
      var games = Drupal.settings.league_game.result;
      games = games.length > 1 ? games[0] : games.join("/");
      var interval = Drupal.settings.league_game.interval;
      var cache = Drupal.settings.league_game.cache;
      setTimeout(league_game_refresh, interval);
      var d =  Math.round(Date.now()/cache);
      $.get( "/league_game/result/" + games + "?t=" + d, function( data ) {
        $.each(data, function( index, value ) {
          $( "#result-game-" + index + " .score-a").html(value.score_a);
          $( "#result-game-" + index + " .score-b").html(value.score_b);
        });
      }, "json" );
    }
  }
}

}(jQuery));

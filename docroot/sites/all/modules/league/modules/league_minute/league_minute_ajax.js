(function($)
{
  $(function()
  {
    league_minute_refresh();
  });

  function league_minute_refresh() {
    var interval = Drupal.settings.league_minute.interval;
    var cache = Drupal.settings.league_minute.cache;
    var game = Drupal.settings.league_minute.game;
    setTimeout(league_minute_refresh, interval);
    var d =  Math.round(Date.now()/cache);
    $( "#minute-game-" + game ).load( "/league_minute/ajax/" + game + "?t=" + d );
  }
}(jQuery));

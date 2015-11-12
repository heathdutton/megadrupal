/**
 * @file
 * Attaches behaviors for the Article Jump module.
 */
(function($) {
  Drupal.behaviors.article_jump = {
    attach: function(context, settings) {
      my_select = Drupal.settings.article_jump.my_selector;
      my_speed = 500 | Drupal.settings.article_jump.scroll_speed;
      arts = $(my_select);
      idx = -1;
      // Read in hotkeys.
      var next_key = Drupal.settings.article_jump.next_hk;
      var prev_key = Drupal.settings.article_jump.prev_hk;
      var my_offset = 0;
      var cur_pos = 0;
      var near_art = 0;
      // Add the hidden hotkey guide.
      var artjump_modal = '<div class="modal">' +
'<p>Keyboard Shortcuts</p><span class="close"><a href="#">close</a></span><hr />' +
'<span class=hotkey>j</span><span class=hk_2> : Scroll to next article.</span><br clear=all />' +
'<span class=hotkey>k</span><span class=hk_2>: Scroll to previous article.</span><br clear=all />' +
'<span class=hotkey>?</span><span class=hk_2>: Show/Hide keyboard shortcuts.</span><br clear=all />' +
'<span class=hotkey>&lt;Esc&gt;</span><span class=hk_2>: Hide keyboard shortcuts.</span></div>';
      // Append the hotkey guide to the body of the page.
      $("body").append(artjump_modal);
      // Returns the index of the nearest article to cur window scroll.
      function nearest_article () {
          var i = 0;
          cur_pos = $(window).scrollTop();
          for (i = 1; i < arts.length; i++) {
              // Add 2 to cur_pos to compensate for Firefox.
              if (cur_pos+2 < ($(arts[0]).offset().top)) {
                  return -1;
              }
              if (($(arts[i-1]).offset().top <= cur_pos+2) && ($(arts[i]).offset().top > cur_pos+2)) {
                  return i-1;
              }
          }
          return i;
      };
      // Bind configured hotkey to scroll window to the next article header
      Mousetrap.bind(next_key, function() {
          near_art = nearest_article();
          idx = near_art+1;
          if (idx >= arts.length) {
              idx = arts.length-1;
          }
          $(".current").removeClass("current");
          $(arts[idx]).addClass("current");
          $('html,body').animate({
              scrollTop:($(arts[idx]).offset().top - my_offset)
          }, my_speed);
      });

      // Bind configured hotkey to scroll window to the previous article header
      Mousetrap.bind(prev_key, function() {
          near_art = nearest_article();
          $(".nearest").removeClass("nearest");
          $(arts[near_art]).addClass("nearest");
          idx = near_art-1;
          if (idx < 0) {
              idx = 0;
          }
          $(".current").removeClass("current");
          $(arts[idx]).addClass("current");
          $('html,body').animate({
              scrollTop:($(arts[idx]).offset().top - my_offset)
          }, my_speed);
      });
      Mousetrap.bind("?", function() {
          var cur_scroll = $(window).scrollTop() + 50;
          $(".modal").toggle();
          $(".modal").offset({top: cur_scroll});
      });
      Mousetrap.bind("esc", function() {
          $(".modal").hide();
      });
      $(".modal span.close a").click(function() {
          $(".modal").hide();
      });
    }

  };
}(jQuery));

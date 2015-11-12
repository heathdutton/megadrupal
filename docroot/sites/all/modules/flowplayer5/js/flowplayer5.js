/**
 * @file
 * Provides the FlowPlayer5 Drupal behavior.
 */

/**
 * The FlowPlayer Drupal behavior that creates the set of FlowPlayer elements from settings.flowplayer5.
 */
(function($) {
  Drupal.behaviors.flowplayer5 = {
    attach: function(context, settings) {
      //alert(JSON.stringify(settings.flowplayer5, true));
      var selector = settings.flowplayer5.selector;
      var config = this.formatPlayList(settings.flowplayer5.config);
      var b = settings.flowplayer5.brand
      // Check branding for commercial use.
      if (b.is_com) {
        flowplayer.conf.key = b.key;
        flowplayer.conf.logo = b.logo;
      }

      // Get current object.
      var o = this;
      //  config.ratio =  30/4;
      //   alert(JSON.stringify(config));
      // this will install flowplayer into an element with id="player"
      $(selector).flowplayer(config).hover(function() {
        if ($(this).hasClass('is-mouseout') && $(this).hasClass('no-toggle')) {
          $(this).addClass('is-mouseover');
          $(this).removeClass('is-mouseout');
        } else if ($(this).hasClass('is-mouseout')) {
          $(this).addClass('is-mouseover');
        }
      }, function() {
        if ($(this).hasClass('is-mouseout') && $(this).hasClass('no-toggle')) {
          $(this).addClass('is-mouseover');
          $(this).removeClass('is-mouseout');
        } else if ($(this).hasClass('is-mouseout')) {
          $(this).removeClass('is-mouseover');
          $(this).addClass('is-mouseout');
        }

      });

      // Update player controls and UI.
      var st = settings.flowplayer5.style;
      if (typeof st != 'undefined') {
        // Check Styple type exist 
        if (typeof st.type != 'undefined') {
          $(selector).addClass(st.type);
        }

        // Check if controls are existed.
        if (typeof st.check != 'undefined') {
          $.each(st.check, function(ci, cv) {
            var root = $(selector);
            o.updatePlayerPreviewControls(ci, cv, root);
          });
        }

        // Check UI attributes are existed.
        if (typeof st.input != 'undefined') {
          $.each(st.input, function(ii, iv) {
            o.updatePlayerPreviewColors(ii, iv);
          });
        }
      }
    },
    formatPlayList: function(config) {
      //alert('yes');
      var pl = config.playlist;
      var con = [];
      var l = [];
      for (i in pl) {
        var o = {};
        o[i] = pl[i];
        l.push(o);
      }
      con.push(l);
      config.playlist = con;
      return config;
    },
    updatePlayerPreviewColors: function(ic, iv) {
      var ics = {
        backgroundColor: function(c) {
          $('.fp-controls').css('background', c);
        },
        timelineColor: function(c) {
          $('.fp-timeline').css('background', c);
        },
        progressColor: function(c) {
          $('.fp-progress').css('background', c);
        },
        bufferColor: function(c) {
          $('.fp-buffer').css('background', c);
        },
        playerBgColor: function(c) {
          $('.fp-engine').css('background', c);
        },
      };
      // console.log(ic +'  ' + iv);
      if (ics[ic]) {
        ics[ic](iv);
      }
    },
    updatePlayerPreviewControls: function(cn, cv, root) {
      // Add classes to implement the new controlls. fixed-controls is-mouseover
      var cs = {overlayed: {con: 0, act: function() {
            root.addClass('fixed-controls is-mouseover');
            root.removeClass('is-mouseout');
          }, reAct: function() {
            root.removeClass('fixed-controls is-mouseover');
            root.addClass('is-mouseout');

          }},
        on_mouseover: {con: 0, act: function() {
            root.addClass('is-mouseout');
            root.addClass('no-toggle')
          }, reAct: function() {
            root.addClass('is-mouseout');
            root.removeClass('no-toggle');
          }},
        time: {con: 0, act: function() {
            root.addClass('no-time');

          }, reAct: function() {
            root.removeClass('no-time');
          }},
        top_left: {con: 'top_left', act: function() {
            root.addClass('aside-time');
          }, reAct: function() {
            root.removeClass('aside-time');
          }},
        volume: {con: 0, act: function() {
            root.addClass('no-volume no-mute');
          }, reAct: function() {
            root.removeClass('no-mute');
          }},
        slider: {con: 0, act: function() {
            root.removeClass('no-mute');
          }, reAct: function() {
            root.removeClass('no-volume no-mute');
          }, },
        play: {con: 'play', act: function() {
            root.addClass('play-button');
          }, reAct: function() {
            root.removeClass('play-button');
          }},
        embed: {con: 0, act: function() {
            flowplayer.conf.embed = false;
            $('.fp-embed').css('display', 'none');
          }, reAct: function() {
            flowplayer.conf.embed = true;
            $('.fp-embed').css('display', 'block');
          }},
        ful_screen: {con: 0, act: function() {
            flowplayer.defaults.fullscreen = false;
            $('.fp-fullscreen').css('display', 'none');
          }, reAct: function() {
            flowplayer.defaults.fullscreen = true;
            $('.fp-fullscreen').css('display', 'block');
          }, },
        darkicons: {con: 'darkicons', act: function() {
            root.addClass('color-light');
          }, reAct: function() {
            root.removeClass('color-light');
          }, },
      };

      if (typeof cs[cn] != 'undefined') {
        //   console.log(cv +' === '+cs[cn].con);
        if (cv == cs[cn].con) {
          //$('#flowplayer5-preview').addClass(cs[cn].act);
          cs[cn].act();
        } else {
          // $('#flowplayer5-preview').removeClass(cs[cn].act);
          cs[cn].reAct();
        }
      }

    }
  };
})(jQuery);

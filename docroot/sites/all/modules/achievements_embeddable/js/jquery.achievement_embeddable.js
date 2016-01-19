/**
 * @file
 * Achievement Embeddable is a jQuery plugin that provides the means to consume
 * the REST services for the Achievements Embeddable module using jQuery.
 * It will allow you to consume two specific resources:
 * - get_user_achievements
 * - achievement_trigger
 * The tests folder contains an example for how to use this library. Please see
 * index.html for more details.
 *
 * Developed by AchieveInternet.
 *
 */
(function( $ ){

  var methods = {
    init : function( options ) {
    },
    showPopUp : function( options ) {

      var settings = $.extend( {
        'image'               : '',
        'points'              : '',
        'unlockedRank'        : '',
        'achievementTitle'   : ''
      }, options);

      // Here we add the divs that will hold the Notification.
      // $(".achievement-notification").html('');
      $(this).html('');
      var appendImage = '<div class="achievement-image">' + options['image'] + '</div>';
      var appendPoints = '<div class="achievement-points-box"> \n'+
        '<div class="achievement-points">' + options['points'] + '</div> \n'+
        '<div class="achievement-unlocked-stats"> \n'+
          '<div class="achievement-unlocked-rank">' + options['unlockedRank'] + '</div> \n'+
        '</div> \n'+
      '</div>';
      var appendBody = '<div class="achievement-body"> \n'+
        '<div class="achievement-header">Achievement Unlocked</div> \n'+
        '<div class="achievement-title">' + options['achievementTitle'] + '</div> \n'+
      '</div>';

      // Here we define the pop-up
      // $(".achievement-notification").append(appendImage, appendPoints, appendBody);
      $(this).append(appendImage, appendPoints, appendBody);
      // notifications = $( ".achievement-notification" ).dialog({ dialogClass: 'achievement-notification-dialog',
      notifications = $(this).dialog({ dialogClass: 'achievement-notification-dialog',
        autoOpen:       false,
        show:           'fade',
        hide:           'fade',
        closeOnEscape:  false,
        draggable:      false,
        resizable:      false,
        height:         104,
        width:          500,
        position:       ['right', 'bottom'],
        closeText:      ''
      });

      // Here we open the pop-up.
      notifications.dialog( "open" );

      // Close the pop-up on click.
      // $( ".achievement-notification" ).click(function(){ notifications.dialog( "close" ); });
      $(this).click(function(){ notifications.dialog( "close" ); });

    },
    getUserAchievements : function( options ) {

      var settings = $.extend( {
        'uid'         : '',
        'serverHost'  : ''
      }, options);

      var selector = $(this).selector;

      $(selector).html('<pre>Calling... [GET ' + options['serverHost'] + '/services/achievements/get_user_achievements/' + options['uid'] + ']</pre>');

      $.ajax({
        url: options['serverHost'] + '/services/achievements/get_user_achievements/' + options['uid'],
        type: 'get',
        dataType: 'jsonp',
        contentType: 'application/json; charset=utf-8',
        success: function(data) {
          $(selector).html('<pre>' + JSON.stringify(data) + '</pre>');
        },
        error: function(data) {
          $(selector).html('<span><em>Error!</em></span>');
          console.log(data);
        }
      });

    },
    achievementTrigger : function( options ) {

      var settings = $.extend( {
        'uid'           : '',
        'achievementID' : '',
        'serverHost'    : ''
      }, options);

      var selector = $(this).selector;

      $(selector).html('<pre>Calling... [GET ' + options['serverHost'] + '/services/achievements/achievement_trigger/' + options['uid'] + '/' + options['achievementID'] + ']</pre>');

      $.ajax({
        url: options['serverHost'] + '/services/achievements/achievement_trigger/' + options['uid'] + '/' + options['achievementID'],
        type: 'get',
        dataType: 'jsonp',
        contentType: 'application/json; charset=utf-8',
        success: function(data) {
          console.log(data);
          $(".results").html('<pre>' + JSON.stringify(data) + '</pre>');

          if ($.isPlainObject(data.defaults)) {
            if (data.defaults.unlock !== undefined) {
              // This means we have unlocked an achievement.
              // Show the Pop-up with the JSON data we got from the Service.
              $('.achievement-notification').achievements('showPopUp', {
                'image'              : '<img src="' + serverHost + '/' + data.defaults.unlock.images.unlocked + '">',
                'points'             : data.defaults.unlock.points,
                'unlockedRank'       : data.defaults.unlock.rank,
                'achievementTitle'   : data.defaults.unlock.title
              });
            }
          }
        },
        error: function(data) {
          $(selector).html('<span><em>Error!</em></span>');
          console.log(data);
        }
      });

    }
  };

  $.fn.achievements = function( method ) {

    // Method calling logic
    if ( methods[method] ) {
      return methods[ method ].apply( this, Array.prototype.slice.call( arguments, 1 ));
    } else if ( typeof method === 'object' || ! method ) {
      return methods.init.apply( this, arguments );
    } else {
      $.error( 'Method ' +  method + ' does not exist on jQuery.tooltip' );
    }

  };

})( jQuery );

(function ($) {

Drupal.kaltura = Drupal.kaltura || {};

/**
 * Inserts SWF object into HTML element.
 */
Drupal.kaltura.insertSWF = function (htmlID, url, swfID, width, height, flashVars, wMode) {
  var swf = new SWFObject(url, swfID, width, height, "9", "#000000");

  if (wMode) {
    swf.addParam("wmode", wMode);
  }
  swf.addParam("flashVars", flashVars);
  swf.addParam("allowScriptAccess", "always");
  swf.addParam("allowFullScreen", "TRUE");
  swf.addParam("allowNetworking", "all");

  swf.write(htmlID);
};

/**
 * @todo: Add doc.
 */
Drupal.kaltura.changePlayer = function (vars, uiconf, change_dim) {
  var url = vars.replace_url.replace("##uiconf##", uiconf);

  var player_width = '0',
      player_height = '0';
  if (vars.site_players[uiconf] != undefined) {
    player_width = vars.site_players[uiconf].width;
    player_height = vars.site_players[uiconf].height;
  }
  if (player_width == '0') {
    player_width = vars.variable_width;
  }
  if (player_height == '0') {
    player_height = vars.variable_height;
  }

  Drupal.kaltura.insertSWF(vars.type + '_ph', url, vars.type + '_ph_player', player_width, player_height, '', 'opaque');

  if (change_dim) {
    $('input[data-kaltura-reflect-width-of-player="' + vars.id + '"]').val(player_width);
    $('input[data-kaltura-reflect-height-of-player="' + vars.id + '"]').val(player_height);
  }
};

  Drupal.behaviors.kaltura = {
    attach: function(context, settings) {
      settings.kaltura = settings.kaltura || {};

      $('.remove_media', context).click(function () {
        $(this).parents('.kaltura-thumb-wrap').nextAll().children('input:hidden').val('');
        $(this).parents('.kaltura-thumb-wrap').siblings('a').remove();
        $(this).parents('.kaltura-thumb-wrap').html('');
        });

      // Insert SWF objects into HTML.
      settings.kaltura.insertSWF = settings.kaltura.insertSWF || {};
      $.each(settings.kaltura.insertSWF, function (id, vars) {
        $('#' + id, context).once('kaltura-insert-swf', function () {
          Drupal.kaltura.insertSWF(id, vars.url, vars.swfID, vars.width, vars.height, vars.flashVars, vars.wMode);
        });
      });

      // Set and change player.
      settings.kaltura.changePlayer = settings.kaltura.changePlayer || {};
      $.each(settings.kaltura.changePlayer, function (id, vars) {
        vars.id = id;

        $('#' + id, context).once('kaltura-entry-widget', function () {
          Drupal.kaltura.changePlayer(vars, vars.saved_player);

          $(this).change(function () {
            Drupal.kaltura.changePlayer(vars, $(this).val(), 1);
          });
        });
      });
    }
  };
    Drupal.behaviors.kaltura_add = {
      attach: function (context, settings) {
        var mediaTypes = {};
        mediaTypes.Video = 1;
        mediaTypes.Image = 2;
        mediaTypes.Audio = 5;
        $('.kentry_add', context).click(function () {
          var field_name = $(this).attr('name');
          var entry_id = $(this).attr('id');
          var media_type = $(this).attr('rel');
          var mediaTypeId = mediaTypes[media_type];
          var src = $(this).parent().prevAll('.views-field-kaltura-thumbnail-url').find('img').attr('src');
          var t = '<div class="title">Added ' + media_type + ' </div><div class="kaltura_field_thumb"><img src="' + src + '"/><br/> <input type="button" title="remove item" class="remove_media" /></div>';
          var mtselect = "#" + field_name + "-media-type input";
          var etselect = "#" + field_name + "-entryid input";
          var thumb_select = "#" + field_name + "-thumb-wrap";
          $(etselect).val(entry_id);
          $(mtselect).val(mediaTypeId);
          $(thumb_select).html(t);
          Drupal.attachBehaviors();
          $('.close').triggerHandler('click');
          }
          );
          /*
           *$('#tab-kaltura_browse a').click(function () {
           *  $(".close").trigger("click");}
           *);
           */
        }
      };
      Drupal.behaviors.kaltura_roate = {
      attach: function (context, settings) {
      $('img.k-rotate').hover(function () {
        KalturaThumbRotator.start(this);},
        function () {
          KalturaThumbRotator.end(this);}
        );}
      };

      Drupal.behaviors.kaltura_prev_roate = {
      attach: function (context, settings) {
      $('.thumb-with-prev').hover(function () {
        var prev = $(this).children('img.k-preview');
        prev.show();
        KalturaThumbRotator.start(prev[0]);
        },
        function () {
          var prev = $(this).children('img.k-preview');
          prev.hide();
          KalturaThumbRotator.end(prev[0]);}
        );}
        };
          function tog (val) {
            switch (val) {
              case 'both':
                $('.details').show();
                $('.views-field-kaltura-thumbnail-url').show();
                break;

              case 'thumbs':
                $('.details').hide();
                $('.views-field-kaltura-thumbnail-url').show();
                break;

              case 'details':
                $('.details').show();
                $('.views-field-kaltura-thumbnail-url').hide();
                break;
            }
          }

      Drupal.behaviors.kaltura_entries = {
        attach: function (context, settings) {
          var current = $('#edit-view-opts').val();
          tog(current);
          $('#edit-view-opts').change(function () {
            var val = this.value;
            tog(val);
          });
        }
      };
        /*
         *Drupal.behaviors.kaltura_search = {
         *  attach: function (context, settings) {
         *      var edit = $('.view-kaltura-existing #edit-kaltura-text, .view-kaltura-list-entries #edit-kaltura-text');
         *      edit.val('Search');
         *      edit.focus( function () {
         *        if ($(this).val() === 'Search') {
         *          $(this).val('');
         *        }
         *        });
         *      edit.blur( function () {
         *        if ($(this).val() === '') {
         *            $(this).val('Search');
         *          }
         *        });
         *    }
         *  };
         */

  // Hide these in a ready to ensure that Drupal.ajax is set up first.
  $(function() {
    if (Drupal.ajax != undefined) {
      Drupal.ajax.prototype.commands.kalturaCallFunction = function(ajax, data, status) {
        var fn = window[data['function']];
        fn.apply(null, data.arguments);
      };
    }
  });
})(jQuery);
